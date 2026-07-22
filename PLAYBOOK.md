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
2. **Run batch-first.** Spend most effort on `reuse` and `family` lanes. A hard
   singleton must not block bulk delivery.
3. **Timebox hand tuning.** Stop after **10 minutes or three meaningful source
   variants**, whichever comes first. Record the best result, diagnosis, and
   next hypothesis, then rotate to another family or lane.
4. **Prefer deterministic searches.** Search declarations, signedness,
   parameter types, expression order, temporary lifetime, common-subexpression
   boundaries, control-flow shape, and supported compiler modes before using an
   expensive permutation search.
5. **Solve families through representatives.** Normalize remaining assembly,
   group related regions, solve one representative, then independently compile
   and verify the source shape against every member.
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
- compiler invocations and cache-hit rate when available;
- median wall-clock time per successful conversion;
- full-build identity and fallback-byte count.

If a lane produces no match within its declared budget, stop it and record the
failure mode. Do not silently continue a low-yield loop.

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

# 3. Generate bounded drafts. Raise limits deliberately by lane.
bun tools/decompile_batch.ts --context work/m2c_context.c \
  --max-bytes 512 --jobs 16 -o work/m2c-ctx gs1-en.gba

# 4. Perform the cheap exact sweep.
bun tools/match_m2c.ts --jobs 16 gs1-en.gba work/m2c-ctx

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

## Overlay wave

Overlay code has independent load addresses and boundary concerns. Do not feed
it blindly through the ordinary fixed-image pipeline.

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
4. exact-C corpus templates with safe symbol parameterization;
5. literal-pool, alignment, and function-boundary ownership;
6. mismatch-block-focused deterministic mutations;
7. compiler-result caching and batch verification;
8. automatic, atomic integration and generated progress reporting.

Every tool spike needs a measurable acceptance test, such as more exact matches,
lower median mismatch, fewer compiler calls per success, or a resolved boundary
cohort. Park a tool idea that does not move its metric within the declared
budget.

## End-of-cycle checklist

- Regenerate the queue and family reports.
- Run cheap exact sweeps before expensive searches.
- Verify every candidate independently.
- Integrate and fully build each successful batch.
- Inspect and publication-check the staged paths.
- Commit verified progress at the requested cadence.
- Record throughput and parked failure modes.
- Start the next cycle in the highest-yield non-blocked lane.
