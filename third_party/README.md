# Third-Party Dependencies

This directory contains all external dependencies for the cppgorating project.

## Currently Included

- **asio/** - ASIO 1.36.0 (Async I/O library) ✓
- **mysql/** - MySQL Connector/C++ (Database connectivity) ✓
- **crow_backup/** - Crow framework (backup, kept for reference)

## Dependencies to Add

The following dependencies need to be cloned to complete the setup:

### Core Dependencies (Required)

1. **Drogon** - Web framework (replaces Crow)
   ```bash
   cd third_party
   git clone --depth 1 https://github.com/drogonframework/drogon.git
   ```

2. **Trantor** - Network library (required by Drogon)
   ```bash
   cd third_party
   git clone --depth 1 https://github.com/an-tao/trantor.git
   ```

3. **jsoncpp** - JSON parsing library
   ```bash
   cd third_party
   git clone --depth 1 https://github.com/open-source-parsers/jsoncpp.git
   ```

4. **OpenSSL** - SSL/TLS support
   - **Windows:** Use vcpkg or download pre-built binaries from https://slproweb.com/products/Win32OpenSSL.html
   - **Linux:** Install via package manager or build from source
     ```bash
     # Ubuntu/Debian
     sudo apt-get install libssl-dev

     # Or build from source
     cd third_party
     git clone --depth 1 https://github.com/openssl/openssl.git
     cd openssl
     ./config --prefix=$PWD/install
     make && make install
     ```

### Additional Features (Required)

5. **libcurl** - HTTP client library
   ```bash
   cd third_party
   git clone --depth 1 https://github.com/curl/curl.git libcurl
   ```

6. **Unittest++** - Unit testing framework
   ```bash
   cd third_party
   git clone --depth 1 https://github.com/unittest-cpp/unittest-cpp.git
   ```

7. **Gumbo Parser** - HTML parsing library
   ```bash
   cd third_party
   git clone --depth 1 https://github.com/google/gumbo-parser.git
   ```

## Setup Instructions

### Quick Setup (All Dependencies)

Run these commands from the project root:

```bash
cd third_party

# Core dependencies
git clone --depth 1 https://github.com/drogonframework/drogon.git
git clone --depth 1 https://github.com/an-tao/trantor.git
git clone --depth 1 https://github.com/open-source-parsers/jsoncpp.git

# Additional features
git clone --depth 1 https://github.com/curl/curl.git libcurl
git clone --depth 1 https://github.com/unittest-cpp/unittest-cpp.git
git clone --depth 1 https://github.com/google/gumbo-parser.git

cd ..
```

### OpenSSL Setup

**Option 1: System OpenSSL (Recommended for Linux/FreeBSD)**
```bash
# Ubuntu/Debian
sudo apt-get install libssl-dev

# FreeBSD
sudo pkg install openssl

# Fedora/RHEL
sudo dnf install openssl-devel
```

**Option 2: vcpkg (Recommended for Windows)**
```bash
vcpkg install openssl:x64-windows-static
```

**Option 3: Build from source**
```bash
cd third_party
git clone --depth 1 https://github.com/openssl/openssl.git
cd openssl
./config no-shared --prefix=$PWD/install
make -j$(nproc)
make install
```

## Directory Structure After Setup

```
third_party/
├── asio/              # ASIO (already included)
├── mysql/             # MySQL Connector (already included)
├── crow_backup/       # Crow backup (kept for reference)
│   └── old_build_system/  # Old Makefile and VS files
├── drogon/            # Drogon framework (clone required)
├── trantor/           # Trantor network library (clone required)
├── jsoncpp/           # JSON parsing (clone required)
├── openssl/           # OpenSSL (optional, or use system)
├── libcurl/           # libcurl HTTP client (clone required)
├── unittest-cpp/      # Unittest++ testing (clone required)
├── gumbo-parser/      # Gumbo HTML parser (clone required)
├── CMakeLists.txt     # CMake configuration
├── README.md          # This file
└── SETUP_INSTRUCTIONS.txt  # Quick setup reference
```

## Notes

- All dependencies are configured for **static linking**
- Clone with `--depth 1` to save disk space
- For production, consider using specific version tags instead of master/main
- Some dependencies may require additional system libraries (pthread, dl, etc.)

## Build Configuration

All dependencies are configured in `third_party/CMakeLists.txt` with static linking options:
- `BUILD_SHARED_LIBS=OFF`
- `BUILD_STATIC_LIBS=ON`
- Platform-specific flags for Windows/Linux/FreeBSD

## Troubleshooting

### Issue: Git clone fails
**Solution:** Check internet connection, or download zip files manually from GitHub

### Issue: OpenSSL not found
**Solution:** Install system OpenSSL or specify path with `-DOPENSSL_ROOT_DIR=/path/to/openssl`

### Issue: MySQL Connector missing
**Solution:** Ensure MySQL Connector/C++ libraries are in `third_party/mysql/lib64/`

## License Information

Each dependency has its own license. Please review:
- Drogon: MIT License
- Trantor: BSD 3-Clause License
- jsoncpp: MIT License or Public Domain
- OpenSSL: Apache License 2.0
- libcurl: MIT/X derivate license
- Unittest++: MIT License
- Gumbo: Apache License 2.0
- ASIO: Boost Software License
- MySQL Connector/C++: GPL v2 with FLOSS exception
