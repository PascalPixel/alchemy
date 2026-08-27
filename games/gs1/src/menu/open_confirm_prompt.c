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
 * The allocated block shares OpenCharacterSelector's heap "kind" (55) and
 * size (0xa70), and the fields touched here at +0x24, +0x10c, and +0x219
 * line up with that sibling's screen_handle, selector_window, and
 * character_count -- this owner and OpenCharacterSelector (main:080a7380)
 * evidently share the same allocated state layout.  +0x178 is read here but
 * has no established name from other evidence.
 *
 * Data_03001e68 is shared with OpenCharacterSelector too: its "suspended"
 * field is toggled around the interaction, and the raw address of the
 * variable (not its pointed-to value) is reused as a base for two further
 * fixed-address fields at +0x24 and +0x54, matching the raw-address-table
 * idiom already established in field_owner_135.c for this same region.
 */
s32 Func_080a5b94(void)
{
    void *state = (void *)Runtime_AllocateHeapBlock(0x37, 0xa70);
    s32 spC;
    s32 sp8;
    s32 sp4;
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
    result = Func_080a5cc0(&spC, &sp8, &sp4);
    Menu_EnsureCancelSound();
    if (result == 1) {
        void *target = FIELD(&Data_03001e68, void *, 0x54);
        u16 flags;
        Ability_GetData(0x3fff & FIELD(state, u16, 0x178));
        flags = (u16)(sp4 | (spC << 10));
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
