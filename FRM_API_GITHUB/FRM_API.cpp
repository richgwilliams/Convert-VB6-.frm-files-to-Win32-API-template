#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

struct Control {std::string type, name; int left = 0, top = 0, width = 0, height = 0;};


int main() {

//program to convert graphics of VB6 Form into C++ template using Win32 API functions to include the GUI Window and controls
size_t formPtr = 0, controlPtr = 0;
int clientWidth = 0, clientHeight = 0, top = 0, left = 0, width = 0, height = 0;

std::string frm_filename = "FRM_API.frm";			    //input file
std::string csv_filename = "FRM_API.csv";			    //output csv file
std::string api_filename = "FRM_API.api";			    //final api output file

std::ifstream frm_file(frm_filename, std::ios::binary); 		// Read in binary mode
if (!frm_file) {std::cerr << "Error opening input file: " << frm_filename << std::endl; return 1;} //error opening input file
std::string frm_content((std::istreambuf_iterator<char>(frm_file)), (std::istreambuf_iterator<char>()));
frm_file.close();

//get the Form and parse the required properties to csv file
std::ofstream csv_out(csv_filename);
if (!csv_out) {std::cerr << "Error creating output file: " << csv_filename << std::endl; return 1;} //error creating output file
csv_out << "Type" << "," << "Name" << "," << "Left" << "," << "Top" << "," << "Width" << "," << "Height" << "\n";

formPtr = frm_content.find("Begin VB.Form", formPtr); if (formPtr == std::string::npos) { return 1;} //error finding VB.Form

size_t typeLast = frm_content.find(" ", formPtr + 9);
std::string formType = frm_content.substr(formPtr + 9, typeLast - (formPtr + 9));
size_t nameLast = frm_content.find_first_of("\r\n", typeLast + 1);
std::string formName = frm_content.substr(typeLast + 1, nameLast - (typeLast + 1));

size_t clientWidthPos = frm_content.find("ClientWidth", formPtr);
if (clientWidthPos != std::string::npos) {std::stringstream ss(frm_content.substr(clientWidthPos + 20)); ss >> clientWidth;}

size_t clientHeightPos = frm_content.find("ClientHeight", formPtr);
if (clientHeightPos != std::string::npos) {std::stringstream ss(frm_content.substr(clientHeightPos + 20)); ss >> clientHeight;}

left = 0; top = 0;  //the vb6 .frm file gives us no information about placement of main cliWindow so set left and top to zero
csv_out << formType << "," << formName << "," << left << "," << top << "," << clientWidth << "," << clientHeight << "\n";

controlPtr = formPtr + 9;   //continue using controlPtr updating it to parse each control in turn


//get each control into a block and parse the required properties to csv file until all are done
while (true) {
    controlPtr = frm_content.find("Begin VB.", controlPtr); if (controlPtr == std::string::npos) {break;}
    size_t typeLast = frm_content.find(" ", controlPtr + 9);
    std::string controlType = frm_content.substr(controlPtr + 9, typeLast - (controlPtr + 9));
    size_t nameLast = frm_content.find_first_of("\r\n", typeLast + 1);
    std::string controlName = frm_content.substr(typeLast + 1, nameLast - (typeLast + 1));
    size_t controlLast = frm_content.find("End\r\n", controlPtr);
    std::string controlBlock = frm_content.substr(controlPtr, controlLast - controlPtr + 3);
    controlPtr = controlLast;   //update controlPtr to look for the next control block

   
    if (controlType == "TextBox" || controlType == "PictureBox" || controlType == "CommandButton" ||
                        controlType == "ListView" || controlType == "Label" || controlType == "Shape") {
        size_t leftPos = controlBlock.find(" Left ", 0);
        if (leftPos != std::string::npos) {std::stringstream ss(controlBlock.substr(leftPos + 21)); ss >> left; }
        size_t topPos = controlBlock.find(" Top ", 0);
        if (topPos != std::string::npos) {std::stringstream ss(controlBlock.substr(topPos + 21)); ss >> top; }
        size_t widthPos = controlBlock.find(" Width ");
        if (widthPos != std::string::npos) {std::stringstream ss(controlBlock.substr(widthPos + 21)); ss >> width; }
        size_t heightPos = controlBlock.find(" Height ");
        if (heightPos != std::string::npos) {std::stringstream ss(controlBlock.substr(heightPos + 21)); ss >> height; }
        csv_out << controlType << "," << controlName << "," << left << "," << top << "," << width << "," << height << "\n";
    }
} 
csv_out.close();


//each csv line represents a form or a control to be converted into an array
std::vector<Control> controls; std::string line; std::ifstream csv_in(csv_filename);
if (!csv_in) { std::cerr << "Error opening input file: " << csv_filename << std::endl; return 1; }
std::getline(csv_in, line);             //dispose of header line
while (std::getline(csv_in, line)) {
    Control ctrl; std::string value; std::stringstream ss(line);
    std::getline(ss, ctrl.type, ',');
    std::getline(ss, ctrl.name, ',');
    std::getline(ss, value, ','); ctrl.left = std::stoi(value) / 10;
    std::getline(ss, value, ','); ctrl.top = std::stoi(value) / 10;
    std::getline(ss, value, ','); ctrl.width = std::stoi(value) / 10;
    std::getline(ss, value, ','); ctrl.height = std::stoi(value) / 10;
    controls.push_back(ctrl);
}
csv_in.close();


//create the new FRM_API.api file, populating the Win32 API functions with the correct values and variables
std::ofstream api_out(api_filename);
if (!api_out) { std::cerr << "Error creating output file: " << api_filename << std::endl; return 1; } //error creating output file

api_out << "LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam) {\n";
api_out << "    switch (umsg) {\n        case WM_DESTROY: PostQuitMessage(0); return 0;\n";
api_out << "        //add other cases here\n    }\n    return DefWindowProc(hwnd, umsg, wParam, lParam);\n}\n\n";
api_out << "int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {\n";
api_out << "    WNDCLASSW wc = {};\n";
api_out << "    wc.lpfnWndProc = WndProc;\n";
api_out << "    wc.hInstance = hInstance;\n";
api_out << "    wc.lpszClassName = L\"GuiWindow\";\n";
api_out << "    if (!RegisterClassW(&wc)) return 0;\n\n";

//adjust main window dimensions to get the required client width and client height, the main form is populated differently 
int mainWidth = 0, mainHeight = 0; for (const auto& ctrl : controls) {if (ctrl.type == "Form") {mainWidth = ctrl.width; mainHeight = ctrl.height; }}
api_out << "    RECT adjR = {0, 0," << mainWidth << ", " << mainHeight << "};\n";
api_out << "    AdjustWindowRectEx(&adjR, WS_OVERLAPPEDWINDOW, FALSE, 0);\n";
api_out << "    int finalW = adjR.right - adjR.left; int finalH = adjR.bottom - adjR.top;\n";
api_out << "    HWND hwnd = CreateWindowExW(0, L\"GuiWindow\", L\"Converted Window\", WS_OVERLAPPEDWINDOW, 0, 0, finalW, finalH, NULL, NULL, hInstance, NULL);\n";
api_out << "    if (!hwnd) return 0; ShowWindow(hwnd, nCmdShow); UpdateWindow(hwnd);\n\n";

//populate controls depending upon control type
std::string controlClass = "", controlSwitches = "", controlHmenu = "";

for (const auto& ctrl : controls) {
    if (ctrl.type != "Form") {
        if (ctrl.type == "ListView") {controlClass = "WC_LISTVIEW";
            controlSwitches = "WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS | WS_TABSTOP"; controlHmenu = "(HMENU)IDC_LISTVIEW_";
        }
        if (ctrl.type == "CommandButton") { controlClass = "WC_BUTTON"; controlSwitches = "WS_CHILD | WS_VISIBLE"; controlHmenu = "(HMENU)IDC_BUTTON_"; }
        if (ctrl.type == "TextBox") { controlClass = "WC_EDIT"; controlSwitches = "WS_CHILD | WS_VISIBLE"; controlHmenu = "(HMENU)IDC_TEXT_"; }
        if (ctrl.type == "Label") { controlClass = "WC_STATIC"; controlSwitches = "WS_CHILD | WS_VISIBLE"; controlHmenu = "(HMENU)IDC_LABEL_"; }
        if (ctrl.type == "Shape") { controlClass = "WC_STATIC"; controlSwitches = "WS_CHILD | WS_VISIBLE"; controlHmenu = "(HMENU)IDC_SHAPE_"; }

        api_out << "    HWND hwnd" << ctrl.name << " = CreateWindowExW(0," << controlClass << ", " "L\"\", " << controlSwitches
            << ", " << ctrl.left << ", " << ctrl.top << ", " << ctrl.width << ", " << ctrl.height << ", "
            "hwnd, " << controlHmenu << ctrl.name << ", hInstance, NULL);\n";
    }
}

//add the message loop
api_out << "\n    MSG msg = {};\n";
api_out << "    while (GetMessage(&msg, NULL, 0, 0)) {TranslateMessage(&msg); DispatchMessage(&msg); }\n";
api_out << "    return (int)msg.wParam;\n}";

api_out.close(); return 0;
}





























































