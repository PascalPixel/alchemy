typedef signed int s32;
typedef unsigned int u32;

s32 Func_0800231c(s32);
s32 Func_08002322(s32);
typedef s32 (*Callback08005208)(s32, s32);

struct Result08005208 {
    s32 field0;
    s32 field4;
    s32 field8;
};

void Func_08005208(u32 arg0, s32 arg1, s32 arg2)
{
    s32 half;
    s32 first;
    s32 result;

    half = (s32)(arg0 + (arg0 >> 31)) >> 1;
    first = Func_08002322(half);
    result = ((Callback08005208)0x0300013C)(
        first,
        Func_0800231c(half) * 0x50
    );
    ((struct Result08005208 *)0x03001CE0)->field4 = arg1;
    ((struct Result08005208 *)0x03001CE0)->field0 = result;
    ((struct Result08005208 *)0x03001CE0)->field8 = arg2;
}
