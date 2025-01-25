cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME "s1995")
project(${PROJECT_NAME} VERSION 0.0.1)

set(JUCE_COPY_PLUGIN_AFTER_BUILD TRUE)

set(JUCE_PATH "../JUCE")
add_subdirectory(${JUCE_PATH} "${CMAKE_CURRENT_BINARY_DIR}/JUCE")

juce_add_plugin(${PROJECT_NAME}
    FORMATS VST3
    COMPANY_NAME "GI"
    PRODUCT_NAME "s1995"
    PLUGIN_MANUFACTURER_NAME "Giulio Iacomino"
    AU FALSE
    AAX FALSE
    LV2 FALSE
    VST FALSE
)

target_sources(${PROJECT_NAME} PRIVATE
    Source/s1995PluginAudioProcessor.cpp
    Source/s1995PluginAudioProcessor.hpp
    Source/s1995PluginAudioProcessorEditor.cpp
    Source/s1995PluginAudioProcessorEditor.hpp
    
    Source/PreADCChain.hpp
    Source/PreADCChain.cpp
    Source/ADCChain.hpp
    Source/ADCChain.cpp
    Source/LoFilter.hpp
    Source/LoFilter.cpp
    Source/DACChain.hpp
    Source/DACChain.cpp
    
    Source/CustomSlider.hpp
    Source/CustomSlider.cpp
    
    Source/AudioMeter.hpp
    Source/AudioMeter.cpp
    Source/AudioMeterProcessor.hpp
    Source/AudioMeterProcessor.cpp
)

set(BINARY_DATA
    Assets/logo.svg
)

juce_add_binary_data(BinaryData SOURCES ${BINARY_DATA})

set(COMPILE_DEFS
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

add_compile_definitions(${COMPILE_DEFS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_dsp
    juce::juce_gui_extra
    juce::juce_graphics
    BinaryData
)

juce_generate_juce_header(${PROJECT_NAME})
