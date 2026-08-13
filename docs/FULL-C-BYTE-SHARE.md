# Executable progress metrics

Alchemy publishes two related byte measurements. They deliberately answer
different questions.

| Label | Numerator | Use |
|---|---|---|
| **Exact C** | byte-identical executable spans emitted from canonical C | contributor progress, history, and commit subjects |
| **DONE** | Exact C plus audited permanent assembly | README and chart completion |

Semantic C is shown as work in progress. Ordinary reconstruction assembly is
still work to convert. Neither contributes to Exact C or DONE.

## Exact C

```text
union(byte-identical executable spans emitted from canonical C)
----------------------------------------------------------------
union(all audited executable spans in the main image and overlays)
```

Read the live value with `make progress`. The tracked report is
[`../metrics/gs1-en-progress.json`](../metrics/gs1-en-progress.json). Progress
uses executable bytes, not owners or functions, because differently sized
owners should not carry equal weight.

Every commit subject starts `☀️ N% – description`, where `N` is the nearest
whole percentage from the staged Exact-C numerator divided by the audited
executable denominator. This is Exact C, never the public DONE share. The
pre-commit gate compares `N` with the staged metric; it does not derive a
percentage from display kilobytes.

## Audited denominator

The GS1-English denominator is the non-overlapping union of executable bytes
in the resident main image and all 96 code overlays. It includes Thumb and ARM
bodies, veneers, literal-bearing executable regions, and executable alignment;
it excludes data and assets. The tracked interval evidence is
[`../metrics/gs1-en-executable.json`](../metrics/gs1-en-executable.json).

`make progress-check` regenerates that evidence and refuses incomplete,
overlapping, or stale ownership. A target whose inventory says
`audit: "incomplete"` cannot publish a percentage. The current audited
denominator is used uniformly for Exact-C history; historical corrections are
annotations, not row-by-row denominator changes. Ordinary decompilation work
does not change the denominator.

## Ownership evidence

Main-image exact spans come from the claimed-code build manifest. Overlay exact
spans come from compiler-filled `AlchemyC_` placeholders backed by canonical C
files. Every span must fit wholly inside its audited executable owner and may
not overlap another claim.

Inline assembly, fixed-register bindings, copied bytes, and other fakematch
scaffolding are excluded even if they happen to link. ROM equality and valid C
ownership are independent yes/no requirements.

## Charts and live dashboard

`make coverage` derives
[`../metrics/gs1-en-coverage-map.json`](../metrics/gs1-en-coverage-map.json) and
the four SVGs under `assets/readme/`. It also synchronizes the README `DONE`
headline and cache keys. Exact C must agree byte-for-byte with the tracked
progress report or the writer refuses to publish.

The dashboard server derives the same categories directly from the worktree,
watches relevant source inputs, and serves `http://localhost:4649/`. A service
may additionally bind a configured LAN address without giving up localhost.

The first-parent Exact-C history is described in
[`full-c-history.md`](full-c-history.md). Historical rows are measured from
their trees rather than trusted from old commit messages.
