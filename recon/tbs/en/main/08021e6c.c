/* Stubborn (2026-10-07, S10-08021e6c session 3): differ 369 -> 357 (listing 1037, draft 1038, common 859). Not matched. Both content loops are now written whole inside the column loop, vram[panel * 3 + cnt * 32 + (col & 3) - count * 3] = tmp3 = panel * 16 + cnt * 4 + col + 0x100, then tmp3 = col & 3 for the tilemap store *(render->tilemap + (cnt + 17) * 32 + panel * 3 + tmp3 - count * 3 + 22 or 20): this is native's content-loop code in everything but register names. The column loop's invariant index and pattern are hoisted into the row loop, reduced there as givs of cnt and, being used across the column loop, kept as per-row copies (mov ip, rN; str rN, [sp, #4]); tmp3, set twice, is no giv, so the value stays (copy + col) + 256 with movs #128; lsls #1 inside; count * 6 is spilled and panel * 2 is a stack giv, as native. The permuter scaffolding tmp7, tmp2, the u16 * temporary and the tile_row, tile_index and pattern bivs are gone. Left: work in sl and the content loops' count * 3 in r9 (native the reverse, about 150 register rows), the index and pattern row bivs swapped (native index r6, pattern r5), the main-loop 0 for gBgScroll[1].x shared with rebuild = 0 as movs r1, #0 (native loads it from the pool: a spilled hoisted 0), stack slots 4 lower than native from &work->count down, and the mode-0 content loop's exit test loading work->count through a hoisted 432 (native cmp lr, fp with count). */
/* Trial (S10 session 3, from 369): mode-1 content loop whole with tmp3 = value as its own statement before the store: 601; the value chained into the store (address expanded first, native's order): 571; the mode-0 content loop the same: 474; tmp7 removed: 416; the mode-0 blank loop in the mode-1 pointer form: 378; work->count + work->selection - 1 in the up-repeat modulus: 357 (on the 369 body: 376). Worse or equal on 378: tmp7 restored 438; the mode-0 content loop as do-while 399; index or value terms in any order 378 (fold); the tilemap store as tilemap[panel * 3 + cnt * 32 + tmp3 - count * 3 + 0x236] 650. alchemy permute 5 min (2 jobs, seed 21) from 378: weighted 5830 -> 5170 at differ 378 (do-while mode-0 plus the modulus swap), of which only the swap carried. */
/* Stubborn (2026-10-07, S10-08021e6c session 2): differ 369 (listing 1037, draft 1036, common 852), body unchanged. New: the tutorial messages are named in all six editions, so the draft compiles everywhere (Editions line); a per-row pattern giv of cnt gives native's unreduced in-loop value, but not yet native's per-row copies or panel * 2 giv (Trial line, session 2). */
/* Stubborn (2026-10-07, S10-08021e6c session 1): differ 369 (listing 1037, draft 1036, common 852), unchanged; body kept. Diagnosed cause of the content loops (loop.c): native computes each tile value inside the column loop as (pattern copy + col) + 256 (adds; movs #128; lsls #1; adds), neither hoisted nor strength-reduced, and keeps the 3 of col & 3 inside too. The value is a halfword store, so its 256 is (subreg:SI (reg:HI const)): loop pass 1 records no giv for it; the rerun pass hoists that constant whenever its threshold (move_movables: 30 = 2 * (1 + 14 registers) at the loop's first movable, minus 3 for each insn moved before it) times savings 1 times life 1 reaches the column loop's insn count, and then reduces the value as a giv of col (adds rN, #1), which native never does. In every form tried, pass 1 hoists the row part of the index first (threshold 30, 27, 24, 21), so pass 2 sees a column loop of 16-18 insns whose first movable is the 3 at threshold 30: both constants move. Native keeps both, so its pass-2 column loop still held at least three movables ahead of the 3 (30 - 3k below the count), most likely its per-row copies of tile_index and pattern and the row part of the index, which native's final code places just before the column loop (str r5, [sp, #4]; mov ip, r6), so they were still inside the column loop when pass 2 ran and pass 1 did not hoist them. Other editions: the draft does not compile there yet (MsgDjinnTutorial* are English-only symbols). */
/* Trial (S10 session 1, at 369): value written pattern + col + 0x100, 0x100 + pattern + col, col + pattern + 0x100 or (u16)(pattern + col) + 0x100 in the mode-1 content loop: 871 (fold makes them one; reduced in pass 2); u16 pattern 866; through a function-scope s32 tile in both content loops 919 (final value replaceable, reduced in pass 1); per-row copies into the function-scope i and num 871 (CSE propagates them); function-scope index and tile copied per row in both content loops 911, copied at the top of each column iteration 468 (native's per-row index copy appears, pass 2 column loop 17 insns, 3 and 256 hoisted, value reduced); mode-1 content loop written whole, vram[panel * 3 + cnt * 32 + (col & 3) - count * 3] = panel * 16 + cnt * 4 + col + 0x100 and the tilemap store the same way: 627, and its code is native's in everything but the reduced value (row bivs +64/+32/+4 in r4 and two others, the panel*2 giv, count*6 spilled, tile_index copied per row); the same with a block-local u16 *vram = (u16 *)0x0600fd6c: 628, pass 2 still 18 insns; the VRAM index and value written whole with the tilemap store through the tile_row biv: 662 (pass 1 hoists the index's row part at 30-21); function-scope index = panel * 3 + cnt * 32 and tile = panel * 16 + cnt * 4 set inside both content column loops: 925. The column loop as a goto loop (no loop notes) gives native's 26-instruction body (ble -52) but 856 overall. */
/* Trial (S10 session 2, at 369, mode-1 content loop only): pattern computed per row from cnt (s32 pattern = panel * 16 + cnt * 4, fold makes it (panel * 4 + cnt) * 4) with tmp3 = pattern + col + 0x100 kept as the shared temporary (tmp3 = col & 3 after it): 895 (F); with tile_index per row as well, panel * 3 + cnt * 32, tile_row = &tilemap[panel * 3]: 891 (A); the same through a panel-level base = panel * 3: 892 (B); the VRAM index written whole: 880 (E). These give native's column body for the value: (pattern + col) + 256 computed inside the column loop, the movs #128; lsls #1 and the movs #3 both left there (tmp3, set twice, is no giv), but tmp3 is spilled (str to sp+12, then add r1, sp, #12; ldrh), there are no per-row copies (mov ip, r6; str r5, [sp, #4]) because the per-row givs are replaceable, and the panel loop reduces panel * 3 and panel * 6 (or no giv at all) instead of native's panel * 2 at sp+8, so greg moves work from r9 to r8 or sl and the whole function shifts. Without tmp3 (value written in the store): 873 (G), value reduced again (adds r1, #1); pattern declared at panel scope 873; col declared after pattern 873; pattern = base + cnt * 4 with base = panel * 16: 873 (CSE folds it back); index = tile_index + cnt * 32 with tile_index fixed per panel: 888, value reduced. Pass-1 loop dump of G: the column loop records pattern + col as a replaceable giv not worth reducing (0 vs 23) and leaves 256 (not desirable), the row loop records no giv, the panel loop finds panel * 2 not worth while (0 vs 47), where native reduces it. */
/* Editions (S10 session 2): the two tutorial messages are now named in every PO (de, es, fr and it: 03172 MsgDjinnTutorialSummon, 03173 MsgDjinnTutorialUnleashCommand, the ids their pool words load; ja: 03538 and 03539), so the draft compiles in all six editions. de, es, fr and it hold the English function apart from call offsets and pool words (cmp of the 2,300 bytes: 87 differ, all BL halves and literals), so they follow English. ja is 2,304 bytes (scaffold 0x00021ece, 0x902 with the pad) and needs its own branch. alchemy diff --edition cannot score them yet: their scaffolds carry no function symbol (require_thumb_scaffold). */
/* Stubborn (2026-10-07, T08-08021e6c session 4): differ 369 (listing 1037, draft 1036, common 852), unchanged since session 3. Not matched. Native's spill slots, top down: mode 52, render 48, rebuild 44, trade_count 40, scale 36, count*6 (mode 1) 32, count*6 (mode 0) 28, &selection 24, message 20, &work->count 16, panel*2 givs 12/8, pattern copy 4; the draft has message at 32 and tmp7's 0/1 at 28 (six extra instructions after the affine stores). */
/* Trial (session 4, at 369): mode-1 content loop with per-row copies s32 index = tile_index, tile = pattern and tile + col + 0x100: 895; tile + col written col + tile: 369. A separate panel for the mode-1 blank or for the mode-1 content loop (any split of the shared panel): 403. Mode-1 blank limit as 6 - work->count > panel: 373. tmp7 used for the position loop's mode test: 371, for the redraw's if (mode): 371; tmp7 declared first: 369; tmp7 = mode != 0 set before the main loop: 457, before cnt = 0: 382. Closing loop through a per-row u16 *row = render->tilemap + (cnt + 17) * 32 + panel * 3, row[(col & 3) + 5]: 419. alchemy permute 5 min (2 jobs, seed 1) from 369: best weighted 12450 at differ 391, nothing taken. The merge of the parallel T08 session 1 line (ff48490c68, 1022) kept the 369 body and its trial lines. */
/* Stubborn (2026-10-07, T08-08021e6c session 3): differ 369 (listing 1037, draft 1036, common 852). Not matched. The three blank loops now index the 32-wide BG map by row and column (tilemap + (cnt + 17) * 32 + panel * 3 + 4 + (col & 3)), which gives native's panel*2 giv plus panel, its per-row copy (adds r1, r0, #0) and its rematerialized 0x448: the mode-1 blank loop matches except panel in ip and its limit in lr (native the reverse). Left: the two content loops (about 250 of the 369; native spills count*6 to sp+32/sp+28, copies the index and pattern bivs per row to r12 and sp+4, and builds index from a panel*2 giv in sp+8/sp+12), the closing loop's last add folded into strh [r3, r7] (native adds r3, r7; strh [r3]), and &work->count at sp+20 where native has sp+16. Still permuter scaffolding (re-measured at 369, removing each is worse): tmp7 = mode != 0 (437 without; it stands in for the content loops' count*6 spills, the frame is native's 60 only with it), the tmp2 and tmp3 temporaries (578 without), and the u16 * temporary for 0x0600fd68 (458 without). Native copies every tile loop's row pointer or row index once per row (adds r1, r0, #0; mov r12, r6; str r5, [sp, #4]): loop.c keeps such a copy when a giv is not replaceable (loop.c 5520-5565: a biv update inside the giv's lifetime, or the giv's register first set elsewhere); its large constant offsets (0x448, 0x46c) are rematerialized inside the column loops, so they are pseudos left without hard registers, which the draft still allocates. */
/* Trial (session 3): blank and closing loops indexed by map row and column instead of the cnt++-first tile_row: closing loop as ((u16 (*)[32])render->tilemap)[cnt + 17][panel * 3 + 5 + (col & 3)] 462 -> 441; the mode-0 blank the same 431, the mode-1 blank 432, all three 408; the mode-1 blank as *(render->tilemap + (cnt + 17) * 32 + panel * 3 + 4 + (col & 3)) 378 (render->tilemap[panel * 3 + cnt * 32 + 0x224 + (col & 3)]: 450; a function-scope tile_row set per row: 419-421; (col & 3) first in the 2D column: 408); the closing loop in the same pointer form 369 (any term order: 369); the mode-0 blank in pointer form 453-458, flat 1D index 392, so it stays 2D. Content loops: the render clear in the pointer form 584-585, the VRAM store as ((u16 *)0x0600f800)[(cnt + 21) * 32 + panel * 3 + 22 + (col & 3) - count * 3] 555, both 612, the whole loop without its bivs 627; tile_index = panel * 2 + panel 446; tile_row = render->tilemap + panel * 3 in either or both content loops 374-582; function-scope index and pattern set per row in both content loops (to make their givs non-replaceable, native's per-row copies) 904, the givs are no longer reduced at all. panel at function scope: 439-480. */
/* Trial (session 3, at 369): mode-1 blank limit as work->count + panel < 6: 369; as a blank = 6 - work->count variable 422; col at panel scope 418; cnt < 3 and col < 3: 369 (panel stays in ip). Content render clears in the 2D form: mode-1 789, mode-0 470; mode-0 in pointer form 501; 0x234/0x236 first in the tile_row sum: 369. Content declarations reordered (count last 524, pattern first 369, pattern = panel << 4 371). message declared last 409, first 383, inside its branch 369. Closing loop as (map[cnt + 17] + panel * 3 + 5)[col & 3] or 17 + cnt, 5 + panel * 3: 369; the mode-0 blank as (map[cnt + 17] + panel * 3 + 2)[col & 3] 453, 17 + cnt order 369. The mode-1 content loop without its tile_row biv, clearing through *(render->tilemap + (cnt + 17) * 32 + panel * 3 + 22 - count * 3 + (col & 3)): 585; through render->tilemap + tile_index + ...: 859. alchemy permute 5 min from 369: 367, only by sharing two more temporaries in the mode-0 content columns (noise, not taken). Session 2's last permuter run from 462 (seed 14): 470, nothing taken. */
/* Trial (session 2): the mode-1 blank loop's rows and columns, the mode-1 content columns and the mode-0 content rows and columns as plain for loops instead of the permuter's while and if/do-while forms: 462, unchanged, kept. The mode-0 content panels as a for loop: 568, kept as do-while. The mode-1 blank rows counted before their columns: 472; as a do-while with tile_row += 32: 471. */
/* Trial (session 2): the mode-0 content loop's tmp4 = col & 3 removed (col & 3 written in place): 467 -> 462. Then without the u16 * temporary: 516; tmp2 = count * 3 removed as well: 525, and also tmp2 = 0x100 + pattern: 523. tmp3 renamed: unchanged. Mode-1 content loop counting cnt before its columns: 477; with index = tile_index copied per row: 477; index and pattern computed per row from cnt: 908. */
/* Trial (session 2): mode-0 blank and closing loops as for (cnt = 0; cnt <= 2; ) { tile_row = &render->tilemap[panel * 3 + cnt * 32]; cnt++; columns } (cnt++ before the columns makes tile_row non-replaceable, native's per-row copy, though native increments cnt after the columns): 484 -> 473 (closing) -> 467 (both). The same in the mode-1 blank loop: 485; in the mode-1 content loop: 483. Declaration order elsewhere: no effect (every adjacent pair swapped, 484). Function-scope tile_row shared by every loop: 495. Content loops with index and pattern computed per row from cnt: 886-926. */
/* Trial (session 2): mode-1 content loop declares tile_row before pattern (alchemy permute 5 min, seed 11, --heat: one rewrite): 636 -> 484. The same order in the mode-0 content loop: 546. tmp7 removed: 534; the u16 * temporary removed: 716. */
/* Trial (2026-10-07): bisecting the 658 candidate below: removing the scale - 256 chain 657, the gBgScroll pointer 657, rebuild != 0 temporary 659, the four temporaries of the redraw test 643, keys & 1 643, the mode != 0 and repeat & 0x90 temporaries 619, the work->count chain 619; the AffineMatrix_BuildForEffect result shared with runtime->auto_enabled is worth 17 (619 with it, 636 without), dropped as noise. */
/* Trial (2026-10-07): the mode-1 blank and closing loops index each row as tile_row = &render->tilemap[panel * 3 + cnt * 32] (native's panel*2 giv plus panel): 787 -> 785 -> 776. The same in the mode-0 blank loop: 868, reverted. The mode-1 content loop in the permuter's mode-0 form: 899, reverted. */
/* Trial (2026-10-07): alchemy permute 8 min from 922 gave 799; its noise removed by measurement: 787. What carried: the position loop as cnt = 0; if (cnt < count) do { ... } while (as a for or while: 853); the content tiles through s32 tile = 0x100 + pattern, then tile + col (direct 0x100 + pattern + col: 933); the mode-0 content panels as a do-while with panel++ before the rows and one temporary for 0x100 + pattern and count * 3 (as the mode-1 for loops: 928, for loops with the shared temporary: 875); cnt = 0 before the redraw test, the sprite loop as if/do-while (a for loop there: 821). Noise, removed at no cost or a gain: tmp4 for scale - 256, the gBgScroll cast, a temporary for keys & 0x300, the mode-1 blank and closing loops as do-while (787 as for), the resource loop as while, a temporary shared between work->count and the sprite test (removing it: 799 -> 790). */
/* Trial (2026-10-07): per-row index loops (tile_row = &render->tilemap[panel * 3 + cnt * 32] inside the row loop) reproduce native's panel*2 giv plus panel in the blank and closing loops: 922 -> 925, 924. With tile_row, tile_index, pattern, panel and col at function scope shared by all tile loops the frame becomes native's 60 and the slots of mode, render, rebuild and trade_count match, but scale keeps a register (native spills it to sp+36) and the content loops lose strength reduction: 938. Inner copies (s32 index = tile_index, tile = pattern in the row): 922, unchanged. */
/* Trial (session 1): message declared after scale (native spills it to sp+20, below scale's sp+36): 1050 -> 1046. */
/* Trial (session 1): every kinds store through num++, num = 0 inside each branch (native addresses kinds[] as [work, offset] with offsets 456+4n, the shape of a CSE-known index): 1046 -> 1009. */
/* Trial (session 1): repeat = keys = 16 before auto_delay = 40, and the same for 200 (native cross-jumps their auto_delay stores into the decrement's): 1009 -> 992. */
/* Trial (session 1): keys = repeat = N rather than repeat = keys = N (native sets repeat first, keys r5, auto_delay pointer r6): 992 -> 956. */
/* Trial (session 1): cnt as the row counter of all five tile loops (native rows in r8, and WaitFrames(1) after the closing loop reuses the r0 = 1 of add r8, r0): 956 -> 930. The draft still puts cnt in r7 there. */
/* Trial (session 1): register struct SelectionWork *work asm("r9"): 930 -> 1122, reverted. */
/* Trial (session 1): s32 repeat, keys declaration order: no change. pattern + col + 0x100: no change. slot declared before cnt: no change. *(tile_row + (col & 3) - count * 3 + 0x236) = 0 in both tile loops gives native's separate count*6 and unhoisted 3: 1009 -> 1017 before the row change, 930 -> 930 (common 551) after it, kept. */
/* Trial (session 1): the closing queue write as IO_WRITE_QUEUE_PUSH(0x04000000, 0x1541, 0x20000), the shared macro: 930 -> 922. */
/* Trial (session 1): s32 tile_index = panel * 3 in the three blank loops, tile_row = &render->tilemap[tile_index]: 922, unchanged (the giv stays panel*6). */
/* Merged from a parallel session whose body scored 1022 (this body 922, kept): */
/* Trial (T08 session 1): every tile loop computes its row's tile_row (and in the panel loops its tile_index and pattern) from panel * 3 + row * 32 inside the row loop, and the zero store is pointer arithmetic, (tile_row + (col & 3) - count * 3)[0x236]: differ 1050 -> 1022. The native reduces only 2 * panel as an induction variable (adds #2, then adds panel), as the draft now does; the old tile_row += 32 form reduced the whole panel * 6 address. The native tilemap store subtracts count * 6 and adds 0x46c separately in the inner loop, which only the pointer form gives. Row-local blank loops alone: 1036. */
/* Trial (T08 session 1), worse: tile_index, the blank-loop tile_index or tile_index and pattern at function scope (1051-1058); message declared in its inner block (1060, puts mode, render, rebuild and trade_count in consecutive slots as native, but scale still gets a register); scale local to the main loop (1060); rebuild = 0 after the panel loops (1066); tile_index += 32 in the blank loops (1083). */
/* Trial (T08 session 1) on 1022: message declared in the auto_mode == 0 block: 1024, but the spill slots then fall in native's order (mode, render, rebuild, trade_count, scale, &selection, message, &count, ..., pattern), so it is a likely base for the next try. Panel-loop indexes written whole inside the store (panel * 3 + row * 32 + (col & 3) - count * 3, value 0x100 + panel * 16 + row * 4 + col): 1038, frame 56, count * 6 spilled as native and tile_index copied once per row, but the value becomes a col induction variable; same with a row-level pattern: 1055. cnt as the tile row counter: 1057. cnt for the palette and party loops: 1022, unchanged. u16 slot: 1028. i and num reused as tile_index and pattern: 1095. A short permute run on 1022 was stopped in its minimize step; its candidate sums party before BattlePlacement_CountValidEntriesFar fills it, so nothing was taken. */
/* Allocation (greg dump of the 1022 draft): work r8, cnt r6, keys r6, repeat r7, slot r10, num r5, result r10; native: work r9, cnt r8, slot r7, keys r5, repeat r7, num r5, result r10. Native's cnt loses r5-r7 to the fill-loop pointers, slot and the two sprite-loop masks (r6 = 0xfffffe00, r7 = -63); the draft's cnt outranks the masks, so one mask lands in r4 and is saved around Runtime_PushSlotEntry (sp+8). Draft lreg: slot 4 refs over 128 insns, cnt 44 over 252, work 84 over 692. */
/* Native spill slots (1022 session), top down: mode, render, rebuild, trade_count, scale, count * 6 (mode 1), count * 6 (mode 0), &selection, message, &count, then 2 * panel and pattern. In the panel loops native keeps count in r11 and count * 3 in r10, so scale loses its register; it also copies the reduced tile_index (to r12) and pattern (to sp+4) once per row, and rematerializes 0x448 inside the mode-1 blank loop. */
/* Trial: indexed fill, party sum and draw loops (for (cnt = 0; cnt <= 5; cnt++) with the -1 test after the resource store), one (cnt - count) * 24 shared by both pos_x stores: differ 1129 -> 1097. */
/* Trial: scale read from Ui_BounceScales into its own variable, then rescaled: differ 1097 -> 1086. */
/* Trial: the trade kinds counted in their own num, not cnt (native keeps it in r5, cnt in r8): differ 1086 -> 1050. */
/* Trial: cnt as the tile row counter (native rows are in r8), cnt declared first, a separate sprite-loop counter: 1050, unchanged. panel and col at function scope: worse (1114 before num). tilemap[panel * 2 + panel]: worse (1083). Mode-1 blank panels indexed as tilemap[0x224 + panel * 3 + row * 32 + (col & 3)]: worse (1071). */
/* Trial (T08 session 1): every tile loop computes its row's tile_row (and in the panel loops its tile_index and pattern) from panel * 3 + row * 32 inside the row loop, and the zero store is pointer arithmetic, (tile_row + (col & 3) - count * 3)[0x236]: differ 1050 -> 1022. The native reduces only 2 * panel as an induction variable (adds #2, then adds panel), as the draft now does; the old tile_row += 32 form reduced the whole panel * 6 address. The native tilemap store subtracts count * 6 and adds 0x46c separately in the inner loop, which only the pointer form gives. Row-local blank loops alone: 1036. */
/* Trial (T08 session 1), worse: tile_index, the blank-loop tile_index or tile_index and pattern at function scope (1051-1058); message declared in its inner block (1060, puts mode, render, rebuild and trade_count in consecutive slots as native, but scale still gets a register); scale local to the main loop (1060); rebuild = 0 after the panel loops (1066); tile_index += 32 in the blank loops (1083). */
/* Trial (T08 session 1) on 1022: message declared in the auto_mode == 0 block: 1024, but the spill slots then fall in native's order (mode, render, rebuild, trade_count, scale, &selection, message, &count, ..., pattern), so it is a likely base for the next try. Panel-loop indexes written whole inside the store (panel * 3 + row * 32 + (col & 3) - count * 3, value 0x100 + panel * 16 + row * 4 + col): 1038, frame 56, count * 6 spilled as native and tile_index copied once per row, but the value becomes a col induction variable; same with a row-level pattern: 1055. cnt as the tile row counter: 1057. cnt for the palette and party loops: 1022, unchanged. u16 slot: 1028. i and num reused as tile_index and pattern: 1095. A short permute run on 1022 was stopped in its minimize step; its candidate sums party before BattlePlacement_CountValidEntriesFar fills it, so nothing was taken. */
/* Allocation (greg dump of the 1022 draft): work r8, cnt r6, keys r6, repeat r7, slot r10, num r5, result r10; native: work r9, cnt r8, slot r7, keys r5, repeat r7, num r5, result r10. Native's cnt loses r5-r7 to the fill-loop pointers, slot and the two sprite-loop masks (r6 = 0xfffffe00, r7 = -63); the draft's cnt outranks the masks, so one mask lands in r4 and is saved around Runtime_PushSlotEntry (sp+8). Draft lreg: slot 4 refs over 128 insns, cnt 44 over 252, work 84 over 692. */
/* Remaining: register allocation. Native keeps work in r9, cnt in r8, slot in r7, result in r10, spills scale to sp+36 (ldrh of the slot for scale_x), count*6 of both tile loops to sp+32/sp+28 and the panel*2 givs to sp+8/sp+12, and frames 60 bytes; the draft keeps scale in r9, work in r10, slot in r8, cnt in r7 and frames 44. Native also reduces panel*2 alone and adds panel (tile index = giv + panel) in every tile loop, where the draft reduces panel*6 whole. */
/* 08021e6a: two zero bytes before this function are alignment, not code: DISPLAY.o ends at 0x08021e6a and a C object aligns .text to 4, so the linker fills them, as it already does at 0x08021dfa. */
/* Shared UiRenderWork, MenuCell, GlyphPalette, BgScroll and MenuSprite replace
 * private views. Original draft: listing 1037, draft 998, differ 1173; shared
 * views preserve that result, then genuine sprite bitfields improve it to
 * draft 1002, differ 1129. Native frame is 60 bytes; draft is 44. Remaining
 * work is loop/spill layout. Whole tutorial message symbols leave the score
 * unchanged; no new devices.
 */
#include "AFFINE.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "IO_WRITE_QUEUE.H"
#include "WINDOW.H"
#include "GLYPH.H"
#include "MENU_LIST.H"
#include "MAP_SCROLL.H"
#include "IO_REG.H"
#include "text/MSG_IDS.H"

struct UiWindow;

struct SelectionEntry {
    struct MenuSprite sprite;
    u16 resource, unused;
    s32 pos_x, pos_y;
    s32 kind;
};

struct SelectionWork {
    struct GlyphPalette palette;
    struct SelectionEntry entries[6];
    struct UiWindow *window;
    s32 count;
    struct AffineTransform affine;
    s32 matrix, selection, previous;
    s32 kinds[6];
};

struct TradeOfferState { u32 available; };

extern u16 Ui_BounceScales[];

extern struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width,
                                           s32 height, s32 style);
extern void UiWork_Finalize(struct UiWindow *work, s32 release);
extern void RenderOutput_PrepareForRedraw(struct UiWindow *work);
extern void UiText_DrawCharacter(u8 *base, s32 index, u32 kind);
extern struct UiWindow *UiText_ShowMessageAndWaitComplete(s32 message,
                                                           s32 x, s32 y);
extern void Runtime_SetIrqHandler(u32 irq, s32 line, void (*handler)(void));
extern void Graphics_SetBg1Priority3(void);
extern void Graphics_ClearBg1ControlBit2(void);
extern struct TradeOfferState *Trade_GetOfferStateFar(s32 index);
extern void BattlePlacement_CountValidEntriesFar(s32 index, u8 *party);
extern void BattlePresentation_WaitForPromptAtFar(s32 x, s32 y);
extern void Audio_PlayCue(s32 cue);

s32 Ui_RunSelectionScreen(s32 mode)
{
    u8 party[4];
    struct UiRenderWork *render;
    struct SelectionWork *work;
    struct MenuCell *runtime;
    s32 rebuild, trade_count, scale, result;
    struct UiWindow *message;
    s32 cnt;
    s32 keys, repeat;
    s32 slot;
    s32 i, num;

    render = (struct UiRenderWork *)gWindowWork[0];
    slot = Resource_LoadIntoFreeSlot(0x400);
    rebuild = 1;
    trade_count = 0;
    work = Runtime_BumpAllocateAlternatePool(sizeof(*work));
    for (i = 255; i >= 0; i--)
        work->palette.slot[i] = 255;
    work->palette.count = 1;
    if (mode == 0)
        work->window = UiWindow_Create(20, 17, 10, 3, 6);
    else
        work->window = UiWindow_Create(22, 17, 8, 3, 6);
    work->previous = -1;
    if (mode == 0) {
        num = 0;
        work->kinds[num++] = 14;
        work->kinds[num++] = 4;
        work->kinds[num++] = 7;
        work->kinds[num] = -1;
        work->selection = gLinkCountdownWork->command_selection[0];
    } else {
        work->selection = gLinkCountdownWork->command_selection[1];
        num = 0;
        work->kinds[num++] = 0;
        work->kinds[num++] = 1;
        if (Trade_GetOfferStateFar(0)->available != 0) {
            BattlePlacement_CountValidEntriesFar(0, party);
            for (i = 0; i <= 3; i++)
                trade_count += party[i];
            work->kinds[num++] = 15;
            if (trade_count != 0)
                work->kinds[num++] = 16;
        }
        work->kinds[num++] = 2;
        work->kinds[num++] = 3;
        work->kinds[num] = -1;
    }
    for (cnt = 0; cnt <= 5; cnt++) {
        work->entries[cnt].resource = slot;
        if (work->kinds[cnt] == -1)
            break;
        UiText_DrawCharacter((u8 *)work, cnt, work->kinds[cnt]);
    }
    work->count = cnt;
    work->affine.scale_x = 320;
    work->affine.scale_y = 304;
    work->affine.angle = 0;
    cnt = 0;
    if (cnt < work->count) {
        do {
            s32 x = (cnt - work->count) * 24;
            work->entries[cnt].pos_x = x + 155;
            if (mode != 0)
                work->entries[cnt].pos_x = x + 171;
            work->entries[cnt].pos_y = 136;
            cnt++;
        } while (cnt < work->count);
    }
    Scheduler_AddOrUpdateCallback(Graphics_SetBg1Priority3, 0x480);
    Runtime_SetIrqHandler(2, 136, Graphics_ClearBg1ControlBit2);

    for (;;) {
        scale = Ui_BounceScales[(gFrameCount * 2) & 31];
        scale = (scale - 256) / 4 + 304;
        work->affine.scale_x = scale;
        work->affine.scale_y = scale;
        gBgScroll[1].x = 0;
        gBgScroll[1].y = 32;
        if (rebuild != 0) {
            rebuild = 0;
            render->menu_busy = 1;
            if (mode) {
                s32 panel;
                for (panel = 0; panel < 6 - work->count; panel++) {
                    for (cnt = 0; cnt <= 2; cnt++) {
                        s32 col;
                        for (col = 0; col <= 2; col++)
                            *(render->tilemap + (cnt + 17) * 32 + panel * 3 + 4 + (col & 3)) = 0xf07f;
                    }
                }
                for (panel = 0; panel < work->count; panel++) {
                    s32 count = work->count;
                    for (cnt = 0; cnt <= 2; cnt++) {
                        s32 col;
                        for (col = 0; col <= 2; col++) {
                            s32 tmp3;
                            ((u16 *)0x0600fd6c)[panel * 3 + cnt * 32 + (col & 3) - count * 3] = tmp3 = panel * 16 + cnt * 4 + col + 0x100;
                            tmp3 = col & 3;
                            *(render->tilemap + (cnt + 17) * 32 + panel * 3 + tmp3 - count * 3 + 22) = 0;
                        }
                    }
                }
            } else {
                s32 panel;
                for (panel = 0; panel < 6 - work->count; panel++) {
                    for (cnt = 0; cnt <= 2; cnt++) {
                        s32 col;
                        for (col = 0; col <= 2; col++)
                            *(render->tilemap + (cnt + 17) * 32 + panel * 3 + 2 + (col & 3)) = 0xf07f;
                    }
                }
                for (panel = 0; panel < work->count; panel++) {
                    s32 count = work->count;
                    for (cnt = 0; cnt <= 2; cnt++) {
                        s32 col;
                        for (col = 0; col <= 2; col++) {
                            s32 tmp3;
                            ((u16 *)0x0600fd68)[panel * 3 + cnt * 32 + (col & 3) - count * 3] = tmp3 = panel * 16 + cnt * 4 + col + 0x100;
                            tmp3 = col & 3;
                            *(render->tilemap + (cnt + 17) * 32 + panel * 3 + tmp3 - count * 3 + 20) = 0;
                        }
                    }
                }
            }
        }
        if (work->previous != work->selection) {
            RenderOutput_PrepareForRedraw(work->window);
            UiText_DrawCharacterAtOffset(work->entries[work->selection].kind + (s32)&MsgCommandName,
                                         (struct RenderInput *)work->window, 0, 0);
            work->previous = work->selection;
            UiText_DrawCharacter((u8 *)work, work->selection, work->kinds[work->selection]);
        }
        cnt = 0;
        work->matrix = AffineMatrix_BuildForEffect(&work->affine);
        if (cnt < work->count) {
            do {
                struct SelectionEntry *entry = &work->entries[cnt];
                if (cnt == work->selection) {
                    entry->sprite.oam.f.affine_index = work->matrix;
                    entry->sprite.oam.f.affine = 3;
                    entry->sprite.oam.f.x = entry->pos_x + scale * 7 / 512 - 14;
                    entry->sprite.oam.f.y = entry->pos_y + scale * 3 / 256 - 20;
                    Runtime_PushSlotEntry(entry, 0xf1);
                } else {
                    entry->sprite.oam.f.x = entry->pos_x;
                    entry->sprite.oam.f.y = entry->pos_y;
                    entry->sprite.oam.f.affine_index = 0;
                    entry->sprite.oam.f.affine = 0;
                }
                cnt++;
            } while (cnt < work->count);
        }
        keys = gKeyState;
        repeat = gKeysRepeat;
        runtime = gLinkCountdownWork;
        if (runtime->auto_enabled != 0) {
            keys = repeat = 0;
            if (runtime->auto_delay == 0) {
                if (runtime->auto_mode == 1) {
                    if (work->kinds[work->selection] == 3)
                        keys = repeat = 1;
                    else
                        keys = repeat = 32;
                    runtime->auto_delay = 30;
                } else if (runtime->auto_mode == 0) {
                    s32 kind = work->kinds[work->selection];
                    if (kind == 16 || (trade_count == 0 && kind == 15)) {
                        if (runtime->auto_enabled == 1) {
                            if (kind == 15)
                                message = UiText_ShowMessageAndWaitComplete((s32)&MsgDjinnTutorialUnleashCommand, 15, 8);
                            else if (kind == 16)
                                message = UiText_ShowMessageAndWaitComplete((s32)&MsgDjinnTutorialSummon, 15, 8);
                            BattlePresentation_WaitForPromptAtFar(102, 155);
                            UiWork_Finalize(message, 1);
                            runtime->auto_enabled++;
                            runtime->auto_delay = 45;
                        } else {
                            keys = repeat = 1;
                            runtime->auto_delay = 200;
                        }
                    } else {
                        keys = repeat = 16;
                        runtime->auto_delay = 40;
                    }
                } else {
                    runtime->auto_delay = 60;
                    keys = repeat = 1;
                }
            } else {
                runtime->auto_delay--;
            }
        }
        if ((keys & 0x300) && mode != 0) {
            result = -2;
            break;
        }
        if (keys & 1) {
            result = work->kinds[work->selection];
            break;
        }
        if (mode != 0 && (keys & 2)) {
            Audio_PlayCue(0x71);
            result = -1;
            break;
        }
        if (repeat & 0x90) {
            Audio_PlayCue(0x6f);
            work->selection = __modsi3(work->selection + 1, work->count);
        } else if (repeat & 0x60) {
            Audio_PlayCue(0x6f);
            work->selection = __modsi3(work->count + work->selection - 1, work->count);
        } else if (gLinkCountdownWork->active == 0) {
            result = work->kinds[0];
            break;
        }
        QueueIoWriteDelay2(0x04000000, 0x1741);
        render->menu_busy = 0;
        WaitFrames(1);
    }
    if (mode != 0)
        gLinkCountdownWork->command_selection[1] = work->selection;
    else
        gLinkCountdownWork->command_selection[0] = work->selection;
    for (cnt = 0; cnt < work->count; cnt++)
        Resource_ResetEntry(work->entries[cnt].resource);
    render->menu_busy = 1;
    UiWork_Finalize(work->window, 1);
    {
        s32 panel;
        for (panel = 0; panel <= 6; panel++) {
            for (cnt = 0; cnt <= 2; cnt++) {
                s32 col;
                for (col = 0; col <= 2; col++)
                    *(render->tilemap + (cnt + 17) * 32 + panel * 3 + 5 + (col & 3)) = 0xf07f;
            }
        }
    }
    render->dirty = 1;
    WaitFrames(1);
    Scheduler_RemoveCallback(Graphics_SetBg1Priority3);
    Runtime_SetIrqHandler(2, 0, 0);
    IO_WRITE_QUEUE_PUSH(0x04000000, 0x1541, 0x20000);
    Runtime_BumpFree(work);
    render->menu_busy = 0;
    WaitFrames(1);
    return result;
}
