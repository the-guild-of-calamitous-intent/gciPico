list(APPEND devs
    fixedpt
)

foreach(app ${devs})
    add_executable(${app} ${app}.cpp)
    target_link_libraries(${app} PRIVATE ${PROJECT_NAME})
endforeach()
