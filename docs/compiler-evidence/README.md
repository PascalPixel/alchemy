# Compiler-lane evidence

Findings that justify a change to the pinned gcc 2.96 Camelot fork, kept here
so the evidence survives the machine the probe ran on. Each entry records the
mechanism, a prototype diff against the fork source, and the collateral
measurement a routed mode needs.

Nothing here is applied. Landing any of it requires the fork change to be
committed to the `alchemy-gcc` repository and the pinned commit updated, then
the digests re-pinned in `tools/alchemy_gcc.ts` — and per `PROVENANCE.md` that
re-pin is admissible only after the source-only build reproduces gs1-en.gba
(SHA-1 `5c4695205413df7db52b9a184815a07783999971`) byte-identically.

## cse-two-insn-immediate

Blocks the largest single class of unconverted overlay functions: the reference
objects rematerialize a two-instruction immediate (Thumb `movs rN,#K` then
`lsls rN,rN,#n`, or `movs`/`negs` for a negatable value) independently at each
call site, while our builds compute it once and copy it from a register, which
also changes the prologue.

**Mechanism.** Not the register allocator — CSE recording an equivalence.
`arm_rtx_costs` prices such a constant at `COSTS_N_INSNS(2)`, so `COST` makes it
12 against 1 for a pseudo. In `cse_insn`'s destination-recording loop, the
register just loaded with the constant is inserted into the constant's
equivalence class; `insert_regs` then merges the quantities of every register
loaded with that value, so `canon_reg` rewrites the later loads and
`delete_trivially_dead_insns` removes them. Single-instruction immediates are
unaffected because their `COST` is already 0 — which is exactly the observed
boundary.

**Prototype.** `cse-two-insn-immediate.diff` adds `-fno-cse-two-insn-immediate`
in the same style as the fork's existing custom flags, delegating the set of
affected constants to a new `TWO_INSN_CONSTANT_P` predicate in
`config/arm/arm.h` (constraint `J` or `K` but not `I`) so `cse.c` carries no
target knowledge. A one-hunk variant (the destination-recording guard alone)
measured identically on all ten targets and touches one fewer installed source;
it is the narrower gate.

**Inertness.** With the flag absent the patched compiler produced byte-identical
assembly to the installed bundles on all 1,335 gcc296-routed sources.

**Collateral.** With the flag on, 109 of those 1,335 change (listed in
`cse-two-insn-immediate-collateral.txt`), so it must be a routed per-source mode
like every other flag the fork carries, never a global default.

**Payoff.** Three functions become byte-exact immediately — resource_3bf:0bec
(140 bytes), resource_3af:1a98 (192), resource_3af:4218 (60). Five more drop to
between 2 and 13 halfwords (about 1,050 bytes), where the entire remaining
residual is a single new transposition class: the reference schedules a
neighbouring one-instruction immediate between a rematerialized constant's
`movs` and its `lsls`, where we keep the pair adjacent. The total population
parked behind this one behaviour is roughly 9,000 bytes, including large sheets
in resource_3af and named members in resource_3ba and resource_3bf whose drafts
were never written because the blocker made them pointless.

**Deliberately out of scope.** The literal-pool sub-class is a different
behaviour and this predicate excludes it: a pool load is one instruction, so
sharing it is not a size change, and at least one reference function is recorded
sharing a pool word itself. Functions mixing both classes can get marginally
worse, which is another reason this is per-source and not a model correction.
