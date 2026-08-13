# Coverage map

`coverage-map` derives the tracked dashboard data, exhaustive owner-target
index, and four README charts from source-controlled ownership evidence. It
does not need ROM bytes, compiler output, or a prior build to calculate exact
and semantic ownership.

Use the repository entry points:

```sh
make coverage        # regenerate the map, targets, SVGs, and README versions
make coverage-check  # report stale tracked coverage without writing
```

The writer updates:

- `metrics/gs1-en-coverage-map.json`;
- `metrics/gs1-en-core-targets.json` and `TARGETS.md`;
- `assets/readme/gs1-en-{core,overlays,images,music}.svg`; and
- the README `DONE` headline and SVG cache keys.

`DONE` means exact C plus the deliberately permanent assembly category. Exact
C alone is the stricter progress metric reported by `make progress`. Semantic
C is drawn separately and never contributes to either claim.

The implementation is native Rust and has a runtime `--self-test`. Its
completed TypeScript-port parity record is archived in
[`../../docs/history/coverage-map-rust-port.md`](../../docs/history/coverage-map-rust-port.md).
