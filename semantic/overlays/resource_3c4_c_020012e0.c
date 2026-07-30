/*
 * resource_3c4 @ 0x020012e0 (56 bytes: 40 code + four pool words).
 *
 * Selects one of two RAM tables from the scene variant held as a signed
 * halfword at Data_02000240[224] (the same selector the byte-exact neighbours
 * `resource_3c4_c_02001270.c` and `resource_3c4_c_020012b0.c` read), installs
 * it, and returns it.
 *
 * The compared pool word is 0x000000ac; the byte-exact neighbours spell that
 * constant as `(s32)&Value_000000ac`, which is the same value.  It is written
 * as a literal here so this source stands alone.
 *
 * The epilogue is `pop {r5} ; pop {r1} ; bx r1`: r0 survives, so the selected
 * table is the observable result.
 */
typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

extern s16 Data_02000240[];
extern u8 Data_0200b8f4[];
extern u8 Data_0200ba74[];

void Func_02004402(void *table);

void *Func_020012e0(void)
{
    void *table;

    if (Data_02000240[224] == 0xac) {
        table = Data_0200b8f4;
    } else {
        table = Data_0200ba74;
    }
    Func_02004402(table);
    return table;
}
