add_library (HelloWorld SHARED)
target_sources (
    HelloWorld
    PRIVATE HelloWorld.hpp HelloWorld.cpp
)
target_link_libraries (
    HelloWorld
    PRIVATE pass_examples_base
)
