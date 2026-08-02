typedef signed int s32;

/* Complete flag-0x200 scene-cell enable wrapper through alignment. */
extern void Func_080770c8(s32 flag);
extern void Func_080091c0(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 first_value, s32 second_value);

void Func_02000d24(void)
{
    Func_080770c8(0x200);
    Func_080091c0(55, 26, 4, 2, 23, 26);
}
