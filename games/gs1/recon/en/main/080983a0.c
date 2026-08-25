#include "types.h"

extern u8 *Data_03001ebc;
extern u8 Data_02000240[];

void Func_08098294(s32 mode);
void Func_0808fe38(s32 mode);
void *Func_0808ba1c(void *entry);
void Func_08091220(s32 value, s32 mode);
void Func_08091200(s32 value, s32 mode);
void Func_08091254(s32 mode);
void Func_080030f8(s32 frames);
s32 Func_0808e4b4(s32 value, s32 count, s32 *result);
void Func_08096b28(s32 handle, void *entry, s32 result);
void Func_080f9010(s32 cue);
void Func_0808f32c(void);
void Func_080041d8(const void *callback, s32 delay);
void Func_080982dc(void);

#define RunBattleEffect08 Func_080983a0

void RunBattleEffect08(void)
{
    u8 **state_slot = &Data_03001ebc;
    u8 *state = *state_slot;
    u8 *scene;
    void **entry_slot;
    u8 *object;
    s32 result;
    s32 value;
    s16 frame;

    Func_08098294(6);
    Func_0808fe38(8);
    scene = state_slot[4];
    entry_slot = (void **)(Data_02000240 + 500);
    object = Func_0808ba1c(*entry_slot);
    *(s32 *)(scene + 0x52c) = *(s32 *)(object + 8);
    *(s32 *)(scene + 0x530) = *(s32 *)(object + 16) - *(s32 *)(object + 12);
    Func_08091220(0x10000, 0);
    Func_08091200(0x10001, 1);
    Func_08091254(1);
    Func_080030f8(1);
    value = Func_0808e4b4(0x50000005, 8, &result);
    if (value != 0) {
        Func_08096b28(value, *entry_slot, result);
    }
    Func_080f9010(0x83);
    *(s16 *)(state + 0xcb8) = 1;
    value = *(s32 *)(scene + 0x52c);
    if (value < 0) {
        value += 0xffff;
    }
    *(s16 *)(state + 0xcbc) = value >> 16;
    value = *(s32 *)(scene + 0x530);
    if (value < 0) {
        value += 0xffff;
    }
    *(s16 *)(state + 0xcbe) = value >> 16;
    *(s16 *)(state + 0xcba) = 0x258;
    *(s16 *)(state + 0xcc0) = 1;
    Func_0808f32c();
    frame = 0;
    do {
        Func_080030f8(1);
        *(s16 *)(scene + 0x52a) = frame;
        frame++;
    } while (frame <= 18);
    Func_080041d8(Func_080982dc, 0xc80);
}
