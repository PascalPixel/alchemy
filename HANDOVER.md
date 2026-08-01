# Alchemy project audit and forward plan

Audit checkpoint: 2026-08-01, `main`. This is the single authoritative current
handover. Historical studies remain useful evidence, but their counters and
queues are not live project status.

Verification status: the complete `bun run verify` gate and
`bun run coverage:check` both pass at this checkpoint. The normal build is
byte-identical, source ownership is complete, and ROM fallback is zero.

## Executive verdict

Alchemy is a complete, byte-identical, source-owned reconstruction of the
8 MiB GS1 English ROM image. It is **not** a fully decompiled C project.

- Exact Full-C Byte Share is **249,940 / 1,343,212 executable bytes
  (18.61%)**.
- Reviewed semantic C adds **816,630 executable bytes**. Exact plus semantic C
  covers **1,066,570 / 1,343,212 bytes (79.40%)**.
- The main executable has a closed semantic census: every ordinary owner has
  exact or reviewed semantic C. That closure has **not** made exact matching
  materially faster by itself.
- The current exact-C 20% threshold is 268,643 bytes. The remaining gap is
  **18,703 exact bytes**. The latest tranche promoted 37 code-overlay owners
  and added 4,896 counted exact bytes; one additional 88-byte inline-assembly
  match was deliberately restored to assembly because it is not canonical C.
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

| Scope | Exact C | Semantic C, excluding exact | Assembly / retained | Executable total |
| --- | ---: | ---: | ---: | ---: |
| Main executable | 102,208 | 415,670 | 30,486 retained | 548,364 |
| 96 decoded code overlays | 147,732 | 400,960 | 246,156 unresolved | 794,848 |
| **Total** | **249,940** | **816,630** | **276,642** | **1,343,212** |

Additional ROM-image facts:

- Total ROM image: **8,388,608 bytes**.
- Data/assets outside the executable denominator: **7,300,700 bytes**.
- Compressed code-overlay streams occupy **539,544 ROM bytes**; their decoded
  executable size is counted in the separate 794,848-byte code-overlay namespace.
- The source-only full build owns **8,388,608 / 8,388,608 bytes**, uses zero
  fallback, and reproduces the reference ROM byte for byte.
- Diagnostic source counts are 1,413 main-image C files, 1,795 exact code-overlay
  C files, 718 semantic main-image C files, 1,066 semantic code-overlay C files,
  and 41 headers. These are useful inventory counts, not progress percentages
  or reliable function counts.

The semantic compiler reviews 838,944 bytes of owner spans, but 22,314 of those
bytes lie outside the audited executable extents, chiefly pool/tail portions of
code-overlay owner spans. The coverage numerator therefore uses 821,520, not
843,840. `tools/build_semantic.ts` now reports both figures instead of silently
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

- Main executable: **102,208 / 548,364 (18.64%)**.
- Code overlays: **147,732 / 794,848 (18.59%)**.
- Combined: **249,940 / 1,343,212 (18.61%)**.

## Main-executable audit

The full-build assembly manifest contains 1,771 rows / 446,044 bytes:

| Manifest disposition | Rows | Bytes |
| --- | ---: | ---: |
| Ordinary `c_candidate` | 596 | 350,912 |
| Split/merge structural exactness debt | 96 | 38,440 |
| **C/exactness debt total** | **692** | **389,352** |
| Retained structural assembly | 1,079 | 56,692 |

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
| Canonical exact C | 102,208 |
| Reviewed semantic C | 415,670 |
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

## Code-overlay audit

The 96 decoded code overlays are the largest open program scope:

- Executable inventory: **794,842 bytes**.
- Exact C: **142,836 bytes**.
- Additional semantic C: **405,850 bytes**.
- Remaining without exact or semantic C: **246,156 bytes**.
- Current semantic-backed exact reading list: **587 owners / 289,384 bytes**
  across 66 code overlays.

The largest reading-list portfolios are `resource_373` (17,232 bytes),
`resource_3b8` (15,028), `resource_3bf` (12,808), `resource_3c8` (10,636),
`resource_372` (9,622), `resource_38f` (9,212), `resource_371` (8,894), and
`resource_383` (8,236).

Discovery is broad but noisy: the live inventory reports 12,046 unconverted
discoveries, including 10,353 contained rows, 10,000 data walks, 126 structural
veneers, 1,005 ordinary discoveries, and 529 ordinary prologue/return rows.
These filters overlap; their counts must never be added as distinct functions.

The exact-twin report shows only 1,804 theoretical recoverable bytes. Its three
remaining families have already demonstrated address-, pool-, veneer-, or
one-to-four-byte compiler differences, so 1,804 is an upper bound, not an easy
batch. During this audit, `resource_373:11d8` converted exactly for 108 bytes
after correcting code-overlay-local call targets and using its already-proven
scheduler route. That is the productive pattern: start from a complete owner,
resolve the module-local ABI correctly, and reuse a demonstrated family route.

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

The wrapper self-test currently reports 46 older-compiler sources, 41 grouped
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

The sibling `alchemy-gcc` repository currently has a seven-file uncommitted
`match0-keeps-input` experiment. It is **not approved, committed, installed, or
deployed**; `dist/` is unchanged. The older compiler binary was rebuilt, the
GCC 2.96 build remains out of date, and there are no dedicated fixtures or
corpus regressions. Preserve it as quarantined research only. Before adoption,
require minimal reproducers, stock/opt-in/opt-out assertions, a clean rebuild,
full exact-corpus regression, and a narrower mechanism if possible.

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

## Tooling audit

The repository has 157 top-level TypeScript tools. Its strongest guarantees are:

- complete source-only build and zero fallback;
- byte-identical ROM verification;
- audited executable inventories and independent exact ownership;
- content-addressed compiler/object caches;
- code-overlay adoption dry-runs and collision checks;
- clean-room publication/provenance scanning;
- semantic compilation and owner-scope checks.

Corrections made in this audit:

- semantic reporting now subtracts 22,320 out-of-executable bytes;
- noncanonical exact-match scaffolds no longer suppress semantic replacements;
- the main retained complement has an exhaustive verification gate;
- semantic supersession is checked live, not only by a self-test;
- dashboard main-image tiles follow natural ownership boundaries;
- the data-maturity graph uses all 7,300,700 data bytes;
- a same-address code-overlay compiler-route collision found by the existing
  lint was moved to explicit path-scoped routes.

Remaining tool debt:

- `remaining_survey.ts` reports only the 596 `c_candidate` rows and omits the 96
  split/merge rows / 38,440 bytes of structural exactness debt.
- Code-overlay discovery's 12,046-row output needs a non-overlapping, confidence-
  tiered queue instead of headline discovery counts.
- Route evidence still lives largely in hand-maintained allowlists and comments.
- Full compiler-corpus regression exists but is not a mandatory CI gate.
- Git hooks protect publication only after the contributor explicitly enables
  them; hook configuration is not itself verification.
- Exact and semantic dashboard ownership updates directly from tracked files,
  but orange retained ownership uses the latest full-build assembly manifest;
  retained-classification changes therefore require a completed full build.

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
current 30,486-byte main-image retained classification, even converting every
other currently measured byte would cap the project headline at **97.73%** and
the main executable at **94.44%**. A future code-overlay retained audit may lower
that mathematical ceiling further.

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
(+21,318). The current checkpoint is 249,940 (+16,996 since the prior day-end).
The latest 4,896-byte tranche came primarily from exact-source sibling
transposition: a 164-byte shape expanded across twelve modules, followed by
smaller structural families. This is a real multiplier, but not exponential
acceleration; no kilobyte-scale identical large-owner family was found.

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

The target is **+18,703 exact bytes**. Treat it as a portfolio, not one heroic
function:

1. Work the 551-owner semantic-backed code-overlay reading list in descending
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

1. Perform the same retained/non-code per-byte audit across all 96 code overlays
   that now exists for the main executable.
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
