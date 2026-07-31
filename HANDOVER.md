# Alchemy handover

Updated: 2026-07-30

The single authoritative handover. Do not add dated handoff files; update this
one in place. Per-function detail lives in `work/claude/notes/`; per-commit
history lives in git. This file holds only what is still *actionable*.

**Read it top to bottom before starting.** Several sections correct advice given
earlier in the project, and the corrections are the parts that save time. Two
"blockers" that stopped work for whole sessions turned out to be stale notes
rather than real limits.

Exact means fully linked machine-code byte equality — not semantic similarity,
not equal object size.

## The two lighthouses

The project runs as two parallel efforts, named after the Golden Sun lighthouses.
Know which one you are before you change anything.
[docs/BRANCH-PROTOCOL.md](docs/BRANCH-PROTOCOL.md) is the short version of who
owns what and what reaches you how — read it first if you are new to a branch.

**Read `docs/BRANCH-PROTOCOL.md` once before your next push.** It is the tracked
statement of who owns what: the ring is `main -> mercury -> venus -> main`, and
nobody pushes to a branch they do not own. For Mercury that means pull `main`,
push only `origin/mercury`, and never touch the coverage map or `README.md` —
anything you want on `main` gets there by banking it here and letting Vale port
it.

**Cadence is every 20 minutes, not hourly.** Vale moved first (MEETING.md
2026-07-30T22:30Z) and Mercury followed at 23:05Z; the protocol doc still says
"about once an hour" because it predates both. Twenty minutes is the number to
work to.

**`MEETING.md` is the channel between the three agents** and travels the ring
with everything else. Anything the other two need to know goes there rather than
in a commit message only they would have to go looking for; evidence and long
reasoning stay here. Read it every pull — items are addressed by `@mercury` and
tagged **ACTION**, and an ACTION is closed by writing a **DONE** entry, never by
silently doing it.

**Standing duty: delete semantic sources your conversions supersede.** `main`
now carries `semantic/`, and `build_semantic.ts` throws `duplicates exact source`
whenever a semantic source and an exact source share an address — which `verify`
runs, so it breaks the bank rather than warning. `bun tools/semantic_superseded.ts
--check` names every one at once before you bank instead of one per build; it is
tracked-tree only, so it needs neither the ROM nor the toolchain. Run it after
adopting and before `bun run verify`.

| | **Mercury Lighthouse** | **Venus Lighthouse** |
| --- | --- | --- |
| branch | `mercury` | `venus` |
| goal | **exact C** — fully linked machine-code byte equality | **semantic C** — readable, correct, not byte-bound |
| this file | authoritative | background; the levers do not bind you |
| direction of flow | exports byte-exact sources to Venus | pulls Mercury's exact C in to override its own semantic C where one exists |

### How work circulates

Three agents, each owning one branch, each pulling from exactly one place. The
flow is a **cycle**, and no agent pushes to a branch it does not own.

**Vale is the master process.** `main` is the trunk and Vale's decisions are
authoritative over both lighthouses. Practically, for Venus and Mercury:

- A conflict between what a lighthouse decided and what Vale decided resolves to
  Vale, and the lighthouse converges rather than arguing at the merge.
- Anything a lighthouse changes about *shared* tooling or process — the `verify`
  chain, the test chain, documentation structure, the branch protocol itself — is
  a **proposal to Vale**, not a decision. Make it, bank it, and flag it in
  `MEETING.md` so Vale can ratify or revert. Do not treat silence as approval.
- Lane work-product is still the lane's own: what you convert, how you scope an
  owner, which overlay you take. Vale does not adjudicate that.
- When a lighthouse must act ahead of Vale to stay unblocked, say so explicitly
  in `MEETING.md` rather than letting the change look like consensus.

| agent | owns | pulls from | takes |
| --- | --- | --- | --- |
| **Vale** | `main` | `venus` | **docs and tools only** |
| **Mercury Lighthouse** | `mercury` | `main` | everything |
| **Venus Lighthouse** | `venus` | `mercury` | everything |

Consequences worth knowing before you act:

- **Semantic C never reaches `main` or `mercury`.** Vale takes only docs and
  tools from `venus`, so `semantic/` stays on `venus` by design. Do not try to
  push it anywhere else.
- **Tooling and HANDOVER edits DO circulate to every branch**, the long way
  round: `venus` → `main` (Vale) → `mercury` (Mercury Lighthouse). That is why a
  measured lever or a corrected tool is worth writing down properly — it is the
  only thing that crosses lane boundaries, and it reaches the other lighthouse
  without anyone coordinating.
- **Exact C reaches `venus` directly** from `mercury`, which is the override
  path: when Mercury makes a region byte-exact, the next Venus pull deletes the
  semantic version. `build_semantic` hard-errors on a duplicate, so this is
  enforced rather than remembered.
- **Bank with `tools/venus_bank.sh`, never by typed shell chain.** It syncs
  spans, verifies, and stops if verify fails. This exists because a `;` where
  `&&` was meant pushed a commit whose verify had failed seconds earlier — sound
  only by luck. In a tree with concurrent writers a failed verify is usually
  another lane mid-file, which is precisely why it must block rather than warn.
- **Pull `mercury` with `tools/venus_pull.sh`, never by hand.** A conflicted
  merge leaves markers in the working tree, and a conflicted `package.json`
  breaks `bun run <anything>` for every concurrent lane — three lanes lost part
  of a session to that window while conflicts were resolved across separate tool
  calls. The script merges and resolves in one process, so the window is
  milliseconds, and it encodes the resolution rules that are easy to get wrong.
- **`MEETING.md` is the standing message board between the three agents.** It
  travels the ring with the other documentation, so a note there reaches everyone
  within a cycle or two. **Take the timestamp from `date -u +"%Y-%m-%dT%H:%MZ"`,
  never from memory** — the merge rule is "keep both sides and re-order by
  timestamp", so a hand-stamped entry lands in the wrong place in the log for
  every agent. Append with a UTC timestamp and an addressee — per-agent sections exist so the ring's merges
  never conflict. Never edit another agent's section; reply in your own, quoting
  the timestamp. Prune your entries once they are acted on.
- **Never push to a branch you do not own.** If Venus work belongs on `main`,
  it gets there because Vale pulls it, not because Venus pushes it.
- **`README.md` and its ROM coverage map belong to Vale. Never edit them from
  `venus` or `mercury`.** The map is regenerated on `main` from the metrics both
  lanes publish, so editing it downstream either conflicts with Vale or reports a
  figure that the branch cannot substantiate. Publish numbers by banking them —
  the map follows.
- **Venus pulls `mercury` every 20 minutes.** Raised from hourly on
  2026-07-30: an hourly merge had grown to 37-45 Mercury commits and 22+
  superseded-semantic deletions, which is more than is comfortable to check by
  eye. At 20 minutes it is a handful of commits and a handful of deletions. Each pull: merge, take Mercury's `src/` and
  routing on conflict, delete every semantic source that now has an exact
  counterpart (`build_semantic` hard-errors if you miss one), re-verify, bank.

Mercury is the slower, stricter run: a function is done only when the linked
bytes match. Venus is the faster, wider run: it covers ground semantically and
adopts Mercury's exact sources whenever Mercury produces one for a region Venus
has already covered. **Exact always wins over semantic** — that is the whole
reason the flow is one-directional.

Practical consequence for Mercury: when Venus reports one of its regions is close
to exact, that region is worth re-probing here, because an exact result would
replace Venus's semantic version outright. Two such candidates were noted and are
still open (§8).

Alongside the exact lane, reviewed semantic C currently accounts for **707,774
executable bytes across 1,364 compiling sources**: 385,850 main-image bytes and
321,924 overlay bytes. Combined with exact C, **924,720 / 1,339,582 executable
bytes** are expressed as C. Build that lane with `bun run build:semantic`; its
sources live under `semantic/` and do not claim byte equality. Use
`semantic/ordinary-blockers.json` to keep proven ABI and multi-region traps out
of the ordinary review queue.


**THE OVERLAY STRICT QUEUE IS EMPTY.** All **68** overlays have zero
unconverted strict rows, holding 315,208 strict bytes between them.
it has drifted twice from being maintained by hand:
`resource_373` (18,044), `resource_3b8` (15,028), `resource_3bf` (13,252), `resource_3c8` (11,916), `resource_372` (9,838), `resource_371` (9,486), `resource_38f` (9,212), `resource_39f` (8,692), `resource_3c4` (8,642), `resource_383` (8,588), `resource_3c5` (7,866), `resource_3a8` (7,780), `resource_391` (7,648), `resource_374` (7,468), `resource_375` (6,424), `resource_37a` (6,200), `resource_37b` (6,032), `resource_3b2` (5,984), `resource_3aa` (5,960), `resource_3b7` (5,954), `resource_3bb` (5,548), `resource_395` (5,504), `resource_3cb` (5,488), `resource_39a` (5,368), `resource_381` (5,328), `resource_377` (5,226), `resource_3b4` (5,104), `resource_3c6` (5,094), `resource_3ae` (5,026), `resource_370` (4,718), `resource_38d` (4,680), `resource_399` (4,672), `resource_3a2` (4,484), `resource_3a7` (4,442), `resource_3c7` (4,252), `resource_37f` (4,216), `resource_3ad` (3,978), `resource_3ca` (3,926), `resource_3bc` (3,768), `resource_3ba` (3,344), `resource_38b` (3,318), `resource_394` (3,282), `resource_3a3` (3,156), `resource_389` (3,056), `resource_3b5` (2,914), `resource_3c2` (2,688), `resource_379` (2,628), `resource_3b6` (2,284), `resource_3ce` (2,274), `resource_38e` (2,206), `resource_3c3` (1,934), `resource_393` (1,820), `resource_398` (1,620), `resource_392` (1,562), `resource_386` (1,142), `resource_38c` (1,024), `resource_3b1` (994), `resource_36f` (888), `resource_3cd` (880), `resource_3b0` (682), `resource_3af` (552), `resource_390` (532), `resource_3c1` (468), `resource_3b9` (444), `resource_397` (318), `resource_384` (248), `resource_378` (220).

**"Converted in full" means zero unconverted STRICT-QUEUE rows, not that every
executable byte of the overlay is C.** Measured across those overlays: their
assembled images total 231,694 bytes, of which semantic sources cover 116,466 and
exact sources 4,398 — **110,830 bytes lie outside any strict row**. That
remainder is veneer and import bands, jump tables, literal pools and inter-owner
data, which are not semantic-C candidates. Any claim that credits a whole
overlay's executable extent to the semantic lane will overstate it by roughly
that proportion.

**Pre-measured and waiting for a fresh agent: `resource_3c8:3068`**, a 26-way
`mov pc, r3` dispatcher. Its boundary is settled — prologue at 0x02003068 saving
`r5,r6,r7,lr` plus `fp/sl/r9/r8` with a 12-byte frame, matching unwind at
0x02003fa8-0x02003fb8 with `r0 = 0` before it, so it returns `s32`. That is
**3,922 bytes as one owner across 18 inventory rows with ~260 static calls**; the
18 sub-rows are `call:` seeds (import identities), not real entries, and the only
true internal structure is the jump table.

Both lanes are drawn together in the README coverage map
(`assets/readme/gs1-en-coverage.svg`, regenerated with `bun run coverage`):
blue is Mercury's exact C, teal is Venus's semantic C, grey is the ground
neither lane has taken yet. It is the fastest way to see where each lane
actually stands before picking work — see §9.

---

## 0. Venus Lighthouse speed policy

The 2026-07-30 speed run established a repeatable method, not a one-family
outlier. Consecutive three-owner cohorts converted tens of thousands of reviewed
bytes while preserving the exact lane and full verification.

1. **Use fresh agents for whole-owner rewrites.** Give each agent one complete
   owner, its measured byte span, call count, warned registers/thunks, and the
   requirement to account for every assembly call. Treat m2c as a hint only.
   Fresh agents completed large 57–74-call owners where long-lived agents began
   returning analysis without implementation. This runtime has three subagent
   slots, so a six-agent experiment runs as two immediately consecutive waves
   of three.
2. **Batch by established ABI or construct family first.** Owners that visibly
   publish callbacks through `Func_080cef64` or `Func_080ed408` share the proven
   six-argument renderer ABI. Fixed transfer, fill, scale, square-root, and
   owner-initializer callbacks are likewise reusable evidence. The semantic
   queue discounts only visibly established renderer families; unknown thunks
   retain the full penalty.
3. **Rewrite from assembly when m2c loses dataflow.** Missing stack-carried
   renderer dimensions, fake thunk arguments, unset call-clobbered registers,
   and conflated high-register lifetimes repeatedly proved to be decompiler
   defects rather than target blockers. Recover them from predecessor blocks,
   literal maps, and callback targets. Do not hand-clean a lossy draft for a
   whole session.
4. **A manifest row is not always a function.** Follow live stack and
   high-register state through direct continuation branches. Register the
   complete executable owner in `semantic/main-regions.json`, excluding literal
   pools and data gaps. `08026080` is the current witness: its advertised
   2,138-byte head is one 3,442-byte owner across three executable ranges.
   Pre-size `split_first` and `merge_with_continuations` candidates transitively
   before assigning them: `080e47b8` was ranked as a 768-byte dispatcher, but
   its live 184-byte frame crosses 16 manifest rows to the sole epilogue at
   `080e660a`. Its full span is 7,762 bytes including embedded pools/alignment,
   with 231 static calls—over 10 times the advertised size. Queue-row bytes
   therefore cannot be used to budget or compare these owners. Pool-map the
   mixed `split_first` rows before admitting their executable subranges.
5. **Verify and bank coherent cohorts.** Run `bun run build:semantic` while
   agents work, then one full `bun run verify` for the settled cohort. Update the
   authoritative metrics above, commit by semantic byte gain, and push before
   starting the next wave.

**Pre-sizing is now tooled: `bun tools/semantic_owner_scope.ts`.** Rule 4 above
requires transitive sizing and a pool map before any continuation row is
assigned; this produces both. It groups continuation rows into whole owners
(a prologue saving lr opens one, an epilogue closes it) and marks rows whose
reconstruction is nothing but `.inst.n` halfwords as DATA — embedded pools and
alignment, excluded from the owner's executable ranges. `--json` for machine
use, an 8-hex stem for one owner's row breakdown, `--self-test` in the test
chain.

It reproduces two independent hand audits exactly, which is the reason to trust
it: `080e47b8` advertised 768 bytes, measured **7,762 executable bytes across 16
rows with 232 calls** (audit: 7,762 / 16 rows / 231 calls), and `0800ebec`
measured **1,804 bytes across 4 rows** (blocker note: "the true 1,804-byte
function spans four regions"). Treat the row grouping as evidence, not proof —
it is a boundary *estimate* to size and assign work, and the admitting agent
still owns the boundary.

**Executable bytes are an UPPER BOUND, and the tool works at ROW granularity.**
Two distinct overcounts follow, both measured. First, an *interior* pool — one
that sits inside a row rather than occupying its own row — is invisible to the
tool entirely: four of `080ec100`'s six code rows contain one, so its true
executable size is 3,126 against a reported 3,358. Second, a whole-row pool A literal pool that
lives in a regular `asm/*.s` row disassembles as perfectly plausible
`lsrs`/`movs` pairs, so it decodes to mnemonics and is counted as code. The tool
now flags such rows `POOL?` and reports `suspected_pool=` per owner (272 bytes
across the open set), but flagging is conservative on purpose — resolving a row
for certain means assembling it at its real base and checking that sibling
`ldr rN,[pc,#imm]` loads land in it. The admitting agent for `080be378` did that
and found 156 of its 3,696 "executable" bytes are pool, giving a true 3,540; the
`080ec100` agent likewise reclassified a whole 46-byte row the tool had kept as
code. Use the tool's number to rank and assign; let the admitting agent settle
the ranges. Expect the settled figure to come in a few percent under.

**Three detection rules it took to get there**, each of which silently corrupted
the numbers before it was fixed:
1. *Group over every manifest row, not just the open ones.* An owner's epilogue
   often sits in a neighbouring row of a different retention, so grouping only
   open rows reported five ordinary owners as needing a boundary audit.
2. *Recognise the interworking return.* This target returns three ways —
   `pop {..,pc}`, `bx lr`, and `pop {rN} ; bx rN` for owners that save high
   registers. Missing the third left 13 owners "unclosed". A bare `bx rN` with
   no preceding `pop` is a jump-table dispatch and must NOT close an owner.
3. *Drop groups with zero executable bytes.* Stranded literal pools and
   alignment are labelled executable gaps in the manifest and inflate the
   remainder while being unconvertible by construction.

**Measured state of the main image.** The remaining continuation rows are not 80
separate jobs and not 31,088 bytes: they collapse into **22 owners / 37,128
executable bytes** (upper bound; at least 272 of those are suspected pool), with
240 bytes of confirmed pool excluded. Of the two groups reported unclosed,
`080bf1e8` is not an owner at all — it is the last literal pool of `080be378`,
settled by that owner's admitting agent. Largest first: `080e47b8`
(7,762 / 232 calls), `080f4168` (4,596 / 108), `080e15e8` (3,858 / 130),
`080be378` (3,696 / 125), `08026080` (3,584 / 69), `080ec100` (3,358 / 87),
`080d765c` (3,156 / 91), `0800ebec` (1,804 / 46), `080d4ce8` (1,392 / 40).
**The `multi_region_function` blocker class is stale.** Five owners carry it, and
its wording is a *request for whole-module accounting* — "admit the head and
continuation as one semantic module" — not a statement that the work is
impossible. That accounting now exists, and the first re-probe confirmed it:
`080dd9c0`, blocked as "only FunctionHead_080dd9c0", was admitted as one
940-byte module across its three rows with all 23 calls placed, and `080ec100`,
blocked as "only the front of a much larger effect function", was admitted as
one 3,126-byte module across nine ranges with all 87 calls placed. Its agent put
the distinction well: the blocker was accurate as written but was a *sizing*
blocker, not a structural one. **Every blocker class in `semantic/ordinary-blockers.json` has now been
re-probed, and one entry of thirteen remains unresolved.** Beyond the five
`multi_region_function` owners below, the 2026-07-31 sweep converted all six
remaining blocked main-image owners — `hidden_register_module` (three of them),
`cross_file_abi`, `shared_stack_context_module` and
`implicit_callee_return_state_module`. Two were disproved on the facts rather
than re-scoped: `080c1798`'s "intentional callee residue" is an r2 that no call
site in the image sets, and `08095778`'s ABI conflict dissolves once the exact
source's `void *` parameter is read as the integer flag id it actually carries.

**Write the date and tool state into every new blocker.** None of those notes was
careless — each was right against the evidence available when written, and what
changed underneath them was the tooling (the `call_via` bank, the `bl` target
rule, whole-module scoping). A blocker that does not say what it was written
against cannot tell the next reader whether it is worth retesting.

**All five `multi_region_function` blockers are now resolved**: `080dd9c0` (940
bytes), `080ec100` (3,126), `080d765c` (2,866), `080e15e8` (3,542) and
`080ddde0` were every one of them a *sizing* blocker, admitted whole once the row
map existed. The class is empty — 10,474 bytes recovered from notes that read as
permanent. The `relocated_kernel_continuation` class also fell, and its post-mortem is the
most useful of the set. `0800ebec` was blocked for a "nonstandard return
contract": inline `mov ip,pc; bx r4` into the relocated kernel, returning into a
62-byte gap row with the frame live. It is an ordinary indirect call —
`mov ip,pc` sets the return to the instruction after the `bx`, both landing sites
continue the same frame, and `LAWS.md` **already recorded this idiom as a
codegen-only difference** from `bl __call_via_r4`. The note had promoted a
byte-exactness fact into a semantic blocker. Admitted whole at 1,714 bytes.

That is the pattern worth carrying: **a blocker written while wearing Mercury's
hat is not automatically a Venus blocker.** Byte-exactness obstacles and semantic
obstacles are different sets, and the overlap is smaller than the notes imply.

What remains in `ordinary-blockers.json` are `hidden_register_module`,
`cross_file_abi`, `implicit_callee_return_state_module` and
`shared_stack_context_module`. Those have NOT been shown stale — judge each on
its own evidence rather than assuming the streak continues.

**Wave result, five multi-row owners: 5/5 admitted, 20,428 advertised bytes and
19,464 registered after the agents settled the pools.** `080e47b8` (7,382 across
10 ranges, 231 calls), `080f4168` (4,476 / 5 ranges, 108), `080ec100` (3,126 /
9 ranges, 87), `080be378` (3,540 / 4 ranges, 125), `080dd9c0` (940 / 3 ranges,
23). Every one of them came in *under* the tool's estimate once interior pools
were resolved, by 1-7%. None parked. This is the same stale-evidence pattern that
§6 documents on the exact side.

**`Func_080072e4`..`Func_08007318` are NOT functions — they are a `call_via rN`
thunk bank.** This is the single most consequential modelling fact in the
semantic tree: 104 of 637 sources reference the bank across 472 call sites, and
the sources disagree with each other about what it means. `asm/080072e4.s` is a
14-entry table, 4 bytes per entry (`bx \register` + `mov r8,r8`), in register
order r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,sl,fp,ip,sp:

| symbol | is | symbol | is |
| --- | --- | --- | --- |
| `Func_080072e4` | call via r0 | `Func_080072fc` | call via r6 |
| `Func_080072e8` | call via r1 | `Func_08007300` | call via r7 |
| `Func_080072ec` | call via r2 | `Func_08007304` | call via r8 |
| `Func_080072f0` | **call via r3** | `Func_08007308` | call via r9 |
| `Func_080072f4` | **call via r4** | `Func_0800730c` | call via sl |
| `Func_080072f8` | call via r5 | `Func_08007310` | call via fp |

So `bl Func_080072f0` is an **indirect call whose target is r3**, with r0-r2 as
the target's arguments — it is not a four-argument routine. The bank exists
because these targets are ARM-mode helpers relocated into IWRAM, so a Thumb
caller needs `bx` interworking to reach them; the ROM copies are catalogued in
`assets/data/saihouchi_arm.json` (7 helpers at `0x08015430`+, 2,652 bytes), which
also lists each helper's known consumers.

**The good spelling is already in the tree.** `semantic/main/080d0ee0.c` declares
`void Func_080072f0(void *, const void *, u32, Transfer_080d0ee0)` — the last
parameter typed as a function pointer, which is faithful. Copy that. The
spellings to fix on sight, in rough order of how misleading they are:
`void Func_080072f0()` (prototype-less, 4 files), `void Func_080072f0(s32, s32,
s32, s32)` (11 files — hides the callee entirely), and any comment calling the
fourth argument dead, opaque scratch, or an address constant. An IWRAM literal
such as `0x03000164` in that position is **the relocated helper being called**,
not scratch.

**Three independent agents rediscovered this in one wave**, which is the measure
of how much time the convention costs when it is not written down: one found the
`call_via` table in `asm/080072e4.s`, one traced `Func_080072f0`'s r3 to the
relocated IWRAM square root at `0x030001d8`, and one found that `Func_080072f4`
sites vary their r4 target between `renderers[0]`, `renderers[1]`,
`renderers[i&1]` and `*(void**)0x03001f0c`. That last one is the case where the
common 6-argument spelling is not merely imprecise but wrong: it silently drops
the varying target, so two different callees collapse into one name.

Corrected in this session: `080a7478` and `0808d9a4` (comments and declarations).
Sweeping the other 102 files is open, mechanical work — the fix is a declaration
and a comment, never a control-flow change.

**Overlay link bases: Reading A is now confirmed, Reading B's anomaly is not.**
Two overlays are independently proven to be linked at **0x02008000**, each by
three separate witnesses: `resource_3bf` (jump-table base `0x0200c64c` against an
embedded table at offset `0x464c`, entries 0x8000 past their case bodies) and
`resource_3c4` (a `mov pc,r3` table base `0x02008e58` whose entries are file
offsets, an installed handler pool word `0x02008fe9` = `Func_02000fe8`+Thumb, and
`0x02008ec9` = `Func_02000ec8`+Thumb). **So on those overlays any pool word in
`0x0200_8xxx..0200_bxxx` is an in-image address at `offset = value - 0x8000`,
and `Data_0200bxxx`-style symbols are in-image data, not RAM globals.**

`resource_3b8` shows a third shape: every `bl` computes an address in an *import
band* above the last code row, whose first 704 bytes are an 8-byte-per-entry
veneer table (`ldr r4,[pc,#0]` / `bx r4` / `.word <main-image address>`). There
the right move is not to resolve the target at all — name the import by the
address its call site computes, which is what the byte-exact
`assets/code/resource_3b8_c_*.c` already do.

**SOLVED — an overlay `bl` stores the target's image offset minus 2.**

    true_target_offset = stored_displacement + 2

Not a pc-relative displacement. Every disassembler, `tools/overlay_show.ts`
included, adds the branch's own pc, which is why its call annotations are wrong
for **every** overlay and wrong in a way that looks plausible. All three of the
long-standing symptoms are this one bug: targets past the end of the image,
targets landing inside the caller's own body, and two sites with *bit-identical*
encodings printing different callees (`resource_39f:1078` has two branches that
both print `bl 0x02003ec2` and are different functions).

Use **`bun tools/overlay_call_targets.ts <overlay> [ownerHex]`**, which applies
the rule and classifies each target as an import veneer (resolving the veneer's
trailing word to the real `Func_08xxxxxx`), a real prologue, or the overlay's own
`call_via` slot. Measured on `resource_39f`: 1,265 call sites collapse to **73
distinct targets** — 1,047 veneer, 116 prologue, 1 `call_via`. The same collapse
holds on 371/372/373/38f/3b8/3bf/3c4/3c8, where 700-1,900 sites reduce to 70-133
distinct displacements.

**When the byte-exact sibling grep comes back empty, grep the overlay's own
`_overlay.s` for `0x0200[89ab]xxx` instead.** One command; on `resource_3b5` it
returned 36 distinct words, **eleven of them odd and resolving to already-banked
byte-exact siblings** (`0x02008031` → `Func_02000030`+1, and ten more). That is a
far stronger link-base proof than a single task-install witness, and the same 36
words named the role of **all fifteen** unconverted rows before any disassembly —
signatures decided in advance. Do this before opening a body.

**A row that sets the same event flag its table key names is a one-shot scene —
proven in one line.** `resource_3ae:0e40`'s handler-table entry is keyed
`0x08ab0032` and its first instruction is `Func_080770c8(0x8ab)`; `:08cc`
read-then-sets `0x8ac` around its only call. That settles both "is this
one-shot" and the scene's identity with no dataflow work at all.

**The asymmetric scene bracket is REAL, not a decoding artefact.** In
`resource_3ae`, both `:0328` and `:051c` have an arm that branches past
`Func_0808a020` straight to the epilogue after `Func_0808a018` has already run.
Preserve it; "fixing" it into a shared close changes behaviour and the multiset.

**`ldrh` + `adds #0x2000` + `ands #0xffffc000` + `lsls/asrs #16` is facing
quantisation to a signed quadrant**, not a sign manipulation. After it, the
`cmp r3, #0x80000000` in `resource_3ae:0328` is simply `facing == 0x8000`, which
reads as a sign test if the quantisation above it was missed.

**`cmp rN,r3 / bls` on a price/coin pair tells you which arm is the purchase**
without knowing any import: only the affordable arm calls the charge import with
the *negated* amount.

**The offset-0 export table is a COMPLETE LINK-BASE PROOF on its own, not just a
root-finder — try it before any other witness.** `resource_3a2`'s six export
words (`0x020091b1, 0x0200807d, 0x020080b1, 0x020080b9, 0x02009181,
0x020080ad`) are all odd, all resolve at `word - 0x8000` onto function starts,
and four of those starts are already-banked byte-exact siblings. That is six
parity witnesses plus four banked cross-checks from a single
`overlay_show <ov> 0 -n 96` — stronger than the jump-table and task-install
witnesses this section recommends, and cheaper than both. The wider `_overlay.s`
grep then confirmed it with zero exceptions: all 21 odd words on function
starts, all 5 even words past the import band.

**A window test spelled `subs / lsls #16 / cmp` is a two-value selector.**
`resource_3a2:12a4` loads a sub-state unsigned, subtracts 4, shifts left 16 and
compares against `0x00010000` — that selects exactly `{4, 5}`. Undo the shift or
it reads as an arbitrary magnitude comparison.

**`movs r2,#N / lsls r2,r2,#1` appears as a displacement AND as a value inside
one owner, and it is a family-wide codegen habit rather than a one-owner trap.**
Documented for `02000180`; `resource_3a2:11b0` does it twice more (448 as a `str`
displacement, then `adds r2,#65` making 513 the stored *value*) and `:0c30`
once.

**Two workspace slots hang off the same `0x03001ebc` pointer and are easy to
conflate.** `+472` is the u16 skip-beat counter; `+448` is an s32 scene/phase id
published on entry with a per-scene constant (32 from `02000180`, 256 from
`resource_3a2:11b0`, 513 from both `:01ec` and `:0c30`).

**An overlay's image offset 0 can be an exported-entry veneer table.** In
`resource_3b5` it is a run of `ldr r4,[pc,#0] / bx r4 / .word 0x0200_8xxx` pairs;
resolving those words under the link base hands you the overlay's *roots* for
free, which is exactly where call-graph-first ordering should start. Entry 0 was
the initialiser, entry 4 the script selector.

**The complement of the "a pool word decodes as a BL pair" trap: a word that
LOOKS like a BL pair can be a genuine constant.** `resource_3b5:007c` loads
`0xf8b6f001` and immediately masks with `0xf000`, so only the low half matters
and it acts as `-0x0fff` — one less than the `-0x1000` its four sibling biases
(`+0x2000`, `+0x1000`, `0`, `-0x2000`) predict, which is the sort of detail a
"tidy" reading destroys. `assets/code`'s `.2byte 0xf001 / .2byte 0xf8b6` spelling
confirms it is data.

**`0x03001e8c` is a pointer TABLE, and its entry 12 is `0x03001ebc`.**
`resource_3b5:0170` loads `[r3,#0]` and `[r3,#48]` off it. Reading those as two
unrelated globals hides that the second is the well-known workspace pointer the
rest of the overlay loads directly.

**THE `bl` RULE DOES NOT INVERT CLEANLY — a pre-rule name is many-to-one AND
one-to-many, so find-and-replace SILENTLY MERGES two callees.** Because the
printed name is `site + 4 + stored_disp`, two different real callees can share
one printed name, and one callee appears under many. This bit 17 of 42 rows in
the `resource_371`/`372` rename pass (17 ambiguous names in `372:31ac` alone).
The method that works: **assign the i-th call occurrence in the source to the
i-th site in address order.** It is checkable on sight, because the last site is
almost always `Func_0808a020` (scene close) and the first `Func_0808a018`.

**Renaming COLLAPSES declarations, and the survivors' return types are not
interchangeable.** Two pre-rule names resolving to one import leaves two
declarations of one function — a hard compile error when they disagree. Pick by
**consumption**, not by order and not by "non-void wins": that tie-break was
wrong in 7 files. A result that is assigned, tested or compared is `s32`; one
never read is `void`. Beware `case N:` labels, which a naive "something precedes
the call" test reads as consumers.

**A jump table whose entries are EVEN is normal for `mov pc, rN`.** `mov pc`
does not interwork on this core, so bit 0 is not a Thumb flag there, while
`offset = word - 0x8000` still applies. The parity heuristic — odd = Thumb
entry, even = data — is for **pool words** and does not carry to table entries.

**Offset 450 of `Data_02000240` is the scene sub-state, cross-overlay.**
`resource_384:01e4` and `resource_378:0070` both read it as a *signed* halfword
and both build the 450 the same odd way; `378` switches 35 ways on it, `384`
three ways.

**The displacement/value trap has a fourth variant: displacement → value →
displacement.** `resource_384:01e4` builds r2 as 448 (a displacement),
`adds #73` → 521 (the stored value), then `subs #71` → 450 (a displacement into
a *different* base). The 448/521 pair is already recorded; the third step is new,
and 450 has no arithmetic relation to either.

**`0x03001ecc` and `0x03001ebc + 16` are the same slot, and one overlay uses
both spellings.** `resource_397:015c` and its near-twin root `:0200` differ in
exactly this, which makes two identical routines look like they touch unrelated
globals.

**A `bl` where only r0 is loaded is not necessarily one-argument.**
`resource_397`'s two `Func_080091a8` sites leave r1/r2 live from the two lines
above, holding the freshly-stored x and z. Reading it as
`Func_080091a8(layer)` drops an established three-argument probe ABI.

**`strh r5,[r5]` with r5 = `0x04000208` is an IME CLEAR, not a store of data.**
Storing the register's own address writes the low halfword 0x0208, whose bit 0
is clear. `resource_36f:02e8` wraps five queue enqueues in
`ldrh/strh r5,[r5] … strh r1,[r5]` critical sections. Read as a data store it
invents a phantom value.

**A modulo helper called with a step of `modulus ± 1` is a SEARCH, not a wrap.**
`resource_3cd:04b0` re-enters `Func_030003ac` with 270 on the first pass and
then 269 or 271 — that is `counter ∓ 1 mod 270`, written as an addition because
the helper needs a non-negative dividend. Reading the later steps as another
plain wrap deletes the entire availability search.

**A close without its open is a real defect IN THE ORIGINAL SOURCE, not live
caller state — convert it, do not skip it.** `resource_3cd:04b0` saves and
restores r9, reads it exactly once as a window handle for `Func_08015018`, and
never writes it; its twin `resource_3ce:0cf4` has two `Func_08015010` calls where
this copy has one. Frame-balanced and self-contained means convert with an
uninitialised local. The skip rule is for state that genuinely crosses the
owner's boundary.

**Two sibling band guards whose biases differ by exactly 0x4000 check each
other.** `resource_3a9:018c` (`+0xffff9fff`) and `:01fc` (`+0x5fff`) share the
`0x3ffe` bound and cover adjacent facing arcs — a free correctness proof on the
trickiest constant in that family.

**A compare chain with a HOLE is not a range.** Both `resource_3a9:007c` and
`:033c` accept `9..15 or 17` over the same halfword, excluding 16 from the
middle. Two independent owners agreeing on the hole confirms it; writing `9..17`
folds it away.

**`overlay_call_targets.ts`'s site count on a jump-table row can be right by
luck.** `resource_3b0:0240` reports 168 code bytes against a 452-byte span
because the linear walk stops at a 56-byte `mov pc,r3` table — yet the naive
scan still agreed at 28 sites. Seed the table from the pool word the dispatch
loads and the `cmp`/`bhi` bound before believing either number.

**A jump table's own base pool word is a FREE link-base witness.**
`resource_3af`'s table base is `0x020080ec` and the table physically sits at file
offset 0x00ec; `resource_3b9`'s is `0x020080ac` at 0x00ac. This is the cheapest
base proof available on any dispatcher row, and it falls out of the same read
that seeds the pool map.

**A 66-entry jump table can be 6 distinct arms and 57 default entries.**
`resource_3b9:007c`. Grouped `switch` cases over the raw selector, plus the
table's own default, is the faithful spelling; one arm per entry is inflation in
its purest form.

**`ldrsh` immediately followed by `lsls #16 / lsrs #16` is a signed load consumed
UNSIGNED, not a redundant pair.** `resource_3c1:0120`. Keep both halves — the
table is declared signed and read unsigned.

**The displacement/value trap with THREE roles in five instructions, all in one
register.** `resource_3c1:022c` runs 448 (displacement) → 256 (stored value, via
`subs #192`) → 456 (next displacement, via `adds #200`). This is the reference
example for the subtractive form, and it independently confirms that `+448`
carries the scene id 256, matching `resource_3a2:11b0`.

**`Func_0808a080(0)->[+0x55]` is a cross-overlay control byte.**
`resource_393:0aac` brackets it (mask with 0x7e on entry, restore on exit) and
`semantic/overlays/resource_370_c_02000054.c` clears the same `record[85]` after
the same accessor. Two independent overlays agreeing is what names the field.

**Reading TWO neighbouring byte-exact siblings before starting is worth more
than reading one.** `assets/code/resource_3b9_c_02000030.c` and `_02000238.c`
between them fixed `Data_02000240[224]` as the map id and `[225]` as the
sub-state, the result spelling, and the significance of sub-state 12 — settling
`resource_3b9:007c` before any dataflow work.

**The four defect shapes the multiset checker reports, and what each one means.**
Learned from auditing 69 converted overlays; 45 were clean, and the failures
sorted into exactly these:

| reading | meaning |
| --- | --- |
| wholesale failure, source names `Func_0200xxxx` the assembly never calls | pre-rule naming — a rename pass, not a re-conversion |
| `asm=1 src=2` on a bracket-close import | a shared close written once per arm; use a label and `goto` |
| `asm=3 src=4` on one target with `asm=2 src=1` on the NEXT | a site attributed to the adjacent veneer entry — they are 8 bytes apart and easy to be one off on |
| a large shortfall (`asm=18 src=3`, `asm=46 src=26`) | a repeated block folded into a loop, or arms merged — but check for an interior pool first, because if the "missing" sites are pool words the *assembly* count is the wrong one |
| `asm=1 src=0` | a call dropped outright, often an intra-overlay call to another row |

Never bend a source to satisfy the checker. Its declaration filter is the
fragile part; one honest "the tool is wrong here" beats a source edited to fit.

**A bracket-close import appearing MORE times in your C than in the histogram
means you wrote a shared close once per arm.** Measured across the eight
worst-failing overlays, three of the eleven genuine count defects were exactly
this — `Func_0808a020` with `asm=1` against `src=2` or `src=3`. Reach the close
with a label and a `goto`; per-arm copies inflate the multiset just as merging
deflates it.

**Audit status, measured over all 58 converted overlays: 32 clean, 26 with
mismatches — but 4,253 of 4,371 mismatch lines in the eight worst are ONE
mechanical defect.** Those files were written before the `target = stored + 2`
rule and name callees by the pre-rule address: 3,308 lines are a
`Func_0200xxxx` the assembly never calls, and 945 are the mirror — the real
import, absent from the source. Control flow and call counts are right; only the
names are wrong, so this is a rename pass, not a re-conversion. Only **11** lines
across those overlays are genuine count differences.

**Mechanise the multiset proof: `bun tools/overlay_multiset_check.ts <ov>
[ownerHex]`.** Four lanes independently hand-rolled this before it was promoted
into `tools/`. It compares the per-target `bl` histogram against
`Func_xxxxxxxx(` counts in the finished C and exits non-zero on any mismatch, so
a lane can gate its own loop on it. Two subtleties it already handles, both of
which cost lanes time: comments and declarations must be stripped before
counting (**including the owner's own definition line**, or the function counts
as a call to itself), and a long `bl` landing inside the owner's own span is a
`goto` rather than a call.

**A near-twin of a BANKED EXACT source is the strongest single proof
available.** `resource_394:07e0` against `assets/code/resource_394_c_020008b0.c`
is 21 steps in the same order differing in four immediates; it named all ten
imports backwards in one read with zero dataflow work, and exposed that one
printed name there takes two different arities at different sites.

**Verify a six-argument extraction against a banked argument LIST, not just an
import identity.** `resource_394:0150`'s else arm is `(0, 0, 1, 4, 6, 9)` —
literally `Func_020019cc(0, 0, 1, 4, 6, 9)` in the exact sibling. That is a free
check catching a swapped `sp+0`/`sp+4`, which nothing else in the row would
catch.

**The exported-entry veneer table's LENGTH is told by the first prologue after
it**, and every entry is a root — six entries to 0x2f in `resource_398`, five in
`resource_394`. Both lanes' call graphs fell out of that in one read.

**A large `call_via` count is NOT evidence of a thunk.** `resource_398:0538` has
**15** sites to `0x02000904`, a bare `bx lr`, and no site loads r3/r4 — it is a
one-argument no-op leaf. Check the argument registers before believing an
indirect call.

**The 12-byte interaction record `{key, param, handler | 1}` is shared across
overlays** (`resource_398` and `resource_394` both use it), and the handler word
names an unconverted row's role before disassembly. Where the key's second word
is an event-flag id, the handler sets that flag — `resource_398:0214`, key
`0x08830008`.

**The strict-queue filter HIDES real dispatchers, and they convert normally.**
`resource_3b1:012c` and `:037c` fail the filter purely *because* they contain a
`mov pc,rN` table: the linear walk stops at the table, so `code_bytes` comes out
a small fraction of `span_bytes` and `calls` reads as 0 or 1. Both converted
without incident, beating their advertised counts by 10 and 11. **The tell is a
prologue row, not contained, whose `calls` is 0–2 against a span of 128+ bytes.**
**This tier is narrower than it looks, and it has now been over-claimed twice.**
Lanes offered `resource_3af:00c4`, `resource_3b9:007c` and `resource_378:0070`
as further instances; all three have `returns=1` and were ordinary
**strict-queue** rows. A `mov pc,rN` table depresses `code_bytes` and `calls`
without removing the row from the queue, so those symptoms alone prove nothing.
**Check `returns` before counting one** — that is the field the strict filter
turns on.
Measured across the whole inventory, only **2 such rows remain unconverted (548
bytes, in `resource_3ca` and `resource_399`)** — so this is a tier worth
knowing about, not a large hidden pool.

**A "band guard" family: `ldrh +6` / `adds 0xffff5fff` / `cmp 0x3ffe` / `bhi` is
an unsigned half-open range test on a wrapped position word** — not a mask and
not a sign trick. It appears seven times across `resource_386` and
`resource_38c` and anchors both overlays' approach-guard families, so
recognising it identifies a whole family from the first row.

**`movs r3,#N / negs r3` (or `movs r3,#0 / subs r3,#N`) is an AND-mask of `-N`,
NOT of `~N`.** Three times here: `resource_386:0570` (−33), `resource_38c:04c8`
(−13), `resource_3b1:02f4` (−13, used for two consecutive stores from one
register). `-33` clears only 0x20; writing `~0x21` is wrong by one bit.

**The additive displacement/value trap recurs VERBATIM across overlays.**
`movs r2,#224 / lsls #1` (448, a displacement) then `adds r2,#73` (521, the
stored value) appears identically in `resource_386:04e4` and
`resource_38c:04c8`. Grep for the constants directly.

**An overlay `bl` to an in-image prologue is ordinary in cutscene overlays.**
`resource_386:02fc → 0200011c` and three sites in `resource_3b1:012c` are plain
intra-overlay calls, classified correctly as `prologue`. Do not treat one as a
sign of a mis-decode.

**`stmia r3!, {r0,r1,r2}` with r3 = `0x040000d4` is a DMA3 CLEAR, not a struct
copy.** `resource_381:330c` zeroes its 404-byte workspace this way: `0x85000065`
is enable | 32-bit | source-fixed with a count of 0x65 words, exactly the size
just requested from `Func_08000148`. The `subs r3,#12` after it merely rewinds
the auto-incremented register and is dead. Read as a struct copy it invents
three phantom fields.

**A workspace-allocator call at the top of BOTH an installer and its task is the
cheapest way to pair them.** `Func_08000148(33, 404)` appears identically in
`resource_381:330c` and `:301c`, and the installer's odd `Func_080000d0` pool
word then names the task outright. The size + id match is a free structural
proof, and it hands you the whole struct layout before either body is opened.

**`adds rN,#255 / lsls #24 / lsrs #24` is a u8 DECREMENT.** Twice in
`resource_381:301c` (a 3-frame tick and a 24-frame blend counter). Read as an
add-255, the "one frame in three" gating that keeps ten call sites off the other
two frames is invisible.

**The pool-word band test is TWO-SIDED.** Documented for `0x02000240` as "below
the band, so not in-image"; the other half matters just as much.
`resource_381:29a4`'s `0x004039d2`/`0x004049d2`/`0x00404a4e`/`0x00403a52` are
below 0x02008000 and are packed argument words, while `0x02c70000` is above it
and is a 16.16 coordinate. Neither is in-image.

**A three-arm decision tree that differs only in WHICH WORD IT LOADS collapses
onto one `bl`** — a sixth shape for the shared-call-site list. `resource_381:29a4`
does it twice; writing the natural per-arm call inflates `Func_0808a330` from 3
to 5. The tell is that the arms end in a `b` to a common `movs r1,#1 / bl`,
not in the call itself.

**Offset-0 veneer tables come in TWO flavours, and only one of them is a call.**
`resource_389`'s table mixes real entry veneers (`ldr r4,[pc,#0] / bx r4`) with
the constant-loader shape (`ldr r0,[pc,#0] / bx lr`) at 0x0b50/0x0b5c/0x0b64,
plus a bare `movs r0,#0 / bx lr` at 0x0b58. Resolving the table therefore hands
you roots *and* exported data-address accessors — three of six entries here were
the latter. Do not assume every entry names a function.

**A three-word scene-script record names its callback AND its actor selector.**
Shape `(selector | flag << 16, callback | 1, parameter)`. Corroborated five times
across `resource_389`/`resource_38e`: `0xffff0008 / 0x02008b6d` where the owner
calls `Func_0808a080(8)`, and `0xffff0063 / 0x0200915d` where 0x63 = 99 is the
scene id the entry-0 root tests. Settles a row's signature *and* its entry
condition with no disassembly.

**`ldrh` + `subs #k` + `lsls #16` + unsigned `cmp` is a 16-bit WINDOWED RANGE
test, not a signed comparison.** `resource_389:121c`'s `(v-2) << 16 <= 0x80 << 9`
is exactly `(u16)(v - 2) <= 1`. Read without the truncation it looks like a
sign/magnitude test against 0x10000.

**The displacement/value trap has a third variant: value-then-mask.**
`resource_38e:04bc` sets r3 = 0, *stores* it, then `subs r3,#13` to make `~0x0c`
as a mask. One register, three roles, no arithmetic relationship between them.

**A `while` entered at its test looks like a `do` in the listing.**
`resource_38e:05dc` `b.n`s *forward past* a five-call body to the test at 0x073a,
which branches backwards. Reading it as a `do` puts one extra execution of five
call sites on every path — five phantom entries in the multiset.

**A row with `code_bytes == span_bytes` has no pool at all, and the alignment
halfword after it belongs to nobody.** `resource_38e:090c` is 102/102, ending at
0x0971 with `0x0000` at 0x0972 outside the row. Do not attach it.

**An inventory "second entry" row can be the `bl`-decoding artefact ITSELF.**
`resource_379:00dc` is listed as a 2,524-byte contained row, and the banked
byte-exact `resource_379_c_02000054.c` calls `Func_020000dc` — but the halfwords
`f000 f839` store 0x072, so the real target is 0x0074 and `0x020000dc` is an
`ldr r1,[pc,#944]` in the middle of a body. The +2 rule does not only invert
*import* names in exact siblings; it **invalidates inventory rows seeded from the
wrong target**. Check any `contained_by` row whose offset is a `bl` target before
treating it as a real entry.

**A jump table is a pool the walk CANNOT SEE PAST — seed it before believing any
gap.** Walking `resource_3c4:259c` without seeding its two `mov pc,r3` tables
reported 2,496 "pool" bytes against a true 228, a 10× overcount that looks
exactly like a mis-spanned row. Read the table first: base = the pool word the
dispatch loads, entry count = the `cmp`/`bls` bound, entries even.

**A long `bl` to the owner's own epilogue and a jump-table entry can name the
same address with different meanings.** In `resource_3c4:259c` five `bl`s and
three of table B's entries all land on 0x2fda. The `bl`s are `goto`s — **excluded
from the multiset** — while the table entries are ordinary `default` arms. Five
phantom calls if the distinction is missed.

**`overlay_call_targets.ts`'s naive overlapping scan agreeing with a proper CFG
walk is CORROBORATION, never a substitute.** It matched per target on both
`resource_3c4:259c` (211/211) and `resource_379:0074` (287/287) — but only
because neither owner's pool happens to hold a BL-shaped word, which is the exact
case the walk exists to survive.

**The pool hop can be the ONLY branch in a kilobyte-plus owner.**
`resource_3ce:029c` is 1,574 bytes of pure straight line whose single branch
instruction exists solely to hop its one pool word — and the hop is
mid-computation (r1 = 236 set before it, consumed after). A walker that treats
"first branch" as structure, or that stops carrying registers across a branch,
silently drops an argument. Same shape at `resource_37a:2108`.

**A `bl` count of 191 to a single import is a SCRIPT TABLE, not a loop.**
`resource_3ce:029c` is sixteen runs of an identical `movs/movs/bl` triple with no
counter, no back edge and no compare; folding it would have deflated the multiset
by 175. The banked byte-exact sibling `resource_3ce_c_020008c4.c` spells its own
run out the same way — check for such a sibling before "tidying" a long run.

**A one-shot gate proves itself when the scene's own tail sets the flag it
tested.** `resource_37a:0488` opens `if (Func_080770c0(0x809)) return;` and closes
`Func_080770c8(0x809)`. Gate and setter agreeing settles "is this one-shot" in
one line — the positive counterpart to the documented trap where a gate flag's
setter lives in a different owner.

**Non-sequential refresh order is a free cross-file witness.** Both
`resource_3ce:029c` and `:0b10` close with `Func_08077010` over slots in the
order 0, 1, 3, 2. Two independently-read owners agreeing on an odd ordering
confirms neither was transcribed with a swapped pair — look for these rather
than smoothing them out.

**A byte-exact sibling names the imports for you — backwards.** The banked
`assets/code/resource_3b6_c_0200073c.c` was written with the printed (wrong)
`bl` names, but resolving its four sites through the rule gives veneer offsets
0x9e0/0xa38/0xa48/0x9e8 → `Func_0808a018`/`0808a170`/`0808a180`/`0808a020`. That
turned an already-banked file into a *proof* of the begin/message/act/end quartet
used by nine of that overlay's fifteen rows, and settled the `void` return and
the `s32` subject type without inference. Diff an exact sibling against
`overlay_call_targets.ts` on sight — the wrong names are a consistent mapping,
so they invert.

Independent confirmations beyond the arithmetic: `resource_39f:00c4`'s three
lookups — the exact case this file previously listed as unexplained, decoding to
join points *inside itself* — all resolve to `0x0200006c`, whose byte-exact
source returns "the occupying slot or 0", which is precisely how each result is
used. And `resource_373`'s 0x55e0 spread is simply two call sites of one callee.

The tool reports ~8% of sites as `unknown`; those are overwhelmingly pool words
that decode as a BL pair (see the trap below), not unresolved calls. Treat a
large `unknown` count as a signal that a span includes its literal pool.

Everything the earlier "import identity" framing got right still holds — two
`Func_` names can be one callee, arities vary per site — but the identity is now
computable rather than opaque, and the veneer's trailing word gives the import's
real main-image address.

**Six overlays are now confirmed at the 0x02008000 base** — `resource_3bf`,
`resource_3c4`, `resource_372`, `resource_39a`, `resource_371` (five witnesses)
and `resource_3c8` (six, three of them drawn from byte-exact `assets/code`
sources, so the base is proven against banked material). Assume the base until
shown otherwise, but confirm it before relying on any pool word.
Cheapest witnesses, in order: a jump-table base pool word against the table's
physical file offset; an installed per-frame callback pool word that equals a
known function start + the Thumb bit; and any `Data_0200bxxx` symbol that lands
inside the image at `value - 0x8000`.

`resource_372` adds the sharpest disproof of the location reading: its *data*
pool words `0x0200c934`/`0x0200c984` resolve under that proven base to file
offsets `0x4934`/`0x4984` — inside the very band its `bl` instructions appear to
target. A `bl` cannot land inside a proven data block. Sharper still, its
`020031ac` contains `bl .L_02003390`, whose "target" is that owner's own `b.n`
over its first literal pool.

**Consequence for the skip rule below:** "a `bl` into an in-image address is a
hidden-context caller" fires only where the target is genuinely reached as code.
On an overlay whose `bl`s are identities, such rows are ordinary and convert
normally — `resource_39a` converted all 64 on that basis. Establish which regime
your overlay is in *before* skipping anything; the cheapest test is whether the
target range extends past the image end.

**Adjacent equal-size rows are worth eyeballing before drafting either.**
`resource_375:19a4` and `:19e8` are 68-byte bodies identical but for one id
(0x087d/0x087e) and one argument (0/1), and `overlay_call_targets.ts` reports the
same four callees over five sites for both. The second file then costs a minute
and comes with a correctness proof. `overlay_twins.ts` finds these across
overlays; within one overlay, sorting rows by span and scanning for equal sizes
is faster than running anything.

**The cheapest witness that a `bl` target is a per-call-site label: find two
near-identical owners.** `resource_371:008c` and `:00d4` are byte-identical over
all 72 bytes except **two** values (an immediate 42 vs 24, a pool word 0x809 vs
0x80a). Their `bl` halfwords are bit-identical, yet the printed targets differ by
exactly 0x48 — the spacing between the two owners. So `Func_0200421c`/
`Func_02004264` and `Func_020044d2`/`Func_0200451a` are provably the *same two
callees* under four names. The same relation holds for the triplet `:1888`/
`:1938`/`:19e8` (targets 0xb0 apart) and the pair `:155c`/`:1680` (0x124 apart).
A two-value diff between sibling owners is the cheapest proof available, and it
is the positive half of the identity finding above — worth looking for early in
any new overlay.

**Two `Func_` names can be the same import, and one name can take different
argument counts** at different call sites in the same owner. Old-style
declarations (`void Func_02004612();`) are therefore mandatory in overlay
sources, not stylistic — all seven `resource_3b8` files need them.

**The interworking epilogue tells you the return type, mechanically.**
- `pop {r0} ; bx r0` — r0 holds the popped *return address*, so nothing is
  returned: the owner is **`void`**.
- `pop {rN} ; bx rN` with N != 0 — r0 survives and **is** the result.
This removed the usual guesswork on 26 of 35 owners in one overlay. It is the
cheapest signature decision available; check it before reasoning about a trailing
call's r0.

**A `bl` to an in-image address that is not a function start is a hidden-context
caller — skip it.** 24 owners in `resource_3c4` were skipped on this rule, each
verified individually: the target lands mid-instruction, or in a frame-unbalanced
epilogue tail, or in code that needs a register the caller never sets (e.g.
`02001318 -> 020013fa` lands inside another function's `bl`; `02001f5c ->
02002028` enters a `add sp,#8 / pop` tail while the caller holds only
`push {lr}`). The bytes are not in doubt — `assets/code/resource_3c4_overlay.s`
spells them literally — the *meaning* is. Two shapes that DO check out and are
ordinary calls: a balanced shared tail declared but not defined, and an alignment
`nop` immediately before a real prologue (calling it is calling the function two
bytes later).

**Third shape that checks out — and it is a THIRD case of that skip rule, not a
skip: a `bl` into the owner's OWN body that lands on an arm running into the
owner's own epilogue is a non-returning `goto`, not a call.** `resource_3a8:0590`
has two of them (0x020005aa and 0x020005b8, both to 0x0200151c, both reported
`unknown` by `overlay_call_targets.ts`). The test is mechanical and takes one
minute: (1) the target is inside the owner's span, (2) nothing branches to it
except by falling past a `b` over it, and (3) following it reaches the owner's
own epilogue, which pops the frame the *prologue* pushed — still intact, because
the `bl` pushed nothing. Then the `bl` executes that arm and returns to the
owner's caller; the clobbered lr is dead because the real return address is on
the stack. Spell it `goto`. Under the existing wording these two look exactly
like "a frame-unbalanced epilogue tail" and would have cost a 4,092-byte owner.
The distinguishing question is *whose* frame the tail unwinds: another
function's (skip) or the caller's own (goto).

**Drafting loop: compile YOUR file alone, not the whole lane.** `bun run
build:semantic` is a shared gate — one agent's broken file blocks validation for
every concurrent lane, and did so for about an hour in one round. Compile a
single source through `sourceToAssemblyPlan` from `tools/alchemy_gcc.ts` while
drafting, and run the full `build:semantic` only to confirm before moving on.

**Cheapest link-base witness: a pool word that is a known function start plus the
Thumb bit.** `0x0200a609` = `Func_02002608 + 1`, `0x0200a7ad` =
`Func_020027ac + 1`, `0x02008801` = `Func_02000800 + 1`. These are per-frame task
pointers passed to `Func_080000d0`/`Func_080000d8`, not data. It works on any
overlay that installs a task and needs no jump table.

**But a byte-exact sibling's POOL-WORD TYPING predates the parity test and can
be wrong.** `assets/code/resource_3ba_c_0200384c.c` declares
`extern u8 Data_0200b1c1[]` and passes it to a two-argument import. `0x0200b1c1`
is **odd** — it is `Func_020031c0` plus the Thumb bit, and the import is the task
installer `Func_080000d0`. A callback spelled as a data array is invisible to the
build and wrong in the model. Trust siblings for field offsets and workspace
pointers; re-check their pool-word kinds against parity.

**Cross-check imports against a byte-exact sibling — it is free and it is
banked.** An `assets/code` source in the same overlay was written with the
*printed* (wrong) `bl` names, e.g. `Func_02002d10`; resolving its own call sites
shows the real import is `Func_0808a080`. Diffing an exact sibling against
`overlay_call_targets.ts` therefore confirms each import's arity and field
offsets against material that already reproduces the ROM, rather than inferring
them. This is how one lane fixed its actor-record layouts instead of guessing.

**`Data_03001ebc` is a pointer CELL, not the workspace.** `ldr r3,[pc] / ldr
r2,[r3]` loads the pointer, so `*(u8 **)Data_03001ebc` is one dereference too
many; the byte-exact `assets/code/resource_3c7_c_0200048c.c` spells it correctly
as `u8 *state = Data_03001ebc`. At least one lane made the error and caught it
against that sibling.

**The skip-beat counter is a general idiom, not a one-overlay quirk** — it
recurs verbatim in `resource_3c6` (`movs r3,#236 / lsls #1` off `0x03001ebc`,
three times), where the two variant arms are *behaviourally identical*, differing
only in where the bump sits relative to the last call.

**A provably dead call is still a call SITE.** In `resource_3a7:0754` the sample
is built with `lsls #11 / lsrs #16`, so it is always non-negative and the `bge`
guarding the unsigned-to-double `+2^32` correction is always taken — the call can
never execute. Deleting it drops one from the multiset. Expect this wherever
soft-float code converts an unsigned value.

**A repeated endpoint block at the end of a builder is two sites, not a loop.**
`resource_3ba:33a0` ends with two 78-byte blocks differing only in a selector
field and one tile offset — and the second's final `bl` sets only r0/r1 where the
first sets r0/r1/r2. Folding them into a two-iteration loop would have deflated
the multiset by four *and* silently normalised away that dropped r2. The tell
here is a dropped argument register rather than a moved counter bump.

**Behaviourally identical skip-beat arms are still DISTINCT call sites.**
Collapsing three such pairs in `resource_3ca:0430` would have deflated the
multiset by six. Identical behaviour is not a licence to merge arms — the
per-target count is over call *sites*, not over distinct behaviour.

**`goto` is sometimes the FAITHFUL spelling, and per-arm copies are not.**
`resource_3b6:05a8` reaches one `Func_0808a170` from two arms and the following
`Func_0808a180` from three. Restructuring into per-arm copies would have inflated
the per-target site count by two; two labels and a `goto` keep the multiset
exact. Treat a shared tail as evidence for a label, not for duplication —
inflation and deflation are the same class of error.

**Two near-twin owners are worth diffing before either is written.** In
`resource_3b6`, `0200066c` and `0200091c` differ in a way that exposed a
genuinely *dropped* argument (0x080b0008 takes `(27, subject)`, 0x080b0010 takes
`(subject)`); folding them would have normalised that away. In the same overlay
`06ec`/`0760`/`08cc` are byte-identical over all 80 bytes except **one pool
word** (0x239e/0x1fbb/0x23ac) with bit-identical `bl` halfwords — three files for
the price of one, with a correctness proof attached.

**Fixed-point argument constants identify an import.** `Func_0808a090`'s
arguments across `resource_3b6:013c` are 0x10000/0x8000, 0x16666/0xb333,
0x1cccc/0xe666 — 1.0/0.5, 1.4/0.7, 1.8/0.9 against 0x10000 as one. Consistent
x/y pairs in 16.16 are cheap evidence for a scale setter, and cost nothing to
check.

**`0x02000240` is BOTH a cross-overlay RAM global block and a plausible file
offset — a live trap for anyone typing a pool word by eye.** In
`resource_3c2` the overlay's largest function sits at file offset 0x240; in
`resource_3b6:03dc` the identical constant is a RAM address, proved not in-image
because it is below the link band (link base 0x02008000, in-image address =
`pool_word - 0x8000`). Spell the RAM one `(u8 *)0x02000240`, as `resource_370`
does. Check the band before deciding which one a constant is.

**The skip-beat guard appears with BOTH polarities, sometimes in one overlay.**
`resource_377:0f90` tests `Func_0808a070(0,0) == 1` twice while `:0578` tests
`== 0`. Assuming the guard is always `!= 0` inverts those beats. Read the
comparison at each site.

**Grep for the skip-beat counter by its constants, not by asymmetry.** It also
appears on BOTH arms of a test, so "empty else" is not the tell — `movs r3,#236 /
lsls #1` off the `0x03001ebc` state pointer is.

**An "empty else that only increments something" is a skip-beat counter, and it
proves branch symmetry.** In `resource_391:0d3c` an eight-instruction sequence
bumps a `u16` at `workspace + 472` and appears on the *absent* side of nearly
every scene-variant test — 10 sites. Recognising it turns ten puzzling unrelated
conditionals into one flag with two equal-length scene variants.

**A gate flag's setter is often in a DIFFERENT owner.** `resource_375:0170`
tests flag 0x0801 on entry and never sets it; the setter is `:0964`, and `:150c`
reads the same flag to pick a scene variant. Its siblings `:0be0` and `:12a0` do
set their own gates, so the asymmetry reads as a transcription error until the
writer is found. Same shape as the shared-globals rule below — before concluding
a one-shot scene is broken, grep the overlay for the flag's writer.

**Two owners that share globals should be read together.** Neither
`resource_38f:08ec` (which sets three globals, installs a task and spins on one
of them) nor `:27ac` (the emitter, the only writer that clears it) is
interpretable alone; together the globals are unambiguous. When a spin-wait reads
a global nothing in the owner writes, find the writer before drafting.

**Overlays share whole routines verbatim — check before drafting anything.**
`bun tools/overlay_twins.ts --unconverted` groups owners by an instruction
skeleton that masks the two things which legitimately differ between copies:
both halfwords of every BL pair (each overlay's veneer table is at a different
offset) and pointer-shaped literal-pool words (the same data table lives at a
different in-image address). Currently **15,458 bytes sit in groups where at
least one member is already converted** — transposable by substituting
constants rather than read from assembly.

The pool masking was necessary, not cosmetic: the hand-found
`resource_3c4`/`resource_39f` twins differ by 21 halfwords out of 192, of which
20 are BL and **exactly one** is a pool word. Masking BL alone found 3,122
bytes; masking pool words too found 15,458.

**`unknown` from `overlay_call_targets.ts` is not evidence of a hidden-context
caller.** Its prologue set came from the inventory, which is incomplete, so seven
ordinary functions in one overlay were reported `unknown`. It now recognises the
`push` opening (0xb4xx/0xb5xx) directly, which took `resource_39f` from 101
`unknown` to 3. Whatever remains is overwhelmingly pool words that decode as a BL
pair — check the target's first halfword before concluding anything.

**...and it works BACKWARD too.** `resource_370:03cc` has one long `bl` forward
to its own `movs r0,#0` exit and one *backward* to its own main-loop head. Same
test either way: nothing pushed, `lr` dead because the return address is already
on the stack.

**A veneer pointing into IWRAM is not always `call_via`.** `resource_370`'s
veneers at file offsets 0x1314/0x131c resolve to `Func_03000380`/`Func_030003ac`
— the relocated divide and modulo helpers — reached as ordinary two-argument
calls through the normal veneer table, with no r3/r4 load at all.

**A `bl` can be a long unconditional branch to the owner's own exit.**
`resource_3c4:259c` has five that resolve to its own epilogue, past `b.n` range.
They are not calls; `lr` is clobbered harmlessly because the epilogue pops the
return address off the stack. This inflates site counts and explains a class of
resolved targets that are neither veneer nor callee.

**State the completeness cross-check as "placed >= row count", never equality.**
The inventory's `calls=` field predates the corrected `bl` decoding and is a
lower bound: measured gaps of 22/23, 19/20, 54/88 in one overlay and 0/17 in
another. Equality is not the target; the multiset comparison below is the actual
proof.

**The inventory's `calls=` field predates the corrected `bl` decoding and is
systematically LOW.** Measured on one overlay: 0 against 17 real calls, 58/61,
62/64, 70/75, 87/91, 143/151, 163/169. It is not a completeness proof in either
direction — use `overlay_call_targets.ts`'s own `sites=` count, or better, the
multiset comparison below.

**Parity of an in-image pool word decides data from code — a one-bit test.**
Under the proven 0x02008000 base, `0x0200aXXX` words split cleanly: **odd** is a
Thumb function entry (a task callback), **even** is an in-image data address (an
animation script or table). Thirteen even words in one overlay all appear as the
second argument of `Func_0808a098`/`0808a0b0`/`0808a168`; every odd one is a
function + 1. Cheaper than any other witness, and it settles whether a pool word
should be declared `extern u8 Data_[]` or as a callback.

**Thumb bit tells a jump table from a handler table.** `mov pc,rN` does not
interwork, so jump-table entries carry **no** Thumb bit; an installed-handler
pool word does (`0x0200c8c9` = `Func_020048c8` + 1). Cheapest way to classify a
table of in-image addresses at a glance.

**`overlay_call_targets.ts` misreports a two-byte `bx lr` leaf as `call_via`.**
The classifier recognises the thunk bank by the `(halfword & 0xff87) == 0x4700`
shape, so any genuine empty hook collides with it — `resource_3bb:3228` is a real
standalone no-op leaf. Before treating a `call_via` classification as an indirect
call, check whether the caller actually loads r3/r4. If it loads nothing, it is a
no-op leaf, not a thunk.

**A pool load before a `bl` is only a `call_via` if it names IWRAM — and the
band is wider than `0x030001xx`.** `0x03001388` is one, so the discriminator is
"IWRAM, and the bank entry is `bx rN`", not a narrow address range. Where the
loaded word is in-image data (`0x0200dxxx`) instead, r3 is an ordinary fourth
argument.

**That pool word is the code address itself, not a pointer cell.** The shape is
`ldr rN,[pc]` then `bx rN`, so `*(Helper *)0x03000164` is wrong and
`(Helper)0x03000164` is right. One lane wrote the dereference and corrected it.

**An overlay can have its OWN `call_via` bank** (`resource_3cb` at 0x020018f0+,
`bx rN / nop` pairs) separate from the main image's at `0x080072e4`.

**Annotate the listing instead of hand-pairing:**
`bun tools/overlay_show.ts <ov> <off> -n <n> | bun tools/overlay_call_targets.ts
<ov> --annotate` rewrites each `bl` with its real callee. Three lanes wrote this
by hand before it was promoted into the tool; it removes the error class below
entirely.

**Resolve site -> target with `--json`, never by pairing the tool's summary
against call shapes.** The summary is a *histogram*, not a mapping. One lane
inferred the mapping from argument shapes and got it exactly backwards —
`Func_0808a080` is the scene-record accessor and `Func_08009278` the
four-argument action, the opposite of what the shapes suggest in isolation. A
two-import owner has a 50% chance of reading plausibly backwards. What settled it
was a third owner using `Func_0808a080(0)` as an accessor independently.

**A shared call site reached with DIFFERENT arguments still must not be
duplicated in C.** `resource_3cb:12e0` has one site reached with r0=0 from one
arm and r0=4 from another; `:0b94` has one site fed four cue ids and another fed
by five paths. Writing the natural per-arm calls injects phantom calls into the
multiset — restructure to a shared `emit:` join instead. A site count alone will
not catch this; the multiset will.

**The multiset check earns its keep — it caught a real over-count on the largest
owner in the project.** `resource_3c8:3068` (3,922 bytes, 248 sites) came out at
228 C calls against 224 real ones. The culprit was a two-instruction tail that
**four** different scenes' jump tables enter directly; writing it inline four
times inflated the count, and one `goto` fixed it. Its 24 `unknown` sites all
resolved to the owner's own `movs r0,#0` return — long `bl`s, not calls.

**The FIFTH shared-call-site shape: the condition-feeding call.** When a lane
transcribes a straight-line run with a simulator and then hand-writes the `if`s
around it, the `bl` whose r0 the `cmp` tests appears twice — once in the
generated run, once in the condition. It inflates by exactly the number of
branches, so on a 306-site owner it reads as a plausible near-miss rather than a
bug. **End each generated segment one site before the test.**

**The shared-call-site trap fires in at least FOUR shapes, and decision trees
hit it constantly.** Writing one C call per arm injected 4-8 phantom calls in
five separate owners of one overlay. The shapes:
- one `bl` reached from several arms (use a `goto` into the shared arm);
- an arm that *falls through* into the next arm's call site (a plain
  fall-through, not a second call);
- several jump-table arms funnelling into one `bl` that differ only in a register
  the arm left set (a `switch` with grouped cases);
- a shared site reached with different arguments (hoist them into locals).
The per-target multiset is what catches all four.

**A "transition family" is the highest-value pattern in a map overlay.** Ten of
`resource_3b2`'s seventeen owners are one family around a single hub: read the
actor's tile cell, switch on column or row, check companions against a band, call
the hub, wait two frames, repaint two collision rectangles. X-axis members pass
the id as `dx`, Z-axis members as `dz` with the rectangle transposed, return legs
negate it. Convert the hub first and each member costs minutes while the family
cross-checks itself. **`overlay_twins.ts` does NOT find these** — the bodies
differ too much — so sort rows by span and eyeball adjacent sizes.

**...but only the SELECTOR is shared, not the arms.** Three `resource_38b`
owners test it against the same constants 0x1e/0x23/0x20 and map them to
*different* targets each time. Reading one owner settles the selector's layout
and nothing else.

**`Data_02000240[224]` is a cross-overlay idiom with a fixed shape** — the
signed halfword at byte offset 448, branched on. Four byte-exact siblings
(`39a:0050`, `3b2:0d48`, `3b7:0044`) plus `370:0384` share it, so reading one
settles the layout for any new overlay that loads `0x02000240`.

**Small pooled constants in byte-exact overlay sources are spelled
`(s32)&Value_000000NN`.** That is a pooling device for the exact lane, not
meaning. A semantic file should write the integer and say so, or a reader will
hunt for a symbol that does not exist.

**`>> 20` on a 16.16 coordinate is the tile-grid idiom**, not an odd shift:
`>> 16` to integers then `>> 4` for the 16-pixel grid. Read as a single shift,
every column and row constant looks arbitrary.

**The span scan can BE the lane.** On `resource_3a7` it exposed a six-member
family, a bit-identical pair differing in eight immediates, and a subset pair —
**twelve of twenty-four files were transpositions**. Sort by (span, calls) before
anything else on a high-row-count overlay.

**Calibration, honestly: the span scan also produces false positives.** In
`resource_38b` two rows matched on span AND `calls` (220/15 each) and were
unrelated, and a 100-byte pair turned out to be caller/callee. The scan costs two
minutes and the disassembly is not wasted, but "equal span and equal calls" is a
candidate filter, not a twin test — confirm with a byte diff before transposing.

**Five lanes have now confirmed this; treat `groups=0` as no information.**

**`overlay_twins.ts` misses same-overlay twins, and it misses them often.**
`resource_399:07a4` and `:088c` are two 232-byte rows differing in five values —
`--unconverted` reports nothing for that overlay, yet sorting rows by span and
eyeballing equal sizes found the pair in seconds and made the second file a
two-minute transposition with a built-in correctness proof. **Always do the
sort-by-span scan even when the tool says `groups=0`.** Reported by four separate
lanes now.

**Equal span AND equal `calls` is a stronger twin filter than span alone** — it
found a bit-identical 64-byte pair differing in three pool words that
`overlay_twins.ts` reported as `groups=0`.

**A TOTAL-count match can hide a permutation — this is why the proof must be
per-target.** On `resource_370:03cc` the naive total was **137 = 137 with five
targets mutually mis-assigned**: `Func_08015070`/`08015280`/`08015088` mis-paired,
and divide swapped with modulo (`Func_03000380`/`030003ac`). The total was
identical before and after the fix. A count proves nothing; only the per-target
comparison caught it.

**Shapes that DEFLATE the multiset — the mirror of the inflation list below,
and the reason to compare per-target rather than eyeball a total.** Both fired in
one overlay:
- A `movs r3,#N / strb` **value** register surviving into the next `bl`, read as a
  phantom trailing argument. This is distinct from the documented `ldrsh`-offset
  variant, which also fired three times in the same overlay.
- A condition written twice, when a generated straight-line body is spliced with
  a hand-written `if`.
A net count can hide one of each. Per-target comparison cannot.

**Two call-site shapes break a naive multiset, both by inflating it.** State the
proof as per-target counts and account for these before trusting a mismatch:
- A `bl` reached from two control paths is one site but would be two C call
  expressions; spell it once and `goto` the shared target
  (`resource_3c5:28a0` at 0x02002af6, entered from both the head and a jump-table
  case that branches into the middle of the body).
- A long `bl` to the owner's own epilogue is not a call at all.

**Completeness proof, best form: compare MULTISETS.** Extract the multiset of
`bl` targets from `assets/code/<overlay>_overlay.s` and compare it to the
multiset of `Func_…(` occurrences in the finished C. On a 2,716-byte owner that
was 245 = 245. This catches dropped *and* phantom calls, which a count alone
cannot.

The inventory's `calls` field is the weaker check because it counts **distinct
targets, not call sites**: `0x02000920` has 20 sites but `calls=18`. The gap is
exactly the number of imports reached with two different argument counts — on
that 2,716-byte owner, 245 sites − 228 distinct = 17 such imports, which the
multiset comparison confirms rather than leaves as a discrepancy to chase.

**And `calls` can UNDERCOUNT outright** where a jump table sits inside the
executable span and disassembles as plausible code: `resource_371:037c` reports
1 call against 4 real ones, and `:06ec` reports 18 against 49. Never treat the
field as an upper bound.

**The overlay image is writable EWRAM, not ROM, and is used as save state.**
`resource_3c8:4bd8` advances byte cursors stored at `Data_0200f72c`/
`Data_0200f78c` — file offsets 0x772c/0x778c under the 0x8000 base — with
`str r3, [r0, r4]`. Dialogue progress lives in the overlay's own data. Do not
model overlay data as `const`.

**Halfword coordinate views: the s16 at +0x0a and +0x12 are the integer parts of
the 16.16 words at +0x08 and +0x10.** The byte-exact `resource_3c8:14f4` already
models the same record twice for this reason (an s32 pair and an s16 pair).
Recognising it avoids declaring an illegal overlapping struct; the
[x, x+7] x [z, z+7] rectangle guards in that overlay are all tile tests on those
halfwords.

**r4 is used as call-clobbered scratch without being saved** in several owners
(`resource_371:011c`, `:01c4`, `:0598`, `:2768` under `push {lr}` or
`push {r5,r6,lr}`; also twice in `resource_372`). Nothing observable depends on
it and the bytes are not in doubt — but it reads as a decoding error, so note it
in the file rather than "fixing" it.

**`contained_by` seeds inside an owner are artefacts of the same thing.** Rows
like `0200153e`, `020028d8`, `02002abc` are plain `movs`/`lsls` instructions in
the middle of an argument block, with the owner's prologue already executed.
Reconstruct the owner whole from prologue to epilogue; they need no separate
treatment and are already excluded by the strict filter.

**A pool word can decode as a `bl`.** `0x02002014`'s clamp constant is the pooled
word `0xf848f003`, which disassembles as `bl 0x2005124`;
`assets/code/resource_39a_overlay.s` spells it as two raw `.2byte`s for exactly
that reason. Any whole-image scan for call targets must exclude pool ranges or it
will invent imports.

**Old-style declarations still need the right return type.** The semantic
toolchain rejects `void Func_X(); if (Func_X() != 0)` with "void value not
ignored as it ought to be". Declare any import used in a condition as `s32` or a
pointer — arity may be left open, the return type may not.

**The `movs r3,#N / ldrsh rX,[r0,r3]` offset register survives into the next
`bl` and reads as a phantom last argument.** A simulator reports
`Func_0808a0b8(slot, x, z, 18)` where 18 is merely the load offset for the +18
halfword. Hit six times in two owners of one overlay — it recurs per-owner, not
per-overlay, so expect it in every cutscene row.

**A pool can be hopped by a bare unconditional `b.n` with no conditional
structure around it** (`resource_3c5:1b10` at 0x02001f0a). A control-flow pool
walk must follow lone forward `b.n`s, not just branch diamonds.

**A real function can hide inside the import band.** `resource_37b:23a4` has an
ordinary `push {r5,lr}` prologue and takes two arguments, but sits between
eight-byte veneer entries, so skimming the band calls it a veneer.
`overlay_call_targets.ts` classifies it correctly as `prologue` — trust the tool
over the neighbourhood.

**Reference example for both displacement/value forms: `resource_399:0f90`.**
Subtractive at `0x02000fda` (448 displacement, `subs #192` gives value 32,
`adds #200` gives the next displacement 232) and additive at `0x02001506` (448
displacement, `adds #73` gives value 521, `subs #65` gives the next displacement
456) — 60 bytes apart in one owner. Read that pair once and the family is
recognisable everywhere.

**The displacement/value trap has a second, ADDITIVE form.** The documented
shape is `subs r3,#192` after a store. The other is `adds r2,#68` / `subs r2,#192`
applied *after* `adds r3,r3,r2`, where the offset that matters is the
*pre*-arithmetic value: reading `resource_37f:092c` as `workspace+516` instead of
`workspace+448` is the natural mistake, and neither owner carrying it has
anything else to catch it.

**A free layout witness: `array_base + count*stride == scalar_base`.** When an
owner writes an array and a scalar drawn from adjacent pool words, that identity
confirms the element count, the stride AND the link base in one arithmetic step,
with no disassembly. `resource_3ca:11c4` has 24 records of 12 bytes at file
offset 0x1af8, and 0x1af8 + 288 = 0x1c18, exactly the counter halfword the same
owner drives — which caught the lane's element count before it could be wrong.

**Order the lane by the CALL GRAPH, not smallest-first.** Resolve every odd
`0x0200_8xxx` pool word in the overlay before drafting anything: each one is
`Func_A` storing `Func_B + 1`, and where B is still unconverted the witness
*names B's role in advance*. On `resource_3a3` all three witnesses were forward
references — they established that `02000c44` was a per-frame callback and
`02000d08` a task before either was opened, which pre-decided their signatures
(`void f(u8 *object)`) and let the pair be cross-checked on field offsets
(`02000c0c` initialises +0x18/+0x1c/+0x64/+0x68; `02000c44` consumes exactly
those). Convert the named callee alongside its installer rather than whenever
its size comes up.

**An empty grep is NOT "no base evidence" — the next witness is the first task
install you meet.** `resource_3a7`'s nineteen byte-exact siblings hold no in-image
pool word at all, and the base arrived free on the first row disassembled
(`0c08`'s `0x02008aa1` = `Func_02000aa0 + 1`, which also named that row).

**Grep for BOTH `0x0200[89ab]` and `Data_0200[89ab]`.** Some overlays' byte-exact
siblings spell in-image data as symbols rather than numeric pool words —
`resource_3a3` returns nothing for the numeric form and everything for the
symbolic one.

**Grep `0x0200[89ab]` across an overlay's ALREADY-CONVERTED semantic files, not
just `assets/code`.** An overlay's task-callback pool words make its rows
self-cross-validating in a chain: four of `resource_395`'s owners name each other
through `+1` pool words, and the largest writes the scratch words the smallest
reads. One command named three of four owners' roles before anything was
disassembled.

**Cheapest link-base witness, full stop — and it needs no disassembly:
`grep -o '0x0200[89ab][0-9a-f]*' assets/code/<overlay>_c_*.c`.** A byte-exact
sibling's pooled task-callback argument is already a proven in-image address.
`assets/code/resource_3bc_c_020001b4.c` passes `0x0200804d` to the installer;
under the 0x8000 base that is `Func_0200004c + 1`, which proves the base AND
names an unconverted row as a task callback — from one read of an eight-line
file. Do this before opening a disassembler on any new overlay.

**Next cheapest: an in-image handler table.** One 24-byte
read of the table at `resource_3c6`'s file offset 0x1ee4 gives two Thumb-bit
witnesses at once (`0x020087c5` = `Func_020007c4 + 1`, `0x020091bd` =
`Func_020011bc + 1`). No jump table, no control-flow analysis — find the table,
read two entries, done.

**An odd in-image pool word passed to `Func_080000d0` is a two-way witness for
free.** It proves the 0x02008000 link base *and* names the installed task. Then
grep the overlay for the counter that task touches — in `resource_37f`, `:092c`
clears the exact word `:1ac8` decrements, cross-validating both files at no cost.

**A sibling family can name its own consumer.** Six 72-byte siblings all tail-call
`Func_020017c0(0)` while the dispatcher calls it with `1`, and that callee's
twelve flag ids are exactly the cue ids the family emits — which gave a 748-byte
owner's argument semantics before anyone disassembled it.

**Two arithmetic traps that an argument-window simulator gets silently wrong.**
(1) One register can be both a stored *value* and the next store's
*displacement*: `subs r3,#192` yields 32, stored at workspace+448, then
`adds r3,#200` yields 232, the offset of the next store. Reading it as
448-192+200 is the natural mistake. (2) A long-lived alias can be *reassigned*
mid-owner far from its uses — `r8` from 0x7000 to 0xb000, `r6` from 0 to 0x9000,
both built by shift chains. Track such registers per-use, not as variables.

**Literal pools inside an owner have bitten three lanes in three different ways.
All three guards are needed together.**

1. *Skip the pool's bytes* — do not decode them as instructions.
2. *But carry register state ACROSS it.* An overlay routinely branches over its
   pool mid-call-setup: `resource_3aa:0770` sets r1=856 and r2=440, `b.n`s over a
   10-word pool, then sets r0=8 and calls — one `Func_0808a0d0(8, 856, 440)`, not
   two fragments. A simulator that resets or restarts at a pool boundary drops
   those arguments silently. Same shape at `:10e8`, and `resource_373` does it
   ten times across three owners.
3. *Never model a pool word as an instruction, even a harmless-looking one* — see
   below.

**A jump table can ABUT its own first case body, with no branch between them.**
The mirror of the pool trap below: `resource_395:12f4`'s table B ends at 0x1477
and entry 0's body starts at 0x1478, and its 41-entry table does the same. The
table ends exactly *at* a target reached only by falling out of the dispatch, so
neither "run to the next label" nor "run to the next branch target" is safe.
Only the control-flow walk is.

**A pool can end MID-ROW, several bytes before the next branch target.** In
`resource_399:0f90` the pool at `0x0200124c` is followed at `0x02001284` by the
body of a spin-wait reached only by a *backward* `beq.n` from below the pool.
Ending the pool at the next branch target would have swallowed two live
instructions. Only a control-flow walk finds this — a heuristic that assumes a
pool runs to the next label cannot.

**Cheap second tell that a gap is a pool: an argument register crosses it.**
`resource_377:0578` sets `movs r0,#8` *before* a bare `b.n` and the `bl` after
the gap consumes it — so the gap cannot be a body boundary. Both "run to the next
label" and "run to the next branch target" would have mis-sized that owner by
~800 bytes. Use it as a sanity check on any walk-derived pool.

**Interior pools vary in size — do not assume the usual 8–12 bytes.**
`resource_3b6:05a8`'s is *six* bytes (one alignment halfword plus a single mask
word), and the halfword immediately after it is code *and* a live `bne` target.
Reading the customary pool length would have eaten a real instruction.

**The manifest's `calls` field is a FLOOR, not an equality test — it only ever
undercounts.** Five `resource_3b6` rows exceeded it (`013c` 54 vs 49, `0328`
24 vs 23, `05a8` 16 vs 14, `07b0` 19 vs 18), in every case with the extra sites
on reachable paths and no interior pool nearby. This documents the *opposite*
direction from the overcount trap above (pool words decoding as BL pairs), and
it is the safe one: a shortfall cannot hide an unplaced call, so treat a row
that beats its advertised count as normal and a row that falls short as a bug.

**Derive the pool map from a CONTROL-FLOW WALK. That method is immune to both
traps below; nothing else is.** Walk the owner from its prologue following
branches, and whatever is never reached as an instruction is pool. On
`resource_391:0d3c` this gave 6,382 code + 318 pool = 6,700 bytes exactly, across
8 pools, on a 672-call owner. Both failure modes below are artefacts of guessing
the pool map from a listing instead.

**The inverse trap: `overlay_show.ts`'s "pool words referenced" list is NOT
authoritative.** Two of its entries in `resource_3a8:0590` are the *low halfword
of a BL pair*, listed because a real pool word (`0x00004ccc`) decodes as
`ldr r4,[pc,#816]`. Excluding them as pool would have dropped two live
argument-setting instructions — the same failure mode as the pool guard below,
in the opposite direction. Derive the pool map from the owner's own control flow
(what the code branches over), not from a referenced-words listing.

**Exclude literal-pool ranges from an argument-window simulator OUTRIGHT — the
"clear only the destination register" guard is NOT sufficient.** A pool word can
decode as a plausible *write* to a live variable: in `resource_3bf:3054` the word
at `0x02003384` decodes as `lsls r5, r0, #4`, which overwrote the live text-line
cursor and produced two wrong line ids that looked entirely plausible. Clearing
the destination register faithfully models an instruction that is not there. Use
the pool map, not a heuristic.

**A `ldr r0,[pc,#0] / bx lr / .word K` pair is a constant loader, not a call.**
`overlay_call_targets.ts` reports it as `unknown`. There is no `push`, so `bx lr`
returns past the `bl`: the site's entire effect is `r0 = K`. Distinct from the
8-byte `ldr r4,[pc,#0] / bx r4 / .word T` import veneer the tool does classify —
do not model it as a call.

**Semi-automated transcription is safe for large call carpets, with one guard.**
For a 7,468-byte owner with 869 calls, a throwaway simulator tracking r0-r3
immediates and pool loads between `bl`s — arity = highest register written in the
window — reduced the work to ~30 hand-written regions. The guard: **clear only
the destination register of an unmodelled instruction, never the whole window.**
Clearing everything drops arguments carried across a `b.n` that hops a literal
pool; clearing nothing leaks a counter-bump constant into the next call as a
phantom argument. Both bugs were hit before it was right. Cross-check the
distinct-target count against the inventory's `calls` field — that caught the one
dropped call, and it is a cheap completeness proof in general.

**SUPERSEDED, kept for the reasoning — an overlay `bl` target is not a location,
and two lanes first explained that differently.** Both were investigating the same puzzle that
`semantic/overlays/resource_394_c_020003f0.c` records as "resident service
addresses that fall numerically inside the overlay's own range". Neither
explanation is settled; do not write either into a file as fact.

*Reading A — shifted link base.* `resource_3bf` behaves as if linked at
**0x02008000**, every absolute pool constant sitting exactly 0x8000 above the
printed offset. Evidence: the dispatcher at `02004638` loads its jump-table base
as `0x0200c64c` while the table is physically embedded at offset `0x464c`, and
each of its eight entries points 0x8000 past the case body it selects — a single
consistent shift.

*Reading B — load-time fixups, address as identity.* In `resource_373`,
`Func_020000c4` encodes three `bl` targets that land *inside itself* at plain
join points of its own control flow, which no call can mean; and `02000030` and
`02005610` are byte-identical bodies whose `bl` encodes `0x020061c0` and
`0x0200b7a0` — the same relative displacement yielding two absolute targets for
provably the same callee. Those two differ by 0x55e0, **not** 0x8000, so a
uniform base shift does not explain `resource_373`.

The readings may both be right for different overlays, or B may subsume A. What
is safe to act on today: the encoded address is a **stable identity for an
import**, not a place to disassemble. Converting by encoded address remains
correct — it is what 394/3bd/3c8 already do — but call it an identity in comments,
and measure your own overlay's base before assuming either. `overlay_show.ts`
resolves `bl` by raw displacement, so its call annotations inherit exactly this
ambiguity.

Convention: keep the printed-offset spelling for symbol names and the raw pool
value for data addresses (both trees already do this, and it is self-consistent),
and note the shift in the file rather than renumbering anything.

**Overlays have their own `call_via` veneers, recognisable only by their setup.**
The main-image thunk bank is identifiable because the bank itself contains
`bx rN`; an overlay veneer is not. The signature is instead: a pool word in
`0x030001xx` loaded into r3/r4 immediately before a `bl`, with the callee's
result returned unchanged. `0x030001d8` is the relocated IWRAM square root, the
same helper the main-image `Func_080072f0` reaches.

**Packed direction words are a family, not a one-off.** `0x0200e190` in
`resource_373` is a 16-entry table indexed by `heading >> 12`, X step in the high
half and Z step in the low half, promoted to 16.16 by `& 0xffff0000` and `<< 16`
rather than by multiply. Three owners in that overlay use it and the
`resource_3bd` conversion recorded the same idiom — recognise it rather than
re-deriving it.

**Inventory field names.** `out/decomp/overlays.json` functions carry `entry` and
`offset`, **not** `address`. A snippet using `x.address.toString(16)` throws and
prints nothing, which reads as "no work in this overlay" rather than as an error.
This cost two lanes time before it was caught.

**Read two or three neighbouring exact sources before starting an overlay.**
`assets/code/` already holds byte-exact C for many overlay functions, and its
field offsets and workspace pointers are proven. One lane independently
rederived `Data_02000240[294]` and a `workspace + 386` store that the adjacent
exact file already contained, and found `0x03001e70 + 76` to be exactly
`0x03001ebc`, the workspace pointer the rest of the overlay loads directly.
Cheapest possible way to fix an overlay's struct layout.

**Pulling from Mercury mid-lane: expect to delete semantic sources.**
`build_semantic` hard-errors when a semantic source duplicates an exact one
(`semantic/main/<stem>.c` against `src/<stem>.c`, or the same basename in
`semantic/overlays/` against `assets/code/`). That is the two-lighthouse rule
enforced mechanically, and it means a Mercury merge *lowers* the semantic byte
count while raising the combined one — 22 main-image and 12 overlay sources went
this way in one merge. Not a regression; do not try to keep them.

Sequence that works: stash in-flight lane work, merge, resolve, delete the
superseded sources, regenerate metrics, commit, restore the stash, then sweep for
duplicates **again** because running lanes will have written more. Tell every
live lane to check `assets/code/<basename>.c` before writing each new file.

Two merge traps, both hit:
- **Union the routing sets in `tools/alchemy_gcc.ts`, never take a side.** Each
  lighthouse adds stems the other lacks. But a scripted union will also happily
  rewrite a stem list *inside* the self-test's expected-flags array, replacing an
  `-O1` conditional with bare stems. The ROM still built byte-identically; only
  `bun run test` caught it. Always run the test chain after a scripted merge.
- **The merge moves the executable denominator**, so the commit subject needs the
  `metrics: correct executable denominator` prefix (§9).

**Registering a main-image owner: two rules that cost a build each.**
1. Every range must be **fully contained in one manifest row**. Agents report
   the code contiguously, which is correct as description but invalid as
   registration: `080e15e8`'s 592-byte span crosses the row boundary at
   `0x080e1a48` and has to be listed as 512 + 80. Split at the boundary; the
   code is still contiguous.
2. Bytes count only once registered. An admitted `.c` file on its own moves the
   metric by the row's *advertised* size — `080ec100` credited 144 of its 3,126
   bytes until its nine ranges were entered.

**The Flash family (`old_agbcc -O1`) has a four-lever recipe.** `08007028`
(112/112) took five probes; `08006d50` (156/156) and `08006e24` (292/292) then
each matched on the **first** probe with the same four levers unchanged. That is
what makes this a family recipe rather than a per-function grind — 560 bytes for
seven probes total. Applied in this order:

1. **Never let a pointer live across a call.** Declaring `u8 *info = (u8 *)...`
   at function top hoists it into a callee-saved register; the reference
   materializes it after the call in a call-clobbered one. Worth 5 halfwords.
2. **Split the read-modify-write so the mask opens the chain**:
   `w = MMIO; w &= 0xFFFC; w |= field; MMIO = w;` matches, while
   `MMIO = (MMIO & 0xFFFC) | field;` loads the field's base too early. Worth 5.
   (Same family as the mask-first RMW rule in §4, on the *other* compiler.)
3. **Keep `base + offset` as base-plus-field, in a nested block placed AFTER the
   masking statement.** A bare `*(u16 *)((u8 *)0x08007C10 + 36)` folds into one
   pool word `0x08007c34` at offset 0; the reference keeps `0x08007c10` pooled
   and uses `[r1, #36]`. Declaring the pointer at the enclosing block's top
   re-hoists it — the nested scope *after* the mask is what makes it work. This
   is the fiddliest of the four and cost three probes on its own.
4. **Name a pointer local to order it against an adjacent built constant.**
   `s32 *status = (s32 *)0x02004C00; f(..., *status);` emits the address `ldr`
   before the `movs/lsls` pair that builds `0x0E000000`; folding the load into
   the call argument reverses them. Worth 3.

Two family facts worth reusing: the info block is reached **indirectly** in the
sector routines (`info = *(u8 **)0x02004C08`, wait-state at `info[16]`, sector
shift at `info[8]`) but **directly** in the chip routines (`0x08007C10`); and
writing both wait-state masks as the same `0xFFFC` literal is what makes
old_agbcc park it in a high register across the call, producing the high-register
save with no source-level coaxing.

**A "direct exit" may be inside the region.** `08006e24` was blocked for "direct
exits into `08006f30`/`08006f32`" and a stack-copied payload. Both claims are
labelling artifacts: `0x08006e24 + 292 = 0x08006f48`, so both addresses lie
*within* the region — they are the two entry points of the function's own
epilogue, promoted to function symbols by the disassembler because they are
branch targets. And the copied payload is never executed here; the Thumb-tagged
pointer to the copy is passed as an argument and executed in the callee, which C
expresses as `(u16 *)((u32)Func_08006f48 ^ 1)` with the length as the difference
of two tagged symbol addresses. **Before believing an "exit", check whether the
target is inside `entry + size`.** It is one subtraction.

What survives is narrower and worth keeping: `Func_08006f48` is genuine
relocatable flash-read code that must run from RAM, and whether *it* is
expressible without inline assembly is untested. That is the real hard case in
this family, and admitting `08006e24` does not prejudge it.

**Adding an `old_agbcc` stem touches FOUR places**, and the self-test guards two
of them separately:
1. `AGBCC_SOURCES`;
2. `AGBCC_OPTIMIZE_O1_SOURCES`, when the unit is `-O1`;
3. the hard-coded `expected` array in `selfTest()` — miss it and `bun run test`
   fails with "old_agbcc source allowlist self-test failed";
4. the **second** hard-coded `-O1` stem list inside `selfTest()`'s
   `expectedFlags` — miss it and the message is "old_agbcc flags self-test
   failed for <stem>".

Both guards are deliberate; update the lists rather than working around them.
Check the self-test's exit status rather than skimming its tail: it prints a
`Bun v...` banner on failure that reads like ordinary output, and the fourth
list is easy to miss because the third one is what fails first.

**Where the remaining semantic work actually is: overlays, not the main image.**
Measured after this session's waves, and it reframes the lane:

| | executable bytes | state |
| --- | --- | --- |
| main image (whole manifest) | 451,338 | 378,986 already semantic |
| main-image continuation owners still open | 16,700 | 17 owners, mostly small |
| **overlay strict rows** | **337,052** | **12,790 semantic — 4%** |

So the main image is close to done and the overlays are barely started: 1,198
strict rows across ~40 overlays, led by resource_373 (18,044), resource_3b8
(15,028), resource_3bf (13,484), resource_3c8 (12,800), resource_372 (10,202),
resource_38f (9,848), resource_3c4 (9,828), resource_371 (9,650). Completing the
semantic lane is now overwhelmingly an overlay job, and anyone budgeting from the
main-image queue will misjudge it by an order of magnitude.

Overlay semantic sources need **no registry entry**: `build_semantic` sizes
`semantic/overlays/resource_NNN_c_0200AAAA.c` straight from `out/decomp/overlays.json`,
unlike main-image owners which must be registered in `semantic/main-regions.json`
before their bytes count. That makes overlay conversion cheaper per byte to
integrate as well as to write.

Mercury works the same overlays for byte-exactness. That is not a conflict — the
branches are separate and exact overrides semantic on merge — but prefer
overlays Mercury is not currently walking, and expect some semantic files to be
replaced later by exact ones.

**Queue trap on a fresh clone.** `bun run semantic:queue` reports `queued=0` on
any clone that has not run m2c, because it only surfaces regions that already
have a draft under `work/candidates`, `work/m2c-ctx` or `work/` — all gitignored.
That is an empty *draft corpus*, not an empty queue. Rank from
`out/full/asm/manifest.json` (bounded work) and `semantic_owner_scope.ts`
(continuation work) instead; rule 3 prefers rewriting from assembly anyway.

Parking rule: park only a specific, evidenced ABI or structural blocker. “m2c
is ugly,” “the owner is large,” and “the first agent ran out of implementation
time” are reassignment signals, not blockers.

**Measured six-agent trial.** Two consecutive three-agent waves admitted five
complete owners for **6,432 executable bytes**. Wave 1 admitted 3/3 owners and
4,408 bytes. Wave 2 admitted `080a112c` (964 bytes) and the complete split owner
`080d0ee0` (1,060 bytes); its third assignment became the `080e47b8` scope audit
above rather than a dishonest head-only conversion. Thus the method delivered
5/6 admissions while the sixth task found and explained a queue-wide sizing
defect. Fix transitive sizing/pool mapping before assigning another continuation
owner; ordinary single-row owners can continue immediately.

The next bounded-owner cohort then admitted **3/3 owners and 9,044 bytes**:
`08023178` (3,320), `08023e70` (2,756), and renderer-family `080ca60c`
(2,968). Their audits accounted for 270 assembly `BL` sites, including four
internal control edges represented as C flow rather than fake callees. This
confirms that large call counts are not themselves a reason to park a bounded
owner. `tools/semantic_queue.ts` now keeps transitive-unsized rows visible but
adds a scope-audit penalty so they cannot masquerade as the cheapest work.

A second large bounded cohort admitted **3/3 renderer-family owners and 8,960
bytes**: `080d1714` (3,384), `080d6970` (3,308), and `080d91dc` (2,268).
Together they account for 272 assembly `BL` sites. Exact dataflow review
resolved their apparent unset inputs, stack-carried publisher dimensions,
runtime handles, and internal frame-loop edges. The renderer family therefore
remains a proven high-yield lane; drain bounded members before returning to
unknown-thunk candidates.

The following unrelated-system cohort admitted **3/3 bounded owners and 6,008
bytes**: `0800aa0c` (1,640), `080acab8` (1,980), and `080f7460` (2,388).
Their 168 assembly `BL` sites include runtime allocator, transfer, renderer, and
uploader callbacks recovered from live register and workspace dataflow. This
confirms that the whole-owner method remains productive after the established
renderer family is exhausted.

The largest remaining bounded-owner cohort admitted **3/3 owners and 7,244
bytes**: `0808c4f8` (2,428), `080a2680` (3,128), and `080b63c8` (1,688).
Their assembly contains 433 `BL` sites. The audits resolved false unset values,
high-register lifetimes, stack-carried outputs, zero-fill transfers, internal
dispatcher edges, and shared mutually-exclusive tails. High call count is now
measured as review cost, not an admission blocker; the three-agent whole-owner
method remains effective on the largest bounded remainder.

The final pre-Venus cohort admitted **3/3 bounded owners and 3,672 bytes**:
`080030f8` (1,076), `080b0aac` (1,272), and `080bf678` (1,324). Its audits
resolved five apparent unset values, reset/SVC thunk semantics, nine
high-register lifetimes, and 193 assembly `BL` sites. One downstream caveat
remains independently blocked: `080c1798` intentionally observes incoming
`r2` residue and is not made ordinary by the `080030f8` conversion.

**Remote-work intake.** Periodically fetch
`origin/mercury` after banking a clean semantic
cohort. Review its delta against the current `venus` ancestry, integrate only
verified nonduplicate work, run the same full verification, then update this
handover. Never merge or pull that branch into a dirty cohort, and never let a
remote metric snapshot overwrite newer authoritative counts.

Latest intake: remote tip `7f7b99b3` was merged after the 6,008-byte semantic
cohort. It added **17,816 exact-C bytes**, taking exact Full-C Byte Share to
**194,390 / 1,339,558 (14.51%)**. The combined tree passed `bun run verify`.

---

## 1. Where to work

In descending order of measured value.

**THE OVERLAY STRICT QUEUE IS FINISHED — and the main image is very nearly
finished too.** Everything below about overlay queues is history, kept because
the method transferred.

**Measured, after two false starts that are worth knowing about.** The main
image's remaining ground by retention class looks like this:

| class | bytes | status |
| --- | --- | --- |
| `keep_structured_asm` | 44,136 | parked by design |
| `keep_asm` | 11,988 | parked by design |
| `split_first` (all `mixed_region`) | 23,432 | **already converted** — see below |
| `merge_with_owner` / `merge_with_function_owner` | 7,088 | 4,528 already covered |
| `adjacent_section_alignment` | 660 | padding |

**All 599 `c_candidate` regions already have a semantic source.** There is no
drafting backlog; the exact lane's constraint is adoption, not authorship.

**The genuine remaining semantic gap is about 2,560 bytes across 36 small
continuation regions** — the ones NOT inside any registered `executable_ranges`
entry. Largest: `0800fd5c` (320), `0808f498` (148), `080e53f4` (136),
`080f474a` (114), `0808b7b8` (108), `080bea9c` (108). Each genuinely has no
independent entry (`live_parent_stack_and_high_registers`, `no_independent_entry`,
`live_owner_`), so each must be folded into its owner's source rather than
written standalone, with that owner's registered ranges extended to match.

**`bun tools/main_xref.ts <address>...`** answers "who references this?" across
the whole image: direct calls, branches, aligned pool words, and pool words
holding **address + 1** — the Thumb bit, which is how an indirectly published
callback is stored. That last case is the one that earns the tool: it is the
only cheap way to tell an unreferenced tail from an independently callable
function, and it proved `0808f498` a real function from five pool words holding
`0x0808f499` with no branch into it at all. **What it cannot decide** is a
region with no inbound reference: a genuine continuation is normally reached by
*falling through*, which leaves no reference, so a pool and a fall-through
continuation are indistinguishable. Of the 32 open continuation regions, 31 come
back that way — narrowed to "pool or fall-through", no further. A reachability
walk from the owner's entry is what settles those.

**"Reached indirectly" is a CLAIM, and `main_xref.ts` is how you check it.** A
lane described all six of its new main-image owners as reached indirectly; four
are confirmed — pool words hold `entry + 1` — but `0801c9c0` and `080b7410` have
**no reference of any kind** anywhere in the image: no call, no branch, no word
holding the address, none holding address + 1, and nothing in the overlay
assembly either. They are reached by a computed address, from data the scan does
not cover, or not at all. The reconstruction is faithful to the bytes either
way; say "caller unknown" rather than "indirect", because the second reads as
established and is not.

**`mov ip,pc; bx rN` (NOT `mov lr,pc`) is the private-ip-return call family**
and the on-sight signature of a `keep_structured_asm` /
`nonstandard_thumb_call_module` owner. Seeing it means **stop**, not decompile.

**`keep_structured_asm` is ABSORBING for continuations.** A `merge_with_owner`
row whose head is `keep_structured_asm` is out of scope even though the row's
own retention looks eligible — the row-level field invites the opposite reading.
Zero of the 126 `keep_structured_asm` rows has a semantic source, by standing
decision.

**A row beginning with an alignment `0000` halfword cannot be a semantic
filename.** `build_semantic` requires `Func_<filename-address>` to exist and the
entry is at row + 2, so such rows must go through `main-regions.json` even when
they are single-range. Four of six owners in one lane hit this.

**`stmia r3!, {r0,r1,r2}` with `r3 = 0x040000b0` programs a whole DMA channel in
one instruction** (SAD/DAD/CNT). The preceding `ldrh/ands/strh` pair on `+10`
masked with `0xc5ff` then `0x7fff` is the standard two-step channel stop. This
identified four sibling functions across three unrelated regions at a glance.

**`objdump --adjust-vma` silently reinterprets `--start-address`.** With
`--adjust-vma=0x080beb08 --start-address=0x080beb08` you disassemble **file
offset 0** — the GBA header — under the address you asked for, and it reads as
plausible garbage code. The correct form is `--adjust-vma=0x08000000
--start-address=<vma>`. This burned a full analysis pass.

**Two traps cost a lane a full pass; do not repeat them.**

**A `split_first` / `mixed_region` row with NO prologue and NO epilogue is not a
mixed region at all — it is one owner's interior.** All 27 `split_first` rows
(23,432 bytes, every byte of the class) already lie inside a registered owner's
`executable_ranges`. The evidence string `mixed_or_invalid_function_boundary`
describes the **classifier's uncertainty**, not a finding about the bytes.
Settle it in one command before any control-flow walk:

    grep -c 'push\|pop\|bx' asm/<row>.s      # 0 means interior, nothing to split

**`semantic_owner_scope.ts <row>` returning `owners=0` means "already converted
and registered", NOT "no owner".** That inverted reading is the trap. Grep the
row address in `semantic/main-regions.json` to confirm in a second call. Note
that file registers spans for only 18 owners against 617 sources, so a source
existing does NOT imply its span is registered or that a neighbouring
continuation is covered — `08021cb8`'s source is a self-contained 208-byte
function ending at 0x08021d88 while its "continuation" starts at 0x08021dfa.
Read before crediting.

Read `asm/<address>.s` — reconstructed disassembly, byte-verified against the
ROM by `build_asm.ts`. **The overlay `bl` rule does NOT apply here**, confirmed
by resolving every `.set sub_*` symbol across four regions against the tree:
thirteen land exactly on a region start with a real source, which the overlay
`stored + 2` artefact would displace mid-instruction. The only non-region
targets were `080072f0`/`080072f4`, the main-image `call_via` thunk bank.

**Overlay strict queues.** Two discovery fixes originally took this queue from
20 rows / 6,110 bytes to 1,334 rows / 311,324 bytes, and rediscovery of
known-exact functions from 14% to 67%. Subsequent exact-C waves have materially
drained that snapshot; **do not reuse its old "untouched" labels.**

Freshest inventory (taken on the Venus side after an exact-C intake): **1,081
strict rows / 205,918 bytes**. Leaders by non-contained strict span were
resource_3c8 8,878 (31 rows), resource_383 8,052 (16), resource_39f 7,638 (38),
resource_3c5 7,374 (16), resource_39a 7,096 (64), resource_3c4 7,024 (58),
resource_3b4 6,242 (65), resource_3b2 5,902 (23), resource_3b8 5,468 (5),
resource_3c6 5,250 (12), resource_3ae 5,212 (21), resource_3bf 5,132 (34).
Mercury has since worked 3c8, 39f, 38f, 372 and 3c5, so regenerate with
`bun tools/overlay_inventory.ts` rather than trusting any ranking in this file —
and never carry a ranking forward across a branch intake.

**Rank by small-row count, not by total strict bytes.** `resource_3b8` is the
witness: 5,468 bytes in only **5 rows**, the smallest 348 bytes and the largest
7,468. A lane spent a full session there and adopted nothing. Overlays with many
small rows convert; overlays with a few huge rows park. Count rows under ~400
bytes and rank on that.

The two fixes were:
1. *Pool skipping* — the scan stopped after a return at the first halfword that
   was not `0x0000`/`0x46c0`, i.e. at every real literal pool.
2. *Entry-shape recognition* — a leaf opening with `bx lr` (`0x4770`),
   `movs rN,#imm` (`0x20xx-0x27xx`) or `ldr rN,[pc,#k]` (`0x48xx-0x4fxx`) was
   refused as an entry, so each such leaf broke the chain and darkened everything
   behind it. Individually worth +154, +37 and +21 rediscovered functions;
   `push {regs}` (`0xb4xx`) contributed 0 and was deliberately left out. These
   shapes are only ever a **relay** — the queue filter still demands
   `starts_with_prologue`, so a widened shape can never itself become a row.

**Raw row counts are now far noisier**: `unconverted_discoveries` grew 3,835 →
12,945 and `data_walk_discoveries` 2,768 → 10,031, because relay seeds landing on
data produce short junk walks. They are all labelled and excluded by the strict
filter, but any consumer summing raw rows will over-report badly. Filtering to an
empty `contained_by` matters more than it used to.

**The cross-overlay adopted-C bridge: measured, and it does not work wholesale.**
424 strict unconverted rows (107,926 bytes) sit at an address where *another*
overlay already has adopted C, which looks like free reuse. Feeding each adopted
source to its address-mate is **1,157 probes and yields exactly zero exact
matches**; 898 of them do not even reach a byte comparison because the two
functions have different spans. Same address in two overlays is not evidence of
the same function — do not budget a session for this sweep, it has been run.

What *does* work is the narrow case the sweep surfaces: rank the survivors by
`differing_bytes / span` and read the small ones. A true sibling shows up as a
handful of differing bytes that are all `bl` displacements, because the same
function in another overlay calls different addresses. Substituting the callee
addresses out of the target's own disassembly then lands it first try — that is
how `resource_384:01d0` converted (5/18 differing → exact by renaming two
`extern` declarations). Three survivors were worth reading; `resource_3c2:0240`
(106/2,068) and `resource_381:0054` (313/3,548) are still open and are the two
largest unexploited leads in this class.

**The small-row tier is the highest hit rate in the overlay queue, and it is now
drained.** It opened at 129 strict rows / 2,558 bytes under 32 bytes — two-call
dispatch stubs and one-compare predicates that read straight off the disassembly
with no drafting loop. **As of 2026-07-30 only 6 rows / 130 bytes remain under 32
bytes**, and three of those are the routing-collision casualties in §7. Current
strict queue: **897 rows / 323,406 bytes**, of which 68 rows / 2,786 bytes are
under 48 bytes and 396 rows / 30,256 bytes are under 128. The cheap tier is gone;
the next band up (48-128 bytes) is where the remaining mechanical work is.

Best overlays by the §1 small-row criterion, recounted after that pass:
`resource_3b4` (45 rows under 400 bytes), `resource_3c4` (37), `resource_3a7`
(31), `resource_39a` (28), `resource_39f` (26), `resource_371` (25).

**`tools/overlay_twins.ts` (arrived from main, 2026-07-30) is the highest-leverage
thing in the overlay lane right now, and nothing has been claimed from it yet.**
It masks `bl` displacements and pool words and digests the instruction skeleton,
so it finds owners that are the same routine across overlays and differ only in
what the linker and the pool make different. **32 twin groups, and not one has a
converted member.** Solving a single member of each transposes to the rest for
**16,846 bytes** of pure substitution work — no assembly reading, just retargeting
the callees and constants out of the twin's own disassembly, which is the play
that converted `resource_384:01d0`.

**The 384x11 group has a structurally correct draft already** —
`docs/TWIN-384-DRAFT.md` carries it with the semantics written out and the three
known divergences named. It emits 173 instructions against the reference's 172;
what is left is register assignment and scheduling in the first quarter. Measure
it by instruction-group equality, not halfwords: at 384 bytes the `bl`
displacements make a raw byte count meaningless (it reads 333/384 while the shape
is right). Start there rather than re-deriving from assembly.

Ranked by what the *second and later* members are worth: 384×11 (3,840 free),
404×8 (2,828), 472×5 (1,888), 164×10 (1,476), 216×6 (1,080), 964×2 (964). Two
carry known blockers — the 60×14 group is §8's squared-distance family at floor 20,
and `resource_391:02a8` in the 164×10 group is §8's `ldrsh`-cursor park — so start
with 384×11 or 216×6, which nothing has attacked. This is a far better use of a
session than walking fresh rows one at a time.

**Then stop reading them by hand — `tools/overlay_wrapper_draft.ts` derives
them.** The whole tail of setup wrappers, forwarders and dispatch stubs has a
body made only of constant materialisation and direct `bl`s, and for that shape
the C is a mechanical function of the disassembly. The tool decodes the row,
models `movs`/`lsls`/`negs`/`ldr`-from-pool into argument values, and emits the
source; `overlay_adopt` stays the oracle.

```sh
bun tools/overlay_inventory.ts                        # refresh first
bun tools/overlay_wrapper_draft.ts --max-span 64 --out work/wrappers
```

**Measured 2026-07-30: 96 rows recognised, 53 byte-exact on the first probe, no
second probe on any of them.** That was 2,848 bytes for one pass. It skips
anything it cannot model rather than guessing, so a `unmodelled=N` count is the
honest remainder, not a failure. Rerun it after every inventory refresh — walking
an overlay reveals new rows, and the marginal cost is one probe each.

Three things it encodes are worth knowing even when drafting by hand, because
each was a wrong first guess that the oracle caught:

- **Argument-setter order names the callee's return type**, exactly as §4 says.
  A reference that sets `r1` before `r0` came from an `s32`-returning callee even
  when the result is discarded; `r0` first came from a `void` one. This decides
  more of these rows than anything else.
- **A pooled constant that also factorises as `k << n`, `k <= 255`, needs
  `(s32)&Value_xxxxxxxx`.** A plain literal builds it with `movs`/`lsls` instead
  and the row misses. `0x1420` (`0xa1<<5`) and `0x13c0` (`0x4f<<6`) were the two
  failures in an otherwise clean batch of 23; every constant that succeeded with
  a bare literal was one that does not factorise. This is §4's table, and the
  factorisation test is the only thing that separates the two cases.
- **`pop {rN}` with N != 0 means the function returns a value.** `pop {r0}` means
  it does not. The epilogue is a more reliable return-type oracle here than the
  body.

**Six-argument calls want function-top locals for the stacked arguments.** The
reference materialises both stack words into *different* registers and then
stores both; we reuse one register and interleave, which is 5 bytes off. Writing
the two stacked arguments as function-top `s32` locals reproduces the reference
exactly — §4's hoisting lever, and no scheduler flag reaches it (all nine were
swept on `resource_398:0148`).

**Re-probing old park notes.** The return-type lever (§4) arrived late, so any
note whose residual is a two-halfword argument-setter swap is stale evidence
rather than a blocker. One sweep closed 8 functions / 1,628 bytes from 41
candidates. See §5 for how to sweep it mechanically.

**The main image** — `docs/DISCOVERY-QUEUE.md`, 727 regions, ~395 KB. Boundaries
are ROM-proven, but that was never the constraint: compiler fidelity is, exactly
as on the overlay side. Seven of ten regions a lane drafted reached a stable floor
within 4-8 probes and then moved for nothing. **Work it by leverage, never by
size** — identical sibling pairs (one draft, two regions), construct families
(one insight amortised over several), and the ten debt regions that already carry
non-baseline routing so the flag is not their unknown. The 2 KB-plus tier parks at
a higher rate than the 40-120 byte rows and costs an order of magnitude more time.

**`asm_c_debt_bytes` is an EXACT-lane metric and semantic work never moves it.**
Only exact adoption deletes an `asm/` region; semantic conversion leaves it in
place by design. Measured: 599 main-image `c_candidate` regions totalling 351,668
bytes still have their assembly present, and 593 of them already have a
`semantic/main/` source. Reading that figure as the semantic lane's backlog
overstates it by more than an order of magnitude — Venus's real remainder is the
strict overlay queue plus the main-image continuation owners.

Do not read the Full-C denominator as the target: it deliberately includes linker
veneers, structural assembly and executable alignment that will never be C. The
tracked measure of real remaining work is `asm_c_debt_bytes`, printed by every
full build.

**The main-image frontier is drafting, and here is the number (2026-07-30).**
616 `c_candidate` regions remain. Every one of them already has *some* draft, but
only **303 have a draft that both compiles and passes the `PROVENANCE.md`
screen**. The other **344 regions — 289,574 bytes — need C written**, because
their only drafts are raw m2c output that fails to compile on m2c's own artifacts
(`subroutine_arg0` undeclared and similar). That is why the exhaustive flag
matrices came back at 1-in-964: there is very little left to sweep, and a lot left
to draft. Budget accordingly.

**"Undrafted" is not "unconvertible", and neither is it a target list.** 104
regions have no draft anywhere, 40,924 bytes, and **not one is a `c_candidate`** —
they are all retained structural classes. `080a8904` is the trap: 14 bytes that
read as a trivially draftable countdown loop, classified `deliberate_busy_wait`.
Always filter by `retention == "c_candidate"` out of the asm manifest; ranking by
size alone will hand you retained regions.

Seed every fresh draft with `bun tools/reference_shape.ts --stem <stem>`, which
reads the return type off the region's own epilogue (§4's rule) instead of leaving
the drafter to discover it through a byte diff.

**The inventory over-reports.** Rows nest, so summing a row set re-counts the same
region many times: resource_381 inflates 17.6x, resource_379 9.1x, resource_37a
5.8x. Always filter to rows with an empty `contained_by`. A lane once burned a
whole session on resource_379 expecting ~22,000 bytes and found 2,628 — 65 of its
66 rows were nested walks inside the veneer bank. `span_bytes` is otherwise
reliable (exact on every row checked across four overlays); it over-reports only
where a walk crossed a jump table, so treat it as an upper bound and derive the
span from the pool rule.

---

## 2. Workflow

**NEVER leave a draft under `semantic/` — scratch belongs in the scratchpad.**
The banking scripts on both lanes (`venus_bank.sh`, `bank_cycle.sh`) sweep with
`git add -A`, deliberately: lanes are told not to commit, so something has to
pick their finished work up, and a completed source sitting untracked between
rounds is the worse failure. The consequence is that **a file under `semantic/`
is a claim of being finished, and the sweep will believe it.** The matching rule
for whoever banks: a commit is not evidence that a proof happened, so a banked
file whose lane report has not been read and recorded is an open item, not a
finished one.

**Span rule.** Function start through its own literal pool. The pool follows the
final return after an optional 2-byte zero alignment word — include both. With no
pool, exclude a trailing `.2byte 0`.

**Step zero on a semantic-backed row: compile Venus's source unmodified and read
the group count.** It costs one command and no writing:

```sh
tools/overlay_group_diff.sh <overlay> <off> <span> semantic/overlays/<file>
```

Where the semantic lane already names callees with `overlay_show.ts`'s raw
annotations — `resource_373` does throughout — the file is a *finished exact
candidate* and the transcription step below is redundant. `resource_373:345c`
came out byte-exact on the first probe with no flags and a struct definition
intact; `:0cd0` needed one flag and no source edit. Batch this across a whole
overlay before drafting anything: the group counts rank the queue for you, and
the free rows fall out immediately.

**When the comparator says exact and `overlay_adopt` rejects, check the callee
names before anything else.** The first suspect used to be a routing collision,
and `cflagsForSource` on the installed path still settles that in one line — but
it is not the only cause, and on `resource_371:011c` it was a clean miss:
default flags on both sides, `groups_differing=0`, `differing_bytes=3`. The
comparator normalises every `bl <target>` to `bl X`, so **it is blind to callee
addresses by construction** and a wrong callee is invisible to it right up to
adoption. Equal sizes with a handful of differing bytes is a branch
displacement, so read the annotation and compare it to what the draft names.
Order of suspicion: callee names, then `cflagsForSource`, then the span.

**Transcribe callee names from `overlay_show.ts`; never extrapolate them.** An
overlay `bl` stores the target's image offset minus two, so `overlay_show`'s
pc-relative `bl 0x...` annotation is wrong for every site — that is exactly what
`overlay_call_targets.ts` exists to correct, and the semantic lane needs the
correction. **The exact lane does not.** It names a callee by the address the
*assembler* must encode, and the assembler computes `site + 4 + stored
displacement`; the annotation's error and the encoding are the same error, so
they cancel. `overlay_show`'s raw `bl 0x0200098c` is the exact lane's
`Func_0200098c`, verbatim, byte for byte. Verified against the banked
`resource_38c:0470`, whose four callees all reproduce as
`site + veneer_offset + 2`.

Two consequences:

- **The one-byte adoption failure disappears.** Equal sizes with a single
  differing byte is a branch displacement, and every instance of it this session
  came from guessing a callee address off the spacing of its neighbours instead
  of reading it. Transcription removes the whole class.
- **Sibling families become a `sed`.** `resource_38c:005c → :00bc` was eight
  symbol renames and four constants and was byte-exact on the first probe.

The rule also predicts two collisions, and both are real. One callee reached
from several sites gets a *different* name at each — correct, and each is right
for its own site. And two genuinely *different* callees reached from sites a
short distance apart can collapse onto **one** name: `resource_38c:0250` has
0x020007aa as both the message import and a two-argument reader,
and `:02f4` has 0x0200089c standing for veneer 0x570 in one arm and 0x560 in the
other. Same arity, one declaration. Different arity, use the `_[a-z]` alias
(`Func_020007aa_b`) that `ADDRESS_SYMBOL` already accepts — **not** a K&R
declaration, which would serve both call shapes at the cost of argument-setter
order at every site (§4).

**Boundary oracle (free).** A dry-run adopt with a two-line stub returns
`adopt=rejected ... size=N/N` when both ends land on instruction boundaries with
no straddling label — independent of your C. It also settles a call's argument
count: on one function a 4-argument spelling measured 17/60 against a 1-argument
spelling's 54/60.

**"Free" means free of *commitment*, not free of *writes*. A dry run mutates the
tree.** Without `--apply`, `overlay_adopt` still copies the draft to
`assets/code/<overlay>_c_<address>.c` — it has to, because §7's routing keys on
the installed path — and it rewrites the overlay `.s` and restores it. Two
consequences, both learned the expensive way on 2026-07-30:

- **Never run two probes against the same overlay concurrently.** This is §10's
  "two lanes must never share an overlay" rule, and it binds *probes*, not just
  adoptions. A 3-way parallel sweep left 13 overlay `.s` files with 661 deleted
  assembly lines and 25 stray installed `.c` files. Nothing warned; `git status`
  was the only tell. Partition a sweep by overlay, or run it serially.
- **A probe run leaves installed C behind even when it fails.** Check
  `git status` after any sweep and clean the residue before banking, or
  `bank_cycle.sh`'s `git add -A` will commit drafts you never adopted.

When cleaning up, resist `rm assets/code/<overlay>_c_0200*.c` — that glob matches
the overlay's whole adopted corpus, not your session's residue. It deleted 89
tracked files in one command here. Use `git status --porcelain` to get the
untracked list and delete exactly that.

**Overlays**

```sh
bun tools/overlay_show.ts <overlay> <offsetHex> -n <bytes>      # disassemble
bun work/claude/overlay_verify.ts <overlay>:<hex4> <draft> <span> [flags]
bun tools/overlay_adopt.ts <overlay>:<hex4> --span N --source <draft> --apply
```

`overlay_verify` prints `size=A/B differing_halfwords=N`; byte-exact is equal
sizes and N=0. ~0.12s per probe, so iterate freely.

**Main image**

```sh
bun tools/candidate_show.ts <path>/<8-hex-stem>.c    # candidate=N reference=M differing_halfwords=K
```

Reference is `asm/<stem>.s`, output is `src/<stem>.c` defining `Func_<stem>`.
`build_claimed` picks up `src/*.c` automatically, and **adoption deletes the
assembly region**: `cp work/claude/main/<stem>.c src/<stem>.c && rm asm/<stem>.s`,
then `bun run build:claimed`. Routing keys on the **stem only**, so a draft
anywhere gets the flags `src/` will get.

**Measuring big functions.** Over ~500 bytes, halfword counts stop meaning
anything: every `bl` displacement is target-absolute, so one positional drift
makes all later `bl` halfwords differ (an 8-of-288-groups draft reported 1,235
differing halfwords). Measure instruction-group equality, splitting at each `bl`.

**Banking.** `tools/bank_cycle.sh "<subject body>"` runs every required check,
picks the `metrics: correct executable denominator` prefix when the denominator
moves, commits and pushes. It runs the three builds concurrently and moves the
staleness check next to the commit, so concurrent walkers do not invalidate a
green run: ~15s warm versus ~190s for a naive sequential `bun run verify`.

---

## 3. Build caching

Content caches under `out/cache` (delete to force a cold rebuild). Measured on a
4-core host:

| stage | cold | warm |
| --- | --- | --- |
| build_claimed (1,376 units) | 15 s | 0 s |
| build_asm (1,807 regions) | 29 s | 0 s |
| build_assets (2,431 entries) | 37 s | 0 s |
| build_full (whole 8 MiB image) | 84 s | 1 s |
| one `overlay_verify` probe | 1.8 s | 0.12 s |

- `build_claimed` keys each object on sha256 of the source bytes plus a stamp of
  the command plan and the compiler binaries it names. Sound because no generated
  source carries a `#include`, so a unit's input closure is one file.
- `build_asm` keys each region on the assembly bytes plus the link address — same
  reasoning, no `.include`/`.incbin` anywhere.
- `build_assets` skips the whole stage against a stamp over every asset source,
  every encoder in `tools/`, the manifest and the build mode. Deliberately
  conservative: any change rebuilds everything.
- `overlay_disasm` caches already-adopted overlay C spans. Without it,
  `assembleOverlay` recompiles every prior adoption in the overlay on *every*
  verification, so the tool got slower as the project progressed.

The byte guarantee does not rest on any of this: `build_full` still re-reads every
region, compares it to the reference ROM, and compares the composed image. A stale
entry cannot produce a wrong image.

**Cache-key trap.** The command plan embeds a fresh mktemp directory, so a naive
key changes every call and never hits. Exclude anything under the work directory
from the stamp.

---

## 4. The lever playbook

Ordered roughly by how often they decide a function.

**Argument-setter order is set by the callee's declared return type.** `s32`
emits `movs r1` before `movs r0`; `void` emits r0 first. `(void)Func(...)` does
**not** work — the `CALL_EXPR`'s own type is unchanged, which is why years of
probes missed this. Where one symbol needs both orders in one function, cast at
the site: `((void (*)())Func_02001234)(0, 50)` — still a direct `bl`, no pool
word, no extra instruction. Mechanism: setters leave RTL expansion in ascending
register order and `sched2` permutes them; a value-returning call frees
`reg_last_sets[r0]`, so the pre-call `movs r0` loses its dependents and `movs r1`
wins, while a void call leaves it collecting dependents, the ranks tie, and the
tie-break falls to original order. **Not universal**: where both spellings give
identical output the call has no live r0 dependency — look elsewhere rather than
grinding. A prototype-less shared declaration blocks it entirely.

**Multi-arity callees use the `_b`/`_c` alias suffix**, not K&R declarations —
and this changes argument-setup order, not just hygiene. A shared prototype-less
declaration suppresses arg0-first ordering at *every* site that uses it.

**Aim the return-type lever at the callee whose setters are out of order, not at
its neighbour.** The setters that come out wrong belong to *one* call; the lever
is that call's own declared return type. On `resource_38c:0124/01e0/0250` the
reference put `movs r1,#0` before `movs r0,#id` on the two-argument call
following a single-argument one, and I changed the *preceding* callee to `s32`,
saw no movement, and parked all three as a flag-resistant class — correctly
measured (the residual survives `-fno-schedule-insns`, `-fno-regmove`,
`-fno-gcse`, `-fno-rerun-cse-after-loop`, `-fno-expensive-optimizations`,
`-fno-peephole`, `-mthumb-immediate-latency`, and locals for either argument)
and wrongly concluded. Declaring the two-argument callee itself `s32` fixed all
three. **A flag sweep cannot find this**, so a clean sweep is not evidence that
the answer is a flag — it is evidence to re-read this section. Before parking on
argument order, name which call owns the bad setters and check *that* callee's
return type.

**The alias rule extends to arity zero, and that case is invisible to the return-
type sweep.** A repeated `bl` to one address where the second site sets *no*
arguments is a void-no-arg alias, not a re-passed constant — the reference is
reusing a live `r0` left by the preceding compare. On `resource_39f:0cd0`,
spelling it `Func_02003a98(0)` cost 2 bytes and 62 halfwords, and neither naming
the result nor passing it back recovered anything; `extern void
Func_02003a98_b(void);` took the function from 156/154 to exactly 0. Tell: the
draft is 2 bytes long and one call site has an argument the reference never sets.

**Constant spelling.** Two inputs decide it; using one alone will mislead you.
What the reference does is the target; the factorisation tells you what gcc does
by default and therefore whether you must intervene. A constant that is `k << n`
with `k <= 255` is buildable in two instructions, so a plain literal yields
`movs #k / lsls #n`; one with no such factorisation pools automatically.

| reference does | factorises `k<<n` | spelling |
| --- | --- | --- |
| pool-loads it | yes | `(s32)&Value_0000XXXX` — force the pool word |
| pool-loads it | no | plain literal — it pools by itself |
| builds `movs`/`lsls` | yes | plain literal — you already get it |

`&Value_` is right in exactly one cell. Applying it because a constant merely
factorises inverts the reference on the third row — a mistake made three times.
When a constant repeats across sites, plain literals also beat `&Value_`: the
symbol form is one rtx that CSE always merges.

**A masked read-modify-write must start its chain with the mask.**
`s32 v = ~12; v &= p->f09; v |= 4; p->f09 = v;` is exact, while the more natural
`s32 v = p->f09 & ~12;` gives the identical instruction stream with two registers
swapped. Splitting the RMW into two statements is necessary but not sufficient —
*which operand opens the chain* decides the register identities.

**The mask-first RMW rule covers OR too — and does *not* generalise past stores.**
`{ s32 w = 1; w |= *p; *p = w; }` gives the reference's `ldrb r2 / movs r3,#1 /
orrs`, while `*p |= 1;` swaps the two registers; that was the last 2 halfwords on
`resource_3c8:1150`, previously filed as "no source lever found". The limit: where
the masked value feeds an *add* rather than being stored back, inline is correct
and splitting is worse (`resource_39f:02a8`, 12 → 19). The lever is about the
store, not the mask.

**Arm order decides branch sense, and it is worth ~15 halfwords.**
`if (x <= K) {A} else {B}` emits `cmp / bgt .else / A`, laying A out first; the
natural `if (x > K) {B} else {A}` lays B first and inverts the condition. Match
the reference's *layout* order, not the reading order you would choose.

**Pad structs explicitly so fields land at their real offsets.** A naive
`u8 pad[0x48]; s32 f48; void *f50;` puts the pointer at 0x4c and silently shifts
every subsequent offset; the resulting diff looks like a scheduling problem.

**Function-top locals for word constants can subsume routed flags — try them
before routing a flag.** The shifted-constant rule above is the narrow case; the
general lever is to hoist *every* non-trivial word call argument into its own
function-top `s32` local, including constants that pool rather than factorise.
Measured on `resource_38f:2608`: 44 halfwords → 15 under
`-fno-cse-two-insn-immediate` → 9 adding `-fsched-low-dest-first` → 2 after a
struct retype → **0 at baseline with all flags removed**, once `0x620000`,
`0x690000` and the pool-only `0x010d0000` each became a function-top local. Two
rules attach: block-scoping the same constants at the call site stayed at 2 (the
function-top/block-scope split is real and load-bearing), and a constant used at
several sites wants **one local per site** — a single shared local re-creates the
`push {r7}` hoist. This is the cheapest thing to try on a function you are about
to route, and it leaves the tree flag-free.

  *But it inverts on call sheets with repeated shifted constants.* A second lane
  measured the opposite sign on `resource_39f:1818` (132 halfwords → 104 worse)
  and `:2004` (148 → 116 worse): there, plain literals composed with
  `-fno-cse-two-insn-immediate` is right and hoisting is actively harmful. The
  two results agree on the mechanism — hoisting wins when each site gets its own
  local, and loses when one constant is shared across many call sites, which is
  precisely the call-sheet shape. **Probe both directions; do not assume the
  sign.** It costs two probes at 0.12 s each.

**Give a sub-object reached through a pointer field its own struct type.** With
`u8 *q = p->f50`, gcc cannot prove `q[0x26]` independent of `p->f23` and keeps
program order, where the reference hoists the `ldrb` above the `strb`. Retyping
`q` as `struct Sub *` closed 7 of 9 halfwords in a single edit. Same family as the
`union Slot` alias lever, but the fix *adds* type distinction rather than blocking
a hoist. Tell: a store/load pair in program order that the reference has swapped,
with both objects reached through a `u8 *` field.

**`&Value_` also applies to loop invariants, not only call arguments.** A pooled
constant used inside a loop is rematerialised per iteration from a plain literal;
declaring `extern u8 Value_fffff800; s32 d = (s32)&Value_fffff800;` in the block
*enclosing the loop* hoists it into a callee-saved register as the reference does.
Function-top placement instead costs 4 bytes.

**Narrowing decides literal-pool *placement*, not just load width.** A pool-split
residual is a source-shape problem, not a CSE problem — neither CSE mode moves a
pool. On `resource_3b8:0108`, `s16 t = (u16field + K) & ~0x3fff;` emits the
reference's `ldr` **and** its mid-function pool with a `b` jumping over it, while
hoisting the same arithmetic through an `s32` local emits an identical `ldr` but
floats the whole pool to the function end, losing 8 bytes and the duplicate pool
word. Attack pool placement by re-narrowing the expression.

**`(s16)x == K` is spelled `((s32)x << 16) == (K << 16)`.** A direct comparison
on an `s16` local compiles to a plain `cmp` and comes out 4 bytes short of the
reference's `lsls #16 / movs #0x80 / lsls #23 / cmp`.

**To pin a constant into a callee-saved register, reuse a live local rather than
declaring a fresh one.** Assigning the constant to the local whose value the
preceding compare just consumed went 117 → 32 halfwords; an equivalent fresh
`s32 m = 0x4000;` is rematerialised at the call site. This is the mirror image of
the "one local holding two call results misallocates" rule — for *call results*
sharing a local hurts, for *constants* sharing is what matches.

**Size short by 2-4 bytes, and the reference re-reads one address, means
`volatile`.** The mechanical tell is the *short size*, not the shape of the diff:
we fold repeated loads the reference keeps. This closed two functions outright
from 18 and 25 halfwords. Two placement rules follow from it: a block-scoped
`volatile` pointer local declared **before** the sibling arithmetic forces two
pool-address pseudos to overlap and so take distinct hard registers (declaring it
after loses the effect, and this fixed a register-identity floor that survived all
seven modes); and a second pointer the reference keeps in a different register
needs its own named local.

  *It applies to a `Data_` global just as much as to a hardware register, and
  there the size is not short.* `resource_371:3f88` and `resource_377:15bc` both
  read `Data_03001e40` twice — once to test bit 0, once to shift — and the
  reference keeps the address in a register and issues **two** `ldr`s where we CSE
  to one. Sizes match exactly, so the short-size tell never fires; the diff just
  looks like register scramble (29 differing bytes on both). Declaring the extern
  `volatile s32` closed both outright, first probe. **Whenever the reference loads
  the same address twice with no store between, try `volatile` before reading the
  diff as an allocation problem** — a plain global is the easy case to miss
  because nothing about the size says so.

**Absolute addresses want `Data_<8hex>` symbols, not integer literals — otherwise
gcc derives one from another.** Writing several nearby RAM addresses as casts
(`*(s32 *)0x02002080 = 0; *(u16 *)0x02002008 = 0;`) lets CSE keep one in a
register and reach the next with `subs r3, #120`, where the reference pool-loads
each separately. `externalSymbol` resolves any `Data_<address>` / `Func_<address>`
/ `Value_<address>` name (`tools/alchemy_gcc.ts`), so `extern s32 Data_02002080;`
gives a SYMBOL_REF gcc cannot fold arithmetically and each address gets its own
pool word. This closed the whole derived-address class on `0800651c` in one edit.

**Stack arguments want function-top locals *only* when nothing precedes them.**
§4's hoisting lever puts the two stacked words of a six-argument call in distinct
registers, which is what the reference does — but if a *call* comes first in the
function, function-top locals stay live across it and land in r5/r6, buying a
`push` the reference does not have. Declare them in a block after that call
instead: `resource_3bb:02c0` went from a span overrun to exact on that change
alone, and `resource_3bc:024c` from 19 differing bytes to exact.

**The standard fix for a renderer/setup call sheet is `void` callees plus
`-fsched-low-dest-first`, in that order.** This landed five times on 2026-07-30
(`38d:1958`, `38d:1984`, `3b5:0260`, `3b4:11d8`, `3bb:02c0`) and the diagnosis is
always the same two-step. First, an `r0` setter sitting *after* the r1/r2/r3
group means the callee is `void`-returning, not `s32` — flip the declaration and
the residual usually halves. Then, if `movs r0` still sits after the `lsls` of a
shifted argument rather than between the `movs` and its shift, that is the
scheduler tie-break and the flag closes it. Neither step alone is enough on a
sheet that needs both: `3b5:0260` went 12 → 6 → 0 across the two. Try the
declaration before reaching for any flag — it costs one probe and needs no
routing entry.

**A repeated shifted constant across call sites needs
`-fno-cse-two-insn-immediate`, and the tell is a prologue.** On
`resource_38d:1984` the same `0xC000` feeds two of three call sites; CSE hoists it
into r5 and the function grows `push {r5, lr}` / `pop {r5}`, while the reference
rematerialises `movs r1,#192 / lsls r1,#8` at each site. No source spelling
reaches it — one local per site does not stop CSE — but the routed mode does.
Pair it with `-fsched-low-dest-first` for the r0-setter position. Its sibling
`:1958` has no repeat and needs only the scheduler mode. **Route the CSE mode by
*path*** (`NO_CSE_TWO_INSN_IMMEDIATE_OVERLAY_SOURCES`), which is overlay-specific
and so immune to §7's address-collision trap; `SCHED_LOW_DEST_FIRST_SOURCES` is
keyed by bare address and is not.

**Re-check every `&Value_` in an old park note against the factorisation table
above.** Four functions in one overlay were parked at floor 2 as "no flag reaches
it" when the actual blocker was a wrong `&Value_` spelling — the scheduler was
never involved.

**Statement order matters.** Moving an independent assignment above a call has
produced exact matches where nothing else did — a permuter's single win in 65,543
candidates was one statement swap. Always try both orders of two independent
statements. Related: assign a pool-address local immediately before first use,
not at function top (33 halfwords on one function).

**Stack arguments want block-scoped locals opened at the call site**:
`{ s32 k5 = 4, k6 = 3; Func(0, 59, 15, 38, k5, k6); }` gives the reference's
`movs / movs / str / str`. Function-top locals do *not* work — the scope must open
at the call. This disproved a "compiler blocker" that had parked five large sheets.

**Big word-store init sheets are the cheapest bytes in the project.** Four
functions of 232-256 bytes each (49 word stores) came out exact on the first try
from flat `s32 *p = Data_...; p[i] = literal;`. Heavy `r8`-`fp` traffic and
`adds r3,#4` chains do not mean difficulty. Look for this shape first.

**For a constant-argument call sheet, declare one function-top `s32` local per
non-trivial constant argument** (exemplar `assets/code/resource_372_c_02002180.c`,
~100 such locals). Plain literals do not reproduce the `movs/movs/lsls` interleave.

**Read the shape off the reference:**
- The epilogue states the return type: `pop {r5} / pop {r0} / bx r0` means `void`
  (r0 dead); `pop {r1} / bx r1` means r0 is live.
- `negs rN,#K` is the mask `~(K-1)`, not `~K`.
- A `movs` chain through one scratch register is a pre-scheduling order
  fingerprint: anti-dependences serialise those materialisations, so their listing
  order *is* their ordinal order and cannot be rescheduled.
- Some `bl` targets resolve inside literal pools — those are out-of-overlay
  callees with link-patched displacements. Name them `Func_020xxxxx`; not data.

**Types and structures:**
- Give an object a `struct` with named fields rather than `u8 *` plus casts when
  a halfword store must reorder past a word load.
- A narrow bitfield (`u16 f06:9;`) beats a hand-written mask/or read-modify-write;
  int-typed mask locals give the identical instruction stream but wrong register
  identities.
- Descriptor locals must be structs, not `s32 buf[N]`, when any field is a
  halfword — otherwise gcc materialises a second frame base.
- A `union Slot { s32 w; u16 h[2]; void *p; }` blocks an alias-based load hoist
  that a same-type cast does not. Tell: `-fno-strict-aliasing` fixes it.
- `switch` on an unsigned global dispatches with `bhi`, not `bgt`.
- Unsigned induction variables wherever a back-edge is `bls`/`bhi`.
- A `u8 f:2` bitfield struct for small masked field stores.

**Arithmetic and control flow:**
- `x / 0x100000` and signed `/ 65536` rather than hand-expanded negative-bias
  shifts; in-place s16 clamp `t <<= 16; t >>= 16;`; `t = (s8)t` for sign
  extension; `(x << 2) + (x << 1)` for `x * 6`.
- `value >= 0xa001 && value <= 0xdfff` reproduces the
  `adds r5,#-0xa001 / cmp #0x3ffe / bhi` range-check idiom.
- Duplicate a call in both arms of an if/else rather than using a ternary:
  cross-jumping then emits the reference's `cmp/beq/movs/b/movs/bl` shape.
- `if (x == 0) { …; return 1; } return 0;` branches to the shared tail where
  `if (x != 0) return 0;` inlines the constant.
- Split a masked byte read-modify-write into two statements to keep the mask
  32-bit; give a destructive shift its own compound-assignment statement.
- Split a shared addend out of sibling scaled expressions: `Func(a, x*16+8,
  y*16+8)` emits `movs r3,#8` twice, while hoisting the multiplies shares one.
- A `pop {r1}/bx` tail is `return Func_xxx(...);` with an `s32` return type.
- `p[108 + i]` (offset inside the index) reproduces a `biv init = base+K` pairing
  that `p += 108` before the loop does not.
- `extern u8 Base[]` plus a function-top `s32 off = 500;` where gcc would
  otherwise fold `Base + 500` into the pool.

**Allocation:**
- **Store order moves register allocation.** Allocno priority is
  `2*refs/live_length`, and a parameter's store position sets its live length.
  Permuting independent store groups is the general fix when two equal-priority
  parameter pseudos land in the wrong registers — 24 orderings found two that
  give zero where the obvious one gave 15.
- A narrow-mask local (`s32 mask = -0xD;`) must sit *inside* the loop,
  immediately before use, or it becomes a loop-invariant allocno and buys an extra
  callee-save.
- `fp` is not usable by Thumb's hi-register `add`, so a constant allocated there
  gets rematerialised; declaring it inside the loop body makes it a loop hoist.
- Named `u32` locals for long-lived masks push them into `r8/lr/ip` (allocated
  last), leaving low registers for products.
- A seemingly dead `= 0` initialiser on a pointer that later receives a call
  result can be load-bearing (84 → 49 halfwords on one function).
- Entry-hoisted int-typed local (`g = 0x986;` before the first branch) makes gcse
  rematerialise a pool constant per block — fails if a use sits in the entry block.
- Two-address `muls` avoids a copy only as `x = a & mask; x *= scale;`.
- gcc 2.96 does not share stack slots between disjoint scopes.
- `volatile` reproduces a repeated non-CSEd load of one address — **only** where
  the reference genuinely repeats the load. Single-read siblings stay plain
  `extern`. Never a general matching device.
- **Compound assignment names the shift's destination register.** `v <<= 16;`
  emits `lsl rV, rV, #16` — writing the result over its own input, because the
  source says the input is dead. `x = v << 16;` allocates a fresh register even
  when `v` is dead immediately after, and no flag reverses that. Reach for it
  whenever the diff is only *which* register an ALU result lands in and the
  reference overwrites the operand. On `resource_373:02a8` it closed 4 of 16
  groups on its own, and the same shape recurs wherever a packed word is split
  into a masked half and a shifted half.
- **Narrow the type at the store, not at the arithmetic.** When the reference
  builds a small constant out of a value already in a register -- a mask from a
  zero it just stored, `movs r3,#0` then `subs r3,#13` for 0xf3 -- the C has to
  give gcc a 32-bit expression to fold through. Declaring the intermediate `u8`
  loses it: `clear -= 13` on a `u8` folds to 243 and gcc materialises that
  directly. Make the local `s32` and cast at the byte store. Same family as the
  pooled-zero and halfword-store levers below, and the same rule states all
  three: narrowing early is what loses the reference's form.
- **The first `return` names the value materialised before the compare.** A
  two-arm predicate compiles as `mov r0, A; cmp; b<COND> end; mov r0, B; end:`
  where `A` is the *first* return's value and `COND` is its condition. So an
  inverted branch sense is fixed by swapping which arm is written first, not by
  negating the condition: `if (x > c) return 0; return 1;` and
  `if (x <= c) return 1; return 0;` differ in emitted code, and only the second
  matches a reference that sets 1 before the compare. `!`, a ternary, and a
  result local each cost an extra instruction instead (13-14 groups vs 5 on
  `resource_3a3:0338`).

---

## 5. Sweep levers mechanically

`tools/return_type_sweep.sh <draft.c> <overlay:offset> <span> [flags...]` greedily
flips callee return types **in both directions** — `void`→`s32` and `s32`→`void` —
keeps improvements, and repeats to a fixpoint. Both directions matter: the
`s32`→`void` half closed two functions the other half could not touch, and for a
while only one direction was tooled, so half the search space was invisible.
At ~0.12s per probe a 60-callee function converges in seconds. It found a 404-byte
function needing a specific 5-of-10 partition of identical-looking call sites that
no hand sweep would have located. One lane closed 8 functions / 1,628 bytes from
41 candidates this way.

**Run `tools/finish_draft.sh <draft.c>` before reasoning about any residual.** It
composes the entire enumerable search in one command and takes a few seconds at
16-way concurrency: ~50 single flag settings (both CSE modes alone and paired per
this section's protocol, the scheduler modes, the fork modes from `FORK_MODES`),
then **137 pairs and descriptor-family triples over a curated interacting-mode
pool**, then **the other approved compiler families** (`old-agbcc`, `gcc2951`,
`pret-early-thumb`, each with and without `-O1`), then the return-type sweep at
baseline and at the winning flag set, then the statement-order sweep. It prints
the best result, the exact options to reproduce it, and the surviving residual.

Two of its stages exist because single-flag probing provably misses things.
Pairing is required because the winning combination is often built from modes that
are individually neutral or worse (`08091174` reaches 3 on
`-mgrouped-dma-store,-fno-cse-pool-immediate` with neither in its top six alone).
Family probing is required because 2 of the 20 conversions banked on 2026-07-30
needed `--family old-agbcc`, which no `--flags` value can express: on `08006878`
the whole gcc 2.96 lane floors at 72 while `old-agbcc -O1` is **0**. A family win
is reported as `BEST IS A FAMILY`, and adopting it means routing the stem through
`AGBCC_SOURCES` rather than a flag set. It writes only to its `--out`
directory and never builds, commits or touches `src/`.

Validated on known cases: it recovers `08090824`'s `-mgrouped-dma-store` win
(9 → 6) and reproduces `080c1fa8`'s floor of 2 with the correct residual. It also
propagates the statement-order probe-count warning, so a null that never exercised
the lever says so instead of reading as a park. The point is division of labour: by
the time you look at a function by hand, everything enumerable is already done, so
the only question left is which §4/§5 lever the residual implies.

**The statement-order lever is tooled too, and its probe count is part of its
result.** `tools/statement_order_sweep_main.ts <draft.c> [--flags …]` permutes
maximal runs of *independent* top-level statements to a fixpoint — the §4 rule
that a parameter's store position sets its live length and therefore its allocno
priority. It was swept over 53 size-exact main-image targets for 10,982 probes:
no zeros, five drafts improved (`080c1fa8` to 5 halfwords, `080a6a98` to 11).
The binding limit is the independence test, not the search: two statements that
both touch memory or call anything are held in order, because source alone cannot
prove they do not alias, and on pointer-heavy drafts that leaves nothing to
permute (`08077394` produced 1 probe, `080ae99c` 3). Where a run *is* independent
the sweep is exhaustive — `08011fd8` took all 721 orderings and proved a real
floor. **So read the probe count before recording a park:** 1-3 probes means the
lever was never actually exercised, not that it failed.

**The levers compose, and the sweep is worth re-running after each flag.** The
working order is `-fno-cse-two-insn-immediate` → sweep → `-fsched-low-dest-first`
→ **sweep again**: the second sweep closed the last 3 halfwords on one function,
because adding a scheduler flag changes *which* return types help. On two other
functions the return type fixed the `movs`/`movs` swaps and
`-mthumb-immediate-latency` then fixed the `movs`/`lsls` ones, neither reaching
zero alone.

**Probe both CSE modes together AND each alone — the full protocol, never a
shortcut.** Measured over 20 re-probed parks, roughly three quarters were
mis-measured. Four were *both-modes-only* wins where each mode alone is neutral or
actively worse: one went 400 → 419 and 411 with each single mode → **18** paired.
A single-mode probe there would have been honestly recorded as a regression and
wrongly inherited as a negative. But the reverse also occurs — on one function the
pool mode alone is harmful while the two-insn mode helps — so the pair is not a
substitute for testing each. A note recording only one CSE mode is unmeasured.

**Size-exactness, not the halfword count, is the signal that the CSE seam is
cleared.** On five of these functions the paired modes brought the emitted size to
exactly the reference span while the differing-halfword count was still in the
tens or hundreds. A size-exact residual is a draft or allocation problem that the
non-flag levers finish; it is not a compiler problem. Judge progress by size
first.

**Sign the range-check operand the way the reference's pool word reads.**
`(u16)(h - 0x3001)` emits the `0xffffcfff` pool word while `(u16)(h + 0xcfff)`
emits `0x0000cfff`. The difference appears *inside the literal pool*, which is
easy to misread as a span error.

**Cross-jump parameterisation.** Where the reference merges two identical blocks
that differ only in one constant — pre-loading `movs r1,#K` before branching into
a shared tail — gcc will not find that merge from two spelled-out blocks. Write it
explicitly: `lim = 194; goto common;` / `lim = 241; goto common;` then
`common: if (y > (lim << 16))`. That took one function from 236 bytes to exactly
224, and it is the size-fixing lever on that shape.

**A `u16` call result needs a `u16` local, not a `(u16)` cast of an `s32` local.**
The cast form folds the `lsls #16 / lsrs #16` zero-extend away and comes out
4 bytes short.

**Two masked byte read-modify-writes need two separate mask locals.** Reusing one
cost a function its register identities *and* sank the second `strb` past four
stores — the same family as one local holding two call results, extended to masks.

**Flat `y > A && y <= B` on two constants** is converted by gcc into the unsigned
`adds`/`cmp`/`bhi` range-check idiom. Nesting the ifs is required to keep two
separate signed compares.

**Decrementing a halfword in place pools `0xffff`.** `*(u16 *)h = *(u16 *)h - 1;`
emits a pooled `0xffff` and an `adds`, where
`{ s32 t = *(u16 *)h; t -= 1; *(u16 *)h = t; }` gives the reference's
`ldrh / subs #1 / strh`. Worth 6 bytes and the whole tail alignment on one
function.

**A struct cursor is actively harmful where an index is right.** Spelling
`struct Ent *e = &Table[k];` and then `e->field` took one function from exact size
and 2 halfwords to 156/160 and 56. Index every field as `Table[k].field` unless
the reference clearly holds a cursor.

**One local holding two independent call results misallocates.** A function sat
at 21 halfwords — a pure r5/r6/r7 identity swap — solely because one `s32` held
the results of two unrelated calls. Giving the second call its own named local
took it to 0; declaration-order permutations did nothing. Same family: **do not
decrement in place when the pre-decrement value is needed again later.**
`x -= 1; f(x); g(x)` sat at 11 where `s32 m = x - 1; f(m); g(x - 1)` reached 0.
In-place mutation lengthens the allocno's live range and flips the priority tie.

**For shifted constant arguments, function-top locals beat the tie-break flag —
and block-scoped locals fail.** Hoisting two `0x…00000` arguments into
function-top `s32` locals reached 0 at baseline where `-fsched-low-dest-first`
was needed otherwise, and closed four functions without routing. Note this is the
**opposite** of the stack-argument rule, where the scope must open at the call
site: register arguments want function-top, stack arguments want block-scoped.

**A `ldr K1 / ldr K2 / subs` pair is a runtime difference of two `&Value_`
symbols.** Plain literals constant-fold, so the reference's two pool words are
only reproducible as `(s32)&Value_000008c8 - (s32)&Value_0000007e`.

**`ldrsh` off a stack descriptor needs a named `s16 *` alias local**:
`((s16 *)&v)[1]` materialises a second frame base, while
`s16 *h = (s16 *)&v; h[1]` is exact. Thumb `LDRSH` has only the register-offset
form, so `movs r3,#10 / ldrsh r2,[r6,r3]` is **not** a `volatile` tell.

**When two live names hold one call result, a copy *round trip* decides which one
a compare reads — and it needs `-fno-rerun-cse-after-loop` to survive.** A
quotient copied `ratio = value;` leaves both names tracing to the call's return
register, so the following compare reads r0 where the reference reads the copy in
r1. Writing the round trip `ratio = value; value = ratio;` makes `value` the copy
instead and the compare takes `ratio`. The second assignment is dead, and it is
load-bearing — the same family as §4's "seemingly dead `= 0` initialiser". Both
halves are required: the round trip alone leaves the halfword, because the rerun
of CSE after loop optimisation collapses the pair and restores the return
register, and the flag alone has nothing to preserve. This closed `080044d0`
(212 bytes) from a floor of 1 that eleven other source spellings and the whole
§7 mode matrix could not move. **Tell:** a single differing halfword on a
size-exact draft where our operand register is the callee's return register and
the reference's is a copy that both sides already emit. Do not read that as §6's
"register-identity-only swap" park class without trying this first.

**The allocno-priority lever applies to constant locals, not just parameters.**
Swapping *declaration* order is neutral; declaring `s32 o;` uninitialised and
assigning `o = 1;` **between its first two uses** changes the live length and
flips the register allocation. That closed a 10-halfword r5/r6 identity swap.

**Guard-clause shape decides where early-exit blocks land.** Flat
`if (bad) return;` guards put those blocks inline; the reference wants them at the
tail. Rewriting as *nested* ifs with the main body innermost was worth 8 bytes on
one function and 143 halfwords on another (it stopped a `movs r0,#0` being
hoisted). Related: `Base + K` folds into the pool, so a function-top base local is
what restores the reference's `movs`/`lsls`/`adds` offset rematerialisation.

**"Not drafted (scan rule)" is the largest remaining seam of stale parks.** Ten
rows in one overlay were triaged on sight as duplicated two-instruction immediates
or duplicated pool loads and never drafted — but that triage predates the two
routed modes that suppress exactly those. Six of the seven then drafted went to 0.
Any note whose reason is a park *class* rather than a measured floor should be
re-read the same way: the class may now be routable.

**A SYMBOL_REF spelling pins *where* a pool load is emitted, not only whether one
is.** §4's factorisation table correctly puts a non-factorising constant on the
"plain literal, it pools by itself" row — but gcc then hoists that `ldr` above
intervening calls. Block-scoping the local does *not* fix it; the
`extern u8 Value_0000240d; s32 t = (s32)&Value_0000240d;` spelling does. That was
the last 11 halfwords on one function.

Minor but mechanical: storing a small constant through an `s16 *` pools it as a
halfword literal (`ldrh r3,.L7`), so the `s16 *q; s32 v; v = 94; *q = v;` idiom is
required rather than stylistic.

**A sweep null was untrustworthy until 2026-07-30.** The script originally matched
only the literal spellings `void` and `s32`, so a callee declared `u32`, `u16`,
`s16` or pointer-returning was invisible to it and the sweep returned a *false
null* on exactly the function the lever would have closed. One function sat parked
at floor 2 for that reason; changing a `u32` return to `void` reaches 0 at
baseline flags. The script now matches any declared return type — but treat any
sweep null recorded in a note **before that date** as unmeasured.

**"movs/lsls interleave" is not a real park class.** A function parked at floor 2
after trying `-mthumb-immediate-latency`, `-fno-sched-depend-count`,
`-mgrouped-dma-store` and the CSE modes went straight to 0 under
`-fsched-low-dest-first`, which had simply never been tried — it post-dates those
notes. Re-probe every note whose residual is a `movs`/`lsls` interleave. This is
the same stale-evidence pattern as the return-type lever, one flag later.

**Size smaller than the reference by 4-8 bytes, with a `push {r6,r7}`/`mov r8`
prologue you do not want, means `-fno-cse-two-insn-immediate` first.** Shifted
constants such as `0x5000`/`0x3000` reused across call sites get hoisted into
callee-saved registers; on one function that flag was the gate that let the other
two levers land.

---

## 6. Park classes

**Real — recognise and skip in seconds:**
- **A pool `ldr` scheduled ahead of the setters the reference emits first.** Two
  independent rows in `resource_373` now show it, so it is a class rather than a
  one-off: `:11d8` (108 bytes, 2 clusters, `ldr r5, POOL` hoisted above a whole
  argument block and above the preceding `bl`) and `:10d8` (256 bytes, 4 groups,
  `ldr r2, POOL` hoisted above the `ldr r3, [r5, #8]` it is added to, plus one
  `mov r3, sl` / `mov r2, r8` pair). `:10d8` reaches those 4 under
  `-fsched-low-dest-first -fno-strength-reduce` with both loop tests respelled
  `step != 40`; that combination is worth keeping, since it took the row from 43
  groups to 4 and only this class is left. Nothing moves the residual: not
  `-fno-cse-pool-immediate`, `-mthumb-load-latency-one`,
  `-mthumb-early-literal-pool`, `-mthumb-entry-literal-first`,
  `-fthumb-minipool-tail-first`, `-fthumb-literal-before-index-shift`,
  `-fno-sched-alias`, `-fsched-store-first`, `-fsched-high-dest-first`,
  `-fno-thumb-contiguous-immediate`, `-fno-cse-follow-jumps`, `-fno-regmove`,
  `-fno-expensive-optimizations`, `-fno-schedule-insns`, nor any callee declared
  non-void. `-fno-schedule-insns2` removes the hoist and costs several argument
  orders instead, which is the tell that the reference had sched2 on with a
  load-ordering rule this fork does not carry. **Do not re-sweep it** — measure
  the group count, name this class, and move on.
  **It runs in both directions**, which is why no single mode fixes it:
  `resource_3c8:06a0` (180 bytes) is *one* swap from exact with the reference
  emitting `ldr r6, POOL` **before** `lsl r3, r4, #4` where this fork emits it
  after — the mirror of the two `373` rows. `-fthumb-literal-before-index-shift`
  and `-mthumb-entry-literal-first` are the modes that name exactly this shape
  and neither moves it. Four rows, two directions, one missing rule: the fork
  places a pool load by its own latency model rather than the reference's.
- ~~The same **two-instruction immediate** built at two or more call sites.~~
  **This class is now disproved — see below.** It is listed here only because
  hundreds of old notes still cite it.
- **DMA descriptor `stmia` groups** from struct members. `-mgrouped-dma-store`
  does *not* group struct-member stores. Correcting `LAWS.md`: with a `volatile`
  descriptor plus the flag, every group *but the last* forms — the peephole does
  fire on volatile MEMs, and what defeats the final group is that its three values
  never land in three consecutive ascending registers.
- **`pop {pc}` epilogues** — that region was built without `-mthumb-interwork`.
- In-line **`0x0200_8xxx` jump tables** (per-overlay runtime base differs from our
  link base), **`mov ip,pc / bx rN` IWRAM calls**, **hand-assembly blobs** (a `bl`
  landing mid-instruction), **shared epilogue fragments**, **veneer thunks**, and
  **mid-function literal-pool dumps** at barriers we cannot reproduce.
- **Register-identity-only swaps**, often where our allocation is strictly better
  or one instruction shorter.
- **Branching leaf with a bare `bx lr`.** The fork returns from any leaf that
  contains a conditional branch with `push {lr}` / `pop {r0}` / `bx r0`; it emits
  the bare `bx lr` only for straight-line leaves. The reference does both. It is
  not register pressure and not the return type -- a leaf using r0-r4 returns
  bare if it is straight-line, and a leaf using only r0-r3 pushes if it branches
  (measured on both, 2026-07-31). Costs four bytes and blocks the row outright.
  Only 13 unconverted main-image owners have the shape, which is why this is a
  park rather than a compiler change: modifying the Thumb epilogue would put
  every byte-exact source in the tree up for re-verification to win ~13 rows.
- **Two loop pseudos allocated in swapped registers.** A loop that carries both a
  walking pointer and a counter: the reference puts the pointer in the register
  the *preceding* instruction just freed and the counter in the next one, and gcc
  gives the reverse. It is not a scheduling class even though it looks like one —
  on `resource_373:02a8` the reference order is only stable because its allocation
  creates an anti-dependency (`mov r2,#0; ldrsh r0,[r3,r2]; ldr r2,POOL` — sched2
  cannot hoist a load into a register the `ldrsh` still reads), so fixing the
  allocation fixes the order for free and nothing fixes the order alone.
  Measured null on `:02a8`: **all 720 permutations of local declaration order**,
  four loop spellings (indexed `for`, indexed `do`/`while`, explicit pointer both
  init orders, pointer-bound `for`), lengthening and shortening each live range,
  and all 39 fork modes plus the stock CSE and scheduler flags — every one lands
  on the same floor. `-fno-schedule-insns2` demonstrates the mechanism but costs
  more elsewhere than it fixes. Blocks the 384-byte 11-overlay twin and
  `resource_373:02a8` (8 overlays); ~5,300 bytes sit behind one answer, so this
  is worth re-probing whenever a new allocation-side mode lands.
- **Per-site contradictory argument orders** where a prototype-less shared
  declaration is required — the per-site cast is the only escape.

**Residual fingerprints that are *not* what they look like:**
- **Pool halfwords transposed within each word** (`ours=0000 ref=034b` alternating
  with `ours=034b ref=0000`) is a **mis-read constant**, not a span or emission
  problem: the draft had `0x34b0000` where the pool word is `0x0000034b`. Read
  constants off `overlay_show`'s pool listing, never off the `lsls` context.
- **A diff inside the literal pool** can be the *sign* of a range-check operand:
  `(u16)(h - 0x3001)` pools `0xffffcfff`, `(u16)(h + 0xcfff)` pools `0x0000cfff`.
  Easily misread as a span error.

**Disproved — do not trust these in old notes:**
- **"Repeated same-bb two-instruction immediate; cse1/cse2 merge it; needs an
  unrouted const-remat mode."** The paired CSE modes *are* that mode, and the
  notes predate them. The tell is that the pair only works together — either
  alone is worse than baseline. Measured: `resource_372:0f38` 84 halfwords
  baseline → 172 with `-fno-cse-two-insn-immediate` alone (and 4 bytes long) →
  152 with `-fno-cse-pool-immediate` alone (4 bytes short) → **54 paired**, and
  the function then closed. `:1154` 140 → 199 / 170 → **36 paired**. Every note
  reading "PARKED ON SIGHT (same-bb repeated 2-insn const)" is stale evidence and
  should be re-probed with both modes on. This is the single largest known pool
  of recoverable parks — treat it the way the return-type lever's backlog was
  treated in §5.
- "Stack-argument allocation blocker" — a source shape (§4), and it had parked
  five large sheets.
- "Not producible from C / needs linker work" on resource_379 — refuted with the
  boundary oracle; the region is adoptable today.
- Two-halfword argument-setter swaps described as "no flag reaches it" — the
  return-type lever post-dates them (§4, §5).
- `-mgrouped-dma-store` by analogy: two regions resembling a routed twelve-store
  block were made *worse* by it (21→24, 25→26). Measure before routing.

---

## 7. Compiler modes

Seven admitted modes, all default-off and routed per source in
`tools/alchemy_gcc.ts`. Mechanism, prototype diffs and collateral lists are in
`docs/compiler-evidence/`.

| flag | what it suppresses | collateral |
| --- | --- | --- |
| `-fno-cse-two-insn-immediate` | sharing a repeated two-instruction immediate | 109/1,335 |
| `-fno-cse-pool-immediate` | sharing a repeated literal-pool constant | 110/2,202 |
| `-fsched-low-dest-first` | ordinal tie-break on r0-r3 argument setters, **and a pool-load hoist** | 139 |
| `-fsched-high-dest-first` | the same on r4-r12, ties with no call in them | 125 |
| `-fno-sched-alias` | a store/load pair proved independent and reordered | 82 |
| `-fsched-store-first` | a store sinking behind arithmetic | 308 |
| `-fno-sched-depend-count` | a store/load swap `-fsched-store-first` does not reach | — |
| `-fno-gcse-insert-load` | a PRE-inserted load the reference lacks | 9 |
| `-fthumb-group-value2-in-place` | the copy forced by `thumb_store_multiple3`'s hard-coded `(reg:SI 2)` when a grouped descriptor's third word is a constant | 1 routed |

**A finished fork mode can sit unrouted and therefore unsweepable — check the
binary, not just this table.** `-fthumb-group-value2-in-place` was implemented in
`alchemy-gcc` during the grouped-descriptor work, is documented in the fork's own
`flags.h`, and exists in the **pinned** `cc1` — but it was in no routing set here
*and* absent from `FORK_MODES` in `tools/mode_sweep.ts`, so neither the router nor
the explorer could ever name it. It was found by extracting option strings out of
the `cc1` binary and diffing them against `tools/`. Sweeping it over 300 clean
drafts moved exactly one function, and moved it a long way: `080b5ad4` went from
28 halfwords at the wrong size to **0 at 64 bytes**, once its tail was also
spelled as a returned call. No re-pin was needed because the binary already
carried it; admission was the routed set plus the `FORK_MODES` entry.

Worth knowing why it was invisible: the routing comment above
`GROUPED_DMA_STORE_SOURCES` had described `080b5ad4`'s blocker precisely — "value0
has a special case in that pass and value2 has none" — and the fork had since
written that special case. The note was correct when made and stale when read,
which is §6's pattern applied to the compiler lane rather than to a park note.
**When a routing comment names a missing compiler capability, check whether the
fork has since grown it.** Extract the option strings from `cc1` and diff:

```sh
strings toolchain/alchemy-gcc/cc1 | grep -oE '^(thumb-[a-z0-9-]+|grouped-dma-store)$' | sort -u
```

Everything else that scan turned up is already either routed or in `FORK_MODES`;
`-fthumb-group-value2-in-place` was the only dark one, so this seam is now closed,
but re-run the diff after any fork change.

**`-fsched-low-dest-first` reaches three residuals, not just the one in the table**:
the r0-r3 `movs` ordinal tie-break; a **pool-load hoist**, an `ldr r2` scheduled
ahead of the `movs r0`/`movs r1` of an argument group; and a
**`movs rN,#K / negs rN,rN` versus `movs r1,#imm` order swap** that appeared
identically on two functions where three source spellings each reached nothing. Neither the `&Value_` spelling nor function-top or
block-scoped locals touch that, so a six-function family would otherwise have been
triaged as a SYMBOL_REF-placement park. Try the flag before believing that park.

**Cautions.** Never combine `-fsched-low-dest-first` with
`-mthumb-immediate-latency` — the latter subsumes and then breaks the same
targets. Route per *function*, never per overlay: in resource_37b
`-fno-cse-pool-immediate` is free on 21 functions while `-fsched-low-dest-first`
regresses three large sheets.

**Adding a mode.** Commit the fork change to `alchemy-gcc`, rebuild
(`./build.sh gcc296 && ./stage.sh gcc296`), re-pin the `cc1` digest, and prove the
source-only build reproduces gs1-en.gba at SHA-1
`5c4695205413df7db52b9a184815a07783999971` *before* the re-pin is admissible
(`PROVENANCE.md`). Then add the routed set **and** a matching explorer mode in
`tools/mode_sweep.ts` — a coverage contract fails the test chain otherwise. Prefer
extending an existing predicate with a disjoint participation set over adding a
competing rule; both scheduler modes and both CSE modes are built that way.

**Overlay routing sets are overlay-blind, and the failure lands two layers away.**
`overlayStem` reduces both `work/…/0200142c.c` and
`assets/code/resource_3a7_c_0200142c.c` to the bare address `0200142c`, so a draft
and its installed copy compile identically — deliberate and correct. The
consequence is not: **the same address in a *different* overlay gets the flag
too.** Adding nine `resource_3a7` addresses to a new set silently recompiled three
unrelated already-exact functions (`resource_373:1554`, `resource_3b2:1740`,
`resource_373:5ae0`) without interworking.

What makes this expensive is the symptom. It is not a byte diff and not an adopt
rejection: `bun run verify` fails inside `build_assets` with **`palette token plan
does not reconstruct input`**, naming `resource_373` — an overlay the change never
mentions — and only after a `tools/` edit invalidates the asset stamp and forces
a cold asset rebuild, which can be several commits later than the edit. If you see
that error, suspect a routing set before you suspect an asset encoder, and diff
`tools/alchemy_gcc.ts` first. `build_assets.ts` does not name the failing entry;
wrap its `buildEntry(entry)` call in a try/catch that logs `entry` to get it in
one run, then revert the wrap.

**Before putting an overlay address in any routing set, run
`ls assets/code/*_c_<addr>.c` and confirm the only hit is the overlay you mean.**

**A tenth mode: no thumb interworking.** One closed overlay family returns through
a single `pop {pc}` where every other reconstructed function returns
`pop {rN}` + `bx rN`. Dropping `-mthumb-interwork` reproduces it exactly; with the
flag on, the two-instruction return overruns the span and `overlay_adopt` rejects
the *placement* (`overlay C placeholder is not zero at …`) rather than reporting a
byte diff — so the shape reads as a tooling bug, not a flag problem. Scope is
measured and small: `pop {pc}` occurs **18 times in all unconverted overlay
assembly against 2,195 `bx` returns**, and those 18 are exactly this family, nine
in `resource_3a7` and nine in `resource_3bf`. Fifteen are routed and converted
(240 bytes); the other three are the collision casualties above and need an
overlay-aware routing key, not a wider set. This is a stock gcc flag, so it needed
no fork change and no re-pin.

**Routing sets are easy to edit into the wrong one.** Two functions that verified
exact were rejected by the adopter because a scripted edit put their paths in the
neighbouring set — the surrounding lines matched in both. The symptom mimics a
tooling bug: `overlay_verify` takes flags from the command line and says 0, while
`overlay_adopt` takes them from routing and disagrees. When those two disagree,
`grep -n "<stem>" tools/alchemy_gcc.ts` against the set boundaries first.

---

## 8. Open problems

**Compiler lane, well specified:**
- **Immediate-build transposition** — but try the composition first. The
  reference schedules an independent `movs r0,#K` *between* a two-instruction
  immediate's `movs` and its `lsls`. **`-fsched-low-dest-first` applied after
  `-fno-cse-two-insn-immediate` closes many of these**: three functions sitting at
  8-17 halfwords with exactly this residual went to 0. Only what survives that
  pairing is a genuine compiler problem, and it is an `insn_cost`/`tmp_class`
  question at `haifa-sched.c:4068-4090`, not a dependent-count one.
- **`0808fecc`** (main image, floor 2): the last-scheduled-insn class rule fires
  before the ordinal tie-break, so a separate mode is needed.
- **`0801a4fc`** (main image, floor 2, 166 bytes): **value0's copy versus value2's
  retargeted definition** in the grouped-descriptor pass. With
  `-mgrouped-dma-store,-fthumb-group-value2-in-place` everything matches except
  the order of two insns before the `stmia`: we emit `ldr r2,[pc]` then
  `adds r0,r5,#0`, the reference emits the copy first. In `arm.c` the pass emits
  `SET r0, value0` *before* `store2` while `-fthumb-group-value2-in-place`
  retargets value2's constant definition **in place**, so their relative order is
  decided by where the constant was defined in the RTL stream, which no source
  spelling moved (four placements of the control word tried, plus the full flag
  matrix with pairing). This is the natural next fork change: emit the value0 copy
  ahead of the retargeted value2 definition. It needs the §7 admission ritual
  (rebuild, re-pin, prove the source-only build still reproduces the SHA-1), so it
  is a deliberate compiler-lane task, not a probe. Draft is at floor 2 and ready.
- **`080c1fa8`** (main image, floor 2): a **`mov rN,sp` versus argument-setter**
  ordinal tie-break — the reference materialises the stack base one slot earlier
  (`mov r6,sp` before `adds r0,r3,#0`; we emit the reverse). Distinct from the
  r0-r3 case `-fsched-low-dest-first` covers, because the earlier insn writes a
  high register from `sp`. Hand levers took this function 5 → 2 (see
  `docs/archive/REPROBE-2026-07-30.md`); the residual survived all 40 flag settings,
  every pairing with `-fno-sched-depend-count`, four statement placements, a
  declaration-initialiser spelling and a return-type sweep. Draft ready at
  `work/reprobe-2026-07-30/reordered/080c1fa8.c`, 84 bytes, baseline flags.
- **resource_391:02a8** (floor 7/164): the reference reuses the register holding
  an `ldrsh` offset for a table cursor; not a sched1 decision.
- **Pool-word emission**: `resource_3af:0bb8` has one surplus pool word (60 vs 56
  bytes) — a dedup/emission issue, not CSE.

**Known-unreachable, do not re-attack:**
- `resource_373:2cb0` — its reference both reloads pool words *and* keeps one in a
  register to derive a related value by an add. Related-value reuse needs exactly
  the recording `-fno-cse-pool-immediate` suppresses, so no whole-function gate can
  match it.
- The 608-byte main-image sibling pair `08098954`/`0809a294` is **solved** by
  `-fsched-high-dest-first` and adopted; the note describing it as unreachable is
  obsolete.

**Discovery: 308 of 934 known-exact functions are still not rediscovered.** The
predecessor census that found the entry-shape gate is the way to find the next
one: of the previously-missed set, 374 had no decoded instruction within 512
bytes — whole dark runs — while the rest were chain breaks. Re-run that census
against the current 308 before inventing a heuristic.

Two successor rules were built, measured and **rejected**; do not rebuild them
(the reasoning is recorded above `nextEntryAfterReturn`):
- *Tail-call chaining* (`b` to a target outside the body) seeds **0** across 96
  overlays. `walk_thumb` follows unconditional branches, so a tail call's callee
  is already absorbed into the caller and the caller's last instruction is the
  callee's return — the existing return chain covers it.
- *Chaining after an unresolved indirect* (`bx rN`) seeds 3,574 with **zero**
  precision: none of them landed on a known entry. What follows an unresolved
  indirect is the dispatch data it reads.

`tools/remaining_survey.ts` filters `retention === "c_candidate"` and so is blind
to 96 debt rows (38,456 bytes); widening it to all five debt retentions is one
line.

**Tooling.** ~~`resource_39c:10c0` verifies at 0 halfwords but `overlay_adopt`
rejects it.~~ **RESOLVED 2026-07-30: it was the §7 routing-set trap**, exactly as
that section warned. `assets/code/resource_39c_c_020010c0.c` is in no routing set,
so the adopt rehearsal (which copies the draft to that path and routes flags by
path) compiled at baseline while `overlay_verify` took
`-fno-cse-two-insn-immediate` from argv. Not a splice bug; its pool is a normal
post-body pool. Adding the path to `NO_CSE_TWO_INSN_IMMEDIATE_OVERLAY_SOURCES`
closes it (`notes/resource_39c-10c0.md`). **General rule: whenever verify and
adopt disagree, the difference is flags, and the first check is whether the
*installed* asset path is routed.**

**Span audit (2026-07-30): mis-spanning is NOT systemic.** The "an advertised row
may be a fragment of a larger owner" hypothesis was tested against six stubborn
parks (`39c:10c0`, `373:2a54`, `379:0074`, `3b2:0030`, `381:29a4`, `3af:3a0c`) —
**all six spans are correct**, and none of the parks is a span problem. Across all
1,337 strict rows, 1,334 end at exactly one return: the walker follows
unconditional branches, so a direct continuation is absorbed into the row and the
fragment failure mode is structurally rare here. **The bulk detector is
`returns == 0`** — the only three such rows are `resource_3bd:0024` (a veneer
misread), `resource_3ca:0f80` (body complete, trailing pool cut 12 bytes short;
true span 340 not 328) and `resource_399:15b4` (**a genuine fragment**: the
advertised end 0x1690 lands inside a mid-function pool that a `b` jumps over, and
the epilogue is at 0x16a4; true span **248**, not 220). A secondary check —
whether a row's end lands on a non-prologue row — flags 74 rows but is almost all
veneer banks, so it is not worth tooling. Adding a `returns == 0` warning to the
inventory writer would cover the real signal in one line.

**Venus Lighthouse candidates worth re-probing here.** Venus covers these
semantically today; an exact result from Mercury would override its version, so
they carry more value than their byte counts suggest. Both were identified but
never re-probed: `resource_3c8:1d48` (floor 3 halfwords) and `resource_379:0074`
(8 of 288 instruction groups differing — measure this one by group equality, not
halfwords, per §2).

**The 0x08006xxx IME-guarded DMA trio — parked on a gcc pool quirk, 220 bytes.**
`0800651c` (64), `080063bc` (76) and `08006408` (80) are one family: disable IME
by storing the register's own address (`strh r0,[r0]` — bit 0 of 0x0208 is clear),
clear or set a few words, restore IME. The reference has **no prologue at all** —
it uses r0-r4 only, r4 being call-used under `-fcall-used-r4`.

Two things were fixed and are worth keeping: the derived-address class closed with
`Data_` symbols (§4), and typing the saved IME value `s32` rather than `u16`
removed a spurious `lsls #16 / asrs #16` sign-extension pair. What remains is one
gcc quirk with no source lever found: for the two `u8` stores it emits a pool
`.word 0` and reloads it with **`ldrh r1, .L3`** instead of reusing the register
that already holds 0, and that extra live value spills `saved` to r5 and buys the
`push`/`pop` the reference does not have. Retyping the shared zero `u8`/`u16`/`s32`
and naming it per site all measure identically.

*The pooled-zero quirk is not confined to that trio, and it interacts with the
dead-store rule.* `resource_3ca:004c` ends with a single `*(u16 *)0x05000000 = 0`
where the reference emits `movs r2,#0`. Without `volatile` gcc **deletes** the
store as dead — the whole tail vanishes — and with `volatile` it emits the same
`ldrh r3, .L3` off a pooled `.word 0`. So the two available spellings are "wrong
bytes" and "no bytes"; there is no third. Treat a lone volatile store of zero as
carrying this blocker until someone finds the lever.

**`resource_3bb:02e8` — base-plus-offset folded into one pool word.** The
reference keeps `0x02000240` in the pool and builds the `0x1F4` byte offset
separately (`movs r2,#250 / lsls r2,#1 / adds r3,r3,r2`) before loading. Every
spelling tried folds them: `(u8 *)0x02000240 + 0x1F4`, `((s32 *)0x02000240)[125]`,
and a `Data_02000240` symbol all emit a single relocated constant, because gcc can
add a constant to either an integer or a SYMBOL_REF at compile time. Floor 23/28.
The offset has to reach the add as something gcc cannot fold, and none of the
obvious spellings does that.

**`tools/mode_sweep.ts` swept 69 modes over `0800651c` and none reaches it** —
floor 33 halfwords at 72 bytes against a 64-byte reference, under `old-agbcc`
(`out/modesweep/0800651c-*/`). The residual classes it reports are
`register=1,literal,cfg`. This is a source-shape problem; do not re-run the sweep.
Note the family is *not* agbcc despite sitting inside the agbcc module — the
reference clobbers r4 without saving, which `AGBCC_CFLAGS` (no `-fcall-used-r4`)
cannot produce.

**`resource_371:0350` — floor 4/44.** Two `ands` chains where the reference opens
on the mask (`adds r3,r1,#0 / ands r3,r2`) and we open on the value. §4's mask-first
recipe does not reach it: `(mask & value)` is canonicalised back by gcc, and
splitting the chain into statements makes it worse (21). Same for the trailing
`eor`. It needs an allocation lever, not an operand-order one.

**The 60-byte squared-distance family — floor 20/60 bytes, two named residuals,
780 bytes behind it.** One fingerprint (`1fwqz6zhzfrzo`), 13 strict unconverted
members, every one 60 bytes: `resource_373:0030`, `389:0030`, `391:0030`,
`392:0030`, `393:0030`, `39f:0030`, `3b2:0030`, `3b4:0030`, `3b5:0040`,
`3bf:0030`, `3c4:0030`, `3c5:0030`, `3c8:02f0`. It is the cheapest large payoff
in the overlay queue — one correct draft closes all 13.

The shape is a 3D squared distance on 16.16 fixed point, and **the call takes four
arguments, not one**. That was the whole discovery: the reference's `ldr r3,[pc]`
of `0x030001d8` immediately before the `bl` is the *fourth* argument, and the
`dz*dz`/`dy*dy` left in r1/r2 are arguments 2 and 3, not dead intermediates.
Spelling it one-argument floors at 46 differing bytes; two-argument at 23;
four-argument at **20**. The constant is a plain literal, not `&Value_` — it has
no `k<<n` factorisation, so it pools by itself (§4's third row).

**The 20-byte floor is stable, so do not re-search the spelling space.** Ten
source shapes were measured: inline-shift deltas (20), deltas-then-shift (24),
shift-inside-the-multiply (26), named `x2`/`y2`/`z2` temporaries (24), a split
`sum` accumulated with `+=` (20), an extra copy temporary for the third addend
(20), declaration-order permutations (24), and a six-locals-first form (51). Five
distinct spellings land on exactly 20 and none goes below. Best draft:

```c
typedef signed int s32;
extern s32 Func_020061c0(s32 sum, s32 c, s32 b, s32 context);
s32 Func_02000030(s32 *a, s32 *b) {
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    return Func_020061c0(dx * dx + dy * dy + dz * dz, dz * dz, dy * dy, 0x030001d8);
}
```

Instructions 1-9 and 13-20 match the reference exactly. Exactly two residuals
remain, and they are worth attacking as one:

1. The reference emits `subs r3,r3,r2` (the `dz` delta) **before** all three
   `asrs`; we emit it between the second and third. Writing the three deltas as
   plain statements and shifting them afterwards *does* produce that order, but
   scrambles register allocation from instruction 4 onward (24 bytes, worse).
2. The reference spends an extra `mov r3,r1` scratch copy before the final
   `adds r0,r0,r3`, then immediately reloads r3 with the pooled constant. We fold
   it to `adds r0,r0,r1`. This is the one instruction of the size difference
   (26 emitted against 27).

**All nine routed modes were swept against the four-argument draft and none beats
baseline** — `-fsched-low-dest-first` is actively worse (15 mismatched
instructions against 10), `-fno-sched-depend-count` worse (12), the rest are
neutral. So this is a source-shape problem, not a routing problem; do not spend
another sweep on it. The untried levers are §4's return-type lever on
`Func_020061c0` (it is currently spelled `s32`-returning) and a struct/array
spelling of the coordinate triple.

Note for whoever picks this up: `work/claude/overlay_verify.ts` still does not
exist on `mercury`, and `mode_sweep.ts` is main-image only — it resolves a stem to
a ROM address, so it cannot take an overlay target. The working overlay loop is a
**dry-run `overlay_adopt.ts`** (no `--apply`), which prints
`differing_bytes=N size=A/B` in about a second:

```sh
bun tools/overlay_adopt.ts resource_373:0030 --source draft.c --span 60
```

Equal sizes confirm the span; drive `differing_bytes` to 0. Because that path
takes flags from routing rather than argv, sweep modes by compiling with `xgcc`
directly and diffing the assembly against `overlay_show.ts` output.

---

## 9. Required checks

`tools/bank_cycle.sh` does all of this. Manually:

```sh
bun tools/full_c_progress.ts --write-inventory
bun tools/full_c_progress.ts --write-report
bun run verify
```

Commit subjects must end in the suffix from
`bun tools/full_c_progress.ts --subject`, and a subject that changes the
executable denominator must begin `metrics: correct executable denominator`.

**`bun run verify` is not optional, and "my change cannot have caused that" is
not a reason to skip it.** On 2026-07-31 `build:claimed` was red for fifteen
commits because a tooling change of mine gave `externalSymbolAssembly` a second
parameter while seven callers still wrote `names.map(externalSymbolAssembly)` —
`.map` passes `(element, index, array)`, so the array index arrived as
`callViaBase` and every main-image `_call_via_rN` resolved to `index + N*4`
instead of `0x080072e4 + N*4`. The assembler grew a long-branch veneer per
affected object, each object outgrew its claimed span, and the link died on
fifteen section overlaps with **no C changed anywhere**. It cost the other two
lanes a stop-the-line and Venus a bisect. Two lessons, in order of importance:

- The red step is red on *your* branch too. It went unseen through four of my
  own banks because I ran everything except the step that was failing.
- **A default parameter is invisible to `.map`.** Defaults fill only
  `undefined`, and an index never is. `alchemy_gcc.ts --self-test` now scans
  `tools/*.ts` for `.map(externalSymbol...)` passed by reference; extend that
  scan rather than trusting review if you ever add a parameter to a callback
  used this way.

**The coverage map is Vale's, and only Vale's.** `assets/readme/gs1-en-coverage.svg`
and `metrics/gs1-en-coverage-map.json` are regenerated on `main` and nowhere
else. Do not run `bun run coverage` from `mercury` or `venus`, and do not hand-edit
either file: you would either collide with Vale or publish a figure your branch
cannot substantiate. Publish numbers by banking them — the map follows within the
hour. It is deliberately not part of `bun run verify`, so a map that lags your
newest commit is never your problem and never blocks a bank.

Vale redraws it from the two lighthouse refs rather than from any working tree.
Mercury pulls from main and never pushes back, so main's `src/` never receives
Mercury's conversions; drawn from its own worktree main's picture would sit frozen
at whatever exact C that branch happens to carry while the project moved on —
173,222 bytes against Mercury's 210,306 when this was found. `--exact-ref` and
`--semantic-ref` select the trees and the map records the choice, so main's
provenance reads `exact_lane: origin/mercury`, `semantic_lane: origin/venus`, and
both `--write` and `--check` re-resolve from that record. The reconciliation
against `metrics/<target>-progress.json` reads that report from whichever tree the
lane came from, so it stays honest across refs.

Two safety properties worth knowing, because both surface as a refused write
rather than a wrong picture: a recorded ref that is not available locally is an
error rather than a quiet fall back to the working tree, and a redraw that cannot
see the semantic lane refuses rather than publishing Venus's half as zero. Either
way the previous picture stands.

**The picture currently understates the overlay semantic lane, and closing that
is Venus's to do.** `tools/coverage_map.ts` sizes an overlay semantic owner
only from a `manual_regions` entry in `semantic/regions.json`. It deliberately
refuses the decoded-region inventory, which is build output and therefore
outside what a tracked-evidence-only tool may read, so an owner missing from
that file is reported in `provenance.semantic_unresolved` rather than
estimated. At 1,002 semantic sources, 384 overlay owners are unlisted: the map
can size 8,458 overlay bytes, while its main-image figure of 382,970 agrees
exactly. Converting overlays does not move the picture; only listing them does.

**Do not declare whole overlays — measurement rejected it.** Venus tested the
idea on 2026-07-31: 110,830 bytes of the overlays it had converted in full lie
outside any strict inventory row (veneer and import bands, tables, literal
pools), so crediting an overlay's whole audited extent overstates the semantic
lane by roughly 27,000 bytes. Strict per-owner rows in `manual_regions` are the
correct route and `semantic_regions_sync.ts` generates them. The `full_overlays`
array below still exists in `coverage_map.ts` and is inert with no entries; it is
kept only so this correction has something to point at.

**The rejected shape, for reference:** `semantic/regions.json` now also takes
a `full_overlays` array:

```json
{ "overlay": "resource_375",
  "evidence": "every executable range is owned by a semantic source; …" }
```

One reviewed assertion sizes every owner in that overlay: the map takes the
overlay's audited executable extent as the lane and subtracts exact C, so a
partly exact overlay stays honest. A claim is ignored unless the overlay
actually carries semantic sources and has an audited extent, so an unbacked
assertion credits nothing. Owners in a claimed overlay stop being reported
unresolved. That is roughly twelve entries against the 384 individual ones.

**Reconcile before you claim.** The twelve overlays this file reports converted
in full hold 174,892 audited executable bytes, of which 31,194 are already exact
C — so declaring them would move the overlay lane from 8,458 to about 143,698.
This file's own per-overlay figures for those twelve sum to roughly 110,732. The
~33,000-byte difference is most likely literal pools and alignment that the
audited extent counts and the per-owner figures exclude, in which case the larger
number is right and the claim is sound. Confirm which it is before declaring: if
any of those overlays still has an unconverted range, the claim inflates the
published picture by whatever that range holds. Put the answer in `evidence`.

`PROVENANCE.md` is authoritative on clean-room rules: semantics only from the
target's own disassembly and this repo. **No `asm()`, no inline assembly, no
register pinning, no barriers, and no `volatile` as a matching device.**

**How to decide the `volatile` question mechanically, instead of arguing intent.**
Three drafting lanes hit it on 2026-07-30 and it resolved three different ways, so
the rule is worth stating: **delete the `volatile`, re-probe, then classify.**

| after deleting it | what it means | verdict |
| --- | --- | --- |
| count unchanged | it was never load-bearing | drop it, adopt (`080060e8`) |
| count worsens, address is memory-mapped or externally mutated, and the reference genuinely repeats the access | semantics the target requires | keep it, adopt (`08006cdc`, `0800eaf8`) |
| count worsens, object is an ordinary local | a matching device | **reject the draft** (`080b386c`) |

The worked cases: `080060e8` marked a timer register `volatile` and reached 0
without it, so it was noise. `08006cdc` writes a Flash command sequence to
`0x0E005555`/`0x0E002AAA`, where repeated stores to one address *are* the
protocol, and plain C would let the compiler fold them away and break the
hardware. `0800eaf8` re-reads one IWRAM global across a chain of `else if` tests
and comes out 8 bytes short without `volatile`, which is exactly §4's documented
tell. `080b386c` declared `volatile u16 slots[15]` — an ordinary stack array with
nothing external touching it, worth 49 halfwords purely by suppressing store
elimination. That last one is the prohibited case and its draft was rejected
despite measuring 0.

**A lane's `confirmed` is not sufficient to adopt.** One verify lane passed
`080060e8` on the reasoning that its `volatile` was legitimate MMIO; the
coordinator's independent screen caught it and the delete-and-re-probe test showed
it was not even needed. Re-screen and re-measure everything at the coordinator
before installing it. Two policy escapes were caught this way in one session, the
other being the register-pinned `080044d0` drafts.

**Concurrency — do not inherit the old two-lane rule.** That limit was measured
when a verification probe cost 1.8 s and a bank cycle 190 s, so compute really was
the binding constraint. The content caches (§3) cut those to 0.12 s and 15 s, and
the limit was never re-derived. Measured afterwards on the same 4-core host with
two lanes running: **load average 1.18, and a probe costs 137 ms under load versus
120 ms idle** — roughly 70% of the machine idle. Lanes are reasoning-bound, not
compute-bound, so run **five or six** and re-measure `/proc/loadavg` before
assuming a ceiling. If probe latency starts climbing well above ~150 ms, that is
the real signal to stop adding lanes.

**Runtime subagent slots are a separate ceiling from cores.** Some runtimes cap
concurrent subagents independently of the hardware — the Codex runtime exposes
three, so a six-agent experiment there runs as two immediately consecutive waves
of three, and Venus Lighthouse cohorts are sized in threes for that reason. On a
runtime exposing five or six slots the measurements above support testing the
larger walker count rather than inheriting the historical two-lane ceiling.
Whichever binds first — slots or cores — is the real limit; check both.

**Where five lanes actually saturates a 4-core host.** Measured with five lanes
running: load average **3.60**, and a warm `build_claimed` that costs ~0 s idle
took **17.6 s** — longer than the entire warm bank cycle on a quiet machine. That
is the ceiling arriving, not a fluke, and a sixth lane there buys nothing but
contention. Memory is never the constraint (943 MB of 16 GB at five lanes). So
the rule is not "five lanes"; it is **roughly one lane per core, verified against
loadavg and probe latency**. On a bigger host, scale up — see §10.

The binding constraints are instead: walkers must never share an overlay (the
overlay `.s` file is the only mutable artifact), and only the main agent may run
git, the build scripts, `full_c_progress.ts` or `overlay_inventory.ts`. The bank
cycle tolerates lanes adopting mid-build, so there is no need to pause them.

**Agent economics.** Permuting is an audit pass, not an engine: one exact hit in
65,543 candidates, though it cost ten minutes and cracked a function a careful
lane had declared unreachable. Run `tools/permute_overlay.ts` over a newly parked
near-miss before believing the park; do not expect it to close a backlog. Cheap
models are not useful for drafting here — the ≤32-byte "easy" population is 90%
veneer thunks, word-table interiors and mid-function fragments, and the genuine
small leaves are already taken as each walk passes them.

**Do not spend an agent on enumerable search — that is the expensive mistake, and
it is independent of model tier.** Measured 2026-07-30 on an 18-core host: `xargs`
over `candidate_show` sustains **114 ms per probe at 18-way concurrency**, so a
flag matrix of 38,480 probes finishes in about four minutes and produced 2 of that
day's 8 conversions on its own. An agent exploring 20 source spellings takes about
six minutes. That is roughly four orders of magnitude less search per minute.
Split the work by whether it can be enumerated:

- **Enumerable** — flag sets and their pairings, callee return types, independent
  statement orderings, constant spellings: script it. `tools/mode_sweep.ts`,
  `tools/return_type_sweep_main.sh`, `tools/statement_order_sweep_main.ts`.
- **Not enumerable** — deciding which lever a residual implies, decoding assembly
  into semantics, noticing that an epilogue contradicts a draft's signature: that
  is what an agent is for, and it wants a strong model.

**Cheap-model breadth was measured and did not substitute for lever selection.**
Four haiku lanes were each given a size-exact near-miss with its residual quoted,
the lever sections named, and the sweeps pointed out. Result: **0 of 4 improved**
across 66 spellings, ~6-8 minutes and ~90k tokens per lane, with honest reports
(one correctly spotted that the statement-order sweep had only produced 3 probes
on its target, so that lever had never been exercised there). Caveat, stated
plainly: those four were the residual-hard remainder — two had already resisted a
full-effort attempt — so this is not a controlled comparison and should not be
read as a model benchmark. What it does show is that quoting the fingerprint and
listing the levers is not enough; choosing the lever is the costly step.

**Workflow orchestration is not the overhead people assume.** Verify stages fired
the moment each solve returned and nothing queued at 12 lanes. But note the cap:
concurrent agents per workflow are `min(16, cores - 2)`, so on an 18-core host a
32-agent workflow runs 16 and queues the rest. For very wide cheap breadth,
separate `Agent` calls avoid that ceiling; for pipelined solve-then-verify work
the workflow is the right shape.

---

## 10. Mercury Lighthouse — running the exact-C effort on a high-compute host

This section is written for an agent taking `mercury` over with substantially
more compute than the 4-core cloud container the measurements below come from.
Those sessions were **reasoning-bound per lane but core-bound in aggregate**; more
cores changes only the second term, so read this as "how to spend cores", not
"how to work". Everything in §1-§9 still applies unchanged.

**Bringing up a bare cloud container (2026-07-30, measured end to end: ~35 min).**
A fresh clone cannot run anything in the exact lane: `bun run verify`,
`candidate_show.ts` and `overlay_verify` all read `roms/gs1-en.gba`, and every
compile routes through `../alchemy-gcc/dist`, which is *built*, not committed.
Four separate things have to be right, and three of them fail in ways that look
like a repository bug rather than a missing prerequisite.

1. **The ROM is yours to supply.** `roms/` and `*.gba` are gitignored and must
   stay that way. Drop the approved image at `roms/gs1-en.gba` and check it
   against tracked `rom.sha1` (`5c46952054…`) before trusting a single measurement.
   Then `git config core.hooksPath .hooks` exactly as `PROVENANCE.md` says.
2. **`arm-none-eabi-as`/`objcopy` are host binutils, not part of the bundle.**
   `build_asm.ts` shells out to them by bare name. `apt-get install -y
   --no-install-recommends binutils-arm-none-eabi` — a few seconds.
3. **Build the compilers from the sibling repo.** Clone `PascalPixel/alchemy-gcc`
   next to this checkout so `../alchemy-gcc/dist` resolves, then
   `./build.sh all` and `./stage.sh all`. gcc296 takes ~3 min on 4 cores; agbcc,
   gcc3 and gs2 a few more each. All five bundles are needed —
   `alchemy_gcc.ts --self-test` builds a plan for `pret-early-thumb` and
   `gcc2951` too, so `bun run verify` fails if either is missing.

   *Two defects here cost a session on 2026-07-30 and are now fixed upstream at
   `alchemy-gcc` `2581e3e`; if you are on an older checkout of that repo you will
   still meet them.* `stage.sh` had no token for `pretearlythumb` or `gcc2951`
   even though `build.sh` built both, so those two bundles could only be staged
   by copying `cc1` into `dist/pret-early-thumb/` and `dist/gcc2951/` by hand.
   And `build.sh` re-stamped its pre-generated `c-parse.c`/`c-gperf.h`/`configure`
   only inside `build_gcc_tree`, so gcc-2.95.1 — which has its own recipe — went
   without: **git does not preserve mtimes**, a fresh clone lands every vendored
   file in the same second, make decides the shipped parser is stale, and a
   modern bison rejects the 1999 grammar (`$$ for the midrule at $4 of
   'structsp' has no declared type`). The same gap let autoconf 2.71 silently
   rewrite the shipped autoconf 2.13 `configure`, which is a tracked file — check
   `git -C ../alchemy-gcc status` after any build that reached for autoconf.
   `build_2951` also omitted `--build`, so its 1999 `config.sub` choked on the
   x86_64 triple `config.guess` reports. **Any alchemy-gcc build failure naming
   bison, gperf or autoconf is this class, not a broken tree.**

**Match the pinned Bun.** `package.json` pins `bun@1.3.14`; the container shipped
1.3.11 and `gba_header.ts --self-test` failed with "GBA logo source must be the
canonical 104x16 monochrome PNG". That check re-encodes the tracked PNG and
demands byte equality, and its IDAT comes from `deflateSync(rows, {level: 9})` —
i.e. it is pinned to Bun's bundled zlib. It is a Bun-version tell, not a corrupt
asset; `git status` on the PNG is clean throughout. Install the pinned version
before concluding anything about the assets.

**A locally built bundle will not match the pinned digests, and that is expected.**
gcc embeds prefix paths, so a from-source build at a different path differs
byte-for-byte while its *codegen* is identical — `build_claimed` came back
`linked=1406 failures=0` on the first try. The one target that reproduced the
pinned digest exactly was `pret-early-thumb`. So do not read a digest mismatch as
a broken build, and do not read a matching digest as the only admissible outcome.
`EXPECTED` now holds a **list** of approved digests per file rather than one, so a
second host can be admitted without evicting the first. The bar for adding to
that list is unchanged and is the whole point: run `bun run verify` and get
`byte_identical=yes rom_fallback_bytes=0` with that bundle first. This container's
bundle was admitted that way on 2026-07-30.

**What the ceiling actually is.** Measured on the 4-core cloud host: one lane
idles at ~120 ms per probe; two lanes, load 1.18, 137 ms; five lanes, load 3.60,
and a warm `build_claimed` that costs ~0 s idle stretched to 17.6 s. Memory never
bound (943 MB of 16 GB at five lanes). So the working rule is **about one lane per
core**, and the falsifiable stopping signal is **probe latency climbing past
~150 ms** — measure it, do not guess it. On a 16-core machine expect 16-20 lanes
to be sane; the scaling is close to linear because lanes share nothing but the
repo.

**Give every drafting lane its own output directory.** Lanes told to write
`work/<shared>/<stem>.c` destroyed each other's files between tranches on
2026-07-30: five confirmed byte-exact drafts vanished, four were recovered only
because the *verify* lanes had copied them into their own scratch, and two regions
that had been solved had to be re-drafted from scratch. Use
`work/<run>/lanes/<stem>/<stem>.c`, and adopt confirmed results promptly instead
of batching them across tranches. Bytes measured exact and then lost are the most
expensive outcome available.

**What must stay serial, at any core count.** These are correctness constraints,
not performance ones:
- Two lanes must **never share an overlay**. The overlay `.s` file is the only
  mutable artifact, and adoption rewrites it. Assign whole overlays — the
  "whole owner, not manifest row" rule.
- Only the coordinator runs git, the build scripts, `full_c_progress.ts` or
  `overlay_inventory.ts`. `tools/bank_cycle.sh` already tolerates lanes adopting
  mid-build (it retries), so lanes never need pausing to bank.

**Assign overlays by small-row count, not by total strict bytes.** This was
measured the hard way: `resource_3b8` ranks second by strict bytes (15,684) but
its queue is 7 rows whose smallest is 348 bytes and whose largest is 7,468 — a
lane spent a full session there and adopted nothing. Overlays with many small
rows convert; overlays with a few huge rows park. Rank candidates by rows under
~400 bytes.

**The largest recoverable backlog is stale park notes, not new territory.** Two
independent classes were disproved this session (§6): the "repeated same-bb
two-instruction immediate" class, which the paired CSE modes close, and several
notes predating `-fsched-low-dest-first` and `-fno-sched-depend-count`. One lane
re-probed six old notes in ~4 minutes and closed three. With many cores, a
dedicated **re-probe sweep across every existing note in `work/claude/notes/`** is
almost certainly higher yield per core-hour than walking new overlays, and it is
purely mechanical. Do that first.

  *Measured on the main image, 2026-07-30, and it does **not** hold there.* An
  18-core run took that advice to exhaustion on `mercury`: 1,035 drafted-but-
  unadopted stems, deduplicated to 3,188 drafts, of which 964 both compile and
  pass the policy screen. Sweeping all 964 against 20 flag settings — each routed
  mode alone, both CSE modes paired, and the §5 compositions — is **19,280 probes
  in 122 s** and yielded **exactly one exact match** (`08078144`, 228 bytes,
  adopted). A 1-in-964 yield is not a backlog. The overlay-side seam this
  paragraph describes was real; **the main-image drafted population has no
  equivalent, so do not budget a session for it.** Full measurement in
  `docs/archive/REPROBE-2026-07-30.md`.

  The tempting inference was that because *no* note under `work/` mentions
  `-fsched-low-dest-first` or either CSE mode — the corpus wholly predates them —
  those modes must be holding back a batch of main-image functions. That is the
  §6 stale-evidence shape, it was the right thing to test, and it is **false**:
  across 964 drafts they produced no additional exact match. They still move
  individual halfword counts, so keep probing them per function; do not expect a
  cohort.

**Screen drafts against `PROVENANCE.md` before believing a zero.** The same run
found two drafts scoring 0 halfwords at 212/212 in `work/hand/080044d0/exact*/`
that are **inadmissible**: they get there with `register s32 ratio asm("r1")`.
Ranking a draft population by score alone puts register-pinned and barrier drafts
at the top of the list, where they read as free adoptions. Grep for `asm(`,
`register … asm`, `volatile` and `__attribute` first. `080044d0`'s best clean
variant is 1 halfword at exact size, which makes it the best ratio in the project
(212 bytes behind one register identity) — and the pinned drafts conveniently
prove which assignment to aim a legitimate allocation lever at.

**The return-type lever had never been run on the main image at all.**
`tools/return_type_sweep.sh` scores through `work/claude/overlay_verify.ts`,
which does not exist on `mercury` and only accepts `<overlay:offset>` targets, so
every main-image park predates the lever entirely — §5's "treat a sweep null
before 2026-07-30 as unmeasured" understates it here, because there was never a
null to record. `tools/return_type_sweep_main.sh` ports it to
`candidate_show.ts`, never overwrites the input draft, and prefers a size-exact
result at equal halfword count. 494 sweeps in 29 s improved 67 drafts (`080c0be4`
105 → 84, `080aad10` 112 → 95) and closed none; improved drafts are saved in
`work/reprobe-2026-07-30/drafts/`. Run it on any newly drafted main-image
function before parking it.

**Host concurrency: trust probe latency, not load average.** On an 18-core M5 Max
with 18 concurrent probes, load average reached **25.6** while per-probe latency
stayed at **~114 ms against a 116 ms idle baseline**. §10's stopping signal
(latency past ~150 ms) never triggered. The 4-core "roughly one lane per core"
rule understates a large machine; load average above core count is not by itself
the ceiling.

**Where to resume walking**, with the next offset each lane had already decoded:
`resource_3c8:07d8` (200), `resource_39f:1520` (176), `resource_38f:0304` (196),
`resource_372:1348` (336 — a direct structural sibling of the adopted `0f38` and
the drafted `1154`; that draft lived under `work/`, so it is gone, but the `0f38`
source is tracked and substituting constants into it should land this quickly).
The overlay ranking in that sentence is stale — §1 carries the recount.

*Superseded for the small tier as of 2026-07-30.* That tier was drained in a
single session: **119 functions, +7,022 exact bytes, 201,278 → 208,300**, on the
4-core cloud container, which is the project's own median day. The method is §1's
— refresh the inventory, run `tools/overlay_wrapper_draft.ts`, probe with a
dry-run `overlay_adopt`, hand-write only what the tool refuses. Roughly half the
functions never needed a second probe. **Start there, not at the offsets above**;
those are 200-336 byte rows and the band under 128 bytes is both larger and
cheaper.

Two conversions in that session needed a routed flag rather than a source
respelling, which is the honest ratio for this tier: `02001050` wanted
`-fno-sched-depend-count` for §4's pool-load hoist, and `020011bc` wanted
`-fsched-low-dest-first` for §7's `movs`/`negs` order swap. Everything else was
source shape.

**Measured 2026-07-30: an 18-core host did not beat the 4-core one on bytes.**
Daily exact-byte gains on the 4-core cloud host, from `docs/full-c-history.csv`:
1,426 / 12,720 / 5,148 / 7,400 / 2,484 / 6,328 / 21,792 / 7,174 / 14,634 / 6,468 /
10,282 / 5,134. Median about 6,800. A full session on the 18-core machine
produced **2,554 bytes**, below their typical day. Confounders, stated so the
comparison is not oversold: those days include overlay conversions, which come in
larger and easier rows, while that session was main-image only; a large share of
it went into tooling and measurement rather than conversion; and it was hours, not
a day.

The conclusion is not "the big machine is bad", it is that §9 was right and the
implication is sharper than it looks: **compute makes enumerable search free, and
enumerable search is nearly exhausted.** In that session 38,480 flag probes ran in
about four minutes and yielded 2 conversions, while agent drafting lanes yielded
7. Cores now buy you `tools/finish_draft.sh` finishing in ~2 s instead of an hour
of hand-probing — real, permanent, and *not* where the remaining bytes are.

So the thing to scale is **concurrent drafting lanes on unconverted regions**, not
probes and not lanes grinding near-miss residuals. Measured hit rates on the same
day: fresh drafting converted **7 of ~18 lanes**; re-probing existing drafts
converted **2 of 1,259 draft-probes**; four cheap-model lanes on residual-hard
near-misses converted **0 of 4**. The binding limits are agent concurrency and
model quality on assembly-to-semantics, so pick the host for how many strong
drafting lanes it can run, not for its core count.

**Measured 2026-07-30: an 18-core host did not beat the 4-core one on bytes.**
Daily exact-byte gains on the 4-core cloud host, from `docs/full-c-history.csv`:
1,426 / 12,720 / 5,148 / 7,400 / 2,484 / 6,328 / 21,792 / 7,174 / 14,634 / 6,468 /
10,282 / 5,134. Median about 6,800. A full session on the 18-core machine
produced **2,554 bytes**, below their typical day. Confounders, stated so the
comparison is not oversold: those days include overlay conversions, which come in
larger and easier rows, while that session was main-image only; a large share of
it went into tooling and measurement rather than conversion; and it was hours, not
a day.

The conclusion is not "the big machine is bad", it is that §9 was right and the
implication is sharper than it looks: **compute makes enumerable search free, and
enumerable search is nearly exhausted.** In that session 38,480 flag probes ran in
about four minutes and yielded 2 conversions, while agent drafting lanes yielded
7. Cores now buy you `tools/finish_draft.sh` finishing in ~2 s instead of an hour
of hand-probing — real, permanent, and *not* where the remaining bytes are.

So the thing to scale is **concurrent drafting lanes on unconverted regions**, not
probes and not lanes grinding near-miss residuals. Measured hit rates on the same
day: fresh drafting converted **7 of ~18 lanes**; re-probing existing drafts
converted **2 of 1,259 draft-probes**; four cheap-model lanes on residual-hard
near-misses converted **0 of 4**. The binding limits are agent concurrency and
model quality on assembly-to-semantics, so pick the host for how many strong
drafting lanes it can run, not for its core count.

**When extra compute stops paying — hand the branch back.** More cores buy
throughput on *parallelisable* work, and this project has two kinds. Fan-out work
scales: walking fresh overlays, the stale-note re-probe sweep, mode sweeps,
`return_type_sweep.sh`, `permute_overlay.ts` runs. Serial work does not: the
compiler problems in §8, a park that needs a new lever rather than another probe,
and any question about whether a mode is admissible at all. Those are one-agent,
one-thread problems where twenty lanes produce twenty copies of the same floor.

The signal to hand back is therefore **not** a byte count — it is when the
*remaining* work is mostly of the second kind. Concretely, hand `mercury` back
when any of these hold:

- The re-probe sweep over `work/claude/notes/` is exhausted and the newly closed
  functions have dried up.
- Lanes are returning parks rather than adoptions — say, under a third of started
  functions closing — because that means the frontier has moved from "apply a
  known lever" to "find a new one".
- The queue that remains is dominated by large rows (over ~1 KB) and the §8
  compiler problems, which are transcription and analysis work, not throughput.
- Probe latency stays flat as lanes are added, but conversion rate does not rise —
  the machine has headroom and the *method* is the constraint.

**How to hand back.** Push `mercury`; every cycle is banked and pushed, so it is
always resumable from origin, and there is no session state outside the repo.
Before handing back, make sure this file reflects what the run learned: §1's
overlay ranking, §4 any new levers, §6 any park class proved or disproved, §7 any
routed mode, §8 the open compiler problems. Levers and disproved park classes are
the most valuable thing a high-compute run produces — a closed function is worth
its bytes, but a lever is worth every function of its shape. Record them even when
the run that found them had cores to spare.

Do not leave findings only in `work/claude/notes/`; the notes decay into stale
evidence exactly the way §6 describes, and this session recovered five separate
categories of it. If a note's blocker was later closed by a mode or a lever, edit
the note rather than leaving the contradiction for whoever reads it next.

**`work/` is in `.gitignore` — findings left there do not survive the session.**
This is stronger than the paragraph above and was learned the hard way on
2026-07-30: a full session of notes and 238 improved drafts sat under
`work/reprobe-2026-07-30/` and were invisible to `git`, so a bank cycle reported
"nothing staged; tree matches HEAD" while the tree looked full of new work. Every
per-function note this project has ever written is in the same position, which is
the mechanical reason the notes corpus decays.

So: **anything you want the next session to have goes in `HANDOVER.md` or
`docs/`.** That session's record is `docs/archive/REPROBE-2026-07-30.md`, with per-stem
floors and their winning flags in `docs/archive/reprobe-2026-07-30-floors.tsv` and
`docs/archive/reprobe-2026-07-30-mode-matrix.tsv`. Drafts stay untracked by convention;
regenerate a floor with `tools/finish_draft.sh` rather than trusting a number
copied from a note.
