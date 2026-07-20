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
void *Func_080b7dd0(s32);
void Func_08009038(void *);
void *Func_080b7f70(void *, s32);
void Func_080b7b30(s32 arg0) {
    void *temp_r0; void *obj; s32 i; void *r; s8 *p;
    temp_r0 = Func_080b7dd0(arg0);
    if (temp_r0 != NULL) {
        obj = M2C_FIELD(temp_r0, void **, 0);
        if (obj != NULL) {
            M2C_FIELD(temp_r0, s32 *, 0x20) = 0;
            M2C_FIELD(temp_r0, s32 *, 0x24) = 0;
            i = 0;
            while ((r = Func_080b7f70(obj, i)) != NULL) { Func_08009038(r); i += 1; }
            p = (s8 *) ((u8 *) obj + 0x54);
            *p = (s8) r;
            M2C_FIELD(obj, void **, 0x50) = r;
        }
    }
}
