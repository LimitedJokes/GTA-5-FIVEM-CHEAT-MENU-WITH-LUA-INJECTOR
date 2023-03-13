#pragma once

#include "imgui/imgui_internal.h"
#include "imgui/imgui.h"

#include <Windows.h>
#include <vector>
#include <iostream>

#include <string>

namespace Demo
{
    bool showDemoWindow = true;
    bool testCheckBox = true;

    int selectedListBoxItem = 0;
    std::vector<const char*> listBoxItems = { "Item 1", "Item 2", "Item 3" };

    int selectedComboItem = 0;
    std::vector<const char*> comboBoxItems = { "Item 1", "Item 2", "Item 3" };

    char inputTextBuffer[256] = { 0 };
    char inputTextWithHintBuffer[256] = { 0 };
    char passwordInputTextBuffer[256] = { 0 };
    char inputTextMultiLineBuffer[1024] = { 0 };

    void renderDemoWindow()
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    void renderCheckBox()
    {
        ImGui::Checkbox("Test Checkbox", &testCheckBox);
    }

    void renderListBox()
    {
        ImGui::ListBox("List Box", &selectedListBoxItem, listBoxItems.data(), listBoxItems.size());
    }

    void renderComboBox()
    {
        ImGui::Combo("Combo Box", &selectedComboItem, comboBoxItems.data(), comboBoxItems.size());
    }

    void renderInputText()
    {
        ImGui::InputText("Input Text", inputTextBuffer, IM_ARRAYSIZE(inputTextBuffer));
    }

    void renderInputTextWithHint()
    {
        ImGui::InputTextWithHint("Input Text with Hint", "Hint Text", inputTextWithHintBuffer, IM_ARRAYSIZE(inputTextWithHintBuffer));
    }

    void renderPasswordInputText()
    {
        ImGui::InputText("Password", passwordInputTextBuffer, IM_ARRAYSIZE(passwordInputTextBuffer), ImGuiInputTextFlags_Password);
    }

    void renderInputTextMultiLine()
    {
        ImGui::InputTextMultiline("Input Text Multiline", inputTextMultiLineBuffer, IM_ARRAYSIZE(inputTextMultiLineBuffer), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));
    }
}


void Render()
{
    ImGui::Separator();
    ImGui::BeginColumns("##Columns", 2); // Creating 2 columns and providing an ID for the columns
    ImGui::SetColumnOffset(1, ImGui::GetWindowWidth() / 2); // comment this out if you want to be able to move the column around :)
    ImGui::Checkbox("TestBox", &TestCheckBox);

    // Use ImGui::PushID() and ImGui::PopID() to create unique IDs for each element
    ImGui::PushID("ListBoxDemo");
    ImGui::PushItemWidth(ImGui::GetWindowWidth() / 3);  //ImGui::PushItemWidth(float item_width) just expands the item width
    ImGui::ListBox("##ListBox", &SelectedListBoxItem, ListBoxArrayDemo, IM_ARRAYSIZE(ListBoxArrayDemo));
    ImGui::PopItemWidth(); //If you don't pop the items width after you call the 'ImGui::PushItemWidth(float item_width)' function, you will get an error
    ImGui::PopID();

    ImGui::Button("My Button"); // Just a button with no custom size etc, it just defaults
    ImGui::Button("My custom sized button", ImVec2(200, 30)); // We added a 'ImVec2' which is just x & y coords. The first param of ImVec2 is an x value(item width) and the second is a y value(item height).

    // Use ImGui::BeginGroup() and ImGui::EndGroup() to group related elements together
    ImGui::BeginGroup();
    if (ImGui::Button("If ImGui::Button", ImVec2(200, 30)))
    {
        // If the button is pressed then... do your code.
    }
    ImGui::EndGroup();

    ImGui::Text("My custom text"); // This only takes 1 param, just some text lol

    ImGui::TextColored(ImColor(255, 0, 0, 255), "My Custom Red Color Text!"); // Colored text, just takes 2 params, A ImVec4 color & text.

    // Use ImGui::BeginTooltip() and ImGui::EndTooltip() to create a tooltip
    ImGui::Text("Hover me!");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("This is a custom tool tip!");
        ImGui::EndTooltip();
    }

    ImGui::PushID("ComboDemo");
    ImGui::PushItemWidth(ImGui::GetWindowWidth() / 3);
    ImGui::Combo("##DemoCombo", &SelectedComboItem, ComboBoxArrayDemo, 
		ImGui::SameLine(); // puts the button called 'Output' beside the input text
		if (ImGui::Button("Output"))
		{
			MessageBoxA(NULL, InputTextDemo, "Custom TextBox Lol.", NULL);
		}
		ImGui::InputTextWithHint("##InputTextWithHintDemo", "Type stuff here...", InputTextWithHintDemo, IM_ARRAYSIZE(InputTextWithHintDemo));
		ImGui::InputTextWithHint("##PasswordInputTextDemo", "Enter your secret password." , PasswordInputTextDemo, IM_ARRAYSIZE(PasswordInputTextDemo), ImGuiInputTextFlags_Password); //replaces your text you type with *
		ImGui::InputTextMultiline("##InputTextMultiLineDemo", InputTextMultiLineDemo, IM_ARRAYSIZE(InputTextMultiLineDemo), ImVec2(ImGui::GetWindowWidth() / 2, 80));

	}
}
