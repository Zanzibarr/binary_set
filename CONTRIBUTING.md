# Contributing to binary_set.hxx

Thank you for your interest in contributing! This document provides guidelines for contributing to the project.

## How to Contribute

### Reporting Bugs

If you find a bug, please open an issue with:
- A clear, descriptive title
- Steps to reproduce the bug
- Expected vs actual behavior
- Your compiler and C++ standard version
- Minimal code example demonstrating the issue

### Suggesting Enhancements

Enhancement suggestions are welcome! Please open an issue describing:
- The enhancement you'd like to see
- Why it would be useful
- Possible implementation approaches

### Pull Requests

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Ensure code follows the existing style
5. Add tests if applicable
6. Update documentation as needed
7. Commit your changes (`git commit -m 'Add amazing feature'`)
8. Push to your branch (`git push origin feature/amazing-feature`)
9. Open a Pull Request

## Code Style Guidelines

### General Principles
- Follow the existing code style
- Use meaningful variable names
- Add comments for complex logic
- Keep functions focused and concise

### Specific Conventions
- **Indentation**: 4 spaces (no tabs)
- **Naming**:
  - Classes: `snake_case`
  - Methods: `snake_case()`
  - Private members: `name_` (trailing underscore)
  - Constants: `UPPER_CASE`
- **Braces**: Opening brace on same line
- **Documentation**: Doxygen-style comments for public APIs

### Example
```cpp
/**
 * @brief Brief description
 *
 * @param param Description
 * @return Description
 */
bool example_method(unsigned int param) {
    if (param > threshold_) {
        return true;
    }
    return false;
}
```

## Testing

- Add unit tests for new features
- Ensure existing tests pass (run tests: `cmake --build build && ./build/tests/run_tests`)
- Test edge cases and error conditions
- Verify no memory leaks (use valgrind or similar)

## Documentation

- Update README.md for new features
- Add Doxygen comments to public APIs
- Include usage examples for complex features
- Update CHANGELOG.md following the format

## Performance Considerations

- Profile performance-critical changes (run benchmarks: `cmake --build build && ./build/benchmarks/run_benchmarks`)
- Consider both time and space efficiency (see [BENCHMARKS.md](BENCHMARKS.md) for detailed analysis)
- Maintain O(1) time complexity for basic operations
- Document any performance trade-offs

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Questions?

Feel free to open an issue for any questions about contributing!