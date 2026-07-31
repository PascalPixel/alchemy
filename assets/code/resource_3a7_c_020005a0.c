typedef unsigned char u8;
typedef signed int s32;

extern u8 *Func_020016d8(s32);
extern void Func_0200168e(s32, s32, s32, s32, s32, s32);
extern u8 *Func_020016f8(s32);
extern void Func_020016e4(s32);

void Func_020005a0(void)
{
    u8 *record = Func_020016d8(18);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 24;
    s32 sixth = 34;

    Func_0200168e(26, 30, 1, 1, fifth, sixth);

    if (record != 0) {
        /* The record is reloaded with the same selector before this store. */
        Func_020016f8(18)[85] = 0;
        record[35] = 1;
    }

    Func_020016e4(0x202);
}
