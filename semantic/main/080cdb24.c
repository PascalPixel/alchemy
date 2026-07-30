typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned char u8;

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_080cd594(void);

void Func_080cdb24(s32 arg0) {
    s32 var_ip;
    s32 var_r0;
    s32 var_r1;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r6;
    s32 var_r7;
    void *var_r5;

    Func_080cd594();
    *(s16 *)0x0400000C = arg0 | 0x6784;
    var_r5 = (void *)0;
    var_ip = 0;
    var_r7 = 0;
    var_r6 = 0;
    do {
        var_r4 = 0;
        var_r0 = var_r6 + 0x100;
        var_r1 = var_r7 * 2;
loop_2:
        var_r4++;
        FIELD(var_r5, s16, 0x06003800) =
            (s16)(var_r0 | var_r1);
        var_r0 += 0x200;
        var_r1 += 2;
        var_r5 = (u8 *)var_r5 + 2;
        if (var_r4 != 8)
            goto loop_2;
        var_r4_2 = 0;
loop_4:
        var_r4_2++;
        FIELD(var_r5, s16, 0x06003800) = 0;
        var_r5 = (u8 *)var_r5 + 2;
        if (var_r4_2 != 8)
            goto loop_4;
        var_r6 += 0x1000;
        var_ip++;
        var_r7 += 8;
    } while (var_ip != 0x10);
}
