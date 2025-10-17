# Contributing to cppgorating

Thank you for your interest in contributing to cppgorating!

## Project Overview

This is a cross-platform port of the original cppgorating project, developed for client Kovarex to enable building on Windows, Linux, and FreeBSD with a unified CMake build system.

## Development Setup

### Prerequisites
- CMake 3.20+
- C++20 compliant compiler
- Git
- All dependencies listed in `third_party/README.md`

### Setting Up Development Environment

1. **Clone the repository:**
   ```bash
   git clone https://github.com/ehteshamawan1/cppgorating.git
   cd cppgorating
   ```

2. **Setup dependencies:**
   ```bash
   cd third_party
   # Run setup commands from SETUP_INSTRUCTIONS.txt
   cd ..
   ```

3. **Build in Debug mode:**
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Debug
   cmake --build .
   ```

## Code Style

### C++ Style Guidelines
- **Standard:** C++20
- **Naming:**
  - Classes: `PascalCase` (e.g., `MainController`, `HtmlParser`)
  - Functions/Methods: `camelCase` (e.g., `getDbConnection`, `handleRequest`)
  - Variables: `camelCase` (e.g., `httpCode`, `readBuffer`)
  - Constants: `UPPER_SNAKE_CASE` (e.g., `MAX_CONNECTIONS`)
  - Files: `PascalCase.hpp` / `PascalCase.cpp`

- **Formatting:**
  - Indentation: 4 spaces (no tabs)
  - Braces: K&R style (opening brace on same line)
  - Line length: Prefer ≤ 100 characters

- **Comments:**
  - Use `//` for single-line comments
  - Use `/** */` for multi-line documentation
  - Document all public APIs and controllers

### CMake Style
- Use lowercase for commands: `add_executable`, `target_link_libraries`
- Use `${VARIABLE}` syntax for variables
- Comment complex logic
- Organize with clear sections using `#=====` separators

## Adding Features

### Adding a New Controller

1. Create controller file in `src/controllers/`:
   ```cpp
   // src/controllers/NewController.hpp
   #pragma once
   #include <drogon/HttpController.h>
   using namespace drogon;

   class NewController : public drogon::HttpController<NewController>
   {
   public:
       METHOD_LIST_BEGIN
       ADD_METHOD_TO(NewController::handleRequest, "/new-endpoint", Get);
       METHOD_LIST_END

       void handleRequest(const HttpRequestPtr& req,
                         std::function<void(const HttpResponsePtr&)>&& callback);
   };
   ```

2. Include in `src/main.cpp`:
   ```cpp
   #include "controllers/NewController.hpp"
   ```

3. Update `src/CMakeLists.txt`:
   ```cmake
   set(HEADERS
       ...
       controllers/NewController.hpp
   )
   ```

### Adding Tests

1. Create test file in `tests/`:
   ```cpp
   // tests/test_newfeature.cpp
   #include <UnitTest++/UnitTest++.h>

   SUITE(NewFeatureTests)
   {
       TEST(TestCaseName)
       {
           // Test implementation
           CHECK(condition);
       }
   }
   ```

2. Update `tests/CMakeLists.txt`:
   ```cmake
   set(TEST_SOURCES
       ...
       test_newfeature.cpp
   )
   ```

### Adding Dependencies

1. Document in `third_party/README.md`
2. Update `third_party/CMakeLists.txt` with configuration
3. Update root `README.md` prerequisites
4. Test on all platforms (Windows, Linux, FreeBSD if possible)

## Testing

### Running Tests
```bash
cd build
ctest

# Or directly
./bin/cppgorating_tests
```

### Writing Tests
- Use Unittest++ framework
- Organize tests by feature/component
- Test both success and failure cases
- Mock external dependencies when possible

### Test Coverage
- Aim for >80% code coverage for new features
- All bug fixes should include a test case

## Building

### Debug Build
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

### Release Build
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Platform-Specific Builds

**Windows (Visual Studio):**
```bash
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

**Linux (GCC):**
```bash
cmake .. -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

**Linux (Clang):**
```bash
cmake .. -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

## Pull Request Process

1. **Fork** the repository
2. **Create a branch** for your feature:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. **Make your changes** following the code style
4. **Add tests** for new functionality
5. **Build and test** on your platform:
   ```bash
   cmake --build build
   ctest --test-dir build
   ```
6. **Commit your changes** with clear messages:
   ```bash
   git commit -m "Add feature: description of feature"
   ```
7. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```
8. **Create a Pull Request** with:
   - Clear description of changes
   - Testing performed (which platforms)
   - Any breaking changes
   - Related issues (if applicable)

## Commit Messages

Follow conventional commit format:

```
type(scope): subject

body (optional)

footer (optional)
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `chore`: Maintenance tasks
- `build`: Build system changes

**Examples:**
```
feat(controllers): add caching for fetch endpoint

Implements Redis caching for external URL fetches to reduce load

Closes #123
```

```
fix(mysql): handle connection timeout correctly

Previously, connection timeouts would cause crashes.
Now properly catches and logs the error.
```

## Platform Compatibility

When making changes, consider:
- **Windows**: MSVC-specific requirements, static runtime
- **Linux**: GCC/Clang compatibility, shared library paths
- **FreeBSD**: BSD-specific headers, package availability

Test on multiple platforms when possible, or clearly mark platform-specific code:
```cpp
#ifdef _WIN32
    // Windows-specific code
#elif defined(__unix__) || defined(__APPLE__)
    // Unix-like systems
#endif
```

## Documentation

Update documentation when:
- Adding new features (update README.md)
- Changing API (update controller comments)
- Modifying build process (update build docs)
- Adding dependencies (update third_party/README.md)

## Getting Help

- **Build issues**: Check README.md troubleshooting section
- **Dependency issues**: See third_party/README.md
- **Questions**: Open an issue with `[Question]` tag

## Code Review

Pull requests will be reviewed for:
- Code quality and style
- Test coverage
- Documentation
- Cross-platform compatibility
- Performance implications

## License

By contributing, you agree that your contributions will be licensed under the same license as the original project.

## Contact

- **Developer**: M. Ehtesham Amin ([@ehteshamawan1](https://github.com/ehteshamawan1))
- **Original Project**: [Kovarex](https://github.com/kovarex/cppgorating)

---

Thank you for contributing to cppgorating! 🚀
