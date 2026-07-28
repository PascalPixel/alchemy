typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Config_080071a8 {
    u8 pad_00[24];
    s32 size;
    u8 shift;
};

extern struct Config_080071a8 Data_08007c10;

s32 Func_080072f0(s32, u8 *, u32, s32);

u16 Func_080071a8(u32 slot, const u8 *source)
{
    const u8 *input;
    volatile u8 *destination;
    s32 *finish;
    u32 saved_ime;
    u32 last;
    s32 remaining;

    input = source;
    slot = (u16)slot;
    destination = (volatile u8 *)(
        0x0e000000 + (slot << Data_08007c10.shift));
    saved_ime = *(volatile u16 *)0x04000208;
    *(volatile u16 *)0x04000208 = 0;
    *(volatile u8 *)0x0e005555 = 0xaa;
    *(volatile u8 *)0x0e002aaa = 0x55;
    *(volatile u8 *)0x0e005555 = 0xa0;

    remaining = Data_08007c10.size;
    if (remaining != 0) {
        do {
            *destination++ = *input++;
            remaining--;
        } while (remaining != 0);
    }

    destination--;
    input--;
    *(volatile u16 *)0x04000208 = saved_ime;
    finish = (s32 *)0x02004c00;
    last = *input;
    return Func_080072f0(
        1,
        (u8 *)destination,
        last,
        *finish);
}
