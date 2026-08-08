> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

# Alchemy project audit and forward plan

Audit checkpoint: 2026-08-02, `main`. This is the single authoritative current
handover. Historical studies remain useful evidence, but their counters and
queues are not live project status.

Verification status: `make test`, `make build-semantic`, and
`make coverage-check` pass at this checkpoint. The normal build remains
byte-identical, source ownership is complete, and ROM fallback is zero. The
complete `make verify` gate last passed before this semantic-only tranche
and should be rerun before the next checkpoint commit.

Operational tooling is now native Rust under `tools`, with the Makefile as
the supported command surface. TypeScript and Python names retained below
identify the implementation that produced a dated result; they are historical
evidence, not live commands or current paths.

## Executive verdict

Alchemy is a complete, byte-identical, source-owned reconstruction of the
8 MiB GS1 English ROM image. It is **not** a fully decompiled C project.

- Exact Full-C Byte Share is **266,832 / 1,343,470 executable bytes
  (19.86%)**.
- Reviewed semantic C adds **949,778 executable bytes**. Exact plus semantic C
  covers **1,216,610 / 1,343,470 bytes (90.56%)**.
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
| 96 decoded code overlays | 161,780 | 536,952 | 52,826 | 43,548 | 795,106 |
| **Total** | **266,832** | **949,778** | **52,826** | **74,034** | **1,343,470** |

Additional ROM-image facts:

- Total ROM image: **8,388,608 bytes**.
- Data/assets outside the executable denominator: **7,298,755 bytes**.
- Compressed code-overlay streams occupy **541,489 ROM bytes**; their decoded
  executable size is counted in the separate 795,106-byte code-overlay namespace.
- The source-only full build owns **8,388,608 / 8,388,608 bytes**, uses zero
  fallback, and reproduces the reference ROM byte for byte.
- Diagnostic source counts are 1,428 main-image C files, 1,834 exact code-overlay
  C files, 701 semantic main-image C files, 1,587 semantic code-overlay C files,
  and 41 headers. These are useful inventory counts, not progress percentages
  or reliable function counts.

The semantic compiler reviews 976,622 bytes of owner spans, but 26,844 of those
bytes lie outside the audited executable extents, chiefly pool/tail portions of
code-overlay owner spans. The coverage numerator therefore uses 949,778, not
976,622. `make build-semantic` now reports both figures instead of silently
adding out-of-scope bytes.

The continuous semantic-overlay campaign has reduced the ranked
`CODE-SUSPECT` queue from 35 to **11 gaps** at this checkpoint. Recent complete
closures include `resource_3a5`, `resource_3bb`, `resource_381`, `resource_376`,
`resource_39b`, `resource_39c`, `resource_3b0`, `resource_3b3`, `resource_3bf`,
and `resource_3c8`, plus bounded tranches in `resource_3a0`, `resource_3a6`,
`resource_3b4`, `resource_3ba`, and `resource_3be`. Every admitted owner is
ordinary C: the repository-wide gate currently scans 5,592 C/H sources with
zero forbidden assembly escapes.

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
| Ordinary compiler-output debt | 585 | 348,300 |
| Other retained structural/low-level rows | 1,171 | 95,012 |
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

`make core-retained-check` is now part of the full verification gate. The
dashboard uses natural source/owner boundaries instead of artificial roughly
equal blocks and reserves dark gray for evidence-backed retained assembly or
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
- Additional semantic C: **536,952 bytes**.
- Remaining without exact or semantic C: **96,374 bytes**, including 43,548
  bytes of evidence-backed retained assembly.
- Current semantic-backed exact reading list: **546 owners / 275,078 bytes**
  across 67 code overlays.

The largest reading-list portfolios are `resource_373` (16,440 bytes),
`resource_3bf` (12,596), `resource_3c8` (10,194), `resource_372` (9,400),
`resource_38f` (8,988), `resource_371` (8,894), and `resource_383` (8,236).

Discovery is broad but noisy: the live inventory reports 11,954 unconverted
discoveries, including 10,335 contained rows, 9,987 data walks, 126 structural
veneers, 928 ordinary discoveries, and 477 ordinary prologue/return rows.
These filters overlap; their counts must never be added as distinct functions.

The exact-twin report now shows only **116 theoretical recoverable bytes** in
two families; `tools/overlay-twins/target/release/overlay-twins --semantic --unconverted` reports
zero, proving there is no remaining known twin-template shortcut for semantic
closure. The new semantic mode prevents the exact-C queue from being mistaken
for unowned code and makes that negative result immediate rather than manual.

`tools/overlay-call-order-check/target/release/overlay-call-order-check` is the other current speedup. It compares
each semantic owner's source-level postorder call sequence with its reachable,
veneer-resolved BL sequence, including conservative IWRAM call-through recovery.
Use it on every new owner alongside the multiset check. The latest ranked
closure admits `resource_3b1:38ac` (1,124 owner bytes), `resource_3b1:48e8`
(1,728), `resource_3bc:2ee8` (1,264), and `resource_39c:2318` (492), followed
by `resource_3b1:4254` (892), `resource_3ba:1e7c` (620),
`resource_3bb:2450` (1,264), and `resource_3bc:2bac` (620). All eight pass the
targeted multiset, ordered-call, and m2c coverage checks. The first four add
2,926 executable semantic-C bytes after pool/tail clipping; the second four add
another 3,396. The project-wide diagnostic now covers 1,150 owners; the latest
four add four passes to the prior 914-pass/214-mismatch audit, so `--all` remains
an audit queue rather
than a green gate; the self-test is wired into `make test`.

`tools/overlay-show/target/release/overlay-show <overlay> <start> <end> --annotate` now performs the
disassembly, overlay-specific BL resolution, and completeness check in one
command. Both offsets and full `0x02000000` RAM addresses are accepted. This
removes the repeated two-command pipe and prevents an absolute address from
silently producing an empty binary. The first owner read through this route,
`resource_373:2a54`, closes a 604-byte one-return gap with all 45 calls matching
in multiset and source/machine order.

The next family pass closes another **5,936 executable semantic-C bytes** in
17 independently verified owners: the 480-byte state driver in resources 3ba,
3bb and 3bc; the 384-byte collision/movement owner in 39b, 3a6, 3b3 and 3be;
and the 296-byte actor-rectangle scan in ten overlays. Shared control flow was
used to amortize reading, but every receiving overlay resolved its own table or
script addresses and passed its own multiset, ordered-call and m2c coverage
checks. The retired `m2c_guard.ts` ignored unresolvable BL-shaped words in skipped
literal data, matching the existing multiset rule instead of demanding a
fictional C call.

The following ranked pass closes `resource_371:0a8c` with another **412
executable semantic-C bytes** and 48/48 ordered, veneer-resolved calls. It also
corrects the adjacent `resource_371:06ec` registration from 880 to its already
source-documented 928-byte extent: the missing 48 bytes were the end of that
dispatcher's literal pool, not the start of the new function. The ranked queue
therefore drops the former 460-byte combined gap cleanly, with code and pool
ownership kept distinct.

The same ranked tranche then closes `resource_3c4:1aba` (370 bytes, 26/26
ordered calls) and the paired interpolation/display-cycle owners
`resource_3ba:2bec` and `resource_3bc:391c` (316 bytes each). The pair shares
control flow but uses independently resolved state addresses and independently
passes every targeted gate. In total this tranche adds **1,414 executable
semantic-C bytes** across four new owners.

The next ranked pass adds another **1,256 executable semantic-C bytes** in five
owners: the three-group formation builder `resource_39c:3948` (284 bytes), the
story-flag-selected spawn/release family member `resource_3ba:30ac` (276), the
terrain-search/spawn owner `resource_3b3:1074` (256), the scene-0x36 setup
`resource_39c:36ac` (220), and the collision-gated tile-center move
`resource_3a0:0d0c` (220). Existing reviewed or exact-C local callees supplied
the private ABIs where available; every owner still resolved its own calls,
data addresses and pool boundary and passed targeted multiset, ordered-call,
m2c coverage and no-assembly checks.

The next ranked owner closes `resource_3ba:1910` with another **208 executable
semantic-C bytes**. Its independently reconstructed 208-byte sibling at
`resource_3bc:2640` supplied the operation and established import ABIs, while
the receiving overlay independently confirmed its mid-owner literal pool and
all eleven calls across six targets. The semantic build, multiset, ordered-call,
m2c coverage, and repository-wide no-assembly gates all pass.

The following ranked pass closes another **904 executable semantic-C bytes**
across five owners: the story-flag-selected sequence/setup owner
`resource_3b1:1078` (204 bytes), the palette-rebuild sibling
`resource_3ba:2ea0` (196), the randomized kind-222 effect spawn
`resource_3a8:3864` (168), the six-candidate same-tile/highest-y selector
`resource_3c8:23d4` (168), and the heading-selected interaction/message owner
`resource_3cb:0d68` (168). The 2-byte alignment at `resource_3ba:2e9e` remains
with its preceding unknown owner instead of being misattributed. Every new
owner passes the semantic build, multiset, ordered-call, m2c coverage, and
repository-wide no-assembly checks; the ranked one-return queue now starts at
164 bytes.

The next ranked tranche closes another **1,028 executable semantic-C bytes**
across seven owners. It pairs the instruction-identical party interaction
owners `resource_3bb:1f14` and `resource_3bc:29ac` (164 bytes each), closes the
two unrelated 164-byte `resource_39c` collision/palette owners at `058c` and
`0d58`, and separates the real 104-byte `resource_39f:2ba4` motion owner from
the 60 bytes of preceding table/pool data that the gap rank groups with it. It
then closes `resource_3b3:26b8` (148 bytes) and `resource_39c:1568` (120).
Family comparison amortized the paired reading, while prologue-first boundary
review prevented data from being mislabeled as C. Every owner passes the
semantic build, multiset, ordered-call, m2c coverage and no-assembly gates; the
ranked one-return queue now starts at 104 bytes.

The following boundary-first pass adds **392 executable semantic-C bytes** in
four owners: the protected-window palette adjustment `resource_392:0abc` (104
bytes), the actor-8 tile trigger `resource_38a:0380` (100), the two-placement
camera script `resource_3b0:040c` (92), and the fixed actor placement/state
setup `resource_3bf:206c` (96). Prologue-first inspection keeps the eight bytes
before `resource_3b0:040c` with the preceding pool instead of claiming the
ranked gap's nominal `0404` start. All four independently pass the semantic
build, multiset, ordered-call, m2c coverage and no-assembly gates; the ranked
one-return queue now starts at 86 bytes.

The next combined ranked/family pass adds **5,714 executable semantic-C bytes**
across 34 owners. Four boundary-first ranked owners contribute 214 bytes while
keeping the import veneers before `resource_39c:0030` and the alignment before
`resource_3c4:1458` out of ordinary C. The main throughput gain is the paired
696-byte movement/redraw and 284-byte nested-kind rectangle family at `0608`
and `08c0`: one control-flow reconstruction is applied to resources 373, 389,
391, 392 and 393, but every overlay independently resolves its key, delta and
direction tables and its call-through callback. Those ten owners contribute
4,890 executable bytes. Twenty adjacent setup, query, transition and leaf
owners add the remaining 610 executable bytes and close the `09dc` follow-on
banks in resources 391-393. Normalizing the instruction skeleton while leaving
pool/table words overlay-specific is the best measured acceleration in this
pass. Every owner passes the semantic build, call multiset, ordered-call, m2c
coverage and no-assembly gates; the ranked one-return queue now starts at 32
bytes.

The next boundary-first pass adds **1,280 executable semantic-C bytes**. Four
independently reconstructed `resource_372` scene owners at `3e18`, `3e48`,
`3fb0` and `40f0` contribute 1,052 bytes. Two 32-byte owners at
`resource_3bb:2e44` and `resource_3cb:0128`, the 28-byte conditional-sound
owner `resource_373:5a08`, and the 24-byte actor-mode owner
`resource_3b3:1428` contribute another 116. The remaining 112 bytes came from
correcting five registrations whose source comments already proved that the
return/alignment/literal-pool tail belonged to the preceding owner: two in
`resource_3bb` and three in `resource_3a4`. Those corrections also exposed and
fixed stale local-veneer names in the two existing `resource_3bb` sources.
Every affected owner passes its targeted multiset, ordered-call and m2c checks.
Two additional prologue-less leaves, `resource_3bc:45d0` and
`resource_389:10c0`, add 24 reviewed bytes outside the executable inventory;
they therefore do not inflate the executable-semantic numerator.

This pass establishes a cheaper first step for the small-gap queue: when a
ranked gap starts on the previous semantic owner's return, compare the
registered span against the source-documented next prologue and referenced
pool before drafting a new function. The check recovered 112 executable bytes
from five stale spans and reduced the one-return queue to 10-byte gaps. Real
prologue-less leaves still remain distinguishable because they begin after the
preceding alignment/pool boundary and pass their own owner-scoped gates.

The following closure pass adds another **9,864 executable semantic-C bytes**
and clears the one-return queue completely. Twelve independently resolved
eight-byte address getters, six four-byte no-op/zero leaves and three corrected
return-tail spans account for 128 reviewed bytes, 16 of them executable. The
first large reuse cohort then splits the repeated 530-byte gap in eleven
overlays into a 56-byte prologue-less integrator and a 472-byte configurable
spawn/copy owner. Those 22 owners add 5,808 reviewed bytes and 5,192 executable
bytes; the integrators lie outside the executable census. The ordered-call
gate caught an inherited error in the older sibling reconstruction: actor 0 is
looked up before the spawn call, not afterward. All eleven normalized sources
now preserve that machine order and independently resolve their variant table.

A second cohort converts five complete owners in each of resources 39f, 3b2,
3c4 and 3c5: the staged 694-byte movement/redraw owner, the 284-byte
kind-selected redraw, a 24-byte owner-bit setter and two 88/100-byte spawn
wrappers. These 20 owners add 4,760 reviewed bytes and 4,656 executable bytes,
with every overlay independently resolving its key, displacement and direction
tables. The remaining 28-byte head of the following integrator is deliberately
not claimed because its tail is already a separate exact-C fragment; preserving
that split is more accurate than manufacturing a whole-owner semantic span.
All 60 targeted multiset, ordered-call and m2c checks pass.

The best measured speedup is now cohorting repeated ranked gaps by exact
boundary shape before reading their bodies. Hash the instruction-only leaf,
resolve every receiving overlay's call multiset and pool words, normalize one
source skeleton, then substitute only independently verified table addresses.
This produced 42 family owners from two detailed control-flow reads while the
owner-scoped gates still caught a real sequencing defect. The ranked queue now
contains 91 multi-return gaps and no one-return gaps.

The first multi-return pass splits `resource_3a8:2000-3184` at the two actual
saved-register prologues instead of treating the 4,484-byte gap as one owner.
The preceding pool ends at `0x02002008`; the first cutscene then owns 1,720
bytes through its trailing pool at `0x020026bf`, and the second cutscene starts
at `0x020026c0`. The first owner is now reviewed semantic C with all **160/160**
veneer-resolved calls matching in both multiset and machine order, including
the sole answer branch and the direct scene/entity field writes. This adds
1,720 executable semantic bytes. The separate 12-byte gap at `0174-017f` is
also split into a four-byte zero-return leaf and an eight-byte literal-address
getter; both pass their zero-call owner gates but lie outside the executable
census, so they add reviewed coverage without inflating the executable metric.
The remaining `resource_3a8` gap is the independently bounded 2,756-byte owner
at `0x020026c0` with 249 veneer calls.

For large linear script owners, the cheaper workflow is now: split on proven
prologues/returns and pool branches first, resolve the complete ordered call
stream once, transcribe calls and the comparatively few memory operations in
address order, then use the multiset and ordered-call gates as immediate
feedback. On `resource_3a8:2008`, that avoided searching for a nonexistent
large twin and turned a 4.4-KiB ambiguous gap into one verified 1.7-KiB owner
plus one cleanly isolated follow-up.

That follow-up is now complete. The 2,756-byte `resource_3a8:26c0` cutscene
passes a **249/249** veneer-resolved multiset and ordered-call reconciliation,
including its three pools, three nullable actor-position copies, four randomized
counters, answer branch, DMA/scene writes and final teardown. Together the two
large owners account for 4,476 executable semantic bytes and 409 independently
checked calls; `tools/overlay-gaps resource_3a8` now reports zero code-suspect
gaps.

The same boundary-first pass then closes 43 smaller owners and reduces the
ranked multi-return queue from 89 gaps to **70**. Nine overlays share the proven
four-byte-zero-leaf plus eight-byte-address-getter split (18 owners). Two
resource_3bb/resource_3bc empty-hook/state-setter pairs, four resource_3b4
hook/heading/blend owners and two resource_3c0 entity owners close another ten.
Four 32-byte DMA3 descriptor writers in resources 392/393, five resource_39c
wrapper owners, three resource_3b3 probe/link owners and three resource_3a0
presentation/effect owners complete the tranche. Every receiving owner resolves
its own data address and passes its own multiset and ordered-call gates.

This continuation adds **3,308 executable semantic bytes** in 44 owners. The
small-owner portion adds 732 reviewed bytes, of which 180 are correctly outside
the executable census; it therefore contributes 552, not 732, to the headline.
The measured acceleration remains decomposition plus exact-shape cohorting:
first split every return into independently bounded owners, then normalize only
when instruction shape is identical and keep all pool/data substitutions
overlay-specific.

The next boundary split closes `resource_37f` completely. The apparent
2,344-byte two-return gap begins with 40 bytes already owned by the preceding
dispatcher's trailing pool, then divides at fresh prologues into a 624-byte
scene-setup owner at `0x02000d1c` and a 1,680-byte scene-sequence owner at
`0x02000f8c`. The first reconciles all **60/60** calls across 20 veneers. The
second preserves its interior pool skip, six nullable entity-record reads,
answer branch, camera choreography and teardown while reconciling all
**174/174** calls across 30 veneers. Both pass syntax, multiset, strict ordered-
call, m2c coverage and no-assembly checks. They add **2,304 executable semantic
bytes**, reduce the ranked multi-return queue to **69**, and leave
`tools/overlay-gaps resource_37f` at zero code-suspect gaps.

A four-gap shape cohort then closes seven more owners across resources 399,
3a6, 3bb and 3bc. The paired 148/84-byte resource_3bb owners have exact
instruction twins in resource_3bc, with all four callback words resolved
independently. Resource_3a6 contributes a four-call scene wrapper plus its
metatile-centered presentation owner; resource_399 contributes the complete
248-byte orbit/randomized-clone owner. The seven recorded spans add 928 reviewed
bytes, 716 of them inside the audited executable census, and reduce the ranked
queue from 69 to **65** gaps.

That resource_399 owner exposed and fixed another throughput hazard in
`tools/overlay-call-order-check`: Discovery treated an inline `mov ip,pc / bx rN`
IWRAM call as a return and silently discarded every later BL. The checker now
detects the reachable call-through idiom and switches to the complete bounded
resolver listing, with a regression test. The formerly truncated owner now
passes all ten direct calls in order; m2c remains correctly refused as a seed
while independently confirming all 9/9 distinct direct callees in the draft.

The highest-yield low-return gap, `resource_3b1:1f3c-2ff0`, is now complete as
two independent scene owners separated at the fresh prologue `0x020027d8`.
The first 2,204-byte owner preserves its branch-skipped thirteen-word interior
pool and all **230/230** calls across 23 targets. The second 2,072-byte owner
preserves three answer branches, the shared scene counter, six nullable record
copies, final teardown and all **203/203** calls across 30 targets. Both pass
multiset and strict-order checks; m2c independently reports 23/23 and 30/30
callee coverage with no truncation hazard. The pair adds **4,276 executable
semantic bytes**, closes the entire former two-return gap and reduces the ranked
queue to **64**.

Two more boundary-first passes close both late `resource_3b1` two-return gaps.
The `5d10-63ac` pair contributes 1,024- and 668-byte owners whose 87 and 63
calls pass multiset and strict-order checks, with m2c covering 29/29 and 23/23
callees. The `50e4-5684` pair contributes 528- and 912-byte owners, including
three nullable actor-position copies, the answer-dependent scene-counter write
and final state publication; its 48 and 80 calls pass the same gates with 24/24
and 27/27 callee coverage. Together they add **3,132 executable semantic
bytes**, leave only three `resource_3b1` code-suspect gaps, and reduce the global
ranked queue to **62**.

The remaining `resource_3b1` gaps are now closed too. The `57ec-5ca4` pair
adds 1,116- and 92-byte owners with 112/112 and 4/4 ordered calls; the
`0728-08a8` pair adds 208- and 176-byte story/answer owners with 19/19 and
17/17 ordered calls. Finally, twelve homologous eight-call story selectors and
four six-call selectors close `1324-1804` as sixteen independently admitted
owners. Explicit per-owner C keeps every branch and constant visible while the
shared boundary/constant-table pass avoids repeating the analysis. This final
tranche adds **2,840 executable semantic bytes**; `tools/overlay-gaps resource_3b1`
now reports zero gaps and the global ranked queue is **59**.

The next boundary-first pass closes five of the six owners in
`resource_3bc:1474-22c4`. The four related state-machine owners at
`1c20`, `1df8`, `1f90`, and `20f0` contribute 1,700 bytes; their 44, 40, 30,
and 40 calls pass independent multiset and strict-order checks with complete
m2c callee coverage. The adjacent 532-byte scene-staging owner at `1a0c`
adds 47/47 ordered calls across seventeen callees. Together the tranche adds
**2,232 executable semantic bytes** and shrinks the remaining head owner to
`1474-1a0c` (1,432 bytes). Because that residual is still one code-suspect
gap, the global ranked queue remains **59**. The full build, coverage check,
owner-scope census and no-assembly gate all pass at the checkpoint.

The adjacent `resource_3bc:232e-25c8` residual is closed immediately after
that checkpoint. The first two bytes were the previously omitted final
`bx r0` of the already semantic `22f4` owner, whose corrected 60-byte span now
passes 5/5 call coverage. The remaining 416- and 248-byte state-machine owners
at `2330` and `24d0` preserve 35/35 and 26/26 ordered calls, including the
120-frame fixed-point motion loop and scene-byte publication. This adds
**666 executable semantic bytes**, removes the overlay certifier's stale
"recorded owner contains no return" finding, leaves three `resource_3bc`
code-suspect gaps, and reduces the global ranked queue to **58**.

The final `resource_3bc` closure adds **3,576 executable semantic bytes** in
six independently admitted owners. The 948-byte `3468-381c` gap splits into
268-, 204-, and 476-byte owners, including the complete deferred-MMIO queue
append logic in ordinary C. The 1,196-byte `40d0-457c` gap splits into the
964-byte per-frame chain renderer and 232-byte descriptor constructor. Reusing
their already reviewed `resource_3ba`/`resource_3bb` homologs, followed by
independent address, pool, multiset and order validation, was the fastest
measured path through that pair. Finally, the 1,432-byte scripted-scene owner
at `1474` preserves its branch-skipped eleven-word interior pool, final message
word, five task phases and all **157/157** calls across 33 resolved callees.
All six pass compilation, multiset, strict-order, m2c coverage and no-assembly
gates. `tools/overlay-gaps resource_3bc` now reports zero code-suspect gaps and the
global ranked queue is **55**. The certifier still reports the two previously
recorded tail findings at `405c` and `4d88`; neither intersects a newly admitted
owner.

Homolog reuse then closes the complete 1,412-byte `resource_3ba:2124-26a8`
gap as a 148-byte palette loader and 1,264-byte command/interpolation renderer.
Their reviewed `resource_3bb:23bc/2450` and `resource_3bc:2ee8` family members
supply control-flow evidence only: the receiving overlay independently resolves
its palette/cache cells, callback, command queue, output state, interior pools
and every import. The two new owners pass 6/6 and 10/10 multiset and strict-order
call checks plus complete m2c callee coverage. The renderer keeps all three
signed interpolation channels, four output modes and both interrupt-masked
deferred-MMIO queue appends in ordinary C. This raises combined C coverage to
**1,177,960 / 1,343,470 bytes (87.68%)** and reduces the global ranked queue to
**54**.

The following boundary-first tranche adds another **1,800 executable semantic
bytes** and reduces the global ranked queue from 54 to **48**. `resource_3bb`
closes `1c78-1df4` as 72- and 308-byte owners; `resource_389` closes `0a58-0b6c`
as one 232-byte spawn/configuration owner plus five small wrappers/leaves;
`resource_381:0f64` contributes a 288-byte formation owner whose two apparent
returns are relocated-helper calls; and `resource_395:172c-1838` adds three
palette/group helpers after correcting the preceding owner's pool boundary.
`resource_39c` then closes `5242-5388` as two retained alignment bytes plus a
324-byte target-following movement owner, and closes `2648-2814` as a 60-byte
palette rotator and 400-byte scripted actor/camera owner. Every ordinary BL is
reconciled in multiset and source/machine order; the relocated arithmetic calls
are ordinary typed C function calls, never assembly embeddings. The current
coverage checkpoint is **1,179,760 / 1,343,470 bytes (87.81%)**.

The next boundary-and-homolog tranche adds **4,720 executable semantic-C
bytes** and reduces the ranked code-suspect queue from 48 to **39**. It closes
owners in `resource_3a6`, `resource_3ca`, `resource_399`, `resource_3ba`, and
`resource_387`; annexes the literal-pool tail and two following owners in
`resource_3b3:1174-13b0`; transposes the independently re-resolved
`resource_373` movement/redraw family into the 980-byte `resource_3bf:0608-09dc`
gap; and closes both `resource_39c:1164-1408` and `18f8-1bd8` placement/event
families. The owner-local multiset and strict-order gates caught and corrected
one converged-call source-shape error in `resource_39c:1340`. All admitted
owners pass compilation, complete m2c callee coverage, strict call checks and
the repository-wide no-assembly scan. The current checkpoint is **1,184,480 /
1,343,470 bytes (88.17%)**.

A structural-queue audit then proves that `resource_3bf:5588-57ec` is exactly
69 fixed eight-byte import veneers followed by all fifteen four-byte
`call_via` entries (552 + 60 = 612 bytes), with no ordinary return outside
those banks. `tools/overlay-gaps` now masks the same exact structures in interior
gaps as it already did in heads and tails; a mixed `resource_3a7` gap retains
its one ordinary return, and synthetic getter/veneer tests pin both directions.
That evidence-only correction removes one false code-suspect row without
changing coverage. The exposed `resource_3a7:1158-13ac` owner then adds **596
executable semantic-C bytes** by transposing the independently reviewed
`resource_3bf:57ec` soft-float core: all eighteen differing halfwords are the
nine BL pairs, no ordinary instruction or pool word differs, and the four
local targets were independently re-resolved with the same 2/3/3/1 call
multiset and order. The ordered-call gate also corrected the older template's
omission of its real constant-loader call, so both siblings now pass 9/9. The
ranked code-suspect queue is **37**, the no-assembly guard scans 5,427 C/H files
with zero forbidden constructs, and combined coverage is **1,185,076 /
1,343,470 bytes (88.21%)**.

The next boundary split closes all **516 reviewed bytes** in
`resource_3a6:0d74-0f78` as six owners: a zero leaf, a literal getter, a
76-byte signed-state scene-record selector, two 28-byte six-argument rectangle
wrappers, and the 372-byte actor-9 particle-descent sequence. The large owner
independently preserves the tile-23 guard, pre-positioning, two symmetric
object creates, 68-frame random-particle loop, presentation/cue updates and
teardown; all 29 BL sites match source/machine order, and the whole overlay's
20 semantic owners pass the strict call gate. Twelve trailing pool bytes fall
outside executable coverage, so the tranche contributes **504 executable
semantic-C bytes**. The ranked queue is **36**, the no-assembly guard scans
5,433 C/H files with zero findings, and combined coverage is **1,185,580 /
1,343,470 bytes (88.25%)**.

The following `resource_3a0:0f30-1148` closure contributes **528 executable
semantic-C bytes** across a 304-byte actor-19 scripted scene, a 224-byte
actor-18 setup, and an eight-byte literal getter. The scene retains three
independently parameterised object spawns, actor presentation, story-state
increment and all 21 calls; the setup retains its repeated record fetches and
all 22 calls. All thirteen semantic owners in the overlay pass strict
source/machine order, both larger owners pass complete m2c callee coverage,
and the ranked queue falls from 36 to **35**. The no-assembly guard scans 5,436
C/H files with zero findings; combined coverage is **1,186,108 / 1,343,470
bytes (88.29%)**.

## Semantic C: what it did and did not unlock

Semantic C is valuable for completeness, review, type/ABI discovery, call-graph
work, and selecting exact candidates. It is not a byte-match accelerator on its
own. Closing the main-executable semantic census did not change the exact
headline because the remaining work is dominated by register allocation,
scheduling, literal-pool placement, compiler vintage, private ABI contracts,
and module boundary/linkage details.

Main-image readability is now maintained as a continuous, independently gated
track rather than left frozen at first-pass semantic closure. Its first forty-four
tranches replace fifty-four mechanical owners with typed source. They cover the
16-by-16 affine tilemap initializer at `080cdb24`, action/help/combatant renderers
at `080a6a98`, `080a8578` and `080a8914`, scene transfer and sprite-palette
owners, inventory and roster selection, modal lifecycle, animation allocation,
and ordered rendering-preset selection, plus transition profiles, a two-window
selection dialog, the tile-script interpreter, a shared effect-slot motion
sequence, the effect-transfer queue at `0801a088`, the 512-color RGB555 fade at
`080f2f10`, the numeric selector input handler at `08029094`, the item-page
renderer at `080b0fa4`, projectile launch at `08098698`, sprite-slot projection
at `0800b074`, battle-decision routing at `080b920c`, and the owner at
`0809b208`, plus particle-manager, terrain-marker, target-classification,
arena-initialization, cursor-wobble, display-tween and dirty text-tile DMA
owners. The preceding batch adds camera-relative projection, guarded letterbox and
deferred-display writes, map-init resource staging, interpolation and paged
display-object updates, an effect watcher, numeric text placement, cartridge
configuration lookup, a terrain-seeded particle manager, `0808bb2c`, and the
typed fixed-stride selection-state initializer at `0801a66c`. The latest ten
also cover `080a5b94`, `08093fa0`, `08094e7c`, `0801f088`, `080799b0`, the
text interpreter at `08017aa4`, fixed-stride merge state at `080aae14`, the
paired-buffer effect owner at `08097644`, sprite/OAM projection at `0800b168`,
and the guarded orbit lifecycle at `08095c08`.
The rendering preset preserves the eight-flag
override as a loop whose highest enabled flag wins. These refactorings do not
change semantic coverage, and the combined full gate remains byte-identical
with zero fallback and zero forbidden C/H assembly constructs. A persistent
Main Image-only worker continues this track in separately verified tranches
while overlay closure proceeds.

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
5. **Cross-host parity is a standing rule (2026-08-05, Pascal).** darwin/linux
   on arm64/x86_64 are all first-class hosts. A compiler mode or routing change
   lands only with every supported host family rebuilt from the committed fork
   source, verified byte-identically, and digest-pinned. The
   native `alchemy-lints` `flag-capability` gate (part of
   `make test`) probes each staged binary with every flag live routing can emit,
   so a host left behind fails immediately with the rebuild procedure named
   instead of failing mid-build inside an unrelated overlay rebuild. Hosts
   without approved digests yet are admissible, not unsupported: build, stage,
   verify green, then pin.

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

The repository's operational tooling is native Rust under `tools`, exposed
through the Makefile and native dispatch groups. Its strongest guarantees are:

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
- `tools/overlay-gaps --ranked` now puts one-return owner-shaped gaps in byte-yield
  order before multi-return residue. The first ranked cohort closed **16,816
  executable bytes across 16 new source owners** rather than spending a pass
  manually scanning every module report.
- the ordered-call verifier now follows bounded resolver evidence past a proven
  jump-table dispatch instead of silently auditing only the pre-table prefix.
- the call resolver now recognises stack-reservation-first GCC prologues such
  as `resource_39c:08ec` (`sub sp,#16` before `push`). This corrected a two-byte
  owner-boundary error and makes both callers visible to the automated checks.
- explicit source-level callback calls now remain visible as `call_via` entries
  in ordered-call audits, while m2c coverage no longer demands a fictitious C
  symbol for the overlay's bare `bx rN` trampoline.
- normalized sibling structure produced an immediate 1,264-byte reuse:
  `resource_3bb:2450` has the same control flow and ten-call sequence as
  `resource_3bc:2ee8`. Its private state block was independently remapped from
  both literal-pool tables, turning a second 600-instruction read into a bounded
  address-substitution review without weakening per-owner verification.
- the coverage map now preserves the executable inventory's proven code-overlay
  veneer and alignment classifications. **43,548 overlay bytes** that were
  previously painted gray as Unknown are now correctly dark-gray retained exact
  assembly; the repeated similarly sized blocks are fixed entry/import banks,
  not suspicious identical unknown functions.

Remaining tool debt:

- `tools/remaining-survey` reports only the 596 `c_candidate` rows and omits the 96
  split/merge rows / 38,440 bytes of structural exactness debt.
- Code-overlay discovery's 12,046-row output needs a non-overlapping, confidence-
  tiered queue instead of headline discovery counts.
- Route evidence still lives largely in hand-maintained allowlists and comments.
- Full compiler-corpus regression exists but is not a mandatory CI gate.
- Git hooks protect publication only after the contributor explicitly enables
  them; hook configuration is not itself verification.
- Exact and semantic dashboard ownership update directly from tracked files.
  Main-image dark-gray ownership uses the verified full-build assembly manifest;
  code-overlay dark-gray ownership uses the audited executable inventory's
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

1. Run `make verify` and `make coverage-check`.
2. Commit and push each coherent exact-C tranche and its generated metrics.
3. Regenerate and commit the first-parent history after the checkpoint commit.

### Phase 1 — reach 20% exact C honestly

The target is **+1,850 exact bytes**. Treat it as a portfolio, not one heroic
function:

1. Work the 540-owner semantic-backed code-overlay reading list in descending
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

1. Extend `tools/remaining-survey` to include all five C-debt retentions and emit a
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
make progress-check
make build-semantic
make semantic-check
make core-retained-check
make coverage-check
make verify
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

---

## Addendum 2026-08-07 — the rate is a throughput problem, not a difficulty problem

Claims that 50% is out of reach were made from the calendar, not the ledger.
The ledger says otherwise.

Byte progress reconstructed from the `[C n/total bytes]` commit markers:

| Period | Commits/day | Bytes/commit |
| --- | ---: | ---: |
| Jul 29 – Aug 1 | 372 | 97 |
| Aug 2 – Aug 6 | 65 | **132** |

Yield per commit **rose** while daily bytes fell fourfold. The residual is not
getting harder per unit of work. What changed is where the work went: Jul 31
was 134 `merge`, 86 `convert`, 82 `semantic:`, while Aug 2–6 was `docs:`,
`dashboard:`, `Metrics`, `Fix`. The throughput was reallocated to meta-work.

The engine that produced the peak was four lanes — mercury, venus, jupiter,
mars — each on a disjoint code overlay, merged into `main` continuously:
290 lane commits and 144,630 bytes in four days.

Arithmetic to 50%: 356,588 bytes remain. At the current, improved 132
bytes/commit that is 2,701 commits, or **113 commits/day** across 24 days.
The fleet sustained 372/day and peaked at 584. The required rate is under a
third of demonstrated capacity, and 256,828 bytes of the gap sit in the
semantic-backed code-overlay reading list (434 owners, 62 overlays) where the
C is already written and only exactification remains.

Operational note: `alchemy-gcc/dist/cc1` is untracked, so lanes must share one
working tree. Separate worktrees would each need their own non-reproducible
gcc build, and cross-lane results would stop being comparable. Give lanes
disjoint overlays (disjoint `exact/` files), and let the orchestrator own git.

The standing falsified hypothesis still binds: more subagents do not help when
they collide on the same compiler floor or lack independently owned targets.
Disjointness is the whole mechanism.
