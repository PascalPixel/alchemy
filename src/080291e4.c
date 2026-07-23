typedef unsigned char u8;
typedef signed int s32;

s32 Func_080162d4(s32, s32, s32, s32, s32);
void Func_080292c4(s32, s32);
void Func_0801c0dc(void *, s32 *);
void Func_080030f8(s32);
s32 Func_0802938c(s32, s32 *, s32 *);
void Func_0801c154(void *, s32, s32);
void Func_0801c17c(s32);
void Func_08016418(s32, s32);

s32 Func_080291e4(void)
{
    u8 object[12];
    s32 coordinate[2];
    s32 output;
    s32 selection;
    s32 handle;
    s32 event;

    selection = 8;
    handle = 0;
    coordinate[0] = handle;
    coordinate[1] = handle;
    handle = Func_080162d4(1, 0, 28, 20, 2);
    Func_080292c4(handle, selection);
    Func_0801c0dc(object, &output);
    goto loop_test;
loop_body:
    if (event == 1)
        Func_080292c4(handle, *(volatile s32 *)&selection);
    Func_0801c154(object,
                  coordinate[0] * 8 + 58,
                  coordinate[1] * 8 + 20);
loop_test:
    Func_080030f8(1);
    event = Func_0802938c(handle, &selection, coordinate);
    if (event != -1)
        goto loop_body;
    Func_0801c17c(output);
    Func_08016418(handle, 2);
    return 0;
}
