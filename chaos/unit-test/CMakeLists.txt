find_package(Boost)

function(single_test arg)
    add_executable(${arg} ${arg}.cpp)
endfunction()

single_test(timed_mutex_fail)
