typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

typedef void (*Callback)(void *, s32, s32);
extern Callback Data_080136e0[];

s32 Func_0800e9dc(u8 *arg0)
{
    s16 index = *(s16 *)(arg0 + 4);
    u8 *entry = (u8 *)(*(s32 *)arg0 + index * 4 + 4);
    Callback callback = Data_080136e0[*(s32 *)entry];

    if (callback != 0)
        callback(arg0, 1, *(s32 *)(entry + 4));
    *(u16 *)(arg0 + 4) = *(u16 *)(arg0 + 4) + 3;
    return 1;
}
