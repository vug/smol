@echo off
setlocal

set URL=https://github.com/runestubbe/Crinkler/releases/download/v2.3/crinkler23.zip
set ZIPFILE=%~dp0crinkler23.zip
set THISFOLDER=%~dp0

REM PowerShell -Command "Invoke-WebRequest -Uri '%URL%' -OutFile '%ZIPFILE%'"

REM PowerShell -Command "Expand-Archive -Path '%ZIPFILE%' -DestinationPath '%THISFOLDER%' -Force"

REM PowerShell -Command "Copy-Item -Path '%THISFOLDER%crinkler23\Win32\Crinkler.exe' -Destination '%THISFOLDER%' -Force"

PowerShell -Command "Rename-Item -Path 'Crinkler.exe' -NewName 'link.exe' -Force"

endlocal