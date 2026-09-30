#include "DMA.H"

/* The quantity, total and coins columns each edition's words leave room for. */
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define QUANTITY_X 64
#define COINS_X    128
#elif defined(TBS_EDITION_ES)
#define QUANTITY_X 72
#define COINS_X    144
#else
#define QUANTITY_X 72
#define COINS_X    136
#endif
#include "FIXED_MATH.H"
#include "RESOURCE.H"
#include "UI.H"
#include "SHOP.H"

extern u8 gKeyState[];
extern u8 gKeysRepeat[];
extern u8 MsgCoins[];
extern u8 MsgPiece[];

struct UiSprite {
    u8 unknown_00[24];
    u16 tile : 10;
    u16 flags : 6;
};

void *Runtime_AllocateBlock(s32 kind, s32 size);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
struct UiSprite *RenderOutput_CreateFar(s32 slot, s32 attributes, s32 window, s32 x, s32 y);
void Audio_PlayCue(s32 cue);
void Shop_FillSelector(s32 value, s32 x, void *buffer);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
s32 WaitFrames(s32 frames);
void UiWork_FinalizeFar(s32 window, s32 mode);
void Runtime_ReleaseHeapBlock(s32 kind);

/* Spins a quantity from minimum + 1 to maximum with left and right, redrawing
   the count and its total price; returns the chosen count, or -1 when
   cancelled. */
s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price)
{
    void *buffer;
    s32 window;
    s32 changed;
    s32 slot;
    s32 count;
    s32 result;
    struct UiSprite *sprite;
#if defined(TBS_EDITION_JA)
    s32 msg;
#endif

    buffer = Runtime_AllocateBlock(14, 0x400);
    changed = 1;
    maximum -= minimum;
    result = -1;
    count = 0;
    window = UiWindow_CreateFar(7, 4, 23, 3, 2);
    if (window != 0) {
        slot = Resource_FindFreeEntry();
        if (slot != 96) {
            VramBlock_LoadCached(slot, 0x100, 0);
            RenderOutput_CreateFar(slot, 0x40004000, window, 0, 0);
            sprite = RenderOutput_CreateFar(slot, 0x40004000, window, 32, 0);
            sprite->tile += 4;
            for (;;) {
                if (*(volatile u32 *)gKeyState & 1) {
                    Audio_PlayCue(112);
                    result = count + 1;
                    break;
                }
                if (*(volatile u32 *)gKeyState & 2) {
                    Audio_PlayCue(113);
                    result = -1;
                    break;
                }
                if (*(volatile u32 *)gKeysRepeat & 32) {
                    Audio_PlayCue(111);
                    changed = 1;
                    count--;
                }
                if (*(volatile u32 *)gKeysRepeat & 16) {
                    Audio_PlayCue(111);
                    changed = 1;
                    count++;
                }
                if (changed) {
                    changed = 0;
                    count = Math_Mod(count + maximum, maximum);
                    Dma_Set(Shop_QuantityTiles, buffer, 0x84000040, (volatile u32 *)0x040000d4);
                    Shop_FillSelector(30, 14, buffer);
                    Shop_FillSelector(minimum + maximum, 0, buffer);
                    Shop_FillSelector(minimum + count + 1, 10, buffer);
                    Shop_FillSelector(minimum, 2, buffer);
                    VramBlock_LoadCached(slot, 0x100, buffer);
                    result = count + 1;
#if defined(TBS_EDITION_JA)
                    /* The Japanese count takes its counter word, and the
                       coins label is the message before it. */
                    UiText_DrawNumberInWindowFar(result, 2, window, 64, 0);
                    msg = (s32)MsgPiece;
                    UiText_DrawCharacterAtOffsetFar(msg, window, 80, 0);
                    UiText_DrawNumberInWindowFar(price * result, 6, window, 96, 0);
                    UiText_DrawCharacterAtOffsetFar(msg - 1, window, 144, 0);
#else
                    UiText_DrawNumberInWindowFar(result, 2, window, QUANTITY_X, 0);
                    UiText_DrawNumberInWindowFar(price * result, 6, window, QUANTITY_X + 16, 0);
                    UiText_DrawCharacterAtOffsetFar((s32)MsgCoins, window, COINS_X, 0);
#endif
                }
                WaitFrames(1);
            }
            WaitFrames(1);
            UiWork_FinalizeFar(window, 2);
        }
    }
    Runtime_ReleaseHeapBlock(14);
    return result;
}
