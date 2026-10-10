**Alchemy compiler study — 10 October 2026**

Completed experiment matrix; recommendation, not a production toolchain change.

The evidence narrows the search to the mid-2000 GCC 2.96 Thumb family, the
older ARM/Thumb lineage represented by agbcc for third-party libraries, and
early stock GCC 2.95 releases for the two compiler-shaped ARM routines. It does
not establish one unmodified GNU compiler that explains both games. A
single Makefile can express these choices without function-specific routing.
No production compiler, option, rule or game source has changed in this study.

The subsequent [2.95.2 Thumb source-rewrite trial](thumb295.md) changes the C
in twelve modules and checks an independent short-leaf stack pattern. It finds
no complete module match and a concrete backend reason to retain July-era GCC
for Thumb; the positive 2.95.2 ARM result remains unchanged.

The [expanded release and commit-history study](vershist.md) closes the missing
point-release and Red Hat package tests. It finds the same ARM matches with
2.95, 2.95.1 and 2.95.2.1, then identifies the January 2000 CSE change that breaks the
glyph cases. A separate August 2000 alias-analysis change marks a local Thumb
compatibility boundary. Fresh builds reproduce both transitions.

This study reopens the previous compiler verdict. Compatibility with our C
is measured separately from historical provenance: much of that C was shaped
under the existing compilers. A high score identifies a useful reconstruction
toolchain; it does not recover Camelot's original executable or build system.

**What was tested.** Public, pinned GNU sources: EGCS 1.1.2; GCC 2.95.2,
2.95.3, 3.0, 3.0.4 and 3.1; and upstream snapshots dated 2000-05-31,
2000-06-30, 2000-07-31, 2000-08-31, 2000-09-30 and 2001-02-11.
The exact July revision is `04179d4a511b13cef92eacdb10a51bcd124fea7a`.
Agscc and agbcc are labelled controls, not stock GNU releases. GCC 2.96 was
a development version, so the date/revision is part of the compiler's name.
[GNU's explanation](https://gcc.gnu.org/gcc-2.96.html) and
[release chronology](https://gcc.gnu.org/releases.html) establish the public
timeline. Later releases are negative controls for TBS; they cannot explain
an earlier build. GCC 3.1 predates TLA's in-image `020610172024` build stamp.

Eleven public source versions built successfully, producing fourteen stock
ARM/Thumb frontends. The compilers were built from untouched source in a pinned 32-bit Debian
container. GCC 10 built a native July 2000 host compiler, which then built
the releases requiring obsolete GNU C lvalues. Its original standard headers
were generated with its own Makefile. Bison 1.28 generated snapshot parsers.
The comparison compiler sources received no scheduler, allocator, ARM or
Thumb patches. Host configuration, logs, archive hashes and executable hashes
are retained. One deliberately modified July build is a separate diagnostic
for agscc's content hashing; it is never included among the stock candidates.

The May 31 snapshot fails to build because its generated `gen_untyped_call`
passes too many arguments to `gen_call`. It is a build failure, not a failed
matching result. Initial modern-host build failures of the later releases
were resolved with the period host compiler. EGCS and 2.95 Thumb use their
supported `thumb-coff` target; the unified backend uses `arm-elf` with
`-mthumb` or `-marm`. GNU announced the unified backend on
[May 1, 2000](https://gcc.gnu.org/news.html).

**Frozen comparison.** Before scoring the new candidates, the current English
builds yielded an inventory of 2,001 C objects and a stratified sample of 336:
145 TBS game Thumb objects, 121 TLA game Thumb objects, 68 library objects and
two metatile ARM objects. Whole parent modules were deterministically assigned
to design or validation. Sampling covers subsystem, object size and ordinary
versus FAKEMATCH/asm-bearing source. The manifest SHA-256 is
`8549bca704e4a663e96603ab293fa14dc5f3391ecc9deacd0b528f08add5747c`.

The design halves contain 73 TBS and 55 TLA objects; validation contains 72
and 66. Options were selected on design modules before validation. The grid
uses O1/O2/Os, interworking, call-used-r4 and ARM7/ARM9 scheduling where the
backend supports them. Each setting applies to a whole code class. The library
grid is explicitly exploratory because those complete modules have no independent
holdout. Repeated editions and shared sources are not independent samples.

Every candidate consumes the same frozen preprocessed tokens and is assembled
with the same assembler as its control. Equality covers all nonempty allocated
sections, their alignment/size/bytes, and symbolic relocations. It is stronger
than a count of similar instructions, but is not a complete linked-ROM adoption
test. Errors, instruction differences and other section/relocation differences
are reported separately. The scripts never rewrite compiler output.

All 266 sampled game Thumb controls and all 68 sampled library controls match.
An expanded control also matches all 1,931 inventoried game Thumb C objects:
676 TBS and 1,255 TLA. This expanded run is a regression check, not another
held-out sample.

| Stock compiler | TBS design / validation | TLA design / validation |
|---|---:|---:|
| EGCS 1.1.2 | 0/73; 0/72 | 0/55; 0/66 |
| GCC 2.95.2 | 1/73; 1/72 | 0/55; 0/66 |
| GCC 2.95.3 | 1/73; 1/72 | 0/55; 0/66 |
| 2000-06-30 | 70/73; 66/72 | 27/55; 26/66 |
| 2000-07-31 | 73/73; 72/72 | 27/55; 28/66 |
| 2000-08-31 | 14/73; 11/72 | 11/55; 12/66 |
| 2000-09-30 | 11/73; 5/72 | 8/55; 8/66 |
| 2001-02-11 | 7/73; 2/72 | 6/55; 4/66 |
| GCC 3.0 | 6/73; 3/72 | 5/55; 5/66 |
| GCC 3.0.4 | 6/73; 3/72 | 5/55; 5/66 |
| GCC 3.1 | 1/73; 2/72 | 1/55; 1/66 |

**Independent binary evidence.** Using Alchemy's ELF, map and Thumb decoding
code, the study scans symbol-bounded functions in the verified English main
images and overlays. Literal pools, jump tables and assembler data mappings
are excluded. A raw halfword `0xf800` is counted as a call only when it follows
`mov lr, Rn` inside the same function. These are bounded observed counts, not
claims to have decoded every executable byte. Unreconstructed main-image code
can include library routines whose ownership is not yet identified.

| Observed pattern in game-labelled code | TBS | TLA |
|---|---:|---:|
| Decoded Thumb instructions | 465,532 | 750,665 |
| Pushes saving lr and at least one low register | 2,838 | 4,242 |
| Of those, first saved register is r5 | 2,838 | 4,239 |
| Adjacent mov / shift / add constant sequences | 0 | 8,064 |
| mov lr / lone BL-suffix calls | 0 | 2,495 |
| Calls to named call-via-register stubs | 761 | 7 |

This strongly supports `-fcall-used-r4` for game Thumb code. The three TLA
push exceptions are in the sound-library neighbourhood. Four of its seven
stub calls are explicit ARM-kernel loaders, and three are in that same sound
neighbourhood. Thus the earlier blanket statement that TLA no longer interworks
with ARM is too strong; the dominant game indirect-call convention differs.

The same ordinary-C probes and target descriptions are checked across candidates.
The tested Thumb backends route an ordinary indirect call through a stub and
materialize a non-shiftable constant such as 301 through a pool load. The relevant
machine-description alternatives do not implement TLA's general lone-suffix
call convention or its additional three-instruction constant split. The binary
pattern is strong evidence for a different code-generation convention. It is
not proof that a particular Camelot employee modified GCC: a vendor compiler
or an original source/assembly layer remains an alternative. No recovered
compiler provenance is claimed.

This is also not explained by overlay call restoration: 2,031 of the observed
TLA suffix calls are in the resident image, and Alchemy's loader restoration
only adjusts an existing BL-prefix/BL-suffix pair. The period binutils 2.10
ARM ELF backend uses the generic relaxation hook and has no call-via-stub to
lr-suffix rewrite. Its documented interworking operation generates veneers.
[GNU ld 2.10 documentation](https://sourceware.org/binutils/docs-2.10/ld_4.html)
supports that distinction. A hypothetical vendor post-link optimizer is not
ruled out by this compiler matrix, but no evidence for one was found and none
is introduced into the reconstruction.

**Stock versus agscc.** Stock July GCC matches 145/145 sampled TBS game objects,
then 675/676 in the expanded regression. All 429 objects whose source lacks
FAKEMATCH/asm markers match. The exception is `goma_iriguchi/iriguchi.c`, which
has a marker elsewhere in its translation unit. Stock emits two constructions
of the same argument constant where agscc reuses a register. Its text stays
3,440 bytes; the extra instruction shifts subsequent bytes, changing 239 byte
positions in the object text.

The stock result is repeatable. A diagnostic build changing only the symbol/label
hash in `simplify-rtx.c` to agscc's content hash reproduces agscc's assembly for
that object exactly and matches all 676 TBS objects in the expanded check.
This is a host reproducibility change with an observable
code-generation effect; calling the resulting compiler completely equivalent
to stock would be inaccurate. The retained source audit finds the two explicit
TLA target extensions separately from host ABI, floating-constant representation,
garbage-collector, hashing and build adaptations. No new compiler patch is proposed.

Stock July with TLA's best stock setting matches 55/121 sampled game objects
and 875/1,255 in the expanded regression. The current agscc control matches all
1,255. The smaller sample intentionally over-represents substantial functions;
the full inventory includes small code and data-only objects. Neither fraction
is a percentage of the whole game's code, and neither proves that the remaining
source could not be rewritten for stock GCC.

Compiling those same 1,255 TLA objects with the installed agscc but with both
reconstructed options off gives exactly the same 875 matches. Thus the two
extensions account for the matching-count gain in this corpus; host-port
differences do not account for it. Stock July also matches all seven current
libgcc C members with their actual runtime flags, including GCC's own
`-fno-inline`. The runtime does not require another compiler.

| Stock compiler | Sound exact / 26 | Flash exact / 42 | Best sound / flash options |
|---|---:|---:|---|
| EGCS 1.1.2 | 8 | 2 | `-O2 -mthumb-interwork` / `-O2 -mthumb-interwork` |
| GCC 2.95.2 | 21 | 8 | `-O2 -mthumb-interwork` / `-O -mthumb-interwork` |
| GCC 2.95.3 | 21 | 8 | `-O2 -mthumb-interwork` / `-O -mthumb-interwork` |
| 2000-06-30 | 1 | 0 | `-O2 -mthumb-interwork` / `-O2 -mthumb-interwork` |
| 2000-07-31 | 1 | 0 | `-O2 -mthumb-interwork` / `-O2 -mthumb-interwork` |
| 2000-08-31 | 1 | 0 | `-O2 -mthumb-interwork` / `-O2 -mthumb-interwork` |
| 2000-09-30 | 1 | 0 | `-O2 -mthumb-interwork` / `-O2 -mthumb-interwork` |
| 2001-02-11 | 1 | 0 | `-O2 -mthumb-interwork` / `-O2 -mthumb-interwork` |
| GCC 3.0 | 1 | 0 | `-O2 -mthumb-interwork` / `-O2 -mthumb-interwork` |
| GCC 3.0.4 | 1 | 0 | `-O2 -mthumb-interwork` / `-O2 -mthumb-interwork` |
| GCC 3.1 | 1 | 0 | `-O2 -mthumb-interwork` / `-O2 -mthumb-interwork` |
| old_agbcc control | 26 | 42 | `-O2 -mthumb-interwork` / `-O -mthumb-interwork` |

The closest stock library candidate, 2.95.2, remains 29/68 exact even after
the uniform library grid. The earlier all-edition library trial agrees:
174/408 objects exact, against 408/408 for old_agbcc. Many flash failures are
unused address words in a different allocated section, not a large instruction
mismatch; omitting those bytes is still a failed reconstruction. Sound's
`sound.c` and `engine.c`, and flash's `program_atmel_flash_block.c`, also have
instruction differences. The separate library compiler is supported by both
the library byte patterns and compatibility, not merely by its version label.
The agbcc source identifies itself as `2.9-arm-000512`; it must not be renamed
"stock GCC 2.95.2" or treated as an untouched GNU release.

**ARM evidence has only two independent routines.** The earlier glyph experiment
is retained alongside the broad corpus: six glyph edition objects and twelve
metatile edition objects. Those are two routines, not eighteen independent
benchmarks. The sources were shaped during that earlier study. The comparison
uses the same six whole-ARM option sets across candidates, including APCS frame
and fp allocation variants. It compares complete text, including pools and
jump tables, with one modern assembler on both sides.

The five older ARM benchmark entries now classified as handwritten assembly
are not silently recycled as compiler mismatches. Their actual evidence is
separate: use of both multiply halves outside a GCC DI register pair, block
transfers with arithmetic-modified registers/writeback, or a copy-and-test
instruction form. Rechecking the recommended stock 2.95.2 backend supports those
exclusions: `mulsidi3` uses an early-clobber DI destination and adjacent `%Q0/%R0`
registers; its high-half pattern discards the low result. Its copy/test pattern
uses `subs #0`. Its block-copy expander starts at r0, and the ordinary multiple
load/store peepholes do not add writeback; the other writeback forms are register
pairs or stack saves/restores. These observations agree with the current
handwritten classifications for that candidate. They are not an exhaustive
incapability proof for every public or vendor compiler, nor are the five routines
counted as wins for 2.95.2.

| Compiler | Glyph exact / 6 | Metatile exact / 12 | Best whole-ARM setting |
|---|---:|---:|---|
| EGCS 1.1.2 | 0 | 0 | O2-r4-iw |
| GCC 2.95.2 | 6 | 12 | O2-r4-iw |
| GCC 2.95.3 | 0 | 12 | O2-r4-iw |
| 2000-06-30 | 0 | 0 | O2-r4-iw |
| 2000-07-31 | 0 | 0 | O2-r4-iw |
| 2000-08-31 | 0 | 0 | O2-r4-iw |
| 2000-09-30 | 0 | 0 | O2-r4-iw |
| 2001-02-11 | 0 | 0 | O2-r4-iw |
| GCC 3.0 | 0 | 0 | O2-r4-iw |
| GCC 3.0.4 | 0 | 0 | O2-r4-iw |
| GCC 3.1 | 0 | 0 | O2-r4-iw |
| agbcc_arm control | 1 | 12 | O2-r4-iw |
| agscc ARM control | 0 | 0 | O2-r4-iw |

In this table `O2-r4-iw` includes `-fomit-frame-pointer`; it names the first
setting among ties, not evidence that every other setting performed worse.

Only stock 2.95.2 reaches 18/18 with this common source and one ordinary setting.
Stock 2.95.3 reaches 12/18, all metatile; agbcc_arm reaches 13/18, adding the
Japanese glyph. The exact setting is `-O2 -mthumb-interwork
-fomit-frame-pointer -fcall-used-r4`. Removing the APCS frame or reserving fp
does not improve the newer backends to an exact object. This supports replacing
the existing ARM executable with 2.95.2 without increasing the compiler count;
it does not prove that two routines identify the historical compiler uniquely.
The separate earlier integration trial built all twelve ROMs byte-identically
with this ARM choice and simpler glyph C. That proposal remains unadopted.

One September ARM capability probe crashes in `single_set_1` with
`-mno-apcs-frame`; that is recorded as a compiler error, not counted as proof
of an absent instruction pattern. All tested Thumb capability probes compile
successfully and retain the stub call and pool-loaded 301.

**What the study does not establish.** It does not identify an exact original
executable from byte similarity, prove that no untested public or vendor snapshot
could do better, or prove a mathematical minimum over every possible C rewrite.
There is no per-function compiler search, no source-shaping search in this broad
matrix, and no credit awarded from these reports. The preprocessing is frozen
to today's production contract; a production replacement must also validate its
own preprocessor macros/headers and all twelve linked ROMs. Complete symbol tables
and full linker layout still belong to those ROM checks.

**Recommended minimum for the reconstruction: three C compiler executables,
six explicit configurations, one build entry point.** This replaces the current
ARM compiler within the existing three-executable arrangement. It adds no
function-specific compiler, option or scheduler patch.

| Owner | Compiler executable | Configuration |
|---|---|---|
| TBS game Thumb | July 31 GCC line, through the current agscc host port | `-O2 -mthumb -mcpu=arm7tdmi -fcall-used-r4 -mthumb-interwork` |
| TLA game Thumb | Same agscc executable | `-O2 -mthumb -mcpu=arm7tdmi -fcall-used-r4 -mtune=arm9tdmi`, plus the two existing reconstructed Thumb options |
| Both games' ARM C | Stock GCC 2.95.2 ARM | `-O2 -mthumb-interwork -fomit-frame-pointer -fcall-used-r4` |
| MusicPlayer2000 | `old_agbcc` | `-O2 -mthumb-interwork` |
| Flash library | Same `old_agbcc` executable | `-O -mthumb-interwork` |
| Shared libgcc C | Same July GCC executable | `-O2 -mthumb -mcpu=arm7tdmi -fno-inline`, ordinary ABI, no interwork |

Preprocessing includes/edition definitions are separate from these code-generation
profiles. `-nostdinc` remains part of the production preprocessing contract.
The two TLA switches are `-mthumb-split-constants` and `-mthumb-call-via-lr`;
they remain reconstruction features, not verified original Camelot modifications.

The compiler count is three distinct C generators, not three checkout directories
or three copies of a wrapper. There is no extra compiler for each language edition.
Whole-game Thumb settings, an ISA-selected ARM group, whole library groups and
the compiler's own runtime recipe fit an ordinary Makefile. Alchemy's existing
one-command build can retain that same fixed table. All GCC source and any host
ports continue to live in the GPL compiler submodule(s), outside main game source.

The practical alternatives are:

| Choice | Evidence and cost |
|---|---|
| One stock July compiler | Best single stock candidate for game Thumb; 675/676 TBS and 875/1,255 TLA C objects, seven runtime members. Fails the complete library and ARM sets. |
| July plus stock 2.95.2 | Covers the tested ARM set as well; still no complete sound/flash match and still the TLA gap. This does not deliver a stock-only reconstruction. |
| The three-executable set above | Preserves the demonstrated Thumb/library routes and uses an unmodified ARM backend that matches both current ARM C routines. This is the smallest demonstrated set with those constraints, not a proof over all possible source rewrites. |

Confidence is high in the July-era Thumb family and in the distinct library
provenance; the exact original snapshot remains unidentified. Confidence in
2.95.2 as a practical ARM choice is high, but historical identification from
only two routines is weak. The evidence for TLA's different instruction patterns
is strong; their original implementation location remains uncertain.

There is no evidence here for another agscc scheduling extension. First pin these
few profiles and remove the demonstrated source devices under stock 2.95.2 ARM.
Keep any remaining TLA scheduling question separate, with a whole-game test.
Further compiler fishing is warranted by a newly observed general fingerprint
or a public candidate implementing one, rather than by one stubborn function.
If "all compilers completely unmodified GNU" is an absolute requirement, the
honest result is that this study has not found a solution satisfying it.

The current source tree contains no uses of the compiler-version, Thumb-mode,
optimization or ARM-architecture predefined macros searched by the study in
`games/*/src` and `games/*/include`. That reduces the frozen-preprocessor concern
for this corpus, but does not remove the need for full driver/build verification
when changing the production toolchain.

**Reproduction.** `PROTOCOL.md` records the design before scoring; `Makefile`
builds the stock candidates; `candidates.tsv` identifies exact revisions.
`corpus.rs` freezes the corpus, `runner.rs` compiles and compares allocated
sections/relocations, and `matrix.rs` selects on design then runs validation.
`fingerprints.rs` uses Alchemy's parsers for the binary inventory. `probe.rs`,
`arm-matrix.rs` and `runtime-prepare.rs` cover the independent probes, ARM
compatibility and compiler runtime. No ROM or compiler-generated object belongs
in the public report or source bundle. Private corpus objects and detailed
addresses remain local study artifacts.

The study used Alchemy glyph `7c99ea6759197b436dd3339b1e8fab3b391375b4`,
agscc `629474adaee19177f64f9f04a3e56ee606c9b235`, and agbcc
`da598c1d918402c42c0c0d7128ba14567f3175e9`. The protocol, authored harness,
source pins, frozen selections and summary TSVs are retained locally under
`out/compiler-study/`, including `study-sources.tar.gz`; the complete private
experiment is `/private/tmp/alchemy-compiler-study`. `REPRODUCE.md` records
the host/container and commands, including the harness's fixed local paths.
The final unchanged production build passed `make compare-editions` for all
twelve editions. No compiler adoption is implied by that check.

**Exact public source pins.** Releases use GNU's original core/full archives;
snapshots use the official GCC mirror at the revisions below. Archive and
compiler executable SHA-256 records are included with the local evidence.

| Candidate | Source |
|---|---|
| gcc20000731 | [gcc-04179d4a511b13cef92eacdb10a51bcd124fea7a](https://codeload.github.com/gcc-mirror/gcc/tar.gz/04179d4a511b13cef92eacdb10a51bcd124fea7a) |
| gcc20000531 | [gcc-e0fa93b3ca30abe9423a3f70cc172c21f2d2fdf0](https://codeload.github.com/gcc-mirror/gcc/tar.gz/e0fa93b3ca30abe9423a3f70cc172c21f2d2fdf0) |
| gcc20000630 | [gcc-e5858cc975401a7ad2db921ab4de1f280765c1f5](https://codeload.github.com/gcc-mirror/gcc/tar.gz/e5858cc975401a7ad2db921ab4de1f280765c1f5) |
| gcc20000831 | [gcc-ccbc3eb78e1b5a23414b3ef0ff7ec3aa811bdfb8](https://codeload.github.com/gcc-mirror/gcc/tar.gz/ccbc3eb78e1b5a23414b3ef0ff7ec3aa811bdfb8) |
| gcc20000930 | [gcc-304daac5d9eaa21727faa45e3e3881608d4ec2a8](https://codeload.github.com/gcc-mirror/gcc/tar.gz/304daac5d9eaa21727faa45e3e3881608d4ec2a8) |
| gcc20010211 | [gcc-1c7c97cc635d4aa08f499bc941918f64502034d3](https://codeload.github.com/gcc-mirror/gcc/tar.gz/1c7c97cc635d4aa08f499bc941918f64502034d3) |
| gcc300 | [gcc-3.0](https://gcc.gnu.org/pub/gcc/releases/gcc-3.0/gcc-core-3.0.tar.bz2) |
| gcc304 | [gcc-3.0.4](https://gcc.gnu.org/pub/gcc/releases/gcc-3.0.4/gcc-core-3.0.4.tar.bz2) |
| gcc310 | [gcc-3.1](https://gcc.gnu.org/pub/gcc/releases/gcc-3.1/gcc-core-3.1.tar.bz2) |
| gcc2952 | [gcc-2.95.2](https://ftp.gnu.org/gnu/gcc/gcc-2.95.2.tar.gz) |
| gcc2953 | [gcc-2.95.3](https://gcc.gnu.org/pub/gcc/releases/gcc-2.95.3/gcc-core-2.95.3.tar.bz2) |
| egcs112 | [egcs-1.1.2](https://gcc.gnu.org/pub/gcc/releases/egcs-1.1.2/egcs-core-1.1.2.tar.bz2) |
