/* 2026-09-24: 4 differing halfwords, 156 of 156 bytes. Holding the
   Data_02000240 base in a local brings the draft from 7 to 4; the rest is
   prologue scheduling: the reference issues sub sp before the two pool
   loads and the 224 offset before the two stores of 8. A sweep of every
   order and do-while wrap of the four opening statements stops here. */
#include "TYPES.H"
#include "TBS_EDITION.H"

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

s32 Func_0808a5d0(s16, s16);
s32 UiText_GetResourceDimensions(s32, s32, s32, s32, s32);
s32 UiWindow_Create(s32, s32, s32, s32, s32);
void UiText_DrawResource(s32, s32, s32, s32);
void UiTimedNotice_Tick(void);
s32 ScheduleCallbackAfterFrames(const void *, s32);

void UiTimedNotice_Create(void)
{
    register u8 *base;
    s32 fourth;
    s32 third;
    s32 y;
    s32 x;
    s32 resource;
    s32 object;
    u16 *field;
    u16 *cfg;

    base = Data_03001ebc;
    cfg = Data_02000240;
    do { y = 8; } while (0);
    x = 8;
    resource = Func_0808a5d0(cfg[224], cfg[225]) + RENDER_RESOURCE_BASE;
    UiText_GetResourceDimensions(resource, (s32)&x, (s32)&y, (s32)&third, (s32)&fourth);
    x = (30 - third) >> 1;
    y = (10 - fourth) >> 1;
    object = UiWindow_Create(x, y, third, fourth, 2);
    /* 生成した番号を+0x230へ保存してから次の処理を呼ぶ。 */
    *(s32 *)(base + 0x230) = object;
    UiText_DrawResource(resource, object, 0, 0);
    field = (u16 *)(base + 0x234);
    {
        register s32 ninety = 90;
        *field = ninety;
    }
    ScheduleCallbackAfterFrames((void *)UiTimedNotice_Tick, 0xc80);
}
