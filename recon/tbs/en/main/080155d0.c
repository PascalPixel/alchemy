/*
 * UiText_DrawGlyphArm: all-edition ARM C draft, not linked or credited.
 * Re-derived from the current listing and our ROMs on 2026-10-09.
 * Stock agbcc_arm with K1's ARM options, measured with:
 *   alchemy diff recon/tbs/en/main/080155d0.c moji --game tbs
 *                --edition tbs-en --arm --rtl --show
 * Native / draft / common / residual:
 *   ja: 193 / 193 / 185 / 16
 *   en: 196 / 196 / 173 / 46
 *   de: 187 / 187 / 176 / 22
 *   es: 187 / 187 / 176 / 22
 *   fr: 187 / 187 / 176 / 22
 *   it: 187 / 187 / 176 / 22
 * Frames and spill offsets now agree: Japanese 100 bytes, all others 44.
 * Japanese's 64-byte unused area still needs the tagged source type below.
 * Multiply operands agree. Japanese and the four European editions differ
 * only in scheduling and consequent pool displacements; English also
 * differs in short-lived register allocation.
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
 *    In English, expand_preferences propagates font_width's return-register
 *    preference 0 into character-32, which dies in its table load. Simple
 *    aliases disappear before this pass. A surviving intermediate or a
 *    later last use of the index is a specific next allocation experiment.
 * S: sched2. sched.c schedule_block schedules backwards; rank_for_schedule
 *    compares priority, dependency class, then original LUID. Source order
 *    alone cannot change an unequal priority or a data/anti dependency.
 *    Native order requires another lifetime/dependency or a scheduling
 *    boundary that survives optimization. The remaining dot and row-tail
 *    sites have not yet been repaired this way.
 *    In the earlier European dot's sched2 graph, pixel shift and address
 *    OR tie at priority 17. At T-8 the former wins because it is
 *    independent of mask=15, while the latter has an anti
 *    dependence through r3. This is not merely an original-order tie:
 *    matching that site needs another RTL dependency or a late-removable
 *    instruction/block, rather than another ordering of those same SETs.
 * J: jump2 cross-jumping requires equal instruction tails after sched2;
 *    arm.c's literal-pool pass then computes distances from the final code.
 *
 * Japanese: eight moved instructions, accounting for all 16 residual entries.
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
 * - .L_font_row (S): cmp ip,r5 must precede add lr,r2,#1, not follow it.
 *   A different rownext lifetime must change their scheduling priority.
 * - .L_glyph (S): ldrh r0,[r3,r1] must follow add rows,#2 and its spill
 *   to [sp,#28]; draft loads first. A later width-load dependence is needed;
 *   expressing rows++ followed by rows[-1] instead creates a postincrement.
 * - .L_row_next (S): lsl ip,ip,#16 must precede str next,[sp,#8]; draft
 *   reverses them. The free one-cycle shift wins the last slot before the
 *   branch over the two-cycle store in stock sched2. A surviving constraint
 *   or late transform is needed; jump2's register-set hoists cannot hoist
 *   this store. This observation does not prove that plain C cannot match.
 * - Both dot bodies and .L_shift are resolved by the measured boundaries
 *   below. They no longer contribute residual instructions.
 *
 * German/Spanish/French/Italian: all have the same 11 residual entries on
 * each side (22 total), solely order and consequent pool displacements.
 * The following exhausts the --show differences.
 * - Entry (S): mov ip,r1; sub sp,sp,#44; ldr font,[sp,#72] are early.
 *   Native copies x and y before the frame adjustment and font/character
 *   setup. The height spill must follow cmp r3,#0; the ldrh now follows
 *   movls r3,#1 as required. Argument lifetimes and the free-compare
 *   constraint must change without disturbing x=r9,y=r8.
 * - Font setup (S): source spill [sp,#24] must follow color1's final add;
 *   color1 spill [sp,#20] must follow color3's initial mov. mov r1,#0
 *   must precede the glyph-width pool load, not follow the width spill.
 *   Requires different color/row initialization lifetimes. The pool-load
 *   displacement difference is consequent on that last instruction move.
 * - Packed-font dot (S): address ADD and OR must follow x's AND/LSL.
 *   The main glyph dot now matches, but the packed-font clipping expression
 *   changes x/index lifetimes, and sharing its new helper breaks allocation.
 *   This path still needs a compatible lreg lifetime and sched2 boundary.
 * - .L_row_next (S): the same shift/store reversal as Japanese.
 * - .L_glyph/.L_wide now match: the one-pass width read preserves the
 *   needed scheduling boundary, and returning width directly gives r3
 *   through reload. jump2 shares str r3,[sp,#32] before the row reload and
 *   pass initialization. The former short width's return-register preference
 *   no longer controls either load. This is resolved, not a remaining site.
 *
 * English: 23 native and 23 draft residual entries (46 total). Its
 * long-lived drawing registers agree, and instruction counts are equal.
 * - Entry (A,S): character-32 still gets r0, native ip. The height block
 *   keeps window in incoming r0 until ldrh, removing the former reload.
 *   But index is now computed after the height spill rather than before
 *   its load, and cmp follows that spill rather than preceding it. Index
 *   needs a lifetime/preference overlapping height in r0, without forcing
 *   window into another register. The frame adjustment and window spill
 *   also need to move after the native argument copies. The direct range
 *   return now emits native movhi r0,#1 / bhi with no result spill.
 * - Character-222 uses ip instead of lr at entry, .L_glyph and .L_shift
 *   (A): another overlapping lifetime/preference consequence of the above.
 * - Font setup (S): registers now agree, but all of color3's mov/add/store
 *   precede color1's mov/add/store. The width-table pool load splits
 *   color3's initialization. Native interleaves the two color constants,
 *   spills them, then loads the table. mov r2,#0 is after the width spill,
 *   native before the width byte load. Different initialization lifetimes
 *   are still needed. The width spill now follows its compare, and the
 *   .L_font_dot source-pointer reload is the native r3.
 * - .L_glyph (A,S,J): multiply-by-26 uses r0 instead of ip; its width ldrh
 *   precedes rows += 2 and the rows spill. Native shares that ldrh with
 *   .L_wide; the draft's two sched2 tails differ and jump2 keeps both loads
 *   (one extra instruction). Delaying this load until after the spill
 *   would expose equal tails to cross-jumping; duplicated base expressions
 *   worsened English to 125. The required surviving C dependence is unknown.
 *   Conversely .L_wide reuses the named index and omits native sub r3,r1,#32.
 *   This CSE/lifetime difference offsets the extra ldrh in the instruction
 *   count. Recomputing character-32 there restores the subtraction but
 *   worsened the earlier 52-residual form to 57; both sites must be solved
 *   together rather than judging this instruction count alone.
 * - Packed-font dot and .L_row_next (S): the same three moves as Europe.
 *   The main glyph dot and .L_shift now match.
 * - Remaining branch and literal-load displacement differences (J) follow
 *   from the different glyph-branch extents and moved width-table load. They
 *   are not separate constants to encode in source. Literal pools and the
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

	/* FAKEMATCH: reuse the tile index for the byte address. A separate
	 * destination lifetime raised en/other to 277/271 in the earlier
	 * volatile-spill trial; scalar reuse also removes the Japanese lifetime. */
	tile = (x >> 3) * (stride - 2) + tile_y;
	line = row_line | ((x & 7) >> 2);
	shift = (x & 3) << 2;
	mask = 15 << shift;
	tile = (u32)vram + (tile << 5);
	tile |= line << 1;
	*(u16 *)tile = (*(u16 *)tile & ~mask) | (mask & color);
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
	/* FAKEMATCH: a one-pass height read preserves its scheduling boundary.
	 * With the European predicate above and English's direct error return,
	 * this lowers ja/en/other from 30/65/28 to 26/52/26. */
	do
	{
		stride = window->height;
	} while (0);
#if defined(TBS_EDITION_EN)
	/* FAKEMATCH: retain the index through the wide-glyph expression.
	 * Recomputing it there raises English's residual from 52 to 57. */
	index = character - 32;
	if (index > 111)
		return 1;
#endif

#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	if (character <= 255 || (font != NULL && character - 222 <= 1))
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
					rownext = row + 1;
					if (bits != 0)
					{
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
			color1 = 0x1111;
			color3 = 0x3333;
			row = 0;
			font_width = UiText_GlyphWidths[character - 32];
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
		rows = (const u16 *)((const u8 *)UiText_Glyphs + (character - 32) * 26);
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
		width = *rows++;
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
	else
	{
#if defined(TBS_EDITION_JA)
		width = 10;
		rows = (const u16 *)(UiText_KanjiGlyphs + (character - 256) * 24);
#else
#if defined(TBS_EDITION_EN)
		rows = (const u16 *)((const u8 *)UiText_Glyphs + index * 32);
#else
		rows = (const u16 *)((const u8 *)UiText_Glyphs + (character - 32) * 32);
#endif
		width = rows[0];
		rows += 4;
#endif
	}
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	if (character - 222 <= 1)
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
