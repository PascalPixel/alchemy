**Expanded compiler releases and GCC history — 10 October 2026**

This extends the broad compiler matrix and the 2.95.2 Thumb source-rewrite
trial. The first matrix did not test every period point release or Red Hat
package. This follow-up closes those named gaps and searches actual upstream
commits around the observed matching transitions. It changes no production
compiler, source, routing, rule or credit.

The early ARM result is a family result: **2.95, 2.95.1, 2.95.2 and 2.95.2.1
each match both tested routines in all 18 edition cases with the same options**. The
original emphasis on 2.95.2 did not establish that patch release uniquely.
Eighteen cases are six glyph editions plus twelve metatile editions, not
eighteen independent functions. No tested release or vendor package supplies
one unmodified compiler for all game and library code.

The practical evidence still supports three roles in one Makefile: the
mid-2000 Thumb family for game code, early 2.95 ARM for the demonstrated
routines, and the agbcc lineage for sound/flash libraries. This is a
compatibility recommendation, not adoption of a new route or a proof that
another source reconstruction could never consolidate them. TLA's two existing
backend extensions remain a separate obstacle to an entirely stock toolchain.

**Release inventory.** The period is 1998 through 2002, with TLA's Japanese
`020610172024` build stamp as the last plausible date for that original build.
The official [GNU release chronology](https://gcc.gnu.org/releases.html)
lists the following. EGCS and GCC were parallel lines before GCC 2.95.

| Year | Official releases |
|---|---|
| 1998 | GCC 2.8.0, 2.8.1; EGCS 1.0.1, 1.0.2, 1.0.3, 1.1, 1.1.1 |
| 1999 | EGCS 1.1.2; GCC 2.95, 2.95.1, 2.95.2 |
| 2000 | No new release in that chronology; the GNU archive also contains the 2.95.2.1 point update |
| 2001 | GCC 2.95.3, 3.0, 3.0.1, 3.0.2, 3.0.3 |
| 2002, before TLA's stamp | GCC 3.0.4, 3.1 |
| 2002, after TLA's stamp | GCC 3.1.1, 3.2, 3.2.1; listed, not tested as candidates for the earlier build |

GCC 2.95.2 is the October 1999 maintenance release after 2.95 and 2.95.1;
the `.2` is an ordinary point-release number. The
[official 2.95.2.1 core patch](https://gcc.gnu.org/pub/gcc/releases/gcc-2.95.2.1/)
changes only version/checksum/changelog metadata, not C compiler logic. Both
ARM and Thumb backends are nevertheless rebuilt and measured independently in
the final matrix, so source equivalence is not substituted for a compiler run.
It reproduces 2.95.2's measured scores: 18/18 ARM edition cases, 2/145 TBS
Thumb objects, 0/121 TLA Thumb objects and 29/68 library objects.

Version banners can obscure this list. The tested EGCS 1.0.1/1.0.2/1.0.3
sources identify themselves as `egcs-2.90.23`, `egcs-2.90.27` and
`egcs-2.90.29`; EGCS 1.1/1.1.1/1.1.2 use `egcs-2.91.57`, `egcs-2.91.60`
and `egcs-2.91.66`. They are not six additional untested official releases.
EGCS 1.0 and GCC 2.7.2.3 precede the chosen period, in 1997; they were not
built in this study.

**Development and vendor names need exact identities.** GNU never released
2.96 as an official release; [GNU's contemporaneous explanation](https://gcc.gnu.org/gcc-2.96.html)
describes the development label. The upstream July snapshot and Red Hat's
patched packages are different candidates. We built Red Hat Linux 7.0's
`2.96-54`, 7.1's `2.96-81`, 7.2's `2.96-98` and 7.3's `2.96-110`, from their
original source RPMs in [Red Hat's archive](https://legacy.redhat.com/pub/redhat/linux/).
All four start from the July 31 base and apply different vendor patches.
This samples the shipped distributions; it is not every subsequent erratum RPM.

The February 11, 2001 snapshot in the original matrix identifies itself as
`2.97 20010211 (experimental)`: 2.97 was already represented. The added 2.95.4
candidate is upstream branch commit
`427608c634a3a508ca0f6cf00cb33749b5d8ea9a`, dated September 18, 2001, the last
branch commit before October 3. Its version string is
`2.95.4 20010319 (prerelease)`. It is not an official release and is not the
separately patched Debian `2.95.4.ds10` package. That Debian package remains
untested. The ARM-specific 2.9-arm lineage represented by agbcc is a separate
control, not another official GNU point release.

**Comparison protocol.** The frozen corpus, assembler, preprocessed tokens,
source strata and design/validation assignment are unchanged from the broad
study: 145 TBS Thumb objects, 121 TLA Thumb objects and 68 library objects.
The complete object comparison includes nonempty allocated sections, flags,
alignment, sizes, bytes and symbolic relocations. The separate ARM comparison
includes complete text, literal pools and jump tables in each edition.

Missing releases and vendor candidates get the same whole-class option grid
and design-selected setting, followed by the previously held-out modules.
Once these scores guide a history search, its results are exploratory;
repeatedly consulting this corpus does not create a new independent holdout.
Existing C was shaped for the current compilers, so compatibility is not
historical identification. The separate source-rewrite trial addresses that
bias for 2.95.2 Thumb.

**Commit search.** The fetched public graph contains 28,420 mainline commits
dated 1998-01-01 through 2002-06-10 17:20:24 UTC, plus 804 additional commits
on the 2.95 branch, 2,470 on the 3.0 branch and 1,369 on the 3.1 branch.
The same union has 536 commits touching `gcc/config/arm`. These are inventoried
commits, not a claim that 33,063 compilers were built. The build-stamp timezone is not known; the UTC
cutoff is the study's explicit date convention.

Three bisections consider all intervening commits in their ancestry intervals:
559 between June 30 and July 31, 663 between July 31 and August 31, and 133
between the actual branch parents of the 2.95.2/2.95.3 release tags. They do
not filter to ARM files: common optimizers can change ARM/Thumb output too.
The local sweeps cover 71 July commits (50 distinct compiler-input manifests),
22 August commits (11 manifests), and five consecutive states around the ARM
transition. These are local intervals, not exhaustive compilation of the whole
fetched graph.

Matching is not monotonic. A bisection can locate a local transition, not
prove that every earlier/later commit is worse or identify a global optimum.
Build failures are recorded separately, and neighbourhood sweeps retain each
commit while grouping only identical C compiler input manifests. Tests,
documentation, translations and other frontends may be omitted from those
manifests; C sources, all target configuration, build inputs, libiberty and
the version string remain. Group aliases are not extra measurements.

The August sample boundary is between
`f95b2ff93ebac90f3b3f993defdefdb67257a909` and
[`bc82c87cad5bfd662ec03441ee2befe9cfcccd42`](https://github.com/gcc-mirror/gcc/commit/bc82c87cad5bfd662ec03441ee2befe9cfcccd42),
on August 22, 2000: 145 to 139 exact TBS objects and 55 to 54 TLA objects
at the frozen profiles. The latter removes an alias-analysis assumption that
the incoming structure-result and static-chain registers point to stack
memory. No ARM scheduler file changes in that commit.

The ARM boundary is between
`8edf278dda97e099a5c5eaed7d62a6117df461c3` and
[`d8c7d0e6a9a4af99b1fb205d0cd2760ea7ff7ab6`](https://github.com/gcc-mirror/gcc/commit/d8c7d0e6a9a4af99b1fb205d0cd2760ea7ff7ab6),
on January 4, 2000, on the 2.95 maintenance branch. Exact ARM cases drop from
18 to 12: every glyph edition changes, while every metatile edition stays
exact. That commit changes CSE's handling of jumps/unreachable code and moves
dead-instruction cleanup after jump optimization. Its other target change is
m68k-specific. This is an optimizer-pass boundary, not evidence that changing
only the ARM scheduler is the missing solution. Two neighbouring states before
the transition score 18; the transition and two following states score 12.

Independent builds of both transition pairs, using fresh extracted source
trees and fresh build directories, reproduce these same scores: Thumb
145/55 before and 139/54 after; ARM 18 before and 12 after. The attribution
therefore does not rest only on incrementally rebuilt compiler executables.

The June–July bisection exposes why a simple binary-search verdict is unsafe.
It ends at July 31 commit
[`22251f806bda549a6d6a740a738ad0ca405a4f52`](https://github.com/gcc-mirror/gcc/commit/22251f806bda549a6d6a740a738ad0ca405a4f52):
141 exact objects plus four compiler crashes become 145 exact objects.
The change guards a null expression in `validate_replace_rtx_1`; it recovers
from a temporary compiler failure. Four separate July 17 revisions also fail
to build during an upstream loop-pass edit. Those failures never count as
negative capability evidence. The neighbouring sweep is needed to distinguish
this recovery from earlier matching states. The completed sweep finds 144
exact objects before the July 30 GCSE change `e251e2a27f3a`, then 141 exact
objects plus four compiler crashes, and 145 after the July 31 fix. These
plateaus hold over all 50 distinct compiler-input states in the local interval.
One temporary Makefile revision, `b3e247b8117c`, races while generating its
parsers with parallel make; serial make succeeds and scores 144. The original
failure and an initial retry-harness PATH error remain separate setup records.

An August 22 pre-boundary compiler, `b413d64dedee926fe22855d70647197ab0c7522c`,
also passed all 145 sampled TBS objects. On all 1,931 existing game Thumb
objects it scores 674/676 TBS and 875/1,255 TLA, versus stock July's 675/676
and 875/1,255. The August TBS differences are `battle/turn.c` and
`battle/command.c`; July's sole difference was the separately studied
`iriguchi` hash-order case. Thus the sample does not identify July 31 uniquely,
and the broader check does not justify replacing July with August. These
whole-corpus checks remain compatibility measurements on existing C.

The earliest unified backend was tested too: the April 8 merge commit
`d5b7b3ae33022fb55255375d3ec9745217d95c96` and April 30 commit
`82c3a53ec9b06b9bce83135f6afc27c1e54d0e7d`. Their fixed-profile TBS scores
are 47/145 and 70/145, each with one compiler error, and neither matches an
ARM edition case in the whole-ARM option grid. They do not bridge the two
families with the frozen C. May 31 remains a documented upstream build
failure. The short-leaf lr-save mechanism studied in the Thumb rewrite trial
enters mainline with the April 8 unified-backend merge; it is absent from the
older separate Thumb backend.

**Host and source integrity.** Compiler target sources are untouched. Builds
use the pinned 32-bit Debian environment, a July 2000 native host compiler and
Bison 1.28 described by the parent study. Original vendor `%prep` recipes
apply the vendor's own patches. Modern RPM requires changing its obsolete
`Copyright:` metadata field to `License:` and allowing the old patch tool's
default fuzz of two; neither changes a compiler patch. GCC's own update
script repairs generated-file timestamps after checkout.

GCC 2.8 and EGCS 1.0 have an unused bytecode emitter rejected by the period
host's `va_arg(float)` check; only that host object is compiled by GCC 10.
Old libiberty mistakes modern linker deprecation warnings for missing libc
symbols; `--no-warnings` is used for that host link probe. These are recorded
host-build accommodations, not scheduler or allocator patches. The host's
case-insensitive filesystem cannot faithfully represent unused MIPS libffi
`.S`/`.s` pairs; compiler-input integrity checks cover the ARM C build inputs.

Compiler history, vendor source/patches and private reference objects stay
outside the game repository and the shareable evidence bundle. No ROM input
feeds compilation, no candidate output is rewritten, and no production route
is selected from these scores.

Validation: the five study-driver tests pass, the clean boundary rebuilds
reproduce the reported changes, and `make compare-editions` prints OK for
all twelve unchanged production editions. No experimental candidate is adopted.

**Complete release/snapshot matrix.** Thirty-one source versions built into 40 ARM/Thumb compiler executables, excluding controls and the additional history revisions. Twenty-six have a Thumb frontend; the five earliest ARM-only releases are in the ARM table. May 31 is the additional failed build. Counts below include compilation/assembly errors in the denominator and report them separately. The exact design-selected options are in the retained TSV.

| Compiler | TBS design / validation | TLA design / validation | Errors TBS / TLA |
|---|---:|---:|---:|
| EGCS 1.1 | 0/73; 0/72 | 0/55; 0/66 | 2 / 1 |
| EGCS 1.1.1 | 0/73; 0/72 | 0/55; 0/66 | 3 / 1 |
| EGCS 1.1.2 | 0/73; 0/72 | 0/55; 0/66 | 3 / 1 |
| GCC 2.95 | 1/73; 1/72 | 0/55; 0/66 | 0 / 0 |
| GCC 2.95.1 | 1/73; 1/72 | 0/55; 0/66 | 0 / 0 |
| GCC 2.95.2 | 1/73; 1/72 | 0/55; 0/66 | 0 / 0 |
| GCC 2.95.2.1 | 1/73; 1/72 | 0/55; 0/66 | 0 / 0 |
| GCC 2.95.3 | 1/73; 1/72 | 0/55; 0/66 | 0 / 0 |
| GCC 2.95.4 prerelease | 1/73; 1/72 | 0/55; 0/66 | 0 / 0 |
| 2000-06-30 | 70/73; 66/72 | 27/55; 26/66 | 0 / 0 |
| 2000-07-31 | 73/73; 72/72 | 27/55; 28/66 | 0 / 0 |
| 2000-08-31 | 14/73; 11/72 | 11/55; 12/66 | 0 / 0 |
| 2000-09-30 | 11/73; 5/72 | 8/55; 8/66 | 0 / 0 |
| 2001-02-11 (2.97) | 7/73; 2/72 | 6/55; 4/66 | 2 / 1 |
| GCC 3.0 | 6/73; 3/72 | 5/55; 5/66 | 0 / 0 |
| GCC 3.0.1 | 6/73; 3/72 | 5/55; 5/66 | 0 / 0 |
| GCC 3.0.2 | 6/73; 3/72 | 5/55; 5/66 | 0 / 0 |
| GCC 3.0.3 | 6/73; 3/72 | 5/55; 5/66 | 0 / 0 |
| GCC 3.0.4 | 6/73; 3/72 | 5/55; 5/66 | 0 / 0 |
| GCC 3.1 | 1/73; 2/72 | 1/55; 1/66 | 2 / 0 |
| Red Hat 2.96-54 | 60/73; 64/72 | 26/55; 27/66 | 0 / 0 |
| Red Hat 2.96-81 | 28/73; 31/72 | 23/55; 22/66 | 0 / 0 |
| Red Hat 2.96-98 | 27/73; 31/72 | 23/55; 22/66 | 0 / 0 |
| Red Hat 2.96-110 | 27/73; 31/72 | 23/55; 22/66 | 0 / 0 |
| 2000-04-08 unified merge | 26/73; 21/72 | 19/55; 21/66 | 1 / 0 |
| 2000-04-30 | 34/73; 36/72 | 22/55; 24/66 | 1 / 0 |

**Library option grid.** These are complete-module matches under the best whole-class setting in the same six-profile library grid; this grid is exploratory. Repeated game/library sources are not independent samples. Early 2.95 remains closer than the unified backend but does not replace agbcc's demonstrated sound/flash coverage.

| Compiler | Sound exact / 26 | Flash exact / 42 | Best sound / flash options |
|---|---:|---:|---|
| EGCS 1.1 | 8 | 2 | lib-O2-iw / lib-O2-iw |
| EGCS 1.1.1 | 8 | 2 | lib-O2-iw / lib-O2-iw |
| EGCS 1.1.2 | 8 | 2 | lib-O2-iw / lib-O2-iw |
| GCC 2.95 | 21 | 6 | lib-O2-iw / lib-O2-iw |
| GCC 2.95.1 | 21 | 6 | lib-O2-iw / lib-O2-iw |
| GCC 2.95.2 | 21 | 8 | lib-O2-iw / lib-O1-iw |
| GCC 2.95.2.1 | 21 | 8 | lib-O2-iw / lib-O1-iw |
| GCC 2.95.3 | 21 | 8 | lib-O2-iw / lib-O1-iw |
| GCC 2.95.4 prerelease | 21 | 8 | lib-O2-iw / lib-O1-iw |
| 2000-06-30 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| 2000-07-31 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| 2000-08-31 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| 2000-09-30 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| 2001-02-11 (2.97) | 1 | 0 | lib-O2-iw / lib-O2-iw |
| GCC 3.0 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| GCC 3.0.1 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| GCC 3.0.2 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| GCC 3.0.3 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| GCC 3.0.4 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| GCC 3.1 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| Red Hat 2.96-54 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| Red Hat 2.96-81 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| Red Hat 2.96-98 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| Red Hat 2.96-110 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| 2000-04-08 unified merge | 1 | 0 | lib-O2-iw / lib-O2-iw |
| 2000-04-30 | 1 | 0 | lib-O2-iw / lib-O2-iw |
| agbcc control | 26 | 42 | O2 interwork / O1 interwork |

**ARM option grid.** Each row uses one setting for all 18 edition cases. Every setting includes `-mthumb-interwork -fomit-frame-pointer`; `r4` means `-fcall-used-r4`, `noframe` adds `-mno-apcs-frame`, and `fixedfp` adds `-ffixed-fp`. Unified candidates additionally use `-marm -mcpu=arm7tdmi`. Equal results across repeated editions do not establish 18 independent functions.

| Compiler | Glyph exact / 6 | Metatile exact / 12 | Best whole-ARM setting |
|---|---:|---:|---|
| GCC 2.8.0 | 0 | 0 | O2-r4-iw (0 errors) |
| GCC 2.8.1 | 0 | 0 | O2-r4-iw (0 errors) |
| EGCS 1.0.1 | 0 | 0 | O2-r4-iw (0 errors) |
| EGCS 1.0.2 | 0 | 0 | O2-r4-iw (0 errors) |
| EGCS 1.0.3 | 0 | 0 | O2-r4-iw (0 errors) |
| EGCS 1.1 | 0 | 0 | O2-r4-iw (0 errors) |
| EGCS 1.1.1 | 0 | 0 | O2-r4-iw (0 errors) |
| EGCS 1.1.2 | 0 | 0 | O2-r4-iw (0 errors) |
| GCC 2.95 | 6 | 12 | O2-r4-iw (0 errors) |
| GCC 2.95.1 | 6 | 12 | O2-r4-iw (0 errors) |
| GCC 2.95.2 | 6 | 12 | O2-r4-iw (0 errors) |
| GCC 2.95.2.1 | 6 | 12 | O2-r4-iw (0 errors) |
| GCC 2.95.3 | 0 | 12 | O2-r4-iw (0 errors) |
| GCC 2.95.4 prerelease | 0 | 12 | O2-r4-iw (0 errors) |
| 2000-06-30 | 0 | 0 | O2-r4-iw (0 errors) |
| 2000-07-31 | 0 | 0 | O2-r4-iw (0 errors) |
| 2000-08-31 | 0 | 0 | O2-r4-iw (0 errors) |
| 2000-09-30 | 0 | 0 | O2-r4-iw (0 errors) |
| 2001-02-11 (2.97) | 0 | 0 | O2-r4-iw (0 errors) |
| GCC 3.0 | 0 | 0 | O2-r4-iw (0 errors) |
| GCC 3.0.1 | 0 | 0 | O2-r4-iw (0 errors) |
| GCC 3.0.2 | 0 | 0 | O2-r4-iw (0 errors) |
| GCC 3.0.3 | 0 | 0 | O2-r4-iw (0 errors) |
| GCC 3.0.4 | 0 | 0 | O2-r4-iw (0 errors) |
| GCC 3.1 | 0 | 0 | O2-r4-iw (0 errors) |
| Red Hat 2.96-54 | 0 | 0 | O2-r4-iw (0 errors) |
| Red Hat 2.96-81 | 0 | 0 | O2-r4-iw (0 errors) |
| Red Hat 2.96-98 | 0 | 0 | O2-r4-iw (0 errors) |
| Red Hat 2.96-110 | 0 | 0 | O2-r4-iw (0 errors) |
| 2000-04-08 unified merge | 0 | 0 | O2-r4-iw (0 errors) |
| 2000-04-30 | 0 | 0 | O2-r4-iw (0 errors) |
| agbcc_arm control | 1 | 12 | O2-r4-iw (0 errors) |
| agscc ARM control | 0 | 0 | O2-r4-iw (0 errors) |

**Limits and retained evidence.** This exhausts the named official releases in the stated period through GCC 3.1; it does not exhaust every vendor erratum, Debian package or development commit. It does not identify Camelot's exact executable or rule out all possible C rewrites. The result is a smaller, evidence-backed family choice and specific upstream transitions to explain, with no justification here for a new target compiler patch.

The complete local experiment remains in `/private/tmp/alchemy-compiler-study`. The source/evidence bundle under `out/compiler-study/vershist/` retains the authored Rust drivers, build recipes, archive/commit pins, frozen inventory, option selections, per-object result TSVs, history searches and clean-build results. It excludes GCC source and patches, compiler executables, generated assembly, ROMs and private reference objects. `HISTORY-REPRODUCE.md` gives the commands and host accommodations. Production compiler choices remain unchanged.
