#include "types.h"
#include "scene.h"

#define FIELD(ptr, type, offset) (*(type *)((u8 *)(ptr) + (offset)))

struct MenuObjectControl {
    u8 padding00[4];
    u16 suspended;
};

extern struct MenuObjectControl *gIw;

s32 Runtime_AllocateHeapBlock(s32 kind, s32 size);

void WaitFrames(s32 frames);

s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);

void Menu_CancelSoundReset(void);

void Menu_EnsureCancelSound(void);
s32 Ability_GetData(s32 flags);

/*
 * Open a modal menu screen and run its blocking interaction body.
 *
 * The address of gIw, not the pointer it holds, is the base for the
 * two fixed-address fields at +0x24 and +0x54; only "suspended" is reached
 * through the pointer itself.  The field read at +0x178 is named by position
 * only and is not otherwise confirmed.
 */
s32 Menu_OpenConfirmPrompt(void)
{
    void *state = (void *)Runtime_AllocateHeapBlock(0x37, 0xa70);
    s32 high;
    s32 unused;
    s32 low;
    s32 result;

    gIw->suspended = 1;
    Menu_SetMode(0, 0, 30, 20);
    WaitFrames(1);
    Menu_Do(0);
    FIELD(state, u8, 0x219) = (u8)Menu_Check((const u16 *)((u8 *)state + 0x208));
    Menu_SetMode2(0, 3, 0, 7);
    FIELD(state, s32, 0x10c) = UiWindow_CreateFar(13, 0, 17, 3, 2);
    Menu_unk2_2(14);
    Menu_unk3_2(0x06002500);
    Menu_CancelSoundReset();
    result = Menu_Place(
        &high, &unused, &low);
    Menu_EnsureCancelSound();
    if (result == 1) {
        void *target = FIELD(&gIw, void *, 0x54);
        u16 flags;
        Ability_GetData(0x3fff & FIELD(state, u16, 0x178));
        flags = (u16)(low | (high << 10));
        FIELD(target, u16, 0x17e) = flags;
    }
    Menu_unk4_2(FIELD(state, s32, 0x24));
    FIELD(FIELD(&gIw, void *, 0x24), u8, 0xea6) = 1;
    Menu_Run();
    Menu_SetMode(0, 0, 30, 20);
    Runtime_ReleaseHeapBlock(0x37);
    gIw->suspended = 0;
    WaitFrames(1);
    Menu_SetMode3(0, 0, 30, 20);
    FIELD(FIELD(&gIw, void *, 0x24), u8, 0xea6) = 0;
    Menu_unk2_4();
    return result;
}
