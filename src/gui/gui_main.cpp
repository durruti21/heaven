// -----------------------------------------------------------------------------------------    
// gui_main
// -----------------------------------------------------------------------------------------    
#include <imgui.h>
#include <memory>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

#include "gui_main.hpp"

#include "config/config_data.hpp"
#include "support/logger.hpp"

// ENUM CLASS FOR GUI ACTIONS
enum class GUIActionsType {
            GUI_ACTION_NONE,
            GUI_ACTION_EXIT,
            GUI_ACTION_SWITCH_CONSOLE,
            GUI_ACTION_ZOOM_IN,
            GUI_ACTION_ZOOM_OUT,
            GUI_ACTION_MOVE_LEFT,
            GUI_ACTION_MOVE_RIGHT,
            GUI_ACTION_MOVE_UP,
            GUI_ACTION_MOVE_DOWN,
            GUI_ACTION_SELECT
};

// -----------------------------------------------------------------------------------------    
// Constructor
// ----------------------------------------------------------------------------------------- 
GUIMain::GUIMain() {
   this->window      = NULL;
   this->renderer    = NULL;
   this->displayMode = NULL;
}

// -----------------------------------------------------------------------------------------    
// Init
// ----------------------------------------------------------------------------------------- 
SDL_AppResult GUIMain::Init(std::string name, std::string version, std::string url)
 {

    Logger *logger = Logger::GetInstance();
    ConfigData *configData = ConfigData::GetInstance();

    // Init states
    showConsole = false;

    // Init SDL
    SDL_SetAppMetadata(name.c_str(), version.c_str(), url.c_str());

    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        logger->Log(Logger::LogLevel::LOG_CRITICAL, "GUIMain::Init Couldn't initialize SDL: " + std::string(SDL_GetError()));
        return SDL_APP_FAILURE;
    }

    // Get current display mode
    int numDisplays;
    SDL_DisplayID * displaysID = SDL_GetDisplays(&numDisplays);
    
    this->displayMode = SDL_GetCurrentDisplayMode(displaysID[0]);
    if (this->displayMode == NULL)
    {
        logger->Log(Logger::LogLevel::LOG_CRITICAL, "GUIMain::Init Failed to Get Current Display mode: " + std::string(SDL_GetError()));
        return SDL_APP_FAILURE;
    }

    // Set OpenGL Attributes
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3))
    {
        logger->Log(Logger::LogLevel::LOG_CRITICAL, "GUIMain::Init Failed to Set OpenGL Major Version: " + std::string(SDL_GetError()));
    }
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3))
    {
        logger->Log(Logger::LogLevel::LOG_CRITICAL, "GUIMain::Init Failed to Set OpenGL Minor Version: " + std::string(SDL_GetError()));
    }
    if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1))
    {
        logger->Log(Logger::LogLevel::LOG_CRITICAL, "GUIMain::Init Failed to Set Multi Sample Buffer: " + std::string(SDL_GetError()));
    } 
    if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4))
    {
        logger->Log(Logger::LogLevel::LOG_CRITICAL, "GUIMain::Init Failed to Set Multi Sample Samples: " + std::string(SDL_GetError()));
    }               

    // Create Window and Renderer
    if (!SDL_CreateWindowAndRenderer("HEAVEN", 
                                     this->displayMode->w, 
                                     this->displayMode->h, 
                                     SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL, 
                                     &this->window, 
                                     &this->renderer)) 
    {
        logger->Log(Logger::LogLevel::LOG_CRITICAL, "GUIMain::Init Couldn't create window/renderer: " + std::string(SDL_GetError()));
        return SDL_APP_FAILURE;
    }


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Merge Icons Fonts into Default Fonts
    io.Fonts->AddFontDefault();
    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 13.0f;
    io.Fonts->AddFontFromFileTTF(configData->gameApp->GetFontsFile().c_str(), 13.0f, &config);


    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(this->window, this->renderer);
    ImGui_ImplSDLRenderer3_Init(this->renderer);    

    // Init Subcomponents
    guiConsole.Init();
    guiMap.Init(this->renderer, this->displayMode->w, this->displayMode->h);
    guiSimIntf.Init(&guiMap);
    guiItemDisplay = std::make_shared<GuiItemDisplay>(this->renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */

}

// -----------------------------------------------------------------------------------------    
// ProcessEvent
// ----------------------------------------------------------------------------------------- 
SDL_AppResult GUIMain::ProcessEvent(void *appstate, SDL_Event *event) {

    Logger *logger = Logger::GetInstance();
    GUIActionsType action = GUIActionsType::GUI_ACTION_NONE;
    std::shared_ptr<SystemBody> selectedObj;

    ImGui_ImplSDL3_ProcessEvent(event);

    // Search for key and mouse actions
    if (event->type == SDL_EVENT_KEY_DOWN) 
    {
        /* Open/Close Debug Console */
        if (event->key.key == SDLK_TAB)
        { 
            action = GUIActionsType::GUI_ACTION_SWITCH_CONSOLE;
        }
        /* end the program, reporting success to the OS. */
        else if (event->key.key == SDLK_ESCAPE)
        {
            action = GUIActionsType::GUI_ACTION_EXIT;
        }  
        /* Zoom in, zoom out commands */
        else if (event->key.key == SDLK_Z)
        {
            action = GUIActionsType::GUI_ACTION_ZOOM_IN;
        }
        else if (event->key.key == SDLK_X)
        {
            action = GUIActionsType::GUI_ACTION_ZOOM_OUT;
        }
        else if (event->key.key == SDLK_W)
        {
            action = GUIActionsType::GUI_ACTION_MOVE_UP;
        }
        else if (event->key.key == SDLK_S)
        {
            action = GUIActionsType::GUI_ACTION_MOVE_DOWN;
        }
        else if (event->key.key == SDLK_A)
        {
            action = GUIActionsType::GUI_ACTION_MOVE_LEFT;
        }
        else if (event->key.key == SDLK_D)
        {
            action = GUIActionsType::GUI_ACTION_MOVE_RIGHT;
        }
    } else if (event->type == SDL_EVENT_MOUSE_WHEEL) 
    {
        if (event->wheel.y > 0)       action = GUIActionsType::GUI_ACTION_ZOOM_IN;
        else if (event->wheel.y < 0)  action = GUIActionsType::GUI_ACTION_ZOOM_OUT;

    } else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            action = GUIActionsType::GUI_ACTION_SELECT;
        }
    }

    // Perform action
    switch (action)
    {
        case GUIActionsType::GUI_ACTION_EXIT:
            logger->Log(Logger::LogLevel::LOG_DEBUG, "GUIMain::ProcessEvent Exiting application ");
            return SDL_APP_SUCCESS;

        case GUIActionsType::GUI_ACTION_SWITCH_CONSOLE:
            logger->Log(Logger::LogLevel::LOG_DEBUG, "GUIMain::ProcessEvent Switching console ");
            this->showConsole = !this->showConsole;
            break;

        case GUIActionsType::GUI_ACTION_ZOOM_IN:
            logger->Log(Logger::LogLevel::LOG_DEBUG, "GUIMain::ProcessEvent Zoom In");
            guiMap.ZoomIn();
            break;

        case GUIActionsType::GUI_ACTION_ZOOM_OUT:
            logger->Log(Logger::LogLevel::LOG_DEBUG, "GUIMain::ProcessEvent Zoom Out");
            guiMap.ZoomOut();
            break;

        case GUIActionsType::GUI_ACTION_MOVE_UP:
            logger->Log(Logger::LogLevel::LOG_DEBUG, "GUIMain::ProcessEvent Move Up");
            guiMap.MoveUp();
            break;

        case GUIActionsType::GUI_ACTION_MOVE_DOWN:
            logger->Log(Logger::LogLevel::LOG_DEBUG, "GUIMain::ProcessEvent Move Down");
            guiMap.MoveDown();
            break;

        case GUIActionsType::GUI_ACTION_MOVE_LEFT:
            logger->Log(Logger::LogLevel::LOG_DEBUG, "GUIMain::ProcessEvent Move Left");
            guiMap.MoveLeft();
            break;

        case GUIActionsType::GUI_ACTION_MOVE_RIGHT:
            logger->Log(Logger::LogLevel::LOG_DEBUG, "GUIMain::ProcessEvent Move Right");
            guiMap.MoveRight();
            break; 
            
        case GUIActionsType::GUI_ACTION_SELECT:
            logger->Log(Logger::LogLevel::LOG_DEBUG, "GUIMain::ProcessEvent Select");
            selectedObj = guiMap.GetObjectAt(event->button.x, event->button.y);
            if (selectedObj)
            {
                guiItemDisplay->SetSystemBody(selectedObj);
                logger->Log(Logger::LogLevel::LOG_DEBUG, "GUIMain::ProcessEvent object selected is " + selectedObj->name);
            }
            break; 
            
            
        default:
            break;
    };
            

    return SDL_APP_CONTINUE;  /* carry on with the program! */

}


// -----------------------------------------------------------------------------------------    
// Iterate
// ----------------------------------------------------------------------------------------- 
 SDL_AppResult GUIMain::Iterate(void *appstate) {


    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    //ImGui::ShowDemoWindow();
    guiSimIntf.Draw(this->displayMode->w,this->displayMode->h);
    if (showConsole) guiConsole.Draw(this->displayMode->w,this->displayMode->h);
    if (guiItemDisplay) guiItemDisplay->Draw(this->displayMode->w,this->displayMode->h);

    // Rendering
    ImGui::Render();

    /* Black Color. */
    const float red   = 0.0; //(float) (0.5 + 0.5 * SDL_sin(now));
    const float green = 0.0; //(float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
    const float blue  = 0.0; //(float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));
    SDL_SetRenderDrawColorFloat(this->renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */
    SDL_RenderClear(this->renderer);

    // Pain Map as overall background
    guiMap.Draw(this->displayMode->w,this->displayMode->h);
    // paint Imgui data
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), this->renderer);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(this->renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */

}


// -----------------------------------------------------------------------------------------    
// Quit
// ----------------------------------------------------------------------------------------- 
void GUIMain::Quit(void *appstate, SDL_AppResult result) {

    // Destroy Imgui
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    // Destroy SDL
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();    
  return;
}

