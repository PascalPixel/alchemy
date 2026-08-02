> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

# Full-C Byte Share

Full-C Byte Share is Alchemy's sole headline progress metric:

```text
union(byte-identical executable spans emitted from canonical C)
----------------------------------------------------------------
union(all audited executable spans in the main image and code overlays)
```

The current exact fraction is generated with `bun run progress`. The
percentage is a round-half-up, two-decimal presentation of that fraction; it
is not stored independently in commit subjects.

## Denominator evidence

For GS1-English, the main-image inventory is the non-overlapping union of the
byte-verified claimed-C and classified reconstruction-assembly manifests.
This includes Thumb and ARM bodies, startup code, structural assembly, linker
veneers, literal-bearing regions, and executable alignment while excluding
data/assets gaps in the ROM image.

Each of the 96 decoded code overlays is independently namespaced. Its tracked,
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
linking. Code-overlay C spans come from the verified `AlchemyC_` placeholder
ownership represented by matching code-overlay C files. Every span must be wholly
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

`tools/coverage_map.ts` publishes the same measurement as four pictures:
[`gs1-en-core.svg`](../assets/readme/gs1-en-core.svg),
[`gs1-en-overlays.svg`](../assets/readme/gs1-en-overlays.svg),
[`gs1-en-images.svg`](../assets/readme/gs1-en-images.svg), and
[`gs1-en-music.svg`](../assets/readme/gs1-en-music.svg), with the tile data in
[`metrics/gs1-en-coverage-map.json`](../metrics/gs1-en-coverage-map.json).

It derives exact and semantic ownership the way the history ledger does—from
tracked trees. Dark-gray retained ownership additionally reads the latest verified
full-build assembly manifest; run the publication redraw after `bun run verify`.
If that manifest is absent, only explicit tracked non-code spans are dark gray and
the unresolved complement remains gray:

* main-image exact C: `src/<address>.c` against audited region boundaries,
  excluding register-pinned, inline-assembly and fakematch sources;
* code-overlay exact C: `AlchemyC_` placeholder spans in `assets/code/*_overlay.s`;
* semantic C: `semantic/` sources sized by `semantic/main-regions.json`,
  `semantic/regions.json`, or their single audited region, clipped to the
  executable union and with exact C subtracted, because exact always wins;
* ROM-image layout: the audited executable union, the compressed code-overlay streams in
  `assets/manifest.json`, and the complement of both as asset data.

For call-target evidence, code-overlay inventory scans the compiler-filled image,
not the zero-filled assembly placeholder image. Canonical assembly listings
still supply the instruction/directive boundaries. This keeps raw leaf and
call-via-bank code visible when its only caller has already become exact C.

The exact-C numbers it derives must equal `metrics/gs1-en-progress.json`
exactly; a disagreement is an error rather than a redrawn picture. That report
is read from the selected exact source tree, so the check also holds when an
explicit ref is used. Semantic C is not part of Full-C Byte Share and is drawn
as a separate colour, never folded into the headline fraction.

`bun tools/dashboard_server.ts` serves a separate live worktree view. It
derives the same ownership map directly in memory, watches `asm/`, `assets/`,
`metrics/`, `semantic/`, `src/`, and the verified assembly manifest, and pushes
a new draw to the browser after relevant changes. It deliberately tolerates a
temporarily stale tracked progress report while editing; the normal publication
path does not.

The map records its input trees in `provenance.exact_source` and
`provenance.semantic_source`. Both `--write` and `--check` re-resolve those
values; `--exact-ref` and `--semantic-ref` can override them, while `worktree`
selects the local tree. Normal `main` publication uses `worktree` for both, so
the checked-in pictures describe the commit being published.

A recorded ref that is not available locally is an error, not a fall back to
the working tree: falling back could quietly republish an older measurement.
Likewise a redraw that cannot see the recorded semantic source refuses rather
than publishing that portion as zero.

The map is regenerated on `main`, so `bun run coverage:check` is not part of
`bun run verify`; regenerate the pictures explicitly when coverage changes.
