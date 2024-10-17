plugins {
    alias(libs.plugins.android.application)
    alias(libs.plugins.jetbrains.kotlin.android)
}

val appNameDev = "TalesAR Dev"
val appNameProd = "TalesAR"

/*
The ARCore AAR library contains native shared libraries that are
extracted before building to a temporary directory.
*/
val arCorePath = "${projectDir}/build/arcore-native"
val thirdPartPath = "${rootDir}/third_party"
val natives: Configuration by configurations.creating

android {
    namespace = "com.mistersomov.talesar"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.mistersomov.talesar"
        minSdk = 28
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"

        shaders {
            glslcArgs += "-c"
        }
        externalNativeBuild {
            cmake {
                cppFlags += listOf(
                    "-std=c++17",
                    "-Wall",
                )
                arguments += listOf(
                    "-DANDROID_STL=c++_static",
                    "-DARCORE_LIBPATH=${arCorePath}/jni",
                    "-DARCORE_INCLUDE=${thirdPartPath}/arcore-sdk",
                )
            }
        }
    }

    buildTypes {
        debug {
            applicationIdSuffix = ".dev"
            isMinifyEnabled = false
            isDebuggable = true
            resValue(type = "string", name = "app_name", value = appNameDev)
        }

        release {
            isMinifyEnabled = true
            isShrinkResources = true
            isDebuggable = false
            resValue(type = "string", name = "app_name", value = appNameProd)
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
    kotlinOptions {
        jvmTarget = "1.8"
    }
    buildFeatures {
        prefab = true
    }
    externalNativeBuild {
        cmake {
            path = file("CMakeLists.txt")
            version = "3.22.1"
        }
        ndkVersion = "26.1.10909125"
    }
    tasks.withType<Test> {
        useJUnitPlatform()
    }
}

dependencies {
    // AR core
    implementation(libs.android.ar.core)
    natives(libs.android.ar.core)

    // Timber
    implementation(libs.timber)

    implementation(libs.androidx.core.ktx)
    implementation(libs.androidx.appcompat)
    implementation(libs.material)
    implementation(libs.androidx.games.activity)

    // Test
    testImplementation(libs.junit)
    testImplementation(platform(libs.junit5.bom))
    testImplementation(libs.junit5)
    testImplementation(libs.test.coroutines)
    androidTestImplementation(libs.androidx.junit)
    androidTestImplementation(libs.androidx.espresso.core)
}

val extractNativeLibraryTask by tasks.registering {
    outputs.upToDateWhen { false }
    doFirst {
        configurations.getByName("natives").files.forEach { file ->
            copy {
                from(zipTree(file))
                into(arCorePath)
                include("jni/**/*")
            }
        }
    }
}
tasks.configureEach {
    if ((name.contains("external") || name.contains("CMake"))
        && !name.contains("Clean")
    ) {
        dependsOn(extractNativeLibraryTask)
    }
}