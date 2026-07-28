# Alchemy handover

Updated: 2026-07-28

This is the single authoritative session handover. Do not add dated handoff
files; update this file in place.

## Current objective

Reach **152,000 exact-C bytes**, prioritizing overlay code.

The game is not fully decompiled. Exact means fully linked machine-code byte
equality, not semantic similarity or equal object size.

## Repository state

- Branch: `main`
- Push target: `origin/main`
- Live Full-C metric after the pending overlay batch:
  **115,724 / 1,338,306 executable bytes**
- Main exact C: **95,652 bytes**
- Overlay exact C: **20,072 bytes**
- Remaining gap to the objective: **36,276 bytes**
- The GS1-English full build is byte-identical with zero ROM fallback.
- The source-only build owns all 8 MiB with zero unowned bytes.
- No background jobs or subagents should be assumed to be running after this
  wind-down.

The denominator rose twice during this session because boundary audits proved
that previously excluded overlay bytes were executable. The guarded correction
protocol is documented in `docs/FULL-C-BYTE-SHARE.md`: a denominator change
requires the regenerated executable inventory and an explicit
`metrics: correct executable denominator` commit subject.

## Session results

Exact overlay C added during this session:

| Overlay owner | Exact bytes | Notes |
| --- | ---: | --- |
| `resource_3a8:0040` | 108 | randomized signed timer/state routine |
| `resource_37a:2924` | 72 | exact with source-scoped `thumb-immediate-latency` |
| `resource_394:03c0` | 48 | preset copy; exposed a 48-byte denominator omission |
| `resource_394:0b8c` | 88 | sentinel-terminated tile-run lookup |
| `resource_3a8:390c` | 100 | effect/render-state update |
| `resource_3a8:3970` | 100 | adjacent effect update |
| `resource_3a8:39d4` | 102 | adjacent effect update; separate alignment retained |
| `resource_3c8:0594` | 22 | object callback |
| `resource_3c8:05ac` | 56 | state-dependent callback dispatch |
| `resource_3c8:0690` | 14 | callback wrapper |
| `resource_3c8:0754` | 132 | effect allocation/setup with one preserved alias |
| `resource_3c8:08a0` | 24 | store created object |
| `resource_3c8:08b8` | 16 | clear stored object; exposed a 16-byte denominator omission |
| `resource_3c8:0910` | 60 | conditional position swap |
| `resource_3bc:0274` | 132 | object/scene initializer |
| `resource_3bc:057c` | 14 | wrapper |

Total exact overlay gain listed above: **1,088 bytes**.

Important commits already made:

- `a66309c6` — exact `resource_3a8:0040`
- `16891eab` — exact `resource_37a:2924`
- `0e2d3fdb` — exact `resource_394:03c0` and guarded denominator correction
- `64b83468` — exact `resource_394:0b8c`
- `caad8f39` — six adjacent exact overlay functions

The final pending batch contains `resource_3c8:0754`, `08a0`, `08b8`, `0910`
and `resource_3bc:0274`, `057c`, plus the regenerated metric inventories.

## What worked

The old overlay inventory is strongly call-seed biased. It misses genuine
prologue owners and sometimes promotes internal branch targets that inherit
registers or stack frames. The productive workflow was:

1. Walk an overlay linearly by genuine prologue and sole-return boundaries.
2. Separate C code, compiler-owned literal pools, structural alignment, veneer
   banks, and data before drafting.
3. Reconstruct maintainable semantic C.
4. Verify at the fixed overlay base.
5. Promote immediately on zero differences.
6. For a near-match, freeze the source and run the existing automated
   1,633-configuration single/compatible-pair sweep once.
7. If the sweep has no exact result, park it and continue to fresh code.

This found productive local runs in `resource_3a8`, `resource_3c8`, and
`resource_3bc`.

## Best restart points

Continue linearly from:

- `resource_3c8` after exact owner `0x02000910`.
- `resource_3bc` at genuine owner `0x0200058c`.

The active agents were interrupted during those scans for this wind-down; do
not assume their next candidates were completed.

Useful ignored semantic drafts and verifiers are under `work/`. Highest-value
parked near-matches include:

- `work/resource_3a8-3864/`: 168/168 bytes, two differing bytes; all 1,633
  configurations exhausted.
- `work/resource_3a8-0504/`: honest 138-byte C symbol plus separate two-byte
  alignment; scheduling mismatch, sweep exhausted.
- `work/resource_37a_2924/`: historical pre-promotion sweep evidence.
- `work/resource_3c8_05e4/`: 172/172 bytes, seven differing halfwords; sweep
  exhausted.
- `work/resource_3bc-0404/`: 160/160 bytes, five differing bytes; sweep
  exhausted.
- `work/resource_394_a90/`: 172/172 bytes, 19 differing halfwords; sweep
  exhausted.
- `work/resource_373-0030/`: twelve-member, 672-byte duplicate family; no exact
  result across 1,633 configurations.

Do not manually source-permute these documented near-matches. Confirm their
floor at most once if tooling changes, then use automated configuration testing
or move to fresh owners.

Large semantic drafts worth retaining, but not immediate exact targets:

- `work/resource_381/semantic.c`: 3,548-byte scene initializer span.
- `work/resource_3a8-0590/`: honest 4,092-byte owner; needs automated
  literal-label emission before useful compilation.
- `work/resource_394_03f0/`: honest 1,008-byte owner.

## Boundary facts

- `resource_381:2fba` is an internal hidden-context continuation of the owner
  at `2eb0`, not a standalone C ABI function.
- `resource_394:01cc`, `resource_3bb:3be4`, and several other inventory entries
  are internal continuations, not honest functions.
- `resource_394` ordinary code ends at the veneer bank beginning `0x0ff4`;
  the remainder is veneers and data.
- `resource_37a` ordinary code ends at the veneer bank beginning `0x296c`.
- Preserve structural `.2byte 0` alignment outside C symbols when object-symbol
  evidence proves it is not compiler-owned code.

## Required checks

Before every exact-C commit:

```sh
bun tools/full_c_progress.ts --write-inventory
bun tools/full_c_progress.ts --write-report
bun run verify
```

Commit subjects must end in the live suffix printed by:

```sh
bun tools/full_c_progress.ts --subject
```

Clean-room rules remain authoritative in `PROVENANCE.md`. Do not use leaked
SDK material or provenance-problematic third-party decompilation content.
`pret` projects may be used only as demonstrably derived generic structural
reference under the existing project policy.

Use at most three active agents total, including the main agent.
