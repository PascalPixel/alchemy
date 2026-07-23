typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

u8 *Func_08077008(s32);
void Func_08077128(s32);
u8 *Func_08077018(u16);
void Func_08077010(s32);

void Func_080b2da8(s32 id, s32 mode) {
    u8 *state;

    state = Func_08077008(id);
    if (mode == 0) {
        *(u16 *)(state + 56) = *(u16 *)(state + 52);
        Func_08077128(id);
    } else if (mode == 1) {
        state[0x131] = 0;
    } else if (mode == 2) {
        state[320] = 0;
    } else if (mode == 3) {
        volatile u16 *entry;
        s32 i;
        s32 mask;

        mask = 0x200;
        entry = (volatile u16 *)state;
        i = 14;
        entry = (volatile u16 *)((u8 *)entry + 216);
        do {
            if ((*entry & mask) != 0 &&
                (Func_08077018(*entry)[3] & 1) != 0) {
                *entry ^= mask;
                Func_08077010(id);
            }
            i--;
            entry++;
        } while (i >= 0);
    }
}
