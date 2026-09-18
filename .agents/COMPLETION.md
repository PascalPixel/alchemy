# Completion

## What counts as complete

Every executable byte, including overlay gaps, must have a complete audited
owner or explained nonfunction role. Exact C means ordinary production C emits
the complete linked extent with zero differing halfwords under the approved
route. Names, plausible behavior, matching size and fuzzy scores are not proof.
Each instance owner is credited once, in its own image; sharing a source
neither multiplies nor reduces credit.

Each game has its own DONE: **☀️** for The Broken Seal and **⚓️** for The Lost
Age.

**DONE = common permanent assembly + common C + game permanent assembly + game
C**, divided by that game's audited executable bytes. Common parts are credited
bytes whose source lives under `games/COMMON/` and compiles for both games; game
parts are the game's own. Each game counts its own images once, so a shared
source adds to both games' DONE, each over its own bytes. A game whose
executable audit is incomplete has no denominator: its DONE is pending, shown as
`?` in the commit prefix and "pending" in the README, never estimated.
`make progress` prints both games with their four parts.

Each game has exactly one committed scoring authority at
`games/<game>/metrics/executable.json`. Its state is `pending`, `audited` or
`verified`; `pending` never produces a numeric score. A numeric denominator
requires a complete main-image audit and every declared overlay, with each
overlay's executable intervals and their excluded complement accounting for
its complete decoded image, with both byte totals checked. Candidate audits
live under ignored `out/`, carry state
`candidate`, and cannot drive ☀️, ⚓️, the README or the dashboard.

Successful source verification writes `out/<target>/reports/verified-code.json`:
the ROM hash, build-input fingerprint, and credited image ranges with source
and category. TBS's complete production build produces it; TLA's owner check
produces it without claiming a complete TLA rebuild. Changed code, headers,
bindings, compiler inputs or reference ROM invalidate the receipt. The scorer
unions its ranges separately in each image against the fixed inventory and
counts C before assembly. COMMON attribution partitions those bytes; it never
duplicates them. The dashboard, README and commit prefix consume that score.
Rendering tiles cannot create credit. DONE and exact-C percentages are named
separately in the same report. Reading a score never runs a build or audit.

Permanent assembly is the reviewed retained-assembly category, not every
unresolved function temporarily assembled to make the ROM build. Compiler
runtime built from the licensed containers counts in that category as
explained, as pret counts linked libgcc; bytes restored from the ROM as a private input never
count. Drafts and unknown code earn no credit. Report exact-C share separately;
converting already credited assembly to exact C improves that share without
increasing DONE. A match that needed a scheduling trick earns nothing
([COMPILER](COMPILER.md)).

Report exact C, unresolved bytes and complete-ROM target coverage separately.
Historical Proven C and Proven ASM labels describe build routes, not knowledge
of Camelot's original language. All twelve targets must eventually rebuild
byte-identically from a clean checkout and approved local inputs;
correspondence checks and compile-only targets do not establish this.

Recover Japanese correspondence before calling English source shared. Measure
actual localization differences and TLA behavior; do not count one recovery
twelve times. Assets must rebuild from maintained inputs, and final source
must be organized into evidenced modules with shared interfaces.

## Retained assembly

Overlay assembly credit is per reviewed range. Apart from the fixed overlay
veneers below, which are the sole exception, a `proven` range must carry
`provenance.credit` of `library` or `handwritten`, evidence, and a nonempty
`provenance.proof` or `provenance.object`, by the same rule as main-image
assembly. A credited range does not credit other ranges of the same kind;
strong reasoning and bare credit labels remain uncredited.

Retained-assembly credit does not establish handwritten or third-party origin.
Search exhaustion, register mismatches, compiler non-emission, repeated
scripts and large functions are not evidence of authorship. Keep unresolved C
candidates in the recovery queue rather than relabeling them to increase DONE.

The classification records remain: main evidence is exposed by the assembly
manifest, overlay evidence lives in
`games/THE BROKEN SEAL/semantic/overlay-assembly.json`, and both are parsed and
validated from the same inputs. Reconstructed scenes use
`structured_scene_module`, not the retired `generated_call_script_module`.
Only Pascal may establish a replacement standard, and it requires positive
evidence of real handwritten or third-party assembly, such as a verified match
to historical assembly runtime source or an independently established
hand-authored machine interface.

## Overlay veneers

Pascal authorizes DONE credit for reconstructed fixed overlay veneers. Use the
shared `overlay_veneer` assembly macro in
`games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC`, preserving each target word and
bank position. Verified entry and import lists live in the overlay's existing
Japanese-named `SRC` directory as `ENTRY.INC` and `IMPORT.INC`; retained
assembly includes them at their original positions. Each credited record uses
kind `veneer`, confidence `proven` and `provenance.credit: reconstructed_veneer`,
with that macro path as its proof. The range must contain complete eight-byte
veneers, start word aligned, lie entirely inside audited veneer intervals and
reproduce the canonical overlay and ROM exactly. This is assembly-linkage
reconstruction credit; it does not identify Camelot's original author, macro
or generator. All other assembly keeps the `library` or `handwritten` evidence
requirement.
