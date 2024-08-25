#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroImportExport.hpp>

#if defined(GP_UNIT_TESTS_LIBRARY)
    #define GP_UNIT_TESTS_API GP_DECL_EXPORT
#else
    #define GP_UNIT_TESTS_API GP_DECL_IMPORT
#endif
