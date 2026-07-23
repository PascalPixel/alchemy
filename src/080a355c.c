typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

void Func_08015278(s32);
void Func_080a1ac0(s32, s32);
s32 Func_08077008(s32);
s8 Func_080a3ddc(s32, void *, s32);
s32 Func_080a35f8(void *, void *);
void Func_080a17c4(s32);
void Func_080030f8(s32);

extern void *Data_03001f2c;

s32 Func_080a355c(s32 arg0)
{
    register void *base = Data_03001f2c;
    register s32 offset = arg0 + 28;
    s32 result = 0;
    register s32 value;
    u8 *byteField;

    value = *(s8 *)(base + offset);
    Func_08015278(*(s32 *)(base + 44));
    byteField = base + 2;
    byteField[offset] = *(u8 *)(base + 0x219);
    if (value == -1) {
        *(u8 *)(base + offset) = result;
        offset = 0;
    } else {
        offset = value * 2;
        Func_080a1ac0(value * 24 - 10, 16);
    }

    {
        s32 itemOffset = offset + 0x208;
        result = Func_08077008(*(u16 *)(base + itemOffset));
    }
    offset = (s32)(base + 0x1C8);
    *(s8 *)(base + 0x218) =
        Func_080a3ddc(result, (void *)offset, 0);
    result = Func_080a35f8(base + 0x208, (void *)offset);
    {
        s32 argOffset = arg0 * 4 + 20;
        Func_080a17c4(*(s32 *)(base + argOffset));
    }
    Func_080030f8(1);
    return result;
}
