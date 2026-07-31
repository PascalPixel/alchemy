typedef unsigned char u8;
typedef signed int s32;

extern u8 *Func_02001724(s32);
extern void Func_020016da(s32, s32, s32, s32, s32, s32);
extern void Func_020016f6(u8 *, s32);
extern u8 *Func_0200174c(s32);
extern void Func_02001738(s32);

void Func_020005ec(void)
{
    u8 *record = Func_02001724(19);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 26;
    s32 sixth = 32;

    Func_020016da(26, 30, 1, 1, fifth, sixth);

    if (record != 0) {
        Func_020016f6(record, 0);
        /* The record is reloaded with the same selector before this store. */
        Func_0200174c(19)[85] = 0;
        record[35] = 1;
    }

    Func_02001738(0x203);
}
