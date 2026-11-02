// -----------------------------------------------------------------------------------------    
// data_types
//
// Description: 
//     The component includes all the definitions of types used on data component
//
// -----------------------------------------------------------------------------------------    
#pragma once

#include <vector>
#include <string>


namespace Data {


    enum class DataResult {
             DATA_OK, 
             DATA_MISSING_FILE, 
             DATA_CORRUPT_FILE, 
             DATA_FAILED_SAVE
    };


    enum class SystemBodyType {
            SB_STAR,
            SB_PLANET,
            SB_MOON,
            SB_ASTEROID,
            SB_COMET
    };


    struct Row {
        std::vector<std::string> values;
    };

    struct QueryResults {
        std::vector<std::string> columnNames;
        std::vector<Row>         rows;
    };

    
};
