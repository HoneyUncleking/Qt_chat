@echo off
echo ====================================
echo Building KalaNet Project with Qt 6.10.1
echo ====================================
echo.

set PATH=C:\Qt\6.10.1\mingw_64\bin;%PATH%
set QT_PATH=C:\Qt\6.10.1\mingw_64
set CMAKE_PREFIX_PATH=%QT_PATH%

echo Cleaning old builds...
rmdir /s /q build-server 2>nul
rmdir /s /q build-client 2>nul

echo.
echo Building Server...
mkdir build-server
cd build-server

echo Configuring Server...
cmake -G "MinGW Makefiles" ^
  -DCMAKE_C_COMPILER=gcc ^
  -DCMAKE_CXX_COMPILER=g++ ^
  -DCMAKE_MAKE_PROGRAM=mingw32-make ^
  -DQt6_DIR="%QT_PATH%/lib/cmake/Qt6" ^
  ..\Server

if errorlevel 1 (
  echo.
  echo ❌ Server configuration failed!
  cd ..
  goto error
)

echo Building Server...
mingw32-make
if errorlevel 1 (
  echo.
  echo ❌ Server build failed!
  cd ..
  goto error
)
cd ..

echo.
echo Building Client...
mkdir build-client
cd build-client

echo Configuring Client...
cmake -G "MinGW Makefiles" ^
  -DCMAKE_C_COMPILER=gcc ^
  -DCMAKE_CXX_COMPILER=g++ ^
  -DCMAKE_MAKE_PROGRAM=mingw32-make ^
  -DQt6_DIR="%QT_PATH%/lib/cmake/Qt6" ^
  ..\Client

if errorlevel 1 (
  echo.
  echo ❌ Client configuration failed!
  cd ..
  goto error
)

echo Building Client...
mingw32-make
if errorlevel 1 (
  echo.
  echo ❌ Client build failed!
  cd ..
  goto error
)
cd ..

echo.
echo Copying DLLs...
copy %QT_PATH%\bin\Qt6Core.dll build-client\Release\ 2>nul
copy %QT_PATH%\bin\Qt6Widgets.dll build-client\Release\ 2>nul
copy %QT_PATH%\bin\Qt6Gui.dll build-client\Release\ 2>nul
copy %QT_PATH%\bin\Qt6Network.dll build-client\Release\ 2>nul
copy %QT_PATH%\bin\Qt6Sql.dll build-client\Release\ 2>nul
copy %QT_PATH%\bin\libgcc_s_seh-1.dll build-client\Release\ 2>nul
copy %QT_PATH%\bin\libstdc++-6.dll build-client\Release\ 2>nul
copy %QT_PATH%\bin\libwinpthread-1.dll build-client\Release\ 2>nul

echo.
echo ====================================
echo ✅ Build completed successfully!
echo ====================================
echo.
echo To run server:
echo   cd build-server ^&^& Release\KalaNetServer.exe
echo.
echo To run client:
echo   cd build-client\Release ^&^& KalaNetClient.exe
echo.
pause
goto end

:error
echo.
echo ====================================
echo ❌ Build failed!
echo ====================================
pause

:end