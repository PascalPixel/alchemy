/* 08024934: SummonMenu_SelectSummon draft.
 * Native 946 instructions, draft 926, common 674, differ 524; frame 372
 * as native's.
 * Session 5: the first page-tile loop written if (index != page / 4)
 * tile = PAGE; else tile = SELECTED: 567 -> 524. GCSE then no longer
 * carries the loop test's (count + 3) / 4 into the body (it is
 * recomputed there, where native reuses it in r0), but every compiler
 * temporary's slot comes out at native's offset (summon 36, ordered 32,
 * sprites 28, cursor 24, row * 2 16, cursor_handle << 16 12), which
 * resolves most of (d) below. Tile is still r4 and index r7. The same
 * test as if (index == page / 4) ... else 526, as a conditional
 * expression 526; the call's x as index + width - quotient - 2 567. From
 * 524: element shared by the draw loop's scan and columns 819; an edge
 * = -1 block local for the arrows 524 unchanged. From 567: an edge = -1
 * block local for the first page loop's call 570 (its 0 then takes r3
 * and the -1 becomes subs r3, #1 by move2add, as in native's second
 * page loop and in BattleMenu_RunActionSelection's first, but native's
 * first loop keeps the 0 in r0 and builds -1 with movs; negs).
 * Session 5 diagnosis, the next step: native's main loop is a real loop
 * (loop notes), as BattleMenu_RunActionSelection's for (;;) is. With the
 * goto loop / done replaced by for (;;) and two breaks, and the first
 * page loop in its original form (tile = PAGE; if (index == page / 4)
 * tile = SELECTED), that page loop matches native instruction for
 * instruction except index (r7 against native's r4 with its caller-save
 * at sp+4): tile r1, quotient r0, window reloaded into r3, the 0 in r0
 * and -1 by movs; negs. The reason: loop.c hoists the main loop's
 * argument 0 (insn 1184, pseudo 331, savings 5) and global-alloc leaves
 * it in no register, so reload materializes it where needed (the twin's
 * pseudo 205 is the same) and r1 is free for tile. The row * 2 giv of
 * the key handling is then a loop giv too, as native's slot 16 shows.
 * Total 595 (do { } while (1) and while (1) 595 too), because
 * find_and_verify_loops moves the `result = ordered[page + row]; break;`
 * block out of the loop (b.n -1238) to the first depth-0 barrier before
 * it: the jump over the skip-path block that stores &ordered after the
 * first scan (mov rX, sp; adds rX, #240; str), which GCSE creates before
 * loop.c runs. Native has the same skip-path block but keeps the A block
 * inline, so in native that barrier did not exist yet when loop.c looked
 * (perhaps loop.c made it from the first scan), or the A block did not
 * qualify (a label at its start, or its guard not a plain conditional
 * jump around it). Solve that and keep for (;;). With for (;;) and the
 * if/else page loop 606. A block local n = count + 3 for the page loop
 * bound makes that loop non-phony too (GCSE's count + 3 insertion after
 * LOOP_BEG is what makes it phony), but gives n + 3 for the negative
 * adjust where native has count + 6: first loop 577, second loop 585.
 * Session 4: the highlight loop has its own tile, the two page-tile loops
 * share the function's tile as ItemList_SelectEntry (BATTLE_LIST.C, the
 * matched twin) does: 575 -> 567 (a tile per loop also 567).
 * Diagnosed with flow.c's weights (refs count loop_depth + 1 over the
 * CFG's natural loops, goto loops included) and global.c's find_reg:
 * index (126 refs, 510 insns, 14 calls, 6 * 126 / 510 = 1.482) is
 * allocated before the column element (1.407), takes r7 and leaves
 * element the caller-saved r4. Native's scan counter and column element
 * share r7, which one element for both gives (37 refs, 88 insns, 2.10,
 * takes r7), but then index falls to r8 (835): the first page-tile
 * loop's tile (9 refs, 16 insns, 1.69) already holds r4, and index's
 * caller-save attempt needs r4 free. With that loop's page test removed
 * (diagnostic only) index takes r4 with saves at sp+4 as native's, 697.
 * Native's tile is r1 and its quotient (count + 3) / 4 r0; the draft's
 * tile conflicts with r1 because local-alloc gives the stack argument's
 * 0 (a REG_EQUIV pseudo) r1 while tile is live, so the quotient takes r1
 * and tile r4. Native's stack zeros carry no register: in its second
 * page loop "movs r3, #0; str r3, [sp]; ... subs r3, #1" is reload's
 * constant 0 and move2add turning the -1 reload into a subtract, and in
 * the first loop its 0 is reloaded into r0 after the quotient dies. Both
 * page loops are phony to loop.c (entry jump target outside the loop),
 * so nothing there is hoisted. Item (e) below is the same move2add:
 * native reaches menu_busy's offset with adds r1, #3 from dirty's reload
 * in r1. In the draft expand writes dirty = 1 as a read-modify-write
 * (load, and 0, ior 1: insns 1244-1258 of the rtl dump, although the
 * fields are plain u8), CSE reuses that and-mask 0 as menu_busy's value,
 * local-alloc gives it r1 from before the first store, and the second
 * offset's reload must take r2. Native's 0 is fresh after the first
 * store. The matched twins compile the same two lines with a fresh 0
 * (BattleMenu_RunActionSelection: K r1, K + 3 r2, 0 r0; ItemList_
 * SelectEntry: 0 r1), so the shared 0 comes from this draft's context,
 * and the r1 for K + 3 then needs reload's spill-register rotation to
 * land on r1. With shared element: tiles
 * shared by the page loops 835, page tile if/else or a conditional
 * argument 821, tile = index + (test ? selected : page) 821. From 567:
 * an edge = -1 variable for the arrows (as the twin has) 567, unchanged;
 * a function-wide zero for the first page loop's argument 864.
 * Session 3, third step: the highlight loop's test written
 * (1 << element & mask) == 0 keeps native's movs 1; lsls; ands; cmp, where
 * (mask & (1 << element)) == 0 is folded to (mask >> element) & 1 (asrs):
 * 598 -> 575; a bit temporary or a u32 cast give the same 575, and
 * !(mask & ...) 598, a conditional expression or if/else 602. Remaining
 * there: affordable r5 / element r6 / tile r4 against native's r6 / r5 /
 * r2, and gFrameCount's address reused from the cursor where native
 * reloads it from the pool. Flag stores through a u8 pointer 612,
 * (&render->dirty)[3] 598; fixed-register element asm("r7") 597 but
 * miscompiles (index shares r7), not kept; first loop's attributes all
 * through sprite 647; sprite[index] tile loop 651; index = 0 before the
 * counts columns 844; required[element] columns 852; element shared by
 * the columns and the highlight loop 856; y * 8 for the name row 598.
 * Since the y step drawn_page, row and page sit in native's r9, r10, r11
 * ((c) below is resolved). From 575: mask = 0 after the cell reads 581,
 * before the counts window 579, after the main window 585 (native stores
 * counts before mask, the draft mask first); nav->row before nav->page
 * 581 (native stores page first, the draft row first).
 * Session 3, second step: y = index * 2 set after the scan, before the
 * glyph load, 663 -> 598, although native computes index * 2 after the
 * load and shares it with the tile store's index * 3; y beside the tile
 * store (before or after it) 663 unchanged; y before the scan's
 * affordable 640; y at the top of the pass 644; col = 0 beside the new y
 * 619.
 * Session 3: the draw loop's columns counted (col = 0 at the top of each
 * pass, col * 2 + 13 and col * 2 + 14, col++) give native's giv start
 * (lsls r3, rX, #1; adds r5, r3, #0; adds r5, #13) and its hoisted 0 for
 * the stack argument: 690 -> 663. col = 0 anywhere after the scan (before
 * the glyph, the tile store, the name or the columns) 687. Remaining
 * cascade there: element takes r4 with caller-saves and index r7, where
 * native has the reverse; their priorities (lreg) are close, element
 * 4 * 19 / 54 against index 6 * 126 / 510, so index is allocated first.
 * From 663: the draw loop on its own counter i 907; element shared by
 * the scan and the columns 870; the page-tile loops on element 909; the
 * display loop on element 663 unchanged; the init and reset loops on
 * element 669; indexed summon->djinn_required[element] columns 984;
 * indexed handle and tile loop 819.
 * Remaining, in order: (a) the draw loop's affordability scan has its
 * first test duplicated by jump.c before loop.c runs, so CSE folds element
 * 0 into summon + 4 (insn 2490), GCSE shares it with the column loop's
 * required (copy 619) and summon + 4 lives in slot 36; native keeps summon
 * there and recomputes summon + 4 for the columns, its first test using
 * loop.c's reduced pointers, which suggests the exit test was duplicated
 * only after loop.c (jump.c copies at most 20 insns). (b) index takes a
 * call-saved register (r7) where native gives it r4 with caller-saves at
 * sp+4, so the loops' r4-r7 differ throughout. (c) row r9 and drawn_page
 * r10 are swapped against native. (d) the compiler temporaries' slots come
 * in a different order (see below). (e) dirty = 1 then menu_busy = 0 load
 * two pool constants where native adds 3 to the first.
 * The draw loop's scan with its own counter n (not the column loop's
 * element) 696 -> 690; as u8 770, s16 764, u16 949 (frame 368); while
 * (element <= 3 && ...) element++ 696. The final reset loop as an indexed
 * for over handles compiles the same (696), so the handle pointer is gone.
 * dirty and menu_busy stores swapped 696.
 * No last variable: count - 1 written inline in the scan's start and the
 * page arithmetic, so it becomes a compiler temporary as native's slot 20
 * suggested, 735 -> 696. Keeping last only for the keys 741; last set after
 * used = 0 735. The remaining slot order of compiler temporaries differs:
 * draft summon + 4 36, row * 2 32, count - 1 28, cursor 24,
 * cursor_handle << 16 20, &glyph 16, ordered 12, sprites 8; native summon
 * 36, ordered 32, sprites 28, cursor 24, count - 1 20, row * 2 16,
 * cursor_handle << 16 12, &glyph 8. Moving the row, drawn_page or page
 * declaration through ten positions changed nothing (696, one 698).
 * Both affordability scans as plain indexed for loops with a break
 * (for (n = 0; n <= 3; n++) if (s->djinn_required[n] > standby[n]) break;)
 * compile to native's pointer loops: the first scan now matches exactly
 * (no reload through ip), 759 -> 735; the draw loop's scan is unchanged
 * from the if/do-while indexed form. Indexed column loop 1036 (frame 368).
 * The draw loop's affordability scan indexed (summon->djinn_required[element]
 * <= standby[element]) lets loop.c make the pointers, so GCSE no longer
 * shares summon + 4 with the column loop: 766 -> 759. Native still keeps
 * summon itself in slot 36 and recomputes summon + 4 for the columns, where
 * the draft keeps summon + 4 in slot 32. Indexed scan with [0] for the first
 * test 884, indexed through a required pointer 881 (both frame 368).
 * The allocation order (greg dump) puts drawn_page last among the
 * function-wide scalars, so row takes r9 and drawn_page r10, swapped against
 * native; declaration order of drawn_row, drawn_page, row, page does not
 * change it (five orders, all 766).
 * Session 2: the draw loop written as a rotated goto loop (no loop notes)
 * gives native's 372-byte frame: loop.c no longer hoists the sprites copy
 * (insn 935, life 13, 17 * 13 >= 123 insns) nor reduces the tile-store giv
 * index * 12 + 8, so the base is reloaded from its slot twice and the offset
 * recomputed each pass, as native does. With that frame, scalars declared
 * in native slot order (count, window, render, drawn_row, cursor_handle,
 * description, counts, mask, preferred, cursor_position, summon) put every
 * user spill slot at native's offset: 796 -> 766. Trials from 796: y set
 * beside the tile store 796 unchanged; unrotated goto draw loop 803, frame
 * 368; rotated goto loop alone 880, frame 372.
 * Session 1 remaining: spill/register allocation,
 * sprite setup/load order, and consequent branches and literal-pool placement.
 * The scan of available and the compaction into ordered now match apart from
 * slot offsets and one reload through ip for the first *have load.
 * Diagnosed: stmt.c's expand_end_loop rolls every exit test within 30 insns
 * of a while loop's top to its bottom, so `while (*need <= *have) { n++;
 * if (n > 3) break; ... }` also moved the n > 3 exit; native keeps only the
 * comparison at the bottom, as `if (c) do { ... } while (c)` gives. Shared
 * function-wide required/supply/element/id/src pseudos crossed calls
 * elsewhere, so the scan's need could not take r0 and id lost a call-saved
 * register; the scan wants its own block locals and an indexed countdown
 * (biv elimination gives native's cmp src, available). In the draw loop,
 * loop.c reduces the tile-store giv index * 12 + 8 (lifetime 10, benefit 11,
 * 124 insns) into slot 8 with a sprites copy at 40; native recomputes it.
 * From 901: if/do-while affordability checks 894 (scan) and 887 (both);
 * block-local need/have/n in the scan 881, plus its own id 862; indexed
 * compaction loop 867, with its own id 848; indexed scan countdown from last
 * 830. Block-local check in the draw loop 839; for-with-break draw loop and y
 * beside the tile store both 830 unchanged. drawn_row = row before mask = 0
 * 825. Indexed display loop (visible[index], &sprites[index]) 796: native's
 * index counts up there, not reversed; index still takes r7 where native
 * leaves it r4 with caller-saves at 4. From 825: used = 0 before the list call
 * 830; counted columns (n * 2 + 13) 827; all-indexed sprite init 825, all
 * through sprite 870, pointer traversal 884; indexed handle and tile loop
 * 878 (native's shape there, offset r5 from 8 and stmia handles, but its
 * base pseudo takes a call-saved register and drawn_page loses r9).
 * Native's draw block uses r9 for a hoisted 0 and r8 for y; drawn_page is
 * dead there. From 796: do { } while (1) main loop 796 unchanged; scalars
 * declared in native slot order (count, window, render, drawn_row,
 * cursor_handle, description, counts, mask, preferred, cursor_position,
 * summon) 838, though every spill slot then sits exactly 8 above native's:
 * reload numbers slots by pseudo, so that order is right once the two
 * draw-loop giv slots go. In a for (;;) loop, loop.c's find_and_verify_loops
 * moves the `result = ordered[page + row]; break;` block out of the loop to
 * the barrier after the scan (b.n -1254 and back); native keeps it inline
 * with result in r6, which a goto main loop (no loop notes) gives: kept at
 * 796 (draft 924 instructions), the -1 tilemap row then takes r6 where
 * native has r5 and native stores a fresh 0 per call. The tile-store giv would stay
 * unreduced at lifetime 1 (119 vs 124, like giv 938), so native's ldrh and
 * strh addresses were probably separate givs at loop time.
 * Native slots, top down: standby 84, count 80, window 76, render 72,
 * drawn_row 68 (drawn_page r9, row r10, page r11), cursor_handle 64,
 * description 60, counts 56, mask 52, preferred 48, cursor_position 40-47,
 * summon 36, ordered 32, sprites 28, cursor 24, count - 1 at 20, row * 2 16,
 * cursor_handle << 16 at 12, &glyph 8, r4 caller-save 4. The draft's extra
 * slots are draw-loop givs (sprite offset index * 12 + 8 and a sprites copy);
 * native recomputes them from index each pass and keeps index in r4.
 *
 * One row local (y = index * 2) for the djinn columns removed the index * 2
 * giv: 914 -> 901. Declaring scalars in native slot order (count, window,
 * render, drawn_row, cursor_handle, description, counts, mask, preferred,
 * cursor_position) gave 956; count and window first alone gave 946. Counting
 * columns (col * 2 + 13) compiled the same as col += 2, with or without y.
 * From 901: y set at the top of the draw loop 1002; y set beside the sprite
 * store 901; indexed summon->djinn_required[element] columns 1116; a sprite
 * pointer for the tile store 924; the tile store after the nibble call 900
 * (against native order, not kept); a counted first pass over available with
 * count - 1 inline instead of last 913, frame 384.
 *
 * Natural trials: early cursor pointer removal 979 -> 969; indexed sprite
 * initialization and handles 977; shared index 963; pointer sprite traversal
 * with indexed handles 914. Moving scalar declarations did not change output.
 * Flat CursorSprite layout worsened the indexed trial to 998; sharing element
 * and column locals worsened it to 1063/1072. A local cursor pointer gave 916,
 * one outside the display loop 920; a short handle left 914 unchanged.
 * A one-pass attribute-store block moved the y shift into native order, 912,
 * but was removed while the wider source shape remains unsettled.
 */
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "BATTLE_SUMMON.H"
#include "MENU_LIST.H"

enum {
    SUMMON_ELEMENT_TILE = 0x5001,
    SUMMON_PAGE_TILE = 0xf301,
    SUMMON_SELECTED_PAGE_TILE = 0xf30b,
    SUMMON_LEFT_ARROW_TILE = 0xf334,
    SUMMON_RIGHT_ARROW_TILE = 0xf335
};

extern u8 MsgAbilityName;
extern u8 MsgAbilityDescription;

void UiWindow_PutGlyph(struct UiWindow *, u32, u32, u32, s32);
void Ability_LoadGlyph(s32, s32, s32 *, s32 *, s32);
s32 Trade_ListFlaggedEntriesFar(u8 *ids);

s32 SummonMenu_SelectSummon(s32 unused0, s32 unused1, const u8 *standby)
{
    struct MenuSprite cursor;
    struct MenuSprite sprites[4];
    u8 available[33];
    u8 ordered[33];
    u16 text[64];
    s32 handles[4];
    u8 visible[4];
    s32 glyph;
    s32 count;
    struct UiWindow *window;
    struct UiRenderWork *render;
    s32 drawn_row, drawn_page, row, page;
    s32 cursor_handle;
    struct UiWindow *description, *counts;
    s32 mask, preferred;
    struct MenuPoint cursor_position;
    const struct SummonDefinition *summon;
    struct MenuCell *nav;
    struct MenuSprite *sprite;
    const u8 *required, *supply;
    s32 used, element;
    s32 id, tile, affordable;
    s32 pressed, repeated, result;
    s32 index;

    render = (struct UiRenderWork *)gWindowWork[0];
    drawn_row = -1;
    drawn_page = -1;
    cursor_handle = (s16)Resource_LoadIntoFreeSlot(128);
    description = UiWindow_Create(0, 4, 30, 4, 42);
    counts = UiWindow_Create(20, 8, 10, 3, 6);
    mask = 0;
    page = ((struct MenuCell *)gWindowWork[42])->page;
    row = ((struct MenuCell *)gWindowWork[42])->row;
    preferred = ((struct MenuCell *)gWindowWork[42])->preferred_row;
    window = UiWindow_Create(13, 11, 17, 9, 6);
    for (index = 0; index < 4; index++) {
        s32 y = index * 2;

        sprite = &sprites[index];
        sprites[index].oam.word.attr01 = MENU_SPRITE_SIZE_16;
        sprites[index].oam.word.attr23 = 0;
        sprite->oam.f.x = window->x * 8 + 8;
        sprite->oam.f.y = (y + window->y) * 8 + 4;
    }
    sprite = sprites;
    for (index = 0; index < 4; index++) {
        s32 slot = Resource_LoadIntoFreeSlot(128);
        handles[index] = slot;
        sprite->oam.f.tile = Resource_GetBuffer(slot, -1);
        sprite++;
    }
    count = Trade_ListFlaggedEntriesFar(available);
    used = 0;
    {
        s32 i;

        for (i = count - 1; i >= 0; i--) {
            const struct SummonDefinition *s;
            s32 n, k;

            k = available[i];
            s = SummonDefinition_Get(k);
            for (n = 0; n <= 3; n++) {
                if (s->djinn_required[n] > standby[n])
                    break;
            }
            if (n == 4) {
                ordered[used++] = k;
                available[i] = 32;
            }
        }
    }
    {
        s32 i, k;

        for (i = 0; i < count; i++) {
            k = available[i];
            if (k != 32)
                ordered[used++] = k;
        }
    }
    ordered[used] = 32;

loop:
    {
        if (page != drawn_page || row != drawn_row) {
            render->menu_busy = 1;
            Ui_SetRectHighlight(window->x + 1, window->y + drawn_row * 2 + 1,
                window->width - 2, 1, 15);
            Ui_FillVramBlockPattern();
            summon = SummonDefinition_Get(ordered[page + row]);
            UiText_CopyMessageString(summon->name_message_id + (s32)&MsgAbilityDescription, text, 52);
            UiText_RenderWideStringAtOffset(text, (struct UiWindow *)description, 0, 4);
            drawn_row = row;
            mask = 0;
            {
                s32 element;

                required = summon->djinn_required;
                for (element = 0; element <= 3; element++) {
                    if (*required++ != 0)
                        mask |= 1 << element;
                }
            }
            if (page != drawn_page) {
                RenderOutput_RedrawSavedRect(window);
                {
                    s32 element, col;

                    col = 1;
                    for (element = 0; element <= 3; element++) {
                        UiWindow_SetTilemapEntry(counts, element + SUMMON_ELEMENT_TILE, element * 2, 0, 0);
                        UiWindow_PutGlyph(counts, standby[element] + 48, col, 0, 0);
                        col += 2;
                    }
                }
                {
                    s32 element, col, y;
                    s32 n;

                    index = 0;
                    if ((id = ordered[page + index]) != 32) {
                    next:
                        col = 0;
                        summon = SummonDefinition_Get(id);
                        for (n = 0; n <= 3; n++) {
                            if (summon->djinn_required[n] > standby[n])
                                break;
                        }
                        affordable = n == 4;
                        y = index * 2;
                        Ability_LoadGlyph(summon->name_message_id & 0x3fff, 0, &handles[index], &glyph, 1);
                        sprites[index].oam.f.tile = glyph;
                        if (!affordable)
                            UiWork_SetParamNibble(2);
                        UiText_DrawCharacterAtOffset(SummonDefinition_Get(id)->name_message_id + (s32)&MsgAbilityName,
                            (struct RenderInput *)window, 16, index * 16);
                        required = summon->djinn_required;
                        for (element = 0; element <= 3; element++) {
                            if (*required != 0) {
                                UiWindow_SetTilemapEntry(window, element + SUMMON_ELEMENT_TILE, col * 2 + 13, y, 0);
                                UiWindow_PutGlyph(window, *required + 48, col * 2 + 14, y, 0);
                                col++;
                            }
                            required++;
                        }
                        UiWork_SetParamNibble(15);
                        visible[index++] = 1;
                        if (index <= 3 && (id = ordered[page + index]) != 32)
                            goto next;
                    }
                    while (index <= 3)
                        visible[index++] = 0;
                }
                drawn_page = page;
            }
            if (count > 4) {
                for (index = 0; index < (count + 3) / 4; index++) {
                    if (index != page / 4)
                        tile = index + SUMMON_PAGE_TILE;
                    else
                        tile = index + SUMMON_SELECTED_PAGE_TILE;
                    UiWindow_SetTilemapEntry(window, tile,
                        window->width - (count + 3) / 4 + index - 2, -1, 0);
                }
            }
            Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1, window->width - 2, 1, 14);
            render->dirty = 1;
            render->menu_busy = 0;
        }
        for (index = 0; index <= 3; index++) {
            if (visible[index] != 0)
                Runtime_PushSlotEntry((s32 *)&sprites[index], 240);
        }
        cursor_position.x = window->x * 8 - 2;
        cursor_position.y = (row * 2 + window->y) * 8 + 20;
        cursor.oam.word.attr01 = 0x40000000;
        cursor.oam.word.attr23 = 0;
        cursor.oam.f.tile = Resource_GetBuffer((u16)cursor_handle, (s32)Resource_FixedBlockBTiles);
        cursor.oam.f.x = cursor_position.x + ((gFrameCount & 4) >> 1) - 4;
        cursor.oam.f.y = cursor_position.y - ((gFrameCount & 4) >> 2) + 248;
        Runtime_PushSlotEntry((s32 *)&cursor, 242);
        affordable = gFrameCount & 8;
        {
            s32 element, tile;

            for (element = 0; element <= 3; element++) {
                tile = 15 - (affordable != 0);
                if ((1 << element & mask) == 0)
                    tile = 15;
                Ui_SetRectHighlight(counts->x + element * 2 + 1, counts->y + 1, 2, 1, tile);
            }
        }
        if (count > 4) {
            for (index = 0; index < (count + 3) / 4; index++) {
                tile = index + SUMMON_PAGE_TILE;
                if ((gFrameCount & 15) <= 11 && index == page / 4)
                    tile = index + SUMMON_SELECTED_PAGE_TILE;
                UiWindow_SetTilemapEntry(window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
            }
            UiWindow_SetTilemapEntry(window, SUMMON_LEFT_ARROW_TILE, window->width - (count + 3) / 4 - 3, -1, 0);
            UiWindow_SetTilemapEntry(window, SUMMON_RIGHT_ARROW_TILE, window->width - 2, -1, 0);
            render->dirty |= 2 << ((u32)(window->y - 1) >> 2);
        }
        nav = gLinkCountdownWork;
        nav->page = page;
        nav->row = row;
        nav->preferred_row = preferred;
        pressed = gKeyState;
        repeated = gKeysRepeat;
        if (nav->auto_enabled != 0) {
            repeated = 0;
            pressed = 0;
            if (nav->auto_delay == 0) {
                nav->auto_delay = 120;
                repeated = 1;
                pressed = 1;
            } else {
                nav->auto_delay--;
            }
        }
        if (pressed & 1) {
            result = ordered[page + row];
            goto done;
        }
        if (gLinkCountdownWork->active == 0 || (pressed & 2)) {
            AudioCommand_PlayFar(113);
            result = -1;
            goto done;
        }
        if (repeated & 128) {
            AudioCommand_PlayFar(111);
            row++;
            if (row == 4 || page + row == count)
                row = 0;
            preferred = row;
        } else if (repeated & 64) {
            AudioCommand_PlayFar(111);
            row--;
            if (row < 0) {
                if (page == ((count - 1) / 4) * 4)
                    row = count - page - 1;
                else
                    row = 3;
            }
            preferred = row;
        } else if (repeated & 16) {
            AudioCommand_PlayFar(111);
            Runtime_SetMainState19();
            if (page + 4 >= count) {
                if (page != 0) {
                    row = preferred;
                    page = 0;
                }
            } else {
                page += 4;
                row = preferred;
                if (page == ((count - 1) / 4) * 4) {
                    row = count - page - 1;
                    if (row > preferred)
                        row = preferred;
                }
            }
        } else if (repeated & 32) {
            AudioCommand_PlayFar(111);
            Runtime_SetMainState19();
            if (page != 0) {
                row = preferred;
                page -= 4;
            } else {
                page = ((count - 1) / 4) * 4;
                row = preferred;
                if (page != 0) {
                    row = count - page - 1;
                    if (row > preferred)
                        row = preferred;
                }
            }
        }
        WaitFrames(1);
    }
    goto loop;
done:
    WaitFrames(1);
    for (index = 0; index < 4; index++)
        Resource_ResetEntry(handles[index]);
    Resource_ResetEntry((u16)cursor_handle);
    UiWork_Finalize(counts, 1);
    UiWork_Finalize(description, 1);
    UiWork_Finalize(window, 1);
    WaitFrames(1);
    return result;
}
