// -----------------------------------------------------------------------------------------    
// config_types
//
// Description: 
//     The component includes all the definitions of types used on config component
//
// -----------------------------------------------------------------------------------------    
#pragma once

namespace Config {


    enum class ConfigResult {
             CONFIG_OK, 
             CONFIG_MISSING_FILE, 
             CONFIG_CORRUPT_FILE, 
             CONFIG_FAILED_SAVE
    };

};
