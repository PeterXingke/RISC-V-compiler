cmake_minimum_required( VERSION 3.10.0 )

project( rvcc )

add_executable(rvcc 
    main2.c
)

SET( CMAKE_C_FLAG "-std=c11 -g -fno-common" )
