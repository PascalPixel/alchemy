# Exact-C history

The machine-readable first-parent ledger is
[`full-c-history.json`](full-c-history.json), with the same rows in
[`full-c-history.csv`](full-c-history.csv).

Each row is measured from that commit's tracked tree against the current audited
executable denominator, used uniformly across history. Commit subjects are
formatted as `☀️ N% – description`, where `N` is the nearest whole Exact-C
percentage; they are never used as measurement data. Main-image and overlay
ownership remain separate, and inline assembly, hard-register pins, and other
noncanonical C never count.

Do not put a hand-maintained “current” number in this document. Read the live
working-tree value with `make progress`; after committing, regenerate history
with:

```sh
cargo run --release --manifest-path tools/dispatch/Cargo.toml -- \
  metrics full_c_history --write
```

Historical denominator corrections are annotated rather than rewritten away;
the full-DAG audit mode measures every reachable commit, while this tracked
ledger remains first-parent for concise project history.
Semantic C is intentionally absent from this ledger.
