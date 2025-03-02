
cmake_minimum_required(VERSION 3.10.2) 

project(UsbScroll VERSION 0.29.3)

#
MACRO (ADD_SOURCE_FILES_FOR_MODULE MODULE_NAME)
   LIST (APPEND SOURCE_FILES_INCLUDE_LIST ${MODULE_NAME})
ENDMACRO (ADD_SOURCE_FILES_FOR_MODULE)

#
MACRO (ADD_SOURCE_FILES_FOR_MODULES)
   FOREACH (MODULE ${ARGN})
      ADD_SOURCE_FILES_FOR_MODULE ( ${MODULE})
   ENDFOREACH (MODULE)
ENDMACRO (ADD_SOURCE_FILES_FOR_MODULES)

#
MACRO (CREATE_MODULE_FILES_LISTS)
   FILE( GLOB SOURCE_FILES_H ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
   FILE( GLOB SOURCE_FILES_CPP ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp)
ENDMACRO (CREATE_MODULE_FILES_LISTS)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++1z" HAS_STDCXX_1z)
if (HAS_STDCXX_1z)
    add_compile_options("-std=c++1z")
else()
    check_cxx_compiler_flag("/std:c++17" HAS_STDCXX_1z_MSVC)
    if (HAS_STDCXX_1z_MSVC)
      ADD_COMPILE_OPTIONS("/std:c++17")
    ELSE()
      MESSAGE("The compiler doesn't support the 1z C++ standard")
      exit()
    ENDIF()
endif()

if(NOT CMAKE_BUILD_TYPE)
  message(NOTICE "Build type was not defined. Set to Release.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Default build type: Release" FORCE)
else()
  message(STATUS "Build type defined as ${CMAKE_BUILD_TYPE}")
endif()

CREATE_MODULE_FILES_LISTS()

find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(OpenMP REQUIRED)

include(${wxWidgets_USE_FILE})

find_library(ESEUsbCam_LIBRARY libESEUsbCam.so HINTS /usr/lib/)

set(UsbScroll "UsbScroll")
add_executable (${UsbScroll} ${SOURCE_FILES_H} ${SOURCE_FILES_CPP})      
target_link_libraries(${UsbScroll} ${ESEUsbCam_LIBRARY} ${wxWidgets_LIBRARIES} OpenMP::OpenMP_CXX) 
