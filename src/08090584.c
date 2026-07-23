typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_08090584(void) {
    u32 line;
    u8 *state;
    u16 value;

    line = *(volatile u16 *)0x04000006;
    state = *(u8 **)0x03001E70;

again:
    switch (FIELD(state, u16, 0x108)) {
    case 3:
        if (line >= FIELD(state, u16, 0x104)) {
            value = *(volatile u16 *)0x04000000;
            *(volatile u16 *)0x04000000 = (value & 0xFFF8) | 2;
            value = 9;
            FIELD(state, u16, 0x108) = value;
        }
        break;
    case 2:
        if (line >= FIELD(state, u16, 0x106)) {
            value = *(volatile u16 *)0x04000000;
            *(volatile u16 *)0x04000000 = value & 0xFFF8;
            value = 9;
            FIELD(state, u16, 0x108) = value;
        }
        break;
    case 1:
        if (line >= FIELD(state, u16, 0x104)) {
            value = *(volatile u16 *)0x04000000;
            *(volatile u16 *)0x04000000 = (value & 0xFFF8) | 2;
            FIELD(state, u16, 0x108)++;
            goto again;
        }
        if (line >= FIELD(state, u16, 0x106)) {
            value = *(volatile u16 *)0x04000000;
            *(volatile u16 *)0x04000000 = value & 0xFFF8;
            value = 3;
            FIELD(state, u16, 0x108) = value;
            goto again;
        }
        break;
    case 0:
        if (line <= 158) {
            value = 1;
            FIELD(state, u16, 0x108) = value;
            goto again;
        }
        break;
    }
}
