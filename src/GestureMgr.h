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
    gesture_window.SendMessageW(WM_USER_UPDATE);
}

void OnGestureEnd(std::wstring command)
{
    gesture_window.SendMessageW(WM_USER_END, (WPARAM)command.c_str());
}

// 识别是否启用手势
class GestureMgr
{
public:
    void Init()
    {
        running_ = false;
        recognition_ = false;
        ignore_mouse_event = false;
    }

    //鼠标右键按下
    bool OnRButtonDown(MSLLHOOKSTRUCT *pmouse)
    {
        if(!ignore_mouse_event && Config::GetInt(L"Basic", L"MouseGesture") && !Config::IsExclude(L"MouseGestureExclude", ::WindowFromPoint(pmouse->pt)) )
        {
            running_ = true;
            init = pmouse->pt;
            current_window = GetTopWnd(::WindowFromPoint(pmouse->pt));
            gesture_recognition.init(pmouse->pt.x, pmouse->pt.y);
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
            else
            {
                ignore_mouse_event = true;
                SendOneMouse(MOUSEEVENTF_RIGHTDOWN);
                SendOneMouse(MOUSEEVENTF_RIGHTUP);
            }
            running_ = false;
            recognition_ = false;
            return true;
        }

        ignore_mouse_event = false;
        return false;
    }

    //鼠标移动
    void OnMouseMove(MSLLHOOKSTRUCT *pmouse)
    {
        if(running_)
        {
            if(recognition_)
            {
                gesture_recognition.move(pmouse->pt.x, pmouse->pt.y);
                OnGestureMove(pmouse->pt.x, pmouse->pt.y);
            }
            else
            {
                // 尚未开始识别
                if(GestureRecognition::GetDistance(init, pmouse->pt)>10)
                {
                    OnGestureBegin();
                    recognition_ = true;
                }
            }
        }
    }
private:
    bool running_;
    bool recognition_;
    bool ignore_mouse_event;
    POINT init;
} gesture_mgr;