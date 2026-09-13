#include "types.h"

#define FIELD(ptr, type, offset) (*(type *)((u8 *)(ptr) + (offset)))

struct MenuObjectControl {
    u8 padding00[4];
    u16 suspended;
};

extern struct MenuObjectControl *Data_03001e68;

s32 Runtime_AllocateHeapBlock(s32 kind, s32 size);
void Func_08015408(s32 x, s32 y, s32 width, s32 height);
void WaitFrames(s32 frames);
void Func_080a1090(s32 unused);
s32 Func_08077158(const u16 *ids);
void Func_080a3354(s32, s32, s32, s32);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void Func_080a2144(s32 index);
void Func_08015418(s32 addr);
void Menu_CancelSoundReset(void);
s32 Func_080a5cc0(s32 *, s32 *, s32 *);
void Menu_EnsureCancelSound(void);
s32 Ability_GetData(s32 flags);
void Func_08015278(s32 screen_handle);
void Func_080a34c0(void);
void Func_08002dd8(s32 kind);
void Func_08015410(s32 x, s32 y, s32 width, s32 height);
void Func_0808a548(void);

/*
 * Open a modal menu screen and run its blocking interaction body.
 *
 * The address of Data_03001e68, not the pointer it holds, is the base for the
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

    Data_03001e68->suspended = 1;
    Func_08015408(0, 0, 30, 20);
    WaitFrames(1);
    Func_080a1090(0);
    FIELD(state, u8, 0x219) = (u8)Func_08077158((const u16 *)((u8 *)state + 0x208));
    Func_080a3354(0, 3, 0, 7);
    FIELD(state, s32, 0x10c) = UiWindow_CreateFar(13, 0, 17, 3, 2);
    Func_080a2144(14);
    Func_08015418(0x06002500);
    Menu_CancelSoundReset();
    result = Func_080a5cc0(
        &high, &unused, &low);
    Menu_EnsureCancelSound();
    if (result == 1) {
        void *target = FIELD(&Data_03001e68, void *, 0x54);
        u16 flags;
        Ability_GetData(0x3fff & FIELD(state, u16, 0x178));
        flags = (u16)(low | (high << 10));
        FIELD(target, u16, 0x17e) = flags;
    }
    Func_08015278(FIELD(state, s32, 0x24));
    FIELD(FIELD(&Data_03001e68, void *, 0x24), u8, 0xea6) = 1;
    Func_080a34c0();
    Func_08015408(0, 0, 30, 20);
    Func_08002dd8(0x37);
    Data_03001e68->suspended = 0;
    WaitFrames(1);
    Func_08015410(0, 0, 30, 20);
    FIELD(FIELD(&Data_03001e68, void *, 0x24), u8, 0xea6) = 0;
    Func_0808a548();
    return result;
}
