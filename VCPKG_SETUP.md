# vcpkg Setup Guide for cppgorating

## Overview

This project now uses **vcpkg** - Microsoft's official C++ package manager - to handle all dependencies automatically. This eliminates the need to manually clone and build dependencies.

## What Changed?

### Before (Manual Dependency Management)
- ❌ Manually clone 6+ repositories
- ❌ Configure build settings for each
- ❌ Deal with missing transitive dependencies (c-ares, zlib, brotli, etc.)
- ❌ Hours of troubleshooting build errors

### After (vcpkg)
- ✅ vcpkg automatically handles ALL dependencies
- ✅ Includes ALL transitive dependencies
- ✅ Pre-built binaries (much faster)
- ✅ Works reliably across different systems

## Step-by-Step Setup (Windows)

### Step 1: Install vcpkg (~5 minutes)

Open PowerShell or Command Prompt as **Administrator**:

```batch
REM 1. Clone vcpkg to C:\vcpkg
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg

REM 2. Navigate to vcpkg directory
cd C:\vcpkg

REM 3. Bootstrap vcpkg (builds the vcpkg tool)
bootstrap-vcpkg.bat

REM 4. Set environment variable (makes vcpkg available everywhere)
setx VCPKG_ROOT "C:\vcpkg"
```

**Important:** Close and reopen your terminal after setting `VCPKG_ROOT` for the change to take effect.

### Step 2: Verify vcpkg Installation

```batch
REM Check vcpkg version
C:\vcpkg\vcpkg.exe version

REM Should output something like: vcpkg package management program version...
```

### Step 3: Clone/Update the cppgorating Repository

```batch
REM If you haven't cloned yet
git clone https://github.com/ehteshamawan1/cppgorating.git
cd cppgorating
git checkout crossplatform-setup

REM If you already have it, pull the latest changes
cd cppgorating
git pull origin crossplatform-setup
```

### Step 4: Configure the Project

Open PowerShell/CMD in the cppgorating directory:

```batch
REM Option 1: Using CMake Presets (Recommended)
cmake --preset windows-release

REM Option 2: Manual configuration
cmake -B build -S . ^
    -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake ^
    -DVCPKG_TARGET_TRIPLET=x64-windows-static ^
    -G "Visual Studio 17 2022" ^
    -A x64
```

**What happens during configuration:**
- vcpkg reads `vcpkg.json` to see what dependencies are needed
- vcpkg automatically downloads and builds:
  - Drogon (web framework)
  - Trantor (network library)
  - jsoncpp (JSON parser)
  - c-ares (DNS library)
  - OpenSSL (SSL/TLS)
  - curl (HTTP client)
  - zlib (compression)
  - brotli (compression)
  - Gumbo (HTML parser)
  - Google Test (testing framework)
- This may take 10-30 minutes on first run (subsequent builds are much faster)
- vcpkg caches everything, so you only build once

### Step 5: Build the Project

```batch
REM Build Release version
cmake --build build --config Release

REM Or build Debug version
cmake --build build --config Debug
```

### Step 6: Run the Application

```batch
REM Configure your database in src/config.hpp first (if needed)
REM Then run the application
.\bin\cppgorating.exe
```

The server will start on `http://localhost:18080`

### Step 7: Run Tests (Optional)

```batch
REM Run all tests
cd build
ctest -C Release

REM Or run the test executable directly
..\bin\cppgorating_tests.exe
```

## Troubleshooting

### Error: "cmake: command not found"
**Solution:** Install CMake from https://cmake.org/download/ or use Visual Studio's CMake

### Error: "Could not find vcpkg"
**Solution:**
1. Make sure `VCPKG_ROOT` environment variable is set:
   ```batch
   echo %VCPKG_ROOT%
   ```
2. Should output: `C:\vcpkg`
3. If not set, run:
   ```batch
   setx VCPKG_ROOT "C:\vcpkg"
   ```
4. **Close and reopen your terminal**

### Error: "Git is not recognized"
**Solution:** Install Git from https://git-scm.com/download/win

### Error: vcpkg fails to download packages
**Solution:**
1. Check your internet connection
2. Check if your firewall/antivirus is blocking vcpkg
3. Try running PowerShell/CMD as Administrator
4. If behind a corporate proxy, configure vcpkg proxy settings

### Error: "MSBuild is not recognized" or Visual Studio not found
**Solution:**
1. Install Visual Studio 2022 (Community Edition is free)
2. During installation, select "Desktop development with C++"
3. Make sure "CMake tools for Windows" is selected

### Build is very slow on first run
**Expected:** vcpkg is building all dependencies from source (can take 10-30 minutes)
**Solution:** Be patient. Subsequent builds will be much faster (< 1 minute)

### MySQL Connector issues
**Solution:** The project can use either:
1. vcpkg's mysql-connector-cpp (automatic)
2. Your custom MySQL Connector in `third_party/mysql/`

If you have a custom connector, it will be used automatically.

## For Linux/FreeBSD Users

### Step 1: Install vcpkg

```bash
# Clone vcpkg
git clone https://github.com/microsoft/vcpkg.git $HOME/vcpkg
cd $HOME/vcpkg

# Bootstrap vcpkg
./bootstrap-vcpkg.sh

# Add to PATH (add this to your .bashrc or .zshrc)
export VCPKG_ROOT=$HOME/vcpkg
export PATH=$VCPKG_ROOT:$PATH
```

### Step 2: Install System Dependencies

```bash
# FreeBSD
sudo pkg install cmake ninja

# Ubuntu/Debian
sudo apt-get update
sudo apt-get install cmake ninja-build

# Fedora/RHEL
sudo dnf install cmake ninja-build
```

### Step 3: Build

```bash
# Clone repository
git clone https://github.com/ehteshamawan1/cppgorating.git
cd cppgorating

# Configure
cmake --preset linux-release

# Build
cmake --build build

# Run
./bin/cppgorating
```

## Advanced Usage

### Clean Rebuild

```batch
REM Remove build directory
rmdir /s /q build

REM Reconfigure
cmake --preset windows-release

REM Rebuild
cmake --build build --config Release
```

### Building Debug Version

```batch
cmake --preset windows-debug
cmake --build build --config Debug
```

### Using Different Compilers

vcpkg works with:
- Visual Studio 2017, 2019, 2022
- MinGW-w64
- Clang
- GCC

Just specify the generator in CMake:
```batch
cmake -B build -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake -G "MinGW Makefiles"
```

## What Gets Installed?

When you configure the project, vcpkg installs these packages automatically:

| Package | Purpose | Size (approx) |
|---------|---------|---------------|
| drogon | Web framework | ~50 MB |
| trantor | Network library | ~5 MB |
| jsoncpp | JSON parsing | ~3 MB |
| c-ares | DNS resolution | ~2 MB |
| openssl | SSL/TLS | ~20 MB |
| curl | HTTP client | ~5 MB |
| zlib | Compression | ~2 MB |
| brotli | Compression | ~3 MB |
| gumbo | HTML parser | ~2 MB |
| gtest | Testing | ~5 MB |

**Total:** ~100 MB (cached for future projects)

## Frequently Asked Questions

### Q: Do I still need to clone dependencies into third_party/?
**A:** No! vcpkg handles everything. The only exceptions are:
- ASIO (already included, header-only)
- MySQL Connector (optional custom version)

### Q: Where are the dependencies stored?
**A:** In `C:\vcpkg\installed\x64-windows-static\`

### Q: Can I use vcpkg for other projects?
**A:** Yes! vcpkg is a general-purpose C++ package manager for any project.

### Q: What if I want to update dependencies?
**A:**
```batch
cd C:\vcpkg
git pull
vcpkg upgrade --no-dry-run
```

### Q: Can I use a different vcpkg location?
**A:** Yes, just change `VCPKG_ROOT` and update CMakePresets.json

### Q: Does this work on FreeBSD?
**A:** Yes! vcpkg supports FreeBSD. Just follow the Linux instructions.

## Support

If you encounter issues:

1. Check this troubleshooting guide
2. Check vcpkg documentation: https://vcpkg.io/
3. Check project issues: https://github.com/ehteshamawan1/cppgorating/issues

## Summary: Quick Commands

```batch
REM One-time setup
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
bootstrap-vcpkg.bat
setx VCPKG_ROOT "C:\vcpkg"

REM Build project (after reopening terminal)
git clone https://github.com/ehteshamawan1/cppgorating.git
cd cppgorating
cmake --preset windows-release
cmake --build build --config Release
.\bin\cppgorating.exe
```

That's it! The entire setup should take about 10-15 minutes (including vcpkg building dependencies).
