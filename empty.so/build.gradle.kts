plugins {
    alias(libs.plugins.android.library)
}

android {
    namespace = "com.fajary.zygiskdlinjector.emptyso"
    compileSdk = 37

    defaultConfig {
        minSdk = 26
    }

    externalNativeBuild {
        ndkBuild {
            path("jni/Android.mk")
        }
    }
}
