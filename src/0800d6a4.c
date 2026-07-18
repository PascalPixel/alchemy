typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

typedef s32 (*Callback)(void *);

s32 Func_0800d6a4(u8 *arg0)
{
    s32 offset = 4;
    s16 initial = *(s16 *)(arg0 + offset);
    s32 table = *(s32 *)arg0;
    Callback callback = *(Callback *)(table + initial * 4 + 4);

    if (callback(arg0) != 0)
        return 0;
    if (*(s16 *)(arg0 + 4) == initial)
        *(s16 *)(arg0 + 4) = *(u16 *)(arg0 + 4) + 2;
    return 1;
}
