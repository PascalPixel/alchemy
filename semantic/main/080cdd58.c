typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Globals {
    u8 *base;
    u8 pad04[0x78];
    void *buffer;
    u8 pad80[0x0c];
    void *target;
};

struct TransferState {
    u8 pad00[6];
    u16 control;
};

extern struct Globals Data_03001e74;

void Func_080cd508(u32);
void Func_080b5038(s32, u16, s32);
void Func_080072f8(void *, s32);
void Func_080030f8(u32);

void Func_080cdd58(u32 arg0)
{
    void *buffer = Data_03001e74.buffer;
    u8 *base = Data_03001e74.base;
    void *target = Data_03001e74.target;
    void (*transfer)();
    void *offset;
    s32 tile;
    s32 y;

    Func_080cd508(arg0);
    Func_080b5038(2, *(u16 *)(base + 0x648), 0);
    ((struct TransferState *)0x03001ad0)->control = 0x20;
    *(s32 *)((u8 *)target + 0x0c) = 0;
    Func_080072f0(0x06003fc0, 0x40, 0x03001ad0, 0x03000164);
    Func_080072f0(0x0600f900, 0x200, -1, 0x03000168);

    offset = 0;
    y = 0;
    tile = 0;
    do {
        s32 x = 0;
        s32 value = tile + 0x100;
        do {
            if (x > 15)
                *(volatile u16 *)((u8 *)offset + 0x0600fb00) = 0xff;
            else
                *(volatile u16 *)((u8 *)offset + 0x0600fb00) = value;
            x++;
            value++;
            offset = (u8 *)offset + 2;
        } while (x != 32);
        y++;
        tile += 16;
    } while (y != 16);

    transfer = (void (*)())0x03000164;
    *(volatile u16 *)0x04000000 = 0x7741;
    *(volatile u16 *)0x0400000a = 0x1f81;
    *(volatile u16 *)0x04000050 = 0x3f42;
    *(volatile u16 *)0x04000040 = 0x00f0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000042 = 0x00f0;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;
    *(volatile u16 *)0x04000052 = 0x100e;
    transfer(buffer, 0x4000);
    transfer((void *)0x06004000, 0x4000);
    Func_080030f8(1);
}
