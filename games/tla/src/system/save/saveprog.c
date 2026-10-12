#include "types.h"
#include "graphics.h"
#include "window.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"

extern u8 MsgSaving;
extern u8 MsgErasing;
extern u8 MsgCopying;
extern u8 MsgDoNotTurnThePower;

extern const u8 Data_0805f585[];

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawResource(s32 resource, struct UiWindow *window, s32 x, s32 y);
void Audio_SetMusicVolumeTargetAndStepFar(s32 target, s32 step);
void Audio_PlayCue(s32 cue);
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
// スペイン語版とイタリア語版の far call 表では、同じ処理へ届く口の名前が違う
void Func_080c8688(void);
#define Func_080c8698 Func_080c8688
#else
void Func_080c8698(void);
#endif

// 見出しの x: フランス語版だけ違う
#if defined(TLA_EDITION_FR)
#define SAVING_X  40
#define ERASING_X 48
#define COPYING_X 48
#else
#define SAVING_X  76
#define ERASING_X 92
#define COPYING_X 84
#endif

// 記録中の画面を開く: 0 は記録、1 は消去、2 は写し
struct UiWindow *SaveProgress_Open(u32 kind)
{
    struct UiWindow *window;
    void *tiles;
    s32 message;

    window = UiWindow_Create(0, 0, 30, 20, 0);
    tiles = Runtime_BumpAllocateAlternatePool(0x100);
    Resource_DecodeType01(Data_0805f585, tiles);
    Dma_Set(tiles, (void *)0x06001000, 0x84000040, REG_DMA3);
    switch (kind) {
    case 0:
        UiText_DrawResource((s32)&MsgSaving, window, SAVING_X, 40);
        break;
    case 1:
        UiText_DrawResource((s32)&MsgErasing, window, ERASING_X, 40);
        break;
    case 2:
        UiText_DrawResource((s32)&MsgCopying, window, COPYING_X, 40);
        break;
    }
    message = (s32)&MsgDoNotTurnThePower;
    UiText_DrawResource(message, window, 40, 88);
    UiText_DrawResource(message + 1, window, 40, 104);
    Audio_SetMusicVolumeTargetAndStepFar(0, 30);
    Audio_PlayCue(298);
    WaitFrames(10);
    MemFree(tiles);
    return window;
}

// 記録中の画面を閉じる
struct UiWindow *SaveProgress_Close(struct UiWindow *window)
{
    WaitFrames(10);
    Func_080c8698();
    Audio_SetMusicVolumeTargetAndStepFar(0x100, 30);
    UiWork_Finalize(window, 0);
    return window;
}
