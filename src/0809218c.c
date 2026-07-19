typedef unsigned char u8;
typedef signed int s32;

void *Func_0808ba1c(s32);
void Func_08009140(void *);
void Func_08009080(void *, s32);
void Func_08009150(void *, s32, s32, s32);

void Func_0809218c(s32 index, s32 first, s32 second)
{
    u8 *object = Func_0808ba1c(index);

    if (object != 0) {
        object[91] = 0;
        Func_08009140(object);
        Func_08009080(object, 2);
        Func_08009150(object, first << 16, *(s32 *)(object + 12), second << 16);
    }
}
