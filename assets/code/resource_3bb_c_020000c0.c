typedef signed int s32;

extern void Func_02003f5e(s32, s32, s32, s32, s32, s32);
extern s32 *Func_0200402c(s32);
extern void Func_02003ff2(s32, s32);
extern void Func_02003f84(s32, s32, s32, s32, s32, s32);
extern s32 *Func_02004052(s32);
extern void Func_02004018(s32, s32);
extern void Func_02003fa8(s32, s32, s32, s32, s32, s32);
extern s32 *Func_02004076(s32);
extern void Func_0200403c(s32, s32);
extern void Func_02003fcc(s32, s32, s32, s32, s32, s32);

void Func_020000c0(void)
{
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 14;
    s32 sixth = 11;
    s32 *record;
    s32 value;

    Func_02003f5e(100, 11, 12, 4, fifth, sixth);

    record = Func_0200402c(12);
    value = record[2] >> 20;
    Func_02003ff2(832, value);
    Func_02003f84(71, 16, 1, 1, value, 16);

    record = Func_02004052(13);
    value = record[2] >> 20;
    Func_02004018(840, value);
    Func_02003fa8(71, 16, 1, 1, value, 16);

    record = Func_02004076(14);
    value = record[2] >> 20;
    Func_0200403c(848, value);
    Func_02003fcc(71, 16, 1, 1, value, 16);
}
