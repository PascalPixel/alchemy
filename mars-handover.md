# mars wake note

Written 2026-08-01 by Garet, for a self who has forgotten everything. Read this,
then `docs/TEAM-OPS.md`, then `HANDOVER.md`. **`work/` is gitignored — nothing
you leave there survives a fresh worktree.** This file is tracked; that is why
it exists.

Handles are machine tokens (branches, worktrees, `chat_post.sh`, kanban
`@owner`). People have names. `mars` is Garet, `venus` is Isaac, `jupiter` is
Ivan, `mercury` is Mia, `vale` is Kraden. **Never let a handle appear as a name
in prose** — I did it once and it read as two people.

---

## 1. A ZERO DELTA IS NOT PROOF OF ACCOUNTING

The closing check for certifying an overlay looks like this:

```
owners + head + tail  ==  image ?
```

**It is weaker than it looks and I nearly sealed on it.** `gapsBetween` skips
any gap of `ALIGNMENT_SLACK` (2) or less, so sub-slack remainders appear in no
gap list, in no `--json`, and in no naive total. On `resource_398` the sum came
out **sixteen short**, and enumerating found eight two-byte `00 00` remainders.

**The rule: enumerate every sub-slack remainder yourself. The sum can only
disagree; it can never confirm.**

**This applies to seals already held.** `resource_3b8` summed to zero — but
because that overlay happens to have **no interior slack at all**, not because
the check is strong. Same clean sum, completely different amount of evidence
behind it. I audited all ten sealed overlays afterwards; every sum closes, so no
seal needed re-opening, but that was luck confirming a weak check rather than a
strong check confirming a result.

**What the audit did find:** two sub-slack remainders tree-wide that are NOT
zero, and both are `bx r0` — `resource_3a4` `0x2d56` (after owner `0x2d08`) and
`resource_3bc` `0x3b7e` (after owner `0x3b40`). Both are **undercounted spans**
that stop between their own `pop {r0}` and their own `bx r0`. Alignment padding
is zero by definition; a non-zero remainder is not padding. Fixed in
`overlay_gaps.ts` at `7b4a9c41` — the skip now tests CONTENT, not just size —
with self-tests in both directions. Sweep E reaches the same two owners from the
other side via "recorded span contains NO return shape": two instruments,
unrelated failure modes, one answer.

## 2. THE A/B/C LIVENESS CONTROL IN HANDOVER IS DEAD

`overlay_published.ts resource_zzz` **used to** fall back to the whole tree and
print `residue=1604`, which made a bogus name a genuine liveness signal. Ivan
made it REFUSE. It now prints `NOTHING SWEPT` and exits non-zero.

**A zero from a refusing tool proves nothing about a real overlay.** Every
A/B/C liveness claim made with the old control since that change is unsupported.

**Use a known-dirty overlay instead**, the way sweep D always required.
`resource_3a4` is the one control that discriminates for all five sweeps at
once, in one session:

```
overlay_published  resource_3a4 -> residue=1
overlay_gaps       resource_3a4 -> code_suspect_gaps=3, tail PROLOGUE-SUSPECT
overlay_certify    resource_3a4 -> sweep_e_findings=4
overlay_gaps       resource_zzz -> exit 1        (refusal still works)
```

**A better tool creating a dead control is a failure mode with no name.** When
someone improves an instrument, ask what it broke in the checks that relied on
its old behaviour.

## 3. MY INSTRUMENT RULES, AS A SET

I caught **six** of my own errors this campaign. Every one by a control; **not
one by review, care, or re-reading.** That is the whole lesson — do not resolve
to be more careful, build the control.

- **Ask what a population should look like before believing its size.** A dead
  `awk strtonum` fabricated "none" for 25 overlays. `maskBanks` used image-wide
  reported 538 phantom rows; the tell was nine spans of 226-2,802 bytes when no
  veneer is 2,802 bytes. A misaligned bank scan reported 54 returns at a perfect
  stride of 8 — **a run at a fixed stride is a table, and a table the instrument
  cannot see is the instrument's fault first.**
- **Do not hand-total a histogram.** I wrote 47 slots; it is 46. Re-run the
  tally, do not re-add the column.
- **Do not infer an entry from a return.** I ruled `396:0x1226` unreachable by
  walking back from a `bx lr`; the entry is `0x1224` and it has four callers.
  The tool that resolves callers knows the entry and you do not.
- **A bound you chose is evidence about you, not about the function.** Emit
  every bound, including the ones you drew by hand.
- **Inert where measured, load-bearing elsewhere** (`HANDOVER` §5n). The
  register-comparing veneer predicate changed 0 of 96 tail counts and was the
  only reason the head sweep could later see all 38 export stubs. **Judge a fix
  by whether its argument is sound, not by whether its number moved.**
- **Never pipe a gate.** `bun tool | tail; echo $?` reports `tail`'s status. It
  bit me the same night I posted the warning about it.
- **A regression invariant compared against a stale baseline is not an
  invariant.** I compared sweep A against a residue figure from earlier the same
  night and got a nonsense delta of 3; four owners had been adopted in between.
  Re-run the committed tool against the same tree.
- **Two METHODS agreeing is evidence; two DRAFTS agreeing is inheritance.**

## 4. TWO-BYTE `bx lr` OWNERS — IDENTICAL BYTES, OPPOSITE DISPOSITION

There are two on the tree and they must be handled differently. **The
discriminator is who references them; the semantics live entirely outside the
function.**

- **`resource_3bb 0x4c` — ADOPTED.** Published TWICE from data-table slots
  (`0x4928`, `0x4934`). Two table entries install it as a handler, so it is a
  game-side no-op callback and belongs to the C population.
- **`resource_398 0x904` — NOT ADOPTED, and do not.** `bl`-reached 15 times from
  inside owners, never published, a single-slot `call_via` bank padded with
  `0000`. Compiler dispatch runtime; belongs to the retained-asm helper bank.

Adopting the second by analogy with the first would verify green and nothing
would ever complain.

## 5. STILL OPEN

- **`resource_39c 0x30`** is the ONE remaining CODE-SUSPECT head, deliberately
  unadopted. It is the 22-byte bitfield setter that sits byte-identically on 14
  overlays, and 39c's copy is the only one with callers (**28 sites**). It was
  held because Ivan is measuring against that overlay — a known flaky compiler
  defect on one of its rows, **ruled leave-it**: 16% baseline, ~0.5% with five
  suppression flags over 400 runs, and suppression is not a fix. **A flaky red
  on `resource_39c` is expected; re-run before reporting it.** Ask before
  adopting; the body is fully derived in the other 13 files.
- **Sweep A's new `A leaf` class holds ten residue owners** nobody has drafted:
  `371:0x2cc`, `392:0xb8c`, `392:0xbac`, `393:0xd5c`, `393:0xd7c`, `396:0x1224`,
  `39c:0x30`, `3a7:0x1424`, `3ad:0x131c`, `3cb:0x128`.
- **`HANDOVER.md` section labels collide** — three `## 5k`, three `## 5j`, two
  `## 5h`, two `## 5i`, because lanes append in parallel and hand-assign the next
  letter. **Cite section TITLES, not numbers**, until someone reconciles them.
- **Sweep D reports candidates; sweep E rules them.** Do not port E's
  pointer-half or dispatched rules down into `ruleTail` — both need the
  owner-start set and resolved `bl` targets, and `overlay_gaps` is the lower
  module `overlay_certify` imports. A RETURN-SUSPECT tail is a work item, not a
  defect.

## Before opening any overlay

1. `grep -n "resource_<id>" HANDOVER.md | grep -i certif` — **and READ the
   hits.** A `certif` match is as likely to be "`resource_3a4` is therefore NOT
   certified closed" as it is to be a seal. I re-certified `resource_3b8` from
   scratch because I skipped this.
2. Sweep D first with `--json`; rank by summed code-suspect bytes.
3. Pair every rank with the deduplicated residue-owner count AND the coverage
   percentage. **Coverage is a flag to investigate, never a verdict** —
   `resource_398` is 52.7% covered and fully certified, because the other 45.8%
   is a tail that is RULED rather than merely counted.
4. Liveness controls in the same session (see §2).
5. Enumerate the sub-slack remainders (see §1).
