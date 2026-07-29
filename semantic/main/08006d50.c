typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

struct FlashConfig {
    u8 padding0[8];
    u8 shift;
    u8 padding9[7];
    u16 wait;
};

extern struct FlashConfig *Data_02004c08;
extern u32 Data_02004c00;
extern void Func_08006ac0(void *);
extern u32 Func_080072f0(u32, u32, u32, u32);

u16 Func_08006d50(u16 index)
{
    u8 buffer[64];
    u32 mask;
    volatile u16 *waitcnt;
    u32 address;
    u16 result;
    u16 value;
    u32 command;
    struct FlashConfig *config;

    value = index;
    if (value <= 15) {
        waitcnt = (volatile u16 *)0x04000204;
        mask = 0xfffc;
        config = Data_02004c08;
        *waitcnt = config->wait | (*waitcnt & mask);
        address = 0x0e000000 + (value << config->shift);
        command = 0xaa;

        *(volatile u8 *)0x0e005555 = command;
        *(volatile u8 *)0x0e002aaa = 0x55;
        *(volatile u8 *)0x0e005555 = 0x80;
        *(volatile u8 *)0x0e005555 = command;
        *(volatile u8 *)0x0e002aaa = 0x55;
        *(volatile u8 *)address = 0x30;

        Func_08006ac0(buffer);
        result = Func_080072f0(2, address, 0xff, Data_02004c00);
        *waitcnt = (*waitcnt & mask) | 3;
        return result;
    }
    return 0x80ff;
}
