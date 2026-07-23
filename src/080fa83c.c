typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

void Func_080fa798(u32);
void Func_080fa928(void);

void Func_080fa83c(u32 value)
{
    u8 *state = *(u8 **)0x03007FF0;
    u32 header = *(u32 *)state;
    u32 field;

    if (header != 0x68736D53) {
        return;
    }

    *(u32 *)state = header + 1;

    field = value & 0xFF;
    if (field != 0) {
        state[5] = field & 0x7F;
    }

    field = value & 0xF00;
    if (field != 0) {
        u8 *entry;

        state[6] = field >> 8;
        field = 12;
        entry = state + 0x50;
        do {
            *entry = 0;
            field--;
            entry += 0x40;
        } while (field != 0);
    }

    field = value & 0xF000;
    if (field != 0) {
        state[7] = field >> 12;
    }

    field = value & 0xB00000;
    if (field != 0) {
        field = (field & 0x300000) >> 14;
        *(volatile u8 *)0x04000089 =
            (*(volatile u8 *)0x04000089 & 0x3F) | field;
    }

    field = value & 0xF0000;
    if (field != 0) {
        Func_080fa928();
        Func_080fa798(field);
    }

    *(u32 *)state = 0x68736D53;
}
