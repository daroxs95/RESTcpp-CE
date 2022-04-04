cmake_minimum_required(VERSION 3.0.0)

project(myproject.myapp VERSION 0.1.0)

include("collector/collector.cmake")#need to be after project()

include(CTest)

enable_testing()

set (CMAKE_CXX_STANDARD 17)


add_executable(myapp 
"src/main.cpp"
"src/service/UserService.cpp"
"src/service/UserServiceAddresses.cpp"
"src/service/UserServicePhones.cpp"
"src/utils/validators.cpp"
)


collect_and_find( oatpp "https://github.com/oatpp/oatpp" "1.3.0" myapp )
collect_and_find( oatpp-swagger "https://github.com/oatpp/oatpp-swagger" "1.3.0" myapp )

set(OATPP_SQLITE_AMALGAMATION ON)
append_var_to_cmake_args( OATPP_SQLITE_AMALGAMATION )
collect_and_find( oatpp-sqlite "https://github.com/oatpp/oatpp-sqlite" "1.3.0" myapp)

if( ${oatpp_FOUND} )
        message( STATUS "oatpp found" )
        target_link_libraries (myapp 
                PUBLIC oatpp::oatpp
                PUBLIC oatpp::oatpp-swagger
                PUBLIC oatpp::oatpp-sqlite )
endif()


add_definitions(
        -DOATPP_SWAGGER_RES_PATH="${_oatpp_DIR}/include/oatpp-1.3.0/bin/oatpp-swagger/res"

        ## SQLite database file
        -DDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/db.sqlite"
        ## SQLite database test file
        -DTESTDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/test-db.sqlite"

        ## Path to database migration scripts
        -DDATABASE_MIGRATIONS="${CMAKE_CURRENT_SOURCE_DIR}/sql"
)


# include_directories( ${_oatpp_DIR}/include/oatpp-1.3.0/oatpp )
# include_directories( ${_oatpp_DIR}/include/oatpp-1.3.0/oatpp-sqlite )
# include_directories( ${_oatpp_DIR}/include/oatpp-1.3.0/oatpp-swagger )
include_directories( src )
include_directories( 3rdParty )
include_directories( 3rdParty/sqlite )


# target_link_directories( myapp PRIVATE ${_oatpp_DIR}/lib/oatpp-1.3.0 )


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)


if(CMAKE_SYSTEM_NAME MATCHES Linux)
    find_package(Threads REQUIRED)
    target_link_libraries(crud-lib INTERFACE Threads::Threads ${CMAKE_DL_LIBS})
endif()

#target_link_libraries (myapp PRIVATE ws2_32 )

install(TARGETS myapp DESTINATION "bin")