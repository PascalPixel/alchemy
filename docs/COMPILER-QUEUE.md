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

## Main-image survey: where the open bytes actually are (2026-08-06)

Every one of the 695 open main-image owners was compiled from its unmodified
semantic source and diffed against its reference region. 581 compared cleanly;
114 threw `compiled function symbols differ`, which is a tool limit, not a
source problem — those regions place a static datum ahead of the entry, so the
`Func_<stem>` symbol does not sit at the region address and
`linkedFunctionExtent` rejects it. Comparing their linked bytes against the ROM
directly puts every one of them at roughly half the region differing, so the
limit is hiding nothing.

One owner was already byte-exact as written and has been promoted:
`080a7380`, 192 bytes, production routing, no source change.

The rest of the field is bimodal, and the shape matters for planning:

| band                            | owners | bytes   |
| ------------------------------- | -----: | ------: |
| exact as written                |      1 |     192 |
| 1–20 differing halfwords        |     50 |   3,748 |
| everything else                 |    643 | 383,652 |

The near-miss band is small. Closing all fifty owners to the last halfword
would add 3,748 bytes, about a tenth of the 37,300 that separates the tree from
a 25% byte share. The remaining 383,652 bytes sit at half-a-region differing:
those are behavioural reconstructions, and each needs ordinary per-owner work,
not a search.

### The near-miss band is one compiler class, not fifty source problems

The four largest near-misses were swept over the full explorer matrix —
78 single configurations each, and 334 including pairs for `0808fecc`. None
moved by a single halfword:

| owner      | bytes | floor | classification        |
| ---------- | ----: | ----: | --------------------- |
| `080a524c` |   316 |     4 | `register=4`          |
| `080b5d3c` |   214 |     4 | `order`               |
| `08092f84` |   188 |     7 | `register=7,literal`  |
| `0800fec8` |   140 |     2 | `register=3,literal`  |
| `0808fecc` |    48 |     2 | `order` (pairs too)   |
| `0800430c` |    76 |     3 | `order`               |

The residues are register-allocation and scheduling tie-breaks, and they are
invariant to source shape as well as to routing. `0808fecc` differs only in
whether `add sp, #4` precedes or follows the return-value move; `0800430c`
only in whether a `movs r0, #1` is scheduled before or after an unrelated
`ands`; `080a524c` differs in nothing but the choice of `r2` over `r3` for a
scratch, four times. Source spellings tried and rejected: reordering the
declarations, reordering independent statements inside the loop body,
splitting and merging the shift pair, returning through the structure field
rather than the local, hoisting and sinking the re-read of a volatile, and —
on `080a524c`, whose semantic source is already parameterised for this — all
seven spellings of `CHANGED_TYPE` from `u8` through `bool`.

So the band is a compiler-lane item, not a reconstruction item. It is worth
roughly 3,748 bytes if a mode covering the epilogue-ordering and scratch-choice
tie-breaks lands, and worth nothing until then. It should not be the next
thing anyone hand-shapes.

### The register-order lane is now swept, not guessed — and it is nearly spent

`-mlow-reg-order=` (fork commit `ac6a2b4`) takes four or eight digits and
overrides the leading entries of `REG_ALLOC_ORDER`: four digits set every basic
block, eight give the entry block its own order and the rest the second. All
576 entry/default pairs were swept against every main-image owner in the
1–24 halfword band.

The result: `080fa264` closes at `-mlow-reg-order=30120123` (with
`-fthumb-leaf-no-lr -fthumb-no-if-convert`), and nothing else in the band
closes. Order lowers residue on many owners — `080b0744` 15 → 9,
`080c1fa8` 15 → 10, `0801a4fc` 9 at best — but never to zero. Layering each of
the 33 fork modes on top of each owner's best order moved exactly one owner by
one halfword (`08011fd8` 10 → 9 under `-mthumb-load-latency-one`).

So the scratch-choice tie-break is real and now reachable, but the surviving
residues are not a single order: on `080a524c` the reference wants `r2` where
the port hands out `r3` in two mid-function blocks while still wanting `r3`
first elsewhere, which no whole-function permutation expresses. That is a
knock-on of an earlier allocation difference, not an ordering knob. The band is
worth less than it looked; the 383,652-byte bulk is where the byte share is.

Sweep hygiene, learned the hard way: `candidate_show.ts` keys its intermediates
by stem in a shared output directory, so two concurrent runs of the *same* stem
corrupt each other and report false zeros. Parallelise across stems, serially
within one.

## Negative: `-fglobal-copy-preference-first` (2026-08-07)

Residual dumps for the whole diff <= 15 band show the dominant remaining class
is register-*number* permutation, not instruction ordering: `08006408` (7),
`0801965c` (10), `08011fd8` (10), `080f4028` (14), `080c1fa8` (15),
`080ae9f0` (13). That is a different class from the three shipped fork modes,
which are all scheduling, so the obvious next lever was preferencing rather
than allocation order.

Two facts framed the attempt. First, `-mlow-reg-order=` already reaches *both*
allocators: `local-alloc.c:425` calls `ORDER_REGS_FOR_LOCAL_ALLOC_BLOCK (-1)`
at the end of local allocation, so the second permutation leaks into
`global_alloc`. Allocation order is therefore exhausted. Second, in
`global.c:find_reg` the copy preferences are applied only as a post-hoc
correction: the two-pass `reg_alloc_order` scan picks `best_reg` first, and only
then is the choice swapped for a copy-preferred register of the same class.
Pass 0 additionally refuses to allocate a register for the first time, so a
pseudo that is only ever copied out of another register tends to land elsewhere
and leave the copy behind.

The mode consulted `allocno[num].hard_reg_copy_preferences` *before* the scan,
single-register modes only. It is a no-op. Instrumented cc1 shows why: at
`find_reg` time the copy-preference set is empty for essentially every allocno
(one preference across all of `080b5d3c`, and it selected the register the
stock scan already picks). `regmove` has consumed the copies before
`global_alloc` runs, and `prune_preferences` clears what is left. The
preferencing lever does not live here; if it lives anywhere it is in
`local-alloc.c`'s `qty_phys_copy_sugg`, which already consults copy suggestions
first — which is itself evidence the class is not reachable this way.

Reverted; the fork is back at `40cdf47` and `dist/cc1` at the pinned
`41b5d62b...`. Do not re-derive this.

### Other measured negatives from the same pass

* `-fthumb-move-before-immediate-alu` spillover across the close pool: 26 owners
  improve, zero close, zero regress. Almost every win needs
  `-fno-schedule-insns2` alongside it (`08021be0` 20 -> 12, `08011568` 23 -> 17,
  `080049ac` 28 -> 24). Only `080170c4` 24 -> 23 and `080f0614` 31 -> 30 fire
  from the mode alone.
* `080a524c` (316 B, diff 4) is four halfwords of pure r2-vs-r3 in one block
  while a mirrored block wants r3 in both. Four source variants all measure 4.
* `08005534` and the DMA-veneer family (`080052f4`, `0800543c`, `08005490`,
  `08005584`, `0800562c`, `0800567c`, `08004144`; ~560 B): `-mgrouped-dma-store`
  fixes the size (72 -> 80) and halves the diff, but all 24 permutations of the
  four declarations floor at 18. The residual is the alloca save/restore trio
  sitting after the DMA stores in ours and before them in the reference; turning
  off both schedulers does not move it, so it is expand order, not scheduling.
* `08077394` (68 B, diff 11) is a pure r0<->r2 swap. All 576 `-mlow-reg-order=`
  combinations and 14 standard `-f` flags leave it at 11-12;
  `-fno-omit-frame-pointer` breaks the size.
* `080b5d3c` (214 B, diff 4) is a clean rotation of two independent copy pairs
  in an inner-loop preheader: ours issues `mov r6,r9; adds r5,r4; mov r7,sl;
  add r1,r8`, the reference issues the second pair first. `-fno-schedule-insns2`
  breaks the size, `-fthumb-move-before-alu` makes it worse (6), and no shipped
  mode goes below 4. Hoisting the totals pointer in source costs 4 bytes and
  88 halfwords.

Scale check for anyone planning off this file: the close pool (diff <= 40) is
112 files / 9,698 bytes, and diff <= 15 is 25 files / 1,864 bytes. Grinding the
entire close pool to zero still leaves ~25k bytes short of 25%. The 408 `asm/`
stems with no `semantic/` counterpart total 17,577 insns, and the 364 tiny ones
are linker veneers that cannot be written in C without `asm(...)`, which the
project forbids. The byte share is in the large semantic owners, which currently
run ~50% diff (`080bbb0c` 6332 B / 3109, `080ea0d8` 5756 / 2815,
`080ab5e4` 4888 / 2364, `08027114` 4224 / 1981, `080f6440` 3804 / 1978).

## Main-image unmatched census (2026-08-07)

`docs/main-unmatched-census.txt` is the first full measurement of the whole
unmatched main-image pool rather than the diff <= 40 slice: every `semantic/08*.c`
owner as `stem reference_bytes candidate_bytes differing_halfwords`. 570
measured, 114 build errors, 277,024 reference bytes outstanding. (The other
1,222 `semantic/` files are overlays and out of scope here.)

The distribution is the finding. Bucketing by `diff / (reference_bytes / 2)`,
i.e. the fraction of halfwords that differ:

| band | files | bytes |
| --- | --- | --- |
| < 0.2 | 7 | 1,342 |
| 0.2-0.4 | 13 | 2,002 |
| 0.4-0.6 | 30 | 5,408 |
| 0.6-0.8 | 53 | 15,108 |
| 0.8+ | 467 | 253,164 |

91% of the outstanding byte mass has essentially no byte relationship to the
reference. This corrects an earlier characterisation of the large owners as
"~50% diff" -- they are ~98%: `080bbb0c` 6,332 B / 3,109 halfwords,
`080ea0d8` 5,756 / 2,815, `080ab5e4` 4,888 / 2,364, `08027114` 4,224 / 1,981.

Their *sizes*, though, are within a few percent of reference (119 files totalling
33,014 bytes sit in the 0.8+ band with |size delta| <= 4). That looked like the
signature of a uniform whole-function register permutation, which
`-mlow-reg-order=` would express. It is not. All 24 permutations on `080bbb0c`
land between 3,036 and 3,148 halfwords (stock 3,109), and on `08027114` between
1,981 and 2,045 (stock 1,981 is already the best). The surface is flat: size
agreement is just gcc emitting a similar instruction count for functionally
equivalent C, not a permuted version of the same sequence.

Consequence for planning. There is no near-match path to 25%. The close bands
below 0.8 total under 24,000 bytes even if every one of them closed, and the
measured closure rate on that pool has been near zero for three sessions of new
compiler modes. Reaching 336,431 requires byte-exact reconstruction of large
owners from scratch -- roughly the eight biggest (`080bbb0c`, `080ea0d8`,
`080ab5e4`, `08027114`, `080f6440`, `080dea70`, `080e7404`, `080d1714`) sum to
about 35,700 bytes. That is eight full manual matching efforts, not a flag
search.

## Allocation-order tie-breaks: measured, negative (2026-08-07)

The close band below ratio 0.2 is dominated by a single residual shape: the two
columns emit the *same* instruction sequence with two low registers exchanged
(`080a524c` r2/r3, `08092f84` and `0801faa8` r6/r7, `080ae9f0` r2/r4/r5,
`08077394` r0/r2, `08020198` r6/r7). The direction is not consistent across
functions, so it is not a global register-order flip; it looked like the
equal-priority tie-break in the allocators being visited in the opposite order.

Tested directly. Two default-off fork switches were added -- `-fqty-order-reverse`
(reverses `q1 - q2` in local-alloc's `qty_compare_1` and `qty_sugg_compare_1`)
and `-fallocno-order-reverse` (reverses `v1 - v2` in global.c's
`allocno_compare`) -- built, staged, and swept over sixteen close-band main-image
stems. Result: not one stem moved. Fifteen were bit-identical to stock under
either switch and under both together; `080b5d3c` regressed 4 -> 60 under
`-fqty-order-reverse`. The tie-break almost never binds, so it is not the source
of the divergence. Both switches were reverted and `dist/cc1` restored to the
pinned `41b5d62b`. Do not re-run this experiment.

Source-form rewrites were tested against the same class and also fail. On
`0801faa8`, reordering the declarations and the initialisers moves the diff only
between 11 and 13, and duplicating the shared `negate_result` tail -- the exact
transform that closed `08003538` -- inflates the function by 12 bytes and the
diff to 68. On `08077394`, three restructurings of the same control flow give 27,
25, and 14 against a stock 11. The original transliteration is already the best
source form; what differs is which pseudo the allocator visits first.

Consequence. The close band is blocked on register allocation, not on flags and
not on source shape, which reinforces the planning conclusion above: the path to
25% is from-scratch reconstruction of the large owners, not near-match repair.
