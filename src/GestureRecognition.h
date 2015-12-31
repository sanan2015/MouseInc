TCHAR str[][4]= {_T(""),_T("↑"),_T("→"),_T("↓"),_T("←"),_T("↗"),_T("↘"),_T("↙"),_T("↖")};

//手势识别
class GestureRecognition
{
public:
    //鼠标移动
    void move(int x, int y)
    {
        points.push_back({x, y});
    }

    //开始一次识别
    void init(int x, int y)
    {
        points.clear();
        move(x, y);
    }

    std::vector<POINT> & get_points()
    {
        return points;
    }

    //手势结果
    std::wstring result()
    {
        std::wstring result;

        if(points.size()<2)
        {
            return result;
        }

        int count[9] = {0};

        POINT last_point = points[0];
        int last_result = 0;

        for (size_t i = 1; i < points.size(); ++i)
        {
            //小于4像素忽略
            if (GetDistance(points[i], last_point) < 4) continue;

            int orientation = GetOrientation(points[i], last_point);
            count[orientation]++;
            for(int j = 0; j < 9; j++)
            {
                if(count[j]>8 && last_result!=j)
                {
                    //
                    result += str[j];
                    last_result = j;
                    for(int k = 0; k < 9; k++) count[k]=0;
                }
            }

            last_point = points[i];
        }

        return result;
    }
    static int GetDistance(POINT a, POINT b)
    {
        int s = (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
        return _floor(SquareRootFloat((float)s));
    }
private:
    static float SquareRootFloat(float number)
    {
        long i;
        float x, y;
        const float f = 1.5F;
        x = number * 0.5F;
        y  = number;
        i  = * ( long * ) &y;
        i  = 0x5f375a86 - ( i >> 1 );   //注意这一行
        y  = * ( float * ) &i;
        y  = y * ( f - ( x * y * y ) );
        return number * y;
    }
    static int _floor(float f)
    {
        int a         = *(int*)(&f);
        int sign      = (a>>31);
        int mantissa  = (a&((1<<23)-1))|(1<<23);
        int exponent  = ((a&0x7fffffff)>>23)-127;
        int r         = ((unsigned int)(mantissa)<<8)>>(31-exponent);
        return ((r ^ (sign)) - sign ) &~ (exponent>>31);
    }
    int GetOrientation(POINT a, POINT b)
    {
        int orientation = 0;
        int x = a.x - b.x;
        int y = a.y - b.y;
        float delta = 2.414f;
        if (x >= 0 && y <= 0)
        {
            //第一象限
            y *= -1;
            if (x > y && delta*y < x) orientation = 2;
            if (x < y && delta*x < y) orientation = 1;
            if(orientation==0) orientation = 5;
            y *= -1;
        }
        else if (x >= 0 && y >= 0)
        {
            //第四象限
            if (x > y && delta*y < x) orientation = 2;
            if (x < y && delta*x < y) orientation = 3;
            if(orientation==0) orientation = 6;
        }
        else if (x<=0 && y>=0)
        {
            //第三象限
            x *= -1;
            if (x > y && delta*y < x) orientation = 4;
            if (x < y && delta*x < y) orientation = 3;
            if(orientation==0) orientation = 7;
            x *= -1;
        }
        else  if (x <= 0 && y <= 0)
        {
            //第二象限
            x *= -1;
            y *= -1;
            if (x > y && delta*y < x) orientation = 4;
            if (x < y && delta*x < y) orientation = 1;
            if(orientation==0) orientation = 8;
        }
        return orientation;
    }
private:
    std::vector<POINT> points;
} gesture_recognition;