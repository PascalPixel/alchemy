#include "TYPES.H"

/* ⚓️ keeps the icon entries four bytes later than ☀️'s State080a8088. */
struct IconMenuState {
    u8 padding_000[0x24];
    s32 handle;
    u8 padding_028[0x24];
    void *entries[32];
};

void *RenderOutput_CreateFromResourceFar(s32 group, s32 index, s32 window, s32 x, s32 y);

void Menu_SpawnIconEntries(struct IconMenuState *state, s32 arg1)
{
    void **output0;
    s32 index0;
    s32 fifth0;
    void **output1;
    s32 index1;
    s32 fifth1;
    void **output2;
    s32 index2;
    s32 fifth2;

    index0 = 0;
    fifth0 = 0xA8;
    output0 = &state->entries[0];
    do {
        *output0++ = RenderOutput_CreateFromResourceFar(2, index0, arg1, 0xF8, fifth0);
        index0++;
    } while (index0 <= 7);
    index1 = 8;
    fifth1 = 0xA8;
    output1 = &state->entries[8];
    do {
        *output1++ = RenderOutput_CreateFromResourceFar(2, index1, arg1, 0x100, fifth1);
        index1++;
    } while (index1 <= 15);
    index2 = 16;
    fifth2 = 0xA8;
    output2 = &state->entries[16];
    do {
        *output2++ = RenderOutput_CreateFromResourceFar(2, index2, arg1, 0x100, fifth2);
        index2++;
    } while (index2 <= 31);
}
