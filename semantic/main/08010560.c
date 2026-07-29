typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_08010424(u16, u16, u32, u32, u32, u32);
void Func_080030f8(u32);

void Func_08010560(u8 *entry, s32 arg1, s32 arg2) {
    s16 *values;
    u32 id;
    u16 delay;

    id = *(u16 *) entry;
    if (id != 0xFFFF) {
        values = (s16 *)(entry + 2);
        do {
            delay = (u16) values[3];
            Func_08010424(id, (u16) values[0], arg1, arg2,
                          (u16) values[1], (u16) values[2]);
            entry += 10;
            Func_080030f8(delay);
            id = *(u16 *) entry;
            values += 5;
        } while (id != 0xFFFF);
    }
}
