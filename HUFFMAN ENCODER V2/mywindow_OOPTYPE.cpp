#include <windows.h>
//#include <shobjidl.h>
#include <stdio.h>
#include <fstream>
//#include <string.h>
#include <HEdriver.h>
//#include "huffmanEncoder_v9.cpp"

#define ID_BUTTON_DECOMPRESS 0x8801
#define ID_BUTTON_COMPRESS 0x8802
#define ID_BUTTON_OPEN 0x8803
#define ID_STATICTEXT_HUFFMANENCODER 0x8803

//use strcat_s instead of strcat s is for secure



class MainWindow
{
public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    PCWSTR  ClassName() const { return classname; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    BOOL createMyLayout();
    //BOOL driver();
    HWND Window() const { return main_hwnd; }

    void register_class(LPCWSTR str)
    {
        WNDCLASS wc={0};

        wc.lpfnWndProc   = WindowProc;
        wc.hInstance     = GetModuleHandle(NULL);
        wc.lpszClassName = str ;

        RegisterClass(&wc);
    }

    MainWindow(LPCWSTR str) : classname(str), main_hwnd(NULL) { }

protected:
    HWND main_hwnd;
    HWND hwndButton_decompress;
    HWND hwndButton_compress;
    HWND hwndButton_open;
    HWND hwndStaticText_huffmanencoder;
    const LPCWSTR classname;
};

//WindowProc gives hwnd made by createWindowEx ???
LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        MainWindow *pThis ;

        if (uMsg == WM_NCCREATE)
        {
        

            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (MainWindow*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
            pThis->main_hwnd=hwnd;
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        //return DefWindowProc(hwnd, uMsg, wParam, lParam);
        else
        {
            pThis = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }

    }

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

    case WM_NCCREATE:
            MessageBox(NULL, L"aa", L"File Path", MB_OK);
            return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(main_hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            EndPaint(main_hwnd, &ps);
        }
        return 0;

    case WM_COMMAND:
        {
              if( HIWORD(wParam) == BN_CLICKED  ) 
              {
                WORD hmenu=LOWORD(wParam);//MessageBox(NULL, (char*)hmenu, L"File Path", MB_OK);
                switch(hmenu)
                {
                    case ID_BUTTON_DECOMPRESS:
                        { ;}//MessageBox(NULL, L"l", L"File Path", MB_OK);}
                        return 0;

                    case ID_BUTTON_COMPRESS:
                    {
                        driver_encodefile();
                        //MessageBox(NULL, L"ll", L"File Path", MB_OK);
                    }   
                    return 0;
                }
              }
        }
        return 0;

    default:
        return DefWindowProc(main_hwnd, uMsg, wParam, lParam);
    }
}


BOOL MainWindow :: createMyLayout()
{
    register_class(classname);

    int screenwidth=GetSystemMetrics(0);

       main_hwnd = CreateWindowEx(
        (DWORD)0,
        classname,
        L"HUFFMAN ENCODER",
        WS_OVERLAPPEDWINDOW,
        (int)(screenwidth*(0.15)),(int)(screenwidth*(0.15)),(int)(screenwidth*(0.3)),(int)(screenwidth*(0.3)),//CW_USEDEFAULT,CW_USEDEFAULT,
        (HWND)0,
        (HMENU)0,
        GetModuleHandle(NULL),
        this
        ) ;

    /*HWND*/ hwndButton_compress = CreateWindow( 
    L"BUTTON",  // Predefined class; Unicode assumed 
    L"COMPRESS",      // Button text 
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_TEXT | BS_CENTER,  // Styles 
    50,         // x position 
    200,         // y position 
    100,        // Button width
    30,        // Button height
    main_hwnd,     // Parent window
    (HMENU)ID_BUTTON_COMPRESS,       // ID menu.
    (HINSTANCE)GetWindowLong(main_hwnd, GWL_HINSTANCE), 
    NULL);      // Pointer not needed.

    /*HWND*/ hwndButton_decompress = CreateWindow( 
    L"BUTTON",  // Predefined class; Unicode assumed 
    L"DECOMPRESS",      // Button text 
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_TEXT | BS_CENTER,  // Styles 
    250,         // x position 
    200,         // y position 
    120,        // Button width
    30,        // Button height
    main_hwnd,     // Parent window
    (HMENU)ID_BUTTON_DECOMPRESS,       // ID menu.
    (HINSTANCE)GetWindowLong(main_hwnd, GWL_HINSTANCE), 
    NULL);      // Pointer not needed.

    hwndStaticText_huffmanencoder=CreateWindow(
        L"STATIC",
        L"HUFFMAN COMPRESSOR",
        /*SS_GRAYRECT |*/ SS_CENTER | WS_VISIBLE | WS_CHILD,
        125,
        100,
        120,
        70,
        main_hwnd,
        (HMENU)ID_STATICTEXT_HUFFMANENCODER,
        (HINSTANCE)GetWindowLong(main_hwnd, GWL_HINSTANCE),
        NULL
        );

/*
WHILE USING CreateWindow or CreateWindowEx lpclassname should 
be a predefined class when anything other than the main window is 
being created otherwise we will get "pthis is NULL" error
*/  
    /*hwndtbox_title=CreateWindow(
        L"STATIC",
        L"EASY COMPRESS",
        WS_VISIBLE | WS_CHILD,
        10,
        10,
        20,
        20,
        main_hwnd,
        NULL,
        (HINSTANCE)GetWindowLong(main_hwnd, GWL_HINSTANCE),
        NULL
        );*/

    // hwndtbox_title=CreateWindowEx(
    //     WS_EX_LEFT,
    //     L"STATIC",
    //     L"EASY COMPRESS",
    //     WS_VISIBLE | WS_CHILD | SS_CENTER,
    //     100,30,200,20,
    //     main_hwnd,
    //     NULL,
    //     (HINSTANCE)GetWindowLong(main_hwnd, GWL_HINSTANCE),
    //     NULL
    //     );

    // hwndButton_open = CreateWindow( 
    // L"BUTTON",  // Predefined class; Unicode assumed 
    // L"OPEN",      // Button text 
    // WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_TEXT | BS_CENTER,  // Styles 
    // 300,         // x position 
    // 200,         // y position 
    // 90,        // Button width
    // 30,        // Button height
    // main_hwnd,     // Parent window
    // (HMENU)ID_BUTTON_OPEN,       // ID menu.
    // (HINSTANCE)GetWindowLong(main_hwnd, GWL_HINSTANCE), 
    // NULL);

    return TRUE;
}

//BOOL MainWindow::driver()
// {
//     char* fname=NULL;
//     if(openD(&fname))
//         return 0;//throw error
//     if(fname!=NULL)
//         MessageBox(NULL, L"Should not be called", L"File Path", MB_OK);
//     //ofstream fnameout("F:/thinkbinary foundation/huffman encoder/fname.txt");
//     //fnameout.write(fname,strlen(fname));
//     //fnameout.close();
//     //huffmanEncoder objCompress(/*"F:/thinkbinary foundation/huffman encoder/stf.txt"*/ fname);
//     //objCompress.driver();
//     return 1;
// }



// int WINAPI openD(char** pfname)
// {
//     HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
//         COINIT_DISABLE_OLE1DDE);
//     if (SUCCEEDED(hr))
//     {
//         IFileOpenDialog *pFileOpen;

//         // Create the FileOpenDialog object.
//         hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, 
//                 IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

//         if (SUCCEEDED(hr))
//         {
//             // Show the Open dialog box.
//             hr = pFileOpen->Show(NULL);

//             // Get the file name from the dialog box.
//             if (SUCCEEDED(hr))
//             {
//                 IShellItem *pItem;
//                 hr = pFileOpen->GetResult(&pItem);
//                 if (SUCCEEDED(hr))
//                 {
//                     PWSTR pszFilePath;
//                     hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

//                     // Display the file name to the user.
//                     if (SUCCEEDED(hr))
//                     {
//                         MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
//                         //use pszFilePath to seave filename using widechartomultibyte
//                         int szreq=WideCharToMultiByte
//                         (
//                         CP_UTF8,
//                         0,//Dont use this -> WC_NO_BEST_FIT_CHARS,
//                         pszFilePath,
//                         -1,/*wcslen(pszFilePath),//sizeof(pszFilePath),//check if pszFilePath is null terminated
//                         /*NULL,
//                         0,
//                         NULL,
//                         NULL
//                         );
//                         *pfname=(char*)malloc(szreq);
//                         WideCharToMultiByte
//                         (
//                         CP_UTF8,
//                         0,//WC_NO_BEST_FIT_CHARS,
//                         pszFilePath,
//                         -1,//wcslen(pszFilePath),//sizeof(pszFilePath),//check if pszFilePath is null terminated
//                         *pfname,
//                         szreq,
//                         NULL,
//                         NULL
//                         );
//                         strreplace(*pfname);*/
//                         WSTR_to_CharStr(pszFilePath,pfname,1);
//                         MessageBox(NULL, L"mm", L"File Path", MB_OK);
//                         //CoTaskMemFree(pszFilePath);
//                     }
//                     pItem->Release();
//                 }
//             }
//             pFileOpen->Release();
//         }
//         CoUninitialize();
//     }
//     return 0;
// }



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    MainWindow win(L"hurray");
    //driver_decodefile();
    win.createMyLayout();
    ShowWindow(win.Window(), nCmdShow);
    //win.driver();
    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
