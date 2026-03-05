# Building ZeroMQ with Buck2 and Reindeer

This repository demonstrates how to build Rust crates with native C++ dependencies (specifically the `zmq` crate / `libzmq`) in Buck2 using Reindeer's fixup mechanism.

## Background

The `zmq` crate depends on `libzmq`, a C++ library. The dependency chain is:

```
zmq (Rust) -> zmq-sys (Rust, FFI bindings) -> zeromq-src (vendored C++ source)
```

Upstream Reindeer's `[[cxx_library]]` fixups only work for crates that have a build script (`build.rs`). Since `zeromq-src` has no build script (it's only a source distribution crate), the fixup globs are never walked and Reindeer reports them as matching no files.

This project uses a [patched Reindeer](https://github.com/ZentriaMC/reindeer) that allows `[[cxx_library]]` fixups for crates without build scripts, making it possible to build `libzmq` entirely through Reindeer fixups — no manual `BUCK` files needed.

## How It Works

### zeromq-src fixup (`third-party/fixups/zeromq-src/fixups.toml`)

Defines a `[[cxx_library]]` that builds `libzmq` from the vendored source:
- Compiles `vendor/src/**/*.cpp` and `vendor/external/sha1/sha1.c`
- Excludes WSS files that require GnuTLS (`wss_engine.cpp`, `wss_address.cpp`)
- Uses a custom `platform.hpp` (in `fixups/zeromq-src/include/`) that detects the target OS via preprocessor macros (`__APPLE__`, `__linux__`, `_WIN32`) instead of requiring per-platform fixup sections

### zmq-sys fixup (`third-party/fixups/zmq-sys/fixups.toml`)

- Disables the build script (`buildscript.run = false`)
- Adds an explicit dependency on the zeromq-src cxx_library via `extra_deps` (needed because `zmq-sys -> zeromq-src` is a build-only dep in Cargo, which gets dropped when the build script is disabled)
- Links the C++ standard library per-platform: `-lc++` on macOS, `-lstdc++` on Linux

### Platform support

`reindeer.toml` defines platforms for cross-compilation:
- `linux-x86_64` (x86_64-unknown-linux-gnu)
- `linux-arm64` (aarch64-unknown-linux-gnu)
- `macos` (x86_64-apple-darwin)
- `macos-arm64` (aarch64-apple-darwin)

## Building

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
├── src/                           # Rust source code
├── third-party/                   # Reindeer related files
│   ├── BUCK                       # Reindeer-generated Buck2 rules
│   └── fixups/                    # Custom fixups for various crates
│       ├── zmq-sys/fixups.toml    # Disable build script, link cxx_library
│       └── zeromq-src/            # cxx_library fixup for libzmq
│           ├── fixups.toml
│           └── include/platform.hpp
├── scripts/reindeer               # DotSlash reindeer binary (patched)
├── BUCK                           # Main Buck2 build file
├── reindeer.toml                  # Reindeer configuration (with platforms)
└── rust-toolchain.toml            # Rust toolchain version
```

## Tool versions
- Buck2: `buck2 a24056c452b43451b9a34aa5b985fef9 <build-id>`
- Rust: See [rust-toolchain.toml](/rust-toolchain.toml)
- Reindeer: `v2026.03.05.00-zentria` ([ZentriaMC/reindeer](https://github.com/ZentriaMC/reindeer), see [scripts/reindeer](/scripts/reindeer))
