#pragma once

#include <string>
#include <unordered_map>

class TemplateEngine {
public:
    static std::string render(const std::string& templateStr, const std::unordered_map<std::string, std::string>& values) {
        std::string result = templateStr;
        for (const auto& [key, value] : values) {
            std::string placeholder = "{{" + key + "}}";
            size_t pos = result.find(placeholder);
            while (pos != std::string::npos) {
                result.replace(pos, placeholder.length(), value);
                pos = result.find(placeholder, pos + value.length());
            }
        }
        return result;
    }
};

namespace Templates {
const std::string JS_COMPONENT = R"(
import type { ViewProps } from 'react-native';
import type { HostComponent } from 'react-native';
import codegenNativeComponent from 'react-native/Libraries/Utilities/codegenNativeComponent';

export interface {{componentName}}Props extends ViewProps {
  // Add your component props here
}

export type {{componentName}}NativeComponent = HostComponent<{{componentName}}Props>;

export default codegenNativeComponent<{{componentName}}Props>('{{componentName}}') as {{componentName}}NativeComponent;
)";

const std::string PACKAGE_JSON = R"(
{
  "name": "{{componentName}}",
  "version": "0.0.1",
  "description": "{{componentName}} React Native Fabric component",
  "react-native": "js/index",
  "source": "js/index",
  "files": [
    "js",
    "android",
    "ios",
    "{{componentName}}.podspec",
    "!android/build",
    "!ios/build",
    "!**/__tests__",
    "!**/__fixtures__",
    "!**/__mocks__"
  ],
  "keywords": ["react-native", "ios", "android"],
  "repository": "https://github.com/your_github_username/{{componentName}}",
  "author": "Your Name your_email@your_provider.com https://github.com/your_github_handle",
  "license": "MIT",
  "bugs": {
    "url": "https://github.com/your_github_username/{{componentName}}/issues"
  },
  "homepage": "https://github.com/your_github_username/{{componentName}}#readme",
  "devDependencies": {},
  "peerDependencies": {
    "react": "*",
    "react-native": "*"
  },
  "codegenConfig": {
    "name": "{{componentName}}Specs",
    "type": "components",
    "jsSrcsDir": "js"
  }
}
)";


const std::string PODSPEC = R"(
require "json"

package = JSON.parse(File.read(File.join(__dir__, "package.json")))

Pod::Spec.new do |s|
  s.name            = "{{componentName}}"
  s.version         = package["version"]
  s.summary         = package["description"]
  s.description     = package["description"]
  s.homepage        = package["homepage"]
  s.license         = package["license"]
  s.platforms       = { :ios => "11.0" }
  s.author          = package["author"]
  s.source          = { :git => package["repository"], :tag => "#{s.version}" }

  s.source_files    = "ios/**/*.{h,m,mm,swift}"

  install_modules_dependencies(s)
end
)";

const std::string BUILD_GRADLE = R"(
buildscript {
  ext.safeExtGet = {prop, fallback ->
    rootProject.ext.has(prop) ? rootProject.ext.get(prop) : fallback
  }
  repositories {
    google()
    gradlePluginPortal()
  }
  dependencies {
    classpath("com.android.tools.build:gradle:7.3.1")
    classpath("org.jetbrains.kotlin:kotlin-gradle-plugin:1.7.22")
  }
}

apply plugin: 'com.android.library'
apply plugin: 'com.facebook.react'
apply plugin: 'org.jetbrains.kotlin.android'

android {
  compileSdkVersion safeExtGet('compileSdkVersion', 33)
  namespace "com.{{componentName}}"

  defaultConfig {
    minSdkVersion safeExtGet('minSdkVersion', 21)
    targetSdkVersion safeExtGet('targetSdkVersion', 33)
    buildConfigField("boolean", "IS_NEW_ARCHITECTURE_ENABLED", "true")
  }
}

repositories {
  mavenCentral()
  google()
}

dependencies {
  implementation 'com.facebook.react:react-native'
}
)";

const std::string REACT_PACKAGE = R"(
package com.{{componentName}}

import com.facebook.react.ReactPackage
import com.facebook.react.bridge.NativeModule
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.uimanager.ViewManager

class {{componentName}}Package : ReactPackage {
  override fun createViewManagers(reactContext: ReactApplicationContext): List<ViewManager<*, *>> =
    emptyList()

  override fun createNativeModules(reactContext: ReactApplicationContext): List<NativeModule> =
    emptyList()
}
)";
}
