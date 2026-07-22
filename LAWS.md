# Compiler law ledger

This ledger records compiler behavior that Alchemy has reproduced from its
approved evidence and exact, installed C matches. It prevents later sessions
from rediscovering the same source-shape constraints.

The clean-room and publication rules in [AGENTS.md](AGENTS.md) apply here.
Externally suggested compiler behavior remains a hypothesis until reproduced
locally. A confirmed law must name its in-repository evidence, scope, and
confirmation date. An exact match proves the cited source shape; broader claims
must be tested on more than one function before being generalized.

## Confirmed laws

### Minimal live-variable form

- **Fingerprint:** a semantically correct candidate contains the expected
  operations but allocates a related value to a different register after extra
  named temporaries extend live ranges.
- **Producing idiom:** use the fewest independently named intermediates that the
  expression requires; eliminate generator-created copy temporaries before
  searching arbitrary declaration orders.
- **Scope:** confirmed for the `0807933x` counter cohort. This is a normalization
  priority, not a universal promise that removing temporaries fixes every
  register-only mismatch.
- **Evidence:** exact installed matches [src/08079338.c](src/08079338.c) and
  [src/08079358.c](src/08079358.c), with the bounded counter-family experiment
  recorded in the former local wall ledger.
- **Confirmed:** 2026-07-21.

### Dual-use pre-read

- **Fingerprint:** a left/right shift pair uses a fresh destination and then
  returns to the argument register, rather than folding the operation in place.
- **Producing idiom:** the source value has another use before the shift, which
  changes its live range; in the confirmed counter cohort the mask expression
  reads the argument before the index conversion.
- **Scope:** confirmed for the `0807933x` counter cohort. Treat other occurrences
  as hypotheses until their second use is independently found.
- **Evidence:** exact installed counter shapes in
  [src/08079338.c](src/08079338.c), [src/08079358.c](src/08079358.c), and their
  neighboring installed cohort members.
- **Confirmed:** 2026-07-21.

### Approved-compiler Thumb prologue

- **Fingerprint:** any Thumb function containing a conditional branch or loop
  saves `lr`; only straight-line leaf functions use a bare `bx lr` return.
- **Consequence:** an internally branching region that never saves `lr` and
  returns through `bx lr` is evidence for reclassification, a different
  toolchain, or deliberate assembly rather than ordinary approved-compiler C.
- **Evidence:** the independently reproduced compiler experiment and classified
  regions documented in [ASSEMBLY.md](ASSEMBLY.md#approved-compiler-prologue-evidence).
- **Confirmed:** 2026-07-19.

### Volatile stores fence load hoisting

- **Fingerprint:** a pure instruction-reorder mismatch in which the candidate
  hoists later loads above earlier stores that the reference keeps in program
  order, while register-only moves still interleave freely and independent
  loads inside one statement still swap.
- **Producing idiom:** the reference statement's store lvalue is
  `volatile`-qualified. The scheduler never moves a later load above a volatile
  store, keeps volatile accesses in program order among themselves, and still
  hoists plain loads above volatile *reads*. A volatile *read* also pins its own
  position among other volatile accesses, which fixes leading temp-load order.
- **Scope:** confirmed for `0809a65c` (all three hoist sites plus the leading
  temp-load site resolved by volatile store lvalues plus one volatile init
  read). The same qualifier did not resolve the `0809802c` prologue
  `sub sp` placement, so this law governs load-versus-store order only, not
  stack-allocation scheduling.
- **Evidence:** exact installed match [src/0809a65c.c](src/0809a65c.c);
  bounded flag experiments (`-fno-schedule-insns{,2}`, `-mtune` sweep,
  sched-spec flags) reproduced the hoist under every non-volatile shape.
- **Confirmed:** 2026-07-22.

### Register-reservation flags are module declarations

- **Fingerprint:** a neighboring handler bundle consistently avoids the same
  register, and the bundle matches when that register is reserved for the
  translation unit.
- **Producing configuration:** an evidenced translation-unit flag, never an
  inline register pin or arbitrary per-function matching knob.
- **Evidence:** the `FIXED_R3_SOURCES` bundle and rationale in
  [tools/alchemy_gcc.ts](tools/alchemy_gcc.ts).
- **Confirmed:** 2026-07-21.

## Hypotheses

Hypotheses are useful search leads, not accepted compiler laws. Promote one only
after an approved local experiment or exact installed match supplies the stated
evidence.

### Pre-epilogue literal pool

- **Claim:** 31 remaining C-debt regions share a structural signature the
  per-function harness has not reproduced: the literal pool is dumped before
  the epilogue behind an inserted `b.n`, instead of after the final `bx`.
  Example: `080b09fc` (candidate exact except pool placement plus one
  scheduling transposition). Enumerated by matching
  `b <label>; <data words>; <label>: pop` in `asm/*.s`.
- **Disproved so far (2026-07-22):** a trailing function in the same
  translation unit (both tiny and >1KB, so simple pool-range pressure is not
  the trigger), `-mno-sched-prolog` (accepted by the driver, changes
  bytes, does not move the pool), and a real multi-function
  `decomp_module.ts` unit (`080b09fc`+`080b0a20`: pool still lands after
  `bx r0`). The approved compiler has not produced a pre-epilogue pool in
  any tested configuration.
- **Next test:** reproduce with several functions and interleaved pool
  pressure in one unit via `decomp_module.ts`-style multi-region compiles;
  study which insn the reorg pass anchors the minipool to when the epilogue
  falls through versus branches.
- **Recorded:** 2026-07-22.

### Store-multiple transfer idiom

- **Claim:** a family of DMA-style regions ends in
  `stmia rB!, {r0, r1, r2}` + `subs rB, #12` with the three words either
  pool constants (`08004838`) or computed in registers (`080b0840`).
  With the adjacent-store peephole disproved (ideal-conditions probe
  emits three `str`), this must be an aggregate store whose load side is
  register-forwarded — a hybrid the harness has not produced.
- **Disproved so far (2026-07-22):** memberwise stores in every
  declaration/init order (loads group but stores stay `str`, registers
  allocate r2/r1 against the needed ascending order); const-local struct
  copy (stack round-trip); static-const and GNU constructor-expression
  copies (direct `ldmia`/`stmia` pair, source outside region); volatile
  destination with memberwise temp (full stack round-trip, no
  forwarding).
- **Next test:** `inline` helper taking the struct by value or returning
  it; aggregate function arguments that the ABI splits into r0-r2.
- **Recorded:** 2026-07-22.

### Cast-literal table access

- **Claim:** `((s32 *)ADDRESS)[index]` can produce an add-then-load address shape
  that an array declaration does not.
- **Current evidence:** this shape improves the still-unconverted `080fb670`
  candidate, but its remaining register floor is open. The function is not
  evidence for a confirmed exact-producing law yet.
- **Next test:** retain the cast-literal shape while independently solving or
  disproving the remaining allocation mismatch.
- **Recorded:** 2026-07-22.

## Retired approaches

Retirement records why a dead angle should not be restarted casually. A future
replacement must state what changed and define an acceptance test.

| Approach | Active/retired | Reason and replacement |
|---|---|---|
| `brute_permute.ts` | retired 2026-07-21 (`8269ce7c`) | Exhaustive prototype superseded by the guided annealer in `permute_v1.ts`. |
| `cfg_extract.ts` | retired 2026-07-21 (`8269ce7c`) | Rebuilder prototype delivered its diagnostic conclusions; surviving queue and permuter operators replaced the stage. |
| `mine_blocks.ts` | retired 2026-07-21 (`8269ce7c`) | Block-mining prototype was folded into the surviving matching approach. |
| `synthesize_expr.ts` | retired 2026-07-21 (`8269ce7c`) | Expression synthesizer completed its diagnostic role; targeted variants and guided permutation replaced it. |
| `synthesize_block.ts` | retired 2026-07-21 (`8269ce7c`) | Block synthesizer completed its diagnostic role; targeted variants and guided permutation replaced it. |
| `permute_decompperm.py` | removed 2026-07-18 (`3ceeab70`) | The Python-era decomp-permuter bridge was removed in the Bun migration. It had produced exact matches on 2026-07-17 (`9ef88b5e`); review that history before deciding whether a Bun bridge is worth rebuilding. |
| `build_context.ts` | retired 2026-07-22 | Plain declaration collector; strict subset of the scoring, struct-folding `m2c_context.ts` context builder that replaced it in the wave pipeline. |
| `close_byte_gaps.ts` | retired 2026-07-22 | The English byte-closure plan completed in `6104a64e`; current ownership and identity are enforced by `build_full.ts` and the canonical component builders. |
| `veneer_island.ts` | retired 2026-07-22 | The byte-closure-era veneer islands are now canonical assembly claims classified and verified by `build_asm.ts`. |
