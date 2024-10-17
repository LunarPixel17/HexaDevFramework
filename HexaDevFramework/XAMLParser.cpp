#include "XAMLParser.h"
#include <fstream>
#include <iostream>
#include <sstream>

static std::vector<UIElement> elements;

void ParseXAML(const std::wstring& filename) {
    std::wifstream file(filename);
    std::wstring line;

    while (std::getline(file, line)) {
        UIElement element;
        if (line.find(L"<Image") != std::wstring::npos) {
            element.type = L"Image";
            swscanf_s(line.c_str(), L"<Image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" imagePath=\"%ls\"/>",
                &element.x, &element.y, &element.width, &element.height, &element.imagePath[0]);
            elements.push_back(element);
        }
        else if (line.find(L"<Button") != std::wstring::npos) {
            element.type = L"Button";
            swscanf_s(line.c_str(), L"<Button x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" text=\"%ls\"/>",
                &element.x, &element.y, &element.width, &element.height, &element.text[0]);
            elements.push_back(element);
        }
    }
}

const std::vector<UIElement>& GetUIElements() {
    return elements;
}
