/* Thirteenth pass (2026-10-07, 39 to 2). Search: the native first probe is selected[k] with k folded to 0 late (combine, from r1's last value) and its 228 in a pseudo (movs r2, #228; no reload), and the native loop is [k++; k > 7 exits; probe] entered after that probe, with no duplicated exit test. Writing the probe out as selected[k] before for (;;) { k++; if (k > 7) break; probe } gives exactly that shape, with no jump.c duplicate test to fold (a for loop's copied test k > 7 stays, the 28 form below). k's copy k = redraw survives only with: k in r1 (a pseudo k is a biv loop.c strength-reduces, 37), a CSE1 block end after the clear call (an empty one-pass block; without it CSE1 deletes the second zero, 29), a second redraw = 0 in the search block (gcse cprop skips a copy whose source is set earlier in its block; cse2 deletes it as a no-op, 29 without), and a volatile asm("" :) that flushes cse2 before the copy (29 without). The flush and the redundant zero are only order/register devices under S2 as amended (Pascal, 2026-10-07): they decide that the ldr r1, [sp, #28] exists, so this form needs a ruling before adoption. Probe as selected[0] 16 (adds r3, #228; ldr r3, [r3] instead of the index register). Id bases: asm("" : "+r"(n) : "r"(base)) at all three sites (12 to 6): base stays a pseudo in r3 (combine cannot fold a register the asm also reads), and its load now feeds the add at cost 2 instead of an asm (a link into an unrecognised insn is free in haifa's insn_cost), so it ties lsls at priority 38 and wins on dependants. 0xf335 call: an empty one-pass block after it (6 to 2): its loop notes make the dirty update a sched2 barrier, so the post-call writes of r1, r2, r3 stop counting as dependants of the argument insns and rank_for_schedule gives str, r0 copy, subs r2, ldr r1, subs r3 as native. Remaining (2): cursor x, mov r2, sl (the reload of row for row * 2) and subs r3, #2 tie at priority 70, class 3, three dependants each, and luid puts subs first; trials: x - 2 split into two statements 35, (list_win->y + row * 2) 16, -2 + x * 8 2 (same), row * 2 first 14, an asm("" : : "l"(row)) use between the shift and the subtract 2 (mov r2, sl then lands in the ldrh delay slot before lsls). Rule: S2 now admits an empty one-pass block that restores a block boundary the bytes prove (Pascal, 2026-10-07). */
/* Twelfth pass (2026-10-07, 39, no gain). Messages: each PO file numbers its own edition's messages, so the four ids are per edition: en 02285 "You have no Djinn.", 02287 "Cannot use the same Djinni twice.", 02200 "Your Djinni is still recovering.", 02201 "Ready to summon."; ja 02287 ジンをもっていません, 02289 同じジンは2かい えらべません, 02202 リカバリー中です, 02203 しょうかん スタンバイじょうたいです (all eight are unnamed "message" entries); in DE, ES, FR and IT the ids 02285, 02287, 02200 and 02201 hold other text (class names, "?"), so those four copies carry their own ids in their pools, still to be read from each copy before the names are added to those PO files. Trials: for (k = is_used = redraw; ...) with no separate is_used = 0 39 (same code: CSE folds both to 0); k = redraw; for (;;) { probe; if (++k > 7) break; } 132 (the loop starts with the probe's exit, so no first probe is peeled before it and the native's movs r2, #228 probe is lost). */
/* Eleventh pass (2026-10-07, 39, no gain). Editions located (alchemy similar --editions over all twelve builds; alchemy diff cannot score them, as each sits inside an unbounded scaffold run): tbs-ja 08023ecc, tbs-de 08022b34, tbs-es 08022f58, tbs-fr 08022e14, tbs-it 08023ec4, each 2756 bytes. de, es, fr and it are relocated copies of the English: only literal-pool words and call targets differ, so one English body serves them once its message ids are names. tbs-ja differs from the English in 8 instructions, all edition values: the CopyMessageString count is 32 for 52 at all five calls (TEXT_COUNT under EDITION_INTERNATIONAL, as COUNTDWN.C has it), and UiText_DrawNumberAtOffset takes 2 and 40 for 1 and 48. Its pool differs in the four message ids, each 2 higher in ja (en 0x8ed "You have no Djinn.", 0x8ef "Cannot use the same Djinni twice.", 0x898 "Your Djinni is still recovering.", 0x899 "Ready to summon."; all are unnamed "message" entries in the PO files, so adoption names them in all six), in ctx->dirty and ctx->menu_busy at 0xf33/0xf36 for 0xea3/0xea6 (WINDOW.H's edition layout), and in relocated data. The 0x666 and 0x45f id bases are the same in ja. CSE, read again in cse.c: cse_end_of_basic_block ends a scan only at a CODE_LABEL reached by fall-through (and, in CSE1, at NOTE_INSN_LOOP_END); a branch is followed only when its label has one use (behind a BARRIER, or around a skipped block, where invalidate_skipped_block is used instead of record_jump_equiv). By the source's structure every path that reaches the search also passes the join's redraw = 0, since the redraw block starts at the label the loop-top tests jump to. cse_basic_block also flushes its table after 1000 insns of one path, but the search lies well under 1000 insns after that label, so the flush does not explain the native copy either. reload_cse_simplify_set (reload1.c) puts only a constant or a register in place of a source, never a stack slot, so the native ldr r1, [sp, #28] confirms that k = (reg redraw) reached reload. Combine (combinable_i3pat) refuses the 0xf335 width - 2 at the r2 move because the move before it sets r1 (sets_function_arg_p on prev_nonnote_insn); the argument moves are emitted in register order (r0 copy, r1, r2 in the sixth pass dump), so the r2 move always follows the r1 move. So the native copy needs three things at once: CSE1 and cse2 must not see redraw = 0 in the copy's extended block, which takes a label at the start of the search block (or a CSE1 loop end plus a cse2 flush, as the 28 trial does), and gcse cprop must not fold the copy, which it skips only when redraw is set earlier in the copy's own block. Trials: do { UiWork_ClearValueNameTables(); } while (0) with for (k = redraw; ...) and no other change 81 (CSE1 stops at the loop end, but cprop folds the copy and the copied test from the join's zero); alchemy permute rejects this draft (it does not permute inline assembly); a 5-minute run on a scratch copy with the asm wrapped in PERM_IGNORE and the clear call, is_used = 0 and the search in PERM_RANDOMIZE compiled 2,001 candidates and none beat the draft (39, weighted 610). */
/* Tenth pass (2026-10-07, 39, no gain). This agscc runs jump, cse, addressof, gcse, loop, cse2, cfg, life, combine, ce, regmove, lreg, greg, flow2, ce2, sched2, jump2: cross-jumping comes after sched2. In the .gcse dump the search's duplicated first probe (insns 3292-3300, offset 228 already folded) sits before NOTE_INSN_LOOP_BEG in bb 31 with the clear call, is_used = 0 and k's init (insn 542), so whatever folds the probe sees the k init in the same block; the native copy needs a pass that knows k = 0 for the probe but keeps k = (reg redraw), not found. Trials: redraw = 0 at the end of the sub_sel != 0 arm, else redraw = sub_sel, with for (k = redraw; ...) 97 (CSE folds sub_sel to 0 from the jump equivalence, the two arms keep r2 and r3 so jump2 does not cross-jump them, and k is still movs r1, #0). A CSE flush is asm("" :) (an operand asm with no outputs is a volatile ASM_OPERANDS, and cse_insn calls flush_hash_table on it; plain asm("") is an ASM_INPUT and does not). is_used = 0; asm("" :); for (k = redraw; ...) 81: CSE1 keeps the copy and writes the duplicated test as redraw > 7, but gcse cprop folds both from the join block's redraw = 0. The same with redraw = 0 moved from the join into the search block before the flush 85: for the first time the native ldr r1, [sp, #28] and the folded movs r2, #228 probe both appear (cprop skips the copy because redraw is set earlier in its block, and folds the probe in the block after the duplicated test through k = redraw and that block's zero), but the zero is stored after the clear call instead of before the cnt test, and the duplicated test stays (cmp r3, #7; bgt). Keeping both zeros 81 (CSE1 deletes the second as a no-op set). So the native shape needs a zero set in the search block that CSE1 keeps and a later pass deletes, plus a fold of the duplicated test. The first half is found: CSE1 (not cse2) ends its run at a NOTE_INSN_LOOP_END (cse_end_of_basic_block, ! after_loop), so with the join's redraw = 0 kept, the clear call wrapped as do { ... } while (0), then is_used = 0; redraw = 0; asm("" :); for (k = redraw; ...): CSE1 keeps the second zero, cprop skips the copy and folds the probe, cse2 deletes the second zero as a no-op, and the native store before the cnt test, ldr r1, [sp, #28] and movs r2, #228 all appear (72). What remains there is the duplicated exit test (ldr r1, [sp, #28]; cmp r1, #7; bgt before the probe): it sits in the copy's block after the flush, so neither CSE pass knows k and cprop skips it; the native needs a block boundary or a CSE fact between the copy and that test. With k also a fixed-register variable, register s32 k asm("r1"), that form scores 28 (29 without the asm("" :) flush): the only search-site difference left is that duplicated test (ldr r1, [sp, #28]; cmp r1, #7; bgt.n +42 before the probe, the native has the load after the probe and no test), and with its 6 bytes the pool offsets, plus the known lsls r5 (0x45f), 0xf335 and cursor x schedule pairs. The 39 body is kept because the do-while (0), the redundant second redraw = 0 and the flush decide which instructions exist, which S2 does not allow a device to do; the next step is a plain source shape with the same effect (a CSE1-only boundary and a zero set in the search block), and a fold of the duplicated test. At the 0x45f site a block-local yy = j * 16 before the base 39 (same code), after the hold 375. Editions: tbs-ja and tbs-de have no Battle_SelectAbility symbol (08023e70 there is other code), so per-edition scores need the copies located first (alchemy similar --editions over all twelve builds). */
/* Ninth pass (2026-10-07, 39, no gain). Merged the parallel branch above (its body was at 55; the 39 body is kept). Trials at 39: the detail 0xf335 width as width - 1 - 1 or (s32)width + -2, and the cursor x as x * 8 - 1 - 1, all 39 (same code); the search bound k < 8 39; k = redraw 39 (CSE still folds it); the probe written cur == selected[k] 43; the search indexed by the function-scope i 91; one function-scope k shared by the djinn scan and the search 80, with k = redraw also 80; at the 0x45f site, j << 4 for both j * 16 39 (same code) and the function-scope y = j * 16 after the base hold 456 (y spills); is_used = 0 before the clear call 127. Frame: sub_cnt (address taken) at [sp, #80], buf at 84 and the cursor sprite at 212 are the frame objects, so [sp, #28] is a reload spill slot and the native k load is the reload of a pseudo copy, as found before. cse.c follows at most PATHLENGTH (10) branches per path, but only the cnt test lies between the native zero store and the search, so the path does not explain it. */
/* Eighth pass (2026-10-07, 45 to 39). The start schedule (cause 3) is closed by two tagged empty asm barriers: one after list_win = UiWindow_Create (mov r9, r0 first, and movs r6, #0 now precedes the tbl store as native), one before UnitGetFar (the owner reload ldr r0, [sp, #76] last); the first alone gives 41, the second alone 41. Remaining (39): the k = redraw search-index load with its pool and branch offsets (about 30 lines), lsls r5, r7, #4 at the 0x45f site, and the subs ties at the detail 0xf335 call and the cursor x. On (1): with redraw = 0 in both arms of the sub_sel block, jump2's cross-jump would merge the two stores into the native's single store at the join, and CSE stops at the join label, but gcse cprop hashes the two identical sets as one expression available on every path and still folds k; whatever folds the first probe to selected[0] also folds the copy, so a form that keeps the copy needs a pass that folds the probe but not k (not found). Trials: u8 or s16 redraw with k = redraw 45 (same code); asm("") after the 0x45f base hold 377; asm volatile on that hold 39 (same count); row held in a lo register by asm before cursor x 61. */
/* Seventh pass (2026-10-07, 45, no gain). (b) measured in cse.c: cse_insn always rewrites k = redraw to k = 0 while redraw's class holds const 0 (the class element const 0 costs 0, a pseudo source 1; fold_rtx returns a REG unchanged, and the src_const test at the end replaces it too); only a user hard-register source (CHEAP_REG, cost 0) would tie and be kept, which the native's [sp, #28] slot rules out. insert_regs also puts redraw into mask's quantity (both set to 0 on the path; make_regs_eqv keeps mask first because mask's last use, the blink loop, is later), so canon_reg turns any asm input of redraw into mask ([sp, #40]): that is why every asm device on k or redraw loads mask. (a) the combine blocker is combine.c's sets_function_arg_p (prev_nonnote_insn (i3)) test (SMALL_REGISTER_CLASSES). Trials: asm("" : "=r"(redraw) : "0"(0)) for the zero, k = redraw, explicit selected[0] probe then while (++k <= 7) 133 (asm input becomes mask; loop.c strength-reduces the explicit loop); redraw = 0 plus asm("" : "+r"(redraw)), k = redraw, explicit probe, for (k++; ...) 161 (redraw stored from mask, and jump.c duplicates the loop's exit test again); cursor_pos.y before cursor_pos.x 65; 0xf335 width as -2 + width 45 (same code). */
/* 08023e70: differ 45 of 1201 (2026-10-06, sixth pass, no gain; diagnoses). (a) 0xf335 call, cause found: in the native the - 2 sits at the r2 argument move, not where precompute_register_parameters emits it. The draft's RTL is 2180 ldrh, 2182 (reg 584) = 583 + -2, 2187 str (stack arg), 2189 r0 copy, 2191 r1 = 0xf335, 2193 r2 = 584; combine refuses to merge 2182 into 2193 because combinable_i3pat (combine.c, SMALL_REGISTER_CLASSES) rejects an i3 setting an argument register whose prev_nonnote_insn (2191) also sets one. With the add at 2193's place, rank_for_schedule gives str, r0 copy, subs r2 (class 3 against ldr r1's anti link on the copy), ldr r1, subs r3: the native order. So the native's - 2 reached the r2 move some other way (args[2].value not a forced register, or an insn between the r1 and r2 moves); source form not yet found. (b) k = redraw: reload_cse_simplify_set (reload1.c) only substitutes a register or a constant, never memory, so the native's ldr r1, [sp, #28] is the reload of a pseudo copy (set k (reg redraw)) that reached reload. cse1 and cse2 fold that copy while redraw = 0 is on their path, and gcse cprop's find_avail_set follows copy chains and only skips a use whose register was set earlier in the same block: a constant store before the cnt test (where the native has it) is available at the search block's entry, so cprop folds 538 even with CSE blinded; one inside the block is folded by cse2. Yet the duplicated first probe (jump.c duplicate_loop_exit_test copies the exit test and body up to the break) needs k == 0 for movs r2, #228. No plain constant store of redraw satisfies both: the native's zero is probably not (set (reg) (const_int 0)) until after cse2, or k's copy is made after cse2. (c) start: in sched2, mov r9, r0 (list window, insn 94) has priority 35 (one anti dependent, the owner reload ldr r0, [sp, #76]) against 39 for adds r5, #168 (the MenuCell read chain to the str [sp, #36] before the call); the native puts it first. Trials: redraw = 0 after the search (before the menu_busy reset) 191; asm("" : "+r"(redraw)) after the zero 728 (first probe no longer folds: lsls/adds instead of movs r2, #228; global allocation shifts); a block-local MenuCell pointer for page, row and preferred_row 45 (same code). */
/* Fifth pass (45). Measured causes, from the sched2 dependence tables and rank_for_schedule (priority, then class against the last scheduled insn, then the count of dependents, then LUID): (1) start: after the Runtime_BumpAllocateAlternatePool call, str r0 (tbl), str r1 (cnt), str r2 (prev_page) and movs r6, #0 (insn 45, code's zero, which CSE reuses for the sub_cnt store) all have priority 134, class 3 and five dependents, so LUID puts movs r6 last; the native needs movs r6 first, and code = 0 cannot move ahead of the tbl copy without reload_cse storing cnt's zero from r6 (all 720 orders of the six zero inits measured: the current order 45, every other 47 or worse). (2) detail-window 0xf335 call: subs r2, #2 (insn 2182), str r3 and the r0 copy tie at 43 with two dependents each; 2182's second dependent is an anti link from the ldrh r2 after the call; by the same rules, one dependent fewer on 2182 would give the native order (str, copy, subs, ldr r1, subs r3); not yet reproduced from source. (3) k = redraw: with for (k = redraw; ...) the .cse dump already folds insn 538 to k = 0 (REG_EQUAL 0), so CSE, not gcse, removes the native's ldr r1, [sp, #28]; COST(pseudo) 1 against const 0 at 0 makes that fold unconditional while redraw = 0 is on CSE's path, and the hash flush at 1000 insns is out of reach (the run starts at the main loop head). Trials: asm volatile on the 0x45f base 45 (ldr r3 now before lsls r5 as native, but the barrier adds a dependent to the code copy adds r3, r6, #0 and it passes lsls r1, r1, #4); the 0x45f asm also tying j ("+r"(j)) 140; the 0x45f id sum as (code & 0xff) + ... 180; (s32) cast on the 0xf335 width 45; (u32) detail_win->y in the dirty update 45. */
/* Fourth pass (45).The djinn scan sets y = 0 before pos and writes pos as the integer sum (cnt << 2) + tbl, as the native adds r5, r3, r2 after movs r6, #0 (47 to 45). Trials: pos = (cnt << 2) + tbl alone, (u32)tbl + (cnt << 2), cnt + tbl, tbl + cnt * 4 all 47; y = 0 before pos = &tbl[cnt] 47. Remaining (45): the k = redraw search-index load (ldr r1, [sp, #28]) with its pool and branch offsets (about 25 lines, and the b.n -740 is the pool pad parity it shifts); start schedule (movs r6, #0 for the zero inits; mov r9, r0 after the list window); sched2 places ldr r3, =0x666 after lsls at two sites (asm link cost 0) and lsls r5, r7, #4 at the 0x45f site; in the detail-window 0xf335 call, subs r2, #2 wins the tie on LUID where the native puts str and the r0 copy first (all priority 43, two dependents each); same at subs r3, #2 in the cursor block. The alchemy permuter refuses this draft (inline assembly is not permuted). Trials at 45: code = 0 right after cnt = 0, or prev_page = prev_row, 45; prev_page before cnt 53; code = 0 last or in one chained assignment 55; saved_row read first 55, row first 53; a menu pointer for the cell reads 600; cursor x as (x - 1) * 8 + 6, << 3, or + -2 45, y as (y + row * 2) 59. Not yet tried for the asm link cost: a device that feeds the held base to a real insn instead of the asm. */
/* Fourth pass, second step (47). The list draw loop indexes the table, while ((code = tbl[page + j]) != 0x80000000): loop.c then reduces &tbl[page + j] to a pointer initialized after the duplicated exit test (native: mov r8, r3 after beq, 0x80000000 in r1), 57 to 47. Trials of the head: code = tbl[page] then entry = &tbl[page] 57; for (entry = &tbl[page]; (code = *entry) ...; entry++) 61; tbl[page + j] reloaded at the loop end 47 (same code); for (; ...; code = *++entry) 57; j = 0 first 57. */
/* Fourth pass, first step (57). The id sum now computes n into a block-local first and holds the base in a block-local asm temporary after it: the base is born after the 255 mask dies and takes r3 at all three sites, as in the native (65 to 57). Trials: n and base at function scope 89 (global-alloc gives base r0); base block-local but held before n 383; asm on n with the constant inline 138 (reload, cross-jumped, 1189 insns); =r/"0" form 57 (same); a fixed register asm("r3") with no barrier 138 (combine folds it back inline). Remaining at those sites: sched2 puts ldr r3, =base after lsls, where the native puts it first: the asm insn costs the load 0 cycles (prio 37 against lsls 38). */
/* Third pass (65). Cause found: in the native, the message-id bases 0x666 (both arms) and 0x45f sit in local-alloc pseudos and take r3, first in the allocation order. The draft adds them inline, so they become rotation reloads, and every detail_win and zero reload after them shifts. Tagged asm-held temporaries (base) do this: 137 to 71. The 0x666 arm then resets detail_win before sub_sel as the native stores do (69), and entry is an integer sum page * 4 + tbl (65). Before that, the page wrap setting page = 0 before row = saved_row took 145 to 137 (from the permuter's best-1; its other rewrites are noise). Remaining: the held constant takes r2 where the native has r3, because its life still overlaps the 255 mask; a statement expression, =r/0 or +l constraints all stay at 69. Also left: the start-of-function schedule (movs r6, #0; mov r9, r0) and the k = redraw search-index load. Trial: initializing the search from redraw instead of 0 is unchanged (145). Further trials: a consumed is_used constraint differs 183, empty barriers stay 145, and reversing the detail reset statements differs 150. Remaining: the native loads the is_used search index from the redraw slot (one extra reload) and keeps the 0x8ef branch separate; the reload registers after it rotate differently. */
/* Parallel branch, fifth pass (2026-10-06, at 55, merged 2026-10-07); causes narrowed from the sched2 priority tables (haifa rank_for_schedule: priority, then class against the last scheduled insn, then dependant count, then luid). Start schedule: movs r6, #0 (pseudo 59, code) and str r0, [sp, #56] (tbl) tie at priority 134 with five dependants each, so luid decides; the native order needs code's zero set before the pool allocation call (sched2 floats it past the call, since r6 is call-saved). Trial: code = 0 before the pool allocation or before res 54, but reload then inherits r6 for cnt = 0 (str r6, [sp, #52]; the native keeps movs r1, #0) and the gWindowWork loads swap; code = 0 before detail_win = 0 or first 71. So the native zero in r6 is set early yet hidden from find_equiv_reg at cnt's reload. Late subs #2 at 0xf335: all five argument insns tie at priority 43, and the native order (str, adds r0, subs r2, ldr r1, subs r3) is exactly what luid order gives when width - 2 sits at the r2 argument move (insn 2181), as if combine had merged it there; the draft keeps it at its precompute position (2170), before the stack store. Combine cannot merge it into the move as the call is written: combinable_i3pat refuses an argument register when the insn before it sets another argument register (sets_function_arg_p; here r1 = 0xf335), so the native's late subtract needs the value to reach the r2 move another way. cursor_pos.x - 2 looks the same: the native subtract follows mov r2, sl. Search index: reload_cse_simplify_set only offers constants and registers, never memory, so the native ldr r1, [sp, #28] must be k = (reg redraw) at reload; slot 28 is redraw's alone (its only stores are 0 at function start, 0 before the cnt test and the 1s in input handling), and CSE (const 0 costs 0, a pseudo 1) and GCSE cprop (which propagates into any copy whose source has a constant available set) both fold it, so no plain source form found yet. */
/* Parallel branch, fourth pass (2026-10-06, 65 to 55, merged 2026-10-07). The page list loop reads its code in the while condition, code = tbl[page + j], with no entry pointer: loop strength reduction then makes the pointer a giv set after the copied exit test (native mov r8, r3 after beq), and the first 0x80000000 test takes r1 as the native does (65 to 55). Trials this pass: a function-scope base asm("r3") 202, 138 without the barriers; block-scoped asm("r3") holds at all three sites 202, and with the sum computed first 104; at the first 0x666 site alone 67 (r3 is right, but its literal load is scheduled after adds r0, r0, r5 where the native loads it before lsls r0, r0, #2), the same with asm("" : "=r"(hold) : "0"(0x666)) 67, and without the barrier combine folds it back (207); on the 55 draft the first-site r3 hold is 57. The permuter with PERM_LINESWAP over the twelve initializations and the three cell loads compiled 1,976 candidates and none beat the draft, so the start-of-function schedule (movs r6, #0 late; mov r9, r0 after the gWindowWork loads) does not come from statement order. Remaining at 55: the ldr r1, [sp, #28] search-index load (its 2 bytes shift every later pool load and branch: about 25 of the 55), the 0x666/0x45f literal loads in r2 where the native has r3 (12), the start schedule (8), and two late subs #imm scheduled one slot early (detail_win->width - 2 for 0xf335; cursor_pos.x = list_win->x * 8 - 2). Trial: y = row * 2 computed before cursor_pos.x 71; the r3 hold at the 0x45f site alone 110 (the loop around it reallocates); the permuter with PERM_RANDOMIZE on the 0xf335 call and the two cursor_pos lines, 473 candidates, none beat 55. The b.n -740 against +44 is only the pool alignment that the missing 2-byte load changes. */
/* MenuCell now includes its automatic-mode word at 0xe0; remove the unused duplicate tail word so selected remains at 0xe4 and total size at 0x104. */
/* Shared PartyStatusWindow from the verified STATUS.C corpus leaves differ 145. Further search trials: combined initialization, is_used as index, while-loop and memory clobber remain 145; do-while 166; index-first initialization 155; initialization before the clear call 148. Redraw memory/output constraints range 447-722. Clobbering r1 reaches 120 but emits mov-zero rather than the native reload, so it was removed. */
/* Stubborn (2026-10-06): CSE seeds the native search index from redraw's spilled zero (ldr r1, [sp, #28]); the draft keeps a fresh zero, and the reload rotation after the 0x8ef branch follows from it. */
/* Third pass (2026-10-06), measured against the 137 draft. Correction: the missing search-index load does not shift the reload rotation. Reload regs from is_used = 0 through the 0x8ef branch (r1, r2, r3, r1, r2, r3) match the native, and an asm tie that does reload redraw into r1 leaves the later swaps unchanged (141). The first divergence is the 0x666 constant reload in the cur & 0x3e0000 branch: native r3, draft r1. After it, detail_win and its zero come in rotation order (native r1/r2, draft r2/r3). Cause still unknown: some reload or local-alloc choice between the 0x8ef zero and insn 676. cse.c sets COST(pseudo) 1 against const 0 at 0, so CSE always folds k = redraw while redraw = 0 is known. The native copy survives only if CSE does not know redraw there, yet the native's first unrolled probe still folds k to 0 (movs r2, #228). No source form found that does both. Trials: asm("" : "=r"(k) : "0"(redraw)) 141 (CSE canonicalises redraw to mask, so it loads [sp, #40] and the first probe keeps cmp/lsls); k = redraw then +r k 149; asm use of redraw before the loop 160, before is_used 175; explicit first probe plus while (++k <= 7) 169 (strength-reduced); do-while from k 166/294; redraw = 0 in both arms of the sub_sel block 207 (cprop still makes k = 0, now movs r1, #0); detail_win = 0 before sub_sel = 0 in the 0x666 arm 142 (cross-jumps with the 0x8ef arm, 1189 insns); 0x666 written first or summed in u32 137; index-first sum 159; code = 0 before cnt = 0 or before the pool allocation 136 but 1199 insns (cnt's zero folds into r6, the native keeps movs r1, #0); prev_page before cnt 145; code = 0 last 147; the permuter's declaration swap, j = 0 hoist and PushValueSlot temporary each leave the count unchanged. */
/* Trials (2026-10-06, second pass): pos as cnt + tbl or as an integer sum 145 (same code); k < 8 145; function-scope j as the index 145 (it gets its own movs r7, #0); redraw = 0 moved before the sub_sel block with k = redraw 155 (CSE still folds it through the skipped block). The -da dumps keep k = (const_int 0) through cse, gcse, loop and combine, and reload_cse then drops it because r1 already holds is_used's zero; the native needs k = (reg redraw) alive at reload. The r1/r2/r3 swaps on the later 0x666 and 0x45f constant reloads follow from the reload register rotation that one missing reload shifts. */

#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "MEM.H"
#include "TYPES.H"
#include "MENU_LIST.H"
#include "BTLSYS.H"
#include "BTLPTY.H"
#include "IWRAM_CALL.H"

struct BattleMenuSelection {
    struct MenuCell menu;
    u32 selected[8];
};

LAYOUT_SIZE_GUARD(BattleMenuSelection_Size, struct BattleMenuSelection, 0x104);
LAYOUT_OFFSET_GUARD(BattleMenuSelection_Selected, struct BattleMenuSelection, selected, 0xe4);

#define MENU_STATE ((struct BattleMenuSelection *)gLinkCountdownWork)
extern u8 Data_03001e90[];
#define CURSOR_OWNER (*(struct PartyStatusWindow **)Data_03001e90)

extern volatile u32 gKeysHeld;
extern const u8 DjinnMenu_BlinkTile[32];
extern const u8 DjinnMenu_ElementXOffsets[4];
extern const u8 DjinnMenu_ElementYOffsets[4];

void RenderOutput_ClearList(void *work);
void UiWork_ClearValueNameTables(void);
void UiWork_PushValueSlot(s32 value, s32 width);
void UiWindow_MarkVisibleTileAttributes(void);
void UiWork_SetAltFlagAndClearTable(s32 flag);
struct UiWindow *DjinnMenu_ShowChangePreview(struct UiWindow *window, s32 owner,
    u32 code, s32 selection, s32 *count);
struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 side);

s32 Battle_SelectAbility(s32 owner, s32 unused)
{
    struct CursorSprite slot;
    u16 buf[64];
    struct UiRenderWork *ctx;
    struct UiWindow *detail_win;
    s32 prev_row;
    s32 res;
    u32 *tbl;
    s32 cnt;
    s32 prev_page;
    struct UiWindow *msg_win;
    s32 mask;
    s32 saved_row;
    s32 sub_sel;
    s32 redraw;
    s32 is_used;
    s32 sub_cnt;
    struct BattleMenuSelection *menu;
    struct BtlUnit *unit;
    struct DjinnRecoveryList *list;
    struct UiWindow *list_win;
    struct UiWindow *icon_win;
    struct PartyStatusWindow *cursor;
    u32 *pos;
    u32 cur;
    u32 blink;
    s32 page;
    s32 row;
    u32 code;
    s32 pages;
    s32 pal;
    struct MenuPoint cursor_pos;
    s32 trig;
    s32 rep;
    s32 opts;
    s32 x;
    s32 y;
    s32 i;
    s32 j;
    struct CursorSprite *cur_spr;

    ctx = (struct UiRenderWork *)gWindowWork[0];
    detail_win = 0;
    prev_row = -1;
    res = Resource_LoadIntoFreeSlot(0x80);
    tbl = (u32 *)Runtime_BumpAllocateAlternatePool(0xa8 * 2);
    cnt = 0;
    prev_page = -1;
    code = 0;
    mask = 0;
    sub_sel = 0;
    sub_cnt = 0;
    redraw = 0;
    is_used = 0;
    msg_win = UiWindow_Create(0, 4, 30, 4, 42);
    UiWork_SetAltFlagAndClearTable(1);
    list_win = UiWindow_Create(21, 9, 9, 11, 6);
    /* FAKEMATCH: sched2 ranks mov r9, r0 (priority 35) after the MenuCell read chain (39); the native copies list_win first, and this barrier also puts movs r6, #0 before the tbl store */
    asm("");
    page = ((struct MenuCell *)gWindowWork[42])->page;
    row = ((struct MenuCell *)gWindowWork[42])->row;
    saved_row = ((struct MenuCell *)gWindowWork[42])->preferred_row;
    /* FAKEMATCH: without it sched2 hoists the owner reload ldr r0, [sp, #76] above mov r11, mov r10 and the saved_row store; the native loads it last */
    asm("");
    unit = UnitGetFar(owner);

    for (x = 0; x <= 3; x++) {
        for (y = 0, pos = (u32 *)((cnt << 2) + (u32)tbl); y <= 19; y++) {
            if (unit->djinn_active[x] & (u32)(1 << y)) {
                *pos = (u32)((x << 8) | y);
                pos++;
                cnt++;
            } else if (unit->djinn_available[x] & (u32)(1 << y)) {
                u32 v;
                s32 k;
                list = &Trade_GetOfferStateFar((u32)owner > 7 ? 1 : 0)->list;
                for (k = 0; k < list->count; k++) {
                    if (list->entries[k].unit_id == owner && list->entries[k].element == x &&
                        list->entries[k].index == y) {
                        break;
                    }
                }
                v = (u32)((x << 8) | y | 0x10000);
                *pos = v;
                if (list->entries[k].turns > 0) {
                    v |= (u32)(list->entries[k].turns << 17);
                    *pos = v;
                }
                cnt++;
                pos++;
            }
        }
    }
    tbl[cnt] = 0x80000000;
    ctx->dirty = 1;

    cur_spr = &slot;
    for (;;) {
        if (page != prev_page || row != prev_row || redraw != 0) {
            cur = tbl[page + row];
            mask = 0;
            ctx->menu_busy = 1;
            Ui_SetRectHighlight(list_win->x + 1, list_win->y + prev_row * 2 + 1,
                          list_win->width - 2, 1, 15);
            if (sub_sel != 0) {
                UiWork_Finalize(msg_win, 1);
                msg_win = UiWindow_Create(0, 4, 30, 4, 42);
                Ui_FillVramBlockPattern();
            }
            redraw = 0;
            if (cnt != 0) {
                /* FAKEMATCH: k in r1 as native; a pseudo k is a biv loop.c strength-reduces in the loop below (adds r2, #4; 2 -> 37) */
                register s32 k asm("r1");
                UiWork_ClearValueNameTables();
                /* FAKEMATCH: one-pass block, a CSE1 block end: without it CSE1 deletes the second redraw = 0 as a no-op (2 -> 29) */
                do { } while (0);
                is_used = 0;
                /* FAKEMATCH: redundant; gcse cprop leaves k = redraw alone only when redraw is set earlier in its block, and cse2 then deletes this as a no-op (2 -> 29) */
                redraw = 0;
                /* FAKEMATCH: a volatile asm flushes cse2's table, so k = redraw stays the native ldr r1, [sp, #28] instead of folding to 0 (2 -> 29) */
                asm("" :);
                k = redraw;
                if ((u32)MENU_STATE->selected[k] == cur) {
                    is_used = 1;
                } else {
                    for (;;) {
                        k++;
                        if (k > 7) break;
                        if ((u32)MENU_STATE->selected[k] == cur) {
                            is_used = 1;
                            break;
                        }
                    }
                }
                if (is_used != 0) {
                    UiText_CopyMessageString(0x8ef, buf, 52);
                    if (detail_win != 0) {
                        UiWork_Finalize(detail_win, 1);
                        detail_win = 0;
                        sub_sel = 0;
                    }
                } else if (cur & 0x10000) {
                    if (cur & 0x3e0000) {
                        UiWork_PushValueSlot((s32)((cur & 0x3e0000) >> 17), 5);
                        {
                            s32 n = (s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff));
                            s32 base = 0x666;
                            /* FAKEMATCH: base is an input of this hold on n, so combine cannot fold it into the add (inline it is a rotation reload, r1/r2) and it takes r3 as native; its load feeds the add directly and gains a dependant, so sched2 puts it before lsls as native */
                            asm("" : "+r"(n) : "r"(base));
                            UiText_CopyMessageString(n + base, buf, 52);
                        }
                        if (detail_win != 0) {
                            UiWork_Finalize(detail_win, 1);
                            detail_win = 0;
                            sub_sel = 0;
                        }
                    } else {
                        detail_win = DjinnMenu_ShowChangePreview(detail_win, owner, cur, sub_sel, &sub_cnt);
                        UiText_CopyMessageString(0x899, buf, 52);
                        mask = 1 << ((cur & 0xf00) >> 8);
                    }
                } else {
                    detail_win = DjinnMenu_ShowChangePreview(detail_win, owner, cur, sub_sel, &sub_cnt);
                    {
                        s32 n = (s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff));
                        s32 base = 0x666;
                        /* FAKEMATCH: base is an input of this hold on n, so combine cannot fold it into the add (inline it is a rotation reload, r1/r2) and it takes r3 as native; its load feeds the add directly and gains a dependant, so sched2 puts it before lsls as native */
                        asm("" : "+r"(n) : "r"(base));
                        UiText_CopyMessageString(n + base, buf, 52);
                    }
                }
            } else {
                UiText_CopyMessageString(0x8ed, buf, 52);
            }
            ctx->menu_busy = 0;
            if (sub_sel == 0) {
                ctx->menu_busy = 1;
                UiWork_Finalize(msg_win, 1);
                msg_win = UiWindow_Create(0, 4, 30, 4, 42);
                ctx->menu_busy = (u8)sub_sel;
            }
            UiText_RenderWideStringAtOffset(buf, msg_win, 0, 4);
            prev_row = row;
            if (page != prev_page) {
                RenderOutput_RedrawSavedRect(list_win);
                j = 0;
                while ((code = tbl[page + j]) != 0x80000000) {
                    UiWindow_SetTilemapEntry(list_win, (s32)((code & 0xf00) >> 8) + 0x5001, 0, j * 2, 0);
                    if (code & 0x3e0000) {
                        UiWork_SetParamNibble(4);
                    } else if (code & 0x10000) {
                        UiWork_SetParamNibble(2);
                    }
                    {
                        s32 n = (s32)(((code & 0xf00) >> 8) * 20 + (code & 0xff));
                        s32 base = 0x45f;
                        /* FAKEMATCH: base is an input of this hold on n, so combine cannot fold it into the add (inline it is a rotation reload, r1/r2) and it takes r3 as native; its load feeds the add directly and gains a dependant, so sched2 puts it before lsls as native */
                        asm("" : "+r"(n) : "r"(base));
                        UiText_DrawCharacterAtOffset(n + base, (struct RenderInput *)list_win, 8, j * 16);
                    }
                    if (code & 0x3e0000) {
                        UiText_DrawNumberAtOffset((s32)((code & 0x3e0000) >> 17), 1, (struct RenderInput *)list_win, 48, j * 16);
                    }
                    UiWork_SetParamNibble(15);
                    j++;
                    if (j > 4) {
                        break;
                    }
                }
                prev_page = page;
            }
            if (cnt > 5) {
                for (j = 0; j < (pages = ((cnt + 4) / 5)); j++) {
                    s32 tile = j + 0xf301;
                    if (j == (page / 5)) {
                        tile = j + 0xf30b;
                    }
                    UiWindow_SetTilemapEntry(list_win, tile, list_win->width - pages + j - 2, -1, 0);
                }
            }
            Ui_SetRectHighlight(list_win->x + 1, list_win->y + row * 2 + 1,
                          list_win->width - 2, 1, 14);
            ctx->dirty = 1;
            ctx->menu_busy = 0;
        }

        if (cnt > 5) {
            for (j = 0; j < (cnt + 4) / 5; j++) {
                s32 tile = j + 0xf301;
                if ((gKeysHeld & 0x100) || (gFrameCount & 15) <= 11) {
                    if (j == (page / 5)) {
                        tile = j + 0xf30b;
                    }
                }
                UiWindow_SetTilemapEntry(list_win, tile,
                              list_win->width - ((cnt + 4) / 5) + j - 2, -1, 0);
            }
            if ((gKeysHeld & 0x100) == 0) {
                UiWindow_SetTilemapEntry(list_win, 0xf334, list_win->width - (cnt + 4) / 5 - 3, -1, 0);
                UiWindow_SetTilemapEntry(list_win, 0xf335, list_win->width - 2, -1, 0);
            } else {
                UiWindow_SetTilemapEntry(list_win, 0xf011, list_win->width - (cnt + 4) / 5 - 3, -1, 0);
                UiWindow_SetTilemapEntry(list_win, 0xf012, list_win->width - 2, -1, 0);
            }
            ctx->dirty |= (u8)(2 << ((u32)(list_win->y - 1) >> 2));
        }

        trig = gKeyState;
        rep = gKeysRepeat;
        opts = (s32)gKeysHeld;
        menu = MENU_STATE;
        if (menu->menu.auto_enabled != 0) {
            rep = 0;
            trig = 0;
            opts = 0;
            if (menu->menu.auto_delay == 0) {
                menu->menu.auto_delay = 60;
                rep = 1;
                trig = 1;
            } else {
                menu->menu.auto_delay = menu->menu.auto_delay - 1;
            }
        }
        menu = MENU_STATE;
        if (menu->menu.active == 0 || (trig & 2)) {
            AudioCommand_PlayFar(0x71);
            code = (u32)-1;
            break;
        }
        if (trig & 1) {
            u32 sel;
            u32 turns;
            if (cnt != 0) {
                sel = tbl[page + row];
                turns = sel & 0x3e0000;
                if (turns == 0) {
                    if (is_used == 0) {
                        menu->menu.page = page;
                        menu->menu.row = row;
                        menu->menu.preferred_row = saved_row;
                        code = sel;
                        break;
                    }
                } else if (is_used == 0) {
                    Ui_FillVramBlockPattern();
                    UiWork_ClearValueNameTables();
                    UiWork_PushValueSlot((s32)((turns & 0x3e0000) >> 17), 5);
                    UiText_CopyMessageString(0x898, buf, 52);
                    UiText_RenderWideStringAtOffset(buf, msg_win, 0, 4);
                }
                AudioCommand_PlayFar(0x72);
            } else {
                code = (u32)-1;
                break;
            }
        }

        if (cnt != 0) {
            if (rep & 0x80) {
                AudioCommand_PlayFar(0x6f);
                row++;
                if (row == 5 || page + row == cnt) {
                    row = 0;
                }
                saved_row = row;
            } else if (rep & 0x40) {
                AudioCommand_PlayFar(0x6f);
                row--;
                if (row < 0) {
                    if (page == ((cnt - 1) / 5) * 5) {
                        row = cnt - page - 1;
                    } else {
                        row = 4;
                    }
                }
                saved_row = row;
            } else if (opts & 0x100) {
                if (detail_win != 0) {
                    for (i = 0; i < sub_cnt; i++) {
                        s32 tile = i + 0xf301;
                        if ((gFrameCount & 15) <= 11 && i == sub_sel - 1) {
                            tile = sub_sel + 0xf30b;
                        }
                        UiWindow_SetTilemapEntry(detail_win, tile,
                                      detail_win->width - sub_cnt + i - 2, -1, 0);
                    }
                    UiWindow_SetTilemapEntry(detail_win, 0xf334, detail_win->width - sub_cnt - 3, -1,
                                  0);
                    UiWindow_SetTilemapEntry(detail_win, 0xf335, detail_win->width - 2, -1, 0);
                    /* FAKEMATCH: one-pass block, a block end: its loop notes make the dirty update a sched2 barrier, so the post-call writes of r1, r2 and r3 no longer count as dependants and the 0xf335 arguments take the native order (subs r2, #2 after the r0 copy; 6 -> 2) */
                    do { } while (0);
                    ctx->dirty |= (u8)(2 << (detail_win->y >> 2));
                }
                if (sub_sel == 0 && sub_cnt != 0) {
                    if (detail_win != 0) {
                        RenderOutput_ClearList(detail_win);
                    }
                    sub_sel = 1;
                    redraw = 1;
                } else {
                    if (sub_sel > sub_cnt) {
                        sub_sel = sub_cnt;
                    }
                    if (sub_sel != 0) {
                        if (rep & 16) {
                            AudioCommand_PlayFar(0x6f);
                            sub_sel++;
                            if (sub_sel > sub_cnt) {
                                sub_sel = 1;
                            }
                            redraw = 1;
                        } else if (rep & 32) {
                            AudioCommand_PlayFar(0x6f);
                            sub_sel--;
                            if (sub_sel <= 0) {
                                sub_sel = sub_cnt;
                            }
                            redraw = 1;
                        }
                    }
                }
            } else if (sub_sel != 0) {
                if (detail_win != 0) {
                    RenderOutput_ClearList(detail_win);
                }
                sub_sel = 0;
                redraw = 1;
            } else if (rep & 16) {
                AudioCommand_PlayFar(0x6f);
                Runtime_SetMainState19();
                if (page + 5 >= cnt) {
                    if (page != 0) {
                        page = 0;
                        row = saved_row;
                    }
                } else {
                    page = page + 5;
                    row = saved_row;
                    if (page == ((cnt - 1) / 5) * 5) {
                        row = cnt - page - 1;
                        if (row > saved_row) {
                            row = saved_row;
                        }
                    }
                }
            } else if (rep & 32) {
                AudioCommand_PlayFar(0x6f);
                Runtime_SetMainState19();
                if (page != 0) {
                    page -= 5;
                    row = saved_row;
                } else {
                    page = ((cnt - 1) / 5) * 5;
                    row = saved_row;
                    if (page != 0) {
                        row = cnt - page - 1;
                        if (row > saved_row) {
                            row = saved_row;
                        }
                    }
                }
            }
        }

        cursor_pos.x = list_win->x * 8 - 2;
        cursor_pos.y = (row * 2 + list_win->y) * 8 + 20;
        cur_spr = &slot;
        ((u32 *)cur_spr)[1] = MENU_SPRITE_SIZE_16;
        ((u32 *)cur_spr)[2] = 0;
        cur_spr->tile = Resource_GetBuffer(res, (s32)Resource_FixedBlockBTiles);
        cur_spr->x = cursor_pos.x + ((gFrameCount & 4) >> 1) - 6;
        cur_spr->y = cursor_pos.y - ((gFrameCount & 4) >> 2) - 8;
        if (cnt != 0) {
            Runtime_PushSlotEntry(cur_spr, 242);
        }
        cursor = CURSOR_OWNER;
        blink = gFrameCount & 4;
        icon_win = cursor->window;
        if (cursor->flags & 2) {
            for (i = 0; i <= 3; i++) {
                pal = 15 - (blink != 0);
                if (((1 << i) & mask) == 0) {
                    pal = 15;
                }
                Ui_SetRectHighlight(icon_win->x + DjinnMenu_ElementXOffsets[i] + 1,
                              icon_win->y + DjinnMenu_ElementYOffsets[i] + 1, 2, 2, pal);
            }
        }
        if (gFrameCount & 4) {
            Iwram_CopyWords((void *)0x06006500, DjinnMenu_BlinkTile, 32);
            Iwram_CopyWords((void *)0x06006520, DjinnMenu_BlinkTile, 32);
        } else {
            Iwram_FillWords((void *)0x06006500, 32, 0x44444444);
            Iwram_CopyWords((void *)0x06006520, DjinnMenu_BlinkTile, 32);
        }
        WaitFrames(1);
    }

    if (CURSOR_OWNER->flags & 2) {
        Ui_SetRectHighlight(CURSOR_OWNER->window->x + 1, CURSOR_OWNER->window->y + 1, 4, 4, 15);
    }
    Resource_ResetEntry(res);
    UiWork_Finalize(msg_win, 1);
    UiWork_Finalize(detail_win, 1);
    UiWork_Finalize(list_win, 1);
    UiWindow_MarkVisibleTileAttributes();
    UiWork_SetAltFlagAndClearTable(0);
    Runtime_BumpFree(tbl);
    WaitFrames(1);
    return (s32)code;
}
