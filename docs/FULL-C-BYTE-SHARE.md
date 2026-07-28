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
changes and numerator regressions are rejected.

Historical commits are not rewritten. `tools/full_c_history.ts` measures every
first-parent tree against the fixed audited denominator and writes
[`full-c-history.json`](full-c-history.json) and
[`full-c-history.csv`](full-c-history.csv). It derives ownership from each
commit tree rather than trusting incompatible legacy subject suffixes.
