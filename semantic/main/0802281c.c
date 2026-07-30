typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef signed int s32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

extern void * volatile Data_03001e74;
extern void Func_08022768(s32, s32, s32, s32, s32);
extern s32 Func_080b5090(s32, s32);

s32 Func_0802281c(u16 *arg0)
{
    s16 var_r2;
    s32 temp_r0;
    s32 var_r0;
    s32 var_r1;
    s32 var_r6;
    u16 var_r3;
    void *temp_r7;

    temp_r7 = Data_03001e74;
    temp_r0 = Func_080b5090(1, 0);
    Func_08022768(0x1D - (temp_r0 * 6), 0, 0x19, 5, 0xF);
    var_r6 = 0;
    if (*arg0 != 0xFF) {
        var_r0 = 0;
loop_2:
        var_r2 = M2C_FIELD(temp_r7, s16 *, 0x58);
        var_r3 = *(u16 *)((u8 *)arg0 + var_r0);
        var_r1 = 0;
loop_5:
        if (var_r2 != var_r3) {
            if (var_r2 == 0xFF) {
                var_r1 = 4;
            } else {
                var_r1 += 1;
                if (var_r1 <= 3) {
                    var_r2 = *(s16 *)(temp_r7 + (var_r1 * 2) + 0x58);
                    var_r3 = *(u16 *)((u8 *)arg0 + var_r0);
                    goto loop_5;
                }
            }
        }
        if (var_r1 != 4) {
            Func_08022768(0x1D - ((temp_r0 - var_r1) * 6), 0, 7, 5, 0xE);
        }
        var_r6 += 1;
        if (var_r6 <= 3) {
            var_r0 = var_r6 * 2;
            if (*(u16 *)((u8 *)arg0 + var_r0) != 0xFF) {
                goto loop_2;
            }
        }
    }
    return 0;
}
