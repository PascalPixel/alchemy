/* Draft, not exact (2026-09-24): candidate=696 reference=712, 317 differing
   halfwords. Hand-written from the assembly on the pattern of
   Menu_SelectQuantity. Residual: the reference keeps the window in r7 and the
   quantity in r8 (this candidate swaps them), spills range, count, other count
   and the resource slot to different stack slots, and reloads range twice for
   Math_Mod. */
/* Item menu: choose how many of the selected item to hand from one party
   member to another. Left and right step the amount, A
   confirms and B cancels with -1. */
#include "TYPES.H"
#include "DMA.H"

#define MENU_FIELD(menu, type, offset) (*(type *)((u8 *)(menu) + (offset)))

struct RenderObject {
    u8 padding00[0x18];
    u16 tile : 10;
    u16 attributes : 6;
};

extern u8 *gMenuWork;
extern volatile s32 gKeysRepeat;
extern volatile u32 gKeyState;
extern const u8 Data_080af08c[];

void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void ItemMenu_SetMsgWin7(void);
s32 RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
s32 Func_080a3d9c(s32 owner, s32 item);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *source);
struct RenderObject *RenderOutput_CreateFar(s32 slot, s32 attributes, s32 window, s32 x, s32 y);
void UiMenu_SlideCursor(s32 x, s32 y);
void UiMenu_PositionCursor(s32 x, s32 y);
s32 Math_Mod(s32 value, s32 modulus);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Func_080b0040(s32 value, s32 column, void *tiles);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void *Owner_GetStateFar(s32 owner);
void UiText_DrawStringAtOffsetFar(void *text, s32 window, s32 x, s32 y);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);
s32 GameFlag_TestFar(s32 flag);

s32 ItemMenu_SelectGiveQuantity(s32 base, s32 range, s32 single)
{
    u8 *menu = gMenuWork;
    u8 *tiles = Runtime_AllocateBlock(14, 0x400);
    s32 other_count = 0;
    s32 changed = 1;
    s32 window = MENU_FIELD(menu, s32, 268);
    s32 count;
    s32 slot;
    s32 quantity;
    struct RenderObject *object;

    ItemMenu_SetMsgWin7();
    RenderOutput_RedrawSavedRectFar(window);
    quantity = base;
    if (single == 0)
        other_count = Func_080a3d9c(MENU_FIELD(menu, u8, 0x21b), MENU_FIELD(menu, u16, 376) & 0x1ff);
    count = Func_080a3d9c(MENU_FIELD(menu, u8, 0x21a), MENU_FIELD(menu, u16, 376) & 0x1ff);
    slot = Resource_FindFreeEntry();
    if (slot == 96)
        goto done;
    VramBlock_LoadCached(slot, 256, 0);
    RenderOutput_CreateFar(slot, 0x40004000, window, 48, 32);
    object = RenderOutput_CreateFar(slot, 0x40004000, window, 80, 32);
    object->tile += 4;
    UiMenu_SlideCursor(128, 40);
    goto check_exit;

update:
    if (changed == 0)
        goto input;
    changed = 0;
    quantity = Math_Mod(range + quantity, range);
    RenderOutput_RedrawSavedRectFar(window);
    UiText_DrawCharacterAtOffsetFar(0xade, window, 32, 0);
    Dma_Set(Data_080af08c, tiles, 0x84000040, (volatile u32 *)0x040000d4);
    Func_080b0040(30, 14, tiles);
    Func_080b0040(range + base, 0, tiles);
    Func_080b0040(base + quantity + 1, 10, tiles);
    Func_080b0040(base, 2, tiles);
    VramBlock_LoadCached(slot, 256, tiles);
    UiText_DrawNumberInWindowFar(quantity + 1, 2, window, 32, 32);
    UiText_DrawCharacterAtOffsetFar((MENU_FIELD(menu, u16, 376) & 0x1ff) + 0x182, window, 16, 8);
    UiText_DrawNumberInWindowFar(count - quantity - 1, 2, window, 16, 24);
    if (single == 0)
        UiText_DrawNumberInWindowFar(other_count + quantity + 1, 2, window, 80, 24);
    UiText_DrawStringAtOffsetFar(Owner_GetStateFar(MENU_FIELD(menu, u8, 0x21a)), window, 16, 16);
    if (single == 0)
        UiText_DrawStringAtOffsetFar(Owner_GetStateFar(MENU_FIELD(menu, u8, 0x21b)), window, 80, 16);

input:
    {
        volatile u32 *keys = &gKeyState;

        if (*keys & 1) {
            Audio_PlayCue(112);
            goto done;
        }
        if (*keys & 2) {
            quantity = -1;
            Audio_PlayCue(113);
            goto done;
        }
    }
    UiMenu_PositionCursor(128, 40);
    {
        volatile s32 *keys = &gKeysRepeat;

        if (*keys & 32) {
            quantity -= 1;
            changed = 1;
            Audio_PlayCue(111);
        }
        if (*keys & 16) {
            quantity += 1;
            changed = 1;
            Audio_PlayCue(111);
        }
    }
    WaitFrames(1);

check_exit:
    if (GameFlag_TestFar(336) == 0)
        goto update;

done:
    RenderOutput_RedrawSavedRectFar(window);
    RenderOutput_ClearListFar(window);
    Runtime_ReleaseHeapBlock(14);
    MENU_FIELD(menu, u8 *, 540)[5] = 13;
    if (GameFlag_TestFar(336))
        quantity = -1;
    return quantity;
}
