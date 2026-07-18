typedef unsigned char u8;

typedef void (*Callback)(void *);

void Func_08004fe4(void *, void *, void *);

void Func_080051e8(void *arg0, void *arg1)
{
    u8 data[48];

    Func_08004fe4(arg0, arg1, data);
    ((Callback)0x030002c0)(data);
}
