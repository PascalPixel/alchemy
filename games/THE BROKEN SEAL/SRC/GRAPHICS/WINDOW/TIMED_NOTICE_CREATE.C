#include "TYPES.H"
#include "TBS_EDITION.H"
#include "FIELD_EVENT.H"

s32 BattleFx_FindConditionResourceFar(s16 scene, s16 entrance);
s32 UiText_GetResourceDimensions(s32 resource, s32 *x, s32 *y, s32 *width, s32 *height);
s32 UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawResource(s32 resource, s32 window, s32 x, s32 y);
void UiTimedNotice_Tick(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);

/* Show the text the party's scene and entrance select, centred in a
   window, and let UiTimedNotice_Tick close it after 90 frames. The window
   and its countdown live in the event work at +0x230 and +0x234. */
void UiTimedNotice_Create(void)
{
    u8 *work;
    s32 height;
    s32 width;
    s32 y;
    s32 x;
    s32 resource;
    s32 window;
    u16 *timer;
    s32 frames;

    work = (u8 *)gEventWork;
    x = 8;
    y = 8;
    resource = BattleFx_FindConditionResourceFar(gGameState.scene, gGameState.entrance) + RENDER_RESOURCE_BASE;
    UiText_GetResourceDimensions(resource, &x, &y, &width, &height);
    x = (30 - width) >> 1;
    y = (10 - height) >> 1;
    window = UiWindow_Create(x, y, width, height, 2);
    *(s32 *)(work + 0x230) = window;
    UiText_DrawResource(resource, window, 0, 0);
    timer = (u16 *)(work + 0x234);
    frames = 90; /* FAKEMATCH: a word temporary keeps 90 out of the HImode pool. */
    *timer = frames;
    Scheduler_AddOrUpdateCallback(UiTimedNotice_Tick, 0xc80);
}
