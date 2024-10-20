#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include "templates.hpp"

namespace fs = std::filesystem;

namespace ValidCommands {
    const std::array<const char*, 1> COMMANDS = {"new"};
    
    constexpr size_t count() { return COMMANDS.size(); };
}

namespace ValidNewOptions {
    const std::array<const char*, 1> OPTIONS = {"fabric"};

    constexpr size_t count() { return OPTIONS.size(); }
}

bool validateCommand(const char* argument) {
    for(const char* command : ValidCommands::COMMANDS){
        if(std::string(argument) == command) return true;
    }
    
    return false;
}

bool validateNewOption(const char* argument) {
    for(const char* option: ValidNewOptions::OPTIONS) {
        if(std::string(argument) == option) return true;
    }
    
    return false;
}

void printValidNewOptions() {
    std::cout << "Valid options for 'new' command are: ";
    for(size_t i = 0; i < ValidNewOptions::OPTIONS.size(); i++) {
        std::cout << ValidNewOptions::OPTIONS[i];
        if (i < ValidNewOptions::OPTIONS.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << '\n';
}

std::string promptForComponentName() {
    std::string componentName;
    std::cout << "Enter the name for your Fabric component (conventionally prefixed with 'RTN'): ";
    std::getline(std::cin, componentName);

    // Trim leading and trailing whitespace
    componentName.erase(componentName.begin(), std::find_if(componentName.begin(), componentName.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    componentName.erase(std::find_if(componentName.rbegin(), componentName.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), componentName.end());

    return componentName;
}

void createNewFabricProject() {
    std::string componentName = promptForComponentName();
    std::cout << "Creating Fabric component: " << componentName << "\n";

    try {
        // Create main component directory
        fs::path componentDir = fs::current_path() / componentName;
        fs::create_directory(componentDir);

        // Create subdirectories
        fs::create_directory(componentDir / "android");
        fs::create_directory(componentDir / "ios");
        fs::create_directory(componentDir / "js");

        // Create <componentName>NativeComponent.ts file
        fs::path jsFilePath = componentDir / "js" / (componentName + "NativeComponent.ts");
        std::ofstream jsFile(jsFilePath);
        if (jsFile.is_open()) {
            std::unordered_map<std::string, std::string> jsValues = {{"componentName", componentName}};
            jsFile << TemplateEngine::render(Templates::JS_COMPONENT, jsValues);
            jsFile.close();
            std::cout << "Created " << jsFilePath.filename() << "\n";
        } else {
            std::cerr << "Failed to create " << jsFilePath.filename() << "\n";
        }

        // Create package.json file
        fs::path packageJsonPath = componentDir / "package.json";
        std::ofstream packageJsonFile(packageJsonPath);
        if (packageJsonFile.is_open()) {
            std::unordered_map<std::string, std::string> packageJsonValues = {{"componentName", componentName}};
            packageJsonFile << TemplateEngine::render(Templates::PACKAGE_JSON, packageJsonValues);
            packageJsonFile.close();
            std::cout << "Created package.json\n";
        } else {
            std::cerr << "Failed to create package.json\n";
        }

        std::cout << "Successfully created directory structure for " << componentName << ":\n";
        std::cout << componentDir.string() << "\n";
        std::cout << "├── android\n";
        std::cout << "├── ios\n";
        std::cout << "└── js\n";
        std::cout << "    └── " << componentName << "NativeComponent.ts\n";
        std::cout << "    └── package.json\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error creating directory structure: " << e.what() << "\n";
    }
}

void handleNewCommand(const char* argument) {
    if (argument == nullptr) {
        std::cout << "Error: 'new' command requires an option. ";
        printValidNewOptions();
        return;
    }
    
    if (validateNewOption(argument)) {
        std::string option(argument);
        if (option == "fabric") {
            createNewFabricProject();
        } else {
            std::cout << "Given valid option: " << option << "\n";
        }
    } else {
        std::cout << "Invalid option provided: " << argument << '\n';
        printValidNewOptions();
    }
}

int main(int argc, const char* argv[]) {
    // We skip argv[0] since it is the name of the program.
    for(int i = 1; i < argc; i++) {
        if (validateCommand(argv[i])) {
            std::string cmd(argv[i]);
            if (cmd == "new") {
                if (i + 1 < argc) {
                    handleNewCommand(argv[i + 1]);
                    i++; // Skip the next argument since we processed it
                } else {
                    handleNewCommand(nullptr);
                }
            }
        } else {
            std::cout << "Invalid command: " << argv[i] << '\n';
            std::cout << "Valid commands are: ";
            for(const char* option : ValidCommands::COMMANDS) {
                std::cout << option << " ";
            }
            std::cout << '\n';
        }
    }
    
    return 0;
}
