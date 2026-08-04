> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

# alchemy-gcc prototype results (2026-07-26)

## Stock-option matrix completion (2026-07-28)

The explorer now includes four previously omitted stock configurations:
`-fno-rerun-loop-opt`, `-fno-caller-saves`, `-fno-force-mem`, and `-Os`.
They are derived from the optimization switches exposed by the checked-in
public GCC sources; in particular, `gcc-2.96/gcc/toplev.c` enables rerunning
the loop optimizer, caller saves, and force-mem at `-O2`.

An exhaustive common matrix of 1,211 compatible pairs was run across all 22
current semantic candidates. It found no shared exact configuration and no
irreducible configuration that improves multiple candidates without regressing
another. The useful single-mode effects were:

| configuration         | improved candidates | halfwords removed | regressed candidates |
| --------------------- | ------------------: | ----------------: | -------------------: |
| `-fno-rerun-loop-opt` |                   1 |                40 |                    0 |
| `-fno-caller-saves`   |                   2 |                 9 |                    3 |
| `-Os`                 |                   2 |                36 |                   11 |
| `-fno-force-mem`      |                   1 |                 3 |                    3 |

The first mode moves `0807a0f4` from 68 to 28 differing halfwords, but does not
make it exact. The second improves both `080113e4` and `080114a0`, so it remains
a useful source-scoped compiler hypothesis even though it is not a safe global
default.

Full 1,366-region exact-C corpus runs confirm that none is globally safe:
`-fno-rerun-loop-opt` preserves 1,337 regions, `-fno-caller-saves` 1,331,
`-Os` 1,068, and `-fno-force-mem` 1,178. These are explorer options, not global
build flags.

`mode_cohort.ts` now refuses a truncated pair count. Previously, each member
ranked its own top 256 pairs, so a cohort could compare different configuration
sets and silently suppress a shared result. It also reports every single
mode's gains and regressions, plus irreducible multi-region improvements, rather
than requiring a mode to be non-regressing across the entire cohort before it
is visible.

The historical-family search also has a deterministic factorial phase now.
It tests all 227 compatible combinations of `old-agbcc`, `pret-early-thumb`, or
GCC 2.95.1 with two stock pass switches on every cohort member. The first full
run compiled that same matrix for all 22 candidates. It found no exact result
and no three-mode configuration that improved even one candidate beyond every
proper family/single-switch subset. This closes the previous blind spot where
such configurations were considered only when a candidate-specific pair had
already reached an exact-sized 2–5-halfword floor.

## Stock O-level catalog closed (2026-07-28)

The remaining code-generation pass switches enabled automatically by the
checked-in GCC sources are now represented:

- common families: `-fno-defer-pop`, `-fno-thread-jumps`,
  `-fno-omit-frame-pointer`, `-fno-cse-skip-blocks`, and
  `-fno-inline-functions`;
- GCC 2.96 only: `-fno-peephole2` and
  `-fno-delete-null-pointer-checks`.

The family restrictions follow the option tables in the public source trees;
GCC 2.95.1 and `pret-early-thumb` reject the two GCC 2.96-only switches.
`-fno-strict-aliasing` is intentionally not an explorer mode: although `-O2`
enables strict aliasing, changing it changes the language aliasing contract
rather than isolating a code-generation pass.

The finalized 22-candidate run tested the same 1,572 compatible pairs and 372
historical-family factorial configurations for every member. It produced zero
new exact results, zero globally non-regressing shared improvements, and zero
irreducible historical-family factorial improvements. Of the newly added
singles, only keeping the frame pointer improved anything (one candidate by six
halfwords), while regressing 20 of 22 candidates by 950 halfwords in aggregate.
The other new passes were inert or regressive.

Full exact-C corpus checks reinforce that result. `-fno-defer-pop` and
`-fno-inline-functions` are inert at the routed `-O2` baseline (1,366/1,366
preserved); `-fno-thread-jumps` preserves 1,363, `-fno-cse-skip-blocks` 1,321,
and `-fno-peephole2` 1,318. Keeping frame pointers changes all 1,366 exact
regions. The GCC 2.96-only switches are deliberately unavailable to explicitly
selected older families; a forced routed-corpus diagnostic also confirms that
they cannot be applied to sources whose normal route is `old-agbcc`.

This satisfies the compiler stopping rule: the coherent historical families,
their source-derived O-level pass toggles, all common pairs, and the structured
alternative-family factorial have been exhausted without a shared exact
configuration. Further broad compiler-option work is frozen unless a fresh
decompilation residual supplies new cross-region evidence.

> Historical prototype log. The kept changes described here were subsequently
> committed, staged into the pinned compiler bundles, regression-tested, and
> pushed in `alchemy-gcc`. Statements below about uncommitted patches or an
> untouched `dist/` describe the time of measurement, not the current state.

Prototyped in `alchemy-gcc/build-296/`, which is a separate tree from the
digest-pinned `dist/`. **`dist/` was never touched**, so every alchemy build in
this repo still uses the approved bundle. Before changing anything, the
prototype toolchain was verified to reproduce `dist` byte-for-byte on a known
source.

Harness (all git-ignored, under `work/`):
`proto_probe.ts` compiles with an arbitrary toolchain prefix, `proto_score.ts`
scores a candidate against its ROM region with one, `proto_regression.ts`
compiles all of `src/` with both and reports any divergence.

Current diff is saved at `work/proto-arm-2026-07-26.patch`.

## Patch 1 — `value1` may be a hard register. KEPT.

`arm_pre_reload` rejected a descriptor whose middle word is not a pseudo
(`REGNO (value1) < FIRST_PSEUDO_REGISTER`, arm.c). A VLA's address is `(reg sp)`
after CSE, so the honest source for the alloca descriptor families could never
group.

- **Regression: clean.** All 1,236 converted sources that route through `xgcc`
  compile byte-identically under both compilers. (The other 6 route through
  `old_agbcc` and are not affected by this file.)
- **It does what it claims.** The honest source for 0800543c
  (`work/hand/0800543c/natural.c`) now emits `stmia r4!, {r0, r1, r2}` /
  `sub r4, r4, #12`. Under stock it emits three separate stores. 70 mismatched
  bytes to 52.
- **It converts nothing on its own.** 52 is still not exact, and it is worse
  than the contrived `(u32)&buffer[words] - (words << 2)` shape, which scores
  31 under stock. It does not move 080054e4 or 08005534 at all (74 and 36 under
  both).

**The previously relayed "up to 11 regions" payoff is not supported.** The
diagnosis of the guard was exactly right and is now
confirmed by measurement, but removing it is necessary, not sufficient: the
second defect on these families — the reference spills its arguments to r5/r6/ip
and reuses the computed stack pointer as the descriptor destination, where we
re-read `sp` — still stands. See `work/hand/080054e4/NOTES.md`.

## Patch 2b — accept volatile stores AND carry the volatility. WORKS. KEPT.

The right version of patch 2. `thumb_scalar_word_store` now accepts a volatile
store, `arm_pre_reload` requires all three stores to agree on volatility (a
group mixing the two would misstate what the hardware sees), and after emitting
`thumb_store_multiple3` it sets `MEM_VOLATILE_P` on each of the group's mems.
That last step is what patch 2 was missing: without it the group is an ordinary
store and a later pass deletes it.

- **Regression: clean.** All 1,238 `xgcc`-routed converted sources byte-identical.
- **The class went from unreachable to reachable.** Both multi-descriptor
  regions tested now keep every descriptor and group every one of them:

  | region   | descriptors | stock                                      | patched                                       |
  | -------- | ----------- | ------------------------------------------ | --------------------------------------------- |
  | 080f377c | 3           | 0 grouped, first descriptor deleted, 104 B | **3 grouped, all present, 116 B against 112** |
  | 0800300c | 2           | 1 descriptor deleted, 108 B                | **2 grouped, all present, 120 B against 112** |

  Getting there also needed the locals-first law applied per descriptor: an
  address computed inline sits between two stores and breaks
  `store1 == next_nonnote_insn (store0)`, which is why only two of 080f377c's
  three grouped at first.

- **Neither converts yet.** 080f377c is 4 bytes over, and the excess is one
  `push {r5}` / `pop` pair: the control word 0x84000080 is shared by two
  descriptors, cse holds it in a callee-saved register, and the reference
  reloads it from the same pool word twice. That is the rematerialisation
  tie-break already recorded in `work/hand/080058ac/NOTES.md`, not a structural
  problem.

Candidates saved at `work/hand/080f377c/080f377c-volatile-proto.c` and
`work/hand/0800300c/0800300c.c`. They only make sense against a compiler with
this patch.

## Patch 2 (first attempt) — accept volatile stores, no propagation. UNSOUND.

This was the 42-region item: `thumb_scalar_word_store` returns 0 on
`MEM_VOLATILE_P`, so a volatile descriptor pointer never groups, while a
non-volatile one loses every descriptor but the last to dead-store elimination.

Dropping the test does make the transform fire. It is still wrong.
`thumb_store_multiple3` carries no volatility of its own, so the group it
produces is an ordinary store that a later pass is free to delete. On 080f377c
the **first of three descriptors disappeared outright**, the region got shorter
(112 B to 104 B) and the score went from 54 mismatched bytes to 92.

Reverted, with that measurement recorded as a comment at the guard so the next
person does not repeat it. **Accepting volatile stores requires the grouped
pattern to carry the volatility first** — an unspec_volatile form of
`thumb_store_multiple3`, or setting `MEM_VOLATILE_P` on the pattern's mems when
it is generated from volatile sources. That is a real change to arm.md, not a
guard removal, and it is the one worth doing next: it is the only route to the
42 multi-descriptor regions, and there is no source-level workaround (both
candidates were tested and ruled out in `work/hand/080f377c/NOTES.md`).

## Patch 3 — relax the `regs_ever_live` guards. TRIED, POINTLESS, REVERTED.

This was the third blocker: `thumb_order_grouped_dma_store` requires r0 and r4
unused _anywhere in the function_, which was thought to confine the twelve-store
`stmia r0!, {r1, r2, r3, r4}` path to argument-free leaves like `src/08004a94.c`.

Dropping both tests changes nothing. 08004bd4 stays at 52 mismatched bytes and
the group still does not form. **That diagnosis was wrong.** The scan breaks at
`index != 12` long before those guards are reached, because dead store
elimination has already removed the descriptor slots a later statement
overwrites, leaving nine stores where the recognizer needs twelve. The
`regs_ever_live` guards were never the binding constraint; the claim came from
reading the precondition list rather than measuring which one fired.

Reverted, with the measurement recorded at the guard.

## Scoreboard for the four published blockers

Two of the four did not survive measurement with the runnable compiler:

| #   | claim                                      | verdict                                                                                                  |
| --- | ------------------------------------------ | -------------------------------------------------------------------------------------------------------- |
| 1   | volatile blocks grouping, 42 regions       | **real and now fixed** (patch 2b): the grouped pattern carries volatility, descriptors survive and group |
| 2   | `value2` copy from hard-coded `(reg:SI 2)` | untested here; still stands on inspection only                                                           |
| 3   | whole-function `regs_ever_live` guard      | **wrong** — not the binding condition, DSE is                                                            |
| 5   | `value1` must be a pseudo                  | real and fixed, but converts nothing alone                                                               |

Blocker 4 (loop-invariant hoisting) was already withdrawn yesterday as
source-reachable. So of five published items, two are confirmed and fixed, one is
wrong, one was withdrawn, and one is still unmeasured. The pattern is that reading a precondition list and
naming the guard that _looks_ responsible is not evidence; only running the
patched compiler is.

## State of alchemy-gcc

Patches 1 and 2b are **applied and uncommitted** in the working tree; patch 3 is
reverted to stock plus its measurement comment. `build-296/gcc/cc1` is built from that state.
`dist/` is untouched and the alchemy build is unaffected either way.

## Where the scheduler tuning actually sits (measured on 080a1090)

080a1090 is 4 bytes from converting and the entire residual is one transposition:

```
reference   mov r1,#137 / mov r3,#255 / lsl r1,r1,#1 / strb r3,[r4,#28]
ours        mov r1,#137 / lsl r1,r1,#1 / mov r3,#255 / strb r3,[r4,#28]
```

The reference slots an independent instruction between the two halves of a
split constant, which is exactly what `-mthumb-immediate-latency` was written
for, and its implementation (`arm_adjust_cost`, +1 on a true dependence whose
producer is `(set (reg) (const_int))`) does match this dependence.

**It fires and still does not help.** With the mode:

```
ours+mode   mov r3,#255 / mov r1,#137 / strb r3,[r4,#28] / lsl r1,r1,#1
```

A gap does open between the constant's halves, but the scheduler fills it with
the _store_ rather than with the other constant, and it also hoists
`mov r3,#255` to the front. 4 mismatched bytes without the mode, 16 with it.

So the remaining lever is not cost, it is **ready-list priority**: which of
several ready insns is chosen to fill the gap. That is `arm_adjust_priority` /
`rank_for_schedule`, not `arm_adjust_cost`. A mode that prefers a constant
materialisation over a memory store when both are ready would close 080a1090,
and 080a47b4 has the same shape (`movs r1,#0` against `adds r5,r0,#0`, plus two
transposed `lsls`). That is the next compiler experiment, and it is a real
tuning loop rather than a one-line guard — worth doing with the harness, not by
inspection.

## Hooks landed, and one that was measured and rejected (2026-07-26)

Two reordering hooks earned their place, each on three regions sharing one
residual:

- `thumb_order_high_register_move` widened to treat a **minipool load** as a
  constant materialisation, not just a `CONST_INT`. A wide or relocatable
  constant reaches its register as a load, and the hook had been ignoring
  exactly the shape it was written for.
- `-fthumb-move-before-alu`, new: put an independent low-register copy ahead of
  an adjacent two-address ALU insn. Both operands of the ALU insn must be
  registers, the copy must be low-to-low, and independence is checked in both
  directions so neither a value nor the flags can change hands.

Together with `-fno-sched-depend-count` and a source reorder these converted
`08002fb0` and `08003e10`.

**Rejected: extending `-fthumb-move-before-alu` to also overtake a constant
materialisation.** It looks like the missing cell of an obvious matrix — the
high-register hook does _constant before high move_, the new hook does _ALU
before low move_, so _constant before low move_ should complete it. Measured
across the six closest parked regions it is a net loss: `08019bac` 6 to 10
bytes, `080b0744` 10 to 18, others unchanged or marginal. Reverted.

The general rule confirmed by these experiments: a hook needs several regions
sharing a residual _and_ a measurement across the parked set before it lands.
Symmetry is not evidence. Seven modes proposed by inspection were
wrong; the three that worked came from `-da` dumps and gating passes one at a
time.

## QUEUED ITEM — per-call-site granularity for `-mcall-arg0-move-first` (2026-08-01)

**Status: specified, with two reproducers and a readable rule. Implementable
from this section without re-deriving anything.**

### What the mode does today

`-mcall-arg0-move-first` (the only member of its family in the fork —
confirmed by `strings dist/cc1 | grep -oE '^call-[a-z0-9-]+$'`) puts the
argument-0 register setter ahead of its immediate companions at a call.
It is **whole-function**: routed per source in
`CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES`, applied to every call site in the
translation unit.

### The reference's own rule

Read off two code-overlay rows in different code overlays. Within one arm of a
dispatcher, the reference orders a two-argument call's register setters as:

- the **last** call of the arm sets **r0 first**;
- **every earlier** call of the arm sets **r1 first**.

Where an arm falls through into the next case, its last call is the
_one-argument_ call, so the last **two**-argument call of that arm is NOT
last and stays r1-first.

This fork gets the general case right and mis-handles exactly the ends: it
promotes the last two-argument call of a fallthrough arm as though it were
the arm's last call, and (in the other direction) declines to promote where
the reference does.

### Reproducer 1 — `resource_3c4:0cd0`, wants arg0-first at 2 of 4 sites

Reproducer: `resource_3c4:0cd0` with a 216-byte draft.

```
bun tools/overlay_adopt.ts resource_3c4:0cd0 --source <src> --where
  -> adopt=rejected differing_bytes=4
```

The reference's final basic block holds four structurally identical
two-argument calls and uses **both** orderings: arg0-first at 0x02000d50 and
0x02000d92, arg1-first at 0x02000d3c and 0x02000d9a. Turning the mode on
fixes 0x02000d92 and breaks the other two — 4 bytes becomes worse.

### Reproducer 2 — `resource_396:1424`, wants arg1-first at 2 of ~30 sites

Reproducer: `resource_396:1424`, using `--span 392`.

```
bun tools/overlay_adopt.ts resource_396:1424 --source <src> --span 392 --where
  -> adopt=rejected differing_bytes=6
  -> differing_at 0x20014da+4 0x200157d+1 0x200157f+1
```

Both differing sites are the **fourth two-argument call of an eight-call arm
that falls through** (case 4 into case 1 at 0x020014e8; case 11 into case 8
at 0x0200158a). The three earlier two-argument calls in each of those arms
come out right, and so does every other site in the function.

### What was measured and does NOT reach it

Neither row moves under: `-fno-schedule-insns2`, `-fschedule-insns`,
`-fno-schedule-insns`, `-fsched-low-dest-first`, `-fsched-high-dest-first`,
`-fno-sched-depend-count`, `-fsched-store-first`, `-fno-sched-alias`,
`-fno-cse-two-insn-immediate`, `-fno-cse-pool-immediate`,
`-fno-gcse-insert-load`, `-fno-regmove`, `-fno-expensive-optimizations`,
`-mthumb-immediate-latency`, `-mhigh-register-move-first`,
`-fthumb-move-before-alu`, `-fthumb-orr-dead-input-reuse`,
`-fno-canonicalize-comparison`, `-fno-thumb-contiguous-immediate`, or
`-mcall-arg0-move-first` itself. **The order does not change under any
scheduler flag, so it is emission order, decided before scheduling** — the
fix belongs where the call's argument setters are emitted, not in a
scheduling hook.

Source spellings tried and rejected across the two rows: pointer aliases,
`void *` casts, comma expressions, unsigned literals, shared and per-site
constant locals, a dummy result local, argument locals on either operand,
argument locals for the preceding call, an empty statement at the case
label, removing the fallthrough comment, and three respellings of earlier
statements in the same block to shift insn uids. The only spelling that
flips the order — consuming the call's return value — overruns the span.

### Acceptance test

Both rows byte-exact: `3c4:0cd0` at 248 bytes and `396:1424` at 392 bytes,
each with no source change from the reproducer above. Per §7, a fork change
needs the rebuild, the re-pin, and the source-only build reproducing
gs1-en.gba at SHA-1 `5c4695205413df7db52b9a184815a07783999971` before the
re-pin is admissible — and then a routed set **and** a `tools/mode_sweep.ts`
entry.

**Do not land this as a wider whole-function mode.** Symmetry is not
evidence (see the rejected hook above), and the whole point of this item is
that the existing whole-function mode is what fails here.
