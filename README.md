# Zygisk-dl-injector
Use to inject any desired .so to any app specified in the `/data/local/tmp/dlinjector/config.txt`. Just make sure to do this first so the module can load the config properly.
```
mkdir -p /data/local/tmp/dlinjector
touch /data/local/tmp/dlinjector/config.txt

# Copy your so to the directory
# cp frida.so /data/local/tmp/dlinjector
# cp libempty.so /data/local/tmp/dlinjector

chown -R 0:0 /data/local/tmp/dlinjector
chcon -R u:object_r:system_file:s0 /data/local/tmp/dlinjector
```

The config structure is like this
```
com.DefaultCompany.game
    dl_path : /data/local/tmp/dlinjector/frida.so
com.DefaultCompany.game
    dl_path : /data/local/tmp/dlinjector/libempty.so
    freeze_loop_wait_time : 500000
<process_name>
    dl_path : <path_to_dl>
    freeze_loop_wait_time : <optional_value_freeze_for_microseconds_loop_until_waiter_should_continue_returns_non_0>
```

This module is pretty simple just parses the config and directly use dlopen.

## Build
To build you can run the `build.ps1` just make sure to change the ndk of the script.
```
$ndk = 'D:\Binaries\Android\ndk\25.2.9519653\ndk-build.cmd'
```
The output will be located at module.zip

There is also an example template of libempty.so on the empty.so folder. To build it just run `empty.so/build.ps1`

## Vscode Intelisense
Make sure to change the includePath on `.vscode\c_cpp_properties.json`, and specify the path of your ndk.