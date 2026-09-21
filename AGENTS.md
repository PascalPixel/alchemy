# Alchemy

Alchemy reconstructs Golden Sun: **The Broken Seal (TBS)** and **The Lost Age
(TLA)** as readable C and editable assets that rebuild the shipped games byte
for byte. Japanese releases are the source editions; localizations are measured
differences. Build IDs use `tbs` and `tla`; the physical roots are
`games/THE BROKEN SEAL` and `games/THE LOST AGE`. Only TBS English currently
has a complete production ROM gate. Twelve successful compilation checks do
not mean twelve rebuilt ROMs. Outside contributions open at 100%.

Read this file in full. It is the **only working guide**, including priorities,
recovery methods, constraints and tooling. `README.md` introduces the project
publicly. There are no topic documents or separate task lists.

## Two directives

1. **Never commit anything a pret-style decompilation would not publish.** The
   publication rules below outrank every goal and percentage.
2. **Reconstruct the source Camelot most likely had on disk:** coherent files,
   names, types, macros, assembly and generated tables with their readers and
   writers. Matching bytes are necessary; a trick that happens to match is not
   reconstructed source. Our names and layout are hypotheses, not lost originals.

Evidence can correct a reconstruction rule, never waive the publication rule.
A refusing tool or failed gate names a problem to fix, not a route around it.

## AI cheating

An agent cheats when it records the expected answer or changes the test instead
of reconstructing the mechanism. Byte equality alone cannot distinguish those
approaches. A smaller exception file, a clever name, or a green gate does not
make an answer table an implementation.

Confirmed examples include the binary COMPRESSION.TOKENS table, hash-selected
recipes in GRAPHICS/COMMON/COMPRESSION.JSON, and the former explicit compression
token lists under TLA WORLD_MAP.JSON's `chunks`. Splitting saved encoding choices
among short JSON arrays does not make them editable map source. Audit these by
their readers and writers, not just file extension or individual array length.
The publication gate rejects known JSON compression token arrays, packed-table
references, nonempty predictor exceptions and captured trailing padding at any
nesting depth, subject only to the temporary frozen-debt exception below.
Input-derived compression settings remain valid. This check covers the known
recipe schemas; it does not certify that arbitrary differently encoded data is
honest. The larger source-and-consumer audit still applies.

For the weekly wrap, Pascal explicitly permits existing compression debt at
commit `d08ee3a28fc92afe15c6215d0997a05659395f1c`. The publication gate admits
only unchanged answers at the same file and JSON position, and the identical
COMPRESSION.TOKENS file. It rejects new, changed or relocated answers and fails
closed if the checkpoint is unavailable. Removing answers or replacing them
with input-derived settings is allowed. This temporary exception grants no
recovery credit; remove it and the legacy readers when the encoder and packer
are recovered. Do not compact, expand or disguise the retained debt.

- Do not replay compression tokens, retain per-resource encoding overrides, or
  copy unexplained padding to make a compressor match. Retire `.lz.json` plans
  by recovering the encoder and packer. Moving their contents into manifests,
  source constants, binary tables, caches or generated files is the same cheat.
- Do not invent compiler switches, accumulate per-function flag combinations,
  patch generated output, or select a lucky compilation. The approved compiler
  contract belongs in [Compiler integrity](#compiler-integrity).
- Do not special-case an owner, input hash, ROM address, byte offset or expected
  output to close a mismatch. A general rule needs evidence for its mechanism
  and checks on inputs beyond the example that suggested it.
- Do not shrink extents, relabel unexplained assembly, restore reference bytes,
  weaken gates or alter accounting to claim progress. The authority for credit
  is [Completion](#completion-and-measurement), not the desired percentage.

Expected output bytes may be an independent test oracle, never an encoder input
or production fallback. Source-built inputs must determine the output without a
reference ROM or recorded decisions. Historical buffer reuse, if demonstrated,
must be reproduced from real build inputs and their order; seeding a buffer with
captured bytes is not recovery. Existing recipes are unresolved reconstruction
debt, not permission to add more. Do not claim their removal until the actual
replacement passes the complete build. Name remaining failures honestly.

## One home for knowledge

Only root `AGENTS.md` and `README.md` may be prose documents. Claude also reads
AGENTS.md; `CLAUDE.md` is retired, including symlinks. No other Markdown,
plain-text, reStructuredText, AsciiDoc or equivalent notes may exist anywhere
in the checkout, **including ignored `out/`, scratch and nested worktrees**.
Do not evade this by renaming prose to `.json`, `.log`, `.ts` or an extensionless
file. Do not create TODOs, handoff documents, plans, reports-as-guides, local
AGENTS files or a replacement memory directory. `alchemy check publication --documents` enforces
the file-format rule; `make verify` includes it.

Upstream compiler and assembler source retains its own documents and GCC
machine descriptions. The approved `agbcc` and `agscc` submodules and actual
compiler/binutils source trees under `out/compilers/` are exempt; an arbitrary
nested `.git` directory is not. Never delete licensed upstream material to make
the document check pass. Binary section dumps are build output, not prose.

| Fact | One maintained home |
| --- | --- |
| General methods, rules and open priorities | This file; replace obsolete guidance instead of appending competing rules |
| Owner names and source paths | Each game's `source-paths.json` |
| Types, declarations and behavior | Maintained C and headers |
| Unit membership, instances, extents and bindings | Existing translation-unit manifests and boundary registries |
| Current owner classification, extent, source and measured score | Existing `recon/en/dossiers.json` entry, with game-qualified identity |
| Current verified credit | Fingerprinted build receipts, generated into stable target report directories |
| Temporary compiler diagnostics | Disposable work directories under `out/` |

A dossier is a compact current registry, not a laboratory notebook. Keep the
owner's classification, complete extent, maintained source, retention state and
latest measured score. Dated attempts, command transcripts, temporary paths,
model responses, rejected-spelling lists and superseded theories are
**baaaaaaad**: they turn JSON into hidden scratch memory and make later agents
optimize against stale stories. Reusable findings belong in the owning code or
the single relevant rule here; temporary diagnostics stay disposable under
`out/`. Old scores are leads, never current verification. Repair references and
validators when an owner moves.

## Recovery: verified bytes per hour

Optimize **new verified exact-C bytes per hour across the whole game**, including
integration and verification time. Report DONE separately. A filename cleanup,
new draft, better similarity, source consolidation or changed accounting is not
new exact C. Converting credited assembly to C raises exact C but may not raise
DONE. Do not use the rounded commit prefix to measure C progress.

### Select a bounded family

1. Read fresh ownership and coverage. Rank the **100 largest unresolved complete
   functions**, including those without drafts, excluding already exact C and
   positively evidenced permanent assembly. Work only on that list's functions
   of **at least 5,000 bytes**. Main and overlays compete together.
   Pascal reaffirmed this minimum after the inventory showed only six currently
   known unresolved functions above it. Keep it until Pascal changes it; do not
   lower it automatically to reach the percentage goal.
   A coverage fragment is not automatically a complete function.
2. Follow their dependencies into our exact C: read callees, callers, shared
   headers, verified siblings and unit context. Group candidates by a supported
   shared interface or residual cause, then choose the best bounded family by
   expected new C per hour. Size alone is not a probability of success.
3. Smaller-function work is limited to humanizing and building needed dependencies
   from the existing byte-exact C corpus. Consolidate only the declarations,
   types or shared routines that the selected large functions need. Verify exact
   callees remain exact, then repair unresolved callers. Do not pursue small
   unresolved functions or independent cleanup campaigns.
4. Keep each selected owner's current extent, source, classification and latest
   score in its dossier. The bounded work plan is temporary; dependency work
   serves the selected large owners and does not create another recovery queue.
5. After closing a representative, test every applicable sibling with its own
   constants, call bindings, layout and complete byte comparison. An empty twin
   sweep is closed until its inputs change. Do not repeat broad censuses blindly.

Use Jev for probabilities over a genuinely large candidate collection when it
is available, using compact non-payload features. Pascal authorizes sending
that material without another permission request. Its rankings are advice, not
proof, compiler authority or adoption. Do not send ROM/reference bytes or
credentials. If unavailable, record that once and use measured evidence; do
not spend the batch repeatedly rediscovering access or inventing probabilities.

### What the productive history actually showed

The July 30–31 historical ledger records 79,558 added C bytes in 24 hours, but
that includes merged work and compiler routes subsequently retired. On August
17 (`3cec47862`), 103,548 credited C bytes were withdrawn after a build audit;
on September 4 (`579b71488`), removing per-file routes withdrew 33,720 more.
Never restore those shortcuts to recreate an old rate. Historical counters
have gaps and changed policy; they are not a certified all-time ranking or a
measure of agent token efficiency.

The September 3–5 scene wave combined batch lifting, complete function discovery,
correct pool/alignment boundaries and verified twins. `b85a4081d` adopted 60
functions from previously unregistered stretches: lack of a draft was a search
opportunity, not evidence that code was intrinsically hard. Reuse the existing
lifter and fix demonstrated recurring defects; do not build a second one.

On September 10, saved counters rose from 592,372 to 631,946 exact-C bytes in
11 hours 20 minutes. Five large owners supplied 27,628 bytes. These commits
preserve the useful evidence:

| Evidence | Transferable technique |
| --- | --- |
| `29cc444ed`, 7,356-byte scene | Symbol addresses, direct calls, correct live values and direct guard reads removed a small size error that had displaced thousands of halfwords. |
| `1d5852f4a`, two owners / 12,632 bytes | One corrected source store order removed 25 differing halfwords. Smaller intermediate scores had been compensating edits, not correct source. |
| `886891632`, 4,092-byte scene | Two pointer locals were one reused variable. Preserving its lifetime explained the reference's scratch-register copies. Merging every pointer was wrong. |
| `b19cf9bb8`, 3,548-byte scene | Previously recorded findings, including those from failed owners, corrected call return types and closed a different large owner. |

The lesson is to recover one true cause and apply it where its preconditions
hold. A lower mismatch count alone does not establish that a change is true.

### Short loop

Read each command's help before scripting around it. Scripts are TypeScript on
Bun or Rust, never Python or shell scripts. Only the minimal root launchers and
Git hooks use shell to enter the Rust tooling.

```sh
./alchemy inspect <owner> --asm
./alchemy inspect <owner> --siblings
./alchemy extract <owner> --out out/work/reference.bin
./psynergy decompile out/work/reference.bin --base <runtime-base> --entry <entry> --span <complete-bytes> --out out/work/candidate.c
./alchemy score out/work/candidate.c --owner <owner> --align --first
./alchemy score --unit <unit-id> --first
```

Extraction prints the loaded runtime base, entry and span; use those, not resource
offsets. Reference bytes remain private. The decompiler takes explicit files,
not game owner identities; resolve its symbols, types and control flow before
scoring. Extraction and decompilation refuse to overwrite outputs.

Keep one canonical candidate. Change one evidenced hypothesis, compile the
object or declared unit, inspect the full residual and preserve only supported
changes. Shared headers, manifests and staging have one editor at a time.
Use existing unit declarations; `alchemy unit scaffold tbs <id> <start> <end>`
creates a main unit when needed. Preserve function order and exact neighbors.
Declare retained members explicitly without granting C credit. A guessed unit
is a hypothesis about original context, not historical proof.

After **three source attempts per function**, park with a concrete residual
or hand off that evidence. Two bounded searches with no new structural fact
close that axis. A missing prototype, corrected boundary, improved tool or
verified sibling can reopen it; another equivalent spelling cannot. Do not
turn a historical “stubborn” label into a permanent ban or compiler verdict.

### How to do final closure

Differing halfwords help us get close; they do not measure how close the source
model is to a solution. A two-halfword residual can remain impossible within
the current reconstruction. Do not keep polishing that reconstruction merely
because it has the best score. Nor does failure of the permuter, or an explanation
of the compiler's decision, prove that the original behavior cannot be emitted.

#### Reverse construction

When repeated local repairs stall, work in reverse:

1. Name the exact detail that must survive compilation: an argument-load order,
   access width, pointer lifetime, branch shape or other observed behavior.
2. Start afresh with a minimal mockup or scaffold. An assembly reference or
   assembly-backed diagnostic base can isolate the surrounding machinery while
   the smallest relevant C passes through the approved compiler. First make
   that compiled C reproduce the difficult detail; copying it into inline
   assembly does not establish that the compiler can emit it.
3. Keep that working detail as the local acceptance check. Add the surrounding
   C back in coherent pieces, checking after each addition. The first piece
   that breaks it identifies the context to investigate: declarations, types,
   lifetimes, control flow, aliasing or translation-unit structure.
4. Repair that context and continue outward to the complete function. Accept
   a temporarily worse whole-function halfword score when the experiment
   establishes a supported source fact. Getting farther away numerically can
   be necessary to reach exactness.

This is a different investigation from permuting the stuck draft, not permission
to reset attempt budgets or run an unlimited search. Keep the witness and first
breaking addition in the active bounded workspace. Put any reusable source fact
in the source, type or rule it corrects, then discard the diagnostic.

Mockups, dummy scaffolding and diagnostic assembly stay private under the active
`out/` batch and earn no completion credit. They must not become artificial
production constraints, forced registers, barriers, fake declarations or byte
patches. Final acceptance still requires credible reconstructed source and the
complete linked owner to match, followed by the production gate. Never shorten
the owner to turn a local witness into a claimed match.

Verified case: `resource_3bd:020013f8` matched all 6,220 bytes in 30 fresh
compilations and passed the full TBS English ROM comparison. A minimal camera
call sequence emitted the required argument order; adding both the later wait
and motion calls reversed it during scheduling. Rebuilding that context exposed
the boundary supplied by `Camera_MoveToActorAndWait`. Ordinary wrappers did not
fix it, and a real loop boundary fixed the local order but broke the full owner.
The macro was admitted by Pascal's explicit exception recorded below; this
result proves its compilation behavior, not that Camelot wrote that macro.

Keep additional proven closure methods under their own named headings here.
Each needs a reproducible witness, the source context it repairs, a complete
verified owner and its limits. A local scheduling success alone is not a proven
closure method.

#### Linked identities

When a retained value names an edition-dependent message or asset, test its
existing linked-symbol representation before treating it as a plain integer.
In Mercury Lighthouse `resource_39c:02004a08`, the message-base symbol restored
the shared lifetime and reduced 90 aligned halfword edits to 17. Proven typed
actor, dialogue and map interfaces closed the remaining edits; all 608 bytes
matched in 30 fresh compilations and the complete TBS English ROM matched.
This uses the established Lunpa linking convention, not a general license to
turn arbitrary constants into symbols. Verify the identity, consumer and binding.

### Read the cause, not the score

| Residual | First evidence to inspect |
| --- | --- |
| Extent or binding | Complete function, pools, entry alignment, continuations and actual runtime targets |
| Control flow or missing effects | Branches, loops, switches, calls and guards against reference and exact siblings |
| Type or access width | Proven structs, offsets, signedness, pointer returns and aggregate interfaces |
| Allocation or scheduling | Correct lifetimes, aliasing, reused locals, dependencies and compiler pass dumps |
| Literal pools or constants | Symbol versus integer meaning, access width, real shared constant lifetimes and pool reach |

- A small insertion shifts every later halfword. Use aligned instruction diffs
  and normalize branch targets diagnostically; never normalize acceptance bytes.
- Under the observed GCC 2.96 route, void versus value-returning calls can
  reverse argument setup order. Treat that as a prototype hypothesis, verify
  it against the callee and siblings, and discard an unused result normally.
  Do not change return types purely to steer scheduling.
- Reconstruct which values are genuinely shared and which are distinct. A
  pointer live after an offset calculation explains a copy; splitting or merging
  every local by style destroys that evidence. Source store order need not
  equal final instruction order. Compiler passes can schedule separate branches
  before merging their common tail; do not pre-merge it reflexively in C.
- Use established structs and shared modules before recreating offsets. Exact
  callee bodies can prove access widths, signedness and missing arguments; DMA
  is one shared primitive inside ordinary C, not a reason to mark callers ASM.
- Signed division and remainder use ordinary `/` and `%` with the proper runtime
  binding. Verify the actual veneer target; a legacy helper name can misstate
  signedness. Typed indirect calls express call-via interfaces even when a
  direct call happens to match.
- GCC nested functions can explain an inherited Thumb `r9` static chain. Read
  the enclosing function and capture layout before declaring a helper assembly
  or inventing a global parameter. Definition position and local lifetimes can
  explain capture offsets. Compile the real enclosing context; only separately
  verified complete helpers may gain credit while a parent remains retained.
- Stack-resident routine wrappers can be ordinary variable-length arrays and
  typed indirect calls. Use only context proved by the reference and exact
  examples, not padding or fixed-register locals.
- Remove inert casts, stores and blocks. Do not keep tricks that improve a
  number without explaining behavior or compiler context.

Use `alchemy score ... --allocator-order`, then `alchemy build allocator <owner>`
and `psynergy inspect allocator <dump-directory>` for actual GCC decisions.
Creation, CSE, combine, allocation, reload and scheduling dumps outrank guessed
register stories. Router `next=` and `repair_hint=` are guarded experiment
suggestions. The existing `recon/compiler-repair-patterns.json` catalog needs
preconditions, regression fixtures and a real closure for each generalized
operation. Manual recovery does not require another catalog entry.

### Integrate and end the batch

Overlay owners use `alchemy adopt <resource>:<address> --source FILE`; main
owners use registered source paths and `alchemy check integrate <directory>
--apply`. Both prove the whole owner before retiring assembly. Shared units
must verify every affected linked C member from the shared object; explicitly
retained holes remain uncredited. Use `alchemy unit flatten --help` for verified
overlay consolidation, not independent adoption of overlapping members.

Close with starting/ending exact-C bytes, net new C, DONE separately, accepted
owners, parked causes, elapsed time and checks actually run. Zero adoption is a
negative result, not permission to expand an identical search. At a deadline,
stop launching work, preserve candidates, finish or revert experiments and verify
accepted changes. Accumulate verified work until DONE has increased by at least
**one full percentage point since the previous commit**, then commit. Compare
unrounded verified credit against the same audited denominator, not rounded
commit prefixes. Do not make smaller progress or documentation-only commits.
Never lower acceptance to hit a goal.

## Disposable output, durable results

`out/` is **not memory or a source corpus**. Existing stable target directories
hold generated builds, caches, receipts and previews. Temporary experiments
use one bounded work directory per active batch; reuse it instead of creating
one directory per source spelling. No new tracking service, queue database,
scoreboard framework, prose archive or experiment index.

Before ending a batch:

1. Put complete useful nonexact C in the game's existing `recon/en/main`,
   `recon/en/overlays` or declared unit source, with its current source and score
   in the dossier. Keep accepted C in `SRC`. Never leave the only useful candidate
   in `out/`.
2. Update the dossier's current classification, extent, source and score. Put
   durable behavior in maintained C and types, and a reusable method in its one
   paragraph here. Do not preserve commands, attempt history or rejected guesses.
3. Remove the batch's throwaway candidates, scripts, copies and reports after
   their useful results have durable homes. Retain only reproducible diagnostics
   needed by a specific active question. Remove landed branches and worktrees;
   `main` is the only branch left behind.

Clean only known-owned scratch. Do not wipe another active task's work or unique
candidate, move notes outside the checkout to evade this rule, or rename notes
as logs. Genuine raw tool logs, structured machine reports and binary sections
are allowed output, not standing guidance. `make clean` removes generated output
and preserves maintained inputs, ROMs and installed compilers. Inspect its scope
and active work first; a cache is rebuildable, a sole draft needs preservation.

## Publication and evidence

Ask of every tracked file, generated or not: is it code, tooling, non-payload
metadata/documentation, or an editable game asset **actually consumed by that
game's byte-identical build**? If neither, it cannot be tracked. Raw code
listings are permitted reconstructed source; ROMs, fragments, extracted binaries,
objects, ELFs, reference-containing patches, integer/hex/base64 byte dumps,
previews, screenshots, converted fonts, animations and player exports are not.
Never transmit those private reference bytes or fill unfinished code from them.
Keep compiler, assembler, linker and runtime-library source or patches in their
own licensed repositories, not this unlicensed repository. No leaks or SDK code.
Runtime objects such as libgcc soft-float and `_call_via_rN` are built from the
approved licensed containers using `recon/compiler-runtime.json`, never copied
or reconstructed into this repository as C, assembly or data.

Admissible sources: locally held ROMs, independent reconstruction here, decoded
data, verified calls/relocations, and public language, hardware, compiler and
format documentation. Never inspect another Golden Sun reconstruction's code,
symbols, types, comments or conclusions. The sole exception is the credited
Tarpman/Karathan compiler-and-flags identification. Unrelated projects can teach
methodology, never supply target-specific facts. Melee's duplicate-family and
typed-module approaches are examples, not Golden Sun source. Verify suspect
visual behavior in an emulator rather than guessing from extracted pictures.

`alchemy check publication --tree` enforces mechanical publication restrictions,
including license markers, patches, foreign gitlinks, embedded media and raw
JSON byte streams outside named typed `values`. Passing it is not permission.
Only `agbcc` and `agscc` gitlinks are admitted. Asset builds reject unconsumed
tracked game inputs; source tracking rejects previews and unregistered private
inputs. A typed table comes from its consumer, not a renamed byte dump.

## Completion and measurement

Exact C is ordinary production C reproducing the **complete linked extent**
under the approved route with zero differing bytes. Size, plausible behavior,
similarity and isolated fragments are not proof. Include literal pools and
follow shared epilogues, continuations, live registers and long branches. Never
shorten expected bounds to fit a candidate. Reviewed main owners can be scored
with `--size <complete-bytes>`; overlay sizes must equal the boundary resolver.
Overlay code loads at `0x02008000`: resource offsets are not runtime addresses.
Require inverse serialization and load round trips; BL-shaped literal data can
also be rewritten by the loader. Bind from reference sites, not candidate offsets.

**DONE = exact C + evidenced permanent assembly**, divided by audited executable
bytes, separately for TBS ☀️ and TLA ⚓️. Report shared/game C and shared/game ASM
parts, count each image range once, C before ASM. Common source does not duplicate
credit within a game. Drafts, unknown code and private ROM-restored input earn
nothing. Assembly-to-C conversions can leave DONE unchanged.

Each game's `metrics/executable.json` is its sole committed denominator. A
complete audit accounts for the main image and every overlay's executable
intervals and excluded complement. Pending audits yield `?`, never estimates;
new candidate audits remain under `out/` and cannot affect displayed scores.
`out/<target>/reports/verified-code.json` records ROM hash, input fingerprint,
source, category and credited ranges. TBS's full build produces its receipt;
TLA's owner check produces its receipt without claiming a full ROM. Changed
inputs invalidate it. Coverage, dashboard, README and prefixes read these same
receipts; rendering cannot create credit. Current receipts cannot score old refs.

Owner binary similarity is one minus unit-cost halfword edit distance divided
by the larger halfword count, including pools. It is diagnostic, not semantic
quality. Corpus similarity, if measured, must weight complete owners by reference
bytes, count exact C as one and absent drafts as zero, and distinguish stale or
unscored drafts from absent drafts. No corpus-wide partial percentage is currently
established. Executable DONE, physical-ROM identification and full-ROM rebuilding
are separate denominators and claims.

### Assembly credit

Unsuccessful searches, compiler non-emission, repeated scripts, register walls
and large functions do not prove handwritten assembly. Retained ranges need
positive `library` or `handwritten` provenance, evidence and nonempty proof or
object identity, with byte comparison. Each range is reviewed independently;
other ranges of its kind inherit nothing. `structured_scene_module` is a draft
scene, not the retired `generated_call_script_module`. Classification stays in
the existing main and overlay registries. Only Pascal changes credit standards.

Pascal's specific exception: fixed overlay veneers reconstructed with the shared
`SRC/SYSTEM/OVERLAY.INC` macro and per-overlay `ENTRY.INC`/`IMPORT.INC` lists.
Credit requires kind `veneer`, confidence `proven`, provenance credit
`reconstructed_veneer`, the macro proof path, complete aligned eight-byte entries
inside audited veneer ranges, and exact overlay/ROM reproduction. This proves
assembly linkage reconstruction, not whether Camelot used a macro or generator.

## Compiler integrity

Game code uses the approved GCC 2.96 route; evidenced prebuilt-library families
have fixed provenance-based routes. Commands live in
`tools/alchemy/src/compiler/routing.rs`. Both games use the same bundle; TLA
alone enables the approved `-mgs2` lowering moved into licensed `agscc` on
September 13. This is reconstructed lowering, not recovered historical source.

Apart from that TLA exception, modifications to agscc must restore historical
stock Red Hat GCC 2.96. Diagnostic/host changes also need Pascal's approval,
specific release/source evidence and fidelity proof. Only Pascal authorizes
pins, executable hashes, family routes or output transformations. A better
score, determinism, version string or approved pin does not supply provenance.

The September 20 aliasing probe compiled the 20 largest maintained exact-C source
files by source-file bytes under identical TBS flags, then added only
`-fno-strict-aliasing`. All compiled; seven changed generated assembly, including
store scheduling in FUNE_HEYA/EXTENDED_CHOREOGRAPHY.C. This was a diagnostic
source comparison, not a linked production-route validation. GCC 2.96 enables
strict aliasing at `-O2`; disabling it is not a neutral matching aid. Investigate
proven object views and direct union accesses before considering a global change.

No per-function flags/routes, invented flags, fixed-register C locals, empty
barriers, forced scheduling, output patches or selected lucky runs. No volatile
ordinary RAM, dummy/duplicate/overwritten stores or `do { } while (0)` barriers.
Pascal's explicit cleanup exception on September 20 standardizes the existing
five `do/while(0)` workarounds as `SCHEDULING_CHEAT(statement)` in TBS
`INCLUDE/TYPES.H`. Its uses are the camera two-call wrapper in
ARUTAMIRA_DOU/ACTOR_PRESENTATION, WINDOW/RUNTIME_SET_PARAM_NIBBLE,
OBJECT/GROUP_STORE_OBJECT_AND_RUN_STEP7, OBJECT/WAIT_TWELVE_FRAMES_THEN_SET_FIELD18,
and the existing loop in BATTLE/EFFECT/SCHEDULE_RATIO_TRANSITION. This names
existing debt; it does not prove historical source or authorize more sites.
Keep the macro expansion unchanged while standardizing; revisit ordinary C
when resolving these explicit exceptions. The camera owner covers 6,220 bytes;
the three single-statement owners cover 144 bytes, and the loop belongs to a
280-byte unit. The separate Data_02009b85 call identity in
HAIDIA_SUKURETA/PROGRESS_ACTOR remains a binding-review exception (1,728-byte
owner), not a reason to run another matching search during this cleanup.
Withdraw a trick-dependent match, mark its current state as retained assembly,
and leave it uncredited rather than protect the percentage. Volatile is justified for I/O,
video/palette memory and genuinely interrupt-shared state.

Inline assembly is allowed only in an evidenced shared macro implementing a
recurring machine interface the approved compiler cannot emit, never a per-owner
scheduling patch. The token-pinned `Dma_Set` in TBS `INCLUDE/DMA.H` is admitted
with its full instruction/operand/clobber contract. Its internal registers do
not authorize caller register forcing. Do not change it or admit fill macros
silently; `raw/classification.json` owns its family evidence.

Track these three reconstructed primitives for review closer to 100%; do not
silently generalize their exceptions or mistake them for recovered original text:

| Primitive | Maintained home | Evidence and later review |
| --- | --- | --- |
| `Dma_Set` | TBS `INCLUDE/DMA.H` | Shared inline assembly; 296 fixed-register kick sites recorded in `raw/classification.json`. Revisit its interface without weakening exactness. |
| `overlay_veneer` | TBS `SRC/SYSTEM/OVERLAY.INC` | Reconstructs fixed linkage stubs under the specific veneer-credit exception. Original macro versus generator remains unknown. |
| `SCHEDULING_CHEAT` | TBS `INCLUDE/TYPES.H` | Explicit temporary scheduling workaround at the five sites listed above. `Camera_MoveToActorAndWait` uses it for the two-call sequence; historical provenance remains unproven. |

GNU GAS 2.10 assembles compiler output and unit slices, including alignment;
modern binutils handles retained syntax and linking under the recorded ABI.
Never suppress ABI errors. Cache identity includes the executable bundle.
Uppercase `.C` is explicitly C, never inferred as C++. Compiler-emitted two-byte
overlay alignment gaps may be declared as `compiler_gaps` only between adjacent
exact owners and compared from actual emitted sections. Do not synthesize fill
from the ROM or alter owner extents to count it.

## Source and shared interfaces

Keep a shallow module tree, not one file per function or a generic SHARED bucket.
Area actors, dialogue and events stay with their separately loaded area module;
one location can own several overlays and their address spaces never merge.
`locations.tsv` and runtime map/resource evidence own placement. Japanese ROM
names supply uppercase short romaji prefixes repeated for sorting, plus one area
word: `RUNPA_DOU`, `RUNPA_JO`, `HAIDIA_MURA`, `HAIDIA_HEYA`. Use evidenced `MURA`,
`MACHI`, `HEYA`, `DOU`, or `SAI` for a return scene. Record decoded labels and
abbreviations in existing evidence. No resource codes, addresses, ordinal names,
codenames, invented geography or English localization names as folder labels.
Remove empty folders after moves; do not create empty symmetry between games.

| SRC directory | Responsibility |
| --- | --- |
| SYSTEM | Startup, scheduling, memory, input, save, link, resource loading and overlay mechanics |
| LIB | Game support/math; compiler runtime stays in its licensed container |
| GRAPHICS | Display, palette, animation, tiles, text, windows and icons |
| SOUND | Audio runtime, separate from sound assets |
| GAME | Character, party, inventory, abilities, Djinn and flags |
| FIELD/COMMON | Shared field engine, maps, camera, objects, events and scripts |
| FIELD/location | A distinct loadable area and its exclusive assets |
| FIELD/COMMON/locations | Distinct loadable modules serving several evidenced places |
| BATTLE, MENU, DEBUG | Their game responsibilities |

Native `SRC`, `INCLUDE`, `SOUND`, `TEXT` names use uppercase ASCII basenames and
extensions. Registries retain established spelling. Use `Subsystem_VerbObject`,
short `pos`, `cnt`, `tbl`, `buf`, `work` locals and ordinary C89, preserving bugs.
Unknown fields stay explicit offsets or `unk`; `pad` means proved padding.
Names are reading aids, not historical claims. Shared declarations/types have
one header owner; do not duplicate declarations per caller or invent struct
meaning to improve a score. Engine calls use registered names; bindings retain
addresses. Legacy `Func_`, `Data_`, `Value_` names are forbidden in instanced
sources/includes and migrated COMMON/LIB code.

Resource directories live beside loaders at `SRC/SYSTEM/RESOURCE/DIRECTORY.JSON`;
field selectors and naming rules live at `SRC/FIELD/COMMON/SCENE_TABLE.JSON` and
`NAME_RULES.JSON`. Asset layout is `recon/assets.json`, not runtime source.
Classification and provisional-source records stay outside SRC. Retained overlay
listings and compression recipes live in `raw/overlays`, battle listings in
`raw/battle`. `OVERLAY_DATA_DIRECTIVES_MAX` must not grow.

The LZSS compressor reconstruction is incomplete. Nearest longest matches,
one-byte lazy matching, and windows of 4,123 bytes for general LZ and 4,092 for
palette LZ reproduce 96/96 Japanese TBS stream bodies, 95/96 in each of its five
Western editions, and 113/114 in both Japanese and English TLA. General LZ
applies the replacement once per stream; palette LZ can repeat it. Choosing the
smaller encoding, palette on ties, selects the observed codec in all 804 tested
streams. These are compression comparisons, not full builds or new exact C.

Overlay compression now defaults to input-derived general/palette encoding,
choosing the smaller result and palette on ties. An existing `_stream.lz.json`
sidecar is a visible temporary exception, not a required file for every overlay.
New overlay exports require automatic byte equality and write no sidecar.
The cleanup targets the 38 sidecars with no captured padding (36 TBS, two TLA);
the remaining 66 retain their old answers under the frozen-debt rule above.
Do not move their padding or tokens into the series manifest to delete a file.

Western TBS resource 3b4 first disagrees at decoded offset 12,092: a two-byte
copy at distance 4,125; another occurs at 12,104. Widening the general window
from 4,123 through 4,128 never closes it and regresses other overlays. TLA
resource 6ae instead rejects the predicted lazy replacement at offset 14,204:
the ROM uses copies of lengths 2 and 2, whereas the model emits a literal and
a length-3 copy. Do not patch either location. The latter is not a distance-limit
failure and needs the actual lazy-matching decision recovered.
At that alternative position, the nearest matching pair already extends to
three bytes (distance 66); 65 of 70 pair candidates extend to three bytes.
A simple nearest-first search-depth cap cannot suppress that alternative.

The broader twelve-ROM general-LZ check also finds TBS resource e2 / TLA 1b2:
at decoded offset 7,804 of 8,192, the ROM uses length 3 at distance 4,124.
Allowing the full distance range in the last 512-byte input block explains
this and Western TBS 3b4, and matches all 280 eligible English TBS resources.
It is rejected: in every TLA edition it breaks previously exact resources 22e
and 232, selecting distances 4,128 and 4,127 at offsets 12,633 and 12,447.
Do not install that final-block heuristic or tune its cutoff to these examples.
The runtime decoder has no reserved-code escape explaining the disputed long
distances. First lazy replacements have different lengths across overlays, so
a single rewind length does not explain the common window limit either.

Packing is separate. A diagnostic replay of preceding physical resources into
one reused buffer explains the padding of 91/96 English TBS overlays, including
55/60 with nonempty padding. Five disagree (370, 376, 37b, 37d, 3c7). Replaying
only earlier overlay bodies explains 88/96. These comparisons used reference
bodies as an oracle; they do not establish a source-built packer. No captured
buffer seed, padding table or resource-specific branch is admitted. Recover
file write lengths and buffer lifetime before removing the legacy plans; their
compact predictor form was bookkeeping reduction, not complete recovery. See
[AI cheating](#ai-cheating).

A follow-up packing probe compared only resources with a forward next pointer:
95 TBS overlays per edition and 113 English TLA overlays. Replaying preceding
physical resources and then each selected stream body explained 90/95 in both
Japanese and English TBS and 113/113 in English TLA. The same five TBS resources
failed in both editions. Writing both candidate codecs into that buffer before
the selected body regressed to 35/95 English TBS, 21/95 Japanese TBS and 25/113
TLA; reversing their order did not help. Separate codec buffers scored 71/95,
62/95 and 90/113 respectively. Two- or four-byte zero flushes, with or without
the tag included in alignment, also regressed. These are rejected packing
models, not production options. The probe used oracle bodies to isolate packing;
even the perfect TLA padding comparison does not prove a source-built packer.

TLA world-map recovery independently regenerated all 1,079 chunk bodies and
37 of 38 associated resource bodies from maintained input with the same LZSS
compressor. Their 1,116 explicit token lists are removed; absent `tokens` means
ordinary compression from input, not a lookup or fallback. Resource 1b2 remains
unresolved at the already documented distance-4,124 discrepancy. Existing
nonempty padding records remain explicit debt, including three zero bytes after
the final chunk; do not call this complete world-map packing recovery.
The following TLA direct-asset sweep removed 79 further stored control records
across eight area/shared-graphics files after complete region comparisons.
Three shared-graphics candidates remained one byte shorter than their declared
streams; their existing recipes were retained, without adding padding or another
override. Resume from these failures and the remaining recipes, not the already
removed controls. The temporary migration code was removed.

Tag-2 tile graphics use greedy copies and update their nibble move-to-front
table only for literals. Input-derived controls match 267/270 English TBS and
482/485 English TLA directory streams. The six failures use long-distance
copies near the input end; applying general LZ's lazy rule regresses this codec.
Automatic tag-2 encoding and a further maintained-input sweep removed 1,341
control fields (1,307 TBS, 34 TLA) across 88 files: 1,017 stored decisions and
324 empty predictor settings. Complete asset builds still
reproduce 2,575 TBS regions and 1,381 TLA regions. Remaining padding and recipes
are not recovered by those removals; the temporary exception in AI cheating
governs that retained debt.

Arena animation compression needs its own parser recovered. A ten-bank,
458-compressed-frame probe found only 28 exact frames with oldest-first greedy
matching, 87 with strictly-longer one-byte lazy matching and 10 when lazy ties
also defer. Nearest-first ties regress; a four-byte minimum is disproved by
actual three-byte copies. Global minimum-payload parsing agrees with the
reference cost in only 316 frames, or 322 when restricted to longest matches;
the other reference frames cost one to three more bytes. Neither optimizer is
an exact compressor. A short diagnostic also rejected advancing the dictionary
scan by the previous match length. These probes used existing arena bodies to
isolate parsing, not as an admitted encoder input. Do not install these models
or preserve new exceptions; recover the remaining dictionary/parser behavior.

The September 20 public-source search found later reconstructions, not Camelot's
original compressor: romhack/GoldenSunCompression uses different window and
lazy-match rules, and gamemasterplc/camelotgcdatatool uses Yaz0-style parsing for
the later GameCube format. Standard GBA LZ as implemented by pret/gbagfx has a
different header and copy encoding. None establishes a drop-in exact encoder.

### The Lunpa standard

Read `SRC/FIELD/COMMON/KUUPUAPPU_RUNPA` and `RUNPA_SUHARA` as the finished-module
examples. Names follow messages, call sites and consumers; types and symbolic
IDs replace raw addresses and arithmetic. Keep coherent responsibility-based
files and header prototypes, no alias defines, per-file bindings, BODY.INC
wrappers or variant scaffolding. `FIELD_EVENT.H` services bind through named
Engine entry points and reproduce the separate argument loads; bindings from
the unit win over legacy aliases. Do not impose these wrappers on another
function without checking its actual call interface.

Typed const tables and consumer-derived macros live with their readers; a unit's
`data` record links rodata through `AlchemyData_<address>`. Listings keep veneer
includes, placeholders and alignment. Unexplained data stays on the explicit
`recon/showcase.json` allowlist and is reported. Comments describe the game.
Temporary register, pool and compiler reasoning stays in the bounded workspace;
only facts that correct maintained source or this guide survive. Rescore every
affected unit and keep overlay audit clean. `alchemy check showcase` enforces
registered folders and their headers; never unregister a folder to pass it.

### Editions, instances and both games

TBS editions share source. `INCLUDE/VERSION.H` selects exactly one of
`TBS_EDITION_JA`, `EN`, `DE`, `ES`, `FR`, `IT`; English is production default.
Only measured code changes justify conditionals or source variants. Keep text
and relocated addresses in assets/bindings, never copied language source trees.
Optional unit `editions` declare complete reviewed owner addresses/extents,
variants and explicit symbols where sites change. Candidate lengths and offsets
cannot invent these facts. `alchemy cross-edition --edition-build OUTPUT OWNER`
proves the selected complete owner; `--all-overlays --edition-build` checks every
member of shared units for six editions. Failed editions fail the command.

A module linked into multiple images is one source unit with `instances`, each
with every member's complete placement and explicit differing bindings, gaps
and editions. Each member calls the same main target or its instance's named
member; do not substitute address macros. `alchemy score --unit ID --all-instances`
proves the family. Parking a member parks that instance. Units with linked
`data` have no instances. A `main` instance needs its own imports/placements,
without overlay fallbacks or gaps. Check siblings before and after adoption;
all binding-equivalent twins join the unit in the same batch. The adoption gates
and `make verify` enforce that; JA/TLA similarity leads remain uncredited.

Source shared by both games lives once in `games/COMMON/SRC/<module>` only after
both independently compile it exact. Declarations live in
`games/COMMON/INCLUDE/<module>`, included through each game's INCLUDE. Registers
use `../../COMMON/SRC/...`; each game retains its own compiler route, placements
and ROM. TLA `alchemy check tla-owners roms/tla-en.gba` verifies complete main and
overlay owners and full overlays carrying assembly credit; it does not rebuild
all TLA. Its missing consumers or one game's missing registration fail sharing.
Only nested .C/.H belong under COMMON. Prove Japanese and cross-game correspondence
before making sharing claims; assets need their own evidence.

## Assets and local viewers

Track editable inputs only when their build consumes them. Runtime tables with
known consumers become typed C. JSON owns reconstruction registries/layouts,
PNG pixels, WAV samples, MIDI sequences and UTF-8 PO localized text; do not add
formats just to rename metadata. Encoders derive whatever they can: layouts are
caller data, not hard-coded Golden Sun logic inside portable codecs.

Area-exclusive assets follow proved consumers. Shared maps live in FIELD/COMMON,
shared tiles/palettes in GRAPHICS/COMMON, character banks in GRAPHICS/CHARACTER,
glyphs in GRAPHICS/FONT and mixed UI banks in GRAPHICS/TILE. Sound assets use
SOUND/{SEQUENCE,SAMPLE,INSTRUMENT}, audio runtime uses SRC/SOUND, and messages
use TEXT. No generic ASSETS tree. Confirm character names from Japanese messages
and runtime descriptors before `CHAR_<ROMAJI>.PNG` or `BATTLE_<ROMAJI>.PNG`;
unproven/shared banks remain CHAR_COMMON.PNG / COMMON.JSON. Palette indices and
all 16 palette bits are preserved. Frame order and runtime association outrank
visual resemblance. The glyph table is 224 records of two-byte advance plus 15
two-byte bitmap rows, not 7,168 bytes of undifferentiated pixels.

`SOURCE.JSON` owns checksummed private map/graphics inputs, even when named PNG
or JSON. `alchemy build assets --extract-sources ROM [--target tla-en]` restores
them from the registered local ROM; `--extract-missing-sources ROM` installs only
absent inputs. Ordinary builds restore missing private sources, then encode and
compare; `--source-only` builds without ROM access. This is not permission for
fallback code or guessed blank map edges. ROM_HEADER.JSON omits the cartridge
logo, whose BIN remains private. Byte equality proves storage, not image geometry,
colors or behavior; verify consumers or emulate before rearranging artwork.

Legacy compression plans and COMPRESSION.TOKENS record unresolved encoder
choices. They are debt governed by [AI cheating](#ai-cheating), not an accepted
end-state asset format. The former `--compact-plans` and `--derive-plans` commands
are removed: packing recorded answers into a binary table or predictor exceptions
did not recover compression. Recognizing the COMPRESSION.TOKENS record format
never justified publishing its answers. Only the frozen-debt exception in
AI cheating temporarily permits the unchanged table. Its legacy
reader remains a build dependency pending recovery, not an approved end state.
Overlay export and asset-index generation now refuse encoder mismatches,
unsupported compressors and trailing reference padding. The former fallback
that manufactured predictor exceptions or retained explicit tokens is removed.
An export refusal is an honest recovery gap; do not restore that fallback or
write its answer by another route. Existing recipes still need removal.
Replace the remaining plans, exporters and readers with the actual compressor
and packer. The three-byte trailing-padding check does not prove recovery.

TEXT/{JA,EN,DE,ES,FR,IT}.PO uses numeric msgid, editable msgstr and context
`message`. Named commands and explicit unknown glyph tokens preserve controls.
The encoder derives Huffman models, lengths and directories; equal keys need
not mean equal text across editions. `--extract-text [TARGET]` reconstructs exact
catalogs; `--verify-text [TARGET]` rebuilds archives. `recon/text.json` owns layout
and identity. Archive equality does not claim a regional ROM build.

GRAPHICS/REVIEW.JSON owns identification. `--review-images OUTPUT` regenerates
private sheets; `make review-images-check` checks sorted names, dimensions,
indices and RGBA independently of PNG compression. Deliberate presentation
changes require visual review before `--update-baseline`. All previews remain
under stable ignored output directories, never tracked or published. Root
PROGRESS.svg is the sole public coverage figure: 830-wide viewBox, 9:16 aspect,
repository names/measurements only, no embedded game font, image or sound.

The local dashboard at 127.0.0.1:4650 separates actual Files (disk bytes), ROM
coverage (physical cartridge bytes), Music, Maps and Text. Coverage uses complete
physical indexes from `alchemy coverage audit --target TARGET --data` or `--all
--data`; unique verified correspondence transfers kind labels across editions,
compression recognition alone does not. Every physical byte appears once.
Audits are explicit, calibrated against TBS, write candidate reports and never
silently replace the committed executable inventory. Current inputs invalidate
stale reports. The dashboard watches inputs, not its executable: restart after
tooling changes with `make dashboard-restart`.

Music playback is bounded, approximate synthesis, not fidelity proof; missing
instruments refuse rather than substitute General MIDI. Text compares build PO
catalogs by physical key. Maps decode from the checksummed ROM through the shared
assembler, not saved renders; actors, animation and script scrolling are not
simulated. Same-origin routes do not expose arbitrary paths. Only Maps uses a
local browser module; other pages prohibit scripts, remote scripts/fonts are
never loaded. Labels use fixed 13px system text and addresses stay in details.

`alchemy build assets --network ROM --target TARGET -o OUTPUT --scenes LIST
--expand --world` follows exits and assembles rooms until story-dependent exits
or world boundaries. Cyan A tiles cut packed rooms; palette variants can be
story states. Rooms meet door arrivals, world-map direction positions places,
stairs separate floors by three metres (16 pixels/metre), and contained rooms
draw below their containing place. Outputs and 3D viewers contain game pictures
and stay local/private. Picture appearance alone cannot prove placement.

## Tooling index

Prefer existing commands. There are two hosts and two crates; every immediate
tool directory must appear here, enforced by `make tooling-index-check`.
**Alchemy builds, Psynergy reads:** Alchemy owns game policy, paths, state,
compilation, encoding, linking and verification. Psynergy owns portable reading,
decoding, analysis and comparison over explicit input. No game-default ROMs,
owners or compiler routes in Psynergy, no aliases exposing an operation in both.

| Tool | Responsibility |
| --- | --- |
| [alchemy](tools/alchemy/) | Golden Sun command dispatch, twelve-target registry, owner lookup and extraction, source adoption, scene integration, compiler routes and provenance, candidate compilation, bindings, translation units, residual classification, matching catalog, overlay loading, serialization, assembly and audits, ROM stages, asset manifests, map networks, coverage, publication checks and dashboard. Its `compiler`, `recovery`, `score`, `matching`, `overlay`, `coverage` and asset and build modules are project integration. |
| [psynergy](tools/psynergy/) | Portable Thumb and objdump decoding, lifetime analysis, C recovery, normalization and alignment, structural and byte comparison, relocation-masked twin search, bounded C repair enumeration, GCC allocation-dump reading, format conversion, explicit subprocess execution, atomic writes, transactional cache storage, and image, MIDI, WAV, text, pixel, Huffman and LZ codecs. Callers supply addresses, symbols, paths, keys, formats and layouts; no Golden Sun owners, default ROMs or compiler routes. |

| Portable command | Responsibility |
| --- | --- |
| `psynergy decompile` | Recover draft C from an image with explicit base, entry and span; optional name and output. |
| `psynergy disassemble` | Read reachable Thumb instructions in the same explicit image window; no owner lookup or game symbol annotations. |
| `psynergy diff` | Compare two supplied binary files, including length differences; `--width 1\|2\|4` sets the comparison unit. Exit 0 means equal bytes, 1 differences, 2 invalid input. No compilation or relocation. |
| `psynergy repair` | Enumerate one or two caller-named, guarded source repairs. Report the finite space; `--choice N` emits one alternative, optionally to `--out FILE`. No scoring, compiler selection or adoption. |
| `psynergy inspect allocator` | Read existing `.rtl`, `.lreg` and `.greg` GCC dumps from an explicit directory; no compiler invocation. |
| `psynergy convert` | Convert files using the directional formats below. No ROM offsets, engine headers or asset manifests. |

| Golden Sun command | Responsibility |
| --- | --- |
| `alchemy extract` | Resolve an owner and extract its reference bytes under ignored `out/`. |
| `alchemy inspect` | Resolve project call sites and symbols; `--asm` adds annotated owner disassembly; `--siblings` lists relocation-masked twins across images with status and binding equivalence. |
| `alchemy score` | Compile a candidate or whole declared unit with the approved route and compare its complete owner, including bindings and overlay serialization; `--unit ID --instance IMAGE \| --all-instances` scores unit instances, including explicitly declared main-image placements. It prints the scored owner's twin count and aligned halfword binary similarity (see [Completion](#completion-and-measurement)). |
| `alchemy match` | Resolve an owner, obtain a decoder-named repair, then compile and score bounded Psynergy alternatives under project policy. `--acceptance-test` checks the five catalog fixtures. |
| `alchemy adopt` | Verify and install a standalone overlay candidate; main integration uses `alchemy check integrate`. |
| `alchemy unit` | `scaffold` declares a main unit; `flatten` consolidates a verified overlay under project ownership. |
| `alchemy bootstrap` | Build and install a missing compiler toolchain from pinned sources. `--check` validates without building; `--build` rebuilds; `--from BUNDLE` imports an admitted distribution. |
| `alchemy build` | `compilers`, `asm`, `claimed`, `full`/`rom`, `assets` and `allocator`. Compiler source builds do not install a distribution. The allocator stage generates canonical GCC dumps for Psynergy inspection. `assets --network` draws map networks and assembles worlds ([Assets](#assets-and-local-viewers)). |
| `alchemy verify` | Run the staged repository's verification contract. |
| `alchemy coverage` | Rebuild and publish project coverage. `audit --target TARGET` inventories every ROM resource-directory pointer, physical spans only for byte-reproduced compressed streams, candidate executable overlay spans from canonical streams and assembler source-line evidence, and the bounded main image as the exact complement of ROM-verified asset regions. Raw pointers are hierarchical and never treated as file extents. `--calibrate` must reproduce the completed TBS audit before any executable method can become authoritative. The audit writes a candidate under `out/`; it never edits the committed scoring manifest. |
| `alchemy check` | `publication`, `commit-progress`, `source-tracking`, `owners`, `tla-owners`, `retained`, `coverage`, `integrate`, `no-asm`, `plan-tails`, `overlay-data`, `progress`, `routes`, `showcase` and `siblings`: repository contracts, not portable file operations. `progress` combines the canonical executable inventory with the current verified build receipt ([Completion](#completion-and-measurement)); `--json` reports DONE and exact C separately, and `--write-report` writes that same result under `out/`. |
| `alchemy cross-edition` | Compare reviewed owner correspondence across Golden Sun editions. |
| `alchemy overlay` | `adopt`, `park`, `audit` and `export`: Golden Sun loader, resource integration and byte-identical retained-source export. |
| `alchemy dashboard` | Serve Files, ROM coverage, Music, Maps and six-edition Text debugging tabs locally. |
| `alchemy format` | Format native JSON; `--check` gates formatting and uppercase names. |

Retired entry points are rejected, not forwarded. Use Psynergy for `decompile`,
`disassemble`, `diff`, `repair` and `convert`; annotated owner disassembly is
`alchemy inspect OWNER --asm`. Old commands and logs are disposable diagnostics,
not instructions to resurrect aliases. `alchemy score --target tla` selects
TLA explicitly; arbitrary ROM overrides cannot substitute a reference. Scoring
one exact unit member still verifies the unit. Default work lives in out/score.

Portable tooling is capped at 100,000 Rust/TypeScript/JavaScript/CSS lines by
`make tooling-size`. Only Pascal can change scope or ceiling. New machinery
must fix a demonstrated recurring blocker, reuse/replace existing code, and
prove a conversion with regression coverage. Do not hide growth in wrappers.
`alchemy format` preserves JSON values, field order and boundaries using the
native two-space/120-column style and packed short tuples.

`psynergy convert FORMAT INPUT OUTPUT` supports decode-lz, words2bin, pairs2bin,
tilemap2bin, png2bpp4, bpp42png, png2bpp8, bpp82png, png2bgr555, wav2pcm8 and
pcm82wav. Bpp is tile-major, palettes little-endian BGR555, PCM8 signed; reverse
tiles need palette/tiles-wide, PCM-to-WAV needs rate, WAV input is mono 8-bit.
Invalid ranges, transparency and overwrites refuse. Engine headers stay in game
manifests. Encoding/repair operations still crossing the builds/reads boundary
are open work below, not permission to invent a third host.

## Build, verify and commit

Install Rust and Ninja, supply checksum-approved ROMs under ignored `roms/`,
initialize submodules in the main checkout and install hooks:

```sh
git submodule update --init
git config core.hooksPath .hooks
make compiler-source-check
make bootstrap
./alchemy --help
./psynergy --help
```

Bootstrap builds pinned agscc/agbcc and checksum-pinned official GNU binutils
2.10 and 2.33.1, retaining upstream source untouched under out/compilers.
It installs six hash-checked executables under tools/compilers and native modern
binutils under tools/binutils; the launcher sets PATH and builds tooling offline.
`bootstrap --check` validates, `--build` builds, `make bootstrap BUNDLE=PATH`
imports an admitted distribution. It refuses different existing installations
and never admits hashes. `make compiler-sources` builds without installing or
admitting binaries. A source build is not necessarily an approved distribution;
new hashes/hosts still need Pascal's authorization and reproduction evidence.
Worktrees can link the main checkout's roms and complete tools/compilers.

Use narrow scores/builds while iterating: build-claimed, build-asm, build-assets,
overlay-check and check-owners. Full assets, coverage and twelve-edition checks
are not part of every source spelling. Before committing, stage explicit paths:

```sh
git diff --cached --check
make verify
make progress-subject
```

The index/worktree must agree. Verify rejects unstaged tracked changes and
untracked files, and checks the staged source, full TBS EN ROM, overlays,
units/instances, owners, siblings, ordinary C, compiler provenance, documents
and tracked publication. It does not rescore all drafts or rebuild twelve ROMs.
`make verify-clean` deletes generated output and runs the gate from scratch.

| Changed surface | Additional check |
| --- | --- |
| Ownership, labels, coverage | `make coverage` |
| Rust tooling or dashboard | `make test` |
| Shared edition/preprocessor logic | `make targets` (compile-only) |
| Assembly classification | `make classification-check` |
| Candidate-corpus policy | `make candidate-corpus-check` |
| Tooling or documents | `make tooling-index-check` |

Coverage reads current verified receipts; rebuild stale source evidence first.
Use `make audit` for exhaustive release/compiler/ownership checks and
`make reports` to regenerate analysis, not in each edit loop. Report a known
unrelated gate failure honestly; never reset unrelated work or bypass it.

Commit subjects start with `make progress-subject`'s exact prefix, and agent
commits include their Co-Authored-By trailer. The hook requires the verified
staged tree. Push only when Pascal asks, only main, after coverage, verify and
(tooling changes) tests pass on the committed tree. The pre-push publication
gate checks outgoing history. No unrequested history rewrite, compiler/route/hash
change, credit-standard change or tooling-ceiling increase.

## Open work

Maintain priorities here, not another checklist. Remove completed items and
update invalidated facts in the same change. Dossiers hold only each owner's
current state; detailed experiments are disposable. Never preserve an old blanket
“bounce” list as authority over improved evidence or another game at the same address.

### TBS recovery

- Pursue the active percentage goal through the ranked largest unresolved
  functions and their exact dependencies, using the recovery loop above. Read
  the current count with `make progress`; no second manually maintained total.
  Main recovery is required; the remaining overlays alone cannot reach 75%.
- Finish credible complete drafts across coherent modules, then tighten them by
  shared causes. Missing drafts count as uncovered, but audit retired/surviving
  C before claiming a function was never drafted. Discard misleading drafts,
  salvage supported behavior, and do not chase local scheduling before fixing
  structure, types, boundaries and calls. Draft coverage is not quality proof.
- Use the reviewed DMA primitive for surrounding C. Remaining save/checksum,
  zero initialization, rendering generation and decompressor callers need their
  own structure/lifetimes recovered. Fill macros are unadmitted. Save checksum
  and heap-decompressor residual searches are parked until new evidence appears;
  the VRAM cache and DMA unit context is available to callers.
- September 19's exact-twin census found no uncredited matches among 4,235 scanned
  owners; reopen only after source, membership, bindings or inventory changes.
- September 20 nested-function recoveries prove particle helpers (200 bytes),
  tile helpers (168) and link helpers (464) through real enclosing contexts.
  Enclosing parents remain retained. Reuse shared VRAM/render/battle types and
  verify unsigned runtime targets before revisiting their callers. Preview and
  target-selection nested helpers remain scored drafts, not adopted C.
- Continue world-map module recovery at resource_371:020039fc, 020024a8,
  02002768, 020027dc, 0200384c, 02004058, 020001c4 and 020006ec from their
  maintained drafts and interfaces: transfer-queue behavior, table loops, prototypes,
  field access and ip-return/Q16 multiply context precede allocation searches.
- Revisit Korima Bridge resource_391:02002974 only with new early-zero lifetime
  evidence; resource_38f:020027ac has different bindings. Debug item menu
  resource_3ce:02000cf4 needs DMA source form and loop order; alignment at
  02001186 remains unresolved. Raribero resource_3c7:020000c8 is one 148-byte
  owner with a missing audited literal at 02000150..02000154; repair the audit
  before claiming complete overlay recovery.
- Recorded floors such as resource_3bd:020013f8, the SpawnConfiguredEffect
  family, and main allocation/scheduling residuals require new evidence, not
  repeated spelling searches. Check current source and fresh scores: past
  classifications and stale names are not proof of impossibility.
- Extend the Lunpa source standard along these dependency paths. Remove legacy
  address names from remaining COMMON/LIB interfaces as verified callers migrate.

### TLA and twelve editions

- Repair the compile-only TLA script-operand unit contract: 38 exact owners are
  declared but OPERANDS.C defines three additional unresolved named helpers.
  Declare full membership without crediting those helpers. Earlier GET_BYTE.C /
  GameFlagBytes binding failures are historical leads; rerun to identify the
  current first failure rather than keeping competing diagnoses.
- Only TBS EN supports full-ROM build. Preserve guards on the other eleven
  targets until each has complete edition link layouts, source/assembly bindings
  and regional asset manifests and an independently byte-identical full image.
  All twelve physical indexes and exact text archives are separate achievements.
- Grow TLA through complete verified shared modules and its own fixed executable
  inventory. Keep the existing object/motion, inventory and Djinn recoveries;
  small scheduling floors remain uncredited. Do not inherit
  TBS compiler-family membership from a name or address resemblance.
- Pascal approved a bounded TLA shared constant-lowering experiment on September
  19. It ended without installing an experimental compiler. Broader synthesis
  regressed exact message owners; keep one game-wide option set and existing controls.
- Continue TLA main:08120454 using exact TBS main:080bbb0c as its control.
  Reconstruct command access, field layouts, raw power, counter lifetimes and
  guards; neither the approved nor experimental candidate is exact. Measure it
  afresh instead of preserving a history of intermediate scores.
- TLA staged actors resource_64e:02000314–0200056c remain nonexact shared-module
  candidates; two hypotheses failed and an 18-overlay survey supplies leads only.
  Field operands 08025bb4, 08025c5c, 08025f9c remain scheduling floors. Do not
  repeat their five failed hypotheses or treat layout-only C as credit.
- Venus resource_64d:02000510 has its complete 5,404-byte draft maintained at
  recon/en/overlays/resource_64d_c_02000510.c. Fix the lifter's
  duplicated lookups, extra end calls and volatile actor RAM before repeating
  this family. The first constant-synthesis mismatch is not fixed by equivalent
  integer spellings.
- Keep 28 further overlay exports private: their raw integer payloads do not
  become publishable because the complete images reproduce. Their owners/results
  have current entries in the TLA registry. Correct DAIRA to DERI using Japanese message 0xe62 in a
  verified migration; Japanese place-name base 0xe5a differs from English 0xe58.
- Finish edition-local data identification and Japanese correspondence from
  actual readers and byte proofs, not broad AI guesses or copied source trees.

### Remaining project work

- Complete the Alchemy-builds/Psynergy-reads boundary for encoders and source
  repair operations still in Psynergy, updating callers and this tooling index.
- Close unknown executable gaps, reduce raw overlay data directives, derive
  remaining compression lookahead/recipes, identify music titles from evidence,
  and place exclusive assets with proved consumers. Admit other compiler hosts
  through the existing evidence/approval process.
- Map viewer: derive TBS walking/collision boundaries, add story-state and
  within-floor draw-order controls, resolve the twelve Anemos/Atteka inlet links
  offset by 16–80 pixels and scene 239's warp semantics, and assemble remaining
  world families. No guessed geometry promoted as fact.
- A future demo intercuts the end of TBS with the start of TLA. First make every
  required scene exact in both ROMs; staff rolls and title screens are excluded.
- At 100%: twelve independently exact clean builds, evidenced source sharing,
  maintained editable assets and coherent modules; then open contributions.
