# cppgorating - Cross-Platform C++ Rating System

A portable, high-performance C++ web application with MySQL integration, built with the Drogon framework for cross-platform deployment on Windows, Linux, and FreeBSD.

## Overview

cppgorating is a REST API web server that demonstrates:
- **Cross-platform compatibility**: Builds on Windows, Linux, and FreeBSD using a unified CMake build system
- **Static linking**: No external runtime dependencies (all libraries statically linked)
- **Modern C++**: Uses C++20 standard with async I/O
- **Drogon framework**: High-performance, non-blocking web framework
- **MySQL integration**: Database connectivity with MySQL Connector/C++
- **HTTP client**: External URL fetching with libcurl
- **HTML parsing**: Content extraction with Gumbo parser
- **Comprehensive testing**: Unit test suite with Unittest++

## Features

- **GET /** - Returns country count from MySQL database
- **GET /fetch?url=\<url>** - Fetches external HTML content
- **GET /fetch?url=\<url>&parse=true** - Fetches and parses HTML (extracts title, meta tags, links, headings)
- **Static linking** - Single executable with no DLL/.so dependencies
- **Multi-threaded** - Auto-detects optimal thread count for performance
- **Portable** - Same codebase builds on Windows, Linux, and FreeBSD

## Prerequisites

### Build Requirements
- **CMake** 3.20 or higher
- **C++20 compiler**:
  - Windows: MSVC 2022 (Visual Studio 17.0+)
  - Linux: GCC 11+ or Clang 13+
  - FreeBSD: GCC 11+ or Clang 13+
- **OpenSSL** (for HTTPS support)
- **MySQL server** (for runtime database connectivity)

### Dependencies (Included in third_party/)
All dependencies are included in the repository or will be cloned during setup:
- Drogon (web framework)
- Trantor (network library)
- ASIO 1.36.0 (async I/O)
- jsoncpp (JSON parsing)
- MySQL Connector/C++ (database)
- libcurl (HTTP client)
- Gumbo parser (HTML parsing)
- Unittest++ (testing framework)

## Quick Start

### 1. Clone the Repository

```bash
git clone https://github.com/ehteshamawan1/cppgorating.git
cd cppgorating
```

### 2. Setup Dependencies

Navigate to the `third_party` directory and clone required dependencies:

```bash
cd third_party

# Core dependencies (required)
git clone --depth 1 https://github.com/drogonframework/drogon.git
git clone --depth 1 https://github.com/an-tao/trantor.git
git clone --depth 1 https://github.com/open-source-parsers/jsoncpp.git

# Additional features
git clone --depth 1 https://github.com/curl/curl.git libcurl
git clone --depth 1 https://github.com/unittest-cpp/unittest-cpp.git
git clone --depth 1 https://github.com/google/gumbo-parser.git

cd ..
```

**Quick Setup Script:**
See `third_party/SETUP_INSTRUCTIONS.txt` for a complete setup script.

### 3. Install OpenSSL

**Windows (vcpkg):**
```bash
vcpkg install openssl:x64-windows-static
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install libssl-dev
```

**FreeBSD:**
```bash
sudo pkg install openssl
```

### 4. Configure Database

Edit `src/config.hpp` with your MySQL credentials:

```cpp
static AccessPoint db{
    "127.0.0.1",    // address
    "your_user",    // user
    "your_db",      // database
    "your_pass"     // password
};
```

### 5. Build the Project

**Windows (MSVC):**
```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

**Linux / FreeBSD (GCC/Clang):**
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

**Using Clang instead of GCC:**
```bash
cmake .. -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
```

### 6. Run the Application

```bash
# From project root
./bin/cppgorating
```

The server will start on **http://0.0.0.0:18080**

## API Endpoints

### GET /
Returns the count of countries from the database.

**Example:**
```bash
curl http://localhost:18080/
```

**Response:**
```
42 countries
```

### GET /fetch
Fetches content from an external URL.

**Parameters:**
- `url` (required): The URL to fetch
- `parse` (optional): Set to `true` to parse HTML

**Example 1: Fetch HTML**
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

**Example 2: Fetch and Parse HTML**
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
        "meta_keywords": "example, domain",
        "links": ["https://...", "https://..."],
        "headings": ["Example Heading", ...]
    }
}
```

## Running Tests

The project includes a comprehensive test suite using Unittest++.

**Run all tests:**
```bash
cd build
ctest
```

**Or run tests directly:**
```bash
./bin/cppgorating_tests
```

**Run specific test suite:**
```bash
./bin/cppgorating_tests -suite CurlTests
```

**Test suites:**
- **DatabaseTests**: MySQL connection and query tests
- **HttpTests**: HTTP endpoint tests (integration)
- **CurlTests**: libcurl HTTP client tests

## Project Structure

```
cppgorating/
├── CMakeLists.txt                    # Root CMake configuration
├── README.md                          # This file
├── src/
│   ├── CMakeLists.txt                # Source build configuration
│   ├── main.cpp                      # Application entry point
│   ├── config.hpp                    # Database configuration
│   ├── AccessPoint.hpp               # Database connection helper
│   ├── controllers/
│   │   ├── MainController.hpp        # GET / endpoint
│   │   └── FetchController.hpp       # GET /fetch endpoint
│   └── utils/
│       └── HtmlParser.hpp            # Gumbo HTML parser wrapper
├── third_party/
│   ├── CMakeLists.txt                # Dependencies configuration
│   ├── README.md                      # Dependency documentation
│   ├── SETUP_INSTRUCTIONS.txt        # Quick setup guide
│   ├── drogon/                       # Drogon framework (clone required)
│   ├── trantor/                      # Trantor library (clone required)
│   ├── jsoncpp/                      # JSON library (clone required)
│   ├── asio/                         # ASIO (included)
│   ├── mysql/                        # MySQL Connector (included)
│   ├── libcurl/                      # libcurl (clone required)
│   ├── unittest-cpp/                 # Unittest++ (clone required)
│   └── gumbo-parser/                 # Gumbo parser (clone required)
├── tests/
│   ├── CMakeLists.txt                # Test configuration
│   ├── test_main.cpp                 # Test runner
│   ├── test_database.cpp             # Database tests
│   ├── test_http.cpp                 # HTTP endpoint tests
│   └── test_curl.cpp                 # libcurl tests
├── cmake/
│   ├── FindMySQL.cmake               # MySQL finder module
│   └── StaticBuild.cmake             # Static linking configuration
├── bin/                              # Build output (gitignored)
└── build/                            # CMake build directory (gitignored)
```

## Development

### Adding New Source Files

1. Add `.cpp` file to `src/` directory
2. Add `.hpp` header to `src/` (or appropriate subdirectory)
3. Update `src/CMakeLists.txt`:

```cmake
set(SOURCES
    main.cpp
    your_new_file.cpp  # Add here
)
```

### Adding New Controllers

Create a new controller in `src/controllers/YourController.hpp`:

```cpp
#pragma once
#include <drogon/HttpController.h>
using namespace drogon;

class YourController : public drogon::HttpController<YourController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(YourController::handleRequest, "/your-endpoint", Get);
    METHOD_LIST_END

    void handleRequest(const HttpRequestPtr& req,
                      std::function<void(const HttpResponsePtr&)>&& callback);
};
```

Controllers are automatically registered by Drogon. Include the header in `main.cpp`.

### Changing Compilers

**Use Clang instead of GCC (Linux):**
```bash
cmake .. -DCMAKE_CXX_COMPILER=clang++
```

**Use MinGW instead of MSVC (Windows):**
```bash
cmake .. -G "MinGW Makefiles"
```

## Troubleshooting

### Issue: CMake can't find OpenSSL
**Solution:** Specify OpenSSL root directory:
```bash
cmake .. -DOPENSSL_ROOT_DIR=/path/to/openssl
```

### Issue: MySQL Connector not found
**Solution:** Ensure MySQL Connector/C++ libraries are in `third_party/mysql/lib64/`

### Issue: Drogon not found
**Solution:** Clone Drogon into `third_party/drogon`:
```bash
cd third_party
git clone --depth 1 https://github.com/drogonframework/drogon.git
```

### Issue: Build errors on Windows
**Solution:** Ensure you're using Visual Studio 2022 or later with C++20 support

### Issue: Linking errors on Linux
**Solution:** Install required system packages:
```bash
sudo apt-get install libssl-dev uuid-dev zlib1g-dev
```

## FreeBSD Deployment

The client (Kovarex) can build on FreeBSD 64-bit:

```bash
# Clone repository
git clone https://github.com/ehteshamawan1/cppgorating.git
cd cppgorating

# Setup dependencies (same as Linux)
cd third_party
# ... clone dependencies ...
cd ..

# Install OpenSSL
sudo pkg install openssl

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(sysctl -n hw.ncpu)

# Run
cd ../bin
./cppgorating
```

## Performance

- **Multi-threaded**: Automatically uses all available CPU cores
- **Non-blocking I/O**: Async operations via ASIO
- **Static linking**: Faster startup, no dynamic library loading overhead
- **Optimized builds**: Use Release mode for production (`-DCMAKE_BUILD_TYPE=Release`)

## Static Linking

All dependencies are statically linked for maximum portability:

**Verify static linking:**

**Windows:**
```bash
dumpbin /dependents bin\cppgorating.exe
```
Should show only Windows system DLLs.

**Linux/FreeBSD:**
```bash
ldd bin/cppgorating
```
Should show minimal system libraries (libc, libm, libpthread, etc.).

## License

[Specify license - consult original repository]

## Credits

- **Original Project**: [Kovarex](https://github.com/kovarex/cppgorating)
- **Cross-Platform Port**: M. Ehtesham Amin ([@ehteshamawan1](https://github.com/ehteshamawan1))

## Dependencies Licenses

- **Drogon**: MIT License
- **Trantor**: BSD 3-Clause License
- **jsoncpp**: MIT License or Public Domain
- **OpenSSL**: Apache License 2.0
- **libcurl**: MIT/X derivate license
- **Unittest++**: MIT License
- **Gumbo**: Apache License 2.0
- **ASIO**: Boost Software License
- **MySQL Connector/C++**: GPL v2 with FLOSS exception

## Contributing

This is a cross-platform port for the client. For contributions, please contact the developer or create issues/PRs on the forked repository.

## Support

For issues related to:
- **Original functionality**: Contact [Kovarex](https://github.com/kovarex)
- **Cross-platform build system**: Create an issue on this repository
- **FreeBSD-specific issues**: Test locally first, then report with full error logs

---

**Built with** ❤️ **for cross-platform C++ development**
