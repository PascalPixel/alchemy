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
  regions documented in [CONVENTIONS.md](CONVENTIONS.md#approved-compiler-prologue-evidence).
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

### Fourth stratum: default-ABI library units

- **Fingerprint:** functions saving r4 that dies before any call —
  impossible under Camelot's `-fcall-used-r4`. First member: the flash
  byte-program sequence `08006dec` (0x0E005555/0x2AAA command writes).
- **Evidence:** the identical draft under our approved cc1 WITHOUT
  `-fcall-used-r4` reproduces the r4 discipline and reaches 13
  mismatched bytes (one pool-load-vs-volatile-store scheduling
  decision), versus 44 under `old_agbcc` — this stratum is
  same-compiler, default-flags: a per-stem flag registry case exactly
  like `FIXED_R3_SOURCES`, needing no second binary.
- **Also observed:** our vintage exempts constant-pool loads from the
  volatile-store fence (readonly memory class); the reference pins
  them. A vintage marker within the same scheduling-divergence family
  as the LUID prologue ordering.
- **Recorded:** 2026-07-22.

### Three-stratum compiler census

- **Fingerprint:** prologue register discipline splits the executable
  image: Camelot code never saves r4 (`-fcall-used-r4` ABI, 988
  installed matches), while two functions inside the audio bank
  (`080f95f0`, `080f9a50`) push r4 as callee-saved — the stock-library
  ABI. Hand-scheduled kernels form the third stratum (retained classes).
- **Evidence:** an identical natural draft of `080f9a50` scores 31
  mismatched bytes under natively built `old_agbcc` (structure aligned,
  ands/tst fusion residue) versus 58 under the approved gcc-2.96 —
  the strongest per-TU vintage discrimination available without an
  installed match. Consistent with the sibling repository's finding
  that the `rom_f9000` audio bank links the prebuilt stock m4a library,
  and with `pret/pokeruby` compiling its m4a unit with `old_agbcc`.
- **Governance:** adopting `old_agbcc` as a second approved compiler
  for the identified stock-library functions is Pascal's call; the
  provenance section of [CONVENTIONS.md](CONVENTIONS.md#approved-compiler-bundle) holds the
  pending decision. Until adopted, these functions stay reconstruction
  assembly.
- **Recorded:** 2026-07-22.

### Scheduler trace instrumentation (native)

- **Capability:** the approved cc1 natively emits the full second-pass
  scheduler decision trace: `-dR -fsched-verbose=7` writes
  `<file>.c.23.sched2` containing per-cycle ready lists (173 snapshots
  on a 184-byte function), chosen insns, priorities, and a
  clock-by-clock schedule visualization. `-dS` covers the first pass.
- **Use:** the register/scheduling tie-break plateau (0807808c,
  0809802c, 08092b54, 0801c154, 0801fda8 class — the tier both
  deterministic search and 400k-step permutation cannot reach) becomes
  mechanically diagnosable: read which insn the ready list preferred at
  the divergence cycle and shape the source to flip that single
  decision, or derive the law that no shape can.
- **First application (0807808c):** the trace proves its transposition
  is priority-forced — the sign-extend chain outranks the store's
  direct call edge by its intermediate hops, and no intra-block source
  construct can change either chain. The reference schedule therefore
  implies a different latency/priority table: the precise experiment is
  a diagnostic 2.96 build carrying the dated pret/agbcc ARM backend
  tables (arm_010110a / arm_020422) run against the tie-break battery.
- **Recorded:** 2026-07-22.

### Address-valued small constants preserve pool loads

- **Fingerprint:** the reference loads a very small numeric word from a literal
  pool and compares it as a full register value, while natural C with the same
  integer literal selects an immediate compare and changes downstream register
  allocation.
- **Producing idiom:** represent the value as the address of a canonical
  absolute symbol, for example `(s32)&Value_00000001`, when the linked pool word
  is independently verified to be that address. The compiler must then retain
  the relocation-backed pool load instead of folding the value to an immediate.
- **Scope:** confirmed for the shared `Data_02000240[237]` state test. This is
  not permission to recast arbitrary integer constants as addresses; the pool
  word and its use must independently support the address-valued reading.
- **Evidence:** exact installed matches [src/0809b5dc.c](src/0809b5dc.c) and
  [src/0809b364.c](src/0809b364.c). In the latter, preserving the pool load and
  delaying the position read also removes an unnecessary r7 live range.
- **Confirmed:** 2026-07-22.

## Hypotheses

Hypotheses are useful search leads, not accepted compiler laws. Promote one only
after an approved local experiment or exact installed match supplies the stated
evidence.

### Externally sourced compiler lore (2026-07-22 research pass)

Recorded from public compiler source and generic community documentation
(no Golden Sun material). Each item is a hypothesis until reproduced
against the approved bundle; full sourced notes in
`work/research/gba-decomp-craft.md` (ignored analysis).

- **Prologue lr-save flag:** community agbcc documents
  `-fprologue-bugfix` as suppressing unnecessary `lr` saves in leaf
  functions that newer vintages otherwise emit. Our confirmed
  Thumb-prologue law (branch implies lr save) may be the unfixed-vintage
  behavior. LOCALLY PROBED 2026-07-22: the approved cc1 REJECTS
  `-fprologue-bugfix` and `-fhex-asm` (`Unrecognized option`) — the
  approved bundle is not pret's agbcc but an earlier or vanilla
  GCC 2.95-class vintage, consistent with its lr-save-on-branch
  behavior, end-of-function pool barriers, and HImode `ldrh .L`
  emission.
- **Long-branch `bl` substitution:** agbcc substitutes `bl` for internal
  branches beyond `b` range in large functions. Candidate ordinary-
  compiler explanation for `nonstandard_thumb_call_module` regions
  (15 regions, 348 bytes). DISPROVED for all 15 members (2026-07-22):
  measured `bl` distances are 12-374 bytes, far inside `b` range — the
  substitution rule cannot explain them. They are the 080f9axx-cluster
  multi-entry shared-tail audio units with genuinely nonstandard
  conventions; their retained-structural classification stands.
- **Public thumb_reorg pool algorithm:** first pool-needing insn +
  ≤1000-byte forward scan for an existing barrier, else insert
  `b label; pool; label:` just before trailing jumps; HImode constant
  loads are converted to SImode pool entries. With text epilogues this
  yields pre-epilogue pools for straight-line functions. Our approved
  cc1 shows end-of-function placement in most shapes — treat pool
  placement, HImode `ldrh .L` emission, and prologue policy as
  compiler-vintage markers.
- **Per-TU compiler vintages are period-normal:** pokeruby's Makefile
  compiles the m4a sound library and libisagbprn with `old_agbcc` while
  game code uses agbcc. Retail GBA images mixing TUs from different
  compiler snapshots is established practice; the three pure-symbol
  pre-epilogue pools, the queue-push head order, and the store-multiple
  idiom are candidates for a second-vintage explanation. Any second
  approved bundle is a project decision with its own evidence bar.
- **Permuter craft imports:** upstream decomp-permuter's manual-mode
  PERM macro family (alternative sets, statement lineswap, deferred
  meta-variables) and per-compiler weight files are proven mechanisms to
  port into `permute_v1.ts`; scoring uses graded objdump-diff penalties
  with stack offsets excluded by default. Matched-example retrieval into
  drafting prompts is the community's highest-value context signal
  (validates tool-investment priority 5).

### Hardware-load width and widening (agent lane, 2026-07-23)

- **Claim:** on u16 hardware reads, `volatile` on the pointer forces the
  reference's `ldrh` where the default compiler arbitrarily selects `ldrsh`;
  independently, loading the value into a `u32` local (rather than relying
  on u16 int promotion) avoids a spurious r8 mask spill and pointer
  strength-reduction. Both are zero-flag source shapes.
- **Current evidence:** 080aac84 candidate improved 158→101 mismatches with
  the two shapes combined; a diagnostic flag pair
  (`-fno-expensive-optimizations -fno-strength-reduce`) reaches the same
  floor, confirming the mechanism. Not yet backed by an installed match.
- **Next test:** apply to other palette/VRAM u16 loops in the queue; promote
  on the first exact install carrying the shape.
- **Recorded:** 2026-07-23 (work/agents/080aac84/NOTES.md).

### Grouped-DMA double-kick gap (2026-07-23)

- **Claim:** back-to-back DMA kicks through one register block defeat the
  evidenced `-mgrouped-dma-store` mode: the first non-volatile, fully
  overwritten, unread 12-byte group is deleted as a dead store before the
  peephole runs; `volatile` preserves it but suppresses the mode entirely.
  No C shape yields both `stmia` groups.
- **Current evidence:** 080a22f4 (48-byte leaf, two kicks): kick #2
  reproduced byte-exact under the mode; best full-function floor 37
  mismatches (work/agents/080a22f4/NOTES.md).
- **Next test:** extend the mode in alchemy-gcc so the DMA block survives
  dead-store elimination (hardware memory class or earlier peephole), then
  allowlist 080a22f4; else classify the double-kick member as
  retained-structural per the family's pre-flag inline-asm hypothesis.
- **Recorded:** 2026-07-23.

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
  `bx r0`), and an early-return guard around the body (the conditional
  jumps forward and the body still falls through into the epilogue, so
  no unconditional branch survives jump optimization to anchor the
  pool). The approved compiler has not produced a pre-epilogue pool in
  any tested configuration WITHOUT a call before the epilogue.
- **Mechanism found (2026-07-22, later, twice corrected):** the layout
  is driven by the POOL REFERENCE MODE. Halfword-context (HImode)
  references — the compiler emits `ldrh rX, .Ln` when a pooled constant
  feeds a u16 expression — carry a short range, so the minipool dumps
  nearby behind an inserted branch. Word (SImode) references reach the
  after-epilogue pool. Evidence: the `0801c154` shape (u16 field merge)
  emits `ldrh r4, .L3` refs and reproduces the exact `b .L; .word ×2;
  pop` layout at size parity (best candidate 4 mismatched bytes,
  register roles only); the `0801e940` shape with an SImode terminator
  ref dumps after `bx` in every variant, as do synthetic SImode-store
  probes with one or two pool words, with and without frames, with a
  trailing call. Pool-content classification of the 31 members: 28
  carry halfword-value const words (u16 store/compare contexts) and are
  reachable; `0801e940`, `08020b14`, and `080b09fc` load their entries
  with word-mode `ldr` yet still sit pre-epilogue — the open question
  is what gives a word-mode entry (a zero-valued symbol) the short
  range there.
- **Horizon confirmed locally (2026-07-22, research pass):** a
  synthetic >1.2KB straight-line function makes our approved cc1 insert
  the `b .L; .word; .L:` dump mid-function at ~1030 bytes — our vintage
  has the same ~1KB `find_barrier` scan the public `thumb_reorg`
  documents (first pool-needing insn, forward scan for an existing
  barrier, else insert). Consequently the large add-sp cohort members
  (bodies longer than the horizon past their first pool reference) are
  reachable with ordinary shapes; combined with the HImode short-range
  rule, only the three small pure-symbol members remain open, and
  per-TU vintage (pokeruby `old_agbcc` precedent) is their leading
  explanation.
- **Next test:** reproduce with several functions and interleaved pool
  pressure in one unit via `decomp_module.ts`-style multi-region compiles;
  study which insn the reorg pass anchors the minipool to when the epilogue
  falls through versus branches.
- **Recorded:** 2026-07-22.

### Queue-push family head order

- **Claim:** the nine-member IME-guarded queue-push family
  (`0800383c 0800387c 080038bc 080038fc 0800393c 0800397c 080039bc
  08003a3c 080039fc`, fingerprint `2ljmmtrr4j1a0`, 576 bytes) all begin
  `ldr r4, =queue` BEFORE the first parameter copy `adds r6, r0`. With
  the critical-section body solved (count read inside the IME-disabled
  window, `-fno-schedule-insns2` shape at exactly 4 mismatched bytes on
  both probed members), this head transposition is the single gate for
  all nine.
- **Disproved so far (2026-07-22):** every declaration order of the
  count/first/second temps with and without `register`, register-
  qualified parameters, `-fschedule-insns`-only crossed with ten
  `-mtune` cores, and all scheduling flag pairs — the approved compiler
  emits the parameter copy first in every configuration (30+ probes).
- **Next test:** search installed matches for any function whose first
  emitted body instruction precedes a parameter copy and study its
  shape; consider whether the family's translation unit used a
  mechanism outside the probed flag space.
- **Vintage sweep negative (2026-07-22, late):** the family matches
  neither sibling compiler. Natively built `old_agbcc` saves r4 and
  keeps copies-first; Rosetta-built gcc-3.0 with `-fcall-used-r4`
  reproduces the register discipline but still emits copies-first heads
  and uses `mov` move forms where the reference bytes encode the
  2.96-style `adds rd, rn, #0`. The reference remains 2.96-class
  output with an unexplained head schedule; an intermediate 2.96-line
  SDK snapshot (compare the dated `arm_010110a`/`arm_020422` backends
  preserved in pret/agbcc) is the remaining vintage hypothesis.
- **Era thumb-backend test (2026-07-23):** pret/agbcc's `gcc_arm`
  (2.9-arm-000512, CYGNUS) built natively for `--target=thumb-elf` (the
  separate 2.95-era thumb backend, no function units) still emits
  copies-first heads for every probed declaration shape, while the SAME
  vintage built for `--target=arm-elf` emits pool-load-first heads at
  emission in ARM mode. The era thumb backend does uniquely pin the
  saved-IME copy directly after its read, matching the reference where
  our 2.96 sinks it. Combined with the prior 2.96 flag/tune/unit sweeps,
  gcc-3.0, and old_agbcc negatives, the head transposition is not
  source-reachable in any available compiler: the family is
  vintage-blocked, not shape-blocked. Do not spend further source-shape
  or permutation budget on the nine members; the remaining explanation
  is a Camelot SDK snapshot whose thumb expand shares the ARM backend's
  address-materialization order.
- **Witness scan (2026-07-22, late):** eleven installed matches begin
  push, pool load, arg copy (e.g. [src/08019908.c](src/08019908.c),
  [src/08006384.c](src/08006384.c)). In every witness the pool load
  feeds a dependent dereference two slots later: the scheduler hoists
  the load for latency and fills the stall with the arg copy. The
  queue-push family's `ldr r4, =queue` has no nearby dependent use
  (the count read sits far below, behind the volatile IME store), so
  the witnesses' mechanism does not apply to the reconstructed shape —
  which suggests the original first statement created a short
  dependent chain on the queue base that the current reconstruction
  lacks, rather than a bare address materialization.
- **Recorded:** 2026-07-22.
- **Extends to an unrelated function, same root cause (2026-07-24):**
  `080043e0` (a 20-entry struct scan under the same IME-guard idiom, no
  family relation to the nine-member queue-push group) hits the identical
  head-order gap: the reference interleaves the entry-address load, the
  `-1` result init, and the IME save/disable in an order this compiler
  never reproduces, at exactly 20/64 mismatched bytes regardless of
  declaration order (flat, nested-block, swapped, no-intermediate-pointer
  — all four variants floor identically). `entry` has no dependent use
  near its load here either (first use is deep in the loop body, behind
  the volatile IME store), matching the queue-push family's structural
  signature. This confirms the gate is a general property of this head
  shape, not an artifact of one family's exact fingerprint — do not
  re-probe declaration-order/scheduling-flag variants on any function
  matching this shape (pool-load address with no nearby dependent use,
  immediately preceding an IME save) without new compiler-side evidence.
- **Third confirmed instance (2026-07-24):** `080042c8` (a 20-entry,
  8-byte-stride array scan at 0x03001A20 under the same IME guard, setting
  a flag bit on every entry matching arg0 or all entries if arg0==0,
  tracking the last matching index) hits the same gate: floors at 20/64
  mismatched bytes with the same `-1` init / entry-pointer load / IME
  save head-order gap, independent of the 5 variants tried. Same shape as
  `080043e0` (entry-pointer pool load with no dependent use before an IME
  save); reinforces treating this as a general compiler-head-scheduling
  limit rather than per-function bad luck.

### Thumb interworking call is never inlined

- **Claim:** GCC 2.96's Thumb backend has exactly one `call_indirect`
  pattern and it unconditionally emits `bl __call_via_rN` /
  `__interwork_call_via_rN` — confirmed directly in the vendored
  `gcc-2.96/gcc/config/arm/arm.md` (`TARGET_THUMB` branch of
  `call_indirect`/`call_value_indirect` is hardcoded to that template) and
  reproduced empirically across independent probe compiles: no C shape
  changes it. A function whose reference bytes instead call a fixed
  IWRAM/interworking-safe function pointer inline (`mov ip, pc; bx rN`
  with no `bl`, no `_call_via_` veneer) cannot be produced by any C source
  this compiler accepts.
- **Witness:** `08097a10` reconstructs semantics exactly (clamp/negate
  arg1, call `Func_080072f0`, mask the result, then call a function
  pointer stored at the fixed address 0x03000118) and reaches 68/68 bytes
  with the call site as the only remaining diff: reference emits
  `movs r0, r0 / mov ip, pc / bx r4` where every candidate emits
  `bl __call_via_r4`. This is the same inline-interworking-call idiom the
  overlay/manifest tooling already classifies as `nonstandard_thumb_call_module`
  for retained structural assembly — treat any function whose only gap is
  this exact call-site shape as a candidate for that classification rather
  than continued C-shape search.
- **Recorded:** 2026-07-24.

### Byte-store QImode constant reuse

- **Claim:** `0800651c` disables IME, zeros five word/halfword globals and
  two individual struct-field *bytes* through the same SImode zero
  register the word stores use, then restores IME. The reference reuses
  one zero register (r2) across every store including the two byte
  (QImode) stores; this compiler always allocates a fresh literal load for
  a byte store even when an SImode zero is already live in a register.
- **Disproved so far (2026-07-24):** five variants (plain casts, explicit
  `u8 *` field pointer local, reordered declarations, explicit named `zero`
  local, `u32` saved-IME value with inline casts) all reproduce the IME
  save/disable/restore idiom and the correct pointer/zeroing values, but
  none induce reuse of the SImode zero register for the QImode stores;
  best floor is 55 mismatched bytes (semantic/extra_instruction: a spilled
  6th live value or a detour around a bigger literal pool). Not yet tried:
  writing the two fields through the same pointer type as the words
  (`*(s32/u16 *)` casts on the struct-plus-offset address rather than a
  `u8 *` byte index), which might change how gcc classifies the store mode.
- **Recorded:** 2026-07-24.

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
- **Sharpened (2026-07-22, later):** the family extends to the alloca
  DMA staging function `080054e4` (family `180iruo8kyxn2`, four
  members), where the reference evacuates its three scalar arguments to
  r5/r6/ip because the store-multiple consumes hard r0-r2 — the
  signature of a fixed-register pattern. Two ideal-condition probes
  (`work/probe/stm_test.c`, `stm_test2.c`: values already in r0-r2,
  base r3, base live after) still emit three `str`. The approved
  compiler has not produced a non-block-move `stmia` in any C shape or
  flag probed. The alloca frame, single pool word via a `Value_`-style
  absolute size symbol, and `__builtin_alloca` availability under
  `-fno-builtin` are all confirmed reproductions from the same
  investigation.
- **Three-backend negative (2026-07-22, research pass):** both public
  agbcc Thumb backends (`gcc/thumb.md` and `gcc_arm/config/arm/thumb.md`)
  contain NO pattern emitting a three-register `stmia` from computed
  values: their block moves are strictly `ldmia`/`stmia` register-pair
  chunks (`movmem12b`/`movmem8b` via `thumb_expand_movstrqi`, align-4,
  ≤48 bytes), and the only direct multi-register store is the DImode
  pair `stmia {%1, %H1}`. Combined with our cc1's ideal-condition
  refusal, the parsimonious hypothesis is now that these sites were
  INLINE ASSEMBLY in the original source (period-normal for DMA
  macros): hard r0-r2 consumption forcing argument evacuation is
  exactly a fixed hand-written sequence's signature. If accepted, the
  sites are retained-structural kernels inside otherwise ordinary
  functions (mixed-region splits), not C debt. Positive-classification
  decision deferred to the classification framework and Pascal.
- **Fourth and fifth negatives (2026-07-22, late):** Rosetta-built
  gcc-3.0 emits three separate `str` under the same ideal conditions,
  and volatile scalar stores (the community DmaSet macro shape) emit
  three `str` under our cc1. New family member `08002f10` sharpens the
  story: its stmia source operand is a live call result and, like every
  member, the destination is a DMA-register block — reading as one
  project-wide DMA-kick macro using a store-multiple for compact atomic
  write ordering.
- **Double-kick exclusivity (2026-07-23):** the two grouped-store
  requirements are mutually exclusive in C for back-to-back kicks through
  one register block: three-wide register staging requires non-volatile
  stores (a value load cannot hoist above a preceding volatile store), and
  non-volatile makes the fully-overwritten first kick a deleted dead store.
  Verified both directions on 080a22f4 with a relaxed-matcher diagnostic
  build (volatile shape emits six strict-order single stores; non-volatile
  shape emits kick #2 only). The member is therefore positively classified
  deliberate_dma_kick_macro (retained structural, confidence strong) in
  asm/classification.json — the family's inline-asm-macro reading, applied.
- **Recorded:** 2026-07-22.

### Complement-form wide masks

- **Claim:** writing a clear-mask as a complement (`field & ~0x1FF`)
  preserves the wide pooled constant `0xFFFFFE00`, while the literal
  `0xFFFFFE00` is narrowed to `0xFE00` through a following u16 store's
  truncation and pools the narrowed word.
- **Current evidence:** `0801c154` candidate (`work/hand/0801c154/`),
  where the complement form fixed the pool word and dropped the
  mismatch from 6 to 4 bytes. Not yet backed by an installed match.
- **Next test:** apply to other masked read-modify-write halfword sites
  in the queue; promote once an exact match installs with the shape.
- **Recorded:** 2026-07-22.

### Shared-term cancellation across subtraction

- **Claim (agent lane, 2026-07-22):** at -O2 the approved compiler
  cancels a shared additive term across a subtraction —
  `(y1 + base) - (y0 + base)` folds to `y1 - y0` — where the reference
  build keeps both additions. `volatile s32` on the intermediates blocks
  the fold but permanently costs two stack slots (old GCC does not
  reuse volatile slots), leaving a +4-byte size gap. Second finding from
  the same investigation: inlining `+1` adjustments at their use sites
  reproduces the reference prologue's r8-spill exactly, while naming
  them promotes a spurious value to r10.
- **Current evidence:** `080164d4` best candidate (128 mismatched, size
  +4) and twelve falsified variants under `work/agents/080164d4/`.
- **Next test:** an anti-cancellation shape without volatile stack
  slots — e.g. reading one term through a call boundary or a
  known-clobbering construct; check the tilemap-clip cohort siblings.
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
