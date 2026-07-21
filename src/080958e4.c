typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_08004278(void *);
void Func_0809bb34(void *);
void Func_08002dd8(s32);
void Func_080030f8(s32);

void Func_080958e4(void) {
    u8 *var_r5 = *(u8 **)0x03001F30;
    u8 *var_r6;
    s32 var_r7;

    Func_08004278((void *)0x08095885);
    var_r6 = var_r5 + 157;
    var_r5 += 88;
    for (var_r7 = 23; var_r7 >= 0; var_r7--) {
        u32 temp_r3 = *(volatile u8 *)var_r6;
        var_r6 += 72;
        if ((temp_r3 << 24) != 0) {
            Func_0809bb34(var_r5);
        }
        var_r5 += 72;
    }
    Func_08002dd8(56);
    Func_080030f8(1);
}

void Func_0809592c(void *arg0) {
    *(u16 *)((u8 *)arg0 + 6) += 0x2000;
}
