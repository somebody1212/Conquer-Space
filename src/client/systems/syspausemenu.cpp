/* Conquer Space
* Copyright (C) 2021 Conquer Space
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "client/systems/syspausemenu.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "client/scenes/universescene.h"
#include "client/systems/sysoptionswindow.h"
#include "common/version.h"
#include "engine/cqspgui.h"


void conquerspace::client::systems::SysPauseMenu::Init() {}

void conquerspace::client::systems::SysPauseMenu::DoUI(int delta_time) {
    if (!to_show) {
        return;
    }

    if (!to_show_options_window) {
        ImGui::SetNextWindowSize(ImVec2(200, -FLT_MIN), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
                                       ImGui::GetIO().DisplaySize.y * 0.5f),
                                ImGuiCond_Always, ImVec2(0.5f, 0.5f));

        ImGui::Begin("Pause menu", &to_show,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));

        const float button_height = 0;
        if (CQSPGui::DefaultButton("Resume", ImVec2(-FLT_MIN, button_height))) {
            to_show = false;
            conquerspace::scene::SetGameHalted(false);
        }
        CQSPGui::DefaultButton("Save Game", ImVec2(-FLT_MIN, button_height));
        CQSPGui::DefaultButton("Load Game", ImVec2(-FLT_MIN, button_height));
        ImGui::Separator();

        if (CQSPGui::DefaultButton("Options", ImVec2(-FLT_MIN, button_height))) {
            to_show_options_window = true;
        }

        ImGui::Separator();
        CQSPGui::DefaultButton( "Exit To Menu", ImVec2(-FLT_MIN, button_height));
        if (CQSPGui::DefaultButton("Exit Game", ImVec2(-FLT_MIN, button_height))) {
            // Kill game
            GetApp().ExitApplication();
        }
        ImGui::PopStyleVar();

        ImGui::Separator();
        ImGui::Text("Version: " CQSP_VERSION_STRING);

        ImGui::End();
    }

    if (to_show_options_window) {
        conquerspace::client::systems::ShowOptionsWindow(&to_show_options_window, GetApp());
    }
}

void conquerspace::client::systems::SysPauseMenu::DoUpdate(int delta_time) {
    if (GetApp().ButtonIsReleased(GLFW_KEY_ESCAPE)) {
        // Then pause
        to_show = !to_show;
        to_show_options_window = false;
        conquerspace::scene::SetGameHalted(to_show);
    }
}