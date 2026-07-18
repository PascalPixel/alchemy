typedef signed int s32;

s32 Func_08078664(s32, s32);
s32 Func_08078698(void);
void Func_080788c4(s32, s32);

s32 Func_080789dc(s32 value)
{
    s32 index = Func_08078698();

    if (index == -1)
        return 0;
    Func_080788c4(index, Func_08078664(index, value));
    return 0;
}
