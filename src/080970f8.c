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
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
void *Func_0808ba1c(u32);
void Func_0800447c(s32, s32, void *);
void Func_080970f8(s32 arg0, s32 arg1) {
    void *temp_r0; void *temp_r0_2; s32 temp_r1; void *temp_r6;
    temp_r6 = *(void **)0x03001F30;
    M2C_FIELD(temp_r6, s16, 0x18) = arg0;
    temp_r0 = Func_0808ba1c((s16) arg0);
    M2C_FIELD(temp_r6, s16, 0x1A) = arg1;
    M2C_FIELD(temp_r6, s32 *, 0x10) = (s32) temp_r0;
    temp_r0_2 = Func_0808ba1c((s16) arg1);
    temp_r1 = (M2C_FIELD(temp_r0, u16, 6) + 0x2000) & 0xC000;
    M2C_FIELD(temp_r6, s32 *, 0x14) = (s32) temp_r0_2;
    M2C_FIELD(temp_r6, s32 *, 0) = temp_r1;
    if (temp_r0_2 != 0) {
        M2C_FIELD(temp_r6, s32 *, 0x38) = (s32) M2C_FIELD(temp_r0_2, s32 *, 0x6C);
        M2C_FIELD(temp_r6, s32 *, 0x3C) = (s32) M2C_FIELD(temp_r0_2, s32 *, 0);
        {
            u8 tmp = (u8) M2C_FIELD(M2C_FIELD(M2C_FIELD(temp_r0_2, void **, 0x50), void **, 0x28), u8, 5);
            M2C_FIELD(temp_r6, u8, 0x44) = tmp;
        }
        M2C_FIELD(temp_r6, s32 *, 4) = (s32) M2C_FIELD(temp_r0_2, s32 *, 8);
        M2C_FIELD(temp_r6, s32 *, 0xC) = (s32) M2C_FIELD(temp_r0_2, s32 *, 0x10);
        M2C_FIELD(temp_r6, s32 *, 8) = (s32) M2C_FIELD(temp_r0_2, s32 *, 0xC);
        return;
    }
    M2C_FIELD(temp_r6, s32 *, 4) = (s32) M2C_FIELD(temp_r0, s32 *, 8);
    M2C_FIELD(temp_r6, s32 *, 0xC) = (s32) M2C_FIELD(temp_r0, s32 *, 0x10);
    M2C_FIELD(temp_r6, s32 *, 8) = (s32) M2C_FIELD(temp_r0, s32 *, 0xC);
    Func_0800447c(0x100000, temp_r1, (u8 *)temp_r6 + 4);
}
