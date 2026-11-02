// -----------------------------------------------------------------------------------------    
// gui_console
//
// Description: 
//     The component shows the Debug Console
//
// ----------------------------------------------------------------------------------------- 

#include <string>
#include <vector>
#include <imgui.h>

#include "gui_console.hpp"

#include "support/logger.hpp"

// -----------------------------------------------------------------------------------------    
// Constructor
// ----------------------------------------------------------------------------------------- 
GuiConsole::GuiConsole()
{
    return;
}


// -----------------------------------------------------------------------------------------
// Init
//
// Description: Initializes the Menu
// -----------------------------------------------------------------------------------------
void GuiConsole::Init()
{
  return; 
}

// -----------------------------------------------------------------------------------------    
// Execute
//
// Description: Execute the increment stablished in the mode defined
// -----------------------------------------------------------------------------------------    
void GuiConsole::Draw(int window_width, int window_height)
{

    // Get Logs
    std::vector<std::string> logs = Logger::GetInstance()->GetLogHistory();

    // Date Time GUI
    int DT_HEIGTH = 200;
//    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize |
//                                           ImGuiWindowFlags_NoCollapse |
//                                           ImGuiWindowFlags_NoTitleBar |
//                                           ImGuiWindowFlags_NoMove |
//                                           ImGuiWindowFlags_NoScrollbar |
//                                           ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::SetNextWindowPos(ImVec2(0,window_height - DT_HEIGTH));
    ImGui::SetNextWindowSize(ImVec2(window_width,DT_HEIGTH));                                           
    ImGui::Begin("Console");
    for (const auto& log: logs) { 
       ImGui::Text(log.c_str());
    }
    ImGui::SetScrollHereY(1.0f);
    ImGui::End();
    return;
}
