cmake_minimum_required(VERSION 3.16)
project(KalaNetClient VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Project information
set(PROJECT_DESCRIPTION "KalaNet - Online Marketplace Client")
set(PROJECT_HOMEPAGE_URL "https://github.com/yourusername/KalaNetClient")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Find required Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Sql)
find_package(Qt6 OPTIONAL_COMPONENTS Test)

# ========================================
# SOURCE FILES
# ========================================

set(SOURCES
    main.cpp
    mainwindow.cpp
	network/ClientSocket.cpp
)

# Pages
set(PAGE_SOURCES
    pages/AdDetailsPage.cpp
    pages/AdsListPage.cpp
    pages/CartPage.cpp
    pages/EditProfilePage.cpp
    pages/ForgotPasswordPage.cpp
    pages/HistoryPage.cpp
    pages/LoginPage.cpp
    pages/MainMenuPage.cpp
    pages/PostAdPage.cpp
    pages/ProfilePage.cpp
    pages/SignupPage.cpp
    pages/WalletPage.cpp
)

# Dialogs
set(DIALOG_SOURCES
    dialogs/AddTokenDialog.cpp
    dialogs/ConfirmBuyDialog.cpp
    dialogs/ErrorDialog.cpp
)

# Combine all sources
list(APPEND SOURCES ${PAGE_SOURCES} ${DIALOG_SOURCES})

# ========================================
# HEADER FILES
# ========================================

set(HEADERS
    mainwindow.h
	network/ClientSocket.h
)

# Page headers
set(PAGE_HEADERS
    pages/AdDetailsPage.h
    pages/AdsListPage.h
    pages/CartPage.h
    pages/EditProfilePage.h
    pages/ForgotPasswordPage.h
    pages/HistoryPage.h
    pages/LoginPage.h
    pages/MainMenuPage.h
    pages/PostAdPage.h
    pages/ProfilePage.h
    pages/SignupPage.h
    pages/WalletPage.h
)

# Dialog headers
set(DIALOG_HEADERS
    dialogs/AddTokenDialog.h
    dialogs/ConfirmBuyDialog.h
    dialogs/ErrorDialog.h
)

# Combine all headers
list(APPEND HEADERS ${PAGE_HEADERS} ${DIALOG_HEADERS})

# ========================================
# UI FILES
# ========================================

set(FORMS
    mainwindow.ui
)

# Page UI files
set(PAGE_FORMS
    pages/AdDetailsPage.ui
    pages/AdsListPage.ui
    pages/CartPage.ui
    pages/EditProfilePage.ui
    pages/ForgotPasswordPage.ui
    pages/HistoryPage.ui
    pages/LoginPage.ui
    pages/MainMenuPage.ui
    pages/PostAdPage.ui
    pages/ProfilePage.ui
    pages/SignupPage.ui
    pages/WalletPage.ui
)

# Dialog UI files
set(DIALOG_FORMS
    dialogs/AddTokenDialog.ui
    dialogs/ConfirmBuyDialog.ui
    dialogs/ErrorDialog.ui
)

# Combine all UI files
list(APPEND FORMS ${PAGE_FORMS} ${DIALOG_FORMS})

# ========================================
# RESOURCE FILES
# ========================================

#set(RESOURCES
 #   resources.qrc
#)

# ========================================
# INCLUDE DIRECTORIES
# ========================================

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/pages
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs
	${CMAKE_CURRENT_SOURCE_DIR}/network
	${CMAKE_CURRENT_SOURCE_DIR}/../Common
)

# ========================================
# EXECUTABLE TARGET
# ========================================

add_executable(KalaNetClient ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES})

# Set properties
set_target_properties(KalaNetClient PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
    OUTPUT_NAME "KalaNet"
)

# Link Qt libraries
target_link_libraries(KalaNetClient
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    Qt6::Sql
)

# ========================================
# COMPILER OPTIONS
# ========================================

if(MSVC)
    target_compile_options(KalaNetClient PRIVATE /W4 /WX)
else()
    target_compile_options(KalaNetClient PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# ========================================
# DEBUG AND RELEASE CONFIGURATIONS
# ========================================

set_target_properties(KalaNetClient PROPERTIES
    DEBUG_POSTFIX "d"
    RELEASE_POSTFIX ""
)

# ========================================
# INSTALL TARGETS
# ========================================

install(TARGETS KalaNetClient
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
    LIBRARY DESTINATION lib
)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md
    ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
    DESTINATION .
)

# ========================================
# CPACK PACKAGING
# ========================================

#set(CPACK_PACKAGE_NAME "KalaNet")
#set(CPACK_PACKAGE_VENDOR "KalaNet Team")
#set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
#set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
#set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
#set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
#set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
#set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
#set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)

# Package generators
#set(CPACK_GENERATOR "ZIP;TGZ;DEB;RPM")

# Windows NSIS installer
#if(WIN32)
 #   set(CPACK_GENERATOR "${CPACK_GENERATOR};NSIS")
  #  set(CPACK_NSIS_DISPLAY_NAME "KalaNet Client")
   # set(CPACK_NSIS_HELP_LINK ${PROJECT_HOMEPAGE_URL})
    #set(CPACK_NSIS_URL_INFO_ABOUT ${PROJECT_HOMEPAGE_URL})
    #set(CPACK_NSIS_MODIFY_PATH OFF)
#endif()

# macOS bundle
#if(APPLE)
 #   set(CPACK_GENERATOR "${CPACK_GENERATOR};Bundle")
  #  set(CPACK_BUNDLE_NAME "KalaNet")
   # set(CPACK_BUNDLE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/app-icon.icns)
    #set(CPACK_BUNDLE_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
#endif()

#include(CPack)

# ========================================
# CUSTOM TARGETS
# ========================================

add_custom_target(run
    COMMAND KalaNetClient
    DEPENDS KalaNetClient
    COMMENT "Running KalaNet Client..."
)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Cleaning all build files..."
)

# ========================================
# PRINT SUMMARY
# ========================================

message(STATUS "========================================")
message(STATUS "KalaNet Client Configuration Summary")
message(STATUS "========================================")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Qt version: ${Qt6_VERSION}")
message(STATUS "Pages: ${PAGE_SOURCES}")
message(STATUS "Dialogs: ${DIALOG_SOURCES}")
message(STATUS "UI files: ${FORMS}")
message(STATUS "========================================")
