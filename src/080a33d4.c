typedef unsigned char u8;
typedef signed int s32;

struct State080a33d4 {
    u8 padding[0x48];
    void *entries[32];
};

void *Func_080150d0(void *, void *, void *, void *, void *);

void Func_080a33d4(struct State080a33d4 *state, s32 arg1)
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
        *output0++ = Func_080150d0((void *)2, (void *)index0, (void *)arg1,
            (void *)0xF8, (void *)fifth0);
        index0++;
    } while (index0 <= 7);

    index1 = 8;
    fifth1 = 0xA8;
    output1 = &state->entries[8];
    do {
        *output1++ = Func_080150d0((void *)2, (void *)index1, (void *)arg1,
            (void *)0x100, (void *)fifth1);
        index1++;
    } while (index1 <= 15);

    index2 = 16;
    fifth2 = 0xA8;
    output2 = &state->entries[16];
    do {
        *output2++ = Func_080150d0((void *)2, (void *)index2, (void *)arg1,
            (void *)0x100, (void *)fifth2);
        index2++;
    } while (index2 <= 31);
}
