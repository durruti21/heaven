// -----------------------------------------------------------------------------------------    
// gui_item_display
//
// Description: 
//     The component is in charge of displaying a panel with the information of the object
//
// ----------------------------------------------------------------------------------------- 
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <SDL_image.h>
#include <string>
#include <algorithm> // required for std::transfor
#include <cctype>    // required for ::tolower

#include "gui_item_display.hpp"
#include "support/logger.hpp"

// -----------------------------------------------------------------------------------------    
// Constructor
// ----------------------------------------------------------------------------------------- 
GuiItemDisplay::GuiItemDisplay(SDL_Renderer *render)
{
   firstTime = true;
   this->renderer   = render;
   this->showPannel = false;   
}

// -----------------------------------------------------------------------------------------
// SetSystemBody
//
// Description: Asigns the system body
// -----------------------------------------------------------------------------------------
void GuiItemDisplay::SetSystemBody(std::shared_ptr<SystemBody> object)
{
    displayedBody = *object;
    //load here the image associated to the object
    std::string S = displayedBody.name;
    std::transform(S.begin(),S.end(),S.begin(),[](unsigned char c) {return std::tolower(c);});
    std::string imagefile = "data/reference/images/sbodies/"+ S +".jpg";
    this->imageTexture = IMG_LoadTexture(this->renderer, imagefile.c_str());
    if (!imageTexture)
    {
       Logger::GetInstance()->Log(Logger::LogLevel::LOG_WARNING, "GuiItemDisplay::SetSystemBody could not load image file: " + displayedBody.name);
    }
    this->showPannel = true;
}

// -----------------------------------------------------------------------------------------    
// Draw
//
// Description: Execute the increment stablished in the mode defined
// ------------_-----------------------------------------------------------------------------    
void GuiItemDisplay::Draw(int window_width, int window_height)
{
    // Date Time GUI
    int DT_WIDTH  = 300;
    int DT_HEIGTH = 300;
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoCollapse |
                                           ImGuiWindowFlags_NoScrollbar |
                                           ImGuiWindowFlags_AlwaysAutoResize;
    std::string helper;


    if (firstTime)
    {
       ImGui::SetNextWindowPos(ImVec2((window_width/2) - (DT_WIDTH/2),(window_height/2)-(DT_HEIGTH/2)));
       firstTime = false;
    }
    ImGui::SetNextWindowSize(ImVec2(DT_WIDTH,DT_HEIGTH)); 
    if (this->showPannel)
    {
      ImGui::Begin("Item Display",&this->showPannel, window_flags);
      helper = "Name: " + displayedBody.name;
      ImGui::Text(helper.c_str());
      ImGui::SetCursorPos(ImVec2(50,50));
      ImGui::Image((ImTextureID)this->imageTexture,ImVec2(200,200));
      ImGui::SetCursorPos(ImVec2(0,250));
      helper = "Mass: " + UnitTypes::BodyMassToString(displayedBody.bodyMass, true);
      ImGui::Text(helper.c_str());
      ImGui::End();
    }
    
    if (!this->showPannel)
    {
        Logger::GetInstance()->Log(Logger::LogLevel::LOG_DEBUG, "GuiItemDisplay::Draw show Pannel is FALSE");
    }
    
}
