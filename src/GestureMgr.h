#include <mutex>
#include <chrono>
#include <thread>
#include <condition_variable>

//OnGestureBegin跨线程调用
void OnGestureBegin()
{
    gesture_window.SendMessageW(WM_USER_SHOW);
}

void OnGestureMove(int x, int y)
{
    gesture_window.Update();
}

void OnGestureEnd(std::wstring command)
{
    gesture_window.End(command);
}

// 识别是否启用手势
class GestureMgr
{
public:
    void Init()
    {
        running_ = false;
        recognition_ = false;
        abort_ = false;
        ignore_mouse_event = false;
        manager_thread_ = new std::thread(&GestureMgr::Thread, this);
    }

    void Exit()
    {
        abort_ = true;
        mouse_down_event_.notify_one();
        mouse_up_event_.notify_one();
        manager_thread_->join();
        delete manager_thread_;
    }

    //鼠标右键按下
    bool OnRButtonDown(MSLLHOOKSTRUCT *pmouse)
    {
        if(!ignore_mouse_event && Config::GetInt(L"Basic", L"MouseGesture") && !Config::IsExclude(L"MouseGestureExclude", ::WindowFromPoint(pmouse->pt)) )
        {
            running_ = true;
            current_window = GetTopWnd(::WindowFromPoint(pmouse->pt));
            gesture_recognition.init(pmouse->pt.x, pmouse->pt.y);
            mouse_down_event_.notify_one();
            return true;
        }

        return false;
    }

    //鼠标右键弹起
    bool OnRButtonUp(MSLLHOOKSTRUCT *pmouse)
    {
        if(!ignore_mouse_event && running_)
        {
            if(recognition_)
            {
                std::wstring command = gesture_recognition.result();
                
                OnGestureEnd(command);
            }
            mouse_up_event_.notify_one();
            running_ = false;
            recognition_ = false;
            return true;
        }

        ignore_mouse_event = false;
        return false;
    }

    //鼠标移动
    void Move(int x, int y)
    {
        gesture_recognition.move(x, y);
        OnGestureMove(x, y);
    }

    //是否进入识别状态
    bool IsRunning()
    {
        return running_;
    }
private:
    void Thread()
    {
        while(!abort_)
        {
            std::unique_lock<std::mutex> event_lock(event_mutex_);

            mouse_down_event_.wait(event_lock);
            if(abort_) break;

            auto now = std::chrono::high_resolution_clock::now();

            std::cv_status result = mouse_up_event_.wait_for(event_lock, std::chrono::milliseconds(255));
            //使用返回值判断std::cv_status::timeout似乎不准，所以这里多加5ms
            if(abort_) break;

            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - now).count();
            if(diff > 250)
            {
                recognition_ = true;

                OnGestureBegin();
            }
            else
            {
                ignore_mouse_event = true;
                SendOneMouse(MOUSEEVENTF_RIGHTDOWN);
                SendOneMouse(MOUSEEVENTF_RIGHTUP);
            }
        }
    }
private:
    bool running_;
    bool recognition_;
    bool ignore_mouse_event;
    bool abort_;

    std::thread *manager_thread_;

    std::mutex event_mutex_;
    std::condition_variable mouse_down_event_;
    std::condition_variable mouse_up_event_;
} gesture_mgr;