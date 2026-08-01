# Executable metric inventories

These target-scoped inventories are generated and checked by
`tools/full_c_progress.ts`. They contain only addresses, interval
classifications, sizes, and clean-room evidence references—never ROM bytes or
disassembly output.

Regenerate an inventory after changing executable classification:

```sh
bun tools/full_c_progress.ts --target gs1-en --write-inventory
```

An inventory with `audit: "incomplete"` deliberately prevents publication of
Full-C Byte Share for that target.

## Coverage map

`<target>-coverage-map.json` holds the tile data behind the README treemap:
ROM areas, executable areas, and the byte split of every tile between
byte-exact C, semantic C, assembly, and asset data. `tools/coverage_map.ts`
derives it from tracked evidence only—the executable inventory, `src/`,
`asm/`, `assets/code/`, `assets/manifest.json`, and the tracked `semantic/`
sources—so a fresh clone can regenerate it without a ROM, a toolchain, or a build
output. Its derived exact-C totals must equal the tracked Full-C report or the
tool refuses to write.

```sh
bun run coverage        # rewrite the map and assets/readme/<target>-coverage.svg
bun run coverage:check  # fail if tracked coverage has moved past the map
```
