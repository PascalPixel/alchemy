typedef unsigned char u8;
typedef signed int s32;

extern u8 *Func_02003ecc(s32);
extern void Func_02003e7a(s32, s32, s32, s32, s32, s32);
extern u8 *Func_02003eec(s32);
extern void Func_02003ed2(s32);

void Func_02001984(void)
{
    u8 *slot = Func_02003ecc(13);
    s32 fifth = 40;
    s32 sixth = 55;

    Func_02003e7a(40, 54, 1, 1, fifth, sixth);
    if (slot != 0) {
        u8 *other = Func_02003eec(13) + 85;
        u8 *flags = slot + 35;

        *other = 0;
        *flags = 2;
    }
    Func_02003ed2(512);
}
