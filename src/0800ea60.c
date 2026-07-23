typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 *Data_03001ebc;
s32 Func_080770c0(s32);

static __inline__ void StoreHalfword(u8 *address, s32 value)
{
    *(s16 *)address = value;
}

u32 Func_0800ea60(u32 value)
{
    u32 index = value >> 14;
    u32 result = 0x3FFF & value;
    u8 *state = Data_03001ebc;

    if (Func_080770c0(0x107) != 0) {
        StoreHalfword(state + 0x182, 0xFA);
    } else if (*(s16 *)(state + 0x19E) == 3) {
        if (*(volatile u32 *)0x03001C94 & 0x100) {
            StoreHalfword(state + 0x182, 0xFC88);
        } else if (*(volatile u32 *)0x03001C94 & 0x200) {
            StoreHalfword(state + 0x182, 0xFC87);
        }
    } else {
        switch (index) {
        case 0:
            StoreHalfword(state + 0x17E, result);
            break;
        case 1:
            StoreHalfword(state + 0x180, result);
            break;
        }
    }

    return result;
}
