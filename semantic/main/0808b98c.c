typedef unsigned char u8;
typedef signed int s32;

void Func_080090d0(void);
s32 Func_0808b824(void);
void Func_0808b3ec(s32, s32);

void Func_0808b98c(void)
{
    u8 *state = *(u8 **)0x03001ebc;
    s32 offset;
    s32 remaining;
    s32 mask;
    void *empty;
    void *entry;
    u8 *child;
    s32 pending;

    empty = 0;
    mask = -2;
    offset = 52;
    remaining = 57;
    do {
        entry = *(void **)(state + offset);
        if (entry != 0) {
            u8 active = 1;

            *((u8 *)entry + 84) = active;
            child = *(u8 **)((u8 *)entry + 80);
            child[29] &= mask;
            Func_080090d0();
            *(void **)(state + offset) = empty;
        }
        remaining--;
        offset += 4;
    } while (remaining >= 0);
    pending = *(s32 *)(state + 4);
    *(s32 *)(state + 4) = 0;
    *(s32 *)(state + 8) = 0;
    *(s32 *)(state + 12) = 0;
    if (pending != 0) {
        Func_0808b3ec(pending, Func_0808b824());
    }
}
