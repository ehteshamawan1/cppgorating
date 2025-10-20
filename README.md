# cppgorating - Cross-Platform C++ Rating System

A portable, high-performance C++ web application with MySQL integration, built with the Drogon framework.

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-3.20%2B-blue.svg)](https://cmake.org/)
[![vcpkg](https://img.shields.io/badge/vcpkg-enabled-brightgreen.svg)](https://vcpkg.io/)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20FreeBSD-lightgrey.svg)](https://github.com/ehteshamawan1/cppgorating)

---

## ⚡ Quick Start

### Prerequisites
- **CMake** 3.20+ ([Download](https://cmake.org/download/))
- **C++20 compiler** (Visual Studio 2022, GCC 11+, or Clang 13+)
- **Git** ([Download](https://git-scm.com/downloads))
- **MySQL Server** (for runtime database connectivity)

### Build (Windows) - 3 Commands!

```batch
REM 1. Install vcpkg (one-time setup)
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
setx VCPKG_ROOT "C:\vcpkg"

REM 2. Clone and build project (reopen terminal first!)
git clone https://github.com/ehteshamawan1/cppgorating.git
cd cppgorating
cmake --preset windows-release
cmake --build build --config Release

REM 3. Run!
.\bin\cppgorating.exe
```

**That's it!** vcpkg automatically handles all dependencies (Drogon, CURL, OpenSSL, etc.)

Server runs on: `http://localhost:18080`

---

## 📋 Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Build Instructions](#build-instructions)
  - [Windows](#windows-build)
  - [Linux](#linux-build)
  - [FreeBSD](#freebsd-build)
- [API Endpoints](#api-endpoints)
- [Configuration](#configuration)
- [Testing](#testing)
- [Troubleshooting](#troubleshooting)
- [Development](#development)

---

## ✨ Features

- **Cross-Platform**: Runs on Windows, Linux, and FreeBSD
- **Static Linking**: No external DLL dependencies
- **Async I/O**: High-performance asynchronous operations with Drogon
- **MySQL Database**: Full MySQL Connector/C++ integration
- **HTTP Client**: Built-in libcurl for external HTTP requests
- **HTML Parsing**: Gumbo parser for extracting data from HTML
- **Comprehensive Tests**: Google Test framework with unit and integration tests
- **Modern C++**: C++20 standard with latest language features

---

## 📁 Project Structure

```
cppgorating/
├── src/                    # Source code
│   ├── main.cpp           # Application entry point
│   ├── config.hpp         # Database configuration
│   ├── AccessPoint.hpp    # Database access point
│   ├── controllers/       # Drogon HTTP controllers
│   │   ├── MainController.hpp
│   │   └── FetchController.hpp
│   └── utils/             # Utility classes
│       └── HtmlParser.hpp
├── tests/                  # Unit tests (Google Test)
├── third_party/           # Legacy dependencies (ASIO, MySQL)
├── bin/                   # Build output
├── vcpkg.json            # vcpkg dependency manifest
├── CMakeLists.txt        # Root CMake configuration
├── CMakePresets.json     # CMake presets for easy building
├── VCPKG_SETUP.md        # Detailed vcpkg setup guide
└── README.md             # This file
```

---

## 🔨 Build Instructions

### Windows Build

#### Option 1: Using CMake Presets (Recommended)

```batch
REM Prerequisites: Install vcpkg first (see VCPKG_SETUP.md for details)
REM Set VCPKG_ROOT environment variable, then reopen terminal

REM Clone repository
git clone https://github.com/ehteshamawan1/cppgorating.git
cd cppgorating

REM Configure (vcpkg installs dependencies automatically)
cmake --preset windows-release

REM Build
cmake --build build --config Release

REM Run
.\bin\cppgorating.exe
```

#### Option 2: Manual CMake Configuration

```batch
cmake -B build -S . ^
    -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake ^
    -DVCPKG_TARGET_TRIPLET=x64-windows-static ^
    -G "Visual Studio 17 2022" ^
    -A x64

cmake --build build --config Release
```

### Linux Build

```bash
# Install vcpkg (one-time setup)
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
export VCPKG_ROOT=~/vcpkg
export PATH=$VCPKG_ROOT:$PATH

# Add to ~/.bashrc or ~/.zshrc for persistence
echo 'export VCPKG_ROOT=~/vcpkg' >> ~/.bashrc
echo 'export PATH=$VCPKG_ROOT:$PATH' >> ~/.bashrc

# Clone and build
git clone https://github.com/ehteshamawan1/cppgorating.git
cd cppgorating
cmake --preset linux-release
cmake --build build -j$(nproc)

# Run
./bin/cppgorating
```

### FreeBSD Build

```bash
# Install system dependencies
sudo pkg install cmake ninja git

# Install vcpkg
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
export VCPKG_ROOT=~/vcpkg

# Build project
git clone https://github.com/ehteshamawan1/cppgorating.git
cd cppgorating
cmake --preset linux-release  # FreeBSD uses linux preset
cmake --build build -j$(sysctl -n hw.ncpu)

# Run
./bin/cppgorating
```

**For detailed setup instructions, see [VCPKG_SETUP.md](VCPKG_SETUP.md)**

---

## 🚀 API Endpoints

### GET `/`
Returns the count of countries from the MySQL database.

**Response:**
```json
{
  "countries": 42
}
```

### GET `/fetch?url=<url>`
Fetches HTML content from an external URL using libcurl.

**Parameters:**
- `url` (required): The URL to fetch

**Example:**
```bash
curl "http://localhost:18080/fetch?url=https://example.com"
```

**Response:**
```json
{
  "url": "https://example.com",
  "status_code": 200,
  "html": "<!DOCTYPE html>..."
}
```

### GET `/fetch?url=<url>&parse=true`
Fetches and parses HTML content, extracting title, meta tags, and links.

**Example:**
```bash
curl "http://localhost:18080/fetch?url=https://example.com&parse=true"
```

**Response:**
```json
{
  "url": "https://example.com",
  "status_code": 200,
  "html": "<!DOCTYPE html>...",
  "parsed": {
    "title": "Example Domain",
    "meta_description": "Example description",
    "meta_keywords": "example, test",
    "links": ["https://www.iana.org/domains/example"],
    "headings": ["Example Domain"]
  }
}
```

---

## ⚙️ Configuration

### Database Configuration

Edit `src/config.hpp` to configure your MySQL connection:

```cpp
static AccessPoint db{
    "127.0.0.1",      // MySQL server address
    "your_username",  // Database username
    "your_database",  // Database name
    "your_password"   // Database password
};
```

### Port Configuration

The server runs on port **18080** by default. To change it, edit `src/main.cpp`:

```cpp
drogon::app().addListener("0.0.0.0", 18080);  // Change port here
```

---

## 🧪 Testing

### Run All Tests

```batch
REM Windows
cd build
ctest -C Release --output-on-failure

REM Linux/FreeBSD
cd build
ctest --output-on-failure
```

### Run Test Executable Directly

```batch
REM Windows
.\bin\cppgorating_tests.exe

REM Linux/FreeBSD
./bin/cppgorating_tests
```

### Run Specific Test Suite

```batch
REM Google Test filters
.\bin\cppgorating_tests.exe --gtest_filter=DatabaseTests.*
.\bin\cppgorating_tests.exe --gtest_filter=CurlTests.HttpGetRequest
```

### List All Tests

```batch
.\bin\cppgorating_tests.exe --gtest_list_tests
```

---

## 🔧 Troubleshooting

### vcpkg not found
```batch
REM Ensure VCPKG_ROOT is set
echo %VCPKG_ROOT%

REM Should show: C:\vcpkg
REM If not, set it:
setx VCPKG_ROOT "C:\vcpkg"

REM IMPORTANT: Reopen your terminal after setting environment variables
```

### Dependencies fail to install
```batch
REM Check vcpkg.json exists in project root
REM Check internet connection
REM Try manual install:
C:\vcpkg\vcpkg install drogon:x64-windows-static
```

### MySQL connection fails
1. Ensure MySQL server is running
2. Check credentials in `src/config.hpp`
3. Verify database and tables exist:
   ```sql
   CREATE DATABASE IF NOT EXISTS your_database;
   USE your_database;
   CREATE TABLE IF NOT EXISTS country (id INT PRIMARY KEY, name VARCHAR(255));
   INSERT INTO country VALUES (1, 'Example Country');
   ```

### Build is slow on first run
This is normal! vcpkg is building all dependencies from source (~10-30 minutes). Subsequent builds will be much faster.

For more troubleshooting, see [VCPKG_SETUP.md](VCPKG_SETUP.md)

---

## 👨‍💻 Development

### Adding New Dependencies

1. Search vcpkg for the package:
   ```batch
   C:\vcpkg\vcpkg search <package-name>
   ```

2. Add to `vcpkg.json`:
   ```json
   {
     "dependencies": [
       "existing-package",
       "new-package"
     ]
   }
   ```

3. Add to `CMakeLists.txt`:
   ```cmake
   find_package(NewPackage CONFIG REQUIRED)
   target_link_libraries(cppgorating PRIVATE NewPackage::NewPackage)
   ```

4. Reconfigure CMake - vcpkg will automatically install the new dependency

### Adding New Source Files

1. Create your `.cpp` and `.hpp` files in `src/` or subdirectories
2. Update `src/CMakeLists.txt`:
   ```cmake
   set(SOURCES
       main.cpp
       your_new_file.cpp
   )
   ```

### Adding New Controllers

Create a new controller in `src/controllers/`:

```cpp
#pragma once
#include <drogon/HttpController.h>

namespace api {

class YourController : public drogon::HttpController<YourController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(YourController::handleRequest, "/your-endpoint", drogon::Get);
    METHOD_LIST_END

    void handleRequest(const drogon::HttpRequestPtr& req,
                      std::function<void(const drogon::HttpResponsePtr&)>&& callback);
};

}
```

---

## 📦 Dependencies (Managed by vcpkg)

All dependencies are automatically installed and managed by vcpkg:

| Library | Version | Purpose |
|---------|---------|---------|
| Drogon | ≥1.9.0 | Web framework |
| Trantor | (auto) | Network library (Drogon dependency) |
| jsoncpp | (auto) | JSON parsing (Drogon dependency) |
| c-ares | (auto) | DNS resolution (Drogon dependency) |
| OpenSSL | 3.x | SSL/TLS support |
| CURL | ≥8.0.0 | HTTP client |
| Gumbo | ≥0.10.0 | HTML parser |
| Google Test | ≥1.14.0 | Testing framework |
| zlib | (auto) | Compression (transitive dependency) |
| brotli | (auto) | Compression (transitive dependency) |

### Legacy Dependencies (Not managed by vcpkg)
- **ASIO** 1.36.0 - Header-only library (included in `third_party/asio`)
- **MySQL Connector/C++** - Optional custom version (in `third_party/mysql`)

---

## 📄 License

[Specify your license here]

---

## 🙏 Credits

**Original Project:** [kovarex/cppgorating](https://github.com/kovarex/cppgorating)

**Cross-Platform Port:** M. Ehtesham Amin ([@ehteshamawan1](https://github.com/ehteshamawan1))

**Frameworks & Libraries:**
- [Drogon](https://github.com/drogonframework/drogon) - High-performance HTTP framework
- [vcpkg](https://github.com/microsoft/vcpkg) - C++ package manager by Microsoft
- [MySQL Connector/C++](https://dev.mysql.com/doc/connector-cpp/8.0/en/) - MySQL database driver
- [libcurl](https://curl.se/libcurl/) - HTTP client library
- [Google Test](https://github.com/google/googletest) - Testing framework

---

## 📞 Support

- **Issues:** [GitHub Issues](https://github.com/ehteshamawan1/cppgorating/issues)
- **Documentation:** [VCPKG_SETUP.md](VCPKG_SETUP.md)
- **vcpkg Help:** [vcpkg.io](https://vcpkg.io/)

---

## 🔄 Migration Notes

If you're upgrading from the manual dependency setup:

1. Delete old dependency folders:
   ```batch
   rmdir /s /q third_party\drogon
   rmdir /s /q third_party\trantor
   rmdir /s /q third_party\jsoncpp
   rmdir /s /q third_party\libcurl
   rmdir /s /q third_party\gumbo-parser
   rmdir /s /q third_party\unittest-cpp
   ```

2. Follow the [Quick Start](#quick-start) instructions above

3. Everything else is automatic!

---

Made with ❤️ using modern C++ and vcpkg
