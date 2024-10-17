#ifndef XAMLPARSER_H
#define XAMLPARSER_H

#include <string>
#include <vector>

// Define a structure for UI elements
struct UIElement {
    std::wstring type;
    int x, y, width, height;
    std::wstring imagePath;  // For images
    std::wstring text;       // For button text
};

// Function to parse the XAML-like file
void ParseXAML(const std::wstring& filename);
const std::vector<UIElement>& GetUIElements();

#endif // XAMLPARSER_H
