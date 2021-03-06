; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "MouseInput"
#define MyAppVersion "0.9"
#define MyAppPublisher "sixziv"
#define MyAppExeName "MouseInput.exe"
[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{706F4DF2-061E-4D21-A8D5-2AD4CAADD4C0}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog
OutputBaseFilename=mysetup
Compression=lzma
SolidCompression=yes
WizardStyle=modern
ArchitecturesInstallIn64BitMode=x64 arm64 ia64
ArchitecturesAllowed=x64 arm64 ia64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "E:\release\MouseInput.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\release\firstrun"; DestDir: "{app}"; Flags: ignoreversion; Permissions: users-modify
Source: "E:\release\settings.ini"; DestDir: "{app}"; Flags: ignoreversion; Permissions: users-modify
Source: "E:\release\charset.cst"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\release\dictionary.dct"; DestDir: "{app}"; Flags: ignoreversion; Permissions: users-modify
Source: "E:\release\D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\release\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\release\libGLESV2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\release\MouseInput.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\release\opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\release\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\release\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\release\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\release\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\release\data\*"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\release\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\release\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\release\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\release\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\release\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\release\vc_redist.x64.exe"; Flags: dontcopy
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[INI]
Filename: "{app}\settings.ini"; Section: "General"; Key: "charset"; String: "{code:ReplaceSlash}/charset.cst";Flags: uninsdeletesection;
Filename: "{app}\settings.ini"; Section: "General"; Key: "dictionary"; String: "{code:ReplaceSlash}/dictionary.dct";Flags: uninsdeletesection;


[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent


[Code]
function ReplaceSlash(Value: string): string;
begin
  Result := ExpandConstant('{app}');
  StringChangeEx(Result, '\', '/', True);
end;
var Option1Page: TInputOptionWizardPage;
var dw:Cardinal;
var RedistInstalled:boolean;
var ret:Integer;
procedure InitializeWizard();
begin
  if(RegValueExists(HKLM,'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\X64','Installed')) then
  begin
    if(RegQueryDWordValue(HKLM,'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\X64','Installed',dw)) then
    begin
      if(dw=1)then
      begin
        RedistInstalled := true;
      end
      else
      begin
        RedistInstalled := false;
      end
    end
    else
    begin
      RedistInstalled := false;
    end
  end
  else
  begin
    RedistInstalled := false;
  end;
  if(not RedistInstalled)then
  begin
    ExtractTemporaryFile('vc_redist.x64.exe');
    Exec(ExpandConstant('{tmp}')+'\vc_redist.x64.exe','','',SW_SHOW,ewWaitUntilTerminated,ret);
  end;
  Option1Page :=
    CreateInputOptionPage(wpInstalling,
      'Configure',
      'configure the application',
      '', False, False);
  Option1Page.Add('Start with windows');
end;
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssDone then
  begin
    if Option1Page.Values[0] then
    begin
      RegWriteStringValue(HKEY_AUTO, 'Software\Microsoft\Windows\CurrentVersion\Run','MouseInput', ExpandConstant('{app}')+'\MouseInput.exe');
    end;
  end;
end;
