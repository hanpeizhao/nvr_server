add_executable(test_smoke
    test_smoke.cpp
)

target_link_libraries(test_smoke PRIVATE
    nvr_platform
)

add_test(NAME smoke COMMAND test_smoke)
