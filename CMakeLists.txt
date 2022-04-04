cmake_minimum_required(VERSION 3.0.0)

project(myproject.myapp VERSION 0.1.0)

include(ExternalProject)


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


ExternalProject_Add( _oatpp
  SOURCE_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/deps/oatpp
  GIT_REPOSITORY      "https://github.com/oatpp/oatpp"
  BINARY_DIR          deps_bin/oatpp
  GIT_TAG             "1.3.0"
  LOG_CONFIGURE       ON
  CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=deps_install
)

ExternalProject_Add( _oatpp-swagger
  SOURCE_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/deps/oatpp-swagger
  GIT_REPOSITORY      "https://github.com/oatpp/oatpp-swagger"
  BINARY_DIR          deps_bin/oatpp-swagger
  GIT_TAG             "1.3.0"
  LOG_CONFIGURE       ON
  CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=deps_install
)

ExternalProject_Add( _oatpp-sqlite
  SOURCE_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/deps/oatpp-sqlite
  GIT_REPOSITORY      "https://github.com/oatpp/oatpp-sqlite"
  BINARY_DIR          deps_bin/oatpp-sqlite
  GIT_TAG             "1.3.0"
  LOG_CONFIGURE       ON
  CMAKE_ARGS          -DOATPP_SQLITE_AMALGAMATION=ON
  CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=deps_install
)
add_dependencies( _oatpp-sqlite _oatpp )
add_dependencies( _oatpp-swagger _oatpp )
add_dependencies( myapp _oatpp _oatpp-sqlite _oatpp-swagger)

add_definitions(
        -DOATPP_SWAGGER_RES_PATH="res"

        ## SQLite database file
        -DDATABASE_FILE="db.sqlite"
        ## SQLite database test file
        #-DTESTDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/test-db.sqlite"

        ## Path to database migration scripts
        -DDATABASE_MIGRATIONS="sql"
)


include_directories( ${CMAKE_CURRENT_BINARY_DIR}/deps_install/include/oatpp-1.3.0/oatpp )
include_directories( ${CMAKE_CURRENT_BINARY_DIR}/deps_install/include/oatpp-1.3.0/oatpp-sqlite )
include_directories( ${CMAKE_CURRENT_BINARY_DIR}/deps_install/include/oatpp-1.3.0/oatpp-swagger )
include_directories( src )
include_directories( 3rdParty )
include_directories( 3rdParty/sqlite )
target_link_directories( myapp PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/deps_install/lib/oatpp-1.3.0 )


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)


target_link_libraries (myapp PRIVATE oatpp-sqlite oatpp-swagger oatpp oatpp-test sqlite)

# check if we are on Windows
if( WIN32 )
  target_link_libraries(myapp PRIVATE ws2_32 )
else()
  find_package(Threads REQUIRED)
  target_link_libraries(myapp PRIVATE Threads::Threads ${CMAKE_DL_LIBS})
endif()





include(GNUInstallDirs)

set_target_properties(myapp
                      PROPERTIES 
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      PDB_OUTPUT_DIRECTORY     "${CMAKE_BINARY_DIR}/bin"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)




### Add some usable files to project build folder
set (RequiredExtras
    "sql"
    "res"
)
get_target_property(BINARY_EXECUTABLE_OUTPUT myapp RUNTIME_OUTPUT_DIRECTORY)
foreach (_dir ${RequiredExtras})
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        list( APPEND extraFiles COMMAND "${CMAKE_COMMAND}" -E copy_directory  "${PROJECT_SOURCE_DIR}/${_dir}" "${BINARY_EXECUTABLE_OUTPUT}/$<CONFIG>/${_dir}")
    else()
        list( APPEND extraFiles COMMAND "${CMAKE_COMMAND}" -E copy_directory  "${PROJECT_SOURCE_DIR}/${_dir}" "${BINARY_EXECUTABLE_OUTPUT}/${_dir}")
    endif()
endforeach()
add_custom_command(TARGET myapp POST_BUILD
                    ${extraFiles}
)

foreach (_dir ${RequiredExtras})
  install( DIRECTORY ${_dir}
        DESTINATION bin
    )
endforeach()

install(TARGETS myapp
    RUNTIME DESTINATION  ${CMAKE_INSTALL_BINDIR}
    RESOURCE DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION  ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION  ${CMAKE_INSTALL_LIBDIR}
    OBJECTS DESTINATION  ${CMAKE_INSTALL_LIBDIR}
)
