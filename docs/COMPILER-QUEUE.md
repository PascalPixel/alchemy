# alchemy-gcc prototype results (2026-07-26)

Prototyped in `../alchemy-gcc/build-296/`, which is a separate tree from the
digest-pinned `dist/`. **`dist/` was never touched**, so every alchemy build in
this repo still uses the approved bundle. Before changing anything I verified
the prototype toolchain reproduces `dist` byte-for-byte on a known source.

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

**The "up to 11 regions" payoff I relayed from the lane agent is not
supported.** The agent's diagnosis of the guard was exactly right and is now
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

  | region | descriptors | stock | patched |
  | --- | --- | --- | --- |
  | 080f377c | 3 | 0 grouped, first descriptor deleted, 104 B | **3 grouped, all present, 116 B against 112** |
  | 0800300c | 2 | 1 descriptor deleted, 108 B | **2 grouped, all present, 120 B against 112** |

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
unused *anywhere in the function*, which I said confined the twelve-store
`stmia r0!, {r1, r2, r3, r4}` path to argument-free leaves like `src/08004a94.c`.

Dropping both tests changes nothing. 08004bd4 stays at 52 mismatched bytes and
the group still does not form. **My diagnosis was wrong.** The scan breaks at
`index != 12` long before those guards are reached, because dead store
elimination has already removed the descriptor slots a later statement
overwrites, leaving nine stores where the recognizer needs twelve. The
`regs_ever_live` guards were never the binding constraint; I inferred them from
reading the precondition list rather than measuring which one fired.

Reverted, with the measurement recorded at the guard.

## Scoreboard for the four blockers I published

Two of the four did not survive contact with a compiler I could actually run:

| # | claim | verdict |
| --- | --- | --- |
| 1 | volatile blocks grouping, 42 regions | **real and now fixed** (patch 2b): the grouped pattern carries volatility, descriptors survive and group |
| 2 | `value2` copy from hard-coded `(reg:SI 2)` | untested here; still stands on inspection only |
| 3 | whole-function `regs_ever_live` guard | **wrong** — not the binding condition, DSE is |
| 5 | `value1` must be a pseudo | real and fixed, but converts nothing alone |

Blocker 4 (loop-invariant hoisting) was already withdrawn yesterday as
source-reachable. So of five published items, two are confirmed and fixed, one is
wrong, one was withdrawn, and one is still unmeasured. The pattern is that reading a precondition list and
naming the guard that *looks* responsible is not evidence; only running the
patched compiler is.

## State of ../alchemy-gcc

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
the *store* rather than with the other constant, and it also hoists
`mov r3,#255` to the front. 4 mismatched bytes without the mode, 16 with it.

So the remaining lever is not cost, it is **ready-list priority**: which of
several ready insns is chosen to fill the gap. That is `arm_adjust_priority` /
`rank_for_schedule`, not `arm_adjust_cost`. A mode that prefers a constant
materialisation over a memory store when both are ready would close 080a1090,
and 080a47b4 has the same shape (`movs r1,#0` against `adds r5,r0,#0`, plus two
transposed `lsls`). That is the next compiler experiment, and it is a real
tuning loop rather than a one-line guard — worth doing with the harness, not by
inspection.
