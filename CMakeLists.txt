cmake_minimum_required(VERSION 3.0.0)

project(myproject.myapp VERSION 0.1.0)

include("collector/collector.cmake")#need to be after project()

include(CTest)

enable_testing()

set (CMAKE_CXX_STANDARD 17)

add_executable(myapp "src/main.cpp" )

# collect( "https://github.com/open-source-parsers/jsoncpp" "1.9.4" myapp)
collect( "https://github.com/PotatoBite/xgetopt" "v1.0.0" myapp)
collect( "https://github.com/oatpp/oatpp" "1.3.0" myapp)


include_directories( ${_oatpp_DIR}/include/oatpp-1.3.0/oatpp )
target_link_directories( myapp PRIVATE ${_oatpp_DIR}/lib/oatpp-1.3.0 )


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)

target_link_libraries (myapp PRIVATE xgetopt oatpp oatpp-test ws2_32 )

install(TARGETS myapp DESTINATION "bin")