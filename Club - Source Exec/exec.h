#pragma once
#include "imgui/imgui.h"
#include "TextEditor.h"
#include "font_awesome.h"
#include <vector>

TextEditor editor;

static int selectedResource = 0;
std::vector<std::string> resources = {
    "_cfx_internal", "fivem", "hardcap", "anti-cheat", "esx_mafia", "esx_gopostal",
    "esx_jail", "mysql-async", "es_admin2", "async", "es_extended", "esx_barbershop",
    "esx_clotheshop", "esx_datastore", "esx_garage", "esx_identity", "esx_lscustom",
    "esx_menu_default", "esx_menu_dialog", "esx_menu_list", "esx_property", "esx_shops",
    "esx_sit", "esx_vehicleshop", "esx_weashops", "es_camera", "instance", "skinchanger",
    "mellotrainer", "bob74_ipl", "coordsaver", "loadingscreen"
};


namespace Executor
{
    void Render()
    {
        // Get the size of the window
        const auto window_size = ImGui::GetWindowSize();

        // Create a text editor and apply settings
        TextEditor editor;
        editor.SetReadOnly(false);
        editor.SetShowWhitespaces(false);
        editor.SetPalette(TextEditor::GetDarkPalette());

        // Set the position of the "Executor" text
        const float title_x_pos = ImGui::GetContentRegionAvail().x / 1.3f;
        ImGui::SetCursorPosX(title_x_pos);

        // Render the "Executor" text and a separator
        ImGui::Text("Executor");
        ImGui::Separator();

        // Render the code editor
        const ImVec2 editor_size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
        ImGui::BeginChild("Editor", ImVec2(0, window_size.y - 110), true);
        editor.Render("Code Editor", editor_size, true);
        ImGui::EndChild();

        // Render the "Run Code" button and check the selected resource
        const ImVec2 button_size = ImVec2(116, 30);
        if (ImGui::Button("Run Code", button_size))
        {
            const std::string selected_resource = resources[selected_resource_index];
            if (selected_resource == "_cfx_internal")
            {
                MessageBoxA(NULL, "You can't execute in _cfx_interal", "redENGINE", MB_OK | MB_ICONERROR);
                return;
            }

            // Execute the code
        }

        // Render the "Load Code from File" button and the resource combo box
        const ImVec2 load_button_size = ImVec2(180, 30);
        ImGui::SameLine();
        if (ImGui::Button("Load Code from File", load_button_size))
        {
            // TODO: implement file loading
        }
        ImGui::SameLine();
        const ImVec2 combo_size = ImVec2(ImGui::GetContentRegionAvail().x, button_size.y);
        ImGui::Combo("Select Resource", &selected_resource_index, resources.data(), static_cast<int>(resources.size()), combo_size);
    }
}

