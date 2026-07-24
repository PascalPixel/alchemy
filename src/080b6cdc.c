typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern s32 Func_080c23c0(s32);

struct Layout {
    u8 pad[4];
    s16 field[6];
};

s32 Func_080b6cdc(s32 arg0)
{
    struct Layout *ptr;
    s32 retval;
    s32 i;

    retval = Func_080c23c0(arg0);
    ptr = *(struct Layout **)0x03001e74;

    for (i = 0; i <= 5; i++) {
        if (ptr->field[i] != 0)
            continue;
        if (retval != 0)
            break;
        if (i <= 4 && ptr->field[i + 1] == 0)
            break;
    }

    return i != 6;
}
