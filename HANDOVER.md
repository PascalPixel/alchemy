> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

# Alchemy project audit and forward plan

Audit checkpoint: 2026-08-02, `main`. This is the single authoritative current
handover. Historical studies remain useful evidence, but their counters and
queues are not live project status.

Verification status: the complete `bun run verify` gate and
`bun run coverage:check` both pass at this checkpoint. The normal build is
byte-identical, source ownership is complete, and ROM fallback is zero.

## Executive verdict

Alchemy is a complete, byte-identical, source-owned reconstruction of the
8 MiB GS1 English ROM image. It is **not** a fully decompiled C project.

- Exact Full-C Byte Share is **266,832 / 1,343,470 executable bytes
  (19.86%)**.
- Reviewed semantic C adds **850,024 executable bytes**. Exact plus semantic C
  covers **1,116,856 / 1,343,470 bytes (83.13%)**.
- The main executable has a closed semantic census: every ordinary owner has
  exact or reviewed semantic C. That closure has **not** made exact matching
  materially faster by itself.
- The current exact-C 20% threshold is 268,682 bytes. The remaining gap is
  **1,850 exact bytes**. The latest tranche promoted the 124-byte code-overlay
  owner `resource_3a7:0944` using source-proven veneer aliases, explicit delta
  dataflow, a labeled loop tail, and a source-scoped `-O3` route. It follows
  the 180-byte main-image owner `08011590` using an existing grouped-DMA/
  scheduler route, and the earlier `080907b0` (116 bytes), `08098b10`
  (248 bytes), and `08077f70` (284 bytes) promotions.
- The largest practical exact-C queue is now the code overlays, not the main
  executable's semantic reconstruction.
- Compiler and permuter improvements reduce the cost of individual trials, but
  neither has produced compounding or exponential delivery. Fresh
  reconstruction plus narrow, already-proven compiler routes remains the best
  measured method.

Use conventional decompilation terminology in all user-facing work:

| Meaning | Preferred term |
| --- | --- |
| Always-resident program | **main image**, **main executable**, or **resident code** |
| RAM-loaded program module | **code overlay** or **RAM-loaded module** |
| Non-code ROM content | **data** or **assets** |
| Shipped 8 MiB file | **ROM image** |

Internal path names such as `assets/code`, historical text, and stable API keys
may retain older identifiers when renaming would damage reproducibility.

## Checked byte accounting

| Scope | Exact C | Semantic C, excluding exact | Unknown assembly | Retained exact assembly | Executable total |
| --- | ---: | ---: | ---: | ---: | ---: |
| Main executable | 105,052 | 412,826 | 0 | 30,486 | 548,364 |
| 96 decoded code overlays | 161,780 | 437,198 | 152,380 | 43,748 | 795,106 |
| **Total** | **266,832** | **850,024** | **152,380** | **74,234** | **1,343,470** |

Additional ROM-image facts:

- Total ROM image: **8,388,608 bytes**.
- Data/assets outside the executable denominator: **7,298,755 bytes**.
- Compressed code-overlay streams occupy **541,489 ROM bytes**; their decoded
  executable size is counted in the separate 795,106-byte code-overlay namespace.
- The source-only full build owns **8,388,608 / 8,388,608 bytes**, uses zero
  fallback, and reproduces the reference ROM byte for byte.
- Diagnostic source counts are 1,428 main-image C files, 1,834 exact code-overlay
  C files, 701 semantic main-image C files, 1,124 semantic code-overlay C files,
  and 41 headers. These are useful inventory counts, not progress percentages
  or reliable function counts.

The semantic compiler reviews 872,406 bytes of owner spans, but 22,382 of those
bytes lie outside the audited executable extents, chiefly pool/tail portions of
code-overlay owner spans. The coverage numerator therefore uses 850,024, not
872,406. `tools/build_semantic.ts` now reports both figures instead of silently
adding out-of-scope bytes.

## What exact C means

The exact numerator admits only canonical C whose compiled and linked ownership
is byte-identical to the reference. Inline assembly, hard-register pins,
fakematches, guessed spans, and semantic equivalence alone do not count.

Exact-C ownership is independently unioned in the main-image and code-overlay
address spaces, then reconciled against the audited executable inventory. The
byte metric is authoritative. File, function, discovery, and region counts are
scheduling diagnostics because their boundaries and scopes differ.

The current split is:

- Main executable: **105,052 / 548,364 (19.16%)**.
- Code overlays: **161,780 / 795,106 (20.35%)**.
- Combined: **266,832 / 1,343,470 (19.86%)**.

## Main-executable audit

The full-build assembly manifest contains 1,756 rows / 443,312 bytes:

| Manifest disposition | Rows | Bytes |
| --- | ---: | ---: |
| Ordinary compiler-output debt | 584 | 348,260 |
| Other retained structural/low-level rows | 1,172 | 95,052 |
| **Assembly manifest total** | **1,756** | **443,312** |

Those values describe exact build ownership, not missing semantic C. Most of
the ordinary assembly rows now have reviewed semantic owners. Conversely, two
56-byte `src/` files (`080a7440` and `080b6e7c`) are byte-matching but use
noncanonical inline-assembly/register scaffolding. They remain outside the exact
numerator and now have explicit semantic replacements.

### Complete retained-complement audit

The former dashboard shortcut painted every byte left after the closed semantic
census orange. That was not adequate evidence. The new guard reconciles every
main-executable byte against the executable inventory, canonical exact C,
reviewed semantic C, the full assembly manifest, and explicit non-code ranges:

| Category | Bytes |
| --- | ---: |
| Canonical exact C | 105,052 |
| Reviewed semantic C | 412,826 |
| Evidence-backed retained assembly/non-code | 30,486 |
| Unaccounted | 0 |
| **Main executable** | **548,364** |

The audit added semantic C for the two 56-byte noncanonical claims and for the
16-byte and 40-byte fixed-point trigonometry helpers. It explicitly records 980
bytes of non-code lookup-table, literal-pool, and alignment spans, including the
512-byte quarter-sine table at `0x08002344`. The remaining 29,506 bytes are
named ABI-special, veneer, relocated runtime, helper, or deliberate low-level
assembly structures. This includes the 932-byte mixed Thumb/ARM IWRAM audio
mixer at `0x080f9674`; its permanent disposition is defensible under the
current explicit `keep_asm` policy, although its manifest confidence remains
`strong`, not a claim that every possible compiler has been disproved.

`bun run core:retained:check` is now part of the full verification gate. The
dashboard uses natural source/owner boundaries instead of artificial roughly
equal blocks and reserves orange for evidence-backed retained assembly or
explicit non-code data.

### 0807 main-image tranche

The `0x08070000–0x0807ffff` bank is independently closed for ownership: its
14,376 executable bytes partition into 8,096 exact C bytes, 5,432 reviewed
semantic-C bytes, and 848 evidence-backed structural bytes. The 848 bytes are
800 bytes of fixed linker veneers (`ldr`/`bx` stubs) and 48 bytes of explicit
two-byte alignment fillers; there is no ordinary or unowned assembly in this
bank. The semantic owners are therefore an exact-C backlog, not an inventory
gap. Bounded compiler/source probes found no exact route yet; the strongest
source-grounded floors currently are `0807905c` at 310 differing halfwords and
`08079460` at 173 under a routed experimental pair. Those probes remain
unpromoted until a canonical source reproduces the reference bytes.

## Code-overlay audit

The 96 decoded code overlays are the largest open program scope:

- Executable inventory: **795,106 bytes**.
- Exact C: **161,780 bytes**.
- Additional semantic C: **420,382 bytes**.
- Remaining without exact or semantic C: **212,944 bytes**.
- Current semantic-backed exact reading list: **539 owners / 274,540 bytes**
  across 67 code overlays.

The largest reading-list portfolios are `resource_373` (16,440 bytes),
`resource_3bf` (12,596), `resource_3c8` (10,194), `resource_372` (9,400),
`resource_38f` (8,988), `resource_371` (8,894), and `resource_383` (8,236).

Discovery is broad but noisy: the live inventory reports 11,954 unconverted
discoveries, including 10,335 contained rows, 9,987 data walks, 126 structural
veneers, 928 ordinary discoveries, and 477 ordinary prologue/return rows.
These filters overlap; their counts must never be added as distinct functions.

The exact-twin report now shows only **116 theoretical recoverable bytes** in
two families; `bun tools/overlay_twins.ts --semantic --unconverted` reports
zero, proving there is no remaining known twin-template shortcut for semantic
closure. The new semantic mode prevents the exact-C queue from being mistaken
for unowned code and makes that negative result immediate rather than manual.

`bun tools/overlay_call_order_check.ts` is the other current speedup. It compares
each semantic owner's source-level postorder call sequence with its reachable,
veneer-resolved BL sequence, including conservative IWRAM call-through recovery.
Use it on every new owner alongside the multiset check: all four new
`resource_3bd` owners pass both. The latest closure also admits the 6,220-byte
`resource_3bd:13f8`, the 4,080-byte `resource_378:088c`, four adjacent
`resource_383` owners totalling 2,824 bytes, and `resource_3b1:1894` at 228
bytes. Each passes the targeted multiset and ordered-call checks. The
project-wide diagnostic currently reports 1,108 owners, 873 passes and 235
pre-existing mismatches, so `--all` is an audit queue rather than a green gate;
the self-test is wired into `bun test`.

## Semantic C: what it did and did not unlock

Semantic C is valuable for completeness, review, type/ABI discovery, call-graph
work, and selecting exact candidates. It is not a byte-match accelerator on its
own. Closing the main-executable semantic census did not change the exact
headline because the remaining work is dominated by register allocation,
scheduling, literal-pool placement, compiler vintage, private ABI contracts,
and module boundary/linkage details.

Use semantic C as an input corpus, not a second finish line:

1. Promote a complete semantic owner only after exact byte verification.
2. Remove its semantic source/registry row immediately after promotion.
3. Keep exact and semantic spans disjoint in reporting.
4. Prefer humanization only where names, types, structures, or cross-file ABIs
   directly expose an exact-source family.

## Compiler audit

Alchemy currently routes among the pinned GCC 2.96 baseline, the older
`old_agbcc` family, and experimental early-Thumb/GCC 2.95.1 routes derived from
acceptable public pret evidence. Non-2.96 routes are GS1-only and every routed
binary is digest/smoke checked.

The wrapper self-test currently reports 46 older-compiler sources, 44 grouped
DMA sources, and 29 code-overlay call-argument sources. The mode/cohort tools use
content-addressed cache keys; the local compiler corpus cache contains 25,980
entries. Direct compiler invocation, reference-once assembly, and caching make
trials materially cheaper, but they do not improve convergence probability.

Measured conclusions:

- A 22-candidate stock configuration matrix found no shared new exact route.
- Broad compiler-option exploration is saturated unless multiple unrelated
  residuals point to one mechanism.
- The hard problems are vintage-specific code generation, register allocation,
  scheduling, literal pools, code-overlay link bases, switch tables, and an expanding
  set of source-scoped route exceptions.
- Compiler-family and mode work is worthwhile only when it improves several
  unrelated owners or precisely explains one reusable structural family.

The sibling `alchemy-gcc` repository has one approved, default-off mode in
`df79270`: `-fthumb-postcall-byte-increment-r2`. It was independently derived
from the `08098b10` reference witness, is routed only to that GS1 main-image
source, and uses the rebuilt Darwin arm64 `cc1` hash
`d12bf2c7b96d2b1b6cec4c09b76f986249285070b1ca09d1ba1baf31b859cc18`.
The full exact corpus regression is clean (`1424/1424`, zero regressions), and
the source-only and full-ROM verification gates pass with zero fallback bytes.
Keep the mode source-scoped and experimental until another unrelated owner
demonstrates the same mechanism; do not broaden it from this single witness.

The clean witness `080907b0` is now exact C (116/116 bytes). Its natural
volatile DMA source leaves an independent destination setup between the first
two scalar stores, uses the live stack pointer as the first descriptor value,
and needs a strict second-descriptor order. The new default-off
`-fthumb-group-value1-before-base` mode admits only that grouped-DMA shape and
restores the reference's immediate/address/base/shift/control order. It is
routed only to GS1 source `080907b0`, alongside
`-mgrouped-dma-store`, `-fthumb-group-control-last`, and
`-fno-sched-depend-count`. The existing compiler regression suite passes, as
does the 0809 routed cohort check (228/228 exact, zero regressions) and the
explicit gcc296 check for the owner (1/1 exact, zero regressions). The rebuilt
Darwin arm64 `cc1` hash is
`9ebef7d0fac03bbd44ce3016b8e06534cde5ef514b29042be9dcbf9414f248ff`.

Compiler policy going forward:

1. Exhaust natural source shape, sibling evidence, and existing routed families
   before proposing a backend change.
2. Require a sampled exact-corpus regression for any wrapper/routing change and
   the full corpus before enabling a new general mode.
3. Record route evidence in a machine-readable registry so path-specific flags
   cannot silently spread to a same-address function in another code overlay.
4. Keep experimental compiler modes default-off and source-scoped until shared
   evidence justifies anything broader.

## Permuter audit

The permuter is a bounded rescue tool, not the main work engine.

- Main-image state: 531 saved targets; current best-score distribution is
  approximately minimum 2, median 64, maximum 395, with four saved recipes.
- Proven exact yield is only a handful of owners: three from the newer annealer,
  one subsequent rescue, and two from the older Python bridge.
- A measured code-overlay run evaluated 65,543 candidates across 15 targets and
  produced one 92-byte exact match; no other target's floor improved.
- The current code-overlay floor set has 11 targets, minimum 4, median 17, maximum
  136, and no saved exact recipes.
- The broad code-overlay matcher measured 300 targets × 527 configurations and found
  seven exact results. That is useful harvesting, not a reason to run an
  unbounded overnight job.

Operating rule: use the permuter only on fresh, independently bounded owners—
normally at most 96 bytes—or on a cohort with a specific shared hypothesis.
Record the starting floor, configurations/evaluations, wall time, best floor,
and stopping condition. Never manually source-permute an already documented
near-match, never promote a merely close result, and never run the retired
overnight queue without new evidence.

Latest bounded probes (2026-08-02) reinforce that rule:

- Fresh main-image `08077428` (2,024-byte owner): a bounded 74-mode plus
  third/fourth-flag sweep reached the exact extent with 153 differing
  halfwords under `-Os -fno-gcse -fno-strict-aliasing`; the first residual is
  the fixed-point mask temporary's register shape. A separate typed-pair
  structural matrix reached 1,988 bytes / 840 differing halfwords, but no exact
  route emerged.
- Fresh main-image `08093c00` (552-byte owner): 73 compiler configurations
  found an exact-size `-O1` result but still 256 differing halfwords. Its first
  residual is loop rotation and frame/index materialisation, so it was parked.
- A bounded 1,800-step, three-restart permuter run on fresh `0807a7a0`
  improved the saved score to 84 differing bytes but found no exact result.
  The candidate remains scratch state; no approximate source was promoted.
- Two source-grounded semantic corrections in the `0807` bank remain
  semantic-only: `0807a7a0` now writes the final snapshot tail at `+0x1fa`
  (not `+0x1d0`), and `08078bf0` now walks its 32 effect slots at their proven
  four-byte stride. Neither correction is an exact-C promotion; both compile
  cleanly and leave the exact-byte metric unchanged.
- The standalone 40-byte `08079390` leaf remains non-exact after bounded
  compiler-family sweeps. `asm/080770d8.s` is a linker veneer whose Thumb
  target is `0x08079391`, so this is an independently callable compiler-output
  function—not a continuation row. Its semantic C has the correct behaviour
  and exact extent, but the original begins by preserving the argument in r4
  while current valid C schedules the address calculation first. Keep it a
  compiler/source-shape investigation; do not hand-permute it.
- Fresh main-image `08078ee8` (284-byte owner): the 329-configuration sweep
  reached 282 bytes and 98 differing halfwords with
  `-fno-rerun-cse-after-loop -fno-force-mem`; no exact route emerged.
- Fresh main-image `08079b24` (160-byte owner): the sweep reached the exact
  160-byte extent and 46 differing halfwords. A bounded 1,800-step,
  three-restart permuter run ended at weighted floor 148 with no exact match.
- Fresh main-image `0809509c` (194-byte owner): the sweep reached the exact
  194-byte extent and 38 differing halfwords with grouped-DMA scheduling.
  A bounded 1,800-step, three-restart permuter run ended at weighted floor 96;
  no exact match was found or promoted.
- A default-route scan of all 704 remaining semantic main-image owners found
  no directly adoptable full-module exact match. It did flag the first 2-byte
  leaf in continuation row `0801c9be` as exact in isolation, but its semantic
  file also owns two non-exact siblings; promoting only that leaf would require
  splitting the row's source/assembly boundaries, so it remains parked.
- Fresh standalone `080a524c` (316-byte owner): 329 compiler configurations
  held the exact extent with four differing halfwords, all one temporary's
  r2/r3 allocation. A bounded 1,800-step, three-restart permuter run ended at
  weighted floor 4 with no exact match.
- Fresh main-image `08079460` (412-byte owner): the expanded 701-configuration
  family/mode sweep and an independent compiler review found no exact route;
  the best result was 187 differing halfwords at the exact extent.
- Fresh main-image `0807905c` (616-byte owner): the automated declaration and
  statement-order sweep evaluated 720 probes without improving its 316-
  halfword floor. The expanded 701-configuration compiler sweep reached 302
  halfwords at 632 bytes, so it was not promoted.
- Main-image `08077394` (68-byte owner): the full 701-configuration sweep,
  quick finish-draft enumerator, and bounded 1,800-step/three-restart permuter
  found no exact result; the baseline remains 27 differing halfwords at the
  exact extent. Main-image `0807a664` likewise found no route: the quick
  enumerator reached 151 differing halfwords at 316 bytes and its bounded
  permuter did not improve the saved floor.
- Fresh main-image `0807a664` was independently reconstructed to a verified
  31-halfword floor at the exact 316-byte extent after automated source-shape
  and compiler sweeps. A narrowly source-gated GCC 2.96 post-reload mode then
  reproduced its compaction/fill register lifetime and final table-load order;
  the clean C source is now exact and the former semantic/assembly pair was
  promoted to `src/0807a664.c`.
- Fresh main-image `080799b0` reached an exact-size 296-byte source with a
  129-halfword floor. A bounded 1,800-step, three-restart permuter run found
  no exact result.
- Fresh main-image `080907b0` moved from a 23-halfword natural-source floor to
  an exact 116-byte owner through the strict grouped-DMA compiler route above.
- Fresh main-image `08011568` (40-byte DMA leaf): grouped-DMA plus
  `-mentry-low-register-order` held the exact extent at an 11-halfword floor;
  48 natural source/configuration variants found no exact route.
- Fresh main-image `080c00d8` (88-byte fill owner): the automated mode sweep
  reached 25 differing halfwords with `-fno-cse-two-insn-immediate`, while 480
  declaration/loop shapes did not improve it; both targets remain scratch-only.

## Tooling audit

The repository has 161 top-level TypeScript tools. Its strongest guarantees are:

- complete source-only build and zero fallback;
- byte-identical ROM verification;
- audited executable inventories and independent exact ownership;
- content-addressed compiler/object caches;
- code-overlay adoption dry-runs and collision checks;
- clean-room publication/provenance scanning;
- semantic compilation and owner-scope checks.

Corrections made in this audit:

- semantic reporting now subtracts 22,310 out-of-executable bytes;
- noncanonical exact-match scaffolds no longer suppress semantic replacements;
- the main retained complement has an exhaustive verification gate;
- semantic supersession is checked live, not only by a self-test;
- dashboard main-image tiles follow natural ownership boundaries;
- the data-maturity graph uses all 7,298,755 data bytes;
- a same-address code-overlay compiler-route collision found by the existing
  lint was moved to explicit path-scoped routes.
- `overlay_gaps.ts --ranked` now puts one-return owner-shaped gaps in byte-yield
  order before multi-return residue. The first ranked cohort closed **16,816
  executable bytes across 16 new source owners** rather than spending a pass
  manually scanning every module report.
- the ordered-call verifier now follows bounded resolver evidence past a proven
  jump-table dispatch instead of silently auditing only the pre-table prefix.
- the coverage map now preserves the executable inventory's proven code-overlay
  veneer and alignment classifications. **43,748 overlay bytes** that were
  previously painted gray as Unknown are now correctly orange retained exact
  assembly; the repeated similarly sized blocks are fixed entry/import banks,
  not suspicious identical unknown functions.

Remaining tool debt:

- `remaining_survey.ts` reports only the 596 `c_candidate` rows and omits the 96
  split/merge rows / 38,440 bytes of structural exactness debt.
- Code-overlay discovery's 12,046-row output needs a non-overlapping, confidence-
  tiered queue instead of headline discovery counts.
- Route evidence still lives largely in hand-maintained allowlists and comments.
- Full compiler-corpus regression exists but is not a mandatory CI gate.
- Git hooks protect publication only after the contributor explicitly enables
  them; hook configuration is not itself verification.
- Exact and semantic dashboard ownership update directly from tracked files.
  Main-image orange ownership uses the verified full-build assembly manifest;
  code-overlay orange ownership uses the audited executable inventory's
  `veneer` and `executable_alignment` intervals. Retained-classification changes
  therefore remain evidence-backed rather than inferred from a complement.

## Data/assets audit

The ROM image is fully source-owned, but data maturity is not uniformly high.

- Tracked asset tree: 13,395 files, about 91 MiB in working form.
- Build manifest: 2,431 asset outputs / 7,840,244 ROM bytes, including compressed
  program streams and non-code data.
- Data-only ROM category: 7,300,700 bytes.
- The maturity catalogue classifies 3,857,517 bytes by representation tier and
  now shows the remaining **3,443,183 bytes** explicitly at the byte-represented
  floor instead of stretching the catalogue to fill the full graph.
- Asset manifest: 112 regions and 18 series.
- Five closure packages remain `availability: pending`: `header_edition`,
  `early_runtime`, `late_runtime`, `mixed_gap_data`, and `residual_pcm_waves`.

Data work improves reconstruction quality but should not displace GS1 exact-C
delivery unless it blocks executable ownership, a code-overlay boundary, or a
required ABI/table interpretation.

## Targets and provenance

Only two build targets are wired: `gs1-en` and `gs2-en`. GS2 currently has a
single four-byte C bootstrap and an incomplete executable inventory. The other
ten intended editions are evidence inputs, not implemented build targets. Do
not describe this as a working twelve-edition architecture.

GS1 English is the only target with a publishable Full-C percentage. The clean
boundary remains: supplied ROMs are private verification inputs; published
source must pass the repository's provenance rules; compiler evidence must be
derived from acceptable public sources such as pret projects and documented so
that derivation is reproducible. Do not import or rely on provenance-problematic
SDK/leak material.

## Metric defect before “100% C”

The present denominator intentionally includes executable literal pools,
veneers, relocated runtime, private-ABI helpers, and retained assembly. With the
current evidence-backed **74,234-byte** retained classification, even converting
every other currently measured byte would cap the all-executable exact-C
headline at **94.47%**; the main executable alone is capped at **94.44%**. The
code-overlay figure presently covers proven veneer/alignment intervals, not yet
an exhaustive retained audit, so that mathematical ceiling may fall further.

Do not silently move those bytes out of the denominator to reach 100%. Preserve
the current exact-C/all-executable series for historical continuity, then add a
second conventional metric only after a per-byte retained audit of both the
main executable and all code overlays:

- **Exact C / all audited executable bytes** — current headline and history.
- **Exact C / C-eligible executable bytes** — future completion metric, with
  every excluded span carrying explicit retained/non-code evidence.

The project can call itself “100% C-eligible exact” only when the second metric
has a complete denominator audit and reaches 100%; it should never call the ROM
image literally assembly-free while retained structures remain.

## Delivery speed and what has been falsified

At the current fixed denominator, first-parent exact ownership ended 2026-07-29
at 147,614 bytes, 2026-07-30 at 211,626 (+64,012), and 2026-07-31 at 232,944
(+21,318). The current checkpoint is 266,832; the latest tranche added the
124-byte code-overlay owner `resource_3a7:0944` using distinct veneer aliases,
explicit delta dataflow, a labeled loop tail, and a source-scoped `-O3` route,
after the 316-byte owner `0807a664` and the earlier clean, source-scoped
witnesses `08077f70`, `08098b10`, `080907b0`, and `08011590`.
This is a real gain,
but not exponential acceleration; no kilobyte-scale identical large-owner
family was found.

Falsified or bounded hypotheses:

- Full semantic C alone does not accelerate exact C.
- More subagents do not help when they collide on the same compiler floor or
  lack independently owned targets.
- Large humanization batches improve readability but do not move exact bytes.
- Broad compiler flag sweeps have not found a shared stock configuration.
- Long permuter runs have extremely low exact yield.
- Twin byte identity is useful, but link address, pools, veneers, and ABI can
  prevent source transposition.

## Prioritized forward plan

### Phase 0 — seal this checkpoint

1. Run `bun run verify` and `bun run coverage:check`.
2. Commit and push each coherent exact-C tranche and its generated metrics.
3. Regenerate and commit the first-parent history after the checkpoint commit.

### Phase 1 — reach 20% exact C honestly

The target is **+1,850 exact bytes**. Treat it as a portfolio, not one heroic
function:

1. Work the 537-owner semantic-backed code-overlay reading list in descending
   byte value, beginning with the top eight code overlays listed above.
2. Give each worker a different code overlay and owner and require an exact adoption
   dry-run before integration.
3. Resolve code-overlay-local imports, pools, link base, and sibling compiler route
   before trying source permutations.
4. Harvest exact twins only where the receiving code overlay verifies independently;
   do not budget the 1,804-byte theoretical twin total as guaranteed output.
5. In parallel, use one lane on fresh 81–320-instruction main-image owners from
   a proven compiler/source family. Avoid already documented floors.
6. Integrate in roughly 4–8 KiB verified tranches, refresh the byte metric, and
   stop/rebalance a lane after two consecutive zero-yield tranches.

### Phase 2 — make exact delivery cheaper

1. Extend `remaining_survey.ts` to include all five C-debt retentions and emit a
   non-overlapping owner queue.
2. Turn code-overlay discovery into ordinary / contained / data-walk / veneer
   confidence tiers with byte totals that reconcile to the inventory.
3. Build a machine-readable compiler-route evidence registry and lint every
   address-only code-overlay route for namespace collisions.
4. Add sampled exact-corpus regression to normal compiler-wrapper changes.
5. Run bounded compiler/permuter cohorts only when at least two unrelated
   residuals predict the same mechanism.

### Phase 3 — define the real completion denominator

1. Complete the retained/non-code per-byte audit across all 96 code overlays;
   inventory-proven veneer/alignment bytes are now classified, but every other
   structural residue still needs the same exhaustive proof used by the main
   executable.
2. Review every retained main-image span whose confidence is below `proven`,
   especially the mixed-mode audio mixer.
3. Publish the C-eligible metric beside, never in place of, the historical
   all-executable metric.
4. Only then forecast the remaining work to 100% C-eligible exactness.

### Phase 4 — broaden reconstruction after the GS1 exact bottleneck

1. Close the five pending data packages where they unblock code or ownership.
2. Complete the GS2 executable inventory before publishing any GS2 percentage.
3. Wire further editions one at a time with the same zero-fallback and
   provenance gates.

## Operational gates

```sh
bun run progress:check
bun run build:semantic
bun run semantic:check
bun run core:retained:check
bun run coverage:check
bun run verify
```

The complete gate must finish with a byte-identical ROM image, zero fallback,
canonical exact-C ownership, no semantic supersession, a reconciled retained
main-image complement, and current tracked metrics.

Historical evidence indexes:

- `docs/PATH-TO-COMPLETION.md` — historical delivery/compiler cohorts.
- `docs/DISCOVERY-QUEUE.md` — historical main-image/code-overlay queue audit.
- `docs/COMPILER-QUEUE.md` — historical compiler experiments and stop rules.
- `docs/full-c-history.md` — first-parent exact-C ledger index.
- `LAWS.md` — source/compiler reconstruction laws.
- `PROVENANCE.md` — clean-room and publication boundary.
