@ECHO OFF

VERIFY OTHER 2>nul
SETLOCAL ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION
IF NOT ERRORLEVEL 0 (
  echo Unable to enable extensions
)

SET "zigBin=D:\zig\zig.exe"

FOR /F "delims=" %%A IN ('cd') DO SET "ORIGINAL_DIR=%%A"
ECHO orig dir: %ORIGINAL_DIR%

SET "logFolder=.\_build_logs"

GOTO :main

:doCommand
SET "logFile=%logFolder%\%~1.log"
SET "command=%~2"
ECHO %command%>"%ORIGINAL_DIR%\%logFile%" 2>&1
%command%>>"%ORIGINAL_DIR%\%logFile%" 2>&1
EXIT /B %ERRORLEVEL%

:main

RMDIR /S /Q "%ORIGINAL_DIR%\%logFolder%"
RMDIR /S /Q "%ORIGINAL_DIR%\_dest"
RMDIR /S /Q "%ORIGINAL_DIR%\.xmake"
RMDIR /S /Q "%ORIGINAL_DIR%\build"

MKDIR "%ORIGINAL_DIR%\%logFolder%"

CALL :doCommand "00_made_build_logs" "echo we did it" && cd>NUL || Goto :END

REM CALL :doCommand "01_config" "xmake config -vD --plat=windows --arch=x64 --kind=shared --mode=debug --runtimes=MD --yes --policies=package.precompiled:n" && cd>NUL || Goto :END
REM CALL :doCommand "01_config" "xmake config -vD --plat=windows --arch=x64 --kind=shared --mode=debug --runtimes=MT --yes --policies=package.precompiled:n" && cd>NUL || Goto :END
REM CALL :doCommand "01_config" "xmake config -vD --plat=windows --arch=x64 --kind=shared --mode=release --runtimes=MD --yes --policies=package.precompiled:n" && cd>NUL || Goto :END
REM CALL :doCommand "01_config" "xmake config -vD --plat=windows --arch=x64 --kind=shared --mode=release --runtimes=MT --yes --policies=package.precompiled:n" && cd>NUL || Goto :END
REM CALL :doCommand "01_config" "xmake config -vD --plat=windows --arch=x64 --kind=static --mode=debug --runtimes=MD --yes --policies=package.precompiled:n" && cd>NUL || Goto :END
REM CALL :doCommand "01_config" "xmake config -vD --plat=windows --arch=x64 --kind=static --mode=debug --runtimes=MT --yes --policies=package.precompiled:n" && cd>NUL || Goto :END
REM CALL :doCommand "01_config" "xmake config -vD --plat=windows --arch=x64 --kind=static --mode=release --runtimes=MD --yes --policies=package.precompiled:n" && cd>NUL || Goto :END
CALL :doCommand "01_config" "xmake config -vD --plat=windows --arch=x64 --kind=static --mode=release --runtimes=MT --yes --policies=package.precompiled:n" && cd>NUL || Goto :END

CALL :doCommand "02_build" "xmake build -vD -a" && cd>NUL || Goto :END

CALL :doCommand "05_xmake_test_ZmqPb_Tests" "xmake run -vD ZmqPb_Tests" && cd>NUL || Goto :END

:END
cd %ORIGINAL_DIR%
ENDLOCAL
EXIT /B %ERRORLEVEL%
