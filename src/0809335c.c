typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080030f8(s32);
void *Func_080048f4(s32, s32);
s32 Func_080090e0(void *, void *);
s32 Func_08009128();
void *Func_0808ba1c();

void Func_0809335c(s32 arg0, s32 arg1) {
    register s32 *global asm("r3");
    void *temp_r5;
    void *temp_r6;
    void *temp_r8;

    temp_r6 = Func_0808ba1c();
    temp_r8 = Func_080048f4(0x1B, 0xCCC);
    temp_r5 = M2C_FIELD(temp_r8, void **, 0x1E0);
    global = *(s32 **)0x03001E70;
    asm volatile("" : "+r" (global));
    if (temp_r6 != NULL) {
        *global = (s32) (temp_r5 + 8);
        Func_080090e0(temp_r5, temp_r6);
        if (arg1 == 0) {
            M2C_FIELD(temp_r5, s32 *, 8) = (s32) M2C_FIELD(temp_r6, s32 *, 8);
            M2C_FIELD(temp_r5, s32 *, 0xC) = (s32) M2C_FIELD(temp_r6, s32 *, 0xC);
            M2C_FIELD(temp_r5, s32 *, 0x10) = (s32) M2C_FIELD(temp_r6, s32 *, 0x10);
            Func_080030f8(1);
            if (M2C_FIELD(temp_r8, s16 *, 0x19E) != 3) {
                Func_08009128();
            }
        }
    }
}
