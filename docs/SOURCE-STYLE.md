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
Do not humanize a source merely to make it look modern or elegant: a small
change in type or alias information can change register allocation and break
the exact bytes. Any identifier, type, or comment change still requires the
owner's routed byte comparison.

Never use `asm(...)`, fixed-register bindings, empty assembly barriers, or any
other assembly escape hatch in C or headers. Byte equality never overrides
that rule.
