typedef unsigned char u8;
typedef signed int s32;

/* Four sites of the record accessor, so four names. */
extern u8 *Func_02004890(s32);
extern u8 *Func_020048a0(s32);
extern u8 *Func_020048b0(s32);
extern u8 *Func_020048ba(s32);
extern s32 Func_020027d8(s32 *);
extern void Func_020048b4(void);
/* 0x02004918 serves two imports: the two-argument mode select and the
 * zero-argument bracket close. */
extern void Func_02004918(s32, s32);
extern void Func_02004918_b(void);
extern void Func_0200481c(s32);
extern void Func_0200482e(s32);
extern void Func_02004844(s32);

void Func_02001ac8(void)
{
    u8 *record;
    u8 *mode;
    u8 saved;
    s32 target[3];

    record = Func_02004890(0);
    mode = record + 85;
    saved = *mode;

    target[0] = *(s32 *)(Func_020048a0(0) + 8) + (s32)0xffe00000;
    target[1] = *(s32 *)(Func_020048b0(0) + 12);
    target[2] = *(s32 *)(Func_020048ba(0) + 16);

    if (Func_020027d8(target) != 0) {
        /* r0 still holds the nonzero result of the test above. */
        Func_020048b4();

        *mode = 0;
        Func_02004918(11, 7);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        Func_0200481c(2);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        Func_0200482e(10);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;
        Func_02004844(4);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;

        *mode = saved;
        Func_02004918_b();
    }
}
