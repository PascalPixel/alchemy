# Alchemy handover

Updated: 2026-07-29 (remote overlay session 3)

This is the single authoritative session handover. Do not add dated handoff
files; update this file in place.

## Current objective

Continue the decompilation, prioritizing overlay code. The prior
**152,000 exact-C byte** target is within ~4 KB as of session 3.

Exact means fully linked machine-code byte equality, not semantic similarity
or equal object size.

## Repository state

- Branch: `main` (session work lands on `claude/continue-decompilation-3drfw0`
  and is merged by the owner)
- The live Full-C metric is printed by `bun tools/full_c_progress.ts --subject`
  and recorded in each commit subject; regenerate the inventory/report before
  reading it.
- The GS1-English full build is byte-identical with zero ROM fallback.
- The source-only build owns all 8 MiB with zero unowned bytes.

## Toolchain on this host (linux-x64)

All compiler bundles were rebuilt from the pinned alchemy-gcc commit
`64d757f` at `/home/user/alchemy-gcc` and re-admitted: gcc296 (gs1),
old_agbcc, gs2, and both experimental comparison compilers
(pret-early-thumb, gcc2951 — previously macOS-only, now per-host digests).
Admission evidence: the composed source-only image reproduces gs1-en.gba
byte-identically (SHA-1 5c4695205413df7db52b9a184815a07783999971) and
`bun run verify` is green end to end. `roms/gs1-en.gba` can be recomposed
from the source-only build products when absent; binutils-arm-none-eabi,
gperf, and bun 1.3.14 are required host packages.

## Session results (this session)

resource_3bc linear walk: exact adoptions 058c (68), 05d0 (2), 05d4 (10),
05e0 (120), 0658 (76), 06a4 (110), 0714 (88), 0a20 (34), 0a44 (64),
0a84 (80), 0ad4 (92), 0b30 (112), 0ba0 (2), 0d70 (18), 0d84 (24),
0d9c (8), 137c (68), 2710 (72), 2758 (308), 2a50 (66). resource_3c8:
094c (30). Parallel agents (both interrupted by a container restart;
their work below survived on disk and is committed) adopted the
small-queue members resource_371:0030, 372:0030, 383:0048, 394:0be4,
399:0030, 3a8:00ac, 3ac:004c, 3ce:007c, walked resource_383 through
02c0/02e8/0310/0354/0378/0400/0454/04bc/04f4, and walked resource_3c8
through 0b08/0b98/0c5c/0cc8/0e7c/0e80/0e88/0f1c.

Reusable shapes proved this session (all backed by exact installs):
- paired stack-argument locals assigned in consecutive statements, second
  pair x-first (resource_3bc_c_020005e0.c);
- named base-pointer copy to keep Data_02000240 base + runtime `250<<1`
  offset (`s16 *table = Data_02000240; *(s32 *)&table[250]`);
- `while (*p != a && *p != b)` guards duplicate their exit test only when
  the guard reads a direct constant dereference and the loop body reads a
  pointer local assigned inside the guard (resource_3bc_c_02000a84.c);
- masked coordinate as `z = obj->z & 0xFFF00000;` then `z + K` in the call
  (resource_3bc_c_02000ad4.c, 02000b30.c);
- signed `/ 65536` for the `(neg ? +0xffff : ) >> 16` shape, plus a copy
  variable for one abs and in-place negation for the other
  (resource_3bc_c_02002758.c);
- HImode constant stores pool as `ldrh .L` unless the value goes through an
  int-typed local; a pointer-local assigned before the value forces the
  reference's address-then-value order (resource_3bc_c_02002710.c);
- one C variable reused for two sequential object fetches produces the
  entry `mov r5, r0` copy (work/claude notes for 13c0).

## Later rounds (same session)

Three further agent rounds adopted ~80 more functions: resource_3ac and
resource_3ce walked to their veneer banks, resource_383 covered
0x052c-0x1e80, resource_399 through 0x0abc, resource_37a through 0x1510,
resource_3bd 0xa54-0xde8, resource_3c8 through 0x1f5e, plus the routed
adoptions below. New overlay flag routes (each entry has an exact-byte
proof recorded in its work/claude/notes file):
NO_SCHED_DEPEND_COUNT_OVERLAY_SOURCES (399 family + 3ce:0244),
THUMB_IMMEDIATE_LATENCY_OVERLAY_SOURCES (399:05dc/0a3c/0abc, 37a:1380),
NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES (37a:0054/0108/0150/01ec,
399:0abc), NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES (383:082c).

Key new lever: hoisting a single int-typed named local into the entry
block (`g = 0x986;` before the first branch) makes gcse rematerialize
the pool constant at each later-block use, defeating the cross-call
CSE-share blocker EXCEPT when a use sits in the entry block itself or
the constant is spelled as a &Value_ symbol. After round 5: resource_383
is COMPLETE to its veneer bank (0x4c6c); 37a's 0x1510-0x296c stretch is
fully classified (adoptable members taken, rest parked); 3bd's 0x0c4
gap is hand-written assembly (not C) and 0x2a8-0x474 is covered.
After round 7, resource_399 and resource_3ca are COMPLETE to their
veneer banks (0x2168 and 0x1314); resource_371/372 are covered through
0x590/0x400, resource_3b0 through 0x4b0. New GROUPED_DMA_STORE_OVERLAY_
SOURCES route (3ca:10d4). Cross-overlay scheduler fingerprint worth a
compiler-lane look: a store-before-independent-load pair order that
would unlock seven parked functions at <=3 halfwords each (3b0:0030,
3b0:0468, 371:02f0, 372:0278/0398/03cc/0400 — see their notes).
After round 6, resource_381 is also COMPLETE to its veneer bank
(0x33d4): every prologue adopted, parked with a best draft, or parked
with a category note (the 3.5 KB root initializer at 0054, the 5.1 KB
1410 sheet, 301c, 2eb0, and the ip-call functions are the category
parks). Remaining frontiers: 399 0x18c4+ (a lane was cut mid-work by a
usage limit while attacking the 399 walk with u16-local HImode pool
shapes), 3ca 0xda4+, 394 0xc2c (hi-reg giant), 3bd 0x474/0x608/0x8c0,
3c8 0x1d48/0x1f60 giants, 3b0 0x180+, 371/372 after 0x0030, and the
giant single functions (379:00dc, 3bc:0da4).

Round-5 escalations for the compiler lane: a register-allocation
parity disease (identical instruction streams, scratch/callee register
identities swapped) accounts for most near-misses (383:4754/47bc/47fc,
399:1704/174c and earlier 3bd:034c); and the inline
`ldr r3,=0x03000118; mov ip,pc; bx r3` IWRAM call idiom has no C
spelling (blocks 399:15b4, 3ca:0194). A mid-function pool-dump gap
blocks 37a:1510 (640 B, instruction-exact otherwise) — needs an
early-literal-pool variant that re-creates duplicate pool entries.

## Blockers discovered (documented under work/claude/notes/)

1. **resource_3bc runtime base is 0x02008000.** Jump-table words in the
   0da4 switch (0x0200_9234...) and callback constants (0x02008659 =
   offset 0x658|1) prove it. compileOverlayC links at OVERLAY_BASE
   0x02000000, so any function embedding compiler-generated absolute
   label addresses (switch tables) cannot verify until per-overlay link
   bases exist. Blocks 0da4 (~1.5 KB) and likely other giants.
2. **resource_3bc tail vintage question.** Five functions (0ba4, 0c5c,
   288c, 29ac, 2a94/2b50) show reference codegen our gs1 cc1 cannot emit
   from any measured source shape: cse never folds const+1/+2 into fresh
   pool words there, register-argument split constants rematerialize per
   call, and the three-way selection keeps cmp/bne+in-arm loads where our
   jump optimization threads them 4 bytes shorter. The gs2 (gcc 3.0)
   bundle derives `add #1` for the same spelling but is worse elsewhere.
   Everything before ~0x2758 in the same overlay matches gs1 exactly.
   Compiler-lane question; do not respin source variants (measured lists
   in the notes).
3. **Allocation-priority rotations** (076c at 164 hw with exact tail,
   13c0 at 36 hw): the short-lived shared constant outranks steps/state
   in our global allocator; reference ranks it below both and pays a
   caller-save pair. Same QTY_CMP_PRI family as LAWS.md.

## Best restart points

- resource_3c8: continue the walker's linear scan (check git log for its
  last adoption).
- resource_370:0054 is a 256-byte multi-descriptor grouped-DMA function
  (three stmia descriptor groups, mid-function pool) — compiler-lane.
- resource_379:00dc is one 2,524-byte function; large but single.
- resource_381 holds ~18 KB of discovered unconverted functions plus the
  3,548-byte semantic draft from the previous session (work/ was not
  preserved in this container; treat as fresh).
- The twelve-member 60-byte family (resource_373:0030 etc.) remains
  parked; no exact result across 1,633 configurations previously.

## Session 3 (this session)

Live metric at last push: **[C 148,066/1,339,340 bytes]** on
`claude/continue-decompilation-3drfw0`. Every commit below passed the full
required-check cycle (byte-identical ROM-mode build).

Overlays walked to a frontier this session, with the next unwalked offset:

| overlay | state | next offset |
| --- | --- | --- |
| resource_3b1 | fully triaged 0x0030-0x10a6 | none (code ends) |
| resource_3bf | walked | 0x1150 |
| resource_380 | park-dominated midsection | ~0x4852 (blobs before) |
| resource_3b8 | walked | 0x0af8 |
| resource_3a4 | walked | 0x0d2c (blob to veneer ~0x3bbc) |
| resource_39e | walked | 0x0bd4 |
| resource_3af | lane cut by usage limit | 0x10a0 |
| resource_39c, 3ba, 39d | lanes in flight | check git log |

New flag routes added to `tools/alchemy_gcc.ts`, each with its own
exact-byte proof recorded in the matching `work/claude/notes` file:

- `NO_STRICT_ALIASING_OVERLAY_SOURCES` (new set): resource_3c9 0104/215c/
  21ac/3600, resource_380:0104, resource_39e:0104. With strict aliasing our
  scheduler treats a `(u16 *)` view store as independent of the struct-field
  re-read and sinks the load below the store pair; the reference keeps the
  original order.
- resource_3b8:049c needs `-mthumb-immediate-latency` **and**
  `-fno-rerun-cse-after-loop` together; neither alone reaches zero.
- resource_3a4 status-window family (09ec, 0a94, 0b3c, 0bd8) under
  `-fno-rerun-cse-after-loop` alone: default flags CSE-hoist a thrice-used
  pool constant into r5.

Later additions: `-fno-rerun-cse-after-loop` also carries resource_3ba:0540
(752 bytes, the largest single routed adoption so far) — the rerun shares the
`0x301` argument between the entry-block call and the else-branch call, where
the reference keeps both sites independent.

New reusable levers proved this session:

- bitfield setter spelling `S.f1 = v` cracks movs/negs mask sequences;
- shared-index `((s16 *)p)[i]` defeats the pool-load hoist;
- in-place s16 clamp `t <<= 16; t >>= 16;`, while the s16-narrowing mask
  spelling instead forces an early mid-function pool dump;
- `f(a, v = K, 0)` keeps K in a callee-saved register where plain spellings
  constant-propagate;
- plain-integer index `Data_02000240[0x22b] = 3` defeats the strb reg-reg
  fold that every pointer-sum spelling produced;
- a **union-typed slot** (`union Slot { s32 w; u16 h[2]; void *p; }`) blocks
  the alias-based load hoist that a same-type cast does not. When
  `-fno-strict-aliasing` is the tell, try this first: it converts a flag
  route into a plain default-flags adoption (proved at resource_39d:0104);
- a `u8 f:2` bitfield struct produces the `movs #13 / negs` mask form where
  plain mask spellings CSE the constant;
- **prototype-less** callee declarations (`void Func_02003b12();`) where one
  veneer target is called at different arities from different sites;
- per-site scoped locals with compound in-place updates
  (`{ s32 x = A; x <<= 20; x += K; }`) for repeated coordinate triples.

New park class (now the single most common blocker in overlay call sheets):
**duplicated-constant argument families.** The reference materializes an
expensive constant (0x10000, 0x20000, 0x30000, -1, 0x33333) separately at
each call site while our -O2 pipeline CSEs it into a callee-saved register,
cascading into a different prologue. It dominates resource_380
0x390/0x6f4/0x0a98, resource_39e 0x388/0x518/0x71c, resource_3b8 0x674,
resource_3a4 0x6dc/0x7e8/0x8d4/0xc9c, and resource_3ba
0x840/0x974/0xdb8/0x1214/0x13e4.

The mechanism is now **proved by synthetic** (`work/claude/t_syn.c`, results
in `notes/resource_3ba-13e4.md`): this cc1 unconditionally CSEs any repeated
**two-instruction immediate** across call sites — independent of literal
spelling (`0x10000`, `65536`, `128 << 9`, `-1`, `0xFFFFFFFFU`), argument
signedness, prototype vs. prototype-less declaration, or an intervening
call — and it survives every one of `-fno-expensive-optimizations`,
`-fno-thumb-contiguous-immediate`, `-fno-strength-reduce`, `-fno-regmove`,
`-fno-rerun-cse-after-loop`, `-fno-gcse`, `-fno-cse-follow-jumps`,
`-fno-cse-skip-blocks`. Single-instruction immediates are unaffected.
**Triage rule:** before drafting any call sheet, scan the reference for the
same two-insn constant built at two or more sites; if present, park without
drafting. This is a compiler-lane item, not a source-shape problem.

Host note: this container has **4 cores**, so at most two walker lanes plus
the main agent are useful; more lanes thrash the compile step.

## Build caching (added session 3)

The three build stages are content-cached, so a round that adopts one function
no longer recompiles all 1,376 C units, reassembles all 1,807 regions, and
re-encodes all 2,431 assets. Measured on this 4-core host:

| stage | cold | warm |
| --- | --- | --- |
| build_claimed | 15 s | 0 s |
| build_asm | 29 s | 0 s |
| build_assets | 37 s | 0 s |
| build_full (whole image) | 84 s | 1 s |

- `build_claimed` keys each object on sha256 of the source bytes plus a stamp
  of the exact command plan and the compiler binaries it names
  (`out/cache/claimed-objects`). Sound because no generated source under
  `assets/code` carries a `#include`, so a unit's input closure is one file.
  A changed source, a changed flag route, or a changed compiler all produce a
  new key. `objectCacheKey` has a self-test in the `bun run test` chain.
- `build_asm` keys each region on sha256 of the assembly bytes plus the link
  address (`out/cache/asm-regions`). Sound for the same reason: no
  `.include`/`.incbin` anywhere in the assembly sources.
- `build_assets` skips the whole stage against a stamp over every asset source,
  every encoder in `tools/`, the manifest, and the build mode
  (`<output>/stage-stamp.txt`). Deliberately conservative — any change to any
  input rebuilds everything.

The byte guarantee does not rest on the caches: `build_full` still re-reads
every region and compares it to the reference ROM, then compares the composed
image as a whole, so a stale entry cannot produce a wrong image. Delete
`out/cache` to force a cold rebuild.

Note the stages are independent once each has its own output tree, so they can
run concurrently (94 s vs 161 s cold for the two full builds); only the
ROM-mode build should write the default `out/full` tree.

## Large call-sheet functions: the locals idiom, and how to measure progress

For a big constant-argument call sheet the reference source idiom is **one
function-top-declared `s32` local per non-trivial constant argument**. Proof by
an adopted sibling: `assets/code/resource_372_c_02002180.c` verifies exact under
default flags, carries about 100 such locals, and reproduces the
`movs r1,#192 / movs r0,#10 / lsls r1,r1,#16` interleave that plain literals
never produce. On resource_379:0074 the ladder was all-literals 47 wrong groups
of 288 → locals at all 117 constant sites → 8 of 288.

**Halfword counts are a bad progress metric for large functions.** Every `bl`
displacement is target-absolute, so any positional drift makes every subsequent
`bl` halfword differ: the 8-of-288 draft still reports 1,235 differing halfwords.
Drive the search on normalized instruction-group equality (split the stream at
each `bl`) and keep the halfword count only as a final gate.

resource_379:0074 remains parked 8 groups and 40 bytes short, with drafts at
`work/claude/notes/resource_379-0074-{best,alt}.c`. Its blocker is register
allocation, not codegen: gcc hands all seven callee-saved registers to the
earliest constant locals, forcing `push {r5,r6,r7,lr}` plus high-register saves.
In resource_372 an early `if (...) return;` splits the entry block at instruction
4, so every constant pseudo crosses a block boundary, global-alloc declines and
reload rematerializes; resource_379:0074 has no early branch and its declarations
and first uses share one ~600-insn block. Dropping the earliest locals, block
scoping, and assignment-at-use were all tried and are all worse (see the note for
the full ladder). Worth considering that the absence of an early branch may mean
our semantic reconstruction is missing an early guard the original had.

### Corrections to earlier advice in this file

- **`(s32)&Value_0000XXXX` is not universal — it is inverted for pool words.**
  It *forces* a pool load. Constants that want `movs #K; lsls #n` need plain
  literals or locals. Use the symbol spelling only when the reference actually
  pool-loads the value.
- **Multi-arity callees do not need K&R declarations.** The repo mechanism is the
  `_b`/`_c` alias suffix, admitted by `ADDRESS_SYMBOL` in `tools/alchemy_gcc.ts`
  and used by the adopted resource_372 sibling.
- **The "following call group arity" rule for argument order is false.**
  resource_379:0074 at 0x41a is a direct counterexample. Argument order falls out
  of the locals idiom plus the callee's declared return type (below), which is
  the mechanism that is actually proven by 15 exact adoptions. The arity rule was
  an artifact of one overlay's uniform call shapes; do not rely on it.
- A large overlay's apparent "hundreds of distinct engine callees" can be a
  mechanical artifact: in resource_379 all 286 `bl`s carry one `H1` and only 41
  distinct `H2` values, mapping onto the 41 veneers at 0xab8-0xbf8. The
  `sub_0200XXXX` names there carry no semantics.

## The biggest lever found so far: a callee's return type sets argument order

The most common park class in every overlay — "per-site contradictory movs pair
order", two halfwords, no flag reaching it — turned out to be **source-reachable
with the pinned compiler and default flags**. 15 functions, 3,020 bytes, adopted
in one round.

Mechanism, proved with `-fsched-verbose=7` dependence tables: argument setters
leave RTL expansion in ascending register order in every case, and `sched2`
permutes them. `rank_for_schedule` (`haifa-sched.c:4117-4129`) prefers the insn
with more forward dependents. For a **value-returning** call, `sched_analyze_1`
frees `reg_last_sets[r0]` and replaces it with the call
(`haifa-sched.c:3763-3769`), so the pre-call `movs r0` loses its later
dependents and `movs r1` wins outright → r1 first. For a **void** call, only
`reg_pending_clobbers[r0]` is set, which appends and leaves `reg_last_sets[r0]`
naming the `movs r0`, so it keeps collecting dependents, the ranks tie, and the
tie-break falls to `INSN_LUID` → original order → r0 first.

So the knob is whether the call redefines r0 at that site, i.e. the callee's type
**in scope at that site**:

- flip one `extern` return type between `void` and `s32` (12 of the 15 fixes);
- where one symbol genuinely needs both orders in one function, cast at the site:
  `((void (*)())Func_020023a8)(0, 50)` — still a direct `bl`, no pool word, no
  extra instruction (2 of the 15).

**`(void)Func(...)` does not work** — the `CALL_EXPR`'s own type is unchanged.
That is why years of probes missed this.

A corpus scan of 13,714 reference argument-setter runs found no correlation with
argument values or count, and the whole 3-argument permutation histogram falls out
of two inputs: whether the call redefines r0, and the arity of the *following*
call group in the same block (a register whose next writer is further away picks
up an extra `REG_DEP_ANTI` per intervening call). The second input is the
resource_379 walker's empirical rule, confirmed as the void-callee case of the
same mechanism.

Two caveats. The per-site function-pointer cast is ordinary C and involves no
`asm`, no register pinning and no `volatile`, but it is unusual enough that the
two functions using it (resource_3ce:10a8, resource_3af:3710) are worth a human
look. And a gated scheduler mode for the same defect was prototyped
(`-fno-sched-dead-call-value`, 47/2,145 collateral, inert by default) and
**deliberately not landed**: it fixed exactly one function that a source cast
already fixes, and would have cost a fork commit plus a re-pin plus the full
PROVENANCE ROM gate. The diff is at /tmp/argorder/h/ if it is ever wanted.

Also settled: inside the scheduler there is no per-site property left to key on
except the call's own RTL form, because `schedule_insns` purges death notes
(`haifa-sched.c:7071`) before dependency analysis. No scheduler predicate can
distinguish two sites of one symbol — which is why the fix has to be in the
source, and closes that search.

## Read this before planning: the per-overlay "remaining bytes" figure lies

`out/decomp/overlays.json` lists nested discoveries that are `contained_by` one
another, so naively summing a row set re-counts the same region many times over.
Measured inflation against the largest genuine function in each overlay:
resource_381 17.6x (96 of its 115 rows are contained), resource_379 9.1x,
resource_37a 5.8x, resource_3a1 4.8x — while resource_373, 3a8, 396, 39f, 3b5
and 3c4 are honest at about 1.0x. A walker burned a whole lane on resource_379
expecting ~22,000 bytes and found 2,628: 65 of its 66 rows were nested walks
starting at successive addresses *inside the veneer bank*, all ending at the same
place. **Always filter to rows with an empty `contained_by`.**

Filtered that way, the identified unconverted function queue is **227 functions
totalling 16,256 bytes**, of which resource_381 holds 7,960 and resource_379
2,422 — so roughly half the queue is one overlay and the whole queue is small.

That is the strategic fact for anyone asked to "finish the decompilation": the
Full-C denominator (1,339,536) deliberately includes linker veneers, structural
assembly, literal-bearing regions and executable alignment, which will never be
C, so 100% is not the target. The tracked measure of real remaining work is
`asm_c_debt_bytes`, printed by every full build and currently **395,816**. The
gap between that and the 16,256-byte identified queue is the honest bottleneck:
most C-debt assembly has not yet been *discovered* as functions. Converting the
known queue is close to exhausted; scaling further needs discovery work, not more
walker lanes over the same inventory.

## Permuting: keep it as an audit pass, not an engine

`tools/permute_overlay.ts` drives the annealing search in `tools/permute_v1.ts`
against overlay targets (that file gained exports — `anneal`, `OPERATORS` and
friends — so nothing is duplicated; its main-ROM behaviour is unchanged). It
reaches about 70 candidates/s at `--jobs 2`, builds each overlay reference once,
mirrors `compileOverlayC`'s command sequence exactly so a zero here is a zero in
`overlay_verify`, filters out any mutation introducing `volatile`/`asm`/
`register` to stay clean-room, and `--reduce` delta-debugs a hit back to minimal
source before adoption.

Measured verdict from 65,543 candidates over 15 targets: **one exact hit**
(resource_3c9:04bc, 92 bytes) and not one other floor moved by a single
halfword. The hit is instructive out of proportion to the score — the whole fix
was moving `limit = 640;` above a call, and the note had swept the *call's*
position but never the threshold assignment. So treat "no source lever found" in
a note as unproven: hand sweeps only explore levers the author imagined, and
this residual class turns on which of two independent statements the scheduler
sees first. Run the permuter once over any newly parked near-miss (minutes) before
believing the park. It also caught a stale note: resource_3c8:1a50 is adopted and
exact, not parked.

What it will not do is close the backlog, and that is now quantitative rather
than a hunch: two operators purpose-built for the "reference keeps source order"
fingerprint were accepted 0-4 times per target and never beat the floor. Those
residuals are `sched2` ready-list tie-breaks, not statement-order effects.

## Compiler lane: the two-insn immediate mode is LANDED

`-fno-cse-two-insn-immediate` is now in the pinned fork (branch
`claude/cse-two-insn-immediate` in alchemy-gcc), admitted here, and routed for
resource_3bf:0bec, resource_3af:1a98 and resource_3af:4218. The admission
followed `PROVENANCE.md`: the source-only build with the rebuilt `cc1`
reproduces gs1-en.gba at SHA-1
`5c4695205413df7db52b9a184815a07783999971` byte-identically, and only then was
the digest re-pinned. Note the coverage contract also requires a matching
explorer mode in `tools/mode_sweep.ts` — the commit is rejected without it.

Three fingerprints remain, each unlocking several functions at once, and each
worth a dedicated lane (all three have targets and floors listed in the notes):

1. **Immediate-build transposition** (~1,050 bytes) — exposed by the new mode:
   the reference schedules a neighbouring one-instruction immediate *between* a
   rematerialized constant's `movs` and its `lsls`. Nine flags already swept and
   inert; needs a new mode.
2. **Per-site argument-emission order** — the most common park class overall.
   Our order is fixed per callee by its declared return type (`s32` gives
   `movs r1` first, `void` gives r0 first), but the reference wants *both*
   orders from one symbol at different sites, so no source spelling reaches it.
3. **gcse PRE load insertion** — resource_37a:0d9c sits at a single halfword;
   the cheapest possible confirmation that a gate works.

## Superseded: the two-insn immediate blocker as first written up

The largest single class of unconverted overlay functions has been traced to one
guard in `cse_insn`'s destination-recording loop, and a gated flag prototype
exists. Full write-up, prototype diff, and collateral list are in
`docs/compiler-evidence/`. Headline numbers: with the flag absent the patched
compiler is byte-identical to the installed bundles on all 1,335 gcc296-routed
sources; with it on, 109 of those change, so it must be a routed per-source mode.
Three functions become byte-exact immediately (392 bytes), five more drop to
2-13 halfwords, and roughly 9,000 bytes of population sit behind the behaviour.

**Not landed, and it cannot be landed from the alchemy repository alone.** The
fork change has to be committed to `alchemy-gcc` and its pinned commit updated
before the digests in `tools/alchemy_gcc.ts` may be re-pinned, and
`PROVENANCE.md` only admits a re-pin after the source-only build reproduces
gs1-en.gba byte-identically. Treat that as a deliberate decision, not a chore.

A smaller follow-up lane falls out of it: once the flag removes the sharing, the
entire residual on the near-exact members is a scheduling transposition (the
reference puts a neighbouring one-instruction immediate between a rematerialized
constant's `movs` and its `lsls`). `-mthumb-immediate-latency` halves it on one
member; the rest of that sweep is recorded in the write-up.

## Required checks

Before every exact-C commit:

```sh
bun tools/full_c_progress.ts --write-inventory
bun tools/full_c_progress.ts --write-report
bun run verify
```

Commit subjects must end in the live suffix printed by:

```sh
bun tools/full_c_progress.ts --subject
```

Clean-room rules remain authoritative in `PROVENANCE.md`. Use at most
three active agents total, including the main agent.
