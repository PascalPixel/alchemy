# alchemy-gcc

`alchemy-gcc/` is the local compiler forge for this reconstruction: releasing
the approved code-generation alchemy into the build while keeping compiler
binaries and third-party environments out of the published source tree.

The ignored directory has one boundary and three roles:

```text
alchemy-gcc/
├── xgcc
├── cc1
├── cpp
├── tradcpp
└── m2c-venv/          optional decompilation helper
```

The four top-level executables are the complete compiler required by the
byte-verified C build. They are native arm64 Mach-O programs and depend only on
macOS `libSystem`; compiler source trees, intermediate objects, nested Git
repositories, duplicate drivers, and target runtime archives do not belong in
the bundle. Native `arm-none-eabi` binutils remain a host dependency supplied
through `PATH`, because they are shared generic assembler/linker tools rather
than part of the approved compiler's code generation.

`bun tools/alchemy_gcc.ts` checks the host architecture, executable set,
and approved SHA-256 digests before compiler use. Every build and matching tool
constructs its compiler invocation through that module; no second compiler
path is permitted.

The directory stays ignored. Public commits contain only this specification,
the validator, reconstructed source, and generic tooling—not compiler binaries
or third-party checkouts.
