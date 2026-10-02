Write-Host "[+] Creating libempty.so"
$ndk = 'D:\Binaries\Android\ndk\25.2.9519653\ndk-build.cmd'

& $ndk clean
& $ndk

Write-Host "[+] libempty.so ready"