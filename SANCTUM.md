# Sanctum: the sealed-owner ledger

Sol Sanctum is sealed until the right keys exist. This ledger records owners
withdrawn from routine attack because **both** search axes are exhausted, so a
later agent stops rediscovering the same floor.

This is not the same concept as retained assembly. `retained_asm` in
[metrics/gs1-en-coverage-map.json](metrics/gs1-en-coverage-map.json), guarded by
`tools-rs/core-retained-audit`, records regions
that are permanently assembly. Sanctum records owners that *could* have a C form
but where the bounded search for it has been run out. Sealing is a statement
about our search, not about the ROM.

[LAWS.md](LAWS.md) records what we proved true. Sanctum records where we stopped.

## The two axes

An owner may only be sealed once both have been run and neither reaches exact:

| Axis | Tool | Searches |
|---|---|---|
| Compiler | [tools/lib/mode_sweep.ts](tools/lib/mode_sweep.ts) | flags and compiler family, source held fixed |
| Source shape | [tools/search/shape_sweep.ts](tools/search/shape_sweep.ts) | equivalent source forms, compiler held fixed |

Exhausting one axis is not a seal. A residual that survives every flag is the
normal starting point for the shape axis, and the reverse holds too.

Both tools are bounded and deterministic, which is what makes a seal meaningful.
The annealers (`tools/search/permute_v1.ts`, `tools/search/permute_overlay.ts`) search wider
but stochastically, so a run that finds nothing proves nothing and can never
justify sealing. A seal may cite an annealer run as further evidence; it may
never rest on one.

## Entry format

Each entry is one list item under `## Sealed`, in this shape, enforced by
`tools-rs/check-sanctum`:

```
- `<owner>` floor=<N>hw axes=compiler,shape — <one line naming what the residual is>
```

`<owner>` is a stem exactly as it appears under `semantic/`, without the
extension. `floor` is the smallest differing-halfword count either axis reached.

Removing an entry needs no ceremony: if an owner goes exact, the checker
**requires** its removal, because a sealed owner that is now solved is a lie the
next agent would believe.

## Sealed

<!-- No owner qualifies yet: the shape axis (tools/search/shape_sweep.ts) is newer than
     every floor record on disk, so nothing has had both axes run against it.
     An empty section here is the honest state, not an oversight. -->

## Immediate shape-axis queue

36 owners have an exhausted **compiler** axis on record and have never had the
shape axis run. They are the first candidates for `shape_sweep.ts`, and the
first candidates to become sealed entries if it also fails. Regenerate the list
with:

```
tools-rs/check-sanctum/target/release/check-sanctum --queue
```

The lowest floors are the best targets: a 10-halfword residual after a complete
flag sweep is a source-shape question by elimination, which is exactly what the
shape axis exists to answer.
