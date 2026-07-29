# Alchemy handover

Updated: 2026-07-29 (semantic-C speed lane)

This is the single authoritative session handover. Do not add dated handoff
files; update this file in place.

## Current objective

Eliminate ordinary assembly through a semantic-C speed lane, while preserving
the existing byte-exact build and its stricter metric.

Semantic-C sources live under `semantic/` and compile with
`bun run build:semantic`. They express reviewed behavior and target ABI but do
not claim original machine-code equality. Exact sources remain under
`assets/code/` and continue to require fully linked byte equality.

## Repository state

- Branch: `speed`
- The live Full-C metric is printed by `bun tools/full_c_progress.ts --subject`
  and recorded in each commit subject; regenerate the inventory/report before
  reading it.
- The GS1-English full build is byte-identical with zero ROM fallback.
- The source-only build owns all 8 MiB with zero unowned bytes.
- Semantic-C lane: **146,206 executable bytes across 371 compiling sources**:
  134,070 main bytes and 12,136 overlay bytes. Combined with exact C,
  **284,000 / 1,338,620 executable bytes** are now expressed as C, with
  1,054,620 remaining.
- The lane includes every still-live source from the curated near-match,
  hand-reviewed, prior, and manual candidate queues. Admission rejects
  exact duplicates, dead/nonordinary owners, overlaps, inline assembly,
  unresolved decompiler constructs, and target-compiler failures.
- Honest outer-owner additions include `resource_381:0054/1410`,
  `resource_394:03f0/0c2c/0e64..0fb4`, `resource_3bd:0474/0608`, and
  `resource_3c8:1d48`. `resource_379:00dc` was rejected as a fake standalone
  owner because it is a shared entry using `resource_379:0074`'s
  saved-register frame; the exclusion is documented under `semantic/`.
- The latest main-ROM cohort adds 20,372 reviewed bytes. The largest repaired
  complete owners are `080bbb0c` (6,332 bytes), `080ea0d8` (5,756 bytes), and
  `080ab5e4` (4,888 bytes); ten smaller raw m2c owners passed the same admission
  gates without repair. Rejected mechanical copies were removed from
  `semantic/`, while their originals remain in `work/m2c-fresh-20260722/` as
  the next repair queue.
- The following speed cohort adds another 14,724 reviewed main-ROM bytes across
  16 owners. Each owner compiles and assembles independently, has no unresolved
  decompiler constructs or inline assembly, and preserves the reference's
  external-call multiset. The repaired sources correct raw m2c pointer scaling,
  access widths, stack aliases, and internal-entry control flow before
  admission.
- The next speed cohort adds 12,054 reviewed main-ROM bytes across 20 owners,
  led by the complete 4,224-byte `08027114` owner. It also replaces several
  decompiler artifacts with explicit local arrays or pointer parameters where
  the reference passes stack-backed data through ordinary ABI registers.
- The latest speed cohort adds 6,822 reviewed main-ROM bytes across 10 owners,
  led by the complete 3,804-byte `080f6440` owner and its 57-call audited
  control flow. The cohort also recovers save-slot replacement, M4A MEMACC,
  scene-transition, grid-input, tile-fade, and object-spawn behavior with
  explicit layouts and corrected hidden arguments.
- The following speed cohort adds 6,016 reviewed main-ROM bytes across nine
  owners, led by the complete 3,656-byte `080dea70` owner and its 101-call
  audited control flow. It restores internal frame-loop continuations, stacked
  call arguments, projection locals, record strides, and signed IDs, alongside
  serial-link buffering, UI borders, resource selection, and object searches.
- The latest speed cohort adds 6,682 reviewed main-ROM bytes across 13 owners,
  led by the complete 3,640-byte `080e7404` owner. It restores both of that
  owner's large frame-processing stages and overlapping stack workspace, plus
  battle-selection, portrait, scene-transition, tile-probe, palette-fade, and
  resource-allocation behavior. Every owner passes target compilation and the
  aggregate semantic admission gates; external-call multisets were audited
  against the reference owners.
- The next speed cohort adds 5,138 reviewed main-ROM bytes across ten owners.
  Its largest additions are the 1,168-byte effect-description renderer,
  864-byte two-choice UI, and 768-byte bordered-window compositor and combat
  status panel. The cohort also restores allocation-slot, active-entry,
  terrain-height, menu-grid, preset-value, and map-rule behavior. All ten
  owners compile and assemble independently and match their reference
  external-call multisets.
- The following 2,128-byte cohort adds four main-ROM owners: two related
  combat-effect selection menus, the text-layout and inter-word-spacing
  calculator, and the 32-cell resource debug viewer. Their shared menu
  navigation was expressed with explicit state layouts while preserving all
  reference call sites, including branch-specific sound and dialog paths.
- The latest 2,760-byte cohort adds the complete status-comparison overview
  and paginated effect list (`08022b44`) plus the interactive 15-entry
  selection editor (`08020bd8`). Both owners compile and assemble
  independently and exactly preserve their reference external-call
  multisets; the editor also restores its grid navigation, animated list
  cursor, add/delete/commit behavior, and palette DMA without decompiler
  constructs.
- The following 1,396-byte addition reconstructs the complete `080cfef4`
  scene loop: staged background blits, scanline-wave generation, palette
  transitions, object activation, and cleanup. Its eight branch-specific
  blit sites and full external-call multiset match the reference owner.
- The next 2,008-byte cohort adds `080bfba4`, which repairs party-entry
  grouping and emits four categories of stat/link results, and `0801d108`,
  the complete status-window and sprite constructor. Both retain every
  branch-specific message/UI call site and match their reference external-call
  multisets.
- The following 1,336-byte owner, `080a9f10`, reconstructs item/effect
  application for one combatant or the whole party: HP/PP restoration,
  revival, status clearing, permanent stat gains, result codes, and party
  recalculation. It corrects raw decompiler switch labels for effect IDs
  `0x104` through `0x109` and preserves all five mutation call sites.
- The next 1,920-byte owner, `080f7f78`, reconstructs the sliding-window
  compressor: modulo-1024 dictionary matching, lazy-match selection,
  short/long back-reference encoding, flag-byte packing, buffer flushes, and
  terminator emission. Four duplicated wraparound searches in the raw output
  are expressed by one reviewed match-search helper while preserving the
  reference's two refill and two flush call sites.
- The following 1,376-byte owner, `0801a98c`, reconstructs the linked-list
  entity and cursor renderer, including position easing, animation phases,
  visibility flags, priorities, selection tracking, and four render groups.
  The previously ambiguous `080b50b0` veneer was verified to take an entity ID
  and a two-word position output, removing the raw decompiler's missing ABI
  context before admission.
- The next 1,264-byte owner, `08099da4`, reconstructs a complete field
  interaction/cutscene: companion-object setup, vertical entrance, tile
  probing and collision retries, synchronized positioning, particle burst,
  camera handoff, timed animation, and cleanup. Its local particle pointer
  array replaces the raw stack alias while all 72 named external call sites
  remain intact.
- The following 1,208-byte owner, `080dd2c4`, reconstructs the timed
  emitter/particle scene, including 16 emitter tracks, a 1,024-record pool,
  character triggers, four blit paths, blend timing, frame presentation, and
  cleanup. The raw decompiler's scaled-pointer error was corrected to the
  reference's 28-byte particle stride before admission.
- The next 1,152-byte owner, `080f26ec`, reconstructs the complete title
  transition: staged tile and map DMA, display setup, skip handling, object
  creation, blend animation, and cleanup. Its shared frame-yield control flow
  preserves all 38 reference call sites, and its independently compiled target
  assembly matches the reference's named-call multiset.
- Remote exact-C integration adds 806 overlay bytes across seven owners:
  `resource_394:0e64`, `resource_3bc:13c0`, `resource_3bd:0eac/0f10/0f6c/0f94`,
  and `resource_3c8:1a50`. The former semantic `resource_394:0e64` copy was
  removed because the same owner is now byte-exact. The staged early-pool mode
  for `resource_394:0ee0` is path-scoped so it cannot perturb unrelated
  overlays that share the same load address.
- The next 1,152-byte main owner, `0801d4cc`, reconstructs the complete
  five-row configuration menu: preset cycling, value wrapping, three indicator
  groups, window redraw, cursor movement, accept/cancel handling, and persistent
  settings. Its independently compiled target assembly preserves every named
  reference call site, including all seven branch-specific sound calls.
- The following 1,132-byte owner, `080d3c80`, reconstructs a complete
  two-stage particle scene: direction-dependent emitter initialization, camera
  movement, delayed emitter activation, sixteen-particle bursts, animation
  selection, character triggers, frame presentation, and cleanup. Its target
  assembly preserves the reference's full external-call multiset, including
  seven distinct random-number sites and both rendering/update paths.
- The next 1,124-byte owner, `080db264`, reconstructs the complete polar-burst
  effect: 1,024-particle initialization, four rotating emission waves, three
  branch-specific sprite renderers, gravity and bounce updates, timed character
  triggers, and cleanup. Its target assembly preserves all 38 named reference
  call sites, including nine RNG sites and three sine/cosine pairs.
- The following 1,096-byte owner, `080d41a4`, reconstructs a staggered
  multi-character burst scene: camera motion, timed foreground/background
  strips, per-character particle emission, gravity and bounce animation,
  character triggers, presentation, and cleanup. Its target assembly preserves
  the complete reference call multiset, including both strip renderers and the
  independent particle renderer.
- The next 1,092-byte owner, `080db6e0`, reconstructs a dual-layer projected
  particle scene: optional grayscale palette setup, two initialized particle
  banks, camera synchronization, depth-scaled rendering, animated emitters,
  mode-specific character triggers, presentation, and cleanup. Its target
  assembly preserves the full reference call multiset, including both
  projection/draw/update paths and all six RNG sites.

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
094c (30). Two work streams (both interrupted by a container restart;
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

Three further rounds adopted ~80 more functions: resource_3ac and
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
parks). Remaining frontiers: 399 0x18c4+ (u16-local HImode pool shapes),
3ca 0xda4+, 394 0xc2c (hi-reg giant), 3bd 0x474/0x608/0x8c0,
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

Clean-room rules remain authoritative in `PROVENANCE.md`.
