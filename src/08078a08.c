typedef signed int s32;

s32 Func_08078664(s32, s32);
s32 Func_08078698(void);
void Func_08078948(s32, s32);

s32 Func_08078a08(s32 value)
{
    s32 index = Func_08078698();

    if (index == -1)
        return 0;
    Func_08078948(index, Func_08078664(index, value));
    return 0;
}
