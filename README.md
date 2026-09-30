# Zygisk-dl-injector
Use to inject any desired .so to any app specified in the `/data/local/tmp/dlinjector/config.txt`. Just make sure to do this first so the module can load the config properly.
```
mkdir -p /data/local/tmp/dlinjector
touch /data/local/tmp/dlinjector/config.txt
chown -R 0:0 /data/local/tmp/dlinjector
chcon -R u:object_r:system_file:s0 /data/local/tmp/dlinjector
```

The config structure is like this
```
com.DefaultCompany.game : /data/local/tmp/dlinjector/frida.so
<process_name> : <path_to_dl_so>
```

This module is pretty simple just parses the config and directly use dlopen.

## Build
To build you can run the `build.ps1` just make sure to change the ndk of the script.
```
$ndk = 'D:\Binaries\Android\ndk\25.2.9519653\ndk-build.cmd'
```
The output will be located at module.zip

## Vscode Intelisense
Make sure to change the includePath on `.vscode\c_cpp_properties.json`, and specify the path of your ndk.