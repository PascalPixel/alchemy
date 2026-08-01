> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

# 100-file humanization batch — 2026-07-28

This run scaled the proven batch-and-isolate method from 20 to 100 already
exact C sources. The files were partitioned into six domain groups, checked
individually, and then verified as one aggregate project build.

## Result

- 100 queued sources attempted.
- 92 queued sources retained byte-exact.
- 8 queued sources parked without debugging.
- 8 adjacent ABI cleanups added during review (`08077c10`, `08078414`,
  `08078480`, `080787dc`, `08078a8c`, `08078af8`, `08079cbc`, and
  `08093304`).
- 100 source files and 6 shared headers in the final reviewed patch.
- Claimed code: 1,345 linked, zero failures, 90,664 bytes.
- Source-only ownership: all 8,388,608 bytes, zero unowned bytes.
- Full ROM: byte-identical, zero fallback bytes.
- Aggregate self-tests: green.

The aggregate passed, so no project-level binary split was necessary.

## Parked sources

These source rewrites changed code generation or exposed an interface that
could not yet be made both defined and byte-exact:

- `src/0807a2e4.c`
- `src/0807a350.c`
- `src/08091240.c`
- `src/080b7e7c.c`
- `src/080f94e0.c`
- `src/080f950c.c`
- `src/080fa1fc.c`
- `src/080fa2a0.c`

## Next 100

The next run is pre-partitioned into ten cohorts of ten. Each cohort is a
natural isolation leaf if the aggregate fails.

### L1 — queued transfer runtime

- `src/080037d4.c`
- `src/08003810.c`
- `src/0800383c.c`
- `src/0800387c.c`
- `src/080038bc.c`
- `src/080038fc.c`
- `src/0800393c.c`
- `src/0800397c.c`
- `src/080039bc.c`
- `src/08003a3c.c`

### L2 — script interpreter state

- `src/0800d654.c`
- `src/0800d674.c`
- `src/0800d6a4.c`
- `src/0800d6d8.c`
- `src/0800d710.c`
- `src/0800d760.c`
- `src/0800d780.c`
- `src/0800d7b4.c`
- `src/0800d7e8.c`
- `src/0800d7f8.c`

### L3 — script operand core fields

- `src/0800e220.c`
- `src/0800e24c.c`
- `src/0800e280.c`
- `src/0800e2b0.c`
- `src/0800e2dc.c`
- `src/0800e308.c`
- `src/0800e334.c`
- `src/0800e364.c`
- `src/0800e390.c`
- `src/0800e3bc.c`

### L4 — script operand word fields

- `src/0800e3e8.c`
- `src/0800e414.c`
- `src/0800e440.c`
- `src/0800e46c.c`
- `src/0800e498.c`
- `src/0800e4c4.c`
- `src/0800e4f0.c`
- `src/0800e51c.c`
- `src/0800e548.c`
- `src/0800e574.c`

### L5 — script operand tail fields

- `src/0800e5a0.c`
- `src/0800e5cc.c`
- `src/0800e5f8.c`
- `src/0800e634.c`
- `src/0800e670.c`
- `src/0800e6ac.c`
- `src/0800e6e4.c`
- `src/0800e720.c`
- `src/0800e75c.c`
- `src/0800e798.c`

### H1 — event entry and shared state

- `src/08091750.c`
- `src/0809177c.c`
- `src/08091780.c`
- `src/080917ac.c`
- `src/080917d0.c`
- `src/080917f4.c`
- `src/08091814.c`
- `src/08091858.c`
- `src/080919d8.c`
- `src/08091c1c.c`

### H2 — runtime work state

- `src/08091c44.c`
- `src/08091d84.c`
- `src/08091d94.c`
- `src/08091dc8.c`
- `src/08091df4.c`
- `src/08091e20.c`
- `src/08091e3c.c`
- `src/08091e6c.c`
- `src/08091eb0.c`
- `src/08091fa8.c`

### H3 — object lookup and position

- `src/08091ff0.c`
- `src/0809202c.c`
- `src/08092054.c`
- `src/08092064.c`
- `src/0809207c.c`
- `src/080920a0.c`
- `src/080920c0.c`
- `src/080920e8.c`
- `src/080920fc.c`
- `src/08092128.c`

### H4 — movement and action staging

- `src/08092158.c`
- `src/0809218c.c`
- `src/080921c4.c`
- `src/08092208.c`
- `src/0809228c.c`
- `src/080922c4.c`
- `src/08092304.c`
- `src/0809233c.c`
- `src/080923c4.c`
- `src/080923e4.c`

### H5 — configuration and animation dispatch

- `src/08092454.c`
- `src/080924d4.c`
- `src/080924ec.c`
- `src/08092504.c`
- `src/08092548.c`
- `src/08092560.c`
- `src/0809259c.c`
- `src/080925cc.c`
- `src/080925e0.c`
- `src/08092708.c`

Process the homogeneous operand cohorts first. Keep volatile transfer ordering,
mixed-width script fields, packed-ID signedness, and object write/call order
unchanged. Place `08091df4`, `080923e4`, `08092454`, `080925e0`, and
`08092708` last within their cohorts because their expression ordering is
known to be codegen-sensitive.
