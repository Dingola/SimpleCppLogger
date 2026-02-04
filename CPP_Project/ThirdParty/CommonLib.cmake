# CommonLib Integration
set(THIRD_PARTY_TARGET "CommonLib")
set(GIT_TAG "main")
set(PROJECT_DIR_NAME "${THIRD_PARTY_TARGET}_${GIT_TAG}")
set(THIRD_PARTY_TARGET_DIR "${THIRD_PARTY_INCLUDE_DIR}/${PROJECT_DIR_NAME}")
set(CMAKE_ARGS "-D ${THIRD_PARTY_TARGET}_BUILD_TARGET_TYPE:STRING=static_library -D MAIN_PROJECT_NAME:STRING=CommonLib")
set(CommonLib_INSTALL_ROOT "${THIRD_PARTY_TARGET_DIR}/${THIRD_PARTY_TARGET}_install")
set(CommonLib_INCLUDE_DIR "${CommonLib_INSTALL_ROOT}/${CMAKE_BUILD_TYPE}/include")
set(CommonLib_LIBRARY     "${CommonLib_INSTALL_ROOT}/${CMAKE_BUILD_TYPE}/lib/CommonLib.lib")
set(CommonLib_DIR "${CommonLib_INSTALL_ROOT}/lib/cmake/CommonLib")

find_package(CommonLib HINTS ${CommonLib_INSTALL_ROOT}/${CMAKE_BUILD_TYPE}/lib/cmake/CommonLib NO_DEFAULT_PATHS)

if(CommonLib_FOUND)
    message("CommonLib found")
else()
    message("CommonLib not found. Downloading and invoking cmake ..")
    build_third_party_project(
        false
        ${THIRD_PARTY_TARGET}
        https://github.com/Dingola/CommonLib.git
        ${GIT_TAG}
        ${THIRD_PARTY_TARGET_DIR}
        ${CMAKE_BUILD_TYPE}
		${CMAKE_ARGS}
    )
	
	find_package(CommonLib REQUIRED HINTS ${CommonLib_INSTALL_ROOT}/${CMAKE_BUILD_TYPE}/lib/cmake/CommonLib NO_DEFAULT_PATHS)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC CommonLib)
