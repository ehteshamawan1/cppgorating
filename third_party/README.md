# Third-Party Dependencies

## ⚠️ IMPORTANT: Dependencies are now managed by vcpkg!

As of version 2.0, this project uses **vcpkg** for dependency management. You no longer need to manually clone dependencies into this directory.

## What is vcpkg?

vcpkg is Microsoft's official C++ package manager that handles all dependencies automatically, including:
- Downloading source code
- Building libraries
- Linking
- Handling transitive dependencies (dependencies of dependencies)

## Dependencies Managed by vcpkg

The following dependencies are automatically installed and managed by vcpkg (see `vcpkg.json` in the root directory):

- **Drogon** - Web framework (includes Trantor, jsoncpp, c-ares automatically)
- **CURL** - HTTP client library
- **Gumbo** - HTML parser
- **Google Test** - Testing framework
- **MySQL Connector C++** - Database connector (optional, can use custom version below)

## Legacy Dependencies (Kept in this directory)

### ASIO (Header-Only)
- Location: `third_party/asio/`
- Status: ✅ **Kept** (header-only library, no building required)
- Purpose: Async I/O operations
- No action needed

### MySQL Connector/C++ (Optional Custom Version)
- Location: `third_party/mysql/`
- Status: ⚠️ **Optional** - You can use either:
  1. vcpkg version (recommended): Automatically installed
  2. Custom version: Place libraries in `third_party/mysql/lib64/`
- Purpose: MySQL database connectivity

## Setup Instructions

### Option 1: Using vcpkg (Recommended)

1. **Install vcpkg** (if not already installed):
   ```batch
   REM Windows
   git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
   cd C:\vcpkg
   bootstrap-vcpkg.bat

   REM Set environment variable
   setx VCPKG_ROOT "C:\vcpkg"
   ```

2. **Configure the project** (vcpkg will automatically install dependencies):
   ```batch
   REM Windows (Visual Studio)
   cmake --preset windows-release

   REM Or manually specify vcpkg
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static
   ```

3. **Build**:
   ```batch
   cmake --build build --config Release
   ```

That's it! vcpkg handles everything else automatically.

### Option 2: Manual Setup (Not Recommended)

If you cannot use vcpkg for some reason, you would need to manually install all dependencies. This is **NOT recommended** due to:
- Complex dependency chains (Drogon needs Trantor, jsoncpp, c-ares, zlib, brotli, OpenSSL...)
- Platform-specific build issues
- Version compatibility problems
- Much longer setup time

## What Happened to the Old Dependencies?

The following directories are **no longer needed** and can be removed:
- `third_party/drogon/`
- `third_party/trantor/`
- `third_party/jsoncpp/`
- `third_party/libcurl/`
- `third_party/gumbo-parser/`
- `third_party/unittest-cpp/`
- `third_party/crow_backup/`

vcpkg manages all of these automatically now.

## Troubleshooting

### "vcpkg not found"
- Make sure `VCPKG_ROOT` environment variable is set
- Or use CMakePresets.json (already configured)

### "Dependencies not installing"
- Check `vcpkg.json` in project root
- Run `vcpkg install` manually in vcpkg directory
- Ensure internet connection is available

### "Build fails with missing libraries"
- Ensure you're using the correct CMake preset: `--preset windows-release`
- Check that vcpkg toolchain is being used
- Verify `VCPKG_TARGET_TRIPLET` is set to `x64-windows-static`

## For Developers

If you need to add a new dependency:

1. Search for it in vcpkg:
   ```batch
   vcpkg search <package-name>
   ```

2. Add it to `vcpkg.json` in the project root:
   ```json
   {
     "dependencies": [
       "existing-package",
       "new-package"
     ]
   }
   ```

3. Add `find_package()` in CMakeLists.txt:
   ```cmake
   find_package(NewPackage CONFIG REQUIRED)
   target_link_libraries(cppgorating PRIVATE NewPackage::NewPackage)
   ```

4. Reconfigure CMake - vcpkg will automatically install the new dependency

## More Information

- vcpkg Documentation: https://vcpkg.io/
- vcpkg GitHub: https://github.com/microsoft/vcpkg
- Project vcpkg.json: See root directory
- CMakePresets.json: Configured for automatic vcpkg integration
