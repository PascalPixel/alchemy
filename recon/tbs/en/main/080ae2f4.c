/* 2026-10-05 sun deep finisher 13: rewritten plainly from the listing, 426
 * to 229 (alchemy diff), no S2 devices.
 * - A plain transcription (struct fields, pointer pairs page = pages and
 *   sel = cursor, natural loops) scored 409.
 * - Declarations ordered as the frame shows them: the address-taken
 *   cursor[2], pages[2], gained, lost first (sp+76 down to sp+60), then
 *   the spilled phase, pending, frame, left (&menu->left_window), right,
 *   render, keys, repeat, preview (&menu->preview_owner), sel
 *   (sp+56 down to sp+20): 415, but the frame and slots line up later.
 * - A second zero-valued index variable, side, for the page tab code
 *   (page[side], sel[side]) gives the native "movs rX, #0; ldr [rX, page]"
 *   loads and the "movs r7, #0; add r7, fp" bound pointer: 339.
 * - The key tests each play cue 113 and leave with -2 or -1 inside the
 *   loop (cross-jumped as in the native): 324.
 * - sel[none] in the up and down branches: loop.c hoists &sel[none] into
 *   the native's second copy of the cursor pointer (sl), and sel spills to
 *   sp+20 with "mov r3, sp; adds r3, #76": the frame is 84 as native: 231.
 * - Chained pair stores "sel[1] = sel[0] = 0" and "page[1] = page[0] = 1"
 *   store element 0 first as the native does: 229.
 * Remaining (229): the three long-lived high registers are rotated (native
 * none r8, sel copy sl, page fp; here sel copy r8, page sl, none fp). The
 * global allocator orders them by refs/live length; dropping the second
 * page store after OwnerAction_DiffSlots (wrong semantics, measured only)
 * puts page in fp and scores 155, so the native's pages pair has fewer
 * weighted references or none more. The native indexes page[] with r8
 * unscaled ("mov r2, r8; ldr r1, [r2, fp]"), so none may be a byte offset
 * rather than an element index. The native keeps a dead
 * "frame % 60 - 5" (__modsi3 then subs r0, #5) before
 * FourObjectMotion_SetSlotPosition, not reproduced; the two -1 arguments
 * after the tab loop share r5 in the native. Declaration-order search over
 * the register locals (150 permutations): no change. 0xc8 is still loaded
 * from the pool as a symbol (Data_000000c8), and 0xbaa is the message
 * "Current status" (EN 2986, unnamed in the catalogue), here
 * MsgCurrentStatus, the second title message being MsgCurrentStatus + 2.
 */
/* NONMATCHING: shared callee return types audited on 2026-09-26.
 * 1020 of 1056 bytes, 515 differing halfwords, 314 aligned edits.
 * Canonical declarations are retained; the remaining source model is not exact.
 * 2026-09-29 slice 4: once alchemy permute kept function-pointer casts
 * abstract it could parse this draft; Unnamed_080ae2f4 scores 9,488 (85
 * register-only, 18 stack-only, 35 operand, 53 reordered, 15 inserted, 35
 * deleted), too far for a 10-minute search, so none was run.
 */
/* Draft, main:080ae2f4, complete extent 0x080ae2f4..0x080ae714 (1056 B).
 * H1: recover registered call contracts, typed menu/render fields and the two
 * stack pairs. Predict the original 84-byte frame and persistent menu pointer
 * before allocation tuning. Admission requires every instruction and both
 * literal pools to match; adoption also requires compare-all/test/coverage/verify.
 * Baseline: 996 B, 508 differing halfwords; aligned distance 374, 357 wrong
 * instructions, 57 other differences. Frame 76 rather than 84 bytes.
 * Lift defects: uninitialized cursor address, lost page-zero fallbacks,
 * duplicated divide, unconditional selected-page highlight, double decrement,
 * lost negative exit result. H1 restores these directly from the full listing.
 * Budget: three independent structural hypotheses / 30 minutes; hard stop
 * after 45 minutes without adoption. No register permutations.
 * Reference has a dead __modsi3(frame, 60)-5 before slot positioning. Its
 * survival is an unresolved compiler-lowering fact, not a semantic dependency.
 * H1 result: 976 B / 1056, 520 differing halfwords, aligned distance 319;
 * 370 wrong instructions, 117 runs (5 pool/immediate, 31 copy/rematerialise,
 * 81 other, no register-only runs). Frame 72; menu pointer r7 rather than r9.
 * Both pairs use fixed stack accesses; early row constant pool is absent.
 * The dead subtract was removed. H1 fails the structural admission predicate.
 * H2: initialize and access each pair through typed inline pointer helpers,
 * retaining a common zero index; keep the row constant in a u16 local. Predict
 * address-based pair accesses, 84-byte frame and the initial two-word pool.
 * This tests RTL inlining/addressability, not an allocation permutation.
 * H2 result: 1020 B / 1056, 515 differing halfwords, aligned distance 314;
 * 334 wrong instructions, 111 runs (5 pool/immediate, 45 copy/rematerialise,
 * 61 other, no register-only runs). Frame 76; persistent pair addresses are
 * recovered (cursor r8, pages sl), but menu stays r7 and render occupies fp;
 * keys/repeat spill at sp+48/+44 instead of reference +32/+28. Pool is at tail.
 * -dL: row symbol starts SI then narrows to HI. Machine minipool dump reports an
 * HImode symbol fixup with range 1020, not short reach: no width/reach claim.
 * H2 does not satisfy the reference frame/pool admission constraint.
 * H3: the contiguous frame/pending/phase/counts/pages/cursor stack area is a
 * typed screen-control aggregate. Predict original stack ordering and reduced
 * live-pointer pressure without register tuning. Final structural hypothesis.
 * H3 result: 1056 B / 1056, 514 differing halfwords, aligned distance 380;
 * 309 wrong instructions, 91 runs (10 pool/immediate, 17 copy/rematerialise,
 * 64 other, no register-only runs). Frame 84 and menu r9 now agree; count and
 * pair slots are 60/64/68/76 as in the reference. Control base stays in r8,
 * however, replacing direct frame loads/stores. Result spills at sp+44;
 * reference retains it in r7. Pool words are at +0x324 (21 words) and +0x414
 * (3), not reference +0x40 (2) and +0x3d4 (19). Dead subtract still absent.
 * Call argument scheduling and loop/exit topology still differ broadly.
 * STOP: three independent structural hypotheses exhausted; no adoption.
 * H2 has the lowest aligned distance, 314; H3 preserves the aggregate-layout
 * observation but does not qualify as a register-only near miss.
 * H3 witness: 79d849562d691b1b12e99bd7db0b94ca3a0c3464. Canonical candidate restored to H2;
 * no new experiment. Complete extent remains not-yet-C, with raw unchanged.
 */
#include "TYPES.H"
#include "SYSTEM.H"
#include "RUNTIME_MEM.H"
#include "IWRAM_CALL.H"
#include "BATTLE_UNIT.H"
#include "RENDER_INPUT.H"
#include "CALLBACK_SCHEDULER.H"

struct MenuCursor {
    u8 unknown00[5];
    u8 state;
};

struct MenuActionWork {
    u8 unknown000[0x10];
    struct RenderInput *title_window;
    struct MenuCursor *cursor;
    u8 unknown018[0x0c];
    struct RenderInput *left_window;
    struct RenderInput *saved_window;
    u8 unknown02c[4];
    struct RenderInput *list_window;
    struct RenderInput *right_window;
    u8 unknown038[0xd4];
    struct RenderInput *message_window;
    u8 unknown110[0x34];
    u16 row_y[4];
    u8 unknown14c[0xce];
    u8 owner;
    u8 unknown21b[0x3d];
    u8 preview_owner;
};

struct MenuRenderWork {
    u8 unknown000[0xea3];
    u8 dirty_rows;
    u8 unknownea4[2];
    u8 menu_busy;
};

extern struct MenuActionWork *gMenuWork;
extern struct MenuRenderWork *gWindowWork;
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
extern u8 Data_000000c8[];
extern u8 MsgCurrentStatus[];
extern const u8 Menu_BackdropFrameTile[];

void Runtime_SetMainState19(void);
struct BattleUnit *Owner_GetStateFar(s32 owner);
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *first,
    struct OwnerActionSlot *second, u16 *out, s32 *first_count, s32 *second_count);
s32 UiWindow_UpdateOrCreate(struct RenderInput **window,
    s32 x, s32 y, s32 width, s32 height, s32 flags);
void UiWindow_CloseIfOpen(struct RenderInput **window, s32 release);
void RenderOutput_ClearListFar(struct RenderInput *window);
void RenderOutput_RedrawSavedRectFar(struct RenderInput *window);
void RenderOutput_PrepareForRedrawFar(struct RenderInput *window);
void UiText_DrawCharacterAtOffsetFar(s32 message,
    struct RenderInput *window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(struct RenderInput *window,
    s32 tile, s32 x, s32 y, s32 palette);
s32 DjinnMenu_DrawStatPreview(struct RenderInput *window,
    s32 x, s32 y, s32 owner, s32 give, s32 take, s32 mode, s32 page, s32 side);
s32 FourObjectMotion_SetSlotPosition(s32 slot, s32 x, s32 y, s32 hidden);
s32 Menu_GetModuloOfSum(s32 value, s32 modulus);
void Menu_UpdateEntryObjectTransforms(void);
void AudioCommand_PlayFar(s32 cue);

s32 Unnamed_080ae2f4(void)
{
    s32 cursor[2];
    s32 pages[2];
    s32 gained;
    s32 lost;
    s32 phase;
    s32 pending;
    s32 frame;
    struct RenderInput **left;
    struct RenderInput **right;
    struct MenuRenderWork *render;
    u32 keys;
    u32 repeat;
    u8 *preview;
    s32 *sel;
    struct MenuActionWork *menu;
    struct BattleUnit *owner;
    struct RenderInput *window;
    u16 *actions;
    void *saved;
    s32 result;
    s32 tile;
    s32 i;
    s32 none;
    s32 side;
    s32 *page;

    pending = 1;
    phase = 0;
    frame = 0;
    menu = gMenuWork;
    menu->cursor->state = 13;
    sel = cursor;
    sel[1] = sel[0] = 0;
    for (i = 3; i >= 0; i--)
        menu->row_y[i] = (s32)Data_000000c8;
    RenderOutput_ClearListFar(menu->list_window);
    WaitFrames(1);
    page = pages;
    page[1] = page[0] = 1;
    actions = Runtime_BumpAllocateAlternatePool(96);
    saved = Runtime_BumpAllocateAlternatePool(332);
    owner = Owner_GetStateFar(menu->owner);
    page[1] = page[0] = OwnerAction_DiffSlots(owner->action_slots, owner->action_slots,
        actions, &gained, &lost);
    Runtime_BumpFree(saved);
    Runtime_BumpFree(actions);
    page[0] = (page[0] - 1) / 6 + 1;
    if (page[0] == 0)
        page[0] = 1;
    page[1] = (page[1] - 1) / 6 + 1;
    if (page[1] == 0)
        page[1] = 1;
    left = &menu->left_window;
    UiWindow_UpdateOrCreate(left, 0, 5, 15, 15, 2);
    right = &menu->right_window;
    UiWindow_UpdateOrCreate(right, 15, 5, 15, 15, 2);
    RenderOutput_RedrawSavedRectFar(menu->message_window);
    RenderOutput_RedrawSavedRectFar(menu->title_window);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCurrentStatus, menu->title_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCurrentStatus + 2, menu->title_window, 0, 16);
    preview = &menu->preview_owner;
    none = 0;
    side = 0;
    while (1) {
        render = gWindowWork;
        keys = gKeyState;
        repeat = gKeysRepeat;
        if (pending) {
            render->menu_busy = 1;
            RenderOutput_PrepareForRedrawFar(menu->left_window);
            RenderOutput_PrepareForRedrawFar(menu->right_window);
            DjinnMenu_DrawStatPreview(menu->left_window, 0, 0, *preview, none, none, 3, none, 1);
            DjinnMenu_DrawStatPreview(menu->right_window, 0, 0, *preview, none, none, 3, cursor[0] + 1, 1);
            render->menu_busy = none;
        }
        if (page[side] > 1) {
            window = menu->right_window;
            for (i = 0; i < page[side]; i++) {
                tile = 0xf031 + i;
                if (i > 9)
                    tile = 0xf030;
                if (i == sel[side])
                    tile -= 0x1000;
                UiWindow_SetTilemapEntryFar(window, tile, window->width - page[side] + i - 2, -1, none);
            }
            UiWindow_SetTilemapEntryFar(window, 0xf128, window->width - page[side] - 3, -1, none);
            UiWindow_SetTilemapEntryFar(window, 0xf129, window->width - 2, -1, none);
            render->dirty_rows |= 2 << (window->y >> 2);
        }
        frame++;
        FourObjectMotion_SetSlotPosition(0, 32, 200, 0);
        if (pending) {
            pending = 0;
            phase = Menu_GetModuloOfSum(phase, 2);
        }
        if ((frame & 3) == 0) {
            if (frame & 4)
                Iwram_CopyWords((void *)0x060052c0, Menu_BackdropFrameTile, 32);
            else
                Iwram_FillWords((void *)0x060052c0, 32, 0x44444444);
        }
        if (keys & 8) {
            AudioCommand_PlayFar(113);
            result = -2;
            break;
        }
        if (keys & 0x303) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }
        if (repeat & 32) {
            sel[none]--;
            sel[none] = Menu_GetModuloOfSum(sel[none], page[none]);
            AudioCommand_PlayFar(111);
            Runtime_SetMainState19();
            pending = 1;
        } else if (repeat & 16) {
            sel[none]++;
            AudioCommand_PlayFar(111);
            Runtime_SetMainState19();
            pending = 1;
            sel[none] = Menu_GetModuloOfSum(sel[none], page[none]);
        }
        WaitFrames(1);
    }
    Scheduler_AddOrUpdateCallback((s32)Menu_UpdateEntryObjectTransforms, 0xc80);
    gWindowWork->menu_busy = 1;
    UiWindow_CloseIfOpen(&menu->message_window, 1);
    WaitFrames(1);
    UiWindow_UpdateOrCreate(&menu->message_window, 13, 0, 17, 5, 2);
    UiWindow_CloseIfOpen(left, 1);
    UiWindow_CloseIfOpen(right, 1);
    RenderOutput_RedrawSavedRectFar(menu->list_window);
    RenderOutput_RedrawSavedRectFar(menu->saved_window);
    RenderOutput_RedrawSavedRectFar(menu->title_window);
    gWindowWork->menu_busy = 0;
    WaitFrames(1);
    return result;
}
