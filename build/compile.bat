@echo off
setlocal

cd C:/Users/Usuario/Downloads/cmake-4.1.2/cmake-4.1.2/project/build
set "JAVA_HOME=C:\Program Files\Java\jdk-25.0.2\"
set "JAVA_EXE=%JAVA_HOME%\bin\java.exe"
set "JAVA_PATH=C:\Program Files\Java\jdk-25.0.2\bin\java.exe"
set "APKTOOL_JAR=C:\Windows\apktool.jar"

set "SOURCE_LIB=libviolet.so"
set "DEST_LIB=C:\Users\Usuario\Downloads\VioletPS\lib\armeabi-v7a\libviolet.so"
set "APK_PROJECT=C:\Users\Usuario\Downloads\VioletPS"
set "OUTPUT_APK=C:\Users\Usuario\Downloads\VioletPS\dist\VioletPS_signed.apk"

echo [1/5] Running custom compile...

echo [2/5] Running ninja...
ninja

echo [3/5] Waiting for %SOURCE_LIB%...
:WaitLoop
if not exist "%SOURCE_LIB%" (
    timeout /t 2 /nobreak >nul
    goto WaitLoop
)

echo [3/5] Replacing .so file...
copy /y "%SOURCE_LIB%" "%DEST_LIB%"
echo [4/5] Building APK with apktool...
"%JAVA_PATH%" -jar "%APKTOOL_JAR%" b "%APK_PROJECT%"
echo [5/5] Signing APK...
"C:\Users\Usuario\AppData\Local\Android\Sdk\build-tools\36.0.0\apksigner.bat" sign --ks "my-release-key.jks" --ks-pass pass:"violet" --out "%APK_PROJECT%\VioletPS_signed.apk" "%OUTPUT_APK%"

echo Done!
pause
