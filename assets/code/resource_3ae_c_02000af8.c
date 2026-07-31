typedef signed int s32;

extern s32 *Func_020020a4(s32);
extern s32 *Func_020020b4(s32);
extern void Func_02002032(s32, s32, s32, s32, s32, s32);
extern void Func_02002044(s32, s32, s32, s32, s32, s32);
extern void Func_02002056(s32, s32, s32, s32, s32, s32);
extern void Func_020016bc(s32);
extern void Func_020016c2(s32);

void Func_02000af8(void)
{
    s32 slot14_pos;
    s32 slot15_pos;

    slot14_pos = Func_020020a4(14)[2] >> 20;   /* [r0,#8], asrs #20 */
    slot15_pos = Func_020020b4(15)[2] >> 20;

    Func_02002032(5, 12, 5, 1, 5, 11);
    Func_02002044(1, 0, 1, 1, slot15_pos, 11);
    Func_02002056(1, 0, 1, 1, slot14_pos, 11);

    Func_020016bc(14);
    Func_020016c2(15);
}
