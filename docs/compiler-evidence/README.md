> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

# Compiler evidence

Findings that justify a change to the pinned gcc 2.96 Camelot fork, kept here
so the evidence survives the machine the probe ran on. Each entry records the
mechanism, a prototype diff against the fork source, and the collateral
measurement a routed mode needs.

The applied entry below is the first one admitted from this ledger. The
remaining entries are still experimental: landing one requires the fork change
to be committed to the `alchemy-gcc` repository and the pinned commit updated,
then the digests re-pinned in `tools/alchemy_gcc.ts` — and per `PROVENANCE.md`
that re-pin is admissible only after the source-only build reproduces gs1-en.gba
(SHA-1 `5c4695205413df7db52b9a184815a07783999971`) byte-identically.

## applied: grouped-DMA stack-zero order

The clean witness `08090824` is now exact C (104/104 bytes). Its reference
orders the zero load before the DMA base load after grouped-DMA formation. The
narrow `thumb_group_zero_before_base` post-reload transform in `alchemy-gcc`
recognizes the complete seven-instruction shape — registers, constants, modes,
and grouped consumer — and reorders only that shape. It is gated by the
existing `-fthumb-group-control-last` route, so it cannot affect unrelated
sources. The previously exact grouped-DMA owners `08005a78`, `08005c68`, and
`resource_3bd_c_02000c98` remained byte-identical in the compiler regression.

The darwin-arm64 `cc1` digest is pinned in `tools/alchemy_gcc.ts` as
`0767fccd6046d0b4dcaae1150a82e505a29e59ca9f4f979e2535e7970f3de449`. The
source-only and full builds both reproduce the ROM byte-for-byte, with no
fallback bytes and no forbidden inline assembly.

## applied: call argument before store

The clean witness `08077f70` is now exact C (284/284 bytes). Its reference
copies the first call argument into `r0` immediately before a preceding store,
where the installed compiler's post-reload Thumb scheduler leaves that copy
after the store. The experimental `-fthumb-call-arg0-before-store` mode adds a
strict, default-off post-reload recognizer for the single-store/single-`r0`
copy/call shape and moves only that copy. The source route also uses the paired
`-fno-sched-alias -fsched-store-first` controls; no inline assembly or
hard-register binding is involved.

The mode was tested across the same 0807/0809 near-match cohort and produced
no collateral changes when enabled by itself. The full source-only and ROM
builds remain byte-identical, with zero fallback bytes. The current bundled
darwin-arm64 `cc1` digest is
`0767fccd6046d0b4dcaae1150a82e505a29e59ca9f4f979e2535e7970f3de449`.

## applied: post-call byte-state increment register

The clean witness `08098b10` is now exact C (248/248 bytes). Its ordinary
GCC 2.96 output has the right control flow and size but keeps the temporary
state pointer in `r1`; the reference keeps that pointer in `r2` for the
post-helper byte increment. The default-off
`-fthumb-postcall-byte-increment-r2` mode recognizes only the complete
call/branch, `r1 <- r8` copy, byte load, `+1`, and byte store shape. It retargets
the hard register after reload by register number and carries the corresponding
reload notes with it. The route is limited to GS1 source `08098b10`.

The full exact-C corpus remains exact (1,424/1,424 members), and the complete
0809 semantic cohort shows no collateral changes. The source-only and full ROM
builds remain byte-identical with zero fallback bytes. The admitted
darwin-arm64 `cc1` is from alchemy-gcc commit `df79270`, digest
`d12bf2c7b96d2b1b6cec4c09b76f986249285070b1ca09d1ba1baf31b859cc18`.

## cse-two-insn-immediate

Blocks the largest single class of unconverted code-overlay functions: the reference
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

**Out of scope for this flag, and covered by its sibling below.** The
literal-pool sub-class is a different behaviour and this predicate excludes it.
Two claims made here originally were wrong and are corrected in the
`cse-pool-immediate` entry: sharing a pool load *is* a size change (it forces a
callee-saved register and a prologue change, 12-36 bytes), and the general
exclusion was mistaken. What survives is narrower: a function whose reference
mixes reloading with related-value sharing of pool words is unreachable by any
whole-function gate.

**Correction to the payoff figure.** The ~9,000-byte population quoted above is
the two-instruction class as originally surveyed, before the pool class was
separated out and before several members were converted by source respellings
instead. Treat it as a historical estimate, not a current backlog.

## cse-pool-immediate

The pool-word sibling, and a materially different defect. For a
two-instruction constant the cost model is correct and only the reference's
preference differs; for a pool constant `arm_rtx_costs` prices at
`COSTS_N_INSNS(3)` what `*thumb_movsi_insn` emits as a single
`ldr rN,[pc,#K]` — wrong by 3x — so the sharing is taken even more eagerly, with
no exceptions from cost. The mechanism is the same `cse_insn`
destination-recording loop; `-da` dumps show the rewrite appearing in pass 3
(`cse`), with `gcse` and `loop` inheriting it unchanged, so neither constant
propagation nor invariant hoisting is responsible.

`cse-pool-immediate.diff` turns `TWO_INSN_CONSTANT_P` into `CSE_CONSTANT_CLASS`,
returning 1 for the two-instruction class, 2 for the pool class, and 0 for a
one-word constant that `COST` already prefers over a register. The classes are
disjoint, each flag reads only its own, and both feed the same three decisions —
so this extends one predicate rather than adding a competing rule.

The destination-recording guard cannot be split: keeping the recording while
suppressing only the cost comparison, and passing `NULL_PTR` as `classp` so the
quantities are not merged, were both built and measured and have no effect.

**Measured.** Inert with the flag absent on 0 of 2,202 gcc296-routed sources;
110 of 2,202 change with it on, so per-source routing only — one currently exact
function regresses from 2 to 119 halfwords under it. Two functions became
byte-exact immediately (resource_39c:14cc at 156 bytes, resource_3bf:175c at 96)
and are adopted. Five more sit at 1-3 halfwords (resource_3bf:4bfc,
resource_394:08b0, resource_3b8:0264, resource_394:07e0, resource_3a4:0c9c —
1,060 bytes) with residuals that are either the known argument-order tie-break
class or draft data bugs, and resource_3af:1db0 (480 bytes) goes from 231 to 7.
The flag also makes emitted size exactly equal the reference span on every member
of the class, taking it off the critical path for a further band whose remaining
residuals are other blockers.

**Main-image witness.** `080108c4` independently reproduces a 32-byte executable-gap
continuation as canonical C with this same CSE switch, paired with the existing
source-scoped `-fno-sched-depend-count` route. It is now promoted to
`src/080108c4.c`; the executable-gap assembly and semantic draft were removed only
after the full source-only and ROM builds both passed byte-identically.

**Two negative results worth keeping.** resource_373:2cb0, the function that
motivated the investigation, is *not* unlocked and cannot be: its reference
reloads several pool words but keeps one in a register to derive a related value
by an add, and related-value reuse needs exactly the recording the flag
suppresses. resource_3af:0bb8 was misattributed to this class in its note — its
residual is one surplus pool word, a pool-emission issue, not CSE.
