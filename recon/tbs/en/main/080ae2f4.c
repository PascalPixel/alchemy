#include "TYPES.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "OWNER_STATE.H"
#include "RENDER_INPUT.H"
#include "CALLBACK_SCHEDULER.H"

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
 * Reference has a dead Math_Mod(frame, 60)-5 before slot positioning. Its
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
 * recovered (cursor r8, pages sl), but menu stays r7 and render/keys occupy
 * fp/sl instead of reference stack slots. Pool remains at the tail.
 * -dL: row symbol starts SI then narrows to HI. -dA minipool dump reports an
 * HImode symbol fixup with range 1020, not short reach: no width/reach claim.
 * H2 does not satisfy the reference frame/pool admission constraint.
 */

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
extern struct MenuRenderWork *Data_03001e8c;
extern volatile u32 Data_03001c94;
extern volatile u32 Data_03001b04;
extern u8 Data_000000c8[];

void Runtime_BumpFree(void *block);
s16 *Runtime_BumpAllocateAlternatePool(s32 bytes);
void Runtime_SetMainState19(void);
struct OwnerInventoryState *Owner_GetStateFar(s32 owner);
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *first,
    struct OwnerActionSlot *second, u16 *out, s32 *first_count, s32 *second_count);
s32 UiWindow_UpdateOrCreate(struct RenderInput **window,
    s32 x, s32 y, s32 width, s32 height, s32 flags);
/* The callee ignores the second argument, which all three reference calls set. */
void UiWindow_CloseIfOpen(struct RenderInput **window, s32 release);
void RenderOutput_ClearListFar(struct RenderInput *window);
void RenderOutput_RedrawSavedRectFar(struct RenderInput *window);
void Func_08015060(struct RenderInput *window);
void UiText_DrawCharacterAtOffsetFar(s32 message,
    struct RenderInput *window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(struct RenderInput *window,
    s32 tile, s32 x, s32 y, s32 palette);
s32 DjinnMenu_DrawStatPreview(struct RenderInput *window,
    s32 x, s32 y, u8 owner, s32 first, s32 second, s32 mode, s32 page, s32 flags);
void FourObjectMotion_SetSlotPosition(s32 slot, s32 x, s32 y, s32 hidden);
s32 Menu_GetModuloOfSum(s32 value, s32 modulus);
void Menu_UpdateEntryObjectTransforms(void);
void Audio_PlayCue(s32 cue);

static __inline__ void Menu_SetPair(s32 *pair, s32 value)
{
    pair[0] = value;
    pair[1] = pair[0];
}

static __inline__ s32 Menu_GetPair(s32 *pair, s32 index)
{
    return pair[index];
}

static __inline__ void Menu_PutPair(s32 *pair, s32 index, s32 value)
{
    pair[index] = value;
}

s32 Unnamed_080ae2f4(void)
{
    s32 cnt;
    s32 tile;
    s32 result;
    u16 *buf;
    void *owner_buf;
    struct OwnerActionState *owner;
    struct MenuActionWork *work;
    struct RenderInput *window;
    struct MenuRenderWork *render;
    u32 keys;
    u32 repeat;
    s32 frame;
    s32 pending;
    s32 phase;
    s32 second_count;
    s32 first_count;
    s32 pages[2];
    s32 cursor[2];
    u16 *pos;
    s32 *page;
    s32 *selection;
    s32 none;
    u16 row_y;

    pending = 1;
    phase = 0;
    frame = 0;
    work = gMenuWork;
    work->cursor->state = 13;
    selection = cursor;
    Menu_SetPair(selection, 0);
    cnt = 3;
    pos = &work->row_y[3];
    row_y = (u16)(s32)Data_000000c8;
    do {
        cnt--;
        *pos = row_y;
        pos--;
    } while (cnt >= 0);
    RenderOutput_ClearListFar(work->list_window);
    WaitFrames(1);
    page = pages;
    Menu_SetPair(page, 1);
    buf = (u16 *)Runtime_BumpAllocateAlternatePool(96);
    owner_buf = Runtime_BumpAllocateAlternatePool(0x14c);
    owner = (struct OwnerActionState *)Owner_GetStateFar(work->owner);
    Menu_SetPair(page, OwnerAction_DiffSlots(owner->action_slots,
        owner->action_slots, buf, &first_count, &second_count));
    Runtime_BumpFree(owner_buf);
    Runtime_BumpFree(buf);
    Menu_PutPair(page, 0, Math_Div(Menu_GetPair(page, 0) - 1, 6) + 1);
    if (Menu_GetPair(page, 0) == 0)
        Menu_PutPair(page, 0, 1);
    Menu_PutPair(page, 1, Math_Div(Menu_GetPair(page, 1) - 1, 6) + 1);
    if (Menu_GetPair(page, 1) == 0)
        Menu_PutPair(page, 1, 1);
    UiWindow_UpdateOrCreate(&work->left_window, 0, 5, 15, 15, 2);
    UiWindow_UpdateOrCreate(&work->right_window, 15, 5, 15, 15, 2);
    RenderOutput_RedrawSavedRectFar(work->message_window);
    RenderOutput_RedrawSavedRectFar(work->title_window);
    UiText_DrawCharacterAtOffsetFar(0xbaa, work->title_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar(0xbac, work->title_window, 0, 16);
    none = 0;

    for (;;) {
        render = Data_03001e8c;
        keys = Data_03001c94;
        repeat = Data_03001b04;
        if (pending) {
            render->menu_busy = 1;
            Func_08015060(work->left_window);
            Func_08015060(work->right_window);
            DjinnMenu_DrawStatPreview(work->left_window, 0, 0,
                work->preview_owner, none, none, 3, none, 1);
            DjinnMenu_DrawStatPreview(work->right_window, 0, 0,
                work->preview_owner, none, none, 3, Menu_GetPair(selection, 0) + 1, 1);
            render->menu_busy = none;
        }
        if (Menu_GetPair(page, none) > 1) {
            window = work->right_window;
            for (cnt = 0; cnt < Menu_GetPair(page, none); cnt++) {
                tile = 0xf031 + cnt;
                if (cnt > 9)
                    tile = 0xf030;
                if (cnt == Menu_GetPair(selection, none))
                    tile += -0x1000;
                UiWindow_SetTilemapEntryFar(window, tile,
                    window->width - Menu_GetPair(page, 0) + cnt - 2, -1, none);
            }
            UiWindow_SetTilemapEntryFar(window, 0xf128,
                window->width - Menu_GetPair(page, none) - 3, -1, none);
            UiWindow_SetTilemapEntryFar(window, 0xf129, window->width - 2, -1, none);
            render->dirty_rows |= 2 << (window->y >> 2);
        }
        frame++;
        Math_Mod(frame, 60) - 5;
        FourObjectMotion_SetSlotPosition(0, 32, 200, 0);
        if (pending) {
            pending = 0;
            phase = Menu_GetModuloOfSum(phase, 2);
        }
        if ((frame & 3) == 0) {
            if (frame & 4)
                ((void (*)(void *, const void *, s32))0x03001388)(
                    (void *)0x060052c0, (const void *)0x080af26c, 32);
            else
                ((void (*)(void *, s32, u32))0x03000168)(
                    (void *)0x060052c0, 32, 0x44444444);
        }
        if (keys & 8)
            result = 2;
        else if (keys & 0x303)
            result = 1;
        else {
            if (repeat & 32) {
                Menu_PutPair(selection, 0, Menu_GetPair(selection, 0) - 1);
                Menu_PutPair(selection, 0, Menu_GetModuloOfSum(
                    Menu_GetPair(selection, 0), Menu_GetPair(page, none)));
                Audio_PlayCue(111);
                Runtime_SetMainState19();
                pending = 1;
            } else if (repeat & 16) {
                Menu_PutPair(selection, 0, Menu_GetPair(selection, 0) + 1);
                Audio_PlayCue(111);
                Runtime_SetMainState19();
                pending = 1;
                Menu_PutPair(selection, 0, Menu_GetModuloOfSum(
                    Menu_GetPair(selection, 0), Menu_GetPair(page, none)));
            }
            WaitFrames(1);
            continue;
        }
        Audio_PlayCue(113);
        result = -result;
        break;
    }
    Scheduler_AddOrUpdateCallback((s32)Menu_UpdateEntryObjectTransforms, 0xc80);
    Data_03001e8c->menu_busy = 1;
    UiWindow_CloseIfOpen(&work->message_window, 1);
    WaitFrames(1);
    UiWindow_UpdateOrCreate(&work->message_window, 13, 0, 17, 5, 2);
    UiWindow_CloseIfOpen(&work->left_window, 1);
    UiWindow_CloseIfOpen(&work->right_window, 1);
    RenderOutput_RedrawSavedRectFar(work->list_window);
    RenderOutput_RedrawSavedRectFar(work->saved_window);
    RenderOutput_RedrawSavedRectFar(work->title_window);
    Data_03001e8c->menu_busy = 0;
    WaitFrames(1);
    return result;
}
