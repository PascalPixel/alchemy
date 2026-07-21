typedef signed char s8;
typedef signed int s32;
typedef unsigned char u8;

void *Func_0808ba1c(void);
void Func_08009240(void *, s32);
void Func_08009088(void *, s32);
void Func_0809ad71(void);
extern u8 Data_02000240[];

void Func_0809ade8(void)
{
    u8 *object = Func_0808ba1c();
    if (object != 0) {
        if (*(void (**)(void))(object + 0x6C) == Func_0809ad71) {
            u8 *state = Data_02000240;
            *(s32 *)(object + 0x6C) = *(s32 *)(state + 0x250);
            *(s32 *)(state + 0x250) = 0;
            Func_08009240(object, *(s8 *)(state + 0x249));
        }
        object[0x5B] = 0;
        Func_08009088(object, 16);
    }
}
