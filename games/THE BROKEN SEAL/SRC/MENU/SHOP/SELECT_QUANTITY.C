#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "IO_REG.H"
#include "MEM.H"
#include "EDITION.H"
#include "DMA.H"

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
#include "text/MSG_IDS.H"

#define gKeyState (*(volatile u32 *)&gKeyState)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)

#define Shop_FillSelector ((void (*)(s32,s32,void*))Shop_FillSelector)


void AudioCommand_PlayFar(s32 cue);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void WaitFrames(s32 frames);
void UiWork_FinalizeFar(s32 window, s32 mode);

s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price)
{
    void *buffer;
    s32 window;
    s32 changed;
    s32 slot;
    s32 count;
    s32 result;
    struct RenderOutput *sprite;
#if !EDITION_INTERNATIONAL
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
            RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 0, 0);
            sprite = RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 32, 0);
            sprite->table.bits.index += 4;
            for (;;) {
                if (gKeyState & KEY_A) {
                    AudioCommand_PlayFar(112);
                    result = count + 1;
                    break;
                }
                if (gKeyState & KEY_B) {
                    AudioCommand_PlayFar(113);
                    result = -1;
                    break;
                }
                if (gKeysRepeat & KEY_LEFT) {
                    AudioCommand_PlayFar(111);
                    changed = 1;
                    count--;
                }
                if (gKeysRepeat & KEY_RIGHT) {
                    AudioCommand_PlayFar(111);
                    changed = 1;
                    count++;
                }
                if (changed) {
                    changed = 0;
                    count = (count + maximum) % maximum;
                    Dma_Set(Shop_QuantityTiles, buffer, 0x84000040, REG_DMA3);
                    Shop_FillSelector(30, 14, buffer);
                    Shop_FillSelector(minimum + maximum, 0, buffer);
                    Shop_FillSelector(minimum + count + 1, 10, buffer);
                    Shop_FillSelector(minimum, 2, buffer);
                    VramBlock_LoadCached(slot, 0x100, buffer);
                    result = count + 1;
#if EDITION_INTERNATIONAL
                    UiText_DrawNumberInWindowFar(result, 2, window, QUANTITY_X, 0);
                    UiText_DrawNumberInWindowFar(price * result, 6, window, QUANTITY_X + 16, 0);
                    UiText_DrawCharacterAtOffsetFar((s32)MsgCoins, window, COINS_X, 0);
#else
                    UiText_DrawNumberInWindowFar(result, 2, window, 64, 0);
                    msg = (s32)MsgPiece;
                    UiText_DrawCharacterAtOffsetFar(msg, window, 80, 0);
                    UiText_DrawNumberInWindowFar(price * result, 6, window, 96, 0);
                    UiText_DrawCharacterAtOffsetFar(msg - 1, window, 144, 0);
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
