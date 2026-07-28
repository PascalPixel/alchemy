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
