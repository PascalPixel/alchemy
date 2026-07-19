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

void Func_080b7e24(void *arg0) {
    s32 temp_r2;
    s32 var_r6;
    void **var_r5;
    void *temp_r0;

    if (arg0 != NULL) {
        temp_r2 = 0xF & M2C_FIELD(arg0, u8 *, 0x54);
        switch (temp_r2) {
        case 1:
            Func_080b7e04(M2C_FIELD(arg0, void ***, 0x50));
            return;
        case 2:
            var_r5 = M2C_FIELD(arg0, void ***, 0x50);
            var_r6 = 3;
            do {
                temp_r0 = *var_r5++;
                if (temp_r0 != NULL) {
                    Func_080b7e04(temp_r0);
                }
                var_r6 -= 1;
            } while (var_r6 >= 0);
            break;
        }
    }
}
