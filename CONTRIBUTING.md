# Contributing to Alchemy

Alchemy is a clean-room reconstruction of *Golden Sun* for GBA. A change is
complete only when the rebuilt bytes match the released ROM. Read
[`PROVENANCE.md`](PROVENANCE.md) before contributing: tooling and methods may
be shared, but game-specific facts must be reconstructed from the approved ROM
set and local project evidence. External projects are not source, naming, or
game-knowledge evidence.

This file is the working method: how one owner goes from ROM assembly to exact
C, and how coordinated rounds multiply one insight across a family. The
numbered per-session checklist lives in [`AGENTS.md`](AGENTS.md) and the
command catalog in [`docs/TOOLS.md`](docs/TOOLS.md); this file is why each
step exists and, above all, where each kind of work must stop. The sections
are ordered the way the work actually runs.

## How progress happens here

The git history has one shape: plateaus, then floods. Days of heavy iteration
that move nothing, then one to three commits that move whole percents. The
floods are not luck and not volume. Every flood commit carries a structural
fact extracted from the assembly or from the compiler's own intermediate
dumps: a type, a lifetime, an aggregate layout, a minimal-variable form, an
un-CSE'd operator. A fact proven on one owner transfers across its family,
which is how single commits like "a cohort sweep closes what six hand edits
could not" happen. The plateaus have the opposite signature: parameterized
searches re-run with fresh seeds and no new fact between them.

The strongest measured example is preserved in
[`docs/history/2026-08-09-exact-c-round-method.md`](docs/history/2026-08-09-exact-c-round-method.md):
one coordinated round moved Exact C from 321,934 to 350,806 bytes, and its
seven throughput factors and six stop rules are folded into the sections
below. The lesson generalizes beyond rounds. Facts are the unit of progress;
iterations are only the unit of cost.

## Looping is the failure mode

Looping is launching another search (a permuter round, a mode sweep, an
expression respelling) without a new structural fact since the previous one.
It feels like work, and the history shows it produces the plateaus. The
tripwire is mechanical, not a judgment call:

- Two consecutive bounded searches with no new structural hypothesis between
  them end the axis. Stop, go back to the assembly or the RTL dumps, and
  extract a fact before any further search.
- A search whose hypothesis you cannot write down ("maybe the permuter finds
  it") is looping before it starts. Do not launch it.
- Rising iteration counts against a flat best score mean the remaining
  distance is structural. No volume of the same search crosses it.

Every session ends in one of exactly three states: an exact witness adopted
through a gate; a new recorded fact (a `LAWS.md` law, a typed field or named
structure in the semantic source, a measured negative result); or an honest
stop recorded in `SANCTUM.md` or the owner's `STATUS.md` entry. "Ran more
iterations" is none of these. A session that only iterated failed, whatever
the score did.

## Read the assembly first

C written before the target assembly is understood is guesswork with extra
steps. A decompiler such as m2c is a reading scaffold, never a source draft;
its output must be checked against the ROM and rewritten as reviewed C.
Before writing or rewriting C for an owner, extract the facts from the target
bytes and write them down; the semantic source itself is the notebook, as
types, named fields, and brief comments:

- Boundaries and entries: prologue form, frame size, callee-saved set, stack
  slots and arguments, shared exit tails.
- Control shape: loop heads and bottoms, switch and jump-table layout, branch
  cascades, fallthrough seams, literal pools.
- Types and signedness read off the loads: `ldrsh`/`ldrh`/`ldrb` and shift
  pairs are declarations, not suggestions.
- Aggregate layout and aliasing: which offsets cluster around which base
  registers, which cells are read through one pointer and written through
  another.
- Materializations: which constants live in literal pools, which are built
  inline, which offsets are immediate and which are register-held. Thumb
  `ldrsh` has no immediate-offset form, so a register-held small offset
  before a signed halfword load is the natural spelling of plain array
  indexing, not an anomaly to imitate.
- Optimizer artifacts: surviving copies, preheader preloads, walking-offset
  induction variables, merged loads, shared tails. These are pass output.
  Note them as evidence of what the compiler did to natural input; never
  transcribe them into C (next section).
- Calls and side effects, and every value's lifetime across them.

Every escalation step later in this file consumes this fact sheet. A session
with no fact sheet has nothing to escalate with, and a wrong-sized or diffuse
candidate almost always means the sheet is missing or wrong.

## Write the compiler's input, not its output

The ROM is gcc 2.96 output at `-O2`. The C we want is the input that produced
it: plain structs, arrays indexed by loop variables, ordinary expressions,
natural statement order. The optimizer then reproduces the ROM shape on its
own, because producing that shape from natural input is exactly what it did
the first time.

The classic mistake is transcribing the optimizer's work back into the
source: hand-strength-reduced walking offsets instead of `arr[i]`, hand-CSE'd
temporaries instead of repeating an expression, hand-hoisted invariants,
pointer walks where the load pattern says indexing. This never "locks in" the
ROM shape. It changes what the earlier passes see: the fork runs global CSE
before loop strength reduction, so source that arrives pre-strength-reduced
exposes copies and address expressions to CSE that natural indexing never
surfaces, and the emitted loop rotates or grows merged loads the ROM does not
have. When a localized region refuses to match, the first question is whether
its source imitates output instead of stating input.

The bound runs in both directions, and both are measured:

- Input, not output, as above. Symptom: optimizer artifacts in the candidate
  that the ROM lacks, or ROM artifacts the candidate cannot produce because
  the source already did that pass's job by hand.
- Machine-producing structure, not modern style: a readable rewrite of a
  proven `resource_373` sibling regressed badly because renaming and
  re-scoping changed alias information, while a literal address-adjusted
  transcription of the proven shape matched. Humanize only while each change
  stays exact, through the alias conventions in
  [`docs/SOURCE-STYLE.md`](docs/SOURCE-STYLE.md); repeated verified aliases
  graduate to shared APIs while uncertain identities stay address-named. And
  a transplanted sibling shape is evidence, not a stamp: adopt it where the
  target's own assembly agrees with it, and expect exceptions (the same
  family also produced an owner where the sibling's declaration scopes
  regress).

## Judge candidates by local shape

The global differing-byte count is a cost meter, not a compass. Branch
displacement, alignment, a missing shared tail, or a shifted literal pool can
make thousands of later bytes differ while identifying only one structural
error, and past a size slip, per-row instruction diffs are garbage. After
every compile:

- Align target and candidate per function and basic block. Across shifted
  regions use slip-immune comparisons: instruction multisets, call
  fingerprints, region censuses.
- Cluster the mismatches into regions and name each region's disease (wrong
  loop shape, wrong lifetime, wrong operand order, missing field) before
  touching the source again.
- A wrong-sized candidate, or mismatches diffused across every region, is a
  reconstruction failure. No search fixes it; return to the fact sheet.
- A ROM-proved block rewrite may temporarily worsen the global score by
  exposing missing code elsewhere. Retain it only with local structural
  proof. Conversely, never keep a score improvement you cannot explain:
  duplicated or misplaced code can score deceptively well.

`decomp_diagnose` reports the canonical production-path score and the
dominant residual class. Dominant register-allocation noise at the reference
size is the signature of correct structure. Anything else is reconstruction
work, not search work.

## Escalation order

Use the smallest source change that explains the residual, and keep the
compiler route fixed while changing C. Deterministic before stochastic,
source before compiler:

1. Wrong size or diffuse differences: back to reconstruction under the two
   sections above. Spend no search budget here.
2. Diagnose the localized candidate with `decomp_diagnose`; inspect local
   disassembly, call sites, and residual clusters.
3. Compare a known exact sibling with `overlay_twins`; transpose a proven
   machine-producing shape before inventing a new one.
4. Try small deterministic changes to types, expressions, lifetimes, and
   statement shape with `shape_sweep`, judged by local region shape.
5. Use `mode_sweep` or `mode_cohort` only when the residual itself points to
   a specific routed compiler choice with ledger evidence behind it, never as
   exploration.
6. Read the per-pass RTL dumps (compile the owner with `-da`). Identify the
   pass that produced the wrong shape by the artifacts it inserts (new
   pseudos, shadow registers, preheader preloads, refresh loads), not by
   text-searching, because dead instructions linger textually across later
   dumps. Then change the source form that pass consumed.
7. Run the permuter last, behind the gate in the next section.
8. Change the compiler only with narrow positive and negative regressions and
   a zero-regression exact corpus, through the routed-mode path described in
   [`AGENTS.md`](AGENTS.md).

Do not run every step merely because it exists: a witnessed sibling shape can
jump straight to exact-path rehearsal. And never re-run a measured dead end.
On the main image, exhaustive compiler-family sweeps, the scheduler-flag
surface, old-agbcc at `-O1`, and per-block register-order forcing have all
been swept without yield. `LAWS.md` and `SANCTUM.md` record which axes are
closed globally and per owner; check both before spending any bounded search.

## The permuter is the last mile

`alchemy_permuter` explores respellings the fact sheet genuinely cannot
decide: register-allocation and ordering noise, never structure. The gate to
launch it at an owner, all parts required:

- Reference size reached and the residual localized into explained clusters,
  with the matching-readiness review in
  [`semantic/README.md`](semantic/README.md) complete.
- The canonical score reports dominant register-allocation noise, not a
  structural class.
- A written hypothesis: which region, which respelling family, and why a
  dependency-preserving permutation could close it.

Budgets are stated before launch and hold. Iterations times source bytes
times three must fit the 128 MiB plan budget, so a 60 kB source affords
roughly 700 iterations per round and a request for 2,000 simply errors; size
the round to the source instead of rediscovering that wall every round.
Tie-accepting walks measurably outperform strict better-only climbs. The stop
rule is the looping tripwire: two stalled rounds with no structural insight
in the winners end the axis. Review every winning diff as source (it must
preserve write/read, read/write, write/write, initialization, and call-order
dependencies, and still read as natural input) and adopt only through the
owner gate. A near-match is never promoted.

A permuter run that finds nothing is cost evidence, not exhaustion proof: it
cannot seal a `SANCTUM.md` axis by itself, and it never licenses "one more
round".

## Park and return

Some owners resist at a small floor for reasons the current evidence cannot
explain. Parking is a first-class outcome, not failure: record the floor and
the genuinely exhausted axes in [`SANCTUM.md`](SANCTUM.md) under its
admission rule, then move to the next owner. Parked owners come back when
something new exists (a law proven on another owner, a newly typed shared
aggregate, a routed compiler behavior with corpus proof); that is how floods
start. `check_sanctum --queue` lists the owners one axis away.

## Parallel family rounds

Parallel work is most effective when it multiplies one piece of understanding
across independent owners. Start with `overlay_twins --leads`: a readable lead
that unlocks one or more transpositions usually has better expected return than
a larger isolated owner. For main-image work, prefer same-size candidates with
a small localized residual. Large diffuse mismatches go back to reconstruction;
they are not automatically high-priority because their byte count is large.

The coordinator establishes a baseline, then gives each worker a contract:

- exact owner or family IDs, source paths, spans, and total potential bytes;
- a disjoint set of files the worker may edit;
- source-only or compiler authority, never an ambiguous mixture;
- clean-room, no-assembly, no-adoption, and no-commit boundaries;
- maximum candidates or iterations, a wall-clock limit, permitted search axes,
  and explicit stop conditions; and
- a required report of source and eventual paths, route/compiler identity,
  sizes, differing halfwords, modes tried, rejected candidates, retained files,
  shared outputs touched, and focused tests.

Do not dispatch a worker with only "continue decompilation." That sentence
delegates owner selection, reconstruction, matching strategy, compiler policy,
adoption, and verification at once and has no terminal state. Once a source
candidate exists for a main-image owner, generate its compact contract and
canonical production-path score with:

```sh
make dispatch-decomp ARGS='decomp_diagnose --agent-brief semantic/OWNER.c'
```

Pass that output to the worker unchanged, followed only by owner-specific local
evidence. The brief deliberately allows one file, one search axis, three
non-improving scores, and 30 minutes. Internal compiler-dump tools may explain a
production score; they never replace it. If the brief says `reconstruct`, the
worker may test one structural hypothesis and must not begin compiler or
permuter search. If it says `exact`, the worker stops and reports the witness;
the coordinator still owns adoption.

For an overlay, the coordinator supplies the same fields but obtains the score
from the canonical overlay path; `decomp_diagnose` accepts numeric main-image
owners. Do not substitute an internal dump-mode score.

`STATUS.md` and discovery reports are advisory, not reservations. Work starts
only after the coordinator assigns the owner and records its write set. Assume
every lane shares the checkout and generated outputs. Workers must not reset,
stage, commit, clean broad output directories, independently claim another
owner, or touch another lane's files. Restore only speculative changes made by
the lane after dispatch and inside its recorded write set; never use broad
`git restore`, `git reset`, deletion, or cleanup around pre-existing work.

All lanes are source-only by default. One coordinator-designated compiler lane
may have exclusive access to `alchemy-gcc/`, its tests, and the staged runtime;
every other lane holds compiler and route fixed. The compiler lane does not
adopt, commit, push, or edit root routing and approved-digest tables. It reports
the evidence and computed digests for central integration. Source lanes report
an exact witnessed mode rather than editing routing themselves.

`out/`, candidate reports, manifests, inventory, generated assembly, and staged
compiler outputs are shared mutable state. Use a supported lane-specific work
directory when available; otherwise the coordinator serializes the command.
Workers never delete or reset shared outputs. Workers also submit proposed
`LAWS.md`, `SANCTUM.md`, `STATUS.md`, routing, and other ledger entries in their
terminal report rather than editing those shared files.

Workers stop after the current bounded experiment when asked. A failed lane is
still useful when it leaves truthful C or negative evidence, but it must remove
only its own speculative regressions and identify every retained change. At a
lane's deadline, stop its descendant commands, confirm none remain, audit its
write set and shared outputs, and collect a terminal report. The coordinator
should work on a separate owner or prepare integration without editing an
active lane's files.

At round close, stop new dispatches and collect every lane before mutating
shared state. Revalidate all claimed matches with the current compiler, add
routes centrally, and rehearse the eventual exact path. Apply overlay
adoptions serially because they rewrite shared overlay assembly. Then run:

```sh
make dispatch-semantic ARGS='semantic_superseded --check'
make inventory                         # after overlay changes
make dispatch-metrics ARGS='full_c_progress --write-report'
make coverage                          # when executable metrics changed
make coverage-check
make verify
```

Do not start another round while a worker, descendant process, unreviewed lane
change, or unresolved shared output remains. If the compiler changed, finish
its focused regressions and full routed zero-regression corpus, then commit and
push the compiler repository first. Every round still closes with one verified
main-repository progress checkpoint.

The measured origin and stop rules for this protocol are preserved in
[`docs/history/2026-08-09-exact-c-round-method.md`](docs/history/2026-08-09-exact-c-round-method.md).

During iteration, invoke a public command through a `dispatch-*` Makefile
wrapper or use a focused native binary. The root Makefile is the workflow
facade; the dispatcher registry supplies the group and command names:

```sh
make dispatch-semantic ARGS='semantic_queue --help'
make dispatch-decomp ARGS='decomp_diagnose semantic/resource_373_c_0200034c.c'
make dispatch-overlay ARGS='overlay_adopt resource_373:034c --source semantic/resource_373_c_0200034c.c --span BYTES --where'
```

Replace the uppercase placeholders with the owner and span under review.
Direct invocation is useful while developing a command:

```sh
cargo run --release --manifest-path tools/semantic-queue/Cargo.toml -- --help
cargo run --release --manifest-path tools/decomp-diagnose/Cargo.toml -- semantic/resource_373_c_0200034c.c
cargo run --release --manifest-path tools/overlay-adopt/Cargo.toml -- \
  resource_373:034c --source semantic/resource_373_c_0200034c.c --span BYTES --where
```

For a main-image exact candidate, `integrate_matches` accepts a directory
containing a `src_<main-address>.c` draft and, with `--apply`, installs
`exact/<main-address>.c`. For a code-overlay candidate, `overlay_adopt` installs
`exact/resource_<overlay>_c_<overlay-address>.c` after its rehearsal passes.

Use the repository's `Makefile` for shared workflows:

```sh
make test          # policy gates and native self-tests
make lint          # architecture and publication/provenance gates
make progress      # current byte-exact counts
make coverage      # refresh dashboard maps when metrics changed
make verify        # complete pre-commit authority
```

## Caching and proof

Native tools cache expensive derived work under ignored `out/` paths. A cache
hit is an optimization, not evidence: cache identities include the relevant
source, compiler, route, and input data, and a changed input must produce a new
identity. If results seem stale, use the tool's report/check mode or remove
only the affected ignored cache after confirming its path; do not treat an old
score as a new build. In particular, an apparent regression observed through a
stale cache is a false signal: after a compiler rebuild, clear the affected
ignored overlay cache (`out/cache/overlay-c/`) before diagnosing drift.

Matching under `semantic/` is not final proof. Compiler flags are routed by
repository-relative source path. Main-image drafts are checked by
`integrate_matches`, which routes a `src_<main-address>.c` candidate as its
eventual `exact/<main-address>.c` path. Overlay drafts are rehearsed by
`overlay_adopt ... --where`, which uses the eventual
`exact/resource_<overlay>_c_<overlay-address>.c` path. In both cases the
adoption pre-flight must assemble the owner and compare its bytes; only then
should the source move into `exact/`.

The diagnostic overlay inventory is a required input file for `overlay_adopt`
and `make build-semantic`. When `out/decomp/overlays.json` is absent or needs
refreshing, run `make inventory` first. It supplies discovery rows, not
byte-exact acceptance proof: `overlay_adopt --span` may select a span that the
inventory does not list, while its assembly boundary and whole-overlay
comparison remain the safety checks.

## Safety and publication

- Never use `asm(...)`, fixed-register bindings, empty assembly barriers, or
  another assembly escape hatch in C or headers.
- Do not copy game-specific code from another decompilation. All game-specific
  facts must come from the approved ROM set and local reconstruction evidence;
  record tool provenance where it matters, and keep retirements recoverable.
- Keep ROM images, executable dumps, build outputs, and caches out of the
  publishable repository. Publish source-format assets only when their
  provenance and reproduction role are documented.
- Do not resolve compiler routing tables or
  [`alchemy-gcc/gcc-2.96/gcc/config/arm/arm.c`](alchemy-gcc/gcc-2.96/gcc/config/arm/arm.c)
  by taking one side of a merge. Use the merge base and preserve the intent of
  each entry.
- Preserve existing user work in a dirty checkout. Inspect the working tree
  before editing and keep unrelated changes out of the commit.

## Committing

Regenerate metrics after executable-source changes. The commit subject must
start with the generated Exact-C progress prefix:

```text
☀️ N% – description
```

`N` is the nearest whole Exact-C percentage from the staged report, never the
public DONE percentage. Use `make verify` before committing; a green focused
test is not a substitute for the complete gate. A deliberate regression or
denominator correction must still be plainly described.
