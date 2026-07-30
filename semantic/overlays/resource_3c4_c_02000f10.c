/*
 * resource_3c4 @ 0x02000f10 (60 bytes).
 *
 * Orders two records by their +16 word: when the first is not already above
 * the second, the two records exchange their +8, +12 and +16 words and a
 * refresh is requested.  The comparison is `bgt`, so it is signed.
 * `pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;

typedef struct Record_02000f10 {
    s32 pad0;
    s32 pad4;
    s32 w8;                /* +8  */
    s32 w12;               /* +12 */
    s32 w16;               /* +16, ordering key */
} Record_02000f10;

Record_02000f10 *Func_02004022(s32 first);
Record_02000f10 *Func_0200402a(s32 second);
void Func_02003f68(s32 flag);

void Func_02000f10(s32 first, s32 second)
{
    Record_02000f10 *a = Func_02004022(first);
    Record_02000f10 *b = Func_0200402a(second);

    if (a->w16 <= b->w16) {
        s32 t;

        t = b->w8;  b->w8  = a->w8;  a->w8  = t;
        t = b->w12; b->w12 = a->w12; a->w12 = t;
        t = b->w16; b->w16 = a->w16; a->w16 = t;
        Func_02003f68(1);
    }
}
