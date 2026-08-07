> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

# Clean-room and publication contract

Alchemy is a clean-room reconstruction. This contract is part of the source
tree so that a fresh clone has the same evidence and publication boundary as
the working repository.

## Evidence boundary

The complete private game-evidence boundary is the explicitly supplied ROM set:

- `gs1-{en,ja,de,es,fr,it}.gba`
- `gs2-{en,ja,de,es,fr,it}.gba`

`gs1-en.gba` is the immediate byte-identical build target. The other eleven
approved ROMs may be compared locally to distinguish shared engine code and
data from edition-specific content.

Do not inspect or import any other Golden Sun checkout, repository history,
source, symbols, labels, pseudocode, scripts, generated output, or notes.
Public decompilation projects such as `pret/pokeemerald` may be consulted only
for generic repository, build, testing, and publication conventions. Do not
copy their game code, labels, assets, or game knowledge.

Generic tools and public architecture or compiler documentation are allowed.
All Golden Sun-specific knowledge must be derived anew from the approved ROM
set and the independently authored sources tracked here. A cross-ROM match is
evidence for shared layout or behavior, not authorship or an original name.

## Publication boundary

Publishable material is canonical, independently reconstructed source:

- C and reconstruction assembly;
- build, verification, and analysis tools;
- semantic metadata and deterministic encoder descriptions;
- authoritative source assets used by the build, including source graphics,
  maps, text, and audio.

Never publish or transmit an approved ROM, a built ROM, raw ROM excerpts,
copied ROM gaps, binary patches, cross-ROM binary diffs, private analysis
output, compiler or disassembler output, toolchains, object files, ELFs, build
products, or opaque extraction dumps. Never send private ROM content or
private generated artifacts to network tools.

Reconstruction assembly is publishable source, but it is not a C
decompilation. A region counts as decompiled only when its C source compiles
byte-identically. Do not use inline assembly, hard-register pins, or copied ROM
bytes to turn a near match into a C claim.

## Local enforcement

After cloning, activate the tracked hooks once:

```sh
git config core.hooksPath .hooks
```

The hooks run the staged and outgoing-history checks implemented by
`tools/check/check_publication.ts`, including commits where a forbidden artifact was
added and later deleted. The same checks can be run directly:

```sh
bun tools/check/check_publication.ts --staged
bun tools/check/check_publication.ts --self-test
```

The file-shape gate is defense in depth; it cannot determine where knowledge
came from. Every contribution must honor the evidence boundary above.

Before claiming a milestone, run the source-only build, the full ROM build, and
the aggregate test suite. “Byte closure” means a zero-fallback, byte-identical
build. “C decompilation” additionally requires the corresponding regions to be
compiled from C.
