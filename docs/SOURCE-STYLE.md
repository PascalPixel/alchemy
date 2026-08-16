# Source style

This guide describes the style of reconstructed C. It is a readability and
provenance convention, not evidence that a name appeared in the original
source. A name or comment must never conceal uncertainty or replace byte-exact
verification. See [`PROVENANCE.md`](../PROVENANCE.md) for the clean-room
boundary and [`CONTRIBUTING.md`](../CONTRIBUTING.md) for the reconstruction
loop.

## Japanese-era naming conventions

The canonical source aims for the concise style common in Japanese console
games of the early 2000s. Use these conventions when the meaning is supported
by evidence:

- Use short romanized names for assets whose meaning is established, such as
  `iwamuro`, `ougonmon`, `taki`, `ki`, and `hashi`.
- Prefer period-appropriate abbreviations in code: `Flg`, `Ev`, `Btl`, `Chr`,
  `Tbl`, `Work`, `Pos`, `No`, `Get`, and `Set`.
- Keep names deliberately opaque when the meaning is not proven:
  `Func_`, `Data_`, address-based names, and `unknown_` are better than a
  confident guess.
- Distinguish a reconstruction name from a recovered original name. Do not
  claim to have recovered an original identifier unless the ROM or another
  permitted source actually supports that claim.

This convention applies to reconstructed identifiers, not to copied code. Do
not import game-specific names, types, comments, or function bodies from
another decompilation project.

## Address names and reconstructed APIs

An address is useful evidence, but it is not a useful API. Once local evidence
establishes what a function does, keep its `Func_XXXXXXXX` relocation or linker
name at the ABI boundary and use a descriptive reconstruction name in the C
body:

```c
extern u8 *Func_0200538a(s32 actor);
#define GetSceneActor Func_0200538a
#define SetupRoofActorsByFacing Func_02000f80

void SetupRoofActorsByFacing(void)
{
    u8 *actor = GetSceneActor(15);
    /* ... */
}
```

This preserves the emitted symbol while allowing contributors to recognize
call families and scene structure. Use three confidence levels:

- Keep `Func_`, `Data_`, and field offsets when behavior is still unknown.
- Use a cautious behavioral name such as `GetSceneActor`, `AdvanceSceneStep`,
  or `ShowPlacementPanel` when calls, arguments, and effects demonstrate it.
- Use a story, map, actor, or asset name only when local reconstruction evidence
  establishes that identity. A plausible interpretation is not enough.

Raw address names should not remain in executable statements or explanatory
comments once an evidence-backed alias exists. A required exported entry may
use the same macro technique, leaving the raw name only in the alias layer.
Code overlays need one extra caution: a synthetic `Func_020...` relocation
name is not necessarily a stable logical callee. Different call sites may reuse
one raw spelling with different signatures or engine targets. Give those sites
separate behavioral aliases rather than forcing one misleading global name.
When the same verified ABI and behavior recur in multiple owners, graduate the
alias and prototype to the appropriate shared header instead of maintaining
slightly different local vocabularies.

Names are reconstruction aids, not claims about Camelot's original identifiers.
Every naming or prototype change still goes through the routed byte comparison;
macro expansion alone does not prove that the inferred API is correct. That
comparison is cheap insurance rather than a deterrent, because the naming layer
is measurably byte-neutral: renaming locals produced byte-identical output over
19 occurrences, and aliasing a callee through the `#define HumanName
Func_<addr>` pattern produced byte-identical output over 18 real call sites,
both verified with `cmp` on the binary (2026-08-16). Humanizing spelling cannot
regress a score, so run it without fear, and read any byte delta after a
naming-only pass as a genuine regression to investigate rather than expected
churn.

## Comments

Comments should be concise and explain facts useful to a future contributor:
relationships between values, invariants, hardware constraints, and any
compiler-sensitive reason that is necessary to understand the exact result.
Do not turn comments into a speculation diary, a screenshot caption, or a
claim about the original source.

Canonical `.c` and `.h` comments are normally brief Japanese UTF-8 comments,
matching the project's Japanese naming convention. English comments are fine
when they make a technical constraint or provenance fact clearer; avoid
duplicating every comment in both languages.

```c
/* マップチップ切替。ヘッダ値に従い表示窓へ文字ブロックを割り当てる。 */
/* セル番号は下位12bit。上位4bitは属性値として扱う。 */
```

Use comments to record a demonstrated invariant, not an unverified semantic
interpretation. If an explanation depends on compiler behavior, record the
reproduction and scope in `LAWS.md` as well.

## Types and structure

Prefer the simplest C shape supported by the evidence. Keep uncertain fields,
casts, aliases, signedness, and control flow explicit until they are proved.
Risk lives in types, scoping, and control flow, not in spelling. A small change
in type or alias information can change register allocation and break the exact
bytes, so do not retype or re-scope a source merely to make it look modern or
elegant. Identifier and comment changes are measurably free, as recorded above.
Every change of any kind still goes through the owner's routed byte comparison.

The same bound runs in the other direction: write the compiler's input, not
its output. State plain indexing and ordinary expressions rather than
transcribing optimizer artifacts such as walking offsets, hand-shared
subexpressions, or hoisted invariants back into the source; the reasoning and
symptoms are in [`CONTRIBUTING.md`](../CONTRIBUTING.md).

A decompiler `goto` chain is the clearest instance of output transcribed as
input, and removing one is the largest structural win measured so far. On
080bbb0c, deleting six decompiler labels took the register-blind structural
distance 508 -> 452 and cut labels from 19 to 5 (2026-08-16); see
[`CONTRIBUTING.md`](../CONTRIBUTING.md) for that instrument. This is not a
license to strip every `goto`: `LAWS.md` records two cases where an explicit
`goto` is exactly what the reference compiled from, the shared-tail CFG where
N duplicated tails do not cross-jump and writing them out cost 97 of 104 bytes,
and the loop whose exit must be a `goto` rather than a `break` to suppress the
loop roll. Delete a `goto` that a decompiler invented, keep one the evidence
demands. Do not extend this to types: full correct `struct BattleUnit` typing
scored **worse** than the decompiler shape on the same owner, and pointer
retyping alone was byte-identical. Struct-shaped source is not automatically
closer to the original.

Never use `asm(...)`, fixed-register bindings, empty assembly barriers, or any
other assembly escape hatch in C or headers. Byte equality never overrides
that rule.
