/*
 * UiText_DrawGlyphArm: all-edition ARM C draft, not linked or credited.
 * Re-derived from the current listing and our ROMs on 2026-10-09.
 * Stock agbcc_arm with K1's ARM options, measured with:
 *   alchemy diff recon/tbs/en/main/080155d0.c moji --game tbs
 *                --edition tbs-en --arm --rtl --show
 * Native / draft / common / residual:
 *   ja: 193 / 193 / 187 / 12
 *   en: 196 / 197 / 188 / 17
 *   de: 187 / 187 / 180 / 14
 *   es: 187 / 187 / 180 / 14
 *   fr: 187 / 187 / 180 / 14
 *   it: 187 / 187 / 180 / 14
 * Frames and spill offsets now agree: Japanese 100 bytes, all others 44.
 * Japanese's 64-byte unused area still needs the tagged source type below.
 * Multiply operands agree. Japanese and the four European editions differ
 * only in scheduling; English also has a window reload and an early range
 * branch. All glyph-header branches and both pixel-write sequences agree.
 * These are instruction-LCS residuals, not a complete-extent byte match.
 * No edition matches or counts; the builds still link the original listing.
 * No register pins, asm barriers, instruction asm or alternative compiler options.
 * Japanese decodes ten 1bpp rows and kanji rows; the other editions decode
 * eight packed rows. English alone rejects character indices outside 32-143.
 * Earlier trials (retained as history, not the current measurements):
 * English direct geometry 325; hoisted row geometry 277; shorter
 * local scopes and do-while loops 248; separate next-pass lifetime 229;
 * integer destination reuse 225. The ten-minute permute search compiled
 * 9,786 distinct candidates: weighted 9,110 -> 6,330, residual 248 -> 215.
 * Its explicit-exit loop forms, applied to the shared edition branches,
 * improve ja/en/other from 226/225/186 to 203/203/156.
 * Rejected trials: direct goto loops (en 270); volatile local stride
 * (en 268); volatile initial height read (ja/en/other 211/209/169);
 * one-pass helper block (en 241). None changes a compiler or its options.
 * packed = 0 added one instruction (en/other 206/157). Instead, the shift
 * is confined to odd columns, avoiding its initial uninitialized read with
 * the same residual counts (en/other 203/156).
 * Saved row predicate plus discarded label leaves residuals unchanged;
 * splitting the pending row pointer shrinks the international frame to 44
 * but worsens ja/en/other to 232/236/195. The label adds no sched2 boundary.
 * With explicit loop exits, a volatile international stride restores the
 * 44-byte frame and gives en/other 186/154 (ja 298, so left plain there).
 * Volatile saved window/rows/result place the four high spills at the native
 * offsets and give en/other 186/153; direct pass++ then worsens them to
 * 199/176 and Japanese to 214, so the next-pass lifetime is retained.
 * Writing the volatile result inside the English character branches restores
 * their separate stores and changes en 186 -> 175, ja/other unchanged.
 * Reintroducing a separate destination pointer internationally worsens
 * en/other to 277/271 with the volatile spill layout; scalar reuse remains.
 * Compiler audit: duplicate successor stores become predicated stores;
 * saved predicates, discarded label addresses and an unused label-address
 * local emit CMP; STR; EOR; BHI, unchanged when reduced to a direct test.
 * They put memory between compare and branch but leave a one-cycle EOR last,
 * so they do not realize the native CMP; LSL; STR; BLS row step. No new
 * stock post-sched2 store-hoisting path was found. Synthetic source and RTL
 * are preserved for review under out/reports/glyph_sched, never linked.
 * Continental permute: 15 minutes, 72,561 distinct candidates, none failed
 * compilation; weighted 5,820 -> 4,620, residual 156 -> 143. Ten minimized
 * rewrites: six independent-statement orders, two declaration orders, one
 * removed result alias and one inverted odd/even-column test. Combining its
 * orders with the volatile spills gives other 183; plain continental locals
 * retain 143 while Japanese/English retain 203/175.
 * Exact nested-loop trial: duplicate a volatile next-pointer store in both
 * row-limit successors. It emits cmp; lsl; bhi; str; b, plus the exit str,
 * and worsens ja/en/other to 239/199/191. It does not resolve store-last.
 *
 * 2026-10-09 glyph branch: the checked-out staging draft measured
 * 203/175/143; the brief's 165/132/90 draft was not in another current local
 * branch or worktree. No deleted files or historical source were consulted.
 * Sharing row, rownext and col between both drawing paths is the largest
 * allocation improvement: ja/en/other 66/106/92 -> 34/76/67 at that step.
 * Scalar destination reuse also works for Japanese with these lifetimes.
 * Writing (x >> 3) * (stride - 2) fixes the MLA operand order and avoids
 * loop.c hoisting stride-2 into another spill. Declaration order gives the
 * native frame slots without volatile. Separate load and shift statements
 * for both 16-bit rows make ip the row bits; with shared counters, other
 * improves 67 -> 61. Ordering packed-font declarations and initialization
 * gives 61 -> 39; separating width from result removes the duplicate width
 * spill, 39 -> 34. Moving Japanese vram initialization between tile_y and
 * row_line gives 34 -> 30. All English volatile devices have been removed.
 *
 * Rejected trials during this attempt (ja/en/other where all measured):
 * - Direct pass++: 34/76/34 -> 81/123/77, five repeated pass reloads.
 * - u16 width: 64/86/42; rows++ then rows[-1]: 89/116/67.
 * - Three statements for tile multiplication/addition: 80/90/48.
 * - Volatile VRAM halfwords: 94/150/134; explicit u32 pixel temporary:
 *   other 92 -> 132; u16 pixel temporary or x &= 3: unchanged.
 * - English volatile saved window: 83; adding saved rows/result: 94;
 *   adding volatile stride: 106; volatile initial height read alone: 82.
 * - English named character-index alias or split range predicate: 76;
 *   duplicating the glyph-base expression for rows and width: 125.
 * - Separate next-column temporary: Japanese 66 -> 79; rownext statement
 *   moves did not improve the shared-counter draft.
 * Five-minute permute runs measured residuals, not just weighted scores:
 * other 96 -> 102 (88,749 candidates), helper-only other 92 -> 158
 * (44,174), Japanese 76 -> 88 (44,156), shared-counter Japanese 34 -> 30
 * (45,992; only the vram statement move retained). The latter's best
 * weighted candidate had residual 36, so was rejected. English's u16-width
 * trial (86; 45,849 candidates) found no improvement. Search output is in
 * ignored out/permute; it never feeds a build or the count.
 *
 * Continuation: sharing the English range-error exit with the final result
 * return changes en 76 -> 65. It restores the packed-font color registers
 * and the source-pointer reload to their native registers, at the cost of
 * one extra predicated result spill. A separate short-lived return alias
 * removes that spill but also loses the improvement (76). The range check
 * still returns 1 without drawing; the other five editions remain unchanged.
 * New rejected trials (based on 30/76/34 unless stated otherwise):
 * - Reuse width as glyph byte offset: 50/90/56; use it as the old row
 *   pointer before dereferencing, or swap width/result roles: unchanged.
 * - Volatile result alone: 72/98/52. Return width directly: 61/103/39,
 *   with native r3 width loads in Europe but a duplicate width store.
 * - One-element result array or initial height copied through width:
 *   unchanged. Subtract then restore character for English's range test:
 *   150; use width for its index: 96.
 * - Range result assignment followed by direct return: 76; put a separate
 *   invalid-character return at the end: 95. Route packed-font returns
 *   through the shared result too: en/other 76/100.
 * - After the shared exit: a two-bit pixel field in GlyphDot gives
 *   267/285/267; u8 shift gives unchanged output. A whole-function inline
 *   wrapper gives 111/140/140 (English 141 with direct range return).
 * - Copy font width through width, or cast its return to u16: unchanged.
 *   Express font offset as (character-32)*32+1024: en 68, other unchanged;
 *   placing that expression after the width lookup gives en 106.
 * - Volatile saved window and rows: 103/146/105. A separate plain rows
 *   temporary and one volatile saved-rows store at the join: 71/113/91.
 *   Moving that store into both glyph branches before the width loads:
 *   91/157/90; precomputing accent in those branches: ja/en 96/162.
 *   These forms get the width load later but disturb allocation elsewhere.
 * - Five-minute English searches from residual 65: weighted and prefix
 *   searches compile 89,731 and 89,730 candidates. Both pick the same color
 *   initialization swap: weighted 2080 -> 1900, residual 65 -> 71, rejected.
 *   Neither advances the matching prefix beyond the first instruction.
 * - Fresh behavioral rewrite requested by Pascal: ordinary for loops,
 *   pointer-based pixel helper, direct error return, switch-based outline,
 *   no devices or artificial locals. Initial ja/en/other 347/344/324
 *   (draft 194/194/189 instructions). GBA-style packed tile addressing and
 *   a default final outline case give 347/358/313. Hoisting the two palette
 *   colors, advancing a row pointer, and breaking after the shifted row
 *   becomes empty give 318/323/288 (draft 185/185/177), verified in all six.
 *   The final frames are 28/36/36 rather than 100/44/44. This clean form
 *   changes loop.c induction lifetimes, greg allocation and reload slots;
 *   it does not isolate the remaining scheduling differences. Rejected.
 *   Scratch source out/glyph-clean.c and its six out/glyph-clean-*.diff
 *   reports are review material only; none enters the build or count.
 * Further trials from 30/65/34, still rejected:
 * - Share the width load after the two glyph branches: en/other 116/84.
 *   A one-field result struct, result union, helper parameter reordering,
 *   const helper locals and signed colors leave the output unchanged.
 *   A two-field result struct gives other 139 and a 52-byte frame.
 * - Macro pixel helper: 137/101/66. u16 result: 113/79/96. u16 colors:
 *   en/other 102/49. Width reused as character: other 91; as row/column:
 *   unchanged. Volatile result: other 52, again no improvement.
 * - Shift bits after the clipping test: 61/92/65. Fetch the first row
 *   in the while condition: 134/158/97. Two do-while row forms both give
 *   67/98/71; switch-based outline gives 53/88/57.
 * - Return width directly: other 39, with an extra width spill. Delayed
 *   volatile width loads give other 85/93; volatile saved window and rows
 *   with a plain glyph pointer gives 97. The latter gets the desired
 *   store-before-load order but breaks allocation and duplicates a spill.
 * - Five-minute European permute search: 90,672 distinct candidates, no
 *   compile failures, none improves weighted 940 / residual 34.
 *
 * Structural regressions followed further at Pascal's request:
 * - Return width directly (other 39) gets both native r3 width loads via
 *   reload's scratch selection, but spills width inside each branch.
 *   Advancing rows before loading width gives 86; volatile width reads in
 *   that form give 80. Typed narrow/wide glyph records give 104. One-pass
 *   blocks around both initializations give 166 (171 with separate result);
 *   a block around the narrow branch alone leaves 39. A three-minute search
 *   from 39 compiled 54,682 candidates without improving its weighted 990.
 * - Keeping window/rows/result in a volatile record preserves the native
 *   44-byte frame and their three high offsets, but gives other 90. Width
 *   loads finally follow the rows store, but jump2 shares both glyph tails,
 *   leaving 185 instructions instead of 187 and different scratch registers.
 *   Making result plain leaves 90; making the whole record plain gives 189.
 *   Thus delayed width loading alone does not restore the native graph.
 * - A one-pass block around the row fetch gives 58 and swaps y/row-line
 *   registers. Splitting that fetch at the clipping condition gives 74;
 *   a two-minute search improves weighted 2600 -> 2310 but residual 74 -> 80.
 *   Splitting identical load/shift statements in both arms gives 73: one
 *   arm has native load/shift/store order, but different reload registers
 *   and store order prevent jump2 from merging the arms. Both remain.
 * - A signed-halfword cast in one conditional shift arm is removed by
 *   optimization, but leaves two comparisons around the shift (other 47,
 *   188 instructions). Moving next++ after that split gives 159; moving
 *   the load into both arms gives 74. Reversing next++/shift order between
 *   the arms gives 195. An empty one-pass clipping block gives 60. These
 *   expose surviving boundaries, but none places the store last and removes
 *   the extra control flow. They are starting points, not matches.
 * - A one-pass block around tile/line/shift calculations gives 269; putting
 *   the remaining dot operations in a second one-pass block gives 255.
 *   The desired early pixel shift appears, but loop.c/flow now weight the
 *   temporaries differently and allocation adds spills. A separate inline
 *   tile-index helper leaves 34. A two-minute GlyphDot-only search from 34
 *   compiled 36,157 candidates without improving weighted 940.
 * - Sharing all five dot temporaries between paths gives 310. Exhaustively
 *   choosing local versus shared scope for each temporary at each dot site
 *   tests all 1,024 forms; none beats baseline weighted 940 (residual 34). Separate
 *   locals reproduce the inline helper's output. A u16 mask gives 134;
 *   u16 line/shift leave 34. An entry height-read block gives 36, retaining
 *   the frame but moving height's load/store earlier than the native code.
 * At that checkpoint the six best residuals remained unchanged. Scratch
 * sources, diffs and searches under out/ document these alternative paths;
 * none is linked, credited, or used as input to a game build.
 *
 * Width-boundary follow-up, 2026-10-09: other 34 -> 28 in all four editions.
 * The 39-residual direct-width form was a useful intermediate: its reload
 * scratch is native r3, but the narrow branch spills width before advancing
 * rows. Saving the old header pointer, advancing rows, then reading width
 * inside a one-pass block delays that load and lets jump2 merge the two
 * width spills. An inline getter first exposed this form; removing the
 * getter and the wide-branch block preserves 28. The retained device is
 * only the narrow-branch block below. All glyph-header setup instructions,
 * width registers and spill slots now match in de/es/fr/it. Japanese and
 * English retain 30/65: applying the direct-width getter to them gives
 * 88/116; retaining their result alias with it gives 59/85.
 * sched.c:sched_analyze_insn explicitly adds dependencies on every prior
 * register use/set and memory operation across LOOP_BEG/END notes, preserving
 * loop-depth reference counts. The surviving narrow width-read notes are
 * the measured cause; simple source reordering or a getter without the
 * one-pass block leaves 39. No compiler code or flags changed.
 * Further controlled trials:
 * - Width accessed as a FontGlyph field sets MEM_IN_STRUCT in RTL but leaves
 *   the earlier residual 34. A u64 result gives 61 and an extra word spill.
 * - Reusing width as row leaves 34 but changes width r0 -> r1 and fixes the
 *   join-store position. Reusing it for row y gives 46; moving rownext after
 *   that calculation gives 131. Reusing width as pixel mask gives 259; as
 *   pixel line, 229. The latter's two-minute search compiles 36,397 candidates
 *   and reaches 32, recovering the desired second-dot address order but
 *   keeping a different line register and two header scheduling differences.
 *   Its alternatives remain in out/permute/glyph-width-line-search.
 * - All 128 choices of one-pass wrapping around individual GlyphDot
 *   statements fail to beat the earlier weighted 940 baseline. Splitting
 *   tile <<= 5 from a one-pass tile += vram gives 247 on the new width form:
 *   the desired dot order appears, but allocation spills x and retains color3.
 *   Plain register parameter hints leave the new residual 28 unchanged.
 * - A one-pass next++ after the row shift gives 32: flow combines the
 *   increment with the load, so reload still puts its spill before the shift.
 *   A separate loaded pointer with next copied back in the one-pass block
 *   gives 161, retaining that pointer in r6 and disturbing the other spills.
 *
 * Entry-boundary follow-up: ja/en/other 30/65/28 -> 26/52/26.
 * A one-pass height read restores Japanese's argument copies and compare
 * order; its height spill remains before the compare. In Europe, compute
 * the font predicate first, then read height in the block: movls now
 * precedes ldrh. In English, reading height in that block before the
 * range check allows a direct error return while retaining the native
 * long-lived drawing registers. The window reload and error spill vanish.
 * Keeping a named index through the wide-glyph expression gives 52 rather
 * than 57, but wrongly eliminates the native wide-branch subtraction.
 * That omitted subtraction offsets the still-extra narrow width load:
 * the 196-instruction count does not establish the native control flow.
 * No compiler or option changes. New controlled trials:
 * - All 21 contiguous groups of two or more GlyphDot statements inside
 *   one-pass blocks give 62..267; none improves 28. Sharing line with the
 *   stride subtraction gives 40 and moves the first address add later,
 *   but changes second-dot registers. Sharing shift gives 231; mask 253;
 *   the previous search's temporary aliases in the helper give 170.
 *   Reusing line as mask gives 267; counted one-iteration loops give
 *   321 (line/shift only) and 351 (whole helper).
 * - European plain named predicate, bitwise OR, or predicate-only block
 *   leaves 28. Height in the following block gives 26. Duplicating height
 *   reads around the predicate gives 55. Equivalent height-copy joins,
 *   redundant breaks and an empty predicate block leave 26. Explicit x/y
 *   copies leave 26; one-pass x/y copies give 101, y alone 83.
 * - English named index with a plain height read gives 64 (direct return
 *   71); index/height blocks before its test give 62 (direct return 69).
 *   Height in a block before index gives 73 with the shared result exit,
 *   but 52 with direct return. Recomputing index at the width-table or
 *   narrow-glyph use leaves 52; at the wide-glyph use gives 57.
 * - A block around the packed-font width read leaves English at 52 with
 *   different scheduling: row initialization is earlier, but its width
 *   spill moves before the clip compare and source spill moves later.
 *   The same block gives other 81, with 186 instructions. A volatile next
 *   pointer with a cached load and explicit post-shift update gives other
 *   68 (187 instructions); its changed allocation is not retained.
 *
 * Dot-boundary follow-up: ja/en/other 26/52/26 -> 16/46/22.
 * An empty loop between pixel shift and address construction restores the
 * native order, but initially changes lreg: the main dot's x pseudo has
 * live length 7 instead of 9, moving it ahead of the tile quantity. Tile
 * gets ip locally, x spills globally, and other residual becomes 241.
 * Reusing line for stride-2, and pixel first for the tile column then the
 * complete output pixel, restores the native registers. The tile-column
 * assignment must precede stride-2: reversing them gives 26 rather than
 * 22 in Europe, with an early stride reload into r1. The final helper
 * matches the complete main dot sequence in all six editions and both
 * Japanese dots. Japanese improves to 18; English/other to 48/22.
 * Packed-font clipping changes the helper's incoming x/index lifetimes,
 * so that path retains its previous helper. Applying the new helper there
 * gives en/other 88/52; moving clipping inside it leaves those figures,
 * while testing the reused pixel temporary gives 90/54.
 * Finally an empty block after the accent x/y changes keeps the result
 * move/spill after y: Japanese 18 -> 16 and English 48 -> 46.
 * All of these changes are permitted C; compiler and flags are unchanged.
 * Other measured alternatives from this checkpoint's initial 26/52/26:
 * - Delayed narrow width reads give ja/en 55/74. Precomputing the accent
 *   offset in both glyph branches gives 43/94. Reusing that offset in
 *   the earlier tests gives 37/93; comma assignment before the font test
 *   gives 37/70. Japanese has one or three extra instructions respectively.
 *   Two two-minute searches compile 17,657/17,658 candidates: weighted
 *   890 -> 820 and 1380 -> 1310, both residual 37 -> 45. Not retained.
 * - A boundary around a volatile next-pointer store gives other 72:
 *   shift precedes store, but comparison follows it. Saved predicates
 *   around load/shift give 30; a predicate-only block gives 50, a combined
 *   predicate/load block 52. Cached-pointer store/shift boundaries give
 *   159/155/159; a pointer alias gives 159. Their volatile forms give
 *   74, except predicate-only and predicate/load blocks at 96.
 * - A complete local work record for the eleven spilled values gives
 *   other 214 plain or 187 volatile; no field layout is retained.
 * - Empty dot boundary after mask construction gives 263/274/265.
 *   Moving mask construction after the address gives 243/272/241.
 *   Sixteen temporary-reuse combinations give other 112..259; extending
 *   tile from x's column helps, but coalesces the MLA input with its result.
 *   Applying that only to the main dot gives 42. Reusing line for stride
 *   and tile for x's column gives 30; its two-minute helper search compiles
 *   34,954 candidates, weighted 770 -> 640, residual 30 -> 28.
 *   Reusing pixel for the final complete expression reaches 26, then
 *   reversing the first two assignments gives the retained 22.
 * - After the dot/accent improvements, putting Japanese rownext=row+1
 *   in both successors of the empty-row test gives 28 and two extra
 *   instructions. The compare precedes the increment, but jump2 does not
 *   hoist the differing successor sequences into the native shared prefix.
 *
 * Font-setup follow-up: ja/en/other 16/46/22 -> 16/31/18.
 * Moving English's range check before the height read changes sched1's
 * entry order: index pseudo 41 is now live before the incoming r0 window
 * is copied to pseudo 32. greg records a hard-r0 conflict for the index,
 * so it takes native ip; accent pseudo 149 then takes native lr. This
 * lowers English to 37, although reload adds a window-pointer load and
 * the range branch now precedes height. The draft has 197 instructions.
 * A boundary after source initialization gives English 33; another after
 * color3 gives 31 and the four European editions 18. Color initialization
 * now matches in English, and Europe's row initialization and width-table
 * pool displacement match. The width spill is now before its comparison;
 * this is a measured tradeoff, not a complete match.
 * All 120 orders of the five independent font initializations, with all
 * 32 boundary placements, were measured in both English and German from
 * the earlier-range-check form. None improves 31/18. These 7,680 trials
 * and their residuals are in out/glyph-font-setup-tbs-{en,de}.tsv.
 * Other source forms measured during this continuation:
 * - Packed dot: 48 combinations of initial math, pixel expression and
 *   mask reuse, then 320 combinations of shift/mask/pixel lifetime reuse,
 *   fail to beat 22. The latter range from 50 to 217. Separating the byte
 *   address from tile in 24 forms gives 50..203. All 128 boundary choices
 *   with two initial-math and two pixel forms (512 trials) give 22..134;
 *   the forms retaining 22 do not fix either address instruction.
 * - The boundary that delays both packed-dot address operations gives
 *   50 with the old math or 52 with the new helper's reused temporaries.
 *   lreg explains the regression: line's lifetime grows from 2 to 5 and
 *   shift's from 4 to 6. Tile takes r1 instead of r0, and shift takes r0
 *   instead of r1. Reusing line for stride makes it globally allocated,
 *   but does not restore tile/shift. Sharing any subset of the six dot
 *   temporaries between paths (128 forms) gives 50..312, not a repair.
 * - Reusing caller row, character or width for x+col, or computing x and
 *   column before color selection, also fails. A plain position alias
 *   leaves 22/52 for the old/new helper. The remaining path needs the
 *   native local-allocation priorities as well as a surviving boundary.
 * - Japanese's six color/width/x initialization orders with eight boundary
 *   choices give 16..22; no setup store moves into its native position.
 * - English's eight entry forms, four narrow-header forms and both wide
 *   index forms give 37 as their minimum. Delaying the narrow width load
 *   on that form gives 75 and swaps index/accent to lr/ip. Recomputing the
 *   wide index gives 40. Neither higher residual is an adopted repair.
 * - u8/u16/s32/s8/s16 font-width forms leave 46/37 before the two setup
 *   boundaries; signed forms return through u8. Volatile gives 118/111.
 *   Seven index/address reuse forms for the width lookup give 46..73 or
 *   37..64; none improves. Splitting index=character; index-=32 across
 *   height/boundary placements gives 128. A split -64/+32 gives 63;
 *   duplicating a plain height read in the error path leaves 37 (the dead
 *   read vanishes), and explicit range-error labels give 65.
 *
 * ARM translation-unit audit requested by Pascal, 2026-10-09:
 * Tile_BuildMetatiles follows this function without padding in all six
 * editions. Both fit stock ARM C. Text_DecodeSymbol/UiText_LookupMessageArm
 * before it and Tile_Decompress4bpp after it have rrx or saves without lr;
 * their assembly is independently proven. The palette kernels are in a
 * separate ROM cluster, not beside this pair. The existing color_darken
 * and color_darken_partial files have four-register write-back stores
 * using r4-r7: the compiler's copy expander fixes r0-r3, while its general
 * ldm/stm peepholes do not write back. No new demotion was justified.
 * The compiler DOES emit pop into ip / bx ip in output_return; two C2
 * comments relying on that return were corrected to independent evidence.
 * alchemy units previously misread old agbcc objects without mapping
 * symbols as Thumb. Its loader now falls back to the actual compiler route,
 * and proposes this pair as a unit. Adjacency is compatible, not proof.
 * Concatenating this draft then metatile.c into one C translation unit
 * leaves the complete glyph draft bytes, including its pool, unchanged in
 * all six: 780 bytes ja, 804 en, 756 others. Residuals remain 30/65/34.
 * Thus a larger file is plausible but simple concatenation does not fix
 * allocation or scheduling. The source remains one uncredited draft.
 *
 * Packed-dot and glyph-header follow-up, 2026-10-09:
 * ja/en/other 16/31/18 -> 14/19/14, measured in all six editions.
 * - Packed dot: the earlier 52-residual delayed-address form becomes useful
 *   after wrapping line and shift in one-pass blocks. Loop notes increase
 *   their weighted references without adding instructions. At that step's
 *   English lreg dump, tile pseudo 75 has refs 21/live 14, and shift 79 has
 *   refs 7/live 6; local allocation gives tile r0 and shift r1. Before the
 *   wrapped calculations they took r1/r0. Both address instructions now
 *   follow AND/LSL. The 48 weight/depth trials reach other 14; combining
 *   the two blocks and removing the empty block retains it. Wrapping only
 *   shift gives 26. Using this helper at every dot gives 116/67/34, so the
 *   main-dot helper keeps its different, measured boundary.
 * - Japanese header: delaying width through the saved header pointer alone
 *   worsens 16 -> 45, extending character beyond the glyph-base load.
 *   Giving the accent offset its own mark variable in both glyph branches
 *   gives 33; also assigning it in the font test gives 27. A comma in the
 *   selection predicate gives 25, but adds three boolean instructions.
 *   Staged tests remove those instructions (16). Duplicating mark's
 *   assignment in both destinations of the null test gives 14. jump.c's
 *   first pass hoists the null destination's assignment after the compare;
 *   cse deletes the duplicate fallthrough assignment. Thus CMP/SUB/BEQ
 *   survives with native registers, and the delayed header load also fits.
 *   A one-pass null-destination block or plain alias leaves 14; split
 *   subtract forms give 19, and the complement form gives 20.
 * - English: the same staged mark assignment, delayed narrow width read,
 *   and recomputed wide index give 19. Keeping the wide named index gives
 *   28; keeping the early narrow width read gives 30. jump2 now shares the
 *   identical width loads, and every glyph-header instruction agrees.
 *   Defining mark before the null test instead gives 21. Of 36 combinations
 *   of selection, width, wide index and entry, 19 is best. Height before
 *   the range branch gives 51 with index r0/accent ip. Reading height before
 *   defining index removes the extra reload (196 instructions) but gives
 *   28. Its lreg/greg dumps show index pseudo 42 inheriting preference 0
 *   from font_width at its REG_DEAD width-table load: global.c's
 *   expand_preferences merges preferences of nonconflicting dying and
 *   defined allocnos. Plain register hints do not change any of 19 tested
 *   entry forms. Shared error-result exits give 40 or 56, not a repair.
 *   On the 28-residual form, plain font-width records/arrays and u8 casts
 *   leave 28; volatile storage gives 110, a wide integer gives 90, delayed
 *   font source setup gives 62/77, and reusing index for font width gives
 *   97. Window aliases give 28..51; an alias plus width boundary gives 90.
 *   Signed/64-bit index types leave 28. None removes the return preference.
 * Other controlled trials, all rejected:
 * - 192 explicit row-record forms (field order, volatility and tail shape):
 *   best other 72. Native frame size remains, but spill slots move. A delayed
 *   cached-pointer store moves behind the shift and also behind the compare.
 * - 64 packed-dot wide-integer/type forms: best other 46. 1,024 context
 *   records with independently volatile fields and two row tails: best 166,
 *   191 instructions; matching field offsets does not restore lifetimes.
 * - A combined entry height/test block leaves ja/other unchanged but gives
 *   English 41 before the mark fix. Pending source aliases give en/other
 *   51/16; blocks around color1 give 77/66. Volatile height gives 28/37/55;
 *   a u16 cast leaves output unchanged. None fixes a remaining store.
 * - Ten Japanese width forms give 16..129: field syntax alone is unchanged,
 *   a delayed plain header gives 45, volatile reads give 42 or 129, and a
 *   whole postincrement in a block gives 79. Only the staged-mark follow-up
 *   repairs the regression. Scratch trials under out/ never enter a build.
 *
 * Width-table follow-up, 2026-10-09: English 19 -> 17, other editions
 * unchanged. Separate the table pointer from the byte read, retain the
 * named character index, and put an empty one-pass boundary between table
 * setup and row zero. The table/row/byte-load order and literal distance
 * now match. Wrapping any one of the three assignments works too; the
 * retained scoped pointer and empty boundary are sufficient. Using
 * character-32 at the alias read instead folds (table+character)-32,
 * destroys index reuse and gives 68. Forty-eight table/order/boundary
 * trials were measured with each spelling; only named-index forms reach
 * 17. The height-first alternative remains 26 and keeps index in r0.
 * - Fresh three-minute searches compiled 27,366 Japanese and 27,250 English
 *   candidates. Japanese weighted 420 -> 410, but residual 14 -> 20:
 *   moving width before color swaps their registers. English weighted
 *   800 -> 790 leaves residual 19: moving row zero after width fixes the
 *   pool load but moves the byte load early. That is the starting point
 *   of the successful table-pointer split, not itself an improvement.
 * - Explicit low frame records (96 forms) give English 144 or worse;
 *   high records (144 forms) give 190 or worse. They break the return
 *   preference chain, but also change many other lifetimes; they do not
 *   restore the scalar graph. No record is retained.
 * - 44 index-last-use forms: dead reads/increments disappear, duplicate
 *   bounds checks normally disappear, and reusing index for row zero
 *   gives 26 on the height-first form. The latter moves the table load,
 *   not index's r0 preference. Thirty-two inline entry helpers give
 *   28..151; 32 direct/split range predicates give 19..147. None removes
 *   the entry reload while retaining native registers.
 * - 56 wide-arithmetic row-pointer forms: casts alone leave other 14;
 *   extra boundaries give 18. A persistent wide address changes loop.c's
 *   induction variable, keeps next in a register and spills x (147 or
 *   worse). 120 negate/complement/increment forms similarly give at least
 *   147. These do not isolate the row-tail store. The second life_analysis
 *   after reload still calls find_auto_inc; a late fold remains possible
 *   in principle, but no tested form preserves the required graph.
 * - 25 Japanese font-read forms: plain aliases and single boundaries can
 *   leave 14; volatile byte reads give 77..95, and one delayed pointer
 *   advance gives 32. None repairs the compare/load order. All rejected
 *   sources and reports remain ignored analysis under out/.
 *
 * Empty-row follow-up, 2026-10-09: Japanese 14 -> 12, others unchanged.
 * Put the empty-row arm first, with rownext=row+1 in both destinations.
 * jump.c's single-SET hoist moves the first increment after the comparison;
 * CSE deletes the duplicate. Native CMP/ADD/BEQ survives sched2, with no
 * added instructions or changed registers. The opposite arm order gives
 * 26 and two extra instructions. Of 32 label/arm/copy forms, the plain
 * empty-first if/else is sufficient; neither labels nor loop notes are
 * needed. Fresh RTL shows the hoisted ADD at jump insn 705, following
 * compare 144, and the duplicate insn 155 gone by CSE.
 * Further controlled trials, not retained:
 * - 100 typed pending-width/copy/branch forms leave en/other at best 17/14.
 *   The copies collapse before allocation; the spill remains early.
 * - 240 whole-function wrapper/parameter-order forms give English at best
 *   104. Another 84 wrappers pass index, height or accent as extra arguments;
 *   best 106. Both disturb the allocation graph beyond the entry sequence.
 * - 80 reused-comparison row blocks give other at best 36. They swap y and
 *   row-line registers, yet still place the next-pointer spill before LSL.
 * - 64 const-parameter/index-initializer forms leave English at best 17;
 *   the height-first form stays 26 unless an early initializer worsens it
 *   to 49. Const alone creates no required hard-r0 conflict.
 * - 96 equivalent English range predicates/entry forms give at best 17.
 *   Shifted or masked ranges retain another instruction (best 18); the
 *   compiler does not reduce them to the native comparison against 111.
 * - 80 Japanese color/width/source-local and setup-block forms give at best
 *   12. Volatile locals change allocation and sometimes add instructions;
 *   none repairs a remaining spill. Analysis stays in ignored out/.
 *
 * Row-cursor and entry follow-up, 2026-10-09, no residual improvement:
 * - 128 single-field struct/union cursor forms: best other 56. The volatile
 *   cursor keeps the 44-byte frame and native registers but moves next to
 *   sp+40 and shifts the other spill slots. A post-shift boundary gives 60:
 *   LSL finally precedes STR, but ADD/CMP then follow both. Zero initializers
 *   and intermediate record copies do not preserve a useful late clobber.
 * - 192 font-predicate/type/boundary forms and 64 sequenced-boolean forms:
 *   best other 14. Reversing the short-circuit OR moves the font load after
 *   y's argument copy as required, but reverses predicate construction and
 *   adds an instruction (19). Explicit booleans do not combine that repair
 *   with the native predicate. The canonical form is retained.
 * - arm.md's ordinary SI and soft-SF stores both use type store1; changing
 *   a scalar copy's mode is not a different one-cycle STR route. Its late
 *   raw-HI load/increment peephole emits word LDR, not the required LDRH.
 *   reload_cse_regs deletes coalesced copies before sched2, and flow's second
 *   pass also deletes ordinary no-op moves: a temporary alone cannot act as
 *   a scheduling dependency that disappears only after sched2.
 *
 * Allocation follow-up, 2026-10-09, no residual improvement:
 * - 48 English lookup-address/entry forms fold the pointer back into the
 *   indexed byte load. Best early-range form 19; height-first 26. Separating
 *   table+index therefore does not break expand_preferences' propagation
 *   from font_width's return register to the dying index.
 * - 144 grouped source/color initializations leave other at best 14.
 * - 128 delayed-copy/reused-test/dot-depth forms give other at best 38.
 *   Fresh greg dumps explain the plain delayed-copy regression: next's
 *   pseudo 105 changes from refs 5/live 46 to refs 13/live 47; x remains
 *   refs 20/live about 160. The pointer allocates ahead of x and y.
 * - 384 outline-update weighting forms can recover native x=r9,y=r8
 *   (best 55), but next keeps r7 and tile_y spills instead. Weighting the
 *   geometry assignments (180 forms) or their helper uses (180 forms)
 *   gives at best 144 and also changes the loop/allocation graph.
 * - 56 geometry-sharing forms pair main tile_y/row_line/vram with packed
 *   vram/tile_y/row_line, following their native register reuse. All seven
 *   sharing subsets reproduce canonical other 14 without the delayed copy;
 *   none repairs that copy's allocation regression (best 55). These larger
 *   regressions were followed explicitly; no new device is retained.
 *
 * Remaining differences, located by labels in recon/tbs/raw/moji.s.
 * The following is a pass-level diagnosis, not a claim to have found the
 * missing C. A stated dependency or lifetime change is a requirement for
 * the next experiment; no tested permitted source form fixes these sites.
 * Generate each edition's --rtl sequentially: the compiler first writes
 * identically named dump files, then alchemy moves them to
 * out/<edition>/obj/drafts/arm/rtl/080155d0.c.*. The --show reports for this
 * attempt are in out/reports/glyph/tbs-<language>.diff (analysis only).
 *
 * Compiler decisions:
 * A: lreg/greg. local-alloc.c QTY_CMP_PRI and global.c allocno_compare
 *    rank refs * floor_log2(refs) * size / live_length, then quantity or
 *    allocno; global.c set_preference also propagates operand preferences.
 *    Shared source variables alter references, conflicts and lifetimes;
 *    declarations alter pseudo order. reload1.c alter_reg assigns spills
 *    in pseudo order. Native spill slots now agree; do not force them with
 *    volatile again. Compare lreg to greg to distinguish allocation from
 *    reload's short-lived address scratch registers.
 *    English's earlier range check creates the hard-r0 conflict that
 *    prevents character-32 from taking font_width's propagated return
 *    preference 0. This fixes index=ip and accent=lr, but puts the height
 *    load in the successor block and requires a window reload. The next
 *    entry form must preserve that conflict with height before the branch.
 * S: sched2. sched.c schedule_block schedules backwards; rank_for_schedule
 *    compares priority, dependency class, then original LUID. Source order
 *    alone cannot change an unequal priority or a data/anti dependency.
 *    Native order requires another lifetime/dependency or a scheduling
 *    boundary that survives optimization. Both dots are now resolved;
 *    row-tail spill placement remains.
 *    In the earlier European dot's sched2 graph, pixel shift and address
 *    OR tie at priority 17. At T-8 the former wins because it is
 *    independent of mask=15, while the latter has an anti
 *    dependence through r3. This is not merely an original-order tie:
 *    matching that site needs another RTL dependency or a late-removable
 *    instruction/block, rather than another ordering of those same SETs.
 * J: jump2 cross-jumping requires equal instruction tails after sched2;
 *    arm.c's literal-pool pass then computes distances from the final code.
 *
 * Japanese: six moved instructions, accounting for all 12 residual entries.
 * - Entry (S): cmp r1,#255 must precede str r0,[sp,#20]. The one-pass
 *   height read restores the argument copies and places the load before
 *   the compare, but reload's spill still precedes it. Delaying that spill
 *   requires a surviving dependency or a later common-tail transform.
 * - Font setup (S): str r3,[sp,#12] must follow x++; str r2,[sp,#16] must
 *   follow cmp r3,#1. The draft stores both early. Delaying the width and
 *   color spill stores needs a dependence surviving reload, not volatile.
 * - First byte and font tail (S): cmp y,#31; ldrb ip,[lr,r3], and cmp
 *   r3,#31; ldrb ip,[r9],#1, respectively; draft puts each load before
 *   its compare. The compare is free; see the memory-before-branch issue
 *   below. No tested source predicate creates the required constraint.
 * - .L_font_row now matches: empty-first branch assignments let jump hoist
 *   ADD after CMP and CSE remove the duplicate, without changing allocation.
 * - .L_row_next (S): lsl ip,ip,#16 must precede str next,[sp,#8]; draft
 *   reverses them. The free one-cycle shift wins the last slot before the
 *   branch over the two-cycle store in stock sched2. A surviving constraint
 *   or late transform is needed; jump2's register-set hoists cannot hoist
 *   this store. This observation does not prove that plain C cannot match.
 * - Both dots, .L_glyph/.L_wide and .L_shift are resolved by the measured
 *   boundaries and staged mark assignment below. Pool displacements agree.
 *
 * German/Spanish/French/Italian: all have the same seven moved instructions
 * on each side (14 total), solely order. Their pool displacements agree.
 * The following exhausts the --show differences.
 * - Entry (S): mov ip,r1; sub sp,sp,#44; ldr font,[sp,#72] are early.
 *   Native copies x and y before the frame adjustment and font/character
 *   setup. The height spill must follow cmp r3,#0; the ldrh now follows
 *   movls r3,#1 as required. Argument lifetimes and the free-compare
 *   constraint must change without disturbing x=r9,y=r8.
 * - Font setup (S): str r2,[sp,#24] must follow color1's final add; it now
 *   precedes both color1 instructions. str r2,[sp,#12] must follow cmp
 *   r8,#31; it now precedes it. Delaying these reload-inserted spills
 *   needs dependencies surviving allocation. Color1's spill, row zero
 *   and the width-table load are resolved by the two boundaries below.
 * - Both dot bodies now match. The packed dot's line/shift block restores
 *   the required lreg priorities as well as the sched2 boundary.
 * - .L_row_next (S): the same shift/store reversal as Japanese.
 * - .L_glyph/.L_wide now match: the one-pass width read preserves the
 *   needed scheduling boundary, and returning width directly gives r3
 *   through reload. jump2 shares str r3,[sp,#32] before the row reload and
 *   pass initialization. The former short width's return-register preference
 *   no longer controls either load. This is resolved, not a remaining site.
 *
 * English: 8 native and 9 draft residual entries (17 total). Its
 * long-lived drawing registers, index ip and accent lr agree.
 * - Entry (A,S,J): sub ip,r1,#32 and sub sp,sp,#44 are too early; cmp
 *   ip,#111 and str window,[sp,#40] precede the x copy. mov r0,#1 / bhi
 *   precede height's load/store, followed by an extra ldr r0,[sp,#40].
 *   Native has the index before ldrh r0, then cmp, height spill, movhi
 *   r0,#1 and bhi. The earlier test fixes allocation by keeping incoming
 *   r0 live across index's definition, but the next form must keep that
 *   hard-register conflict while moving height into the entry block.
 *   reload can then reuse the incoming window, and jump2 can predicate
 *   the return move. No tested form satisfies both requirements yet.
 * - Font setup (S): str r3,[sp,#12] must follow cmp r7,#31, not precede
 *   it. The color constants, source/color stores, row/table order and
 *   width-table literal distance now match. The free-compare/spill
 *   constraint remains to be fixed.
 * - .L_glyph/.L_wide now match: staged mark assignments and the header
 *   boundary preserve native registers; jump2 shares their width load.
 *   Recomputing the wide index restores sub r3,r1,#32 and its shift.
 *   The entry window reload alone explains the draft's extra instruction.
 * - .L_row_next (S): the same shift/store move as Europe. Both dots and
 *   .L_shift now match.
 * - The remaining range-branch displacement (J) follows from its early
 *   position. All later branch and pool displacements agree. This is not
 *   a constant to encode in source. Literal pools and the
 *   complete extent still require byte verification before any adoption.
 */

#include "types.h"
#include "uiwindow.h"
#include "glyph.h"
#include "text_font.h"

extern const u8 UiText_GlyphWidths[];

static __inline__ void GlyphPackedDot(u32 stride, u32 x, u32 tile_y, u32 row_line,
	u8 *vram, u32 color)
{
	u32 tile;
	u32 line;
	u32 shift;
	u32 mask;
	u32 pixel;

	/* FAKEMATCH: reuse line for stride-2, pixel for the tile column/output,
	 * and tile for the address. The one-pass line/shift block both delays
	 * address construction and raises these values' loop-weighted refs.
	 * lreg then gives tile r0 and shift r1 instead of r1/r0. This matches
	 * the packed dot in all five editions: en/other 31/18 -> 27/14.
	 * An empty boundary alone gives other 48; wrapping shift alone gives 26. */
	pixel = x >> 3;
	line = stride - 2;
	tile = pixel * line + tile_y;
	do
	{
		line = row_line | ((x & 7) >> 2);
		shift = (x & 3) << 2;
	} while (0);
	tile = (u32)vram + (tile << 5);
	tile |= line << 1;
	mask = 15 << shift;
	pixel = (*(u16 *)tile & ~mask) | (mask & color);
	*(u16 *)tile = pixel;
}

static __inline__ void GlyphDot(u32 stride, u32 x, u32 tile_y, u32 row_line,
	u8 *vram, u32 color)
{
	u32 tile;
	u32 line;
	u32 shift;
	u32 mask;
	u32 pixel;

	/* FAKEMATCH: reuse line for stride-2, pixel for the tile column, and
	 * tile for the byte address.
	 * Together with the empty loop's scheduling boundary, these lifetimes
	 * restore the native dot sequence: both Japanese sites and the outline
	 * site in the other editions. Residuals ja/en/other: 26/52/26 -> 18/48/22.
	 * Without these lifetimes the same boundary spills x (other 241). */
	pixel = x >> 3;
	line = stride - 2;
	tile = pixel * line + tile_y;
	line = row_line | ((x & 7) >> 2);
	shift = (x & 3) << 2;
	do { } while (0);
	tile = (u32)vram + (tile << 5);
	tile |= line << 1;
	mask = 15 << shift;
	pixel = (*(u16 *)tile & ~mask) | (mask & color);
	*(u16 *)tile = pixel;
}

s32 UiText_DrawGlyphArm(struct UiWindow *window, u32 character,
	u32 x, u32 y, const u8 *font)
{
	const u16 *rows;
#if !defined(TBS_EDITION_JA) && !defined(TBS_EDITION_EN)
	const u16 *header;
#endif
	u32 width;
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	u32 mark;
	u32 result;
#endif
	u32 stride;
#if defined(TBS_EDITION_EN)
	u32 index;
#elif !defined(TBS_EDITION_JA)
	u32 use_font;
#endif
	u32 row;
	u32 rownext;
	u32 col;
#if defined(TBS_EDITION_JA)
	/* FAKEMATCH: four unused RenderInput records reserve the measured
	 * 64-byte gap: without them the native 100-byte local frame becomes 36. */
	struct RenderInput frame[4];
#endif
#if !defined(TBS_EDITION_JA) && !defined(TBS_EDITION_EN)
	use_font = character <= 255 || font != NULL;
#endif
#if defined(TBS_EDITION_EN)
	/* FAKEMATCH: test the named index before reading height. This restores
	 * index=ip and the accent offset=lr, lowering English from 46 to 37
	 * despite one extra window reload. With the mark/header form below,
	 * recomputing the wide index now lowers English from 28 to 19. */
	index = character - 32;
	if (index > 111)
		return 1;
#endif
	/* FAKEMATCH: a one-pass height read preserves its scheduling boundary.
	 * This originally lowered ja/en/other from 30/65/28 to 26/52/26.
	 * English's range test now precedes it for the measured reason above. */
	do
	{
		stride = window->height;
	} while (0);

#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	/* FAKEMATCH: define mark in both destinations of the font-null test.
	 * Together with the delayed width read below, this keeps character in
	 * r1 and mark in ip and places the narrow header load after its pointer
	 * spill: ja 16 -> 14. Defining mark before the test instead gives 16;
	 * a comma assignment inside the combined predicate gives 25. English
	 * keeps mark in lr; with its wide index recomputed, en 27 -> 19 and
	 * jump2 shares the now-identical width loads. */
	if (character > 255)
	{
		if (font == NULL)
		{
			mark = character - 222;
			goto glyph_wide;
		}
		mark = character - 222;
		if (mark > 1)
			goto glyph_wide;
	}
#else
	if (use_font)
#endif
	{
		if (font != NULL)
		{
#if defined(TBS_EDITION_JA)
			const u8 *source;
			u32 color;
			u32 font_width;
			u32 bits;

			color = 0x1111;
			font_width = 8;
			x++;
			if (character - 222 <= 1)
			{
				character += 32;
				x -= 2;
				y--;
				font_width = 2;
			}
			character -= 32;
			source = font + character * 10;
			row = 0;
			bits = *source++;
			if (y > 31)
				goto font_done;
			{
				while (1)
				{
					u32 tile_y;
					u32 row_line;
					u8 *vram;

					col = 0;
					/* FAKEMATCH: put the empty-row increment first and
					 * repeat it in the drawing arm. jump hoists the first
					 * increment after CMP; CSE removes the second. This
					 * restores CMP/ADD/BEQ: Japanese 14 -> 12. Reversing
					 * the arms leaves two extra instructions and gives 26. */
					if (bits == 0)
						rownext = row + 1;
					else
					{
						rownext = row + 1;
						tile_y = (y + row) >> 3;
						vram = (u8 *)0x06002500;
						row_line = ((y + row) & 7) << 1;
						do
						{
							if (bits & 1)
								GlyphDot(stride, x + col, tile_y, row_line, vram, color);
							col++;
							bits >>= 1;
						} while (bits != 0);
					}
					row = rownext;
					if (rownext > 9)
						break;
					bits = *source++;
					if (y + rownext > 31)
						break;
				}
			}
font_done:
			return font_width;
#else
			const u8 *source;
			u32 color1;
			u32 color3;
			u32 font_width;
			u32 packed;
			u32 color;
			u8 *vram;

			source = font + character * 32;
			/* FAKEMATCH: separate source, colors and row setup. These two
			 * boundaries restore the color initialization order, lowering
			 * en/other from 37/22 to 31/18. Width's spill is now early. */
			do { } while (0);
			color1 = 0x1111;
			color3 = 0x3333;
			do { } while (0);
#if defined(TBS_EDITION_EN)
			{
				const u8 *glyph_widths = UiText_GlyphWidths;

				/* FAKEMATCH: separate the table pointer from the indexed
				 * read across a scheduling boundary. This restores table
				 * load, row zero, byte load: en 19 -> 17. Reordering row
				 * and width alone leaves 19; spelling character-32 instead
				 * of index here changes address folding and gives 68. */
				do { } while (0);
				row = 0;
				font_width = glyph_widths[index];
			}
#else
			row = 0;
			font_width = UiText_GlyphWidths[character - 32];
#endif
			if (y > 31)
				goto font_done;
			vram = (u8 *)0x06002500;
			while (1)
			{
				u32 tile_y;
				u32 row_line;

				col = 0;
				rownext = row + 1;
				tile_y = (y + row) >> 3;
				row_line = ((y + row) & 7) << 1;
				while (1)
				{
					if ((col & 1) != 0)
						packed >>= 4;
					else
						packed = *source++;
					if ((packed & 13) == 1)
					{
						color = color3;
						if ((packed & 15) == 1)
							color = color1;
						if (((x + col) >> 3) <= 29)
							GlyphPackedDot(stride, x + col, tile_y, row_line, vram, color);
					}
					col++;
					if (col > 7)
						break;
				}
				row = rownext;
				if (rownext > 7)
					break;
				if (y + rownext > 31)
					break;
			}
font_done:
			return font_width;
#endif
		}
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
		mark = character - 222;
#endif
		rows = (const u16 *)((const u8 *)UiText_Glyphs + (character - 32) * 26);
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
		{
			const u16 *header = rows;

			rows++;
			/* FAKEMATCH: preserve the header-read boundary. With mark
			 * calculated before the address, ja's load/store order matches.
			 * The same read alone gives 45 by extending character's life.
			 * English also needs this boundary for jump2 to share the load;
			 * with mark and the wide index above, en 30 -> 19. */
			do
			{
				width = *header;
			} while (0);
		}
#else
		header = rows;
		rows++;
		/* FAKEMATCH: keep the width read after the row-pointer update.
		 * With width returned directly, this merges the duplicate width
		 * spill and lowers de/es/fr/it from 39 to 28 residual instructions. */
		do
		{
			width = *header;
		} while (0);
#endif
	}
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	goto glyph_selected;
glyph_wide:
#else
	else
#endif
	{
#if defined(TBS_EDITION_JA)
		width = 10;
		rows = (const u16 *)(UiText_KanjiGlyphs + (character - 256) * 24);
#else
		rows = (const u16 *)((const u8 *)UiText_Glyphs + (character - 32) * 32);
		width = rows[0];
		rows += 4;
#endif
	}
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
glyph_selected:
	if (mark <= 1)
	{
		x -= 3;
		y -= 2;
		/* FAKEMATCH: keep the zero result after both coordinate changes.
		 * This restores the native order: ja 18 -> 16, en 48 -> 46. */
		do { } while (0);
		result = 0;
	}
	else
		result = width;
#endif

	{
		register u32 pass;
		const u16 *next;
		const u16 *rows1;

		rows1 = rows + 1;
		pass = 0;
		while (1)
		{
			u32 color;
			u32 bits;
			u8 *vram;
			/* FAKEMATCH: a separate next-pass lifetime avoids five repeated
			 * pass reloads. Direct pass++ raises ja/en/other residuals from
			 * 34/76/34 to 81/123/77 with the shared row and column locals. */
			u32 next_pass;

			if (pass == 3)
				color = 0x1111;
			else
				color = 0x5555;
			if (!(window->flags & 64))
				goto draw;
			if (pass <= 1)
			{
				if (pass == 1)
					x += 2;
				else
					x--;
				goto draw;
			}
			if (pass == 2)
			{
				x--;
				y++;
				goto draw;
			}
			y--;
draw:
			row = 0;
			bits = *rows;
			bits <<= 16;
			next = rows1;
			next_pass = pass + 1;
			if (y > 31)
				goto pass_next;
			vram = (u8 *)0x06002500;
			while (1)
			{
				u32 tile_y;
				u32 row_line;

				col = 0;
				rownext = row + 1;
				tile_y = (y + row) >> 3;
				row_line = ((y + row) & 7) << 1;
				while (1)
				{
					if ((s32)bits < 0)
						GlyphDot(stride, x + col, tile_y, row_line, vram, color);
					bits <<= 1;
					if (bits == 0)
						goto row_next;
					col++;
					if (col > 13)
						break;
				}
row_next:
				row = rownext;
				if (rownext > 11)
					break;
				bits = *next++;
				bits <<= 16;
				if (y + rownext > 31)
					break;
			}
pass_next:
			pass = next_pass;
			if (pass > 3)
				break;
		}
	}
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	return result;
#else
	return width;
#endif
}
