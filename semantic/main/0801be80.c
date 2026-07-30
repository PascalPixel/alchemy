typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_080030f8(u32);
void Func_08003f3c(u32);
void *Func_0801a910(s32);
void Func_0801b9a8(void *, u16);
void Func_0801ba34(u8 *);
void Func_0801c21c(void);

/*
 * Collapses the active formation to its selected entry, recenters the
 * remaining list nodes, and creates the corresponding display record.
 */
u32 Func_0801be80(u8 *state) {
    s32 temp_r1;
    s32 var_r8;
    s32 var_r8_2;
    u16 temp_r2;
    u16 temp_r3;
    u32 temp_r9;
    u8 *temp_r0;
    u8 *temp_r0_2;
    u8 *temp_r3_2;
    u8 *var_r2;
    u8 *var_r3;
    u8 *var_r6;
    u8 *var_r6_2;
    u8 *var_r7;
    u8 *var_r7_2;
    u8 *var_r7_3;

    var_r8 = 0;
    temp_r9 = M2C_FIELD(state, u16 *, 0x39C) + M2C_FIELD(state, u16 *, 0x39E);
    Func_0801ba34((u8 *) state);
    Func_0801b9a8(state, M2C_FIELD(state, u16 *, 0x39E));
    M2C_FIELD(state, s16 *, 0x3A2) = 0x21;
    Func_080030f8(1U);
    M2C_FIELD(state, s16 *, 0xA) = 0;
    M2C_FIELD(state, s16 *, 0x3E) = 0;
    M2C_FIELD(state, s16 *, 0x2E2) = 0;
    M2C_FIELD(state, s16 *, 0x2FA) = 0;
    Func_0801c21c();
    var_r6 = M2C_FIELD(state, void **, 0x348);
    if ((var_r6 != 0) && (M2C_FIELD(state, u16 *, 0x39E) != 0)) {
loop_2:
        var_r6 = M2C_FIELD(var_r6, void **, 4);
        var_r8 += 1;
        if (var_r6 != 0) {
            if (M2C_FIELD(state, u16 *, 0x39E) != var_r8) {
                goto loop_2;
            }
        }
    }
    M2C_FIELD(var_r6, u16 *, 0x1C) = (u16) M2C_FIELD(var_r6, u16 *, 0x10);
    M2C_FIELD(var_r6, u16 *, 0x1E) = (u16) M2C_FIELD(var_r6, u16 *, 0x12);
    var_r7 = M2C_FIELD(state, void **, 0x348);
    if (var_r7 != 0) {
        do {
            if (var_r7 != var_r6) {
                M2C_FIELD(var_r7, u16 *, 0x18) = (u16) M2C_FIELD(var_r6, u16 *, 0x10);
                M2C_FIELD(var_r7, s16 *, 0x14) = (s16) ((s32) ((s16) M2C_FIELD(var_r6, u16 *, 0x10) - M2C_FIELD(var_r7, s16 *, 0x10)) >> 1);
            }
            var_r7 = M2C_FIELD(var_r7, void **, 4);
        } while (var_r7 != 0);
    }
    Func_080030f8(2U);
    var_r7_2 = M2C_FIELD(state, void **, 0x348);
    if (var_r7_2 != 0) {
        do {
            if (var_r7_2 != var_r6) {
                Func_08003f3c((u32) M2C_FIELD(var_r7_2, u16 *, 0xC));
                M2C_FIELD(var_r7_2, s16 *, 0xA) = 0;
            }
            var_r7_2 = M2C_FIELD(var_r7_2, void **, 4);
        } while (var_r7_2 != 0);
    }
    M2C_FIELD(state, void **, 0x348) = var_r6;
    M2C_FIELD(var_r6, s32 *, 0) = 0;
    M2C_FIELD(var_r6, s32 *, 4) = 0;
    M2C_FIELD(var_r6, u16 *, 0x18) = 4U;
    var_r7_3 = M2C_FIELD(state, void **, 0x34C);
    var_r8_2 = 0;
    if (var_r7_3 != 0) {
        do {
            M2C_FIELD(var_r6, u16 *, 0x18) = (u16) (M2C_FIELD(var_r6, u16 *, 0x18) + 0x10);
            var_r7_3 = M2C_FIELD(var_r7_3, void **, 4);
            var_r8_2 += 1;
        } while (var_r7_3 != 0);
    }
    temp_r1 = var_r8_2 * 2;
    M2C_FIELD(state, u16 *, temp_r1 + 0x3A4) =
        M2C_FIELD(state, u16 *, 0x39C);
    M2C_FIELD(state, u16 *, temp_r1 + 0x3AE) =
        M2C_FIELD(state, u16 *, 0x39E);
    M2C_FIELD(var_r6, s16 *, 0x14) = (s16) ((s32) ((s16) M2C_FIELD(var_r6, u16 *, 0x18) - (s16) M2C_FIELD(var_r6, u16 *, 0x10)) >> 1);
    M2C_FIELD(state, s16 *, 0x39A) = 0;
    M2C_FIELD(state, u16 *, 0x39E) = (u16) (0x80 | M2C_FIELD(state, u16 *, 0x39E));
    Func_080030f8(2U);
    temp_r0 = (void *) Func_0801a910(1);
    M2C_FIELD(temp_r0, u16 *, 0xA) = (u16) M2C_FIELD(var_r6, u16 *, 0xA);
    M2C_FIELD(temp_r0, u16 *, 0x20) = (u16) M2C_FIELD(var_r6, u16 *, 0x20);
    M2C_FIELD(temp_r0, u16 *, 8) = (u16) M2C_FIELD(var_r6, u16 *, 8);
    M2C_FIELD(temp_r0, u16 *, 0xC) = (u16) M2C_FIELD(var_r6, u16 *, 0xC);
    M2C_FIELD(temp_r0, u16 *, 0xE) = (u16) M2C_FIELD(var_r6, u16 *, 0xE);
    temp_r2 = M2C_FIELD(var_r6, u16 *, 0x10);
    M2C_FIELD(temp_r0, u16 *, 0x10) = temp_r2;
    temp_r3 = M2C_FIELD(var_r6, u16 *, 0x12);
    M2C_FIELD(temp_r0, u16 *, 0x18) = temp_r2;
    M2C_FIELD(temp_r0, u16 *, 0x12) = temp_r3;
    M2C_FIELD(temp_r0, u16 *, 0x1A) = temp_r3;
    M2C_FIELD(temp_r0, u16 *, 0x1C) = (u16) M2C_FIELD(var_r6, u16 *, 0x1C);
    M2C_FIELD(temp_r0, u16 *, 0x1E) = (u16) M2C_FIELD(var_r6, u16 *, 0x1E);
    M2C_FIELD(temp_r0, s16 *, 0x14) = 0;
    M2C_FIELD(temp_r0, s16 *, 0x16) = 0;
    M2C_FIELD(temp_r0, s16 *, 0x22) = 0x100;
    M2C_FIELD(temp_r0, s16 *, 0x26) = 0x100;
    temp_r0_2 = temp_r0 + 0x28;
    M2C_FIELD(temp_r0_2, u8 *, 5) = (u8) (-0xD & M2C_FIELD(temp_r0_2, u8 *, 5) & ~0x20 & ~0x10 & 0x3F);
    M2C_FIELD(temp_r0_2, u8 *, 7) = (u8) ((0x3F & M2C_FIELD(temp_r0_2, u8 *, 7)) | 0x40);
    M2C_FIELD(temp_r0_2, u8 *, 9) = (u8) (0xF & M2C_FIELD(temp_r0_2, u8 *, 9));
    M2C_FIELD(temp_r0_2, u16 *, 8) = (u16) ((0xFFFFFC00 & M2C_FIELD(temp_r0_2, u16 *, 8)) | (0x3FF & M2C_FIELD(temp_r0, u16 *, 0xE)));
    M2C_FIELD(var_r6, u16 *, 0xA) = 0U;
    M2C_FIELD(state, void **, 0x348) = 0;
    var_r3 = M2C_FIELD(state, void **, 0x34C);
    if (var_r3 != 0) {
        var_r6_2 = var_r3;
        var_r2 = M2C_FIELD(var_r6_2, void **, 4);
        if (var_r2 != 0) {
            do {
                var_r6_2 = var_r2;
                temp_r3_2 = M2C_FIELD(var_r6_2, void **, 4);
                var_r2 = temp_r3_2;
            } while (temp_r3_2 != 0);
        }
        var_r3 = 0;
        M2C_FIELD(var_r6_2, void **, 4) = temp_r0;
        M2C_FIELD(temp_r0, void **, 0) = var_r6_2;
    } else {
        M2C_FIELD(state, void **, 0x34C) = temp_r0;
        M2C_FIELD(temp_r0, void **, 0) = var_r3;
    }
    M2C_FIELD(temp_r0, void **, 4) = var_r3;
    return temp_r9;
}
