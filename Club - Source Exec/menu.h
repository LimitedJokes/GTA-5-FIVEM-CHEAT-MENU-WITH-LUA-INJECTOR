#pragma once

#include "font_awesome.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui.h"

#include "default.h"
#include "exec.h"
#include "resources.h"
#include "luamenus.h"
#include "config.h"

#include "gui.h"

ID3D11ShaderResourceView* logo = NULL;

namespace Menu {
    // Constants and variables
    const int LOGO_WIDTH = 18;
    const int LOGO_HEIGHT = 18;
    int selectedTab = 0;

    // Renders the logo image
    void renderLogo() {
        ImGui::SetCursorPosX(74);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 9);
        ImGui::Image((void*)logo, ImVec2(LOGO_WIDTH, LOGO_HEIGHT)); 
        ImGui::NewLine();
    }

    // Renders a menu button and sets the active tab if clicked
    void renderMenuButton(const char* icon, const char* label, int tab) {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
        ImGui::SetCursorPosX(0);
        Gui::Seperator("##main_seperator", 225, 1);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
        ImGui::SetCursorPosX(0);
        if (ImGui::Button(icon label, ImVec2(225, 46))) {
            selectedTab = tab;
        }
    }

    // Renders the license and build information
    void renderFooter() {
        ImGui::SetCursorPosY(380);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
        ImGui::Text("Licensed to #4278");
        ImGui::SetCursorPosY(399);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
        ImGui::Text("Build Date: Feb 10 2021");
    }

void renderSelectedTabContent() {
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 10); // Makes sure that the column goes the whole GUI height

    ImGui::NextColumn();

    switch (selectedTab) {
        case 0:
            Default::Render();
            break;
        case 1:
            Executor::Render();
            break;
        case 2:
            Resources::Render();
            break;
        case 3:
            Menus::Render();
            break;
        case 4:
            Config::Render();
            break;
        default:
            // Display an error message to the user or revert to a default tab
            break;
    }
}


bool TextEditor::IsOnWordBoundary(const Coordinates& aAt) const
{
	if (aAt.mLine >= (int)mLines.size() || aAt.mColumn == 0)
		return true;

	auto& line = mLines[aAt.mLine];
	auto cindex = GetCharacterIndex(aAt);
	if (cindex >= (int)line.size())
		return true;

	if (mColorizerEnabled)
		return line[cindex].mColorIndex != line[size_t(cindex - 1)].mColorIndex;

	return isspace(line[cindex].mChar) != isspace(line[cindex - 1].mChar);
}

void TextEditor::RemoveLines(int aStart, int aEnd)
{
    assert(!mReadOnly);
    assert(aEnd >= aStart);
    assert(static_cast<size_t>(aEnd - aStart) <= mLines.size());

    // Update error markers that are after the lines being removed
    ErrorMarkers etmp;
    for (auto& i : mErrorMarkers)
    {
        if (i.first >= aEnd)
        {
            etmp.insert({i.first - (aEnd - aStart), i.second});
        }
        else if (i.first < aStart)
        {
            etmp.insert(i);
        }
        else
        {
            // Skip error markers that are within the lines being removed
            continue;
        }
    }
    mErrorMarkers = std::move(etmp);

    // Update breakpoints that are after the lines being removed
    Breakpoints btmp;
    for (auto i : mBreakpoints)
    {
        if (i >= aEnd)
        {
            btmp.insert(i - (aEnd - aStart));
        }
        else if (i < aStart)
        {
            btmp.insert(i);
        }
        else
        {
            // Skip breakpoints that are within the lines being removed
            continue;
        }
    }
    mBreakpoints = std::move(btmp);

    // Remove the lines
    mLines.erase(mLines.begin() + aStart, mLines.begin() + aEnd);

    // Set the text changed flag
    mTextChanged = true;
}
