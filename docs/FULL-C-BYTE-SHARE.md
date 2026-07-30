# Full-C Byte Share

Full-C Byte Share is Alchemy's sole headline progress metric:

```text
union(byte-identical executable spans emitted from canonical C)
----------------------------------------------------------------
union(all audited executable spans in the main image and overlays)
```

The current exact fraction is generated with `bun run progress`. The
percentage is a round-half-up, two-decimal presentation of that fraction; it
is not stored independently in commit subjects.

## Denominator evidence

For GS1-English, the main-image inventory is the non-overlapping union of the
byte-verified claimed-C and classified reconstruction-assembly manifests.
This includes Thumb and ARM bodies, startup code, structural assembly, linker
veneers, literal-bearing regions, and executable alignment while excluding
asset-owned ROM gaps.

Each of the 96 decoded overlays is independently namespaced. Its tracked,
byte-round-tripping canonical assembly distinguishes instruction lines from
data directives. The inventory maps assembler listing addresses back to those
source lines, adds PC-relative literal targets, fixed `ldr`/`bx` veneers,
verified C placeholder spans, and two-byte executable alignment, and records
the complement as excluded data. It rejects overlaps and incomplete decoded
byte accounting.

The tracked inventory is
[`metrics/gs1-en-executable.json`](../metrics/gs1-en-executable.json).
`bun run progress:check` regenerates the interval union and rejects a stale
inventory.

GS2-English currently has only a compiler/bootstrap source and no complete
executable classification. Its target-scoped inventory is deliberately marked
`incomplete`; the metric fails closed instead of publishing a partial
denominator.

## Numerator evidence

Main C spans come from the normal claimed-code manifest after compilation and
linking. Overlay C spans come from the verified `AlchemyC_` placeholder
ownership represented by matching overlay C files. Every span must be wholly
contained in its target's audited executable union and ownership may not
overlap.

Legacy inline-assembly or hard-register fakematches are excluded even if an old
claimed build links them. Source ownership and ROM equality remain separate
yes/no verification properties.

## Commit subjects and history

Every new commit subject ends with:

```text
[C X/Y bytes]
```

The hook checks the exact fraction against
`metrics/gs1-en-progress.json` from the Git index, not the unstaged working
tree. Changes to executable source or inventory require that regenerated
report to be staged. After the one legacy-to-Full-C transition, denominator
changes are rejected by default and numerator regressions are always rejected.
If a boundary audit proves that executable bytes were previously excluded or
non-executable bytes included, the correction must stage the regenerated
executable inventory and use an explicit
`metrics: correct executable denominator` commit subject. Ordinary
decompilation commits cannot change the denominator.

Historical commits are not rewritten. `tools/full_c_history.ts` measures every
first-parent tree against the fixed audited denominator and writes
[`full-c-history.json`](full-c-history.json) and
[`full-c-history.csv`](full-c-history.csv). It derives ownership from each
commit tree rather than trusting incompatible legacy subject suffixes.

## Coverage map

`tools/coverage_map.ts` publishes the same measurement as a picture:
[`assets/readme/gs1-en-coverage.svg`](../assets/readme/gs1-en-coverage.svg),
a treemap of the cartridge and of the audited executable
denominator, with the tile data in
[`metrics/gs1-en-coverage-map.json`](../metrics/gs1-en-coverage-map.json).

It derives ownership the way the history ledger does—from tracked trees rather
than from a build—so it runs without a ROM or toolchain:

* main-image exact C: `src/<address>.c` against audited region boundaries,
  excluding register-pinned, inline-assembly and fakematch sources;
* overlay exact C: `AlchemyC_` placeholder spans in `assets/code/*_overlay.s`;
* semantic C: `semantic/` sources sized by `semantic/main-regions.json`,
  `semantic/regions.json`, or their single audited region, clipped to the
  executable union and with exact C subtracted, because exact always wins;
* ROM layout: the audited executable union, the compressed overlay streams in
  `assets/manifest.json`, and the complement of both as asset data.

The exact-C numbers it derives must equal `metrics/gs1-en-progress.json`
exactly; a disagreement is an error rather than a redrawn picture. The
semantic lane is not part of Full-C Byte Share and is drawn as a separate
colour, never folded into the headline fraction.

Because the semantic lane lives on `venus` and the exact lane advances on
`mercury`, the map records which tree each lane came from. `bun run
coverage:check` only fails on lanes the current branch owns; a lane read from
another branch's ref is refreshed by running `bun run coverage`, not by
failing this branch's verification.
