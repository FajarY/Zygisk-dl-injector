Write-Host "[+] Creating module.zip"
$ndk = 'D:\Binaries\Android\ndk\25.2.9519653\ndk-build.cmd'
$soname = "libdlinjector.so"

cd module

& $ndk clean
& $ndk

cd ..
if (Test-Path out)
{
    Remove-Item out -Recurse -Force
}
if(Test-Path module.zip)
{
    Remove-Item module.zip -Force
}


New-Item -ItemType Directory out\zygisk -Force

Copy-Item module\libs\arm64-v8a\$soname   out\zygisk\arm64-v8a.so
Copy-Item module\libs\armeabi-v7a\$soname out\zygisk\armeabi-v7a.so
Copy-Item module.prop out\
Compress-Archive -Path out\* -DestinationPath module.zip -Force

Write-Host "[+] module.zip ready"