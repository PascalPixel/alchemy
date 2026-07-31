typedef unsigned char u8;
typedef signed int s32;

extern void Func_020042ce(void);
/* Three sites of the record accessor, so three names. */
extern s32 *Func_020042f4(s32);
extern s32 *Func_020042ec(s32);
extern s32 *Func_02004366(s32);
/* Five sites of one import. */
extern s32 Func_02001796(s32, s32, s32, s32, s32, s32);
extern s32 Func_020017aa(s32, s32, s32, s32, s32, s32);
extern s32 Func_020017bc(s32, s32, s32, s32, s32, s32);
extern s32 Func_020017ce(s32, s32, s32, s32, s32, s32);
extern s32 Func_020017e0(s32, s32, s32, s32, s32, s32);
extern void Func_02004368(void);

void Func_02001520(void)
{
    s32 x;
    s32 z;

    /* No argument register is written before this branch. */
    Func_020042ce();

    /* Both coordinates are 16.16 fixed point reduced to whole tiles with
     * `asrs #20`, i.e. 16 fractional bits plus a 16-unit tile pitch. */
    x = Func_020042ec(11)[2] >> 20;
    z = Func_020042f4(11)[4] >> 20;

    Func_02001796(2, x, z, 1, 1, 0xff);
    Func_020017aa(2, x + 1, z, 1, 1, 0);
    Func_020017bc(2, x - 1, z, 1, 1, 0);
    Func_020017ce(2, x, z + 1, 1, 1, 0);
    Func_020017e0(2, x, z - 1, 1, 1, 0);

    if (x == 36 && z == 24) {
        u8 *record = (u8 *)Func_02004366(11);

        record[85] = 0;
        *(s32 *)(record + 20) = (s32)0xfffe0000;
        *(s32 *)(record + 12) = (s32)0xfffe0000;
    }

    /* Common exit; no argument registers are set. */
    Func_02004368();
}
