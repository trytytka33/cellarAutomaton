# CMakeList.txt: plik projektu CMake najwy¿szego poziomu, wykonaj konfiguracjê globaln¹
# i uwzglêdnij podprojekty w tym miejscu.
#
cmake_minimum_required (VERSION 3.8)

# W³¹cz prze³adowywanie kodu XAML na gor¹co dla kompilatorów MSVC, jeœli jest to obs³ugiwane.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("VirusSpread")

set(SOURCES 
    src/Simulation.cpp 
    src/main.cpp 
    src/LoadMap.cpp
)

set(HEADERS 
    header/Simulation.h 
    header/LoadMap.h
)


set(SFML_INCLUDE_DIR "D:\\kozack rzeczy\\SFML\\SFML-3.0.0\\include")
set(SFML_LIBRARY_DIR "D:\\kozack rzeczy\\SFML\\SFML-3.0.0\\lib")
set(SFML_DIR "D:\\kozack rzeczy\\SFML\\SFML-3.0.0\\lib\\cmake\\SFML")

link_directories(SFML_LIBRARY_DIR )
include_directories(SFML_INCLUDE_DIR )

find_package(SFML COMPONENTS Graphics Window System REQUIRED)

add_executable (VirusSpread ${SOURCES} ${HEADERS})

target_include_directories(VirusSpread PRIVATE header)

target_link_libraries(VirusSpread PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VirusSpread PROPERTY CXX_STANDARD 20)
endif()
