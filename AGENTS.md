# Rules

1. Treat `baserom.gba` as the sole game evidence; never inspect or import another checkout, history, source, symbols, labels, pseudocode, scripts, outputs, or notes.
2. Generic tools and public architecture/compiler documentation are allowed; derive all game knowledge anew.
3. Track independent tools and byte-verified C; keep the ROM, toolchains, dumps, generated analysis, assembly, and build products ignored.
4. Never transmit, paste, upload, or expose ROM bytes or anything derived from them; never give local content to network tools.
5. Work locally, inspect staged names before every commit, and never push this branch.
6. Claim completion only when this branch, given the private ROM and approved compiler, independently builds a byte-identical full ROM; every region called decompiled must be emitted from compiled reconstructed source.
