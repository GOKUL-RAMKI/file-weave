cmake_minimum_required(VERSION 3.21)

project(FileRelationshipGraphVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_library(core STATIC
    src/FileScanner.cpp
    src/MetadataCollector.cpp
    src/Tokenizer.cpp
    src/InvertedIndex.cpp
    src/SimilarityAnalyzer.cpp
    src/RelationshipScorer.cpp
    src/GraphBuilder.cpp
    src/DisjointSet.cpp
    src/Kruskal.cpp
    src/GraphLayout.cpp
)
target_include_directories(core PUBLIC include)

add_executable(app
    src/main.cpp
    src/MainWindow.cpp
    src/GraphView.cpp
    include/MainWindow.h
    include/GraphView.h
)
target_link_libraries(app PRIVATE core Qt6::Widgets)

if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${Qt6_DIR}/../../../bin")
    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET app POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}" $<IF:$<CONFIG:Debug>,--debug,--release>
                    --no-translations "$<TARGET_FILE:app>"
            COMMENT "Deploying Qt runtime next to app.exe")
    endif()
endif()

enable_testing()

foreach(test test_scanner test_tokenizer test_similarity test_graph_builder test_kruskal test_layout)
    add_executable(${test} tests/${test}.cpp)
    target_link_libraries(${test} PRIVATE core)
    add_test(NAME ${test} COMMAND ${test})
endforeach()
