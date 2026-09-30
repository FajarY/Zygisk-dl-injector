#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <android/log.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>

#include "zygisk.hpp"
#include "log.hpp"
#include "utils.hpp"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

#define CONFIG_PATH "/data/local/tmp/dlinjector/config.txt"

class MainModule : public zygisk::ModuleBase {
public:
    Api *api;
    JNIEnv *env;
    linked_list* dl_to_load = NULL;
    
    void onLoad(Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
    }

    void postAppSpecialize(const AppSpecializeArgs *args) override {
        if(access(CONFIG_PATH, F_OK) == 0)
        {
            LOGI("Config detected, checking wether to inject or not");

            const char *process = env->GetStringUTFChars(args->nice_name, nullptr);
            
            LOGI("Process name is %s", process);

            char* config = read_file(CONFIG_PATH);
            if(config != NULL)
            {
                int dl_count = search_parse_config(config, process, &dl_to_load);

                LOGI("Detected %d dl(s) to load", dl_count);

                free(config);

                if(dl_count == 0)
                {
                    exit_module();
                }
            }
            else
            {
                LOGI("Failed when reading config");
            }
            
            env->ReleaseStringUTFChars(args->nice_name, process);
        }
        else
        {
            LOGI("Config is not detected on %s", CONFIG_PATH);
            LOGE("access(%s) failed: %s", CONFIG_PATH, strerror(errno));

            exit_module();
        }

        linked_list* current = dl_to_load;
        while (current != NULL)
        {
            char* dl_path = (char*)current->value;

            LOGI("Loading %s", dl_path);

            void* handle = dlopen(dl_path, RTLD_NOW);

            if(handle == NULL)
            {
                LOGI("Failed when loading %s with error %s", dl_path, dlerror());
            }
            else
            {
                LOGI("Loaded %s with handle %p", dl_path, handle);
            }

            current = current->next;
        }

        free_linked_list_and_its_value(&dl_to_load);
    }

private:
    void exit_module()
    {
        api->setOption(zygisk::DLCLOSE_MODULE_LIBRARY);
    }
};

REGISTER_ZYGISK_MODULE(MainModule)