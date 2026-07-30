# Compiler law ledger

This ledger records compiler behavior that Alchemy has reproduced from its
approved evidence and exact, installed C matches. It prevents later sessions
from rediscovering the same source-shape constraints.

The clean-room evidence boundary in [PROVENANCE.md](PROVENANCE.md) and the
source-only publication gate in
[tools/check_publication.ts](tools/check_publication.ts) apply here.
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
  regions recorded in the tracked classification ledger and its generated
  manifest.
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
- **Governance:** `old_agbcc` is admitted only for the source-scoped regions
  whose compiler provenance and pinned bundle digests are recorded in
  [tools/alchemy_gcc.ts](tools/alchemy_gcc.ts). Other functions stay
  reconstruction assembly until they have their own exact-byte evidence.
- **Recorded:** 2026-07-22.
- **The stratum is wider than the audio bank (2026-07-24).** `08006c24` — a
  byte-compare helper in the `DEFAULT_ABI_SOURCES` library TU, nowhere near
  `rom_f9000` — is byte-exact 66/66 under `old_agbcc` with no extra sub-switch,
  and is the first non-m4a unit to earn `AGBCC_SOURCES` membership. Its
  already-matched sibling `08006b84` also compiles exact under `old_agbcc` from
  its committed `src/08006b84.c`; it matches under *both* compilers, which is
  why the family was never questioned. `08006c24` is the first member that
  discriminates, and the discrimination is threefold and measured, not inferred:
  (i) `REG_ALLOC_ORDER` `{3,2,1,0,12,14,...}` gives the fork's block-local temps
  r3/r2 and leaves r1/r0/r4 for the long-lived pointers, whereas `old_agbcc`
  allocates ascending so its locals take r0/r1 and its globals take r2/r3 —
  which is exactly the reference (counter r2, right pointer r3, left pointer
  r4), predicting all five global and all seven local assignments; (ii) the
  loop-invariant `0xFFFF` copy is given `HI_REGS` by `*thumb_movsi_insn`
  alternative 8's `*lh` constraint — the `*` suppresses `l` for regclass — so it
  lands in `ip` with `push {r4,lr}` where the reference has r5 with
  `push {r4,r5,lr}`, and no ordinary-C spelling removes the copy because the
  reference emits one too (`adds r5,r0,#0`); (iii) `arm_reorg` dumps the minipool
  at the *last* barrier in range and gcc-2.96's flow2 leaves a trailing barrier
  after the epilogue, which for any function this short is always within
  `fix->address + 1020`, so the fork emits the pool post-epilogue with no
  padding while the reference dumps at the barrier after the unconditional `b`
  and pays two bytes of `.align 2`. **Suspect, not yet proven:** the whole
  default-ABI family (`08006a00`, `08006b84`, `08006ba8`, `08006c24`,
  `08006dec`, `08007098`, `080fadf0`) is `old_agbcc`, and the
  `-fno-schedule-insns*` entry for `08006b84` in `UNSCHEDULED_SOURCES` is a
  fork-side workaround for that. Each member still needs its own exact-byte
  proof before it joins `AGBCC_SOURCES` — that rule does not relax.
- **`08006dec` immediately confirmed it (2026-07-24).** The flash-write region
  had survived seventeen fork compiler modes, an `s8`/`u8` typing fix and a
  dropped store, and still sat at 11 mismatched bytes with `register_only`
  dominant. Rerouted to `old_agbcc` unchanged it fell to 5 and the class flipped
  to `instruction_reorder`; one source edit then made it byte-exact 56/56. Two
  lessons: a stubborn `register_only` plateau inside a suspected-vintage TU is
  worth one reroute probe before any further source search, and the byte count
  understates how close a wrong-compiler candidate is — the C was already right.
- **`old_agbcc` has no post-reload scheduler to argue with.** It rejects
  `-fno-schedule-insns` and `-fno-schedule-insns2` outright, so emitted order is
  RTL order and every transposition is an expansion-order question, not a
  `rank_for_schedule` one. On `08006dec` the reference emits the argument's pool
  address *before* the reloaded `*source` byte, which is what forces the address
  into r3; the fork-era candidate emitted the byte first, leaving r0 dead and
  free so the address landed in r0. Giving the address its own named local
  assigned in its own statement after the aliasing store —
  `s32 *status; ... status = (s32 *)0x02004C00; ... f(..., *source, *status)` —
  puts that pseudo's birth ahead of the reload and closes all three
  instructions. Initialising the same local at its declaration instead widens
  its live range across the whole body and costs r5, `push {r4,r5,lr}` and 19
  instructions: for this pattern the assignment must sit between the store and
  the call.
- **`080fadf0` is the fourth family member proven, and it carries three
  reusable `old_agbcc` levers (2026-07-24).** The m4a panning/envelope routine
  (104B) matched on the third variant with no sub-switch. (i) **Zero-extension
  shape is chosen by the local's declared type, not by the casts.** The
  reference does `ldrb; lsls #24; lsrs #24` and later reuses the shifted value
  with `lsrs #25`. Declaring the loaded byte as a `u8` local is wrong — combine
  folds load+zero_extend into a bare `ldrb` and the halving collapses to
  `lsrs #1`. The redundant shift pair appears only when the value lives in an
  SImode pseudo truncated at each use: `u32 r = chan->rightVolume;` with
  `(u8) r` at every use. CSE then shares the `<<24` value between the compare
  operand and the `>>25` halving. (ii) **Post-reload cross-jumping does not
  merge duplicated tails**, because register allocation differs between the
  copies and cross-jumping declines. Writing the natural nested if/else with
  the tail duplicated in all four arms cost 97 of 104 bytes; explicit `goto`s
  that reproduce the reference's shared-block order got the CFG exactly. When
  the reference has one shared tail block reached by `b` from several arms, the
  C must have one shared label, not N copies. (iii) **`old_agbcc` evaluates the
  right operand of `+` and `*` first here**, so `a + b` emits `ldrb` of `b`
  then of `a`. On this region that operand order alone was the entire final
  6-byte residual. Also worth keeping: a store through a struct pointer may
  alias, so the following field reads reload from memory for free — no
  `volatile` and no barrier needed to reproduce a reference reload.
- **`regmove` runs before `local-alloc`, so some two-address operand choices
  are unreachable from C (2026-07-24).** `080a3d9c` closed to two bytes and
  stopped: `ands r2,r3` where the reference has `ands r3,r2`. The AND is a
  two-address `*thumb_andsi3_insn` whose operand 1 carries `%0`, so
  `regmove_optimize`'s forward pass may overwrite either input. It rejects the
  mask operand inside `fixup_match_1` at `reg_is_remote_constant_p` — CSE
  attaches `REG_EQUAL` to the constant load unconditionally, but that set lives
  in the entry block and is not reachable through `LOG_LINKS` — and retargets
  the loaded value instead. The reference needs no regmove fix at all, because
  local-alloc's `update_equiv_regs` later sinks the constant next to its use and
  ties the two pseudos. **The pass order is the whole point:** at regmove time
  the constant-materialisation pair is not in the stream yet (local-alloc
  inserts it), so there is nothing between the AND and its consumer for any
  source shape to interpose. Every documented exit from `fixup_match_1` was
  checked and measured against 48 variants; the ones that do flip the operand
  all cost more elsewhere, and the family has a hard floor of three mismatches.
  `-fno-regmove` on the identical source is byte-exact, so this earned a
  `NO_REGMOVE_SOURCES` registry entry. **The reusable test:** when a residual is
  a two-address instruction overwriting the wrong one of two inputs, read
  `.15.regmove` for `Fixed operand N of insn M` before spending variants — if
  regmove made the choice, the C cannot unmake it.
- **The reroute probe is bounded, and the bound is measured (2026-07-24).**
  Because `08006dec` fell to a reroute, every stubborn `register_only` plateau
  in the queue that no agent owned was rerouted through `old_agbcc` unchanged.
  All five got *worse*, none better: `08077394` 48→52 bytes (and 64/68 short),
  `080f9ef8` 46→44 but 24 instructions wrong, `080a3354` 50→110,
  `080fb2a4` 37→40 (32/40 short), `08004760` 26→42. Two of the five do not even
  reach the region size under `old_agbcc`, which is the strongest possible
  signal: the fork's minipool placement is load-bearing for them. So
  `register_only` on its own is *not* a wrong-compiler tell — the tell is
  `register_only` **plus** membership in a TU already known to be vintage. The
  `old_agbcc` stratum stays confined to the m4a bank and the `08006xxx`
  default-ABI library TU; do not spend another probe on a fork region merely
  because its plateau is register-shaped. Note especially that `080f9ef8` and
  `080fb2a4` bracket `080fadf0` in address space and still reroute badly, so
  address adjacency to a vintage unit is not evidence of shared provenance
  either.

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

### Let the compiler derive its own induction variable

- **Fingerprint:** the reference walks an array with an auto-incrementing
  pointer (`ldmia rN!, {r0}`) driven by a *countdown* register compared against
  zero (`subs r6, #1; cmp r6, #0; bge`), even though the loop visits entries
  0..n-1 in ascending order. Hand-writing that shape in C — an explicit
  `entry = state->objects` pointer plus an explicit `remaining = n - 1`
  countdown — reproduces the loop body byte-for-byte but misorders the
  preheader.
- **Producing idiom:** write the loop the way a person would have written it,
  `for (index = 0; index < n; index++)` over `state->objects[index]`, and let
  `-fstrength-reduce` invent both the pointer and the countdown. The derived
  induction variables are created *after* the other preheader values, so they
  are emitted last, which is exactly where the reference puts them.
- **Why it is worth a law:** for [src/080a9d84.c](src/080a9d84.c) the
  hand-written countdown form was already correct in every instruction of the
  loop body and off only in the six-instruction preheader. A 579-variant
  exhaustive sweep over every legal declaration permutation and every
  literal-versus-named-local choice of the three loop-invariant constants
  bottomed out at 10 byte mismatches and could not close it; neither could any
  of seven candidate compiler-mode flags (`-mhigh-register-move-first`,
  `-mearly-frame-allocation`, `-fno-schedule-insns2`, `-fno-gcse`,
  `-fno-strength-reduce`, `-fno-expensive-optimizations`,
  `-fno-cse-follow-jumps`). Switching to the canonical forward `for` loop went
  to zero on the first try. When a candidate is close and the residue is
  preheader ordering, suspect a hand-derived induction variable before
  suspecting a compiler flag.
- **Caution about siblings:** the structural sibling that produced this
  candidate, [src/080a9cbc.c](src/080a9cbc.c), *is* installed in the
  hand-written countdown form. A sibling's shape being byte-exact for its own
  region does not make that shape canonical; two different sources can converge
  on the same code when the preheader has fewer values to order.
- **Confirmed:** 2026-07-24.

### Name the index when the reference keeps a register-offset load

- **Fingerprint:** the reference computes an array index into a register of its
  own and then loads with a register-offset address (`adds r3, r3, r2;
  ldrsb r6, [r6, r3]`), keeping the array base in the base register. The
  obvious C — the index expression written inline inside the subscript or the
  cast — instead folds the base into the address and reassociates, emitting an
  extra add of the base plus a zero index register. On
  [src/080a6a00.c](src/080a6a00.c) that cost four bytes and 108 mismatches.
- **Producing idiom:** assign the index expression to its own local first, then
  subscript with that local:

  ```c
  slot = entries[offset] + 0x260;
  value = ((s8 *)base)[slot];
  ```

  Spelling the same thing as `((s8 *)base)[entries[offset] + 0x260]` does not
  work, and neither does parenthesizing the addition inside a pointer cast —
  the compiler reassociates through both.
- **Scope:** this is the reason an m2c-style `static __inline__` load helper can
  match when its manual expansion does not. The helper's parameters force the
  index to exist as a value; the local reproduces that without the helper,
  which `src/` does not allow. When a verified candidate carries such a helper,
  reach for a named local before assuming the helper itself was load-bearing.
- **Relation to temp count:** consistent with the [src/08077cb8.c](src/08077cb8.c)
  finding that the *number* of distinct temporaries matters and their names do
  not. Here too the lever is whether a value gets a temporary at all.
- **Confirmed:** 2026-07-24.

### A two-byte shortfall is a region boundary, not a source shape

- **Fingerprint:** the candidate disassembles identically to the reference —
  `instruction_mismatches` is 0 and `dominant` is `exact` — but `actual_size`
  is two bytes under `expected_size`, and those two bytes are the whole of
  `byte_mismatches`. The reconstruction assembly ends with a lone
  `movs r0, r0` after the final `bx`, which is the encoding of a zero
  halfword.
- **Why no source can close it:** that halfword is the linker aligning the
  *next* Thumb function to four bytes. A translation unit holding a single
  function has nothing following it to align against, so the compiler never
  emits it, and `verifyCandidate` slices the candidate at the linked symbol
  size, which stops at `bx`. No attribute, dummy symbol, or trailing
  declaration moves it.
- **Producing action:** this is a region split, not a decompilation. Drop the
  trailing `movs r0, r0` from `asm/<stem>.s`, add the padding address to
  `asm/alignment.json`, and raise `alignment_padding` in
  `asm/classification.json` by one file and two bytes. Then the C installs
  normally.
- **Evidence:** [src/0809a3c4.c](src/0809a3c4.c), 134 bytes, whose region was
  136. `65c2100a` had already made the same split at `0x080a9a56` for
  [src/080a99b0.c](src/080a99b0.c), from the other direction — padding
  *between* two functions of one region rather than at its end.
- **Scope:** applies only when the trailing bytes are zero. A region ending in
  a stray `bx lr` (`0x4770`) — `asm/08093054.s` is one — is a different
  structural category and is not alignment padding.
- **Confirmed:** 2026-07-24.

### Inline helpers can be load-bearing, and the integrator's guard is a heuristic

- **Fingerprint:** `tools/integrate_matches.ts` rejects any candidate matching
  `/inline_fn|^(static|inline)\b/m` as "carries an m2c helper". That guard
  exists to keep raw m2c output out of `src/`, but it is a text test, not a
  judgement about whether the helper is doing work.
- **Evidence:** [src/08011164.c](src/08011164.c) keeps a `static __inline__`
  two-halfword copy helper and matches exactly at 80 bytes. Expanding the
  helper by hand into the loop body — the remedy the register-offset law
  recommends trying first — gives 76 bytes and 48 mismatches. Its adjacent
  sibling [src/080110e0.c](src/080110e0.c) carries the identical helper and
  was installed the same way; nine `src/` files use the idiom.
- **Producing action:** try the named-local expansion first, because when it
  works the result is cleaner and installs automatically. When it does not,
  install by hand and say in the commit message why the helper is
  load-bearing. Do not treat the rejection as a verdict on the match.
- **Confirmed:** 2026-07-24.

### Loop syntax picks one of three `expand_end_loop` outcomes

- **Fingerprint:** a candidate whose arithmetic is already right has its loop
  bodies emitted in the wrong order, or ends a loop with `bne <outer head>` +
  `b <body>` where the reference has `beq <body>` + `b <outer head>`. Register
  allocation is not involved; the instructions are the right ones in the wrong
  places, and the mismatch count is large out of proportion to how wrong the
  source looks.
- **Mechanism.** `expand_end_loop` (`gcc-2.96/gcc/stmt.c:2256`) runs two
  transforms in sequence, and which one fires is decided by how the loop is
  written:
  - Its *first* transform looks for a conditional jump feeding an
    unconditional jump to the loop's `end_label`, retargets the conditional
    straight at `start_label`, and sets `needs_end_jump = 0` (`stmt.c:2325-2329`).
  - Its *second* transform — "roll the exit test to the end", `stmt.c:2338` —
    is gated on `needs_end_jump` still being 1 (`stmt.c:2367`) and moves the
    top test to the bottom, reordering the body around it. Its scan accepts a
    jump only when the destination is the loop's own `end_label` or
    `alt_end_label` (`stmt.c:2479-2484`).

  Three consequences follow, and all three are source-visible choices:
  - **`break` rolls the loop; `goto` past it does not.** `break` compiles to
    exactly the `end_label` jump the roll scans for. A `goto` to a label placed
    *after* the loop is a different label, the scan finds nothing, and the body
    keeps program order. The loop's own spelling is irrelevant — `for(;;)`,
    `while (1)`, `do {} while (1)` and a bare backward `goto` all emit the same
    bytes.
  - **A bottom-tested `do`-`while` removes the trailing `b <newstart>`
    entirely,** because the first transform clears `needs_end_jump`. This is
    not cosmetic: with a top-tested loop the emitted tail is
    `bne Lend; b newstart; BARRIER; Lend:`, and whether that becomes the
    reference's inverted `beq newstart` depends on what follows `Lend`. When
    real code follows, `jump.c:434`'s "conditional jump jumping over an
    unconditional jump" inverts it and the bytes match. When another loop's
    back-edge follows, `follow_jumps` (`jump.c:348`, which runs earlier in the
    same per-insn iteration) first threads `bne Lend` to that back-edge, and
    the inversion can no longer apply. The same source shape therefore matches
    in one loop and misses by three bytes in its identical twin, purely
    because of what sits after it.
  - **Once `needs_end_jump` is 0, a loop's entry guard has to be written in the
    source.** `duplicate_loop_exit_test` needs a `NOTE_INSN_LOOP_BEG` whose
    next non-note insn is an unconditional jump (`jump.c:318-325`); suppressing
    the roll means the loop no longer starts with one, so the compiler will not
    synthesize the guard and an unguarded `do`-`while` is simply a different
    function.
- **Producing idiom:** when a loop's body is emitted in the wrong order, change
  how the loop is *left* before touching what is in it — `goto` past the loop
  instead of `break`. When a loop's bottom is the wrong branch, make it
  bottom-tested (`do { ...; if (++i > N) break; } while (cond);`) and write the
  entry guard explicitly.
- **Scope:** the mechanism is read from the fork's own source and applies to
  every loop it compiles, but the producing idiom is confirmed on one witness.
  Do not assume `goto` is always the right exit; it is the right exit when the
  reference did not roll.
- **Evidence:** exact installed match
  [assets/code/resource_36f_c_02000054.c](assets/code/resource_36f_c_02000054.c),
  364 bytes, the largest overlay region converted so far. Its three loops
  exercise all three consequences: the outer dispatch loop needs the `goto`
  exit, both waits need the bottom test, and both need written guards at
  `0x02000132` and `0x0200008a`. The measured ladder was 263 mismatches with
  top-tested waits and a `break` exit, 75 with `break` alone, 3 with the `goto`
  exit, and 0 once the waits became guarded `do`-`while`s.
- **Confirmed:** 2026-07-25.

### A mid-function literal pool is a compiler layout choice, not a source shape

Some overlay functions place their literal pool between the entry block and the
loop that uses it, with an unconditional branch over the pool, where the
reconstruction places the same two words after the return and needs no branch.
The two layouts are otherwise instruction-for-instruction identical, so no
rewrite of the C changes the outcome: source order controls which word lands
first in the pool, but nothing in C controls where the pool is dumped.

Rule out the shared-region reading before spending on this. A branch over a pool
looks exactly like a branch into a region shared with another function, and the
two call for opposite responses. Decide it by scanning the overlay for branches
that target the loop head: `resource_39b:0e50` and `resource_39c:0e10` are
reached only from their own entry block and their own back-edge, which makes the
branch a pool skip and the function an ordinary one.

GCC dumps a pending pool at a barrier and otherwise defers it to the end of the
function, inserting a jump around the table when it dumps early. A function this
short gives it no reason to dump early, so the gate is closed by
`-mthumb-early-literal-pool` in alchemy-gcc, which searches from the last real
fix up to, but not into, the natural barrier and lets the existing barrier cost
pick the split. Route an overlay to it through
`EARLY_LITERAL_POOL_OVERLAY_SOURCES`.

The flag only restores the layout. Two source-level facts closed the last six
bytes of the pair that motivated it, and both are worth trying on any function
this flag is applied to. Instruction scheduling must stay **on**: it is what
groups the two pool loads ahead of the counter's `movs`, and turning it off to
control source order breaks that grouping. And the loop counter must be declared
ahead of the pointer, which is what gives the counter `r2` and leaves the
pointer `r3`; declaring the pointer first swaps the two and costs six bytes that
no optimisation flag recovers (`-fno-regmove`, `-fno-strength-reduce`,
`-fno-cse-follow-jumps`, `-fno-expensive-optimizations`, `-fno-gcse`,
`-fno-force-mem` and `-fno-thread-jumps` were each measured at no effect).

- **Confirmed:** 2026-07-25. Two members, `resource_39b:0e3c` and
  `resource_39c:0dfc`, both 34 bytes and sharing one shape. The draft reaches
  the reference's exact instruction sequence and register allocation; the
  residual 29 mismatched bytes are entirely the shift from pool placement.
  Reordering the source to put the value ahead of the pointer changed the pool
  word order without moving the pool, leaving the count unchanged at 29. Closed
  the same day: the flag took the pair to 11 mismatched bytes, scheduling and
  declaration order took it to 0, and both members were adopted. A first count put eighteen of the
  thirty-three functions then remaining on this layout, but that scan treated
  any short forward branch as a pool skip and so counted every `if`/`else` join
  as well. Counting properly, by asking whether a referenced pool word sits
  before the function's own `bx`, gives **two** of the thirty-one now remaining:
  `resource_3c0:0ce4` and `resource_3a4:39c8`. The flag is correct and the pair
  it was built for is converted, but it unblocks a handful of functions, not a
  majority. Decide this by pool-word address, never by branch shape.

### A symbol's address load is placed by reload, not by expansion order

Several overlay tails load the stored value through a pointer and only then
materialise the destination's address, where the reconstruction materialises the
destination first. The two differ by about five bytes and by which of `r2` and
`r3` holds what, with the instruction count identical.

Source shape does not reach it. Nine forms were measured on `resource_397:026c`
-- ternary, ternary through a pointer, explicit pointer select, a value local in
both declaration orders, two separate stores in both polarities, and a widened
compare. Pointer select is the best at five mismatched bytes and the two-store
form at six reproduces the reference's cross-jumped branches exactly, and still
hoists the address load. Roughly fifteen flags were measured at no effect,
including every scheduler lever and both fork-specific scheduling flags.

Expansion order does not reach it either, and this is the useful part. Turning
off post-reload scheduling leaves the tail destination-first, which rules out
the scheduler. A `-fstore-value-first` flag was then built, extending
`expand_assignment`'s existing rhs-before-lhs path (the one gcc already uses for
calls) to a scalar load through a pointer; it was confirmed present in the built
`cc1` and changed no byte, alone or with scheduling off. The reason is that on
Thumb a MEM whose address is a `symbol_ref` has no address load at expansion
time at all: the load is created when reload legitimises the address. Ordering
anything earlier cannot move an instruction that does not exist yet. That flag
was reverted rather than left in the fork as a no-op.

So this gate lives in ARM address legitimisation during reload, which is a much
deeper change than the pool-placement flag, and it should be sized as such
before anyone starts. Do not spend another sweep on source forms or on
scheduling flags; both are exhausted and recorded here.

- **Confirmed:** 2026-07-25. `resource_397:026c`, a VCOUNT-gated scroll write,
  held at five mismatched bytes throughout.

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

### In-place pointer advance and preheader statement order (2026-07-24)

- **Claim:** two separate shapes, both surfaced by `080a9d3c`. (a) When the
  reference advances a pointer in place (`adds r5, #200`), the source must
  advance *the same variable it later loads from* — deriving a second pointer
  from a first costs a `adds rD, rS, #0` copy that no flag removes. (b)
  Instructions in a loop preheader are emitted in **source statement order**,
  so hoisting `for` initializers out of the loop header and ordering them
  deliberately is a real lever, not cosmetic.
- **Current evidence:** on `080a9d3c` (72 bytes, `-fno-strength-reduce`),
  rewriting the walk as a hand-written `*slot++` over an in-place-advanced
  pointer took the candidate 59 → 10 mismatches; hoisting the `for`
  initializers out of the header and permuting their order took it 10 → 7.
  At 7 the size is correct and the instruction sequence is identical to the
  reference — the only residue is that `index` and the walking pointer hold
  each other's call-saved registers. Not backed by an installed match.
- **Also observed (negative):** declaration order, loop form (`for` / `while` /
  `do`-`while`), and bound spelling (`index <= 4` vs `index < 5`) are all
  canonicalized away and never moved a byte.
- **Next test:** the residual swap survived 48-, 36-, 10- and 12-variant
  source sweeps plus two full 15-flag allowlist sweeps, so the remaining lever
  is likely allocator-internal rather than source-level. Promote on the first
  exact install carrying either shape.
- **Recorded:** 2026-07-24.

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
- **Correction (2026-07-30):** the second half of that claim is wrong, and the
  real blocker is narrower. Measured on 080f377c (three kicks) and 0800300c (two
  kicks) with a `volatile` descriptor plus `-mgrouped-dma-store`: **every group
  but the last one forms** — 2 `stmia`s of 3 and 1 of 2 respectively. So the
  peephole does fire on volatile MEMs. What defeats the final group is that its
  three values never land in three consecutive ascending registers. Attack that
  register-assignment problem, not the peephole's volatile handling
  (work/claude/notes/main-grouped-dma-multi-kick.md).
- **Current evidence:** 080a22f4 (48-byte leaf, two kicks): kick #2
  reproduced byte-exact under the mode; best full-function floor 37
  mismatches (work/agents/080a22f4/NOTES.md).
- **Next test:** extend the mode in alchemy-gcc so the DMA block survives
  dead-store elimination (hardware memory class or earlier peephole), then
  allowlist 080a22f4; else classify the double-kick member as
  retained-structural per the family's pre-flag inline-asm hypothesis.
- **Recorded:** 2026-07-23.

### Grouped-DMA descriptor group is not atomic to the scheduler (2026-07-24)

- **Claim:** `-mgrouped-dma-store` produces the right `stmia rN!, {…}; subs rN,
  #12` pair, but leaves the two pool loads that feed it (`ldr` of the register
  base and of the control word) as ordinary insns. The second scheduling pass
  is then free to hoist them — and the `add sp` — above unrelated work in the
  same block. The reference keeps base load, control load, `stmia` and `subs`
  as one uninterrupted run, which is what a single output template emitting its
  own pool references would give.
- **Current evidence:** two independent regions, both at the correct size with
  the correct instruction multiset, differing from the reference *only* in where
  those pool loads sit.
  - `080c08a8` (56 bytes) pins at **18** mismatches under `-fno-schedule-insns2`
    and **26** with scheduling on; the residual is the two loads and `add sp`
    floating above the zeroing store.
  - `0800d304` (60 bytes) pins at **12** under the mode alone — six instructions
    transposed, no more. The reference runs `lsrs`/`lsls` *first* and the two
    pool loads after; ours hoists both loads to the top of the block.
- **What the set pins down:** `0800d304` gets *worse* with
  `-fno-schedule-insns2` (12 → 14), because disabling sched2 also loses the
  `adds r1` / `orrs r2` order the reference does have. So the reference is
  normally scheduled — it is specifically the descriptor's own pool loads that
  will not move. That is the signature of a group emitted as one template, not
  of scheduling being off.
- **Corollary, and the reason two of these installed:** when the descriptor
  block is the *only* thing in the function with anything to schedule, killing
  sched2 outright is indistinguishable from pinning the group, so the region
  matches exactly. [src/08002f10.c](src/08002f10.c) is that case — 4 mismatches
  under the mode alone, **0** at 44 bytes with `UNSCHEDULED_SOURCES` added, and
  its whole residual was the control-word pool load sitting one slot early.
  [src/0800bc48.c](src/0800bc48.c) needed no second mode at all: **0** at 40
  bytes on the routing alone. So the gap only bites where the function has
  other schedulable work — which is exactly why it went unnoticed until three
  larger regions hit it at once.
- **Why the twin matched:** `080284dc` is byte-exact under the same mode because
  a `bl` sits after its descriptor block and fences it, so there is nothing for
  the loads to hoist past. The match is an accident of surrounding code, not a
  better-shaped source — worth remembering before reading any single grouped-DMA
  match as proof the mode is complete.
- **Also observed (negative):** the residual is not source-position. Sinking the
  competing load (`object = *(u32 **)0x03001f00`) to just before its use makes
  it *worse* (33 scheduled / 42 unscheduled), and dropping the temp entirely
  scores the same — the scheduler, not the source order, decides.
- **Fifth witness, and it names the pass that does the damage.** `08005a78`
  (72 bytes, the flash-read DMA kick) sits at **15** mismatches unrouted, **17**
  on `-mgrouped-dma-store` alone and **11** with `-fno-schedule-insns2` as well;
  at 11 every remaining pair is a transposition — `register_only` 0,
  `semantic` 0, `instruction_reorder` 6. Its `.23.sched2` dump shows the
  scheduler *picking the reference order* in the preheader (ready lists at
  t=43/44/46 choose `movs r2,#128`, then `ldr r1,=0x040000D4`, then `lsls`), and
  `thumb_order_grouped_dma_store`'s third loop — the one headed "Keep a
  small-immediate construction contiguous", arm.c ~6520-6543 — then rewriting
  `[set REG const][set REG2 const][set REG (ashift REG n)]` into
  `[move][shift][load]` and destroying it. So the fixup that installs the group
  is itself the obstacle, and no RTL order survives it: any order that would
  print the reference's gets rewritten, and only `ldr`-first matches its
  pattern. That is a sharper statement than "the scheduler decides" and it is
  the same loop the next alchemy-gcc change has to touch.
- **Two of its three residual transpositions are separately pinned.** The
  `adds r0, r5, #0` / `adds r0, #80` pair is a reload copy, which reload inserts
  immediately before the add, so RTL order can place the descriptor's base
  literal before or after the pair but never inside it — interleaving needs
  post-reload scheduling, and with sched2 on the block's priorities put
  `ldr r2,=0x840003FC` (priority 4) ahead of `adds r0,#80` (priority 3) while
  the reference emits it last. And the entry `adds r6, r1, #0` argument copy is
  emitted by `assign_parms` ahead of all body insns, so with sched2 off the
  entry literal can never precede it. `-mthumb-entry-literal-first` exists for
  exactly that shape but rejects this one on both guards (it needs the literal's
  destination in r4-r7, ours is r3; it needs a `CONSTANT_POOL` MEM source, ours
  is still `CONST_INT` at `arm_reorg`).
- **Also observed (shape, not scheduling).** Two of `08005a78`'s original 41
  mismatches were nothing to do with the family. Writing the descriptor through
  three independent absolute addresses costs a second pool word (0x040000D8) and
  an extra load; one struct pointer gives one base and one pool word, which is
  the 4-byte size fix. And the completion spin must read through a `volatile`
  pointer or GCC folds it into an infinite loop with no memory access at all.
- **Next test:** make the descriptor group a single output template in
  alchemy-gcc so sched2 cannot split it, then re-measure `080c08a8`,
  `0800d304` and `08005a78` together; a toolchain change, so it belongs in the
  alchemy-gcc lane rather than a batch.
- **Recorded:** 2026-07-24.

### An old_agbcc region can be pinned by one callee-saved choice (2026-07-24)

- **Claim:** `080fb2a4` (40 bytes, the m4a tempo control) belongs to the
  `AGBCC_SOURCES` family on every structural test, and still cannot be admitted,
  because the reference gives a *short-lived* quantity a callee-saved register
  while a call-clobbered one is provably free. Membership needs an exact-byte
  proof; this one stops one allocation short of it.
- **Why the family is certain.** Under `old_agbcc` the candidate reproduces all
  thirteen body instructions in the reference's exact order — same opcodes, same
  operand roles, same literal pool. Under the routed Camelot fork it is
  structurally impossible on three independent counts, none of which any C
  reaches: the fork sign-extends the incoming `u16` parameter (`lsls; asrs`)
  whether it is spelled `u16`, `s16` or `unsigned short`, where reference and
  `old_agbcc` both emit `lsls; lsrs`; it never emits the `adds r2, r0, #0`
  argument copy; and it re-loads `ldrh r3, [r0, #30]` instead of forwarding the
  just-stored field. The `gs2` bundle is byte-identical to the fork on all
  three. The region also sits between `080facf8` and `080fb2cc`, both already
  listed.
- **What is left.** Reference: `lsrs r1, r1, #16` (tempo stays in its argument
  register), `ldr r3, [r2, #52]` (ident), `ldrh r4, [r2, #28]` (the multiplier).
  Ours: tempo to `r3`, ident to `r1`, and the multiplier reusing `r1` after the
  compare kills it — four bytes shorter, because reusing a low register means no
  `push {r4, lr}` / `pop {r4}; pop {r0}; bx r0`. The reference's `r3` is dead at
  its `ldrh` and it still reached past it for `r4`.
- **What was ruled out.** Ten source shapes under `old_agbcc` — named tempo
  temporary, both multiply operand orders, named `u32` multiplier, both operands
  named, a real struct typing, if-wrapped body versus early return, `s32`
  parameter narrowed inside, named ident local, `register` on the parameter, and
  a named `s32` product — every one produced byte-identical output at 32 bytes.
  So did `-O1`, `-O3`, `-fno-cse-follow-jumps`, `-fno-gcse`,
  `-fno-expensive-optimizations`, `-fcaller-saves`, `-fno-defer-pop`, and each
  of the four `old_agbcc` mode switches. `-ffixed-r3` reaches the right size and
  frame and leaves a clean 7 bytes, but it is a false lead and must not be
  installed: the reference *uses* `r3`, for the ident it compares.
- **Where it goes.** Not a source-shape park and not a C problem — an allocation
  order in a binary we did not build. Leave `080fb2a4` in assembly and out of
  `AGBCC_SOURCES` until the alchemy-gcc lane can explain why the reference
  preferred `r4` over a free `r3`; the candidate is saved so the measurement
  does not have to be repeated.
- **Recorded:** 2026-07-24.

### The four-word-record transform is over-fitted to its first witness (2026-07-24)

- **Claim:** `thumb_group_four_word_records` — the half of
  `-mgrouped-dma-store` that emits `stmia rN!, {r1,r2,r3,r4}` for 12-word
  block initialisation — is written against the one region it was derived from.
  It requires the base to be **r3** and to be **dead at the twelfth store**.
  Neither holds for a stack-allocated block or for any function that consumes a
  call's return value (`regs_ever_live[0]` disables it there), so the transform
  cannot fire for whole families that visibly use the idiom in the ROM.
- **Current evidence:** three batch-8 regions independently hit the same wall,
  each after the drafting agent had confirmed semantics and reached a
  byte-correct head and pool: `080049ac` (51, global base, blocked by the
  return-value guard), `08004cb4` (56, stack base), `08004cf0` (54, stack base
  staying live for three diagonal stores). A fourth, `080bd7a4` (52), is
  blocked by the volatile half of the same mode: the peephole fires only on
  non-volatile MEMs, but without `volatile` CSE folds its three identical
  descriptor writes into one, so no C shape yields all three `stmia` groups.
- **Why this is worth stating separately:** these four are not near-misses to be
  ground down with more source variants — the agents had already run their
  budgets out against a transform that provably cannot fire. Four regions'
  worth of effort was spent discovering one toolchain limitation four times.
- **Next test:** in the alchemy-gcc lane, relax the base-register and liveness
  guards and re-measure all four; any such change must re-verify the existing
  grouped-DMA installs, since it widens a transform they already depend on.
- **Recorded:** 2026-07-24.

### Four Camelot Thumb switches are reachable but unevidenced (2026-07-24)

- **Claim:** `arm.h` in the approved 2.96 tree defines four `-m` switches that
  no registry entry uses and no match has ever needed:
  `-mno-sched-prolog`, `-mentry-low-register-order`,
  `-mpreserve-single-bit-test`, and `-mthumb-and-sets-cc`. They are reachable
  from `extraCompilerFlags` today, so they belong in every sweep, but nothing
  yet establishes that any Golden Sun object was built with one.
- **Current evidence:** swept against the three best-placed near-misses —
  `0808fecc` (4), `080b2720` (4), `08006dec` (6) — alone and on top of each
  stem's known-best mode. `-mpreserve-single-bit-test` and
  `-mthumb-and-sets-cc` changed nothing at all on any of the three;
  `-mentry-low-register-order` made all three strictly worse (15, 20, 11);
  `-mno-sched-prolog` made two worse and left `08006dec` unchanged.
- **What that pins down:** `-mno-sched-prolog` is the informative one. On
  `0808fecc` it holds `sub sp, #4` at the top of the prologue while the
  reference schedules it down past three argument setup insns — so the
  reference's prologue *is* scheduled, and the switch is not a Camelot default.
  It also leaves `0808fecc`'s epilogue transposition untouched, which rules the
  scheduler out as the cause of that residual and points at the order in which
  the function-end expander emits the return-value copy and the stack restore.
- **Why record a negative:** all four are cheap to try and expensive to
  rediscover. Without this entry the next agent finds them in `arm.h`, assumes
  an unexplored lever, and spends a budget re-measuring the same three stems.
- **Recorded:** 2026-07-24.

### The post-call return-value copy cannot be sunk from C (2026-07-24)

- **Claim:** when the only residual is that the reference emits the
  `adds rN, r0, #0` copy of a call's return value *after* the following stores
  and the candidate emits it immediately after the `bl`, no ordinary C shape
  fixes it. The decision is made inside sched2 by a priority comparison whose
  inputs are not expressible in source.
- **Evidence:** `08095fcc`, 124 bytes, 6 mismatches, one instruction. A
  `-fsched-verbose=5` trace of basic block 0 gives the copy and the competing
  `subs r1, #1` equal priority 29 and the `strh` priority 28. The copy wins the
  tie on dependent count (5 against 3), which `rank_for_schedule` weighs ahead
  of `INSN_LUID`, so even reversing the statements in the source cannot flip it.
  The `strh`'s priority is *capped* at 28 by an r3 anti-dependence on the
  address load that begins the local-struct copy chain, so it can never rise
  above the copy's. Turning sched2 off is worse (12+); sched1 is a no-op here.
- **Search already spent:** 171 variants. All 40 legal orderings of the five
  post-call statements — best three tie at 6. Moving the call after the
  decrement costs 131. Separate temporaries for the call result, for both call
  arguments, and for the two struct loads; every declaration permutation; eight
  counter forms; commutative reorderings; struct-pointer and array retypings.
  Declaration order had literally no effect.
- **Why it matters beyond this region:** this is the same residual class the
  registry already records for `0808fecc` — "the return-value copy and the
  stack restore in the opposite order" — and the preceding law shows
  `-mno-sched-prolog` leaves that one untouched. Two independent witnesses now
  point at one missing mode in the family of `-mcall-arg0-move-first` and
  `-mhigh-register-move-first`: a return-value copy that stays put. That is
  alchemy-gcc lane work, not source work, and both stems should be re-measured
  the day such a switch exists.
- **Recorded:** 2026-07-24.

### An entry-block pool load never chooses r4 (2026-07-24)

- **Claim:** when the reference's first instruction loads a literal pool word
  into `r4` and the candidate uses `r3`, the difference is not reachable from C.
  `REG_ALLOC_ORDER` in the approved tree's `arm.h` lists `{3, 2, 1, 0, 12, 14,
  4, 5, 6, 7, ...}`, so a block-local quantity in the entry block first-fits to
  `r3` unconditionally; `r4` requires `r0`–`r3` to be simultaneously busy at
  that point, which in the entry block means live incoming parameters.
- **Evidence:** `08096c80`, 92 bytes, now 2 mismatches. `r4` was reproduced
  only by giving the function four parameters live across the load — which
  changes the size. Ruled out at `r3`: every declaration permutation; four
  typings of the base pointer; extra named temporaries for the address and for
  the inner load; `M2C_FIELD` and array-subscript forms; an `extern` symbol
  instead of the integer literal; `volatile` and `const` qualification; a split
  address literal; `register` on either or both pointers. All six fork `-m`
  switches were checked for information: none yields `r4`, and
  `-mentry-low-register-order` yields `r0`, consistent with it permuting block
  0's low order to `{0, 1, 3, 2}`.
- **Corroboration:** across the whole ROM only 21 functions load a pool literal
  into `r4`, all large and high-pressure, and none of the 1,222 matched sources
  in `src/` does. The reference's choice here is anomalous for the fork's
  allocation order, which is itself the evidence that the original compiler
  ordered the entry block differently.
- **Recorded:** 2026-07-24.

### Local-allocator priority is a function of the instruction stream, not of the source (2026-07-24)

- **Claim:** when two quantities are swapped between registers and nothing else
  differs, the deciding quantity is `QTY_CMP_PRI` in `local-alloc.c`,
  `floor_log2(n_refs) * n_refs * size / (death - birth)`. A source edit can only
  move that ratio by changing a reference count or a live span, and both are
  usually pinned by call clobbers and by the position of the region's calls. When
  they are pinned, the swap is unreachable from C.
- **Correction (2026-07-24, later): `size` is in WORDS, not bytes.** It is set
  from `PSEUDO_REGNO_SIZE` (regs.h:92, `alloc_qty` call at local-alloc.c:1861),
  which is `(GET_MODE_SIZE + UNITS_PER_WORD - 1) / UNITS_PER_WORD`. On this target
  that is **1 for every scalar mode** — QImode, HImode and SImode all score the
  same, and only DImode is 2. So the factor drops out of every comparison you will
  actually make, and a byte quantity competes with a word quantity on equal terms.
  Reading it as bytes inflates every pointer or `int` quantity fourfold against
  every `u8` one and predicts the wrong register; that is exactly how `08006dec`
  came to be recorded as inseparable.
- **Correction (2026-07-24, later):** the denominator is NOT the `.lreg` header's
  "across L insns". `qty[q].birth` and `qty[q].death` are set by
  `reg_is_born (reg, 2 * insn_number)` and
  `qty->death = 2 * this_insn_number + output_p` (local-alloc.c:1687, 1926), where
  `insn_number` is reset per basic block in `block_alloc`. So the divisor is
  **2 × (death insn index − birth insn index) within the one basic block**, while
  the header's `L` is `reg_live_length`, a whole-function count. The two agree
  only for a single-block region. `global.c`'s `allocno_compare` genuinely does
  use `live_length`, which is why the header value is right for the global pass
  and wrong for the local one. Using `L` for a local-alloc question predicts the
  outcome backwards — see the `0801fd34` law below, where it did.
- **Which pass owns a pseudo, read straight off the dumps:** `dump_conflicts`
  (global.c:1869-1875) builds the `;; N regs to allocate:` line by skipping every
  allocno with `reg_renumber >= 0`, i.e. every pseudo **local-alloc already
  assigned**. So a pseudo that *appears* on that line was left to `global_alloc`
  and its divisor is the header's `L`, with the line itself already printed in
  priority order — no arithmetic needed. A pseudo that does *not* appear was taken
  by `local-alloc`, and its divisor is the doubled per-block span above, which you
  must read off the `.lreg` RTL. This is the discriminator; apply it before
  computing anything. Note that `;; 0 regs to allocate:` is a *complete* answer,
  not an empty one: it means local-alloc assigned everything and the global model
  does not apply anywhere in the function. `08006dec` prints exactly that, so the
  measurement once recorded below for it used the wrong divisor; see the
  retraction in its own paragraph.
- **Evidence, `080a3e88`** (102-byte region, parked at 8 mismatches): the whole
  instruction stream agrees; only `arg0` and `arg1` exchange `r6` and `r8`. The
  park stands, but its arithmetic was re-derived on 2026-07-24 after the divisor
  and `size` corrections above, because the original numbers used the header's
  `L`. The `.greg` line is `;; 1 regs to allocate: 34`, so `arg0`, `arg1` and
  `ptr` are all local-alloc's and all take the doubled in-block span. Reading the
  births and deaths off the `.lreg` RTL by in-block index: `ptr` is born at 8 and
  dies at 21 (3 refs, `3/26 = 1154`), `arg1` born 3 dies 19 (`2/32 = 625`),
  `arg0` born 2 dies 24 (`2/44 = 455`). Same order as before — the correction
  halved every ratio uniformly and changed no outcome — so the allocator still
  serves `ptr`, `arg1`, `arg0`.
- **Why it cannot be separated, stated tightly.** The exchange needs
  `P(arg0) > P(arg1)`, i.e. `2/(2*(24 - b0)) > 2/(2*(19 - b1))`, i.e.
  `b0 - b1 > 5`. Both deaths are pinned by which call consumes which argument.
  Both births are argument copies out of `r0`/`r1`, which must precede the first
  `bl` at index 6 because it clobbers them, so both indices are confined to the
  window `[2, 5]` and `b0 - b1` cannot exceed 3. There is no assignment of birth
  positions in that window that separates them, so the exchange is unreachable
  from C — not merely unreached by the twelve source shapes tried. (Those still
  matter as corroboration: naive temporaries are removed by copy propagation
  before flow, and one that survived would be tied by `combine_regs` and make it
  worse.) This is the form a park proof should take — a pinned interval and an
  inequality that does not fit inside it, not a list of shapes that failed.
- **Retracted, `08006dec`** (56-byte region; was recorded here as a second
  inseparable exchange, at 17 mismatches). It was neither. Re-measured
  2026-07-24: the region prints `;; 0 regs to allocate:`, so its recorded ratios
  ("1 vs 0.5") used the global divisor on a wholly local-allocated function and
  meant nothing — and worse, the candidate they were measured on was not
  semantically the reference. Two real defects hid under the register story.
  First, m2c had typed the flash addresses `s8`, which makes `0xAA` the negative
  constant `-86` and materialises it as `movs r4, #0x56; negs r4, r4` instead of
  `movs rN, #170`. Second, m2c had rendered the third unlock write as a dead
  local assignment, so the store was simply absent. Retyping to `u8` and writing
  all three stores exposed the next layer: two writes to `0x0E005555` with no
  intervening read are a dead store, and this compiler's DSE deletes the first,
  leaving the region two bytes short. Qualifying the command writes `volatile`
  is what a flash-command sequence needs anyway, and it took the region from 17
  byte mismatches to 11 with the size and the whole literal pool exact and
  `semantic` at zero. The residual is a genuine three-way rotation (values
  `r3`↔`r2`, address B `r2`↔`r3`) over thirteen further source shapes and all
  eight fork switches.
- **Where `08006dec` actually stops, measured with the corrected model.** It
  prints `;; 0 regs to allocate:`, so every divisor is the doubled in-block span.
  With `size` correctly 1 word for both, each store value scores
  `floor_log2(2) * 2 * 1 / 2 = 1.0` and the second unlock address scores
  `1 * 2 * 1 / 4 = 0.5`, so the values are allocated first and take `r3` while the
  address takes `r2`. That is precisely what the candidate emits — the model now
  predicts our own output exactly, which is what makes the negative trustworthy.
  The reference is the other way round, so it needs the address to outrank the
  values, and it cannot: GCC expands a store's *address* before its *value*, so a
  freshly materialised store value always has the minimum possible span (born,
  then dead at the store, `D = 2`) while the address always has an insn between
  its birth and the store (`D ≥ 4`). Merging all the values into one pseudo does
  not rescue it either — the best reachable is `floor_log2(6) * 6 / 12 = 1.0`
  against the address's `0.5`, and dropping a 6-reference quantity below `0.5`
  would need a span longer than the entire basic block. Under
  `REG_ALLOC_ORDER`'s `{3, 2, 1, 0}` the reference head is therefore unreachable
  from C: it requires the allocator to hand out `r2` before `r3`, and neither the
  default order nor `-mentry-low-register-order`'s `{0, 1, 3, 2}` does that (`r0`
  and `r1` are taken by the incoming argument copies, which have hard-register
  suggestions and are allocated first, so both orders reduce to `{3, 2}` here).
- **The proof does not depend on our candidate.** Read it off the reference's own
  bytes: at `08006df4` it loads the address, at `08006df6` it materialises the
  value, at `08006df8` it stores. So in the reference's *own* instruction stream
  the value is born one insn after the address and both die at the same store —
  the value's span is strictly shorter and its reference count equal, so under
  any priority function monotonic in `n_refs` and decreasing in span, the value
  outranks the address. Yet the reference gives the address `r3` and the value
  `r2`. Whatever order that allocator used, it served `r2` before `r3`. No source
  shape can change which register a compiler hands out first, so this is a
  compiler-side difference, established without appeal to anything we compiled.
- **Lane item, and a second witness for the family.** `08006dec` closes if
  `arm_order_regs_for_local_alloc_block` gains a `{2, 3, 1, 0}` low order — the
  fork's default with `3` and `2` transposed. It is not the only region whose
  entry block wants a register the fork's order cannot hand out: `08096c80`
  (below) needs an entry-block pool load in `r4` where `{3, 2, 1, 0}` gives `r3`,
  and its own law already reads that as evidence the original compiler ordered
  the entry block differently. Be careful not to over-merge them — `{2, 3, 1, 0}`
  would give `08096c80` `r2`, so one permutation does not serve both, and neither
  region alone pins the order. What the two together do establish is that the
  entry-block low order is a real axis of difference between this fork and the
  original, and that it is worth a third witness before anyone designs the
  switch.
- **Consequence:** a pure two-register exchange with an otherwise identical
  instruction stream is a park, not a puzzle *once the two ratios are measured and
  found inseparable* — and "measured" means measured on a candidate you have first
  confirmed is semantically the reference, with the divisor the `.greg`
  discriminator selects. `080a3e88` was parked that way and stands. `08006dec` was
  parked on numbers computed with the wrong divisor from a candidate that was
  missing a store, and the register exchange it described was downstream of that
  missing store, not the fault. A register-only diagnosis is the *easiest* verdict
  to reach wrongly, because a semantic defect upstream re-colours everything after
  it and still reports as `register_only`. Check the size, the literal pool and
  the constants' *materialisation* before you believe the registers.
- **Second witness, and a hard unreachability it exposes (`080a8578`,
  2026-07-25, parked at 17 bytes / 13 instructions, size and pool both exact).**
  Two pseudos sit 0.9% apart in the ratio -- `arg1` at `floor_log2(7)*7/37` =
  0.37837 against a pointer-plus-offset at `floor_log2(3)*3/8` = 0.37500 --
  and the flip needs one extra zero-byte RTL insn to lift the first
  denominator from 37 to 38. The numerator and both of the second quantity's
  inputs are pinned by emitted instructions, so only that one denominator can
  move. The reason it cannot: the only C-reachable way to keep the constant
  `0xBE6` in its own insn is to give it a second use, and a second use makes it
  live across the region's call, which forces it into a callee-saved register
  and costs more bytes than the flip saves. The two requirements are mutually
  exclusive, which is a sharper statement than "the ratio is pinned" -- here
  the ratio is movable and the move is still unreachable.
- **The reusable half: in Thumb, a `(set r0 <pseudo>)` after a call can never be
  combined away.** `combinable_i3pat` returns 0 when the destination is a hard
  register satisfying `FUNCTION_VALUE_REGNO_P`, the source is not a `CALL`, and
  `SMALL_REGISTER_CLASSES` holds (combine.c:1387-1396) -- and
  `SMALL_REGISTER_CLASSES` is `TARGET_THUMB` (arm.h:1128), so it holds for
  every region in this project. Any return-value copy therefore survives to
  allocation and its live range always counts. Before proposing that a source
  edit will delete such an insn to shorten a span, check this first; it is a
  standing no.
- **Recorded:** 2026-07-24; second witness and the combine finding 2026-07-25.

### A statement's position sets a live length, and one insn of live length can flip the allocator (2026-07-24)

- **Claim:** the exchange described in the previous law is winnable whenever a
  statement can be moved without changing the emitted instructions but with the
  effect of moving a birth or a death by one insn. `global.c`'s `allocno_compare`
  uses `floor_log2(n_refs) * n_refs * size / live_length`, the same shape as
  `QTY_CMP_PRI` but over the whole-function `live_length`, and the ratios of two
  competing quantities are often within a few percent, so a one-insn change to
  `live_length` decides the register.
- **Diagnostic:** compile with `-dl` and read the `.lreg` dump. It prints
  `Register N used R times across L insns` for every pseudo, plus
  `;; N regs to allocate:` in priority order in the `.greg` dump. `R` is the
  `n_refs` both passes use, with loop-depth weighting already folded in (×2 per
  real reference inside one loop). `L` is `reg_live_length` and is the divisor
  for the **global** pass only; for a **local-alloc** question take the divisor
  from the RTL insn indices inside the block, per the correction in the law above.
  When a quantity never leaves one basic block the local pass is the one that
  decides it, and the `.greg` `regs to allocate` line then reports only what the
  local pass left over.
- **Evidence, `08077348`** (76-byte region, now matched): the accumulator, the
  countdown and one of {`count`, strength-reduced pointer} take `r6`, `r5`, `r7`,
  and the loser is caller-saved into the one stack slot around the inner call.
  Measured: countdown `2*7/11 = 1.273`, accumulator `3*9/38 = 0.711`, pointer
  `2*7/24 = 0.583`, `count` `2*5/18 = 0.556`, so the pointer took `r7` and `count`
  was spilled — the reference is the other way round. Every ref count is pinned:
  `count` is set once and used four times (two compares, the loop-count init, the
  final call argument) and the pointer's three references are the giv init plus a
  load and an increment inside the loop. What was *not* pinned was `count`'s
  birth. Writing `total = 0;` **before** `count = Func_080795fc();` instead of
  after moves the zero-init out of `count`'s live range, giving `2*5/17 = 0.588`,
  which clears the pointer's `0.583`. The emitted code is unchanged — GCC sinks
  the `movs r6, #0` back below the call either way — but the allocation flips and
  the region goes from 17 mismatching bytes to exact.
- **Corollary:** the residual byte count badly overstates this defect. The wrong
  register propagates: because the loser is caller-saved, its save must precede
  the call, so the post-reload scheduler also reorders the loop tail. `08077348`
  showed 28 bytes across three apparently separate defects (register exchange,
  scheduling transposition, a missing post-call copy) that were one decision.
- **Consequence:** before parking a register exchange, dump `-dl`, compute both
  ratios, and ask which of the four inputs is not pinned. Reference counts usually
  are; a birth or a death often is not, and any statement that GCC will sink or
  hoist back into place is free to move.
- **Recorded:** 2026-07-24.

### Birth order decides a two-operand add, and reload pins its tie copy (2026-07-24)

- **Claim:** for a Thumb two-operand instruction whose operands are two pseudos
  that die into it, the one **born last** takes `r3`. Its birth is later, its
  death is the same insn, so `death - birth` is smaller and `QTY_CMP_PRI` is
  larger; `REG_ALLOC_ORDER` for `LO_REGS` is `{3, 2, 1, 0, 4, 5, 6, 7}`, so the
  highest-priority quantity is handed `r3` first. The result then shares the
  loser's register. This is a property of statement order in the source, and it
  is the reason a "just compute that value earlier" edit flips two registers.
- **Evidence, `0801fd34`** (72-byte region, parked at 4 mismatches): the loop head
  computes `*src + i * 8`. Written as one expression, `*src` stays a lazy MEM and
  `force_reg` emits its `ldr` *after* the shift — wrong order, right registers
  (load `8/2 = 4.0` beats shift `8/4 = 2.0`, so the load takes `r3` as the
  reference has). Written with the load as its own statement (seven spellings
  tried: a named base, a named accumulator, and five orderings around them) the
  `ldr` comes first as the reference has it but the registers exchange, because
  the shift is now born last (`8/2 = 4.0` against the accumulator's `30/8 = 3.75`)
  and steals `r3`. Winning both at once needs the earlier-born pseudo to have the
  shorter span, which is a contradiction. Folding the whole `* 0x300` chain into
  the accumulator to raise its `n_refs` was tried and refuted: the accumulator
  becomes the call argument and lands in `r0`, moving every register in the chain.
- **Second half, reload's tie copy:** the same region's tail wants
  `adds r1, #22` to sit *between* `adds r2, r3, #0` and `adds r2, #16`. It cannot.
  `*thumb_addsi3` (arm.md:496) has constraints `"=l,l,l,..."` / `"%0,0,l,..."` /
  `"I,J,lL,..."`; `b = t + 16` with `b != t` cannot take alternative 2 because 16
  is not constraint `L`, so it takes alternative 0, which ties operand 1 to
  operand 0, and reload emits the reg-reg copy **immediately before the add** —
  never one insn earlier. A source-level `b = t;` does not help: CSE's `canon_reg`
  rewrites the following `b += 16` to `b = t + 16` and the copy dies. Note that
  `*thumb_movsi_insn` prints a lo→lo move as `add %0, %1, #0`, so the copy and an
  `addsi3` with constant 0 share the encoding `0x1c1a` and are indistinguishable
  in the disassembly. The tie copy and its consumer are one inseparable pair, and
  the only knob is which side of a neighbouring independent insn that pair sits
  on: eight statement orderings gave either `[c-add][copy][b-add]` or
  `[copy][b-add][c-add]`, never the reference's `[copy][c-add][b-add]`.
- **The escape does not work either:** a source-level copy *can* be kept alive by
  redefining `t` between `b = t;` and `b += 16;`, which stops `canon_reg`
  rewriting the increment. But a surviving real `movsi` is then hoisted above the
  `lsls` that starts the expression, costing more than it saves (6 mismatches
  against 4). Neither `-fno-schedule-insns` nor `-fno-schedule-insns2` moves it,
  so the hoist is an expansion-order effect, not a scheduling one.
- **Diagnostic that settled it — proving a reference is *not* scheduled:** compile
  with `-dR -fsched-verbose=5` and read `<file>.c.23.sched2`. It prints the whole
  haifa dependency graph with a `prio` column per insn, the ready list at each
  cycle, and the resulting order. `rank_for_schedule` compares `INSN_PRIORITY`
  first and `INSN_LUID` only on ties, so any reference that emits a lower-priority
  insn before a ready higher-priority one cannot have come from this scheduler.
  `0801fd34`'s reference does that twice — prio 5 before prio 6, and a prio-2 insn
  ready at cycle 0 placed dead last — which proves the tail is unscheduled and
  makes `-fno-schedule-insns2` the correct lane. Use this before spending any
  effort on a "the scheduler moved it" theory; it answers the question in one
  compile instead of a sweep.
- **Recorded:** 2026-07-24.

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
- **Unit-gating negative (2026-07-23):** the era thumb backend's
  `thumb_010110a.md`/`thumb_020422.md` (pret/agbcc) declare zero
  `define_function_unit` entries, unlike our 2.96 `arm.md`, which gives
  Thumb loads/stores 2-cycle latencies and ARM7 write-buffer modeling. To
  test whether that structural difference alone explains the head order,
  a diagnostic build (alchemy-gcc branch `diagnostic-tables`, commit
  `6b82dbf`) gated `write_buf`/`write_blockage`/`core` off for Thumb on
  our 2.96 `arm.md`, leaving everything else unchanged. Result: negative,
  and worse than doing nothing — `0800383c` floors at 21 mismatched bytes
  under the gated build versus the 12-mismatch floor already recorded
  above; sched2 still reorders the head under uniform latencies. Neutering
  the current backend's tables is not equivalent to the era thumb
  backend's actual (and structurally different) RTL expand: the gap is not
  a tunable-parameter difference on 2.96, reinforcing the vintage-blocked
  conclusion rather than opening a new lever. `diagnostic-tables` carries
  no adoptable result and was not merged.
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
- **Fourth and fifth confirmed instances (2026-07-24):** `08006408`
  (slot-allocate: checks a busy flag, then IME-guards a struct write and
  claims a global slot) and `08004278` (the same 20-entry/8-byte-stride
  0x03001A20 scan as `080042c8`/`080043e0`, this time clearing a matched
  entry) both float at the same head-order gap (52/... and 74/...
  mismatches respectively) with the identical signature: a pool-address
  load with no dependent use until deep inside the guarded body, sitting
  ahead of the IME save. Five independent functions now confirm this is a
  general property of this head shape. Do not spend further per-function
  search budget rediscovering it; if the endgame-ordering pass reaches
  this cohort, treat the shape itself (IME critical section whose first
  loaded pool address has no near dependent use) as the classification
  criterion rather than re-deriving it function by function.

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
- **Related, distinct shape — shared single-instruction `bx r3` veneer
  (2026-07-24):** `0800070c` hits a third variant of this same "compiler
  cannot emit this control transfer" class: no IME guard, and the tail
  does `bl Func_0800070a` where `Func_0800070a` is itself a bare
  `.thumb_func`-labeled `bx r3` sitting mid-stream inside
  `asm/executable_gaps/080006fc.s` — an indirect tail-call through a
  *locally shared* one-instruction veneer, not the fixed `__call_via_rN`
  helpers `call_indirect` always targets and not the `mov ip, pc; bx rN`
  inline form either. 87/... mismatched bytes; ordinary body reproduces
  fine up to this tail. Treat any function whose only gap is a `bl` to a
  bare mid-stream `bx rN` label the same way as the other two call-site
  shapes: a structural-classification candidate, not a further C-shape
  search target.
- **Second witness, and a retracted claim (2026-07-24):** `080f9ef8` is the
  same shape one level subtler, and it had been *claimed* as C in
  `ce93ad4d`. Its C reproduces all 34 instructions of the region exactly
  — prologue, the single-bit `tst`, the whole list walk, the literal pool —
  under `-mpreserve-single-bit-test -mentry-low-register-order
  -mthumb-and-sets-cc` on the default ABI. The one surviving difference is
  the call site: the compiler emits `bl _call_via_r3`, which
  `alchemy_gcc.ts` resolves to the fixed runtime thunk bundle at
  `0x080072f0`, while the reference branches to `0x080f9ee8` — the
  `pop {r3} / bx r3` tail of `asm/080f9c90.s`, the function immediately
  preceding it. The original translation unit satisfied its `_call_via_r3`
  from a `bx r3` already present in a sibling function's epilogue; a
  per-function link cannot express that, and no C shape reaches it (the
  pointer has to be live in r3 across a `bl` to a named symbol, which needs
  a disallowed register pin). Retracted the claim, restored
  `asm/080f9ef8.s`, and dropped the now-sourceless
  `SINGLE_BIT_TEST_ENTRY_ORDER_SOURCES` routing. The lesson for the
  reviewer: a candidate can be instruction-for-instruction perfect and
  still not be a match, because `bl` target resolution is a link-model
  property, not a codegen one — always read `build_claimed.ts`'s
  `failures=` count, never just the instruction diff. The three compiler
  modes above are recorded here so the routing can be restored verbatim if
  the veneer-placement model is ever solved.
- **Independently re-derived (2026-07-24):** a fresh agent given only the region
  and no access to this entry reached the same conclusion from the other end. It
  found the starting candidate wrongly called `Func_080f9ee8` *directly*, and that
  writing the reference's **indirect** call —
  `M2C_FIELD(Data_03007ff0, void(**)(s32), 0x2C)(arg)` — supplies all six bytes
  the candidate was missing (`ldr r3,[pc,#28]` / `ldr r3,[r3,#0]` /
  `ldr r3,[r3,#44]`, the `0x03007ff0` pool word, and a 2-byte pad), reaching 68/68
  bytes with three mismatching bytes on the single `bl` line. It also measured the
  allocation and found `;; 4 regs to allocate: 34 35 33 43` is *already* the
  reference order with no exchange to fix, and that an A/B of `-fcall-used-r4`
  against `-fcall-saved-r4` leaves every pseudo number, `n_refs` and
  `live_length` bit-identical while moving the dispositions onto the reference's
  `push {r4,r5,r6,lr}`. Two witnesses, opposite directions, same verdict: leave
  `asm/080f9ef8.s` as assembly.

### A hoisted shared constant needs the widest mode first (2026-07-24)

- **Claim:** `loop.c`'s `combine_movables` will only merge a later constant-set
  into an earlier one when
  `GET_MODE_BITSIZE (m->set_dest) >= GET_MODE_BITSIZE (m1->set_dest)`. So a
  QImode zero appearing *before* an SImode zero blocks the merge and the SImode
  zero is not hoisted to the preheader; reverse the order and it is. `-dL` reports
  the rejected candidate as "life 1, savings 1, not desirable".
- **Consequence for source shape:** when a region needs one shared zero
  materialised once in the preheader, give it a single `s32` local and use it for
  every width, rather than letting a narrow store create its own zero first.
  Introducing `s32 zero = 0;` and storing it through both the byte and the word
  path was the edit that fixed this on `080f9ef8`'s body.
- **Adjacent, same region:** splitting an `and` into its own statement while
  reusing the *loaded* temporary as its destination ties the AND's destination to
  the load in reload and yields the reference's `ldrb r0` / `movs r3, #7` pairing;
  a fresh temporary for the AND breaks the tie. Also ruled out there, and worth
  not retrying: `while` versus `do/while` on a provably-entered loop is
  byte-identical, and `7 & field` versus `field & 7` is canonicalised by 2.96
  before it reaches RTL.
- **Recorded:** 2026-07-24.

### regmove's backward pass undoes its forward pass, and a remote constant stops it (2026-07-24)

- **Claim:** when a commutative, tied insn (`%` and `0` on the same pattern, e.g.
  `*thumb_andsi3_insn`) has one operand copied from an incoming hard register and
  the other from a constant, regmove ties it *twice*, in opposite directions.
  Pass 0 runs forward and `fixup_match_1` reports "Fixed operand 1"; pass 1 runs
  backward through the same helper and reports "Fixed operand 2", undoing it. The
  guard at regmove.c:1194 only skips when
  `replacement_quality (comm) >= replacement_quality (src)`, and a pseudo copied
  from a hard argument register has quality 1 against a constant's 3 — so the
  backward pass always wins and the AND ends up tied to the constant.
- **Remedy:** make the constant *remote*. Assign it to a variable in an earlier
  basic block than the insn that uses it. `reg_is_remote_constant_p`
  (regmove.c:1646) then returns 1 and `fixup_match_1` bails out of the backward
  pass, leaving the forward pass's tie standing. The forward pass is unaffected
  because there `src` is the argument pseudo, whose defining insn carries no
  `REG_EQUAL` note.
- **The hoist is free.** `update_equiv_regs` runs in local-alloc, after regmove,
  and sinks the constant's `movs` back down to its single use. On `08092b08` the
  `s32 three = 3;` was written above the call and the emitted `movs r3, #3` came
  out exactly where the reference has it, at `08092b2a`. This is the same "any
  statement the compiler will sink is a free knob" principle as the live-length
  law, applied to a pass ordering rather than a ratio.
- **Effect measured:** `08092b08` went 21 → 10 mismatches on this edit alone, and
  it was the edit that put the two global allocnos in the reference's registers —
  raising the argument's reference count from 2 to 4 by making the AND write back
  into its own pseudo, which moves its priority from `2*2/14 = 0.143` to
  `2*4/16 = 0.5` against the object pointer's `2*5/22 = 0.455`. The allocator was
  never the thing to edit; regmove was.
- **Adjacent, same region — a narrow mask wants an `s32` *variable*, not a
  literal.** Writing `-0xD & p[9]` lets the front end's `shorten` narrow the mask
  to `unsigned char` (one `movs #243`, wrong size). Naming an `s32` byte temp is
  also wrong: `PROMOTE_MODE` makes it an SImode pseudo, the AND becomes
  SImode-to-SImode, regmove ties it and the reference's mask copy disappears. An
  `s32 mask = -0xD;` variable with inline byte loads is the shape that works —
  `shorten` cannot fire on an `int` variable, `convert_to_integer` still
  distributes the QI truncation over `&`/`|` so the constant materialises as
  `movs #13; negs`, and both AND operands stay `(subreg:SI (reg:QI ...))`, which
  regmove skips because `GET_CODE (src) != REG`.
- **Recorded:** 2026-07-24.

### A dead instruction the compiler always deletes is unreachable from C (2026-07-24)

- **Claim:** a reference region can contain an instruction whose result is
  provably dead, and no C source compiles to it, because the pass that would
  delete it runs unconditionally. This is a distinct negative class from a
  register exchange or an ordering park: there is no ratio to separate and no
  statement to move. The instruction is not reachable, full stop.
- **Evidence, `08077394`** (68-byte region, closed as a measured negative at
  *one* instruction — a second pass reached 68/68 bytes with all 27 emitted
  instructions and all three pool words byte-identical, so the entire 53-byte
  residual is the two-byte slide left by the one missing instruction). The
  reference opens
  `push {lr}` / `mov r3, lr` at `08077396`. Every path then overwrites `r3`
  before reading it, so the copy is dead on arrival. The only backend path that
  emits a bare `mov rN, lr` with `lr` as the *source* and no matching earlier
  `mov lr, rX` is `arm_return_addr` (arm.c ~10277), reachable from C only via
  `__builtin_return_address(0)` — and because the value is dead, flow deletes it
  again before it can be emitted. There is no C that survives its own optimiser
  here.
- **Corpus check, which is what makes it a law and not a guess:** across the whole
  game, `push {lr}` immediately followed by `mov rN, lr` occurs in exactly one
  place — this one. All 87 other `mov rN, lr` sites pair with an earlier
  `mov lr, rX`, i.e. they are the second half of a high-register save, not a
  return-address read. A shape that appears once in 2,058 regions and has no
  compilable spelling is not a shape you have failed to find; it is one the
  compiler did not produce.
- **Diagnostic:** when a residual instruction has no consumer, stop looking for a
  source shape and check whether any pass deletes it. If the value is dead and the
  deleting pass is unconditional, the region's ceiling is that instruction. Record
  the ceiling and take the improvement that gets you there — the `08077394`
  candidate still moved the region from 72 bytes / 48 mismatches to the exact 68
  with a clean body, which is worth keeping even though it can never reach zero.
- **`byte_mismatches` is the wrong ranking key at this stage of a region.** The
  earlier 72-byte candidate scored 48 and the correct 68-byte one scores 53, yet
  the 48 was measured on an oversized build with the argument in the wrong
  register and 19 instruction mismatches. Compare `actual_size` against the
  region first, then instruction mismatches; only then bytes.
- **Adjacent findings, same region, worth not re-deriving:** the signature is
  `u8 *Func_08077394(s32)` — read off the already-matched caller `src/08077348.c`,
  not guessed — with `extern u8 Data_02000500[];` and `extern u8 *Data_03001f28;`.
  A single merge variable for the two returned pointers is needed to break a
  `*thumb_mulsi3` reload tie.
- **Possible lane work:** a fork switch suppressing DCE of the `arm_return_addr`
  set would close this region. It is the only known use, so it is low priority.
- **Recorded:** 2026-07-24.

### A pool word holding a Thumb function carries the interworking tag (2026-07-25)

- **Claim:** when the stock object stores a Thumb function's address as data, the
  linked word has bit 0 set. That is the ARM ELF rule for an `R_ARM_ABS32`
  relocation against a Thumb function symbol, and it applies to the game's own
  pool words, not just to hand-written interworking code.
- **Evidence, `08006ba8`** (the flash reader, 124 bytes). Its pool holds
  `0x08006b85` and `0x08006ba9` — the tagged addresses of `Func_08006b84` and of
  the region itself — and the body immediately clears the tag with
  `movs r0, #1` / `eors r3, r0` so the copy loop can read halfwords. The
  self-referencing word `0x08006ba9` came out right all along, because that
  symbol is a real label in the compiled object; only the *external* one was
  wrong.
- **Harness defect this exposed:** `externalSymbolAssembly` emitted
  `.thumb_func` followed by `.set NAME, 0x…`. GAS only records a branch type for
  a symbol defined by a label, so on a `.set` alias the Thumb marking is silently
  dropped and every data reference links to the plain even address. Branches were
  unaffected, which is why 1,236 regions matched with the bug present. `.thumb_set`
  is the alias form that keeps the branch type: the branch offset stays correct
  *and* the data word gets its tag. Fixed in `tools/alchemy_gcc.ts`.
- **Fallout, and the rule it produced:** the fix broke exactly one installed
  region, `0801a32c`, which had spelled a pointer *table* at `0x08031864` as
  `Func_08031864`. That spelling had been byte-neutral only because the stub was
  losing the tag. Do not reach for `Func_` to name an address just because it
  makes a pool word come out right — after this fix, `Func_` asserts Thumb code
  and costs a byte if the target is data.
- **Second-order finding:** `0801a32c` needs *four* distinct external symbols all
  valued `0x08031864`, because arm.c's minipool keeps one entry per distinct
  `SYMBOL_REF` and the reference has four identical pool words. Spelling them
  identically lets cross-jumping merge the switch arms and collapses the region
  from 64 bytes to 24. `ADDRESS_SYMBOL` therefore accepts an optional trailing
  letter (`Data_08031864_b`) for a second symbol at an already-named address.
- **Recorded:** 2026-07-25.

### old_agbcc has no instruction scheduler at all (2026-07-25)

- **Claim:** in a region routed to `old_agbcc`, no transposition is ever a
  scheduling question. The compiler rejects `-fno-schedule-insns` and
  `-fno-schedule-insns2` outright, and `-da` produces no `.sched` dump files,
  so emitted order is RTL order. Every out-of-order pair is an expansion-order,
  `regmove`, `combine`, or `reload` question instead.
- **Why it matters:** most of this ledger's ordering laws were measured on the
  Camelot fork, where `sched2` is the usual culprit and `rank_for_schedule`
  priority is the usual blocker. Carrying that model into an `old_agbcc` region
  sends an agent hunting a pass that is not in the binary. Establish which
  compiler the region is on *before* reasoning about order.
- **Evidence, `08006a00`** (120 bytes, exact). Its four transposed instructions
  at `-O2` are a `regmove` rename, not a schedule: the front end narrows the
  interrupt-enable read-modify-write to a HImode `BIT_IOR`, so the ior's first
  operand is a `subreg`. `regmove_optimize`'s forward pass skips non-`REG`
  sources, follows the `%` commutativity marker to the other operand, and
  renames the ior's destination into the shift-result pseudo. `-O1` and
  `-fno-regmove` give byte-identical output, because `flag_regmove` is set
  unconditionally at `optimize >= 2`.
- **Corollary, and the reason `-O1` keeps recurring on this compiler:** three of
  the four `-O1` routes in `AGBCC_OPTIMIZE_O1_SOURCES` exist for three unrelated
  passes — `duplicate_loop_exit_test` (`08006ba8`), a `local-alloc` tie that
  removes a reload input reload (`08007098`), and `regmove` (`08006a00`).
  `-O1` is not a compatibility mode for this object; it is a coarse way to spell
  "one specific `-O2` pass did not run", and the sibling `08006dec` is
  byte-identical at both levels, which is what proves the level is per-address.
- **Scope:** confirmed for `old_agbcc` only. The fork does have both scheduling
  passes, and the existing `sched2` laws stand for it unchanged.
- **Recorded:** 2026-07-25.

### Pre-call argument setters diverge on two independent `sched2` defects (2026-07-25)

- **Claim:** the fork's `sched2` mis-orders the run of cheap argument setters in
  front of a call for two separate reasons, and each one has its own witness.
  **(A) The `movs`→`lsls` edge costs one cycle, and in the reference it costs
  two.** A two-insn Thumb constant therefore issues back-to-back here, where the
  reference fills the gap with an independent setter. **(B) Among insns that are
  otherwise tied, `rank_for_schedule` prefers the one with more forward
  dependents, and at *some* sites the reference behaves as if that rule is
  absent** and falls straight through to the `INSN_LUID` tie-break, i.e. original
  order. Neither is reachable from C: both are decided after reload, on an insn
  stream the source no longer controls. **(A) is a global law; (B) is not — see
  the measurement below.**
- **(B) is a per-site lever, not a global law — measured.** An earlier version of
  this entry claimed the reference behaves as if the depend-count rule is absent,
  full stop. That is false. With the rule gated off globally, `build_claimed.ts`
  reports `linked=1239 failures=260`: the rule is *present* in the reference for
  the large majority of code. Worse, it is not even source-scoped. Inside the
  single function `resource_3a0_c_02000048.c` the reference needs the rule ON at
  block 0's prologue shuffle (`Ready list (t = 4): 23 19 21`, reference takes
  `21`, which only the depend count reaches) and OFF at block 1's pre-call pair
  (`Ready list (t = 51): 73 75`, reference takes `73`, which only `INSN_LUID`
  reaches). So `-fno-sched-depend-count` is a narrow per-source escape hatch for
  functions whose every tie wants original order, not a model correction.
- **Hypothesis "count only *true* dependents" is refuted.** The natural repair —
  that the reference counts `REG_DEP_TRUE` edges and the fork over-counts by
  including anti and output edges — does not survive the RTL. At the block 0 site
  above, insn 21's two extra dependents are insns 26 and 28, and 26's dependence
  list reads `(insn_list 24 (insn_list:REG_DEP_OUTPUT 6 (insn_list:REG_DEP_ANTI 21
  (nil))))` — both extras are non-true. Dropping non-true edges makes 19/21/23 tie
  and `INSN_LUID` picks 19, which breaks the site the full count gets right.
- **`-mcall-arg0-move-first` is not subsumed by `-fno-sched-depend-count`.** The
  pre-existing peephole at `arm.c:6736` patches exactly (B)'s symptom for 21
  overlay sources, and on `resource_3a0_c_02000048.c` the two levers are not
  interchangeable: the scheduler flag fixes the call site the peephole fixes and
  simultaneously breaks the prologue shuffle the peephole leaves alone. Keep both.
  Conversely the peephole does not cover `3cd:004c`, whose r0 setter is an
  immediate (`movs r0,#13`) rather than a register move, which is all the peephole
  matches: with `-mthumb-immediate-latency -mcall-arg0-move-first` that function
  stays at 8 mismatched bytes, and with `-mthumb-immediate-latency
  -fno-sched-depend-count` it reaches 0.
- **Why (B) looks like an ABI rule, and why that phrasing was wrong.** The extra
  forward dependent is always contributed by the block's return insn, which
  depends on the *last writer* of each hard register. For a pre-call setter of
  r1 or r2 that last writer is the setter itself; for a setter of r0 it is the
  call, because the call redefines r0. So the r0 setter is systematically one
  dependent short and systematically loses. The observable — "the higher-numbered
  argument register's setter wins" — is a consequence of that, not a rule in its
  own right, and it is only sampled on pairs where the low register is r0. An
  r1-versus-r2 tie is unmeasured and would fall through to `INSN_LUID`.
- **What this replaces.** Two superseded mechanisms, neither of which should be
  repeated. First, that the `movs`/`lsls` pair is a single unsplittable RTL insn:
  false — reload splits it well before `sched2` (two insns in `.18.greg`). Second,
  that the tie is an artifact of `CALL_INSN_FUNCTION_USAGE` / `LOG_LINKS` order:
  also false — `rank_for_schedule` never reads either, and `-fsched-verbose=5`
  names the deciding rule outright.
- **Evidence for (A), `resource_3c7:0030`** (18 bytes, 4 mismatched, unresolved).
  Reference `movs r1,#129 / movs r0,#14 / lsls r1,r1,#1`; fork `movs r1,#129 /
  lsls r1,r1,#1 / movs r0,#14`. The `.23.sched2` dump:
  ```
  ;;	Ready list (t =  0):    11  30      -> schedules 30 (movs r1,#129)
  ;;	Ready list (t =  1):    11  31      -> schedules 31 (lsls r1,r1,#1)
  ;;	Ready list (t =  2):    11
  ```
  The shift is ready one cycle after its producer, so it never yields the slot.
  Under a two-cycle edge insn 31 is not in the list at `t = 1` at all, only `11`
  is, and the emitted order becomes the reference's — with no appeal to (B).
- **Evidence for both, `resource_3cd:004c`** (36 bytes, 10 mismatched,
  unresolved). Two divergences in one function, one per defect.
  ```
  2000056:  lsls  r1,r1,#1   | 2000056:  movs  r0,#13     <- (A)
  2000058:  movs  r2,#0      | 2000058:  lsls  r1,r1,#1
  200005a:  movs  r0,#13     | 200005a:  movs  r2,#0
  2000060:  movs  r1,#0      | 2000060:  movs  r0,#13     <- (B)
  2000062:  movs  r0,#13     | 2000062:  movs  r1,#0
  ```
  The second pair is the clean (B) case: `movs r0,#13` and `movs r1,#0` are
  independent, equal priority, and adjacent pre-`sched2` in the reference's
  order, so only the depend-count rule can transpose them. `-fsched-verbose=5`
  prints the counts in its forward-dependence table and then acts on them:
  ```
  ;;      insn  code    bb   dep  prio  cost   blockage units
  ;;       25   173     0     1    65     1    1 - 32   core	: 28
  ;;       27   173     0     2    65     1    1 - 32   core	: 41 28
  ;;	Ready list (t = 70):    25  27      -> schedules 27, then 25
  ```
  Insn 41 is the return; insn 25 sets r0, whose last writer is call 28, so 25
  carries one dependent and 27 carries two. Equal priority (65 = 65), so
  `haifa-sched.c:4097-4110` decides, and `haifa-sched.c:4115`'s `INSN_LUID`
  tie-break — which would have preserved the reference's order — is never
  reached.
- **The two defects are independently necessary.** In `3cd:004c`'s first group
  the ready list at `t = 35` is `17 21 37`; dropping (B) alone still schedules
  `37` (the shift), because `INSN_LUID (37) < INSN_LUID (21)`. Only a two-cycle
  edge removes `37` from that list, and only then does dropping (B) pick `17`
  over `21` and reproduce `36 17 37 21` exactly. Conversely `3c7:0030` needs
  only (A) and `3cd:004c`'s second group needs only (B). The pair of changes
  predicts all three divergences byte-for-byte; either alone predicts one.
- **The control that isolates (B) from insn shape.** Swap the arguments —
  `f(0x102, 0xE)`, so the split lands in r0 and the cheap move in r1 — and the
  same compiler *does* interleave: `mov r0,#129 / mov r1,#14 / lsl r0,r0,#1`,
  with `Ready list (t = 1): 29 13` taking `13`. Identical insn structure,
  identical LUID order, opposite outcome. That is (B) reversing sign exactly as
  the last-writer account predicts: the shift now targets r0 and loses the
  return's dependence, the cheap move now targets r1 and gains it.
- **Levers measured and rejected on `3c7:0030`:** `-fno-schedule-insns`,
  `-fno-schedule-insns2`, `-fno-regmove`, `-fno-defer-pop`, `-fno-force-mem`,
  `-fno-cse-follow-jumps`, `-O1`, `-Os`, and `-mcall-arg0-move-first` are all
  exact no-ops on the emitted order. Eight source shapes — the constant as a
  local, the cheap arg as a local, both as locals, a comma expression, an
  unprototyped callee, a narrowed `u16` parameter, a `static` initialiser, and
  `0x81 << 1` written out — all stay at 4 mismatched bytes. Ten equivalent
  shapes were tried in a later pass and also all stayed at 4.
- **Why neither is reachable from C.** For (A), priority is the critical-path
  length to the block end; both candidates are one hop from the call, so they
  always tie, and the reference's `movs r0,#14` cannot be given a longer path
  without adding instructions the reference does not have. For (B), the extra
  dependent comes from the return insn's view of the last writer of a hard
  register, which is fixed by the calling convention once the argument list is
  fixed — and the argument list is what the reference's own call sites dictate.
- **Where the cost comes from.** `arm.md:254` gives the `core` unit a ready-delay
  of 1 for `core_cycles = single`, and `arm_adjust_cost` (`arm.c:2439`) adds
  nothing for a register-to-register data dependence — it only zeroes anti and
  output edges, forces 1 for edges into a call, and special-cases load-after-store.
  So there is no ALU result latency in the fork's model at all.
- **Both are implemented in the fork and both are default-off.** (A) is
  `-mthumb-immediate-latency` (`ARM_FLAG_THUMB_IMMEDIATE_LATENCY`, bit 24): in
  `arm_adjust_cost`, a true data dependence whose producer is a `(set (reg)
  (const_int))` returns `cost + 1`. It is only observable into a non-call insn,
  since edges into a call already return 1 earlier in that function. (B) is
  `-fno-sched-depend-count`, a gate on `haifa-sched.c:4097-4110`. With both off,
  `build_claimed.ts` gives `linked=1239 failures=0` — adding them changed nothing
  that already matched.
- **What they close.** `resource_3c7:0030` goes 4 → 0 mismatched bytes on
  `-mthumb-immediate-latency` alone (and `-fno-sched-depend-count` alone is a
  no-op there, as predicted). `resource_3cd:004c` goes 10 → 8 on (A) alone and
  8 → 0 with (A) and (B) together. Main-ROM `080babdc` is a third independent
  witness for (A): its natural 144-byte C differs only by the order of
  `mov r8,r3` and `mov r6,sp`, and the latency mode moves that pair for a
  4 → 0-byte result.
- **Scope:** measured on the `xgcc` fork at `-O2`. `old_agbcc` has no scheduler
  and cannot produce the interleave at all, in either direction. (A) is witnessed
  only on `movs`-immediate to `lsls`; the general "all ALU results cost 2" form is
  unmeasured, and the implemented flag is deliberately narrower than that — it
  fires only when the producer's source is a literal constant. (B) is witnessed
  only on r0-versus-rN pairs, and only *some* of those: see the per-site
  measurement above.
- **Recorded:** 2026-07-25; mechanism corrected and second witness added the same
  day; (B)'s global form refuted and both flags implemented and measured the same
  day; main-ROM witness added 2026-07-27.

### Identical large constants in an argument list are a basic-block question (2026-07-25)

- **Claim:** N identical constants ≥ 256 in one call's argument list collapse to
  one materialisation plus N-1 register copies when the source spells them as
  literals at the call, and stay as N independent `movs`/`lsls` pairs when the
  source assigns them to locals in an *earlier* basic block. Local CSE only
  unifies within a block, so the block boundary is the whole lever.
- **Mechanism:** `precompute_register_parameters` in `calls.c` forces any
  argument with `rtx_cost (value, SET) > 2` into a pseudo under
  `SMALL_REGISTER_CLASSES && *reg_parm_seen`. `arm_rtx_costs` returns 0 only for
  `(unsigned) INTVAL < 256`, and a `thumb_shiftable_const` costs
  `COSTS_N_INSNS (2) == 6`. So *every* argument constant ≥ 256 becomes a pseudo,
  and if several identical ones are precomputed in the same block, local CSE
  unifies them. This is unconditional in `calls.c`: no flag reaches it.
- **Evidence, `resource_37d:0054`, `37e:0054`, `37c:0054`** (76 bytes each, all
  exact). The residual was the argument block for
  `Func_0200012a(0x10000, 0x10000, 0x10000)`: five insns emitted (one
  materialisation plus two `adds rN,rM,#0`) against the reference's six (three
  `movs #128` and three `lsls #9`, interleaved). The two-byte body delta became
  a four-byte span delta through the `.align 2` before the literal pool.
  Hoisting the three values to C89 locals at the top of the function, with the
  call inside a later `if`, put the assignments in a different block from the
  call and closed all three functions with no flag route. On `37c` the same
  change also resolved a second residual, a `str`/`lsls` transposition at
  `02000064`, because the extra live pseudos shift the allocator's numbering.
- **Corpus check:** 39 sites in the reference overlay corpus show the split
  form and none show the CSE'd form. The only chained-copy sites are `f(v,v,v)`
  with a *variable*, which the default route already reproduces — so when a
  reference shows N independent split pairs, the source had named values, and
  when it shows copies, the source passed one value N times.
- **Consequence for reviewers:** locals that look gratuitous next to a call are
  load-bearing. `assets/code/resource_37{c,d,e}_c_02000054.c` carry a comment
  saying so; do not fold them back into the argument list.
- **When the lever is unavailable — `resource_37a:2614`, parked** (744 code /
  784 span bytes, best 604 mismatched). The lever above needs an *earlier* basic
  block to hoist into, and this function very nearly does not have one: the
  project's own walk finds 231 linear insns, 69 calls, **one** conditional
  (`0x0200287c`) and **one** unconditional branch (`0x020028ee`) in the whole
  744 bytes. The entry block therefore runs 617 of 744 bytes and holds nearly
  every call site, so for almost all of its argument constants there is no
  earlier block in which to name them. Before reaching for this law on a long
  function, count its branches; a near-branchless region cannot use it.
- **A mechanism that was proposed for `37a:2614` and does not hold.** An agent
  lane reported the function as a single straight-line block whose argument
  constants are over-CSE'd by `cse1`, on the theory that `update_equiv_regs`'
  `REG_BASIC_BLOCK (regno) < 0` gate can never fire. Two direct counts refute
  the shape of that account and it should not be repeated. It is not one block
  (two branches, above). And the residual runs the *other* way: within the code
  region the reference emits **7** split `movs #imm` / `lsls #k` pairs and one
  `adds rN,rM,#0` copy, while the best candidate (`/tmp/ov37a2614/candD.c`,
  four separate `u8 *` locals) emits **15** split pairs and no copies. Our
  output over-splits where the reference shares; the CSE deficit is on our side.
- **Recorded:** 2026-07-25; `37a:2614` park and the refuted mechanism 2026-07-25.

### The grouped-DMA flag is one switch driving two opposed transforms (2026-07-25)

- **Claim:** `TARGET_GROUPED_DMA_STORE` cannot be used to obtain the head of a
  region without also accepting a rewrite of its tail, because two unrelated
  transforms read the same flag and there is no way to enable one alone.
- **Mechanism:** `arm_pre_reload` early-returns unless the flag is set
  (`gcc-2.96/gcc/config/arm/arm.c:6557`), so its `gen_thumb_store_multiple3`
  — the only producer of the `stmia`-triple plus `subs r3, #12` shape — needs
  it. `arm_reorg` calls `thumb_order_grouped_dma_store` under the identical
  test (`arm.c:6869`), and that function's third loop (`arm.c:6503-6543`)
  matches `move (reg <- CONST_INT); load (other reg <- CONST_INT);
  shift (ASHIFT of the move's dest)` and executes `reorder_insns (shift,
  shift, move)`, turning `move; load; shift` into `move; shift; load`. The
  flag is a single bit (`arm.h:404`), so a region needing the first transform
  gets the second whether or not its tail can survive it.
- **Consequence for reviewers:** when the flag fixes a region's head and breaks
  its tail, that is not a source-shape problem and no rewriting of the C will
  reconcile it. Record it and move on, or split the flag in the fork — gating
  the third loop separately is the tracked fork change this motivates.
- **Evidence:** `08004760`. Its head requires the flag; its four-byte tail is
  `movs r2, #128; ldr r3, [pc, #28]; lsls r2, r2, #3`, which is exactly the
  triple the third loop rewrites. Both halves were read in the fork's source
  rather than inferred from the mismatch.
- **Why this is not yet a confirmed law:** it explains a region that is still
  open. It predicts a failure, and the prediction has one witness; a second
  region showing the same head/tail split would promote it.
- **Recorded:** 2026-07-25.

### A signed byte load may keep its expander form only through an unsigned temp (2026-07-25)

- **Claim:** reading a byte into an unsigned temporary and casting afterwards
  (`u32 cell = *row++; v = (s8) cell;`) preserves the three-instruction
  `extendqisi2` expander form — `ldrb`, `lsls`, `asrs`, with a destination
  register distinct from the load's — where the direct `v = *(s8 *) row++;`
  lets `combine` fold the pair into `sign_extend:SI (mem:QI)` and emit a
  single `ldrsb`.
- **Consequence for reviewers:** a reference showing `ldrb`/`lsls`/`asrs` on a
  byte that is plainly signed is not evidence of a missing optimization or a
  different compiler; it is evidence about where the cast sits relative to the
  load in the source.
- **Evidence:** `resource_3bc:004c`, where the change moved the region from 103
  mismatched bytes to 36 — the largest single step measured on it.
- **Why this is not yet a law:** the function is not closed. 29 bytes of
  residual remain, so the shape is not proven by an exact match, and the
  measured improvement could be partly incidental. Retest it on a second
  signed-byte region before relying on it.
- **Recorded:** 2026-07-25.

### GCC 2.96 nested-function static-chain register

- **Claim:** `080e73a0` reads r9 as a live-in value with no in-function
  write, wrapped in a callee-save push/pop dance — exactly GCC 2.96's
  nested-function static-chain convention (confirmed in vendored
  `gcc-2.96/gcc/config/arm/arm.h`: `STATIC_CHAIN_REGNUM` is r9 for Thumb,
  r8 for ARM). Compiling the function as a GNU C nested function inside a
  synthetic enclosing function structurally reproduces the exact
  push-r9/store-to-stack/dereference-chain-minus-offset shape, confirming
  the mechanism — but doing this for real requires reconstructing an
  *unknown* enclosing function, which is exactly the disallowed
  static/helper-function shape (LAWS.md publication rules), and the
  compiler additionally emits the nested function as a non-`.global`
  `Func_080e73a0.0` local symbol the harness cannot match by name. A plain
  register pin (`register void *g asm("r9")`) does not reproduce the
  save/restore dance either (68 vs 98 bytes) — no legal flat-file C shape
  produces this pattern. Route any function whose live-in-r9-with-no-write
  signature matches this to structural classification, not further search,
  unless the true enclosing function is independently recovered.
- **Further instances (2026-07-24):** `080bd850` and `08015fb8`, a family of
  two — both call `Func_080072f0`, and they are the only two ROM functions
  with this shape besides `080e73a0`. On `080bd850` the 52-byte body is
  byte-for-byte identical to the reference including all three pool words
  (`0x03001B10`, `0x06010000`, `0x03000164`); the entire 20-byte residual is
  the static-chain frame, and it is four separate sequences, not one:
  the high-register "desperation" save `mov ip,r3 / mov r3,r9 / push {r3} /
  mov r3,ip`, the slot allocation `sub sp,#4`, the spill `mov r3,r9 /
  str r3,[sp,#0]`, and the teardown `add sp,#4 / pop {r3} / mov r9,r3`.
  `current_function_needs_context` in `function.c` is what emits all of it —
  `expand_function_start` allocates the slot and does
  `emit_move_insn (last_ptr, static_chain_incoming_rtx)`. Compiled as a GNU C
  nested function inside a synthetic enclosing function the region reaches
  70/72 exact (the two differing bytes are a reload-scratch pick, r2 vs r3),
  which settles the mechanism beyond doubt while remaining an illegal
  deliverable for the same two reasons as `080e73a0`.
- **Judge the body, not the byte count.** `decomp_diagnose` scores the saved
  `080bd850` candidate at 68 mismatches / 52 bytes while the earlier, worse
  candidate scored 62 / 68 — the old one only scored lower because its junk
  `volatile` stores padded the output nearer the 72-byte region. On any region
  in this family the missing static-chain frame guarantees a large residual
  that says nothing about the C, so compare instruction-by-instruction from
  the top of the body instead.
- **Two real body fixes came out of it anyway**, both reusable: the
  `struct MapBase { u16 unused; u16 offset; };` +
  `((struct MapBase *)0x03001B10)[object[0x1C]].offset` form, copied from
  matched sibling `src/0800be70.c`, makes the access a COMPONENT_REF that does
  not fold the `+2` into the pool word; and dropping `volatile` from the two
  byte loads fixes both their order and an extra `muls` copy.
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

### Signed less-than against a literal canonicalises (2026-07-25)

- **Fingerprint:** the reference has `cmp rN, #K` / `blt`, the candidate has
  `cmp rN, #K-1` / `ble`, everything else identical, and the branch targets
  agree. Two instructions, four bytes, cascading into every offset after them.
- **Mechanism:** GCC rewrites a signed `x < K` into `x <= K-1` before branch
  emission. The rewrite is not reachable from the source: `x < K`,
  `x >= K` with the arms swapped, `K <= x`, `!(x < K)` and `x > K-1` all
  produce the `K-1`/`ble` form, and the value's type (`s16` vs `s32`) does not
  change it.
- **Scope:** blocks `resource_3a9:00e4` at 2 mismatched bytes and
  `resource_37f:00ec` at 16 (its four-way range chain multiplies the cascade).
  Both are otherwise exact, with every pool word, register and branch target
  matching.
- **Located and closed (2026-07-25).** The rewrite is `fold`'s "Change
  X >= CST to X > (CST - 1)" in `fold-const.c`, not a back-end choice, which
  is why every source spelling folds to the same form; `combine.c`'s
  `simplify_comparison` carries a second copy. Both are now gated behind
  `-fno-canonicalize-comparison` in the alchemy-gcc fork, on by default and
  inert until a source is routed through it. `resource_3a9:00e4` matches
  exactly under it and is installed; `resource_37f:00ec` goes from 19 to 14
  mismatched bytes, its residual now a basic-block ordering question rather
  than a comparison one.
- **Recorded:** 2026-07-25.

### Commutative register operand order is not source-reachable (2026-07-25)

- **Fingerprint:** the candidate is exact except for `adds rd, rA, rB` where the
  reference has `adds rd, rB, rA`, or `ldrb rd, [rA, rB]` against
  `ldrb rd, [rB, rA]`. Two bytes per site, and the branch offsets after it
  usually still agree, so the mismatch count stays tiny.
- **Not reachable from C.** Writing `base + offset` and `offset + base`, naming
  the offset in a local first, and casting either side to the pointer type all
  produce the same operand order. `expand_binop`'s commutative swap
  (`optabs.c:651`) explicitly does *not* fire for register-plus-register with a
  fresh target -- it swaps only for `CONST_INT` or when the target already
  equals `op1` -- so the order is fixed before it, at tree level.
- **Three witnesses, all otherwise exact:** `resource_394:0b3c` (2 mismatched
  bytes), `resource_394:0be4` (4), `resource_371:0350` (4, the same rule
  applied to `ands`). Each is semantically correct with every pool word,
  register and branch target matching.
- **Why it is not gated.** Unlike the comparison canonicalisation, which had a
  single site rewriting one comparison code, this one governs operand order for
  every commutative operation in the tree. A flag would change far more code
  than the three functions it would close, and the reference's order is not
  obviously derivable from a rule we could state. Left open deliberately.
- **Recorded:** 2026-07-25.

### Overlay singleton idioms (2026-07-25)

Recurring source-shape rules found while converting the overlay
singletons. Each is an installed byte-exact match.

- **A byte threshold is tested on the shifted value, not a truncated copy.**
  The reference reads a byte, adds one, stores it back, then compares
  `value << 24` against `threshold << 24`. That is what GCC emits for
  `(u8) n > 80` when `n` is an `int`; declaring `n` as `u8` instead inserts a
  `lsls/lsrs` truncation pair the reference does not have. Keep the counter an
  `int` and cast at the comparison. Evidence:
  `assets/code/resource_3b1_c_020000d8.c`.
- **A large addend that only reaches a halfword store must be a symbol.**
  `*(u16 *) p = v + 0xFA10F601` narrows to `+ 0xF601` because only the low half
  survives the `strh`; the reference keeps the full pool word, so the addend is
  a link-time constant (`(s32) &Value_fa10f601`) that GCC cannot fold.
  Recorded against `resource_3c9:0400`, which reaches 2 mismatched bytes this
  way and is otherwise blocked on register choice.
- **Selector variants beyond the plain chain.** The `Data_02000240[224]`
  preamble also feeds or-conditions (`v == A || v == B`), negated guards
  (`if (v != A) { ... }` around the chain), and range checks
  (`v <= HI && v >= LO`). All three convert with the same symbol spelling.
  Evidence: `resource_3bf_c_02000a34.c`, `resource_3b2_c_02000d48.c`.
- **Label-refused regions are shared epilogues, and the refusal is right.**
  Six verified byte-exact overlay candidates cannot be spliced:
  `resource_38f:0284`, `resource_3b4:0a50`, `resource_3b3:14c4`,
  `resource_39a:0f30`, `resource_3a6:0d80`, `resource_3c8:2f8c`. The
  mechanism, confirmed from ROM bytes rather than inferred: a larger enclosing
  function reaches these regions with a real Thumb `bl` (e.g. `f000 f82b` at
  `0x02002f6a` targeting `0x02002fc4`), lands on a `ldr r0, pool` /
  `b` pair, and exits through the shared `pop {r1}` / `bx r1`. The `bl`'s own
  link register is discarded: the pop retrieves the *enclosing* function's
  saved `lr`, so the fragment both sets a return value and returns from its
  caller's caller. That is a deliberate size optimisation, which means the
  region is not a standalone function and its epilogue is not the candidate's
  to own. Splicing it would delete a live branch target. These belong to the
  multi-region-function classification, not to the conversion queue; the C is
  correct and parked, and closing them is boundary work.

### Overlay pool words are symbols, not integer literals (2026-07-25)

- **Claim.** In overlay C, every constant that the reference keeps in its
  literal pool must be spelled as a declared symbol, not as an integer
  literal. Spelling it as a literal changes two things at once: a small
  comparison operand becomes a `cmp rN, #imm` instead of a pooled
  `ldr rN, pool / cmp rM, rN`, and two literal return values let GCC collapse
  an if/else into load-one-then-conditionally-replace, losing the reference's
  `bne / ldr / b / ldr` shape. Declaring them —
  `extern u8 Value_00000067;` for the compared value and
  `extern u8 Data_02009c04[];` for each returned address — restores both.
- **Evidence.** `resource_3aa:0030` is 30 mismatched bytes with the m2c draft
  and 27 with the literal-spelled law-conformant draft; with all three pool
  words as symbols it is **exact**. The same template then matched 21 further
  instances byte-for-byte on the first compile.
- **Corollary, the shape scan.** Whole cohorts fall to one template. A byte
  signature over the reference (`push {lr}`, `Data_02000240[224]` index,
  pooled compare, two-way return) finds 22 instances across the overlays;
  extracting each one's three pool words and filling the template converted
  all of them.
- **The selector generalises to N ways.** The same preamble feeds a chain of
  `ldr r3,pool / cmp r2,r3 / bne +1 / ldr r0,pool / b end` blocks with a final
  unguarded `ldr r0,pool`, one to eleven deep. Scanning for the chain and
  filling one template produced **58 more functions, all byte-exact on the
  first compile** (54 adopted; four refused by the straddling-label check
  because code outside branches into their region). Read the value once into
  an `s16` local when the chain is longer than one comparison; a single
  comparison reads the array directly. Overlay C functions went 92 to 167 in
  one pass, so the shape scan, not the near-miss queue, is where overlay
  conversions come from.

### Overlay discovery misses externally-called functions (2026-07-25)

- **Measurement.** Of those 22 selector instances, `overlay_inventory.ts` had
  inventoried only 5. The other 17 are not contained in a larger discovery and
  are not misclassified — they are simply absent. Discovery seeds from the
  first prologue, from overlay-internal tagged pointers, from the control-flow
  walk, and from prologues immediately after a return; a function whose only
  callers live in the main image or in an external pointer table satisfies
  none of those.
- **Attempted fix, and why it was reverted.** Seeding every unclaimed
  word-aligned `push {..., lr}` does find all 17, but the walk cascades from
  those seeds and raw discoveries go from 565 to 19,596 (14,030 of them data
  walks). A post-walk quality gate on the seeds themselves does not contain it,
  because the inflation comes from functions discovered transitively. Do not
  re-attempt without a containment strategy for the cascade.
- **What shipped instead.** `overlay_adopt.ts --span BYTES` adopts an entry the
  inventory does not list. The inventory was never the safety gate: the region
  boundary check, the straddling-label check and the rehearse-and-compare
  against the current overlay bytes all read the assembly. Their strength is
  visible in `resource_38f:0284`, which the label check refuses because code
  outside the region branches into it.
- **Consequence for the frontier count.** The "78 decompilable entries"
  measurement is a floor, not a ceiling: it counts what discovery found. At
  least 17 convertible functions sat outside it, and the shape scan is the way
  to find more.

### Overlay C may retain externally-called internal entries (2026-07-28)

- **Claim.** Replacing an exact overlay routine with C does not require deleting
  secondary entry labels inside its byte span. Split the assembly placeholder
  at each externally referenced `.L_<address>` and retain the label between
  the two `.space` segments. The compiled C still owns every byte in the span,
  while callers continue to branch to the original address.
- **Safety gate.** Only labels referenced as complete symbol tokens outside the
  adopted region are retained. Their address-derived offsets must lie inside
  the region, and `overlay_adopt.ts` still rebuilds and compares the complete
  overlay before accepting any change. Alias preservation therefore weakens
  neither the region-boundary check nor byte identity.
- **Evidence.** The four selector routines previously parked by the
  straddling-label check now rehearse and rebuild exactly:
  `resource_39a:0f30` (156 bytes, two entries),
  `resource_3b4:0a50` (84 bytes, two entries),
  `resource_3b3:14c4` (100 bytes, one entry), and
  `resource_38f:0284` (48 bytes, one entry). This moves 388 executable bytes
  from ordinary overlay assembly into maintainable C without moving any entry
  address.

### Load a selected halfword through a wide unsigned carrier (2026-07-28)

- **Claim.** When a branch selects a `u16 *` and the selected halfword is then
  written to a hardware register, load it into a `u32` local before naming the
  destination. This keeps the source semantics explicit and makes the vintage
  compiler emit `ldrh selected`, then load the destination address, then
  `strh`.
- **Evidence.** `resource_397:026c` was five differing bytes when written as
  `Data_0400001c = *source`: the compiler loaded the destination first and used
  the opposite registers. A separate `u16` local selected `ldrsh` and was
  eleven bytes away. A `u32 value = *source` reconstructs all 52 bytes exactly
  and passes whole-overlay adoption.

### Constants stored after a call are assigned after the call (2026-07-25)

- **Claim:** when a function calls, then stores a small constant, the reference
  materialises that constant with `movs` *after* the call and keeps the minimal
  push mask. Two candidate defects collapse into this one source rule:
  writing the store as a literal (`*(u16 *) 0x05000000 = 0`) pools the value
  through the HImode path above, and hoisting the constant into a local
  declared *before* the call (`s32 zero = 0; f(); *p = zero;`) makes it live
  across the call, forcing a callee-saved register and widening `push {lr}` to
  `push {r5, lr}`. Declaring the local but assigning it after the call —
  `s32 zero; f(); zero = 0; *(u16 *) ADDR = zero;` — gives both the `movs`
  materialisation and the narrow prologue.
- **Evidence, `resource_3ca:004c`.** The law-conformant draft (declared
  `extern s16 *Data_03001ebc;` plus the `[182]` array-index form) with the
  literal store is 13 mismatched bytes over 40 emitted against 36 expected —
  the delta being the unwanted pool word the HImode entry predicts. Hoisting
  the zero before the call fixes the pool word but costs `push {r5, lr}` and
  scores 17. Assigning after the call gives **size 36/36 and 4 mismatched
  bytes**, with every load, store, register and pool word exact.
- **Residual, and what it is not.** The remaining 4 bytes are one
  transposition: the reference emits the value (`movs r2, #0`) before the
  address (`movs r3, #160 / lsls r3, r3, #19`), the fork emits the address
  first. It is *not* scheduling — `-fno-schedule-insns`,
  `-fno-schedule-insns2`, `-O1`, `-mthumb-load-latency-one`,
  `-mthumb-immediate-latency` and `-fno-sched-depend-count` all leave it at 4,
  so the order is fixed at RTL expansion. Five source spellings (pointer local
  assigned after the value, `s16`/`u16`/`s32` value types, `volatile` store,
  declared destination array) are all also 4. Closing it needs the expander,
  not another source variant.

### The 0x0030 duplicate family is one copy from exact (2026-07-25)

- **Scope.** Twelve overlay entries share fingerprint `1fwqz6zhzfrzo`
  (`resource_373/389/391/392/393/39f/3b2/3b4/3bf/3c4/3c5:0030` and
  `resource_3b5:0040`, 672 bytes). One source shape converts all twelve.
- **Shape.** A squared-distance helper over two `s32 *` walked with `*p++`:
  `dx = *a++ - *b++; dy = *a++ - *b++; dz = *a - *b;` then three `>>= 16`
  shifts, then the products. Naming matters and is not free: naming the first
  and third products while leaving the middle one inline
  (`xx = dx*dx; zz = dz*dz; f(xx + dy*dy + zz, zz, dy*dy, 0x030001D8)`)
  reproduces the reference's `dx², dy², dz²` emission order. Naming all three,
  or none, or the first two, each rotates the allocator and scores worse
  (28, 26, 28 against 14).
- **`-mthumb-load-latency-one` is load-bearing here.** The same source is 18
  mismatched bytes without it and 14 with it; it is the fork flag added for the
  dependent-load delay, and this family is a second witness for it.
- **Residual.** 14 bytes, size exact at 60/60, everything through the third
  multiply exact. The reference copies the last addend before adding it
  (`adds r3, r1, #0 / adds r0, r0, r3`) where the fork adds in place
  (`adds r0, r0, r1`); the 2-byte shift cascades into the pool offset, the
  branch offset and the pop register. Four tail spellings (named sum,
  explicit copy temp, `+=` accumulation, CSE-split addend) are all 14, so the
  copy is allocator-internal, not shape-reachable.

### The reference pools HImode immediate stores (2026-07-25)

- **Claim:** a store of an integer constant through a `short`/`u16` lvalue
  materialises the value with a literal-pool `ldrh`, not a `mov`, even when the
  value fits the Thumb `I` constraint (0-255). `*(u16 *) p = 0xff` is
  `ldrh r3, .LN / strh r3, [r0]`. This is a property of the reference, not a
  fork defect, and it is load-bearing: 25 of the 1,239 byte-exact functions
  depend on it.
- **Mechanism.** `*thumb_movhi_insn` (`arm.md:4360`) spells its load alternative
  `"mn"`. The `n` makes a `CONST_INT` match alternative 1 as well as the `"I"`
  alternative at index 5, and because alternative 1 comes first, reload
  satisfies it with `force_const_mem`. The movhi expander has already turned
  `*(short *) p = K` into `(set (reg:HI) (const_int K))` + `(set (mem:HI) (reg:HI))`
  by then, so the constant is an HImode register load and the pool wins.
- **Why it looks like a bug, and the measurement that says it isn't.**
  `*thumb_movqi_insn` spells the same alternative `"m"`, and
  `*thumb_movsi_insn` puts its `"I"` alternative *ahead* of the load. So QImode
  and SImode both emit `mov` and only HImode pools — exactly the shape of a
  transposition typo. Dropping the `n` does make HImode behave like the other
  two, and large HImode constants still reach the pool through `"m"`. It also
  regresses `build_claimed.ts` from `failures=0` to `failures=25`. Witness:
  `080b6a60`, whose `*output = 0xff` the reference pools. **Do not "fix" this.**
- **Corollary for candidate C.** A near-miss whose only defect is an unwanted
  pool word at an `strh` is telling you the *candidate* is wrong, not the
  compiler: the reference stored something that was not an HImode constant at
  expand time. `resource_3ca:004c` is the open example — 13 mismatched bytes,
  40 emitted against 36 expected, the whole delta being one extra pool word,
  where the reference has `movs r2,#0 / movs r3,#160 / lsls r3,r3,#19 /
  strh r2,[r3]`. Its value reached `strh` from an SImode register. Four source
  shapes that do not dodge it: `volatile` on the destination, hoisting the load
  into a local, spelling the destination `u16 *`, and hoisting the destination
  pointer into a local (that one is worse, 22).
- **Scope:** measured on the `xgcc` fork at `-O2`. QImode and SImode immediate
  stores are unaffected and use `mov`.
- **Recorded:** 2026-07-25.

### `resource_3ca:004c` narrows to one priority-ordered pair (2026-07-25)

- **State:** 4 mismatched bytes, 36/36 size, from 13 and 40/36. Not closed.
- **What fixed the size.** The candidate stored an HImode constant, which the
  fork pools (see the law above), costing an extra literal word. Giving the
  value an SImode home — `s32 z = 0; *(s16 *) 0x05000000 = z;` — makes the store
  `(set (mem:HI) (subreg:HI (reg:SI)))` and the pool word disappears. The
  declaration must sit **after** the call: declared before it, `z`'s live range
  crosses the call, the allocator takes call-saved `r5`, and the function grows
  a `push {r5}` / `pop {r5}` the reference does not have.
- **The residual is a priority decision, not a tie.** `.23.sched2` at the one
  divergent site:
  ```
  ;;       29   173     0     4    34     1    1 - 32   core	: 56 55 35
  ;;       51   173     0     3    35     1    1 - 32   core	: 55 52
  ;;	Ready list (t = 41):    29  51      -> schedules 51, reference wants 29
  ```
  Insn 29 is `mov r2,#0` feeding the `strh` directly; 51 is `mov r3,#160`
  feeding `lsl r3,r3,#19` and then the same `strh`. That extra hop makes
  prio 35 against 34, so `rank_for_schedule` returns on priority and never
  reaches any tie-break. **No tie-break flag can close this**, which is why
  `-fno-sched-depend-count` and `-mcall-arg0-move-first` are both no-ops here.
- **Why `-mthumb-immediate-latency` does not close it either.** It lifts both
  priorities, 34 → 35 and 35 → 36, because insn 51 is an immediate move too.
  The gap is preserved. Closing it would need the *store's data operand* edge
  to cost more than the ALU edge — immediate→store 3 against immediate→ALU 2 —
  which is unwitnessed and does not follow from defect (A).
- **The two sites disagree about `sched2`, in one function.** With `sched2` on,
  the prologue is exact (`ldr r3,.L / movs r2,#182 / ldr r3,[r3]` — the
  load-latency interleave) and the tail is transposed. With
  `-fno-schedule-insns2` the tail is exact and the prologue transposes instead.
  Both settings sit at 4 mismatched bytes. This is the second function known to
  want one `sched2` decision each way, after `resource_3a0_c_02000048.c`.
- **Best candidate kept at** `/tmp/best/resource_3ca_004c.c` (not adopted).
- **Recorded:** 2026-07-25.

### The overlay `0x0030` distance family walks its arguments with `*p++` (2026-07-25)

- **Claim:** the twelve-member `0x0030` family — `373`, `389`, `391`, `392`,
  `393`, `39f`, `3b2`, `3b4`, `3bf`, `3c4`, `3c5`, and `3b5:0040` — reads its two
  coordinate triples through *post-incrementing pointers*, not indexed fields.
  The reference emits `ldmia r0!, {r5}` / `ldmia r1!, {r3}`, which is what gcc
  generates for `*p++`; the m2c candidates spelled it `M2C_FIELD(arg0, s32 *, 0)`
  and got `ldr r3, [r1, #0]`. Rewriting the three component differences as
  ```c
  temp_r5 = (*arg0++ - *arg1++) >> 0x10;
  temp_r4 = (*arg0++ - *arg1++) >> 0x10;
  temp_r3 = (*arg0  - *arg1 ) >> 0x10;
  ```
  with `s32 *` parameters makes the whole nine-instruction load-and-subtract
  prologue byte-exact in all twelve.
- **Second lever, the squared terms.** The reference computes `d0*d0` first, so
  it needs its own temporary; the candidates left it inline in the call
  argument and got it emitted last. Hoisting it to `temp_r0` ahead of `temp_r2`
  and `temp_r1` makes all three `adds rN,rM,#0 / muls rN,rM` pairs exact.
- **Effect:** 33-34 mismatched bytes down to 19-20, uniformly, at the correct
  60/60 size. 168 bytes across the family. Not closed.
- **This corrects the recorded cause.** The family was parked as blocked on the
  `*thumb_mulsi3` earlyclobber item in the fork lane. It is not: with the two
  levers above, all three multiply pairs match byte-for-byte, and the
  earlyclobber never enters. That fork item should be re-justified on a
  different witness or dropped.
- **The residual, identical in all twelve, is three things.** (1) The reference
  issues `subs r3,r3,r2` in the cycle straight after the `ldr r3,[r0,#0]` that
  feeds it, which the fork's two-cycle load latency forbids, so the fork delays
  it past two `asrs`. (2) The reference copies `adds r3,r1,#0` before its last
  `adds r0,r0,r3`, where the fork adds `r1` directly — the fork is two bytes
  shorter and pads. (3) The reference's interworking epilogue pops into `r1`,
  the fork into `r0`. `-fno-schedule-insns2` makes (1) worse (30 bytes), and
  `-fno-regmove` does not touch (2).
- **Candidates kept at** `work/near/overlay_0030_family/` (not adopted).
- **Recorded:** 2026-07-25.

### Fixed RAM addresses are declared globals, not `(void *)` literals (2026-07-25)

- **Claim:** where a function touches a fixed RAM address more than once, or
  touches two fields of the same object, the reference keeps the base in a
  register for the whole body. An m2c candidate that spells the address as
  `M2C_FIELD((void *) 0x02009930, s32 *, 0)` re-materialises the constant at
  every use — a fresh `ldr rN, [pc, #k]` each time, and a fresh pool word.
  Declaring the object instead, `extern s32 Data_02009930[];`, gives the
  reference's shape. The repository already uses this convention
  (`extern s16 Data_02000240[];` and friends).
- **Corollary, the array-index form.** A base that is indexed rather than
  offset also changes the address arithmetic. `Data_02000240[224]` on an
  `s16 []` emits `ldr r3,pool / movs r1,#224 / lsls r1,r1,#1 / adds r3,r3,r1`,
  where the literal-address spelling emits a single pool word holding the
  already-summed address. The reference uses the former, so the base symbol and
  the index are both recoverable from the emitted arithmetic.
- **Evidence, `resource_3b8:0030`** (48 bytes). The literal spelling gives
  32/48 and 43 mismatched bytes; `extern s16 Data_02000240[]` with `[224]`
  gives 40/48 and 34, and reproduces the whole address computation exactly.
  The base 0x02000240 recovered this way is a symbol the tree already declares.
- **Evidence, `resource_3b0:0030`** (76 bytes). The reference holds 0x02009938
  in `r4` and 0x02009930 in `r5` across the body — hence its `push {r5, lr}` —
  and walks a third pointer with `ldmia r3!, {r1}`. The candidate reloaded both
  constants from the pool at every field access. Declaring both and writing the
  walk as `*temp_r3++` takes it from 61 mismatched bytes to 23, with the entire
  middle of the function exact. The residual is at the two chained
  dereferences of `**(s32 ***) 0x03001E70`: the reference issues them
  back-to-back into the same register, the fork's two-cycle load ready-delay
  fills the gap with an unrelated pool load. `-mtune=arm8`, `-mtune=arm9tdmi`
  and `-mtune=strongarm` all make it worse (36), so the `arm7tdmi` timing is
  not the thing that is wrong.
- **Residual on `3b8:0030`,** both unexplained: the reference compares against
  `0x8B` loaded from the pool rather than `cmp rN,#139`, though 139 satisfies
  Thumb's `I`; and it keeps a plain if/else with a `b` over the join where the
  fork loads the default before the compare and reloads on one arm, which is
  8 bytes shorter. `-fno-cse-follow-jumps`, `-fno-thread-jumps`, `-fno-gcse`
  and `-O1` are all no-ops on both.
- **Recorded:** 2026-07-25.

### The reference issues a dependent load in the next cycle (2026-07-25)

- **Claim:** the fork gives a load's result a ready-delay of 2, so a dependent
  insn cannot issue until two cycles later and `sched2` fills the hole with
  something unrelated. The reference issues the dependent insn in the very next
  cycle. Chained pointer dereferences are the clean witness: the reference emits
  `ldr r3,[pc,#60] / ldr r3,[r3] / ldr r3,[r3]` three in a row.
- **Mechanism.** `arm.md:263`, `(and (eq_attr "ldsched" "!yes") (eq_attr "type"
  "load,store1")) 2 2`. `arm7tdmi` is not in the `FL_LDSCHED` group so
  `arm_ld_sched` is 0 and this is the applicable line; the `ldsched yes` line at
  260 also gives ready-delay 2, so no tuning selects delay 1. `.23.sched2`
  confirms it directly: `insn 14: queued for 2 cycles`.
- **Implemented as `-mthumb-load-latency-one`** (`ARM_FLAG_THUMB_LOAD_LATENCY_ONE`,
  bit 30): in `arm_adjust_cost`, a true data dependence whose producer is a
  `(set (reg) (mem))` returns `cost - 1` when `cost > 1`. Default off, and
  `build_claimed.ts` is unchanged with it off.
- **It reproduces the reference's schedule and closes nothing.** On
  `resource_3b0:0030` the head becomes exact — the three chained loads go
  back-to-back and the unrelated `ldr r4,[pc,#56]` lands in the reference's
  slot — which no other lever achieves. But the chain then occupies `r1` where
  the reference uses `r3`, and that register cascade costs more than the
  schedule gains: 23 mismatched bytes without the mode, 29 with. It is a no-op
  on the whole `0x0030` family (19-20 either way), takes `3c8:00f6` from 22 to
  21, and makes `3ca:004c` worse (4 to 8). No allocation lever aligns the
  registers: `-mentry-low-register-order` gives 37, and
  `-mhigh-register-move-first`, `-mearly-frame-allocation` and
  `-mthumb-entry-literal-first` are all no-ops at 29.
- **So the defect is real and is not the whole story.** Whatever the reference
  does, it also allocates the dereference chain to a single register across all
  three loads. Until that half is understood the mode should stay unrouted.
- **Note on flag space:** bit 30 was the last freely usable bit of
  `target_flags`; bits 0-29 are taken and 31 is the sign bit, which the
  `-ARM_FLAG_*` negative entries in `TARGET_SWITCHES` cannot express. A further
  `-m` mode needs `target_flags` widened first.
- **Recorded:** 2026-07-25.

### The overlay frontier is 57 functions, not 573 (2026-07-25)

- **Measurement.** `out/decomp/overlays.json` lists 573 unconverted overlay
  entries totalling 216,458 bytes, which overstates the remaining work by an
  order of magnitude. Classifying them by the inventory's own fields (disjoint,
  in this order):
  ```
  structural_veneer            43
  data_walk                   237
  unresolved > 0               95
  jump_tables > 0               0
  code_bytes < 8              107
  span - code > 64             13
  code_bytes 8..40             25
  code_bytes 41..96            53   (plus 21 above 96)
  ```
  Only 78 entries are functions of a shape the pipeline will even attempt, and
  `overlay_match.ts --all --max-bytes 96` selects 57 of them.
- **Those 57 are the near-miss set already known.** Sweeping them produces 39
  compiled candidates whose mismatch counts reproduce `/tmp/near.list` exactly,
  and 18 build failures. There is no untried pool of easy overlay wins: every
  overlay function that can be compiled at all has been compiled, and what
  remains is the residual analysis in the entries above.
- **There is no untouched convertible block behind the filters.** Every one of
  the 95 `unresolved > 0` entries also has `code_bytes < 8`; the ordering of the
  classification above just counts them under `unresolved` first. So the 95, the
  107 sub-8-byte entries, the 237 `data_walk` and the 43 veneers are all the
  same kind of thing — data, padding, or two-byte fragments misfiled as
  functions, 482 entries in total. Closing them is an inventory-classification
  job, not a decompilation one, and it will not add a single compiled function.
  The whole decompilable overlay remainder is the 78, and the 57 the sweep
  selects are the only ones with candidates today.
- **Recorded:** 2026-07-25.


# Addenda (2026-07-26)

Established this session against the live tree. Kept separate from the body
above so the provenance stays clear.

## Screen a target before drafting it

`grep 'mov\s*ip, pc' asm/<stem>.s`. A hit means the return address is preserved
in `ip`, and **neither approved compiler can emit that**: the fork's only two
Thumb indirect-call patterns produce `bl _call_via_rN`, and so does `old_agbcc`.
43 `c_candidate` regions carry it, including two of the most inviting by size
(`08097a10` at 68 B, `080b7f20` at 80 B). `0800070c` is a related case — it
tail-jumps to RAM through `ldr r3, [pc]` / `bx r3`.

## Control-flow shape

- **One result block means one condition, not a chain of early returns.** If the
  reference zeroes a result register after the entry work and has a single block
  that sets it, write one disjunction assigning a variable — `s32 result = 0;
  if (a || b || c) result = 1; return result;`. Four `if (…) return 1;`
  statements materialise the 1 inside the first arm and move everything after
  it. 080b27b0: 36 halfwords as early returns, byte-exact as one condition.

- **A dispatch that tests every value up front is a `switch`.** `cmp #1 / beq /
  cmp #2 / beq / b` is a small switch. An `if/else if` chain gives
  `cmp #1 / bne / body / cmp #2 / bne / body`. 0800c0f4: 36 halfwords as a
  chain, byte-exact as a switch.

- **A duplicated match body means loop rotation — write the rotated form.** Peel
  the first test out and duplicate the body, as GCC's own rotation produces.
  The natural `do { … break; } while` lets GCC hoist the match block's constants
  into callee-saved registers and enlarge the prologue. 08004278: 58 bytes out
  natural, 4 bytes explicit.

## Constants and widths

- **Storing a literal below word width goes through the pool.**
  `*(u16 *)P = 0;` emits `ldrh` from the constant pool; assigning through a
  same-width local emits `movs`. This one keeps recurring — it decided
  080b5b08, 080a9d3c and 080175c0.

- **An accumulator must be as wide as the arithmetic.** A `u16` running sum
  wraps every `+=` in `lsls #16 / lsrs #16`. The reference's plain `adds` means
  a 32-bit local narrowed only at the final store. 080060e8: 54 halfwords at
  124 B against 22 at exactly 116 B on that change alone.

- **Related pool constants get derived from each other.** GCC turns a second
  nearby constant into `subs r0, #4` off the first. If the reference loads both
  from the pool, spell them as distinct `Value_<addr>` externs. `Value_<8 hex>`
  links to that address; a trailing `_a`/`_b` names a second symbol for the same
  address, which is how the stock objects kept duplicate pool words alive.

- **A compound assignment picks a commutative operation's destination.**
  `control = 0x84000000 | words;` computes into the variable's register and
  copies; `control = 0x84000000; control |= words;` computes into the constant's
  register and the copy disappears. Operand order in the expression does
  nothing — GCC canonicalises. Worth 9 halfwords on 08021be0, and *harmful* on
  080052f4, so check both ways.

## Indexing

- **`array[index]` versus a walking pointer is a real choice.** Strength
  reduction rewrites an index into a pointer; where the reference keeps
  `ldrb r3, [r2, r6]` with the loop counter as the offset, route the stem
  through `-fno-strength-reduce` (080a9d3c). Where the reference walks, do not.
  080b90ac wants the element-index spelling and gets *worse* with the flag.

- **A register-offset load needs its displacement in its own local.** Inline,
  GCC emits a separate add; hoisted into an `s32 offset`, it emits the single
  `ldr r0, [r5, r3]`. 08011fd8 went 100 B to 96 B on that. The inverse also
  happens (080ae99c wants the separate add), so match what the reference does.

## DMA descriptors

- **Grouped DMA is a compiler mode, not a source shape.** `stmia rN!, {r0, r1,
  r2}` then `subs rN, #12` is `-mgrouped-dma-store`. The source is three locals
  assigned *first*, then three stores through a `u32 *`. Constants written
  inline lose the grouping, because the address materialisation lands between
  two stores and `arm_pre_reload` requires them adjacent.

- **Struct fields work; whole-struct assignment does not.** `dma->source = …`
  groups exactly like `dma[0] = …`; `*dma = (struct …){…}` lowers to `memcpy`,
  which the gate rejects as an unsupported external symbol.

- **Zero-fill descriptors take the address of an uninitialised local.**
  `u32 zero; u32 *source = &zero; *source = 0;` then the three stores. Sixteen
  never-attempted regions carry this fingerprint; two of them converted on the
  first draft.

- **Descriptor pointer and status pointer are two pointers.** When a region
  writes a descriptor then polls DMA3CNT, declare both and make only the polled
  one `volatile`. One volatile pointer for everything loses the grouping; one
  plain pointer for everything loses every descriptor but the last.

- **A VLA in a nested block, never `__builtin_alloca`.** A reference frame with
  *two* stack-pointer saves (`mov r7, sp` and `mov r8, sp`, restored in that
  order) is a variable-length array declared in a nested block, which emits both
  a function-level and a block-level save. `__builtin_alloca` emits only the
  first and cannot produce that frame.

## Compiler behaviours worth knowing

- **The pre-reload scheduler is inert.** 40 converted sources compile
  byte-identically with `-fschedule-insns` and `-fno-schedule-insns`. Every
  member of `UNSCHEDULED_SOURCES` is carried by `-fno-schedule-insns2` alone.

- **`arm_reorg` pulls split constants back together, and sometimes should not.**
  `-fno-thumb-contiguous-immediate` disables it. 080a1090 was four bytes out for
  a whole session on exactly this and is byte-exact with the flag.

- **Read the RTL dumps before proposing a mode.** On 080a1090 three modes were
  proposed by inspection and every one made it worse (4 bytes to 16, to 29, and
  no change). `-da` plus gating passes one at a time found the real cause in
  twenty minutes. The `.18.greg` and `.23.sched2` dumps show what each stage
  actually did; sched2 is frequently innocent.

## Addendum (2026-07-26): the DMA descriptor plus completion poll

Eight `c_candidate` regions program a DMA descriptor and then poll DMA3CNT until
the transfer finishes — `08005a78`, `080058ac`, `08005b64`, `08094730`,
`08005920`, `08077d38`, `0808e9c0`, `080030f8`. `08005a78` is the first of them
converted, and it needed three things the earlier laws did not cover.

- **The grouped store's "three locals first" rule is stricter than it reads.**
  `store1` must be the *immediately* next insn after `store0`; only one gap is
  tolerated, between `store1` and `store2`, and only a constant setup. So
  `dma[1] = (u32)buffer;` fails — the address computation lands between the
  stores. Assign every descriptor word to a named local *before* the three
  stores, including the destination, or nothing groups.

- **One pool word loaded twice is not a source shape.** The reference loads
  `0x040000d4` once for the transfer's base and again for the poll. Both are the
  same integer constant in the same basic block, cse merges them, and 2.96 has
  no `-fno-cse`; `-fno-gcse` only reaches the cross-block case. Two
  materialisations of one constant require two pseudos and only the compiler can
  make them: `-fthumb-split-group-base`.

- **Descriptor setup order permutes the constant pool.** When the source word
  needs arithmetic (`record + 80`), the interleaved `add` hides the three setup
  insns from `thumb_order_grouped_dma_store` and the control load stays hoisted.
  That misorders the pool as well as the insns, so it was worth 9 halfwords, not
  4. `-fthumb-group-control-last` sinks it.

Two further notes from the same region:

- **Declaration order of the descriptor locals does nothing.** All five
  permutations of `source`/`dma`/`control` score identically; GCC canonicalises
  it. Do not spend variants on it.

- **Hand-reorder the generated assembly before writing a compiler hook.** Take
  the `.s`, move the insns into the reference's order, reassemble and relink. If
  that is not exact, the hook was never going to be enough. On `08005a78` it
  came back EXACT, which is what justified two new options; the fork's own
  history is full of modes proposed by inspection that made regions worse.

## Addendum (2026-07-26): three small regions traced to compiler differences

Recorded so nobody re-derives them. Full traces in `work/hand/<stem>/NOTES.md`.

- **`080a1f74`** is size exact at 96B with the switch dispatch, pool order and
  rotated loop all matching. What is left is that GCC 2.96 lowers `(s8)x == -1`
  by sign-extending (`lsls #24 / asrs #24`) and comparing against -1, where the
  reference compares both sides shifted (`lsls #24` on each, against
  `0xFF << 24`) and never materialises the extension. Holding -1 in a register
  across the loop then pushes the index into `ip`. Only four `c_candidate`
  regions share the pattern. Two source facts from it *are* reusable: the four
  nearby ROM pointers must be `extern Data_<addr>` symbols or GCC derives them
  from one another, and the `u8`-compared-to-`0xff` reading is wrong (it is
  84B, shorter than the reference).

- **`0800651c`** disables interrupts with `strh r0, [r0]` — storing the IME
  pointer's own low halfword, 0x0208, whose bit 0 is clear. Our compiler will
  not produce that: `*ime = 0x0208` pools the constant separately (64B → 82B)
  and `*ime = 0` uses a zero register. Source-level findings that did help: a
  `u16` local round-trips through `lsls/asrs` where `u32` does not, and the
  cleared globals must be `extern Data_` symbols or GCC derives 0x02002008 from
  0x02002080 with `subs r3, #120`.

- **`080049ac`** has exactly the twelve consecutive word stores that
  `thumb_group_four_word_records` wants, and it is the cleanest instance of that
  shape left. Two things block it. The four-word transform only runs inside
  `thumb_order_grouped_dma_store`, so it needs `-mgrouped-dma-store`; but that
  also enables the three-word grouper in `arm_pre_reload`, which runs first and
  consumes stores 0-2, so by `arm_reorg` there is no run of twelve left. **The
  two transforms conflict, and the three-word one should decline a run that
  continues to twelve.** Even fixed, the four-word transform requires
  `regs_ever_live[0]` clear and this function returns a value in r0. It
  commandeers hard registers after reload, so that guard is not safe to relax on
  inspection — it wants a real liveness test at the insn.

## Addendum (2026-07-26): masking versus shifting is mostly a TYPE choice

**81 `c_candidate` regions zero-extend a 16-bit value by anding against a
pooled `0x0000ffff`; 33 others use the `lsl #16 / lsr #16` pair.** No converted
source reproduces the masked form. That looked like a compiler difference. It
is mostly not — it is a choice of parameter and variable type, and it is
available today:

    u32 explicit_mask (u32 x)   { return x & 0xffff; }
        ldr r0, <pool 65535> / and r0, r0, r3        <- the masked form

    u32 from_u16_param (u16 x)  { return x; }
        lsl r0, r0, #16 / lsr r0, r0, #16            <- the shift pair

With two masked values GCC loads `0xffff` once and shares it, exactly as the
references do. **So a region showing `ands` against a pooled 0xffff wants wide
(`u32`/`s32`) parameters and locals with explicit `& 0xffff`, not `u16` typed
ones.** A `u16` parameter forces the shift pair and can never match. This is
the single most reusable finding of the session and it is source-level.

Do not reach for the compiler here. `zero_extendhisi2` is *not* the path a
promoted `u16` parameter takes — a flag added there has no effect whatsoever
(built and measured, then reverted).

**The same law covers locals, and it is what produces the mid-function pool.**
Probed:

    u32 value = base + 8;   t->a = value;   t->bits = value;   /* 9-bit field */
        add / strh raw / ldrh <pool 0x1ff> / and / ldrh field / and / orr / strh
        then `b' over a mid-function pool                <- the reference shape
    u16 value = base + 8;   same two stores
        add / lsl #16 / lsr #16 / ...                    <- cannot match

So: **store the raw wide value to the u16 field, then let the bitfield insert
mask it.** Declaring the intermediate `u16` forces a truncation the references
never have. Note the masked form also loads small pool constants with `ldrh`,
and puts the pool mid-function with a branch around it — both fall out of the
`u32` form and neither is reachable with `u16`.

### The family is 40, not 81 — and how the count went wrong twice

The figures published earlier today (81, then 74) were both inflated by a lazy
fingerprint: "file contains a pooled `0x0000ffff`" AND "file contains an
`ands`", with no check that they were the same register. Resolving pc-relative
loads against the pool and tracking register liveness gives the real numbers:

| | count |
| --- | --- |
| regions with a pooled `0x0000ffff` | 105 |
| …where a live `0xffff` is genuinely an AND source | **40** |
| …pooled for something else entirely | 65 |

Most of that 65 is a completely different idiom: `if (x < 0) x += 0xffff;
x >>= 16` — the rounding constant for a signed divide by 65536, as in
`08094154`. It has nothing to do with masking.

Of the real 40: **32 mask once** and should be reachable with `u32` locals,
**8 mask twice** (`(x & 0xffff) & 0x1ff`) and need the combine our GCC performs
and the reference declines. Both regions small enough to have been drafted so
far — `080b0a20` at 29 insns and `080a1a40` at 49 — are in the *twice* group,
so the reachable 32 start at 121 instructions.

**The lesson is about the measurement, not the masks.** A fingerprint that
matches on two independent features of a whole file will over-count badly; the
family shrank by half once the pool word was actually resolved to the register
that consumes it. Resolve the operand before quoting a number.

Related and reusable regardless: an ordinary C bitfield reproduces the
`ldrh / ands / orrs / strh` insert shape correctly. Only nine converted sources
use one, against 95 `c_candidate` regions carrying the insert fingerprint.


## Addendum (2026-07-26): narrow types are almost always the wrong reading

Confirmed independently three times today, and it is the cheapest large win
available when a draft is close but too long.

**A narrow parameter or local costs an extension the references do not have.**

| written as | emitted |
| --- | --- |
| `u16 x` parameter, only ever stored with `strh` | `lsls #16 / asrs #16` at entry |
| `s32 x` parameter | `adds rN, r0, #0` |
| `u16 v = *(u16 *)p;` then used | `ldrsh` plus `lsls #16 / lsrs #16` |
| `s32 v = *(u16 *)p;` then used | `ldrh`, which already zero-extends |
| `u16` local passed to a 32-bit parameter | `lsl #16 / lsr #16` pair |
| `u32` local with an explicit `& 0xffff` | `and` against a pooled 0xffff, shared across uses |

Evidence: `080936a0` (`arg1` is `s32` though only ever `strh`-stored, worth 4
bytes), `080ae9f0` (`s32 value` from a `u16 *` load, worth 8 bytes and 32
halfwords), and the direct probe behind the masking addendum above.

**So: default every parameter and local to `s32`/`u32` and narrow only at the
point of use.** The store width comes from the pointer cast or the struct member,
not from the variable's type. A narrow variable is only right when the reference
actually shows the truncation.

**Related: mutate the parameter rather than introducing a local.** `arg2 -= 3`
emits the two-operand in-place `subs r2, #3`; `adjusted = arg2 - 3` emits the
three-operand `subs r2, r1, #3` and adds a live value that perturbs the whole
allocation. Verified on `080ae9f0`, worth 4 halfwords and the correct prologue.


## Addendum (2026-07-26): spell a bitfield extract the way the reference does

The two natural C spellings of "take bits n..m" produce different, equally
correct instruction sequences, and GCC does **not** convert between them:

    (x << 26) >> 30      ->  lsl r3, r3, #26 / lsr r3, r3, #30        (2 insns)
    (x >> 4) & 3         ->  lsr r3, r3, #4 / mov r2, #3 / and r3, r2 (3 insns)

Both survive into a comparison unfolded — probed directly, `if (((x<<26)>>30) > 1)`
emits the shift pair then `cmp`/`bls`, exactly as the references do. **65
`c_candidate` regions contain an lsl/lsr extract pair across 95 sites**, so
reaching for `>> k & mask` out of habit costs an instruction each time.

Read the reference and match its form. The shift pair means the source shifted
left then right; the shift-and-mask means it shifted right then masked.

**And the meta-lesson, which cost time today: look at the disassembly, not the
score.** A region that got 4 bytes shorter was written up as "GCC folds the
extract into the comparison". It does not, and never did — the shortfall was a
missing register copy elsewhere. The count moved for an unrelated reason and the
explanation was invented to fit it.


## Addendum (2026-07-26): two compiler options built, measured, and reverted

Both were inert, both for the same reason, and the reason is worth more than
either option: **the behaviour was not in the layer the flag targeted.** Record
them so nobody spends the time again.

- **`-fthumb-zero-extend-mask`** — emit `and` against a pooled 0xffff from
  `zero_extendhisi2` instead of the `lsl #16 / lsr #16` pair. No effect: a
  promoted `u16` *parameter* never reaches that expander. The masked form turned
  out to be reachable from C all along by widening the type (see the masking
  addendum), so no compiler change was needed at all.

- **`-fno-tie-copied-quantities`** — stop `local-alloc`'s `combine_regs` tying a
  dying copy's source and destination into one quantity, so the move survives.
  Targeted at `08006088`, where the reference accumulates in r2 and copies to r0
  partway through. No effect, and the RTL dumps say why: **there are zero
  pseudo-to-pseudo copies in `.00.rtl`**, before any pass runs. `result = packed`
  never becomes a copy insn — the expander treats the two names as one value —
  so there was never a tie for `combine_regs` to make.

**The check that would have caught both in a minute: dump the RTL and confirm the
construct you intend to change actually exists at that stage.** `-da` and a grep
of `.00.rtl` costs nothing next to building a compiler.

This is the same discipline that made the two *landed* options work. For
`08005a78` the payoff was verified by reordering the generated assembly and
relinking before any code was written; for `08006088` that same check proved a
single edit gives an exact link, but proving *what* to change is not the same as
proving *where* it lives.

## Addendum (2026-07-26): mask at the load, not in the arithmetic

`0800bfa4` converted on this alone, going from 15 halfwords out to byte exact:

```c
a = origin[0];                              /* 15 halfwords out */
x = thing->field8 - (a & 0xffff0000);
```
```c
a = origin[0] & 0xffff0000;                 /* byte exact */
x = thing->field8 - a;
```

Identical semantics. The first keeps the raw load and the masked value alive as
two values, so the subtraction needs a third register and the function grows a
callee-saved register it never needed — `push {r5, r6, lr}` against the
reference's `push {r5, lr}`. The second lets the mask overwrite the loaded value
in place, and the subtraction then reuses that register, which is what the
reference does.

Same family as the compound-assignment law above: **where a value's
transformations are split across statements decides which register each result
lands in.** But it reads backwards from the store case — here you want *more* in
one statement, not less. Put the mask on the load so the loaded register is
consumed rather than preserved.

Confirmed on the way in and worth reusing: reading both words of a pair into
locals *before* using either (`a = origin[0]; b = origin[1];` then the two
subtractions) reproduces the reference's up-front load pair. Folding each load
into its own subtraction interleaves them and costs 4 halfwords on its own.


## Addendum (2026-07-26): `Func_080072xx` is usually not a function

The compiler runtime's indirect-call veneer table sits at **0x080072e4**, one
four-byte entry per register:

| address | is |
| --- | --- |
| 0x080072e4 + 4N | `_call_via_rN` — literally `bx rN` |
| 0x080072f0 | `_call_via_r3` |
| 0x080072f8 | `_call_via_r5` |
| 0x08007300 | `_call_via_r7` |

Verified by disassembling the ROM, not assumed. So `bl Func_080072f8` is **an
indirect call through r5**, not a call to a function at that address, and a
prologue `ldr r5, <pool>` that looks dead is the target being loaded.

**200 `c_candidate` regions call a veneer, across 669 call sites.** Read one as
an ordinary function and the whole region will be mis-modelled — `080c00d8`
looked like three calls to three helpers and is actually three calls to one
IWRAM routine.

### How to spell it — and this is the opposite of what it first looks like

**Call the veneer directly, with the target as a trailing argument.** That is
what the 28 converted sources already do, and it is not shape debt as first
recorded here — it is the only spelling that controls the register:

```c
void Func_080072f0(void *a, u32 b, u32 c, void *target);   /* _call_via_r3 */
Func_080072f0(&Value_0600028c, 20, control, &Value_03000164);
```

The ABI forces the fourth argument into r3, `_call_via_r3` is the veneer at
0x080072f0, and the two agree by construction. Measured on `080b5ad4`: this form
emits `bl _call_via_r3` and `push {lr}`, both matching the reference.

The honest-looking function-pointer spelling does **not** work for matching:

```c
Routine run = (Routine)&Value_03000164;   /* pointer lands wherever */
run(&Value_0600028c, 20, control);        /* -> bl _call_via_r5, push {r5, lr} */
```

It is semantically clearer and the machine behaviour is identical, but nothing
ties the pointer to a chosen register, so GCC picks one and emits that veneer.
On `080b5ad4` it chose r5 and cost a callee-saved register; on `080c00d8` it
chose r7. Pinning a register in C is disallowed here, so there is no way to
steer it.

**So: read the veneer address to learn which register the original used, then
call that veneer directly with the target in the matching argument position.**
The `rN` is a fact about the reference's allocation, and the direct call is how
you reproduce it.

## Addendum (2026-07-26): a score that does not move is not proof of no effect

`alchemy-gcc b726765` adds `-fthumb-group-value2-in-place`, clearing the
long-recorded residual on `080b5ad4` and the other grouped-DMA regions whose
third descriptor word also feeds a following call. `thumb_store_multiple3`
hard-codes r0/r1/r2, so `arm_pre_reload` copies the value into r2; when that
value has a second use as a call argument the allocator homes it elsewhere and
the copy survives. `value0` already had a special case for this shape; `value2`
had none. That region's descriptor and constant pool are now byte-exact — 28
differing halfwords over 68 bytes down to 11 over 64.

**The first version of the transform was wrong in a way the score could not
show.** Retargeting `value2`'s definition to r2 without moving its *other* use
left that use reading a pseudo with no definition; the allocator handed it a
stale r4 and the call took garbage as its third argument. The differing-halfword
count did not change at all, because the wrong register shifted no offsets. Only
diffing the generated assembly revealed it.

So a compiler change that leaves the score unchanged has not necessarily done
nothing — it may have done something wrong. **Diff the assembly, both when a
change appears to do nothing and when it appears to work.** That is the third
time in one session that a score alone pointed somewhere wrong: it also produced
a fabricated "GCC folds the extract" mechanism, and an inflated family count
twice over.

Note the fork is ahead of the staged `dist/` bundle by this option. Nothing is
routed against it yet, and it is default-off, so the build is unaffected;
restage before routing any source to it.

## Addendum (2026-07-26): a size constant can need the `Value_` treatment too

The related-constants law is written up for *addresses* that GCC derives from one
another. It applies just as much to a plain integer that the reference loads from
the pool. On `0800d304` the allocation size `0x4e8`:

```c
s32 size = 0x4e8;                        /* movs #157 / lsls #3, no pool word */
s32 size = (s32)&Value_000004e8;         /* ldr r5, <pool>, as the reference */
```

Written as a literal, GCC materialises it and the region comes out **four bytes
short** because the pool word disappears. As a linked symbol it is a pool load.
Worth 22 halfwords there, from 28 to 6, and it is the first thing to check when a
region is a few bytes *under* the reference.

Also from that region, and it contradicts `080ae9f0`: **declaration order inside
a descriptor block is sometimes load-bearing.** On `080ae9f0` all five
permutations of the descriptor locals scored identically. On `0800d304`,
declaring `words` and `control` *before* the three descriptor words is required —
it matches the reference's shift-then-load order and no flag recovers it
afterwards. So do not conclude from one region that ordering is inert; check it
when a block's instructions are right but their order is not.

And a limit on the veneer law above: **the direct-call spelling only pins r0-r3.**
`0800d304` calls `_call_via_r6`, and no argument position reaches r6, so the
function-pointer form is the only option there. It happens to work — GCC puts the
buffer in r6 by itself, because it is the value that must survive the DMA.

## Addendum (2026-07-26): the parameter-save hoist, and a call spelled honestly

`08019bac` converts. It had been parked for a session at 3 halfwords with its
residual written up as needing "a third hook … one shape for one region", and
deliberately left alone. Two things closed it.

**The flag combination found on `0800d304` transfers.** `-mgrouped-dma-store
-fthumb-move-before-alu -fno-thumb-contiguous-immediate -fno-sched-depend-count
-mhigh-register-move-first` took it from 10 halfwords to 3 with no new work. When
a region is close, try the full set another member of its family needed before
concluding anything.

**`-fthumb-hoist-parameter-save`** (alchemy-gcc) closes the last 3.
`thumb_order_high_register_move` only swaps a save with an immediately preceding
constant setup; here `mov sl, r1` had to travel past a pool load *and* the copy
consuming it. The new option walks such a save back over insns touching neither
of its registers, stopping at another parameter save — which keeps the saves in
parameter order — and at anything reading or writing either register, which is
also what keeps the prologue's `mov rN, sl` between the save and the caller's
value.

**Its source was also semantically wrong and nobody had noticed**, because the
bytes were close. It called `Func_080072fc(first, second)` — a two-argument
direct call to `_call_via_r6`, which sets no target at all; the region only
worked because GCC happened to leave the buffer in r6. Written honestly as
`((CopiedCode)buffer)(first, second)` it scores identically and actually means
what it does. A byte score cannot tell you a call has no target: read the veneer.

## Addendum (2026-07-26): the VLA family is a different residual, measured

After `08019bac` converted, the same six-flag set was applied to the VLA-plus-DMA
family it sits next to. It helps a little and converts none of them:

| region | `-mgrouped-dma-store` alone | with the full set |
| --- | --- | --- |
| `08005340` | 19 | 15 |
| `08005534` | 18 | 15 |
| `080052f4` | 16 | 15 |
| `0800543c` | 21 | 21 |
| `080b90ac` | 24 | 25 |

All size-exact. Their residual is **not** the one today's options address. It is
the placement of the stack-pointer update against the descriptor store:

    candidate   stmia r4!, {r0,r1,r2} / subs r4,#12 / mov sp, r1
    reference   mov sp, r1 / ldr r0 / orrs r2, r4 / stmia r3!, {r0,r1,r2}

The reference commits the VLA allocation *before* programming the descriptor;
ours sinks it past the transfer. That also swaps which of r3/r4 holds the DMA
base and which the control constant. `-mearly-frame-allocation` does not reach
it, and neither does reordering the block's declarations — unlike `0800d304`,
where reordering was decisive. Splitting `control = 0x84000000 | words` into a
compound assignment makes these *worse* (68B against 76B), the opposite of its
effect elsewhere.

So: the two DMA families look alike and are not. The wrapper family
(allocate / transfer / call / free) is closed.

**And the VLA family is NOT a well-specified compiler task, though it looked
like one.** Hand-verified both ways on `080052f4`:

- Reordering the whole block to the reference's schedule — about fifteen insns,
  including reassigning which of r3/r4 holds the DMA base and which the control
  constant — relinks **EXACT**. So the semantics are right and the C is done.
- Hoisting *only* `mov sp, r1` above the descriptor store, changing nothing
  else, leaves it at **15 differing halfwords — exactly where it started.**

The stack-pointer placement is therefore a symptom, not the cause. There is no
single behaviour to hook here; the reference simply scheduled the block
differently throughout. That is a far worse prospect than the single-insn
rotations that converted `08005a78`, `0800d304` and `08019bac`, and anyone
picking this up on the strength of "the sp update is in the wrong place" will
lose the time.

The check that separates the two cases costs one minute: make the *minimal*
edit, not the full one. If the minimal edit does not move the count, the residual
is a schedule, not a rule.

## Addendum (2026-07-26): sweep every parked candidate when a flag set lands

After the six-flag set converted `08019bac`, it was applied to **all 134 parked
candidates at once** rather than to the family it came from. That is a cheap
sweep — a few minutes — and it reranks the whole board:

| region | halfwords under the set |
| --- | --- |
| `08093054` | 2 → **converted** |
| `0800430c` | 3 |
| `08096c80` | 3 |
| `0800307c` | 5 |
| `08006408` | 7 |
| `080f7f30` | 7 |
| `08021be0` | 8 |

`08093054` closed with `-fthumb-entry-saves-descending`: ours always saves
parameters in order, r0's home then r1's, and some references do the opposite.
Two adjacent independent copies, and an earlier session had recorded 74 mode
combinations all stuck at the same 2.

**Two cautions from the sweep itself.** `timeout` is not a macOS command — a loop
using it fails silently and reports nothing, which looked exactly like "no
candidate is close". And `candidate_show` applies each source's *routed* flags on
top of `--flags`, while `work/fresh_check.ts` applies only what you pass; on
`0800430c` that is the difference between 3 halfwords and 14. Compare like with
like before concluding anything about a region.

`0800430c` is at 3 and hand-verified: sinking `movs r0, #1` past two independent
insns relinks EXACT. `0800307c` and `08006408` show the same shape — a constant
setup wanting to issue *later*, the mirror of `thumb_order_entry_literal`. No
option was written for it, because the reference's stopping point does not follow
an obvious rule: on `0800430c` the constant sinks exactly two positions and stops
before a `cmp` it could equally have passed. That wants tracing, not guessing.

## Addendum (2026-07-26): when the reference derives a constant, write the arithmetic

`08096c80` clears bits with `subs r3, #17`, deriving the mask from a `4` it has
live for two nearby stores (`4 - 17 = -13 = ~12`). Ours materialised
`movs r3, #243`. An earlier session tried `&= ~12`, `&= -13`, an `s32` widen and
an arithmetic form, and recorded that the derivation was "cse's related-constant
decision and not reachable from the source".

It is reachable. Write the mask **in terms of the live variable**:

```c
u8 flag = 4;
object[85] = flag;
object[35] = flag;
child[9] &= ~(flag + 8);      /* derives; `~12' and `flag - 17' do not */
```

Same value, but the dependency is explicit, so GCC derives rather than
materialising. Note the exact form matters: `flag - 17` scores the same as `~12`.
Only keeping the `~` outside reaches it.

This is the general lever for the related-constants family, and it is the mirror
of the `Value_<addr>` trick: when the reference *loads* two related constants
separately, break the relationship with distinct symbols; when it *derives* one
from another, name the source value and write the arithmetic.

## Addendum (2026-07-26): the IME-disable idiom, and check the tree before concluding

Disabling interrupts around a critical section, the references store the IME
register's **own address** into it:

    ldr  r0, [pc, #40]    @ 0x04000208
    ldrh r4, [r0, #0]     @ save
    strh r0, [r0, #0]     @ disable: writes 0x0208, bit 0 clear

Only bit 0 of IME is meaningful, so any even value disables. Write it literally:

```c
volatile u16 *interrupt_master = (volatile u16 *)0x04000208;
u32 saved = *interrupt_master;
*interrupt_master = (u16)(u32)interrupt_master;   /* not `= 0' */
...
*interrupt_master = saved;
```

`= 0` uses a zero register and `= 0x0208` pools the constant separately; neither
matches.

**And the process point, which cost more than the idiom is worth.** Earlier today
I tested those two spellings on `0800651c`, failed, and wrote up "a cse/combine
difference in the reference compiler, not a source shape — not worth a mode".
The correct spelling was already sitting in `work/hand/0800430c/0800430c.c`,
written by an earlier session. **Before concluding that no source shape exists,
grep the existing candidates for the construct.** Two failed guesses is not a
search.

## Addendum (2026-07-26): recover the caller-visible signature before tuning allocation

`08096c80` was parked at three differing halfwords after its mask arithmetic was
already exact. The draft declared both the function and its first callee with no
arguments:

```c
u8 *Func_080090c8();
u8 *Func_08096c80(void)
{
    return Func_080090c8();
}
```

That was not a harmless prototype omission. Multiple existing callers pass four
arguments to `Func_08096c80`, and the reference forwards the untouched `r0-r3`
set into `Func_080090c8`. Writing the interface honestly:

```c
u8 *Func_080090c8(s32, s32, s32, s32);
u8 *Func_08096c80(s32 kind, s32 x, s32 y, s32 z)
{
    return Func_080090c8(kind, x, y, z);
}
```

keeps all four argument registers live to the call. That naturally moves the
nearby pool value into r4 and converts the complete 92-byte region with no
compiler option.

**Caller arity is therefore allocation evidence, not cleanup to postpone.**
Before permuting locals in a near candidate, grep its callers and check which
incoming registers the reference reads or forwards before overwriting them. An
empty parameter list in old C suppresses type checking and can still produce
plausible code, but it also shortens live ranges that decide every later
register. Recover the signature first; tune the source only after it agrees with
the observed ABI.

## Addendum (2026-07-27): preserve a shared zero before its stack store

`08095290` exposed one more strict post-reload scheduling fingerprint. Its
natural source uses the same zero both to initialize a stack word for a DMA
source and to initialize state kept in a saved high register. With the existing
grouped-DMA route enabled and the new mode disabled, the compiler emits:

```asm
mov  r0, sp
movs r3, #0
str  r3, [r0]
mov  r9, r3
```

while the reference preserves the zero in r9 before storing it:

```asm
mov  r0, sp
movs r3, #0
mov  r9, r3
str  r3, [r0]
```

`-fthumb-high-move-before-stack-store` restores that order. It is not a general
scheduler preference: the transform requires the complete four-insn
fingerprint, SI-mode hard registers, a literal zero, a non-volatile stack
address, a low source register, a saved high destination, independence, and the
source register's death note on the high move. The death note moves to the
store, its actual final use. The mode is default-off, is routed only to GS1
`08095290`, and has explicit opt-out, exact-order, non-stack control,
neighboring-source, direct-compiler, and GS2 exclusion coverage.

The source law is simpler: when one logical zero initializes both persistent
state and a stack-backed DMA source, keep it as one ordinary scalar value.
Do not split it, add volatility, or force registers to manufacture the
reference order. First prove the four-instruction reorder relinks exactly; only
then admit the narrow compiler fingerprint.

## Addendum (2026-07-27): an indirect kernel's return register is ABI evidence

`080d40ec` calls the relocated word-copy kernel at `0x03001388`. Declaring its
function pointer as returning `void` leaves the generated region the right size
but transposes the final control load and destination setup. The relocated ARM
kernel advances r0 while copying and returns through `bx lr` without replacing
it, so its observable return value is the end destination pointer. Declaring
the call honestly as:

```c
typedef void *(*WordCopy)(void *destination, const void *source, s32 size);
```

restores the exact tail under the otherwise sufficient `-fno-gcse` route.
Return types on indirect relocated code are therefore not cosmetic allocation
hints. Disassemble the target and record what it actually leaves in r0 before
tuning a caller; a `void` guess can shorten a live range even when the caller
does not consume the value.

## Addendum (2026-07-27): callback pointers outrank apparent gap continuity

`080944ec` was initially packaged as an executable continuation after
`08094428`. The boundary evidence says otherwise. `08094428` returns at
`0x080944e0`; its alignment and literal pool occupy the remaining bytes through
`0x080944eb`, so execution cannot fall through into `0x080944ec`. Separately,
the odd Thumb pointer `0x080944ed` appears at `0x080947e0` when `08094730`
registers the callback and again at `0x0809480c` when `080947e4` unregisters
it. Those are independent entry references to an 88-byte function ending at
`0x08094544`.

The callback does use r4 without preserving it, but that is not structural
evidence: the GS1 compiler contract globally includes `-fcall-used-r4`, and an
ordinary C reconstruction produces the same ABI behavior. The region therefore
belongs in `compiler_output`, not `executable_gap_continuation`.

When a gap-shaped region has both a clean predecessor return/pool boundary and
an odd-address callback reference, classify the callback entry independently.
Adjacency alone must not hide ordinary decompilation debt or shrink the ledger
denominator.

## Addendum (2026-07-27): a call-clobbered register is not an argument register

`080c0be4` reads `r4` before any instruction defines it and uses that value as
the first input to `Func_080022ec`. Its same-sized sibling `080c0cec` receives
the corresponding fourth argument conventionally in `r3`. This is not an
effect of the project-wide `-fcall-used-r4` option: that option lets generated
code clobber `r4`, but it does not change the ordinary Thumb parameter
locations from `r0`–`r3` plus the stack.

No direct branch or tagged pointer to `080c0be4` was found, while `080c0cec`
has both direct callers and a tagged veneer. A computed caller cannot be ruled
out, but it would only confirm the hidden context contract; ordinary C still
cannot name the incoming `r4` value. `080c0be4` therefore belongs in
`hidden_register_context_module`, not in the C denominator.

Screening only for a callee-saved register that is never written is too weak.
The decisive test is whether it is read before the function gives it a value.
Compiler ABI flags must then be interpreted separately as allocation/clobber
rules and argument-passing rules.

## Addendum (2026-07-27): preserve conservative record aliasing when the ROM does

The natural typed-structure reconstruction of `08017004` is semantics-complete
and exact-size, but differs at 15 halfwords because the compiler moves local
result stores ahead of later window-record loads. Naming the same naturally
aligned halfword fields through one `FIELD(base, type, offset)` record view
keeps their common byte-address provenance visible and restores the reference
schedule without volatility, barriers, register pins, or undefined accesses.

This is not a license to replace every recovered structure with raw offsets.
Use the typed form first. If it is exact-size and the entire residual is
load/store motion across calls, compare the reference's alias model: an
original generic record or union view may have been deliberately conservative.
Named accessors can retain readable field semantics while expressing that
evidence honestly.

## Addendum (2026-07-27): stock-compiler routes must be verified through the real pipeline

`080fa6a0` is byte-exact only under the already-approved stock `old_agbcc -O2`
audio-library route; the Camelot fork remains 228 bytes away and no supported
single mode closes it. The independently recovered state is exactly `0xfb0`
bytes: a `0x50`-byte header, twelve `0x40`-byte channel records, and two
`0x630`-byte PCM buffers.

The repository invokes `old_agbcc` directly for this cohort rather than
running the normal preprocessor first. A scratch source containing
`#include`/`#define` directives was exact only after manual preprocessing and
failed through `candidate_show`. The integrated translation unit therefore
uses local fixed-width typedefs and expanded MMIO expressions, and its exact
proof is through the same source route used by the claimed build.

The callback slots also need an honest placeholder representation. Local GS
call sites prove that two slots eventually hold one-argument and three-word
callbacks, while initialization writes the same no-argument placeholder to
all four slots. Small unions with a `placeholder` member record that state
without incompatible function-pointer casts or borrowed external labels.
When a compiler cohort bypasses a build stage, always promote the source only
after re-running the repository's real route; a manually preprocessed scratch
proof is not yet an integration proof.
