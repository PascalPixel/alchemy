typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08015278(s32);
void Func_08077010(u8);
void *Func_08077080(s32);
void Func_080a112c(s32, u8, s32, s32);
s32 Func_080a602c(s32);
s32 Func_080a63e4(s32);
s32 Func_080a6ccc(s32);
s32 Func_080a9f10(u16, u8, u8, s32);

/* Runs the multi-stage item/effect selection and confirmation workflow. */
s32 Func_080a5cc0(s32 *arg0, s32 *arg2) {
    s32 sp0;
    s32 *sp4;
    s32 *sp8;
    s16 *var_r1_2;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 var_fp;
    s32 var_r1;
    s32 var_sl;
    u32 var_r5;
    u8 temp_r3;
    u8 temp_r3_2;
    u8 *temp_r7;

    var_r5 = 0;
    sp8 = arg0;
    sp4 = arg2;
    sp0 = 0;
    temp_r7 = *(void **)0x03001F2C;
    var_fp = 0;
loop_49:
    if ((sp0 == 0) && (Func_080770c0(0x150) == 0)) {
        switch (var_r5) {
        case 0:
            M2C_FIELD(temp_r7, s16 *, 0x174) = 0;
            Func_080a3cf8(0, 0xAE9);
            if (Func_080a602c(0) == -1) {
                sp0 = 1;
                var_fp = -1;
            }
            Func_08015270(M2C_FIELD(temp_r7, s32 *, 0x2C));
            var_r5 = 1;
            break;
        case 1:
            Func_080030f8(1U);
            Func_08077008((s32) M2C_FIELD(temp_r7, u8 *, 0x21A));
            var_r5 = 0;
            if (M2C_FIELD(temp_r7, u8 *, 0x218) == 0) {

            } else {
                temp_r3 = M2C_FIELD(temp_r7, u8 *, 0x268);
                switch (temp_r3) {
                case 0:
                    var_r1 = 0xAEA;
block_17:
                    Func_080a3cf8(0, var_r1);
                    break;
                case 1:
                    var_r1 = 0xAF1;
                    goto block_17;
                case 2:
                    Func_080a3cf8(0, 0xAF0);
                    break;
                }
                Func_080a9cbc();
                Func_080a112c(M2C_FIELD(temp_r7, s32 *, 0x24), M2C_FIELD(temp_r7, u8 *, 0x21A), 0, 0);
                temp_r0 = Func_080a6ccc(0);
                var_r5 = 0;
                if (temp_r0 == -1) {

                } else {
                    temp_r3_2 = M2C_FIELD(temp_r7, u8 *, 0x268);
                    var_r5 = 2;
                    if (temp_r3_2 == 0) {

                    } else {
                        if (temp_r3_2 == 1) {
                            Func_080a65e4((s32) M2C_FIELD(temp_r7, u8 *, 0x21A), temp_r0, 0);
                            Func_08015278(M2C_FIELD(temp_r7, s32 *, 0x2C));
                            Func_080a1d08(0xAE2, -1, -1);
                        } else {
                            Func_080a65e4((s32) M2C_FIELD(temp_r7, u8 *, 0x21A), temp_r0, 1);
                            Func_08015278(M2C_FIELD(temp_r7, s32 *, 0x2C));
                            Func_080a1d08(0xAE3, -1, -1);
                        }
                        var_r5 = 0;
                    }
                }
            }
            break;
        case 3:
            Func_080a3cf8(0, 0xAEB);
            var_r5 = 4;
            if (Func_080a63e4(0) != -1) {

            } else {
                var_r1_2 = (s16 *)(temp_r7 + 0x220);
block_44:
                *var_r1_2 = 1 | M2C_FIELD(temp_r7, u16 *, 0x220);
                var_r5 = 1;
            }
            break;
        case 2:
            temp_r0_2 = Func_080a5fe0();
            if (temp_r0_2 == 1) {
block_31:
                var_r5 = 3;
            } else if (temp_r0_2 == 2) {
                M2C_FIELD(temp_r7, u8 *, 0x21B) = 9U;
                var_r5 = 4;
            } else {
                sp0 = 1;
                var_fp = 1;
                *sp8 = (s32) M2C_FIELD(temp_r7, u8 *, 0x21A);
                *sp4 = 0x3FFF & M2C_FIELD(temp_r7, u16 *, 0x178);
            }
            break;
        case 4:
            var_sl = 0;
            var_fp = Func_080a9f10(M2C_FIELD(temp_r7, u16 *, 0x178), M2C_FIELD(temp_r7, u8 *, 0x21A), M2C_FIELD(temp_r7, u8 *, 0x21B), 0);
            if (M2C_FIELD(temp_r7, u8 *, 0x21B) == 9) {
                M2C_FIELD(temp_r7, u8 *, 0x21B) = (u8) M2C_FIELD(temp_r7, u8 *, 0x21A);
                var_sl = 9;
            }
            if (var_fp != -1) {
                Func_08077120((s32) M2C_FIELD(temp_r7, u8 *, 0x21A), 0 - M2C_FIELD(Func_08077080(0x3FFF & M2C_FIELD(temp_r7, u16 *, 0x178)), u8 *, 9));
            }
            Func_08077010(M2C_FIELD(temp_r7, u8 *, 0x21A));
            if (var_fp != -1) {
                Func_080a112c(M2C_FIELD(temp_r7, s32 *, 0x24), M2C_FIELD(temp_r7, u8 *, 0x21B), 0, 0);
                Func_080aa460();
                Func_08015278(M2C_FIELD(temp_r7, s32 *, 0x2C));
                Func_080a1d08(M2C_FIELD(temp_r7, s16 *, 0x25A) + 0xBEF, 0, -1);
            } else {
                Func_080f9010(0x72);
                Func_08015278(M2C_FIELD(temp_r7, s32 *, 0x2C));
                Func_080a1d08(M2C_FIELD(temp_r7, s16 *, 0x25A) + 0xBEF, var_fp, var_fp);
            }
            if (var_fp != -1) {
                var_r1_2 = (s16 *)(temp_r7 + 0x220);
                var_fp = 1;
                goto block_44;
            }
            M2C_FIELD(temp_r7, s16 *, 0x222) = 1;
            if (var_sl != 9) {
                goto block_31;
            }
            M2C_FIELD(temp_r7, u16 *, 0x220) = (u16) (M2C_FIELD(temp_r7, u16 *, 0x220) | 1);
            var_r5 = 1;
            break;
        default:
            sp0 = 1;
            break;
        }
        goto loop_49;
    }
    if (Func_080770c0(0x150) != 0) {
        var_fp = -1;
    }
    return var_fp;
}
