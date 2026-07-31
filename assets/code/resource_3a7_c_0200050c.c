typedef unsigned char u8;
typedef signed int s32;

extern u8 *Func_02001644(s32);
extern void Func_020015fa(s32, s32, s32, s32, s32, s32);
extern u8 *Func_02001664(s32);
extern void Func_02001652(s32);

void Func_0200050c(void)
{
    u8 *record = Func_02001644(16);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 23;
    s32 sixth = 32;

    Func_020015fa(26, 30, 1, 1, fifth, sixth);

    if (record != 0) {
        /* The record is reloaded with the same selector before this store. */
        Func_02001664(16)[85] = 0;
        record[35] = 1;
    }

    Func_02001652(0x200);
}
