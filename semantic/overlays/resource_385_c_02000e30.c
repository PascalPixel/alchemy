typedef signed int s32;

/* Complete paired scene-cell presentation update. */
extern void Func_080091c0(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 first_value, s32 second_value);
extern void Func_08009180(s32 kind, s32 first_x, s32 second_x, s32 value,
                          s32 first_mode, s32 second_mode);

void Func_02000e30(void)
{
    Func_080091c0(41, 43, 1, 1, 13, 25);
    Func_08009180(40, 42, 12, 22, 3, 3);
}
