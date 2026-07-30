typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_08002304(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
s32 Func_08003d28(s32 *);
void Func_08003dec(void *, s32);
s32 Func_08003fa4(s32, s32, s32);
u32 Func_08004458(void);
void Func_0801908c(void *);

/* Updates the eight active map-object lists and their attached sprite state. */
void Func_080191cc(void) {
    s32 sp0;
    s32 sp4;
    s32 sp10[2];
    u8 *temp_r7;
    s32 var_fp;
    u16 temp_r3_3;
    u16 temp_r3_4;
    u16 temp_r3_5;
    u16 temp_r3_6;
    u16 temp_r3_7;
    u32 temp_r3_2;
    u32 temp_r5_2;
    u32 temp_r5_3;
    u8 temp_r3_8;
    u8 temp_r4;
    u8 temp_r5;
    u8 temp_r8;
    u8 *temp_r3;
    u8 *var_r6;
    u8 *var_sl;

    temp_r3 = *(void **)0x03001E8C;
    var_sl = temp_r3 + 0x500;
    var_fp = 0;
loop_1:
    if (!(1 & M2C_FIELD(var_sl, u16 *, 0x16))) {

    } else {
        var_r6 = M2C_FIELD(var_sl, void **, 0);
loop_36:
        if (var_r6 != 0) {
            temp_r7 = var_r6 + 0x10;
            if (M2C_FIELD(var_sl, u16 *, 0x12) == 4) {
                M2C_FIELD(var_r6, u16 *, 0xC) = 2U;
                M2C_FIELD(var_r6, u8 *, 5) = 8U;
            }
            temp_r3_2 = M2C_FIELD(var_r6, u8 *, 5) - 2;
            switch (temp_r3_2) {
            case 0:
                if (M2C_FIELD(temp_r3, u16 *, 0x12B6) == 0x60) {

                } else {
                    temp_r3_3 = (0xFFFFFC00 & M2C_FIELD(temp_r7, u16 *, 8)) | (Func_08003fa4((s32) M2C_FIELD(temp_r3, u16 *, 0x12B6), 0x80, (((*(u32 *)0x03001800 >> 2) & 7) << 7) + 0x080368D4) & 0x3FF);
                    M2C_FIELD(temp_r7, u16 *, 8) = temp_r3_3;
                    M2C_FIELD(var_r6, s8 *, 0xE) = (s8) temp_r3_3;
                    temp_r4 = 0x3F & M2C_FIELD(temp_r7, u8 *, 7);
                    temp_r5 = (((-0xD & M2C_FIELD(temp_r7, u8 *, 5) & ~0x10) | 0x20) & 0x3F) | 0x80;
                    M2C_FIELD(temp_r7, u8 *, 7) = temp_r4;
                    M2C_FIELD(temp_r7, u8 *, 5) = temp_r5;
                    temp_r8 = M2C_FIELD(var_r6, u8 *, 8);
                    sp4 = 0x08033E60;
                    sp0 = (s32) temp_r4;
                    M2C_FIELD(temp_r7, s8 *, 4) = (s8) (temp_r8 + M2C_FIELD((void *) Func_08002304(*(s32 *)0x03001800, 0x50), u8 *, 0x08033E60) + 2);
                    M2C_FIELD(temp_r7, u8 *, 5) = (u8) (temp_r5 & ~3);
                    M2C_FIELD(temp_r7, u8 *, 7) = (u8) (-0x3F & temp_r4);
                }
                break;
            case 3:
                if (!(*(u32 *)0x03001800 & 1)) {

                } else {
                    temp_r5_2 = Func_08004458();
                    M2C_FIELD(temp_r7, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r7, u16 *, 6)) | (((M2C_FIELD(var_r6, u16 *, 6) + ((u32) (((u32) (temp_r5_2 * 3) >> 0x10) + ((u32) (Func_08004458() * 3) >> 0x10)) >> 1)) - 1) & 0x1FF));
                    temp_r5_3 = Func_08004458();
                    M2C_FIELD(temp_r7, s8 *, 4) = (s8) ((M2C_FIELD(var_r6, u8 *, 8) + ((u32) (((u32) (temp_r5_3 * 3) >> 0x10) + ((u32) (Func_08004458() * 3) >> 0x10)) >> 1)) - 1);
                }
                break;
            case 4:
                if (M2C_FIELD(var_r6, u16 *, 0xC) == 0) {
block_28:
                    M2C_FIELD(temp_r7, u8 *, 7) = (u8) (-0x3F & M2C_FIELD(temp_r7, u8 *, 7));
                    M2C_FIELD(temp_r7, u8 *, 5) = (u8) (-4 & M2C_FIELD(temp_r7, u8 *, 5));
                    M2C_FIELD(temp_r7, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r7, u16 *, 6)) | (0x1FF & M2C_FIELD(var_r6, u16 *, 6)));
                    M2C_FIELD(temp_r7, s8 *, 4) = (s8) (u16) M2C_FIELD(var_r6, u8 *, 8);
                } else {
                    sp10[0] = 0x02000200;
                    sp10[1] = 0;
                    M2C_FIELD(temp_r7, u8 *, 7) = (u8) ((-0x3F & M2C_FIELD(temp_r7, u8 *, 7)) | ((Func_08003d28(sp10) & 0x1F) * 2));
                    M2C_FIELD(temp_r7, u8 *, 5) = (u8) (M2C_FIELD(temp_r7, u8 *, 5) | 3);
                    M2C_FIELD(temp_r7, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r7, u16 *, 6)) | ((M2C_FIELD(var_r6, u16 *, 6) + 0xFFFB) & 0x1FF));
                    M2C_FIELD(temp_r7, s8 *, 4) = (s8) (M2C_FIELD(var_r6, u8 *, 8) + 0xFB);
                    M2C_FIELD(var_r6, u16 *, 0xC) = (u16) (M2C_FIELD(var_r6, u16 *, 0xC) + 0xFFFF);
                }
                break;
            case 5:
                M2C_FIELD(sp10, s16 *, 0) = 0x100;
                M2C_FIELD(sp10, s16 *, 2) = 0x100;
                temp_r3_4 = M2C_FIELD(var_r6, u16 *, 0xC) + 0x300;
                M2C_FIELD(var_r6, u16 *, 0xC) = temp_r3_4;
                M2C_FIELD(sp10, s32 *, 4) = temp_r3_4;
                M2C_FIELD(temp_r7, u8 *, 7) = (u8) ((-0x3F & M2C_FIELD(temp_r7, u8 *, 7)) | ((Func_08003d28(sp10) & 0x1F) * 2));
                M2C_FIELD(temp_r7, u8 *, 5) = (u8) ((-4 & M2C_FIELD(temp_r7, u8 *, 5)) | 1);
                M2C_FIELD(temp_r7, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r7, u16 *, 6)) | (((M2C_FIELD(var_r6, u16 *, 6) - (Func_08002322((u16) M2C_FIELD(sp10, s32 *, 4) + 0xE800) >> 0xE)) - 2) & 0x1FF));
                M2C_FIELD(temp_r7, s8 *, 4) = (s8) ((M2C_FIELD(var_r6, u8 *, 8) - (Func_0800231c((u16) M2C_FIELD(sp10, s32 *, 4) + 0x6800) >> 0xE)) - 2);
                break;
            case 2:
                if (*(u32 *)0x03001800 & 1) {
                    M2C_FIELD(var_r6, u16 *, 0xC) = (u16) (M2C_FIELD(var_r6, u16 *, 0xC) + 1);
                }
                sp0 = 0x08033EB0;
                M2C_FIELD(temp_r7, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r7, u16 *, 6)) | ((M2C_FIELD(var_r6, u16 *, 6) + M2C_FIELD(((u32) (Func_08002304((s32) M2C_FIELD(var_r6, u16 *, 0xC), 0x14) << 0x10) >> 0xF), s8 *, 0x08033EB0)) & 0x1FF));
                M2C_FIELD(temp_r7, s8 *, 4) = (s8) ((M2C_FIELD(var_r6, u8 *, 8) + M2C_FIELD((((u32) (Func_08002304((s32) M2C_FIELD(var_r6, u16 *, 0xC), 0x14) << 0x10) >> 0xF) + 1), u8 *, 0x08033EB0)) - 2);
                break;
            case 15:
                temp_r3_5 = M2C_FIELD(var_r6, u16 *, 0xC) + 1;
                M2C_FIELD(var_r6, u16 *, 0xC) = temp_r3_5;
                M2C_FIELD(temp_r7, s8 *, 4) = (s8) (M2C_FIELD(var_r6, u8 *, 8) - M2C_FIELD((temp_r3_5 & 0xF), u8 *, 0x08033EE8));
                break;
            case 12:
            case 13:
            case 14:
                temp_r3_6 = M2C_FIELD(var_r6, u16 *, 0xC) + 1;
                M2C_FIELD(var_r6, u16 *, 0xC) = temp_r3_6;
                M2C_FIELD(temp_r7, s8 *, 4) = (s8) (M2C_FIELD(var_r6, u8 *, 8) + M2C_FIELD((temp_r3_6 & 0xF), u8 *, 0x08033EE8));
                break;
            case 16:
                temp_r3_7 = M2C_FIELD(var_r6, u16 *, 0xC) + 1;
                M2C_FIELD(var_r6, u16 *, 0xC) = temp_r3_7;
                M2C_FIELD(temp_r7, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r7, u16 *, 6)) | ((M2C_FIELD(var_r6, u16 *, 6) - M2C_FIELD((temp_r3_7 & 0xF), s8 *, 0x08033EE8)) & 0x1FF));
                M2C_FIELD(temp_r7, s8 *, 4) = (s8) (M2C_FIELD(var_r6, u8 *, 8) + M2C_FIELD((0xF & M2C_FIELD(var_r6, u16 *, 0xC)), u8 *, 0x08033EE8));
                break;
            case 6:
                if (M2C_FIELD(var_r6, u16 *, 0xC) != 0) {
                    M2C_FIELD(sp10, s16 *, 0) = 0x140;
                    M2C_FIELD(sp10, s16 *, 2) = 0x140;
                    M2C_FIELD(sp10, s32 *, 4) = 0;
                    M2C_FIELD(temp_r7, u8 *, 7) = (u8) ((-0x3F & M2C_FIELD(temp_r7, u8 *, 7)) | ((Func_08003d28(sp10) & 0x1F) * 2));
                    M2C_FIELD(temp_r7, u8 *, 5) = (u8) (M2C_FIELD(temp_r7, u8 *, 5) | 3);
                    M2C_FIELD(temp_r7, u16 *, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r7, u16 *, 6)) | ((M2C_FIELD(var_r6, u16 *, 6) + 0xFFF8) & 0x1FF));
                    M2C_FIELD(temp_r7, s8 *, 4) = (s8) (M2C_FIELD(var_r6, u8 *, 8) + 0xF8);
                    M2C_FIELD(var_r6, u16 *, 0xC) = (u16) (M2C_FIELD(var_r6, u16 *, 0xC) + 0xFFFF);
                } else {
                    goto block_28;
                }
                break;
            case 7:
            case 8:
            case 9:
            case 10:
                Func_0801908c(var_r6);
                break;
            }
            temp_r3_8 = M2C_FIELD(var_r6, u8 *, 5);
            if (temp_r3_8 == 2) {
                if (M2C_FIELD(temp_r3, u16 *, 0x12B6) != 0x60) {
                    Func_08003dec(temp_r7, (s32) M2C_FIELD(var_r6, u8 *, 0xF));
                }
            } else if (temp_r3_8 != 0xD) {
                Func_08003dec(temp_r7, (s32) M2C_FIELD(var_r6, u8 *, 0xF));
            }
            var_r6 = M2C_FIELD(var_r6, void **, 0);
            goto loop_36;
        }
    }
    var_fp += 1;
    var_sl += 0x24;
    if (var_fp != 8) {
        goto loop_1;
    }
}
