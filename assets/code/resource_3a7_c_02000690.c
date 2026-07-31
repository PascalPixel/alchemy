typedef unsigned char u8;
typedef signed int s32;

extern u8 *Func_020017c8(s32);
extern void Func_0200177e(s32, s32, s32, s32, s32, s32);
extern void Func_0200179a(u8 *, s32);
extern u8 *Func_020017f0(s32);
extern void Func_020017dc(s32);

void Func_02000690(void)
{
    u8 *record = Func_020017c8(21);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 28;
    s32 sixth = 33;

    Func_0200177e(26, 30, 1, 1, fifth, sixth);

    if (record != 0) {
        Func_0200179a(record, 0);
        /* The record is reloaded with the same selector before this store. */
        Func_020017f0(21)[85] = 0;
        record[35] = 1;
    }

    Func_020017dc(0x205);
}
