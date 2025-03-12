# Convert VB6 .frm Files to VS 2022 C++ Win32 API Template

This project provides a command-line tool to convert VB6 `.frm` files into Visual Studio 2022 C++ Win32 API graphical template code. Its primary purpose is to facilitate the conversion of VB6 form layout information into a starting point for modern C++ Win32 applications.

## Problem Solved

When migrating legacy VB6 applications to modern C++ within Visual Studio 2022, recreating the graphical user interface can be time-consuming and error-prone. This tool automates the process of generating a basic Win32 API template that mirrors the layout of controls defined in the VB6 `.frm` file, significantly reducing the initial effort required.

## How It Works

The converter parses the VB6 `.frm` file and extracts information about the form's layout and controls. It then generates two output files:

1.  **`.csv` File:** A comma-separated values file containing the extracted control properties, which can be easily imported into spreadsheet software like Microsoft Excel for analysis.
2.  **`.api` File (C++ Template):** A C++ source code template containing Win32 API calls to create a main window and the controls defined in the `.frm` file. This serves as a foundation for building your C++ application's UI.

## Input (FRM_API.frm)

A sample input file, `FRM_API.frm`, is included in this repository. This file represents a basic VB6 form layout. To use the tool, you should place your `.frm` file in the project's root directory and rename it `FRM_API.frm`.

## Output (CSV and API)

After running the converter, you will find two output files in the project's root directory:

* **`FRM_API.csv`:** This file contains a table of control properties, such as control type, position, size, and text. You can use this file to review the extracted information or to further process it.
* **`FRM_API.api`:** This file contains C++ source code that uses the Win32 API to create the form and controls. It is designed to be a starting point, and you will likely need to modify it to add functionality and customize the appearance of your application. Note that while the file is named `.api` to avoid confusion with existing `.cpp` files, it is indeed a C++ source file.

**The Example `FRM_API.frm` converts to:**

    // Generated Win32 API code...
    LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam) {
        switch (umsg) {
            case WM_DESTROY: PostQuitMessage(0); return 0;
            //add other cases here
        }
        return DefWindowProc(hwnd, umsg, wParam, lParam);
    }
    int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
        WNDCLASSW wc = {};
        wc.lpfnWndProc = WndProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = L"GuiWindow";
        if (!RegisterClassW(&wc)) return 0;
    
        RECT adjR = {0, 0,1911, 1137};
        AdjustWindowRectEx(&adjR, WS_OVERLAPPEDWINDOW, FALSE, 0);
        int finalW = adjR.right - adjR.left; int finalH = adjR.bottom - adjR.top;
        HWND hwnd = CreateWindowExW(0, L"GuiWindow", L"Converted Window", WS_OVERLAPPEDWINDOW, 0, 0, finalW, finalH, NULL, NULL, hInstance, NULL);
        if (!hwnd) return 0; ShowWindow(hwnd, nCmdShow); UpdateWindow(hwnd);
    
        HWND hwndcmdLogs  = CreateWindowExW(0,WC_BUTTON, L"", WS_CHILD | WS_VISIBLE, 780, 912, 100, 70, hwnd, (HMENU)IDC_BUTTON_cmdLogs , hInstance, NULL);
        HWND hwndtxtError  = CreateWindowExW(0,WC_EDIT, L"", WS_CHILD | WS_VISIBLE, 1464, 120, 433, 82, hwnd, (HMENU)IDC_TEXT_txtError , hInstance, NULL);
        HWND hwndmemConvert  = CreateWindowExW(0,WC_EDIT, L"", WS_CHILD | WS_VISIBLE, 1020, 996, 100, 34, hwnd, (HMENU)IDC_TEXT_memConvert , hInstance, NULL);
    
        MSG msg = {};
        while (GetMessage(&msg, NULL, 0, 0)) {TranslateMessage(&msg); DispatchMessage(&msg); }
        return (int)msg.wParam;
    }


**Notes:**

The program is especially useful for converting VB6 `.frm` files that represent a main window with many controls. Sometimes the relationship between VB6 screen units (twips, pixels, user etc) can be a consideration. The program divides the VB6 unit by 10. If you wish, reading the .csv file into Excel allows you to study the control dimensions and maybe make numeric alterations later on. The generated code is a template, error handling and other features will need to be edited and added as you develop your new C++ program.


**Limitations:**

* This tool provides a foundational conversion of VB6 form layouts.
* Support for VB6 controls and properties is limited.
* The generated C++ code serves as a template and requires manual adjustments for full functionality and desired aesthetics.
* This tool is intended as a starting point, empowering developers to build upon the generated code for their specific needs.


**Acknowledgments:**

The author acknowledges the assistance of the Gemini and ChatGPT large language models as co-authors, also Gemini for the refining of the README.md and improving the clarity of the project description. The author considers these Large Language Models (LLMs) essential for the understanding of Win32 API and C++ programming.

