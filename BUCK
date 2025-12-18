rust_binary(
    name = "buck2-fail-zmq",
    srcs = glob(["src/**/*.rs"]),
    deps = [
        "//third-party:zmq",
        "//vendor/zeromq:zmq",
    ],
    linker_flags = select({
        "config//os:macos": [
            "-lc++",
        ],
        "config//os:linux": [
            "-lstdc++",
        ],
        "DEFAULT": [],
    }),
)
