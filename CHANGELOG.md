# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-12-08

### Added
- Initial release of binary_set.hxx
- Examples
- Tests
- Benchmarks
- License

### Features
- Space-efficient storage using bit manipulation (1 bit per element)
- O(1) add, remove, and contains operations
- STL-compatible iterators
- Range-checked element access with meaningful exceptions
- Tree-based subset searching in `bs_searcher`
- Header-only library with zero dependencies