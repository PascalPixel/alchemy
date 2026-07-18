typedef unsigned short u16;

typedef u16 (*Callback)(u16);
extern Callback Data_02004c14;

u16 Func_08005904(u16 value)
{
    return Data_02004c14(value);
}
