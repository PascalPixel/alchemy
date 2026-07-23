typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_02000030(void *arg0) {
    u32 index;
    register u8 kind;
    register s32 enabled;
    register u16 normal;
    register u16 special;

    index = 0;
    kind = 2;
    enabled = 1;
    normal = 0x69;
    special = 0x6E;
    do {
        M2C_FIELD(arg0, u8, 0x16) = kind;
        M2C_FIELD(arg0, s32, 4) = enabled;
        M2C_FIELD(arg0, u16, 0) = normal;
        if (index == 4 || index == 7) {
            M2C_FIELD(arg0, u16, 0) = special;
        }
        index++;
        arg0 = (u8 *)arg0 + 0x18;
    } while (index <= 0xE);
}
