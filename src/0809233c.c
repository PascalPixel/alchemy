typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

u8 *Func_0808ba1c(u32);
void Func_08092064(s32, s32, s32);
u8 *Func_08092054(s32);
void Func_080923e4(u32, s32, s32);
void Func_08009080(void *, s32);
void Func_0809228c(s32, s32, s32);
void Func_08009098(void *, void *);

extern s32 Data_02000240[];
extern u8 Data_0809fbcc;

void Func_0809233c(u32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *object = Func_0808ba1c(arg0);

    if (object != 0) {
        u8 *other;

        Func_08092064(arg0, 0x9999, 0x4CCC);
        other = Func_08092054(Data_02000240[125]);
        if (other != 0)
            Func_080923e4(arg0, *(s32 *)(other + 8),
                          *(s32 *)(other + 16));
        object[91] = 0;
        Func_08009080(object, 2);
        Func_0809228c(arg0, arg1, arg2);
        Func_08009098(object, &Data_0809fbcc);
        *(s16 *)(object + 100) = arg3;
    }
}
