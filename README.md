# React Native Handyman

React Native Handyman is a small CLI inspired by Callstack's [React Native Builder Bob](https://github.com/callstack/react-native-builder-bob) library. For now, all it does is scaffold the directory structure and initial file contents for [Fabric Native Components](https://github.com/reactwg/react-native-new-architecture/blob/main/docs/fabric-native-components.md). It only does steps 1-3. You'll have to run [step 4 and beyond](https://github.com/reactwg/react-native-new-architecture/blob/main/docs/fabric-native-components.md#4-native-code) yourself.

## Goals

1. Create convenient utilities for building Fabric Native components (and maybe eventually Turbo Modules).
2. For me to learn C++ with a practical project.

## Design Choices

1. The Handyman will not target backwards compatibility. This is mostly a learning tool, not meant for production use cases. I want to focus on learning the future of React Native, rather than building robust tools for all users.
1. Any output from Handyman should be flexible and unopinionated, since the development process for the new architecture is still evolving. If there are choices we can avoid making, we will.

## Installation

Right now, you'll probably have to build from source. I've been building this as a CLI in Xcode. You should be able to clone the repo, open it in Xcode, and build it from there.

Once you have it built, you can run it from the terminal by calling `./path/to/your/build/react-native-handyman`, or you can set up a symlink to use it as a CLI tool with `sudo ln -s path/to/your/build/react-native-handyman /usr/local/bin/handyman`. Other methods like adding it to your PATH are possible, but I haven't tried that out yet.

### Releases Page

I don't have a lot of experience releasing CLI tools in C++, but [I will upload builds to the releases page](https://github.com/coolsoftwaretyler/react-native-handyman/releases) if that's helpful for folks out there.

## Usage

Once you have the CLI available in your terminal, you can create a new Fabric Native Component with the following command:

```sh
react-native-handyman new fabric
```

You will be prompted for the name of the component, and then the CLI will create the necessary files and directories from your current directory.
