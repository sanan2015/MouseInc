class AboutWindow :
    public ATL::CWindowImpl<AboutWindow>
{
public:
    DECLARE_WND_CLASS(app_name)

    BEGIN_MSG_MAP(AboutWindow)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_ACTIVATE(OnActivate)

        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
    END_MSG_MAP()

private:
    LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct)
    {
        SetIcon(::LoadIcon(::GetModuleHandle(NULL), L"APPICON_WHITE"));

        ResizeClient(350, 198);

        InitFont();
        CenterWindow();

        ImageFromIDResource(L"donate", donate);

        int x = 168;
        int y = 10;
        int width = 350 - x;

        RECT pos = { x, y, x + width, y + hight };
        text1.Create(m_hWnd, pos, L"版本：2.1", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL);
        text1.SetFont(font);

        y += hight;
        pos = { x, y, x + width, y + hight };
        text2.Create(m_hWnd, pos, L"时间：2015年12月31日", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL);
        text2.SetFont(font);

        y += hight;
        pos = { x, y, x + width, y + hight };
        text3.Create(m_hWnd, pos, L"版权：shuax", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL);
        text3.SetFont(font);

        y += hight;
        pos = { x, y, x + 36, y + hight };
        text4.Create(m_hWnd, pos, L"感谢：", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL);
        text4.SetFont(font);

        pos = { x + 36, y, x + 76, y + hight };
        link2.Create(m_hWnd, pos, L"ONEO", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL);
        link2.SetHyperLink(L"http://oneo.me/");

        pos = { x + 76, y, x + width, y + hight };
        text5.Create(m_hWnd, pos, L"提供美术支持", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL);
        text5.SetFont(font);

        y += hight*2;
        pos = { x + 0, y, x + width, y + hight };
        link.Create(m_hWnd, pos, L"访问官方网站", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL);
        link.SetHyperLink(L"http://www.shuax.com/?from=mouseinc");

        return 0;
    }

    void OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther)
    {
        if (nState == WA_INACTIVE)
        {
            SetIcon(::LoadIcon(::GetModuleHandle(NULL), L"APPICON"), false);
        }
        else
        {
            SetIcon(::LoadIcon(::GetModuleHandle(NULL), L"APPICON_WHITE"), false);
        }
    }

    void OnPaint(HDC dc)
    {
        PAINTSTRUCT  ps;
        CDC hdc = BeginPaint(&ps);
        Graphics graphics(hdc);

        graphics.DrawImage(donate, 0, 0);

        EndPaint(&ps);
    }

    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        return 0;
    }

    LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        ShowWindow(SW_HIDE);
        return 0;
    }
private:
    LRESULT OnCtlColorStatic(UINT, WPARAM, LPARAM, BOOL&)
    {
        return (LRESULT)::GetStockObject(WHITE_BRUSH);
    }
    void InitFont()
    {
        LOGFONT lf = { 0 };
        NONCLIENTMETRICS ncm = { sizeof(NONCLIENTMETRICS) };
        SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, false);
        lf = ncm.lfMessageFont;
        hight = -lf.lfHeight * 2;

        ATLVERIFY(font.CreateFontIndirect(&lf));
    }
    Image *donate;
    CStatic text1;
    CStatic text2;
    CStatic text3;
    CStatic text4;
    CStatic text5;
    CHyperLink link;
    CHyperLink link2;
    int hight;
    CFont font;
};