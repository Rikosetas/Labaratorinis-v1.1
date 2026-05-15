[Setup]
AppName=StudentuRezultatai
AppVersion=3.0
DefaultDirName=C:\Program Files\VU\Aleksas-Vebra
DefaultGroupName=VU\Aleksas-Vebra
OutputDir=installer
OutputBaseFilename=Setup
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin

[Files]
Source: "C:\Users\Alex\Desktop\Labaratorinis-v1.1\x64\Release\ConsoleApplication1.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{autodesktop}\StudentuRezultatai"; Filename: "{app}\ConsoleApplication1.exe"
Name: "{group}\StudentuRezultatai"; Filename: "{app}\ConsoleApplication1.exe"

[Run]
Filename: "{app}\ConsoleApplication1.exe"; Description: "Paleisti programa"; Flags: nowait postinstall skipifsilent runascurrentuser