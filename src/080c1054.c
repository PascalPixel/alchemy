typedef signed short s16;
typedef signed int s32;

s32 Func_080b6c08(s32, s16 *);
void Func_080c0f98(s32, s32);

void Func_080c1054(void)
{
    s16 values[14];
    s32 count = Func_080b6c08(3, values);
    s32 index;

    for (index = 0; index < count; index++)
        Func_080c0f98(values[index], 0);
}
