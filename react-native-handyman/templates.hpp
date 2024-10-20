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
}
