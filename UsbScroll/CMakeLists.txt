
cmake_minimum_required(VERSION 3.10.2) 

set(CMAKE_GENERATOR_PLATFORM x64)

project(UsbScroll VERSION 0.29.3)

#
MACRO (ADD_SOURCE_FILES_FOR_MODULE MODULE_NAME)
   list (APPEND SOURCE_FILES_INCLUDE_LIST ${MODULE_NAME})
ENDMACRO (ADD_SOURCE_FILES_FOR_MODULE)

#
MACRO (ADD_SOURCE_FILES_FOR_MODULES)
   FOREACH (MODULE ${ARGN})
      ADD_SOURCE_FILES_FOR_MODULE ( ${MODULE})
   ENDFOREACH (MODULE)
ENDMACRO (ADD_SOURCE_FILES_FOR_MODULES)

#
MACRO (CREATE_MODULE_FILES_LISTS)
   file( GLOB SOURCE_FILES_H ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
   file( GLOB SOURCE_FILES_CPP ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp)
ENDMACRO (CREATE_MODULE_FILES_LISTS)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++1z" HAS_STDCXX_1z)
if (HAS_STDCXX_1z)
    add_compile_options("-std=c++1z")
else()
    check_cxx_compiler_flag("/std:c++17" HAS_STDCXX_1z_MSVC)
    if (HAS_STDCXX_1z_MSVC)
      ADD_COMPILE_OPTIONS("/std:c++17")
    else()
      MESSAGE(FATAL_ERROR "The compiler doesn't support the 1z C++ standard")
      exit()
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Build type was not defined. Set to Release.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Default build type: Release" FORCE)
else()
  message(STATUS "Build type defined as ${CMAKE_BUILD_TYPE}")
endif()

CREATE_MODULE_FILES_LISTS()

find_library(ESEUsbCam_LIBRARY ESEUsbCam HINTS ../Libraries/ESEUsbCamLib/lib/)

set(wxWidgetsLibs "")
FOREACH(wxLib wxmsw32u_core wxbase32u)
	find_library(wxLibFound ${wxLib} HINTS ../Libraries/wxWidgets/lib/vc14x_x64_dll NO_CACHE)
	list (APPEND wxWidgetsLibs optimized ${wxLibFound})
	set(wxLibFound "wxLibFound-NOTFOUND")
ENDFOREACH()

set(wxWidgetsLibsDebug "")
FOREACH(wxLib wxmsw32ud_core wxbase32ud)
	find_library(wxLibFound ${wxLib} HINTS ../Libraries/wxWidgets/lib/vc14x_x64_dll NO_CACHE)
	list (APPEND wxWidgetsLibsDebug debug ${wxLibFound})
	set(wxLibFound "wxLibFound-NOTFOUND")
ENDFOREACH()

list (APPEND dll_list_optim ../Libraries/wxWidgets/lib/vc14x_x64_dll/wxbase32u_vc14x_x64.dll ../Libraries/wxWidgets/lib/vc14x_x64_dll/wxmsw32u_core_vc14x_x64.dll)
list (APPEND dll_list_debug ../Libraries/wxWidgets/lib/vc14x_x64_dll/wxbase32ud_vc14x_x64.dll ../Libraries/wxWidgets/lib/vc14x_x64_dll/wxmsw32ud_core_vc14x_x64.dll)
list (APPEND dll_general ../Libraries/ESEUsbCamLib/lib/libusb-1.0.dll  ../Libraries/ESEUsbCamLib/lib/ESEUsbCam.dll)

find_package(OpenMP REQUIRED)

set(UsbScroll "UsbScroll")
add_executable (${UsbScroll} ${SOURCE_FILES_H} ${SOURCE_FILES_CPP})      
target_compile_definitions(${UsbScroll} PUBLIC wxUSE_UNICODE=1 _UNICODE UNICODE WXUSINGDLL)
target_link_libraries(${UsbScroll} ${ESEUsbCam_LIBRARY} OpenMP::OpenMP_CXX ${wxWidgetsLibs} ${wxWidgetsLibsDebug}) 
target_include_directories(${UsbScroll} PRIVATE ../Libraries/ESEUsbCamLib/include ../Libraries/wxWidgets/include ../Libraries/wxWidgets/lib/vc14x_x64_dll/mswu)
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${UsbScroll})

set(dll_command "")
FOREACH(dbg opt IN ZIP_LISTS dll_list_debug dll_list_optim)
   list (APPEND dll_command COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/$<IF:$<CONFIG:Debug>,${dbg},${opt}> $<TARGET_FILE_DIR:${UsbScroll}>)
ENDFOREACH()
FOREACH(gnrl IN LISTS dll_general)
   list (APPEND dll_command COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/${gnrl} $<TARGET_FILE_DIR:${UsbScroll}>)
ENDFOREACH()

add_custom_command(TARGET ${UsbScroll} POST_BUILD ${dll_command})
