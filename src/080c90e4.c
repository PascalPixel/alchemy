typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Position {
    u8 unknown[4];
    u16 x;
    u16 y;
};

extern u32 Data_03001eec;
extern struct Position Data_03001ad0;

void Func_080c90e4(void)
{
    u8 *base = (u8 *)Data_03001eec;
    u32 *counter = (u32 *)(base + 0x7790);

    (*counter)++;
    if (*counter == *(u32 *)(base + 0x7794)) {
        Data_03001ad0.x += *(s32 *)(base + 0x7798);
        Data_03001ad0.y += *(s32 *)(base + 0x779C);
        *counter = 0;
    }
}
