typedef unsigned char u8;
typedef signed int s32;
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u8 *Func_080fb750(s32 arg0, void *arg1) {
    u8 *cursor;
    u8 value;

    cursor = M2C_FIELD(arg1, u8 **, 0x40);
    value = *cursor;
    M2C_FIELD(arg1, u8 *, 0x1E) = value;
    cursor++;
    M2C_FIELD(arg1, u8 **, 0x40) = cursor;
    return cursor;
}
