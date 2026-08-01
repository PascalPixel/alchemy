# Alchemy handover

This file is the neutral operational entry point for continuing the
decompilation. It intentionally contains no process assumptions about who or
what performs the work.

## Current state

- `main` is the authoritative integration branch.
- GS1 English Full-C Byte Share is **244,808 / 1,343,206 executable bytes
  (18.23%)**.
- Reviewed semantic C compiles successfully. After exact-C supersession and
  executable-extent accounting, it contributes **821,588 additional bytes**;
  exact and semantic C together cover
  **1,066,396 / 1,343,206 bytes (79.39%)**.
- The main-image ordinary semantic census is closed and enforced by
  `bun run semantic:check`.
- The denominator increased by 204 bytes in this checkpoint: inventory now
  scans compiled exact-C overlay bytes for call targets instead of zero-filled
  placeholders, recovering one 36-byte leaf and 168 bytes of raw call-via
  banks that were always executable.
- The most recent complete verification proved a byte-identical, source-owned
  ROM with zero fallback. Re-run it after any new integration.

The tracked metric files are authoritative when this summary becomes stale:

```sh
bun run progress
bun run coverage:check
```

## Continue decompilation

1. Select an executable owner that is neither exact C nor reviewed semantic C.
2. Establish its complete boundary, literal pools, entry evidence, return
   shape, and direct/indirect calls from the reconstructed bytes.
3. Prefer readable byte-exact C. Use semantic C when exactness is not yet
   attainable, without weakening or bypassing any ownership gate.
4. Run `bun tools/overlay_twins.ts --unconverted` before drafting a fresh
   overlay owner. It compares ordinary assembly against already exact C and
   exposes cross-overlay templates; the corrected scan closed 7,456 bytes in
   the 384- and 404-byte families in one pass.
5. Do not manually permute a previously documented compiler floor. Test an
   established compiler family or bounded automated cohort, record the floor,
   and move on when it does not close.
6. Admit exact C only when the affected linked bytes and whole image reproduce
   exactly. Exact C supersedes semantic C for the same owner.

Useful commands:

```sh
bun tools/overlay_inventory.ts
bun tools/overlay_twins.ts --unconverted
bun tools/overlay_call_targets.ts <resource> <offset>
bun tools/overlay_multiset_check.ts <resource> <offset> <semantic-source>
bun tools/m2c_guard.ts <resource> <offset>
bun run build:semantic
bun run semantic:check
bun run build:claimed
bun run verify
bun tools/dashboard_server.ts
```

The dashboard derives coverage from the live worktree and refreshes when
source or evidence changes. It is an operational view; checked-in metrics and
SVGs remain the authoritative published `main` snapshot.

## Durable references

- [`LAWS.md`](LAWS.md): established compiler and source-shape behavior.
- [`PROVENANCE.md`](PROVENANCE.md): clean-room and publication boundary.
- [`docs/PATH-TO-COMPLETION.md`](docs/PATH-TO-COMPLETION.md): measured backlog.
- [`docs/COMPILER-QUEUE.md`](docs/COMPILER-QUEUE.md): compiler hypotheses and
  bounded floors.
- [`docs/FULL-C-BYTE-SHARE.md`](docs/FULL-C-BYTE-SHARE.md): metric definition.
- [`docs/SOURCE-STYLE.md`](docs/SOURCE-STYLE.md): canonical source style.

## Non-negotiable gates

Do not claim completion from a local function match alone. Before integrating a
coherent change, run the checks proportional to its scope; before publishing a
checkpoint, run the full gate:

```sh
bun run verify
```

The final state must remain byte-identical, source-owned, and free of ROM
fallback. Classification or denominator corrections must be explicit rather
than hidden as apparent coverage gains.
