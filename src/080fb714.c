typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u8 *Func_080fb714(s32 arg0, void *arg1) {
    u32 cursor;

    cursor = (u32)M2C_FIELD(arg1, u8 **, 0x40);
    cursor = *(u8 *)cursor;
    M2C_FIELD(arg1, u8 *, 0x2D) = cursor;
    cursor = (u32)M2C_FIELD(arg1, u8 **, 0x40);
    cursor++;
    M2C_FIELD(arg1, u8 **, 0x40) = (u8 *)cursor;
    return (u8 *)cursor;
}
