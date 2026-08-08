> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

# Full-C history ledger

This is the human-readable index for the generated first-parent exact-C
history.  The authoritative machine-readable ledger is
[`full-c-history.json`](full-c-history.json), with the same rows in
[`full-c-history.csv`](full-c-history.csv).

## Scope

- Target: GS1 English only.
- Metric: exact Full-C byte ownership against the fixed audited executable
  denominator.
- History: every first-parent commit, measured from that commit's tracked tree;
  commit subjects are not trusted as the data source.
- Main-image and code-overlay ownership are recorded separately in every row.
- Noncanonical C (inline assembly, hard-register pins, fakematches, and similar
  scaffolding) is excluded.  Historical denominator corrections are annotated
  rather than rewritten away.

## Current reference point

The live checked metric is **265,064 / 1,343,410 bytes (19.73%)**: 104,192
main-image bytes and 160,872 code-overlay bytes. This reference point is a working
tree publication metric, not a promise that the final history row has already
been committed.  Regenerate the ledger after a commit with:

```sh
tools/full-c-history/target/release/full-c-history --write
```

Then verify the current metric independently with:

```sh
make progress-check
```

The ledger intentionally reports exact C only.  Reviewed semantic coverage is
documented separately in [`../HANDOVER.md`](../HANDOVER.md) and is never folded
into this history.
