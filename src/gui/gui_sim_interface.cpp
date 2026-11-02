// -----------------------------------------------------------------------------------------    
// gui_sim_interface
//
// Description: 
//     The component is in charge of drawing and hanling the sim interface menus
//     buttons and dialog boxes.
//
// ----------------------------------------------------------------------------------------- 
#include <imgui.h>
#include <string>

#include "gui_icons.hpp"
#include "gui_sim_interface.hpp"

#include "data_store/data_store.hpp"
#include "engine/sim_engine.hpp"
#include "support/types/unit_types.hpp"

// DEFINITIONS
static const char* periods[] { "30 d", " 5 d", " 1 d", " 8 h", " 3 h", " 1 h", "20 m", " 5 m", " 2 m", "30 s", " 5 s", " 1 s"};
static const int   increments[] {2592000, 432000, 86400, 28800, 10800, 3600, 1200, 300, 120, 30, 5, 1 };

// -----------------------------------------------------------------------------------------    
// Constructor
// ----------------------------------------------------------------------------------------- 
GuiSimInterface::GuiSimInterface()
{
    this->selectedPeriod = 0;
    return;
}


// -----------------------------------------------------------------------------------------
// Init
//
// Description: Initializes the Menu
// -----------------------------------------------------------------------------------------
void GuiSimInterface::Init(GuiMap *simMap)
{
  DataStore::GetInstance()->simTime->SetSimulationPeriod(increments[this->selectedPeriod]); 
  this->map = simMap; 
}



// -----------------------------------------------------------------------------------------    
// Execute
//
// Description: Execute the increment stablished in the mode defined
// -----------------------------------------------------------------------------------------    
void GuiSimInterface::Draw(int window_width, int window_height)
{

    // Get Date Time Data
    std::string date = DataStore::GetInstance()->simTime->GetFormatedDate("%Y-%m-%d %H:%M:%S");
    // Date Time GUI
    int DT_WIDTH  = 150;
    int DT_HEIGTH = 30;
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoCollapse |
                                           ImGuiWindowFlags_NoTitleBar |
                                           ImGuiWindowFlags_NoMove |
                                           ImGuiWindowFlags_NoScrollbar |
                                           ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::SetNextWindowPos(ImVec2(window_width-DT_WIDTH,0));
    ImGui::SetNextWindowSize(ImVec2(DT_WIDTH,DT_HEIGTH));                                           
    ImGui::Begin("Date_Time",nullptr, window_flags); 
    ImGui::Text(date.c_str());
    ImGui::End();

    // Control Time    
    ImGui::SetNextWindowPos(ImVec2(window_width-DT_WIDTH,DT_HEIGTH));
    ImGui::SetNextWindowSize(ImVec2(DT_WIDTH,DT_HEIGTH));  
    ImGui::Begin("Time Control",nullptr, window_flags);  
    ImGui::SetNextItemWidth(60);
    if (ImGui::Combo(" ", &this->selectedPeriod, periods, IM_ARRAYSIZE(periods), IM_ARRAYSIZE(periods)))
    {
        // execute change of increment
        DataStore::GetInstance()->simTime->SetSimulationPeriod(increments[this->selectedPeriod]);

    }
    ImGui::SameLine();
    if (SimEngine::GetInstance()->GetExecutionMode() == SimEngine::ExecutionModeType::EXEC_SIM_STEP)
    {
      if (ImGui::Button(ICON_SIM_PLAY)) SimEngine::GetInstance()->Execute(); 
    } else {
      if (ImGui::Button(ICON_SIM_STOP)) SimEngine::GetInstance()->Pause(); 
    }
    ImGui::SameLine();    
    if (ImGui::Button(ICON_SIM_STEP)) SimEngine::GetInstance()->Step(); 
    ImGui::End();

    // Map Info
    int RNG_WIDTH  = 150;
    int RNG_HEIGTH = 30;

    ImGui::SetNextWindowPos(ImVec2(window_width-RNG_WIDTH, window_height-RNG_HEIGTH));
    ImGui::SetNextWindowSize(ImVec2(RNG_WIDTH,RNG_HEIGTH));    
    ImGui::SetNextWindowBgAlpha(0.0f);                                       
    ImGui::Begin("Map Info",nullptr, window_flags); 
    ImGui::Text(UnitTypes::ToString(this->map->GetMapWidth()).c_str());
    ImGui::End();
      

    return;
}
