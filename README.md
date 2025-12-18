# Getting trolled by Buck2 and Reindeer

This repository demonstrates the challenges of using Rust crates with native dependencies (specifically the `zmq` crate) in Buck2 builds, and provides a working solution.

## The Problem

The `zmq` crate depends on the native ZeroMQ C++ library (`libzmq`). When using Reindeer to convert Cargo dependencies to Buck2, native dependencies often fail because:

- Reindeer doesn't automatically handle native library builds
- The `zmq-sys` crate uses the `zeromq-src` crate, which always builds `libzmq` from source (vendored), but Reindeer struggles to translate this build process to Buck2
- Buck2's build system requires explicit native library definitions

**Specific issue with `zeromq-src`**: While it's possible to define `cxx_library` rules for other native dependencies (e.g., `libmimalloc-sys` works fine), attempting to create fixups for `zeromq-src` results in errors about unused globs.

Reindeer reports that glob patterns in `fixups.toml` (like `vendor/src/**/*.cpp`, `vendor/external/**/*.c`, header files, etc.) match no files, even though these files exist in the vendored source. This suggests Reindeer's glob resolution doesn't work correctly with `zeromq-src`'s vendored source structure, making it impossible to use Reindeer's fixup mechanism for this crate. 

See commit 043aceedcfe3cfbac98bcdbc3a360de1a66b3333 for reference. Apply it and try `just buckify`

As a result, we must manually define the entire ZeroMQ build in Buck2 rather than relying on Reindeer's automatic translation.

## The Solution

This project includes a workaround that manually builds `libzmq` from source using Buck2's build system:

- **`vendor/zeromq/BUCK`**: Defines a complete Buck2 build for ZeroMQ 4.3.4 from source
- **`third-party/`**: Contains Reindeer-generated Buck2 rules for Rust crates

## Building

### Build Steps

1. **Generate Buck2 rules from Cargo dependencies** (if needed):
   ```bash
   just buckify
   ```

2. **Build the project**:
   ```bash
   just build
   ```

## Project Structure

```
.
├── src/                   # Rust source code
├── vendor/zeromq/         # Manual Buck2 build for libzmq
├── third-party/           # Reindeer related files
│   ├── BUCK               # Reindeer-generated Buck2 rules
│   └── fixups/            # Custom fixups for various crates
├── scripts/reindeer       # dotslash reindeer script
├── BUCK                   # Main Buck2 build file
├── reindeer.toml          # Reindeer configuration
└── rust-toolchain.toml    # Rust toolchain version
```

## Why This Matters

This example highlights a common pain point when migrating Rust projects to Buck2: **native dependencies require manual intervention**. While Reindeer handles pure Rust crates well, crates with native dependencies often need:

1. Manual native library builds (like `libzmq` here)
2. Custom fixups to help Reindeer understand the dependency chain
3. Careful configuration of linker flags and include paths

## Tool versions
- Buck2: `buck2 a24056c452b43451b9a34aa5b985fef9 <build-id>`
- Rust: See [rust-toolchain.toml](/rust-toolchain.toml)
- Reindeer: `v2025.11.24.00` (See [scripts/reindeer](/scripts/reindeer))
