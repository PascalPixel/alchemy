# Decompilation playbook

This is the operating guide for converting Alchemy's remaining reconstructed
assembly into byte-exact C. It optimizes verified conversions per wall-clock
hour. The evidence, clean-room, build, and publication rules in
[AGENTS.md](AGENTS.md), [STATUS.md](STATUS.md), and
[PUBLICATION.md](PUBLICATION.md) take precedence.

## Evidence boundary

All Golden Sun-specific facts must be derived independently from the approved
ROM set and this repository's clean-room outputs. Never inspect or accept code,
symbols, labels, pseudocode, assembly patterns, scripts, outputs, or notes from
another Golden Sun checkout.

External advice may contribute game-agnostic process ideas only. Treat every
suggested compiler behavior, source shape, function boundary, name, type, or
game-specific claim as an untrusted hypothesis until it is independently
supported by approved evidence. Do not put an unverified hypothesis into source
or documentation.

Generic public tools and public architecture or compiler documentation remain
allowed, exactly as AGENTS.md rule 2 states. The boundary excludes game
knowledge from other Golden Sun work, not generic methodology: clean-room
caution is not a reason to delete legitimate generic tooling, and tooling is
not a cover for importing game facts.

Never send ROM bytes, ROM-derived assembly, private analysis, toolchains, or
build products to a network service. All matching work stays local.

## Definition of done

An ordinary assembly region is decompiled only when asm-free C emits the exact
linked bytes and relocations at its fixed address. Inline assembly, register
pinning, and compiler barriers do not count as C conversion.

The immediate English byte-closure milestone requires a source-only,
byte-identical `gs1-en.gba` build with no copied-ROM fallback. Full
decompilation additionally requires that no ordinary `asm/` C-debt remains;
only positively justified compiler-unproducible stubs may remain there. Project
completion is the larger twelve-ROM and repository-quality milestone defined in
`STATUS.md`.

## Operating rules

1. **Work the queue, not the address order.** Rank by exact reuse, structural
   family leverage, candidate closeness, type confidence, complexity, and
   expected conversions per unit time.
2. **Run batch-first.** Batch means whole-queue waves and cohort levers, not a
   fixed lane preference. Clear `reuse` and `family` members whenever they
   exist, but when the regenerated lane totals show the queue dominated by
   `research` singletons, the batch lever is context, draft normalization, and
   diagnosis-cohort work that lifts many singletons at once. A hard singleton
   must not block bulk delivery.
3. **Timebox hand work by kind.** Shape tuning against an existing draft stops
   after **10 minutes or three meaningful source variants**, whichever comes
   first. Semantics-first drafting, for functions the draft generator rejects
   (`M2C_ERROR`, `M2C_BITFIELD`, `M2C_MEMSET`, `M2C_MEMCPY`) or structurally
   mangles, is a separate activity with its own budget: up to **45 minutes**
   to produce a correct natural draft, which then enters the automated lanes
   instead of being tuned toward byte identity by hand. Either way, record the
   best result, diagnosis, and next hypothesis, then rotate.
4. **Prefer deterministic searches.** Search declarations, signedness,
   parameter types, expression order, temporary lifetime, common-subexpression
   boundaries, control-flow shape, and supported compiler modes before using an
   expensive permutation search.
5. **Solve families through representatives.** Normalize remaining assembly,
   group related regions, solve one representative, then independently compile
   and verify the source shape against every member. When structural families
   are exhausted, group by diagnosis signature instead (register-rotation,
   epilogue-shape, scheduler-hoist, entry-register classes) and design one
   lever per cohort; a cohort sharing one gate falls like a family.
6. **Turn discoveries into leverage.** A hand-won match should leave a
   generalized operator, template, type constraint, boundary rule, or concise
   independently evidenced note when the lesson is reusable.
7. **Measure results, not activity.** Optimize exact C regions and executable
   bytes per wall-clock hour, not commits or compiler invocations.
8. **Commit verified batches about every 10 minutes when matches exist.** Do
   not manufacture a commit when there is no verified conversion.

## Authoritative measurements

Regenerate reports rather than copying a stale snapshot into this document.

```sh
bun tools/build_full.ts --source-only
bun tools/decomp_queue.ts --refresh --jobs 16 --limit 400 \
  --targets-out out/decomp/targets.txt
bun tools/decomp_families.ts --top 10
bun tools/overlay_inventory.ts --top 10
```

Report at least:

- converted C region count and exact-C executable bytes;
- remaining ordinary assembly regions and bytes;
- retained structural assembly regions and bytes;
- candidates attempted and exact matches by lane;
- dominant-mismatch-class mix of the diagnosed failures (context improvements
  must shrink the semantic share; normalization must shrink the register-only
  share);
- compiler invocations and cache-hit rate when available;
- median wall-clock time per successful conversion;
- full-build identity and fallback-byte count.

If a lane produces no match within its declared budget, stop it and record the
failure mode. Do not silently continue a low-yield loop.

Plan against the C-debt regions and bytes in the families report, never
against the byte-reconstruction percentage: that percentage includes assets
and retained structural assembly and does not forecast decompilation time.

## Work lanes

### Reuse

Use when an existing candidate or verified C template is already close. Try
type and declaration constraints first, followed by a small deterministic
variant set. Verify the whole region, not only its instruction mnemonic shape.

### Family

Use when multiple regions share an exact or normalized structural fingerprint.
Choose the simplest representative. Keep family parameters explicit: constants,
callees, data symbols, offsets, widths, and signedness. A family is complete
only after each member independently passes exact compilation and linking.

### Module and boundary

Use when a mismatch suggests shared literal pools, alignment ownership,
neighboring functions compiled as one translation unit, embedded data, or a
multi-region function. Establish region ownership before source-shape search.
Use `tools/decomp_module.ts` for justified adjacent-region experiments. Never
decompile through an unresolved data island.

### Research

Use for large, unique, or unresolved-control-flow functions after higher-yield
lanes have work in flight. Recover semantics and types first. Keep experiments
bounded by the same 10-minute/three-variant rule unless the work is explicitly
a reusable tool or cohort investigation with a measurable acceptance test.

## Standard ordinary-region wave

Run this sequence after a material improvement to types, context, source
normalization, variant operators, or compiler-mode evidence.

```sh
# 1. Verify ownership and regenerate the ranked debt reports.
bun tools/build_full.ts --source-only
bun tools/decomp_families.ts --top 10

# 2. Refresh independently derived type and context evidence.
bun tools/track_bases.ts
bun tools/mine_structs.ts
bun tools/m2c_context.ts

# 3. Draft the whole queue. Drafting is cheap; bound the search stages,
#    not the drafting, or the large regions that hold most of the remaining
#    bytes never receive a draft at all.
bun tools/decompile_batch.ts --context work/m2c_context.c \
  --max-bytes 8192 --jobs 16 -o work/m2c-ctx roms/gs1-en.gba

# 4. Perform the cheap exact sweep.
bun tools/match_m2c.ts --jobs 16 roms/gs1-en.gba work/m2c-ctx

# 5. Diagnose and rerank failures.
bun tools/decomp_queue.ts --refresh --jobs 16 --limit 400 \
  --targets-out out/decomp/targets.txt

# 6. Search bounded source variants and evidenced compiler modes.
bun tools/search_queue_variants.ts --limit 100 --variants 128 --jobs 16
bun tools/search_compiler_modes.ts --limit 100 --jobs 16

# 7. Use permutation only on a bounded, diagnosed target set.
bun tools/permute_v1.ts --targets out/decomp/targets.txt \
  --steps 5000 --restarts 8 --jobs 16
```

The numeric limits are starting budgets, not promises. Reduce them when a lane
is unproductive; increase them only when recent results justify the cost.

`search_queue_variants.ts` also reads the optional ignored
`work/decomp-seeds.json` manifest. Its `seeds` object maps an eight-digit entry
stem to one path or an array of paths containing semantics-reviewed hand
candidates. The queue draft remains a seed too. This is the feedback bridge
from hand diagnosis into deterministic search; do not list a candidate merely
because it has a low byte-mismatch score. Use `--no-seeds` for a queue-only
control run or `--seeds FILE` for another manifest.

Prefer permutation targets that are the smallest independently verifiable
whole function or block preserving the mismatch. Small inputs improve compile
throughput and shrink the mutation space, but do not repeatedly permute a
register-only or scheduler-only plateau: reducing such a function can remove
the live ranges that produce the reference allocation. After a bounded run,
route those plateaus to compiler trace diagnosis.

**Overnight annealing is a recorded failure mode, not a default.** Repeated
unattended `decomp_overnight.ts` campaigns have spent 8–9 hours and produced
zero exact matches (Pascal, 2026-07-23): the remaining queue is dominated by
scheduler/allocation plateaus that statement-level mutation cannot reach at
any step count — a 40,000-step × 8-restart run floors exactly where a 3,000-step
run does. Short, bounded permuter attempts (minutes) followed by diagnosis of
the floor — mismatch class, pass-dump evidence, one recorded hypothesis — beat
unattended hours every time so far. Do not start an unattended overnight
campaign unless a specific recent result (new context, new operator, new
compiler mode) justifies re-sweeping the queue, and say which result that is.

For genuinely unattended hours, `bun tools/decomp_overnight.ts` remains
available (resumable, checkpointed waves). It references optional stages that
do not exist yet (`cfg_extract.ts`, `synthesize_expr.ts`, `synthesize_block.ts`,
`mine_blocks.ts`): build them or delete the references. A campaign script must
not imply capacity that is not there.

## Overlay wave

Overlay code has independent load addresses and boundary concerns. Do not feed
it blindly through the ordinary fixed-image pipeline.

Overlay conversions also sit outside the `[N of M]` commit counter, which
counts main-ROM C-target regions (`tools/check_commit_progress.ts`). Track
overlay progress separately, or widen the denominator in the same commit that
first counts overlay work; never let the primary metric drift silently.

```sh
# Inventory executable entries, boundaries, and structural families.
bun tools/overlay_inventory.ts --top 10

# Generate and verify bounded direct candidates.
bun tools/overlay_match.ts --limit 100 --max-bytes 512 \
  --jobs 16 --variants 16

# Compare overlay instruction shapes with the verified exact-C corpus.
bun tools/compiler_template_index.ts --jobs 16 --top 20
bun tools/overlay_template_match.ts
```

The inventory's `unconverted_discoveries` count is deliberately not a
function-debt denominator: conservative control-flow discovery retains
veneers, shared helper-table slots, overlapping literal entries, and other
structural leads. `ordinary_unconverted_functions` is the high-confidence
bounded search queue, but it can still contain shared-tail or internal-label
entries and is not exhaustive of functions with unresolved control flow or
jump tables. Report converted functions separately until every remaining
discovery has an explicit C/structural/data classification; never add either
automatic queue count to the converted count and call the result function
coverage.

An instruction-shape template hit is a lead, not a conversion. Relocated bytes,
literal ownership, call targets, data symbols, and the entire claimed extent
must still match exactly.

## Diagnosis and routing

Use `tools/decomp_queue.ts` and the compiler diff to choose the next action.

| Symptom | First response |
|---|---|
| Semantic or type mismatch | Recover parameter, return, pointee, width, and signedness constraints; regenerate the draft. |
| Correct operations, wrong registers | Minimize live temporaries; search declaration and expression-lifetime variants. |
| Instruction reordering | Search statement order and supported compiler modes with module-level evidence. |
| Branch or loop mismatch | Search a bounded library of equivalent loop, early-return, condition-polarity, and short-circuit shapes. |
| Literal or address mismatch | Audit boundary and pool ownership; route to the module lane. |
| Missing or extra instruction | Check expression decomposition, CSE boundaries, promotions, dead stores, and function extent. |
| Matching prefix with size mismatch | Recheck boundary, alignment tail, literals, and embedded data before tuning C. |

Compiler flags describe an original translation unit; they are not arbitrary
per-function matching knobs. Accept a flag only with independently reproduced
module-level or ABI evidence, and document that evidence beside its build-rule
implementation.

## Law ledger

Confirmed compiler behavior is a compounding asset; sessions must not relearn
it. Maintain a tracked `LAWS.md` with one entry per law: the assembly
fingerprint, the producing C idiom, one in-repo example (address and source
file), the evidence source, and the confirmation date. A law enters the ledger
only with in-repo provenance (an installed match, a tracked document, or an
evidenced build rule). Externally suggested compiler lore goes in a separate
hypothesis section until reproduced locally. Operating rule 6 has no force
without this durable home, and every hand-won match must end with a ledger
append or an operator/tool improvement.

Seed entries with in-repo provenance today:

- **Minimal live-variable form** reproduces the original allocation; each
  extra named temporary rotates the allocator. Confirmed by installed
  counter-family matches (`src/08079338.c`, `src/08079358.c`). Because the
  draft generator names every intermediate, temporary elimination to fixpoint
  is a normalization pass, not a search dimension.
- **Dual-use pre-read**: a fresh-destination shift pair appears when the
  shifted value has a second read before the shift; single-use forms fold in
  place. Confirmed during the same counter-family investigation.
- **Cast-literal table access** (`((s32 *)ADDR)[i]`) reproduces add-then-load
  addressing that array declarations do not. Structure confirmed at the still
  unconverted `080fb670`; treat the register floor there as a separate open
  gate.
- **Prologue `lr` rule**: the approved compiler saves `lr` in any Thumb
  function containing a branch, even a leaf ([ASSEMBLY.md](ASSEMBLY.md)). An
  internal branch without an `lr` save is reclassification evidence, not
  C debt.
- **Register-reservation flags are module declarations**, accepted only with
  bundle-level evidence and a comment, as implemented for the fixed-register
  handler bundle in `tools/alchemy_gcc.ts`.

## Hand-session protocol

Before starting, write down the candidate, diagnosis, expected experiment, and
stop condition. Prefer a natural, semantics-first draft with few live locals to
incrementally contorting generated C.

During the session:

1. Verify the function extent and instruction mode.
2. Recover observable behavior, calls, memory widths, signedness, and likely
   parameter/return constraints.
3. Compile a baseline and preserve its diff score.
4. Try at most three source changes that test distinct hypotheses.
5. Stop immediately on an exact match, or park at the time/variant limit.

A meaningful variant tests a different causal hypothesis. Cosmetic rewrites or
repeated random mutations count against the budget when they consume attention.

The parking record belongs under an ignored analysis directory and should
contain no raw ROM excerpt. Record:

- target and claimed extent;
- best candidate path and score;
- dominant mismatch class;
- variants tried and what each disproved;
- suspected lane or family;
- cheapest next experiment.

## Integration and commit gate

Integrate only exact matches. `tools/integrate_matches.ts` performs guarded
replacement from a match directory; review its proposed changes before using
`--apply`.

```sh
bun tools/integrate_matches.ts <matches-dir>
bun tools/integrate_matches.ts --apply <matches-dir>
bun tools/build_full.ts --source-only
bun tools/build_full.ts
git diff --check
git add <intended-source-and-manifest-files>
bun tools/check_publication.ts --staged
git diff --cached --name-status
```

Before committing, confirm:

- exact bytes and relocations pass at the linked address;
- no inline assembly, register pinning, or barrier was introduced;
- only canonical publishable inputs are staged;
- manifests and classifications agree with source ownership;
- the full source-only build is byte-identical with zero fallback bytes;
- the commit subject ends with the current `[N of M]` counter.

Commit a coherent verified batch as soon as the approximately ten-minute commit
interval arrives. If a long search produces no matches, report the attempt and
rotate; do not weaken the verification gate to preserve cadence.

## Tool-investment priorities

Prefer improvements that reduce work across many candidates:

1. authoritative debt and family classification;
2. type/signature constraint propagation;
3. generated-draft normalization and temporary-lifetime reduction;
4. permuter operator coverage: audit `permute_v1.ts` against the generic
   operator families public permuters document (temporary introduction and
   elimination, statement reordering, type flips, control-shape rewrites) and
   port what is missing, benchmarked on a fixed target set by matches per
   CPU-hour;
5. nearest matched-pair retrieval: given a target's normalized fingerprint,
   surface the closest installed matches and their C beside every dossier and
   hand session;
6. exact-C corpus templates with safe symbol parameterization;
7. literal-pool, alignment, and function-boundary ownership;
8. mismatch-block-focused deterministic mutations;
9. compiler-result caching and batch verification (a persistent compile server
   only if profiling shows process spawn dominating a search loop);
10. automatic, atomic integration and generated progress reporting.

Every tool spike needs a measurable acceptance test, such as more exact matches,
lower median mismatch, fewer compiler calls per success, or a resolved boundary
cohort. Park a tool idea that does not move its metric within the declared
budget.

## Endgame ordering

When the queue thins, sequence the last mile deliberately: split the mixed
code/data regions first (boundary debt precedes matching); merge proven
multi-region functions before attempting their C; clear module windows around
shared-pool walls; document every accepted flag set with its evidence; confirm
every surviving `asm/` file carries a positive structural justification in
`asm/classification.json`, where absence of a C match is never a
justification; then regenerate STATUS.md from the tools and re-verify the
full build.

## End-of-cycle checklist

- Regenerate the queue and family reports.
- Run cheap exact sweeps before expensive searches.
- Verify every candidate independently.
- Integrate and fully build each successful batch.
- Inspect and publication-check the staged paths.
- Commit verified progress at the requested cadence.
- Record throughput and parked failure modes.
- Start the next cycle in the highest-yield non-blocked lane.
