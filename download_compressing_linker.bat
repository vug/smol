@echo off
setlocal

set URL=https://github.com/runestubbe/Crinkler/releases/download/v2.3/crinkler23.zip
set ZIPFILE=%~dp0crinkler23.zip
set THISFOLDER=%~dp0

PowerShell -Command "Invoke-WebRequest -Uri '%URL%' -OutFile '%ZIPFILE%'"
PowerShell -Command "Expand-Archive -Path '%ZIPFILE%' -DestinationPath '%THISFOLDER%' -Force"
PowerShell -Command "Rename-Item -Path '%THISFOLDER%\crinkler23\Win32\Crinkler.exe' -NewName 'link.exe' -Force"
PowerShell -Command "Rename-Item -Path '%THISFOLDER%\crinkler23\Win64\Crinkler.exe' -NewName 'link.exe' -Force"


endlocal