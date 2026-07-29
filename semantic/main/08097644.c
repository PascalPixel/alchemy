typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080022f4(s32, s32);
void *Func_080090c8(s32, s32);
s16 *Func_0808d394();
void *Func_08092054(void);
void Func_080978c4(void);
void *Func_08185000(s32);

/* Advances the wave buffer and periodically spawns its traveling map effect. */
void Func_08097644(void) {
    s32 target_pos[3];
    s32 source_pos[3];
    s16 *var_r6;
    s16 temp_r0_4;
    s32 temp_r0;
    void *temp_r0_2;
    void *temp_r6;
    void *temp_r9;
    u32 var_r5;
    u8 temp_r2;
    u8 temp_r3;
    u8 temp_r3_2;
    u8 temp_r3_3;
    void *temp_r0_3;
    void *temp_r5;
    void *temp_r7;

    temp_r7 = *(void **)0x03001EA8;
    temp_r2 = M2C_FIELD(temp_r7, u8 *, 0x294);
    if (temp_r2 != 0) {
        M2C_FIELD(temp_r7, u8 *, 0x294) = (u8) (temp_r2 + 0xFF);
        return;
    }
    var_r6 = temp_r7 + ((1 ^ M2C_FIELD(temp_r7, u8 *, 0x28A)) * 0x144);
    var_r5 = 0;
    do {
        temp_r0 = Func_08002322(Func_080022f4((M2C_FIELD(temp_r7, u16 *, 0x288) + (var_r5 * 8)) << 0x10, 0xA0));
        var_r5 += 1;
        *var_r6 = (s16) (temp_r0 >> 0xE);
        var_r6 += 1;
    } while (var_r5 <= 0x9FU);
    M2C_FIELD(temp_r7, u16 *, 0x288) = (u16) (M2C_FIELD(temp_r7, u16 *, 0x288) + 4);
    temp_r3 = M2C_FIELD(temp_r7, u8 *, 0x28A) ^ 1;
    M2C_FIELD(temp_r7, u8 *, 0x28A) = temp_r3;
    if (temp_r3 != 0) {
        Func_08091200((M2C_FIELD(temp_r7, s8 *, 0x28D) << 0xA) | ((s8) M2C_FIELD(temp_r7, u8 *, 0x28C) << 5) | (s8) M2C_FIELD(temp_r7, u8 *, 0x28B) | 0x200000, 1);
        Func_08091254(1);
        Func_080978c4();
    }
    Func_0808e0b0(Func_08092054(), 0);
    temp_r3_2 = M2C_FIELD(temp_r7, u8 *, 0x295);
    if ((temp_r3_2 == 0) || (temp_r3_2 == 8) || (temp_r3_2 == 0x10)) {
        temp_r6 = Func_08092054();
        temp_r0_2 = Func_08092054();
        temp_r9 = temp_r0_2;
        if ((temp_r6 != 0) && (temp_r0_2 != 0)) {
            target_pos[0] = M2C_FIELD(temp_r6, s32 *, 8);
            target_pos[1] = M2C_FIELD(temp_r6, s32 *, 0xC) +
                (M2C_FIELD((void *)Func_08185000(
                    *Func_0808d394((s16)M2C_FIELD(temp_r7, u16 *, 0x290))),
                    s8 *, 8) << 0x10) + 0xFFFE0000;
            target_pos[2] = M2C_FIELD(temp_r6, s32 *, 0x10);
            source_pos[0] = M2C_FIELD(temp_r9, s32 *, 8);
            source_pos[1] = M2C_FIELD(temp_r9, s32 *, 0xC) +
                (M2C_FIELD((void *)Func_08185000(
                    *Func_0808d394((s16)M2C_FIELD(temp_r7, u16 *, 0x292),
                                   M2C_FIELD(temp_r9, s32 *, 8))),
                    s8 *, 8) << 0x10) + 0xFFFE0000;
            source_pos[2] = M2C_FIELD(temp_r9, s32 *, 0x10);
            temp_r0_3 = Func_080090c8(0x119, source_pos[0]);
            if (temp_r0_3 != 0) {
                temp_r5 = M2C_FIELD(temp_r0_3, void **, 0x50);
                M2C_FIELD(temp_r0_3, s8 *, 0x55) = 0;
                M2C_FIELD(temp_r0_3, s32 *, 0x30) = 0xA3D7;
                M2C_FIELD(temp_r0_3, s32 *, 0x34) = 0xA3D7;
                temp_r0_4 = Func_080044d0(
                    target_pos[2] - source_pos[2],
                    target_pos[0] - source_pos[0]);
                M2C_FIELD(temp_r0_3, s32 *, 0x6C) = 0x08097A55;
                M2C_FIELD(temp_r5, s8 *, 0x26) = 0;
                M2C_FIELD(temp_r0_3, s16 *, 6) = temp_r0_4;
                M2C_FIELD(temp_r5, u8 *, 9) = (u8) ((-0xD & M2C_FIELD(temp_r5, u8 *, 9)) | 4);
                Func_08009150(temp_r0_3, target_pos[0], target_pos[1],
                              target_pos[2]);
            }
        }
    }
    if (M2C_FIELD(temp_r7, u8 *, 0x295) == 0) {
        Func_080f9010(0x82);
    }
    temp_r3_3 = M2C_FIELD(temp_r7, u8 *, 0x295) + 1;
    M2C_FIELD(temp_r7, u8 *, 0x295) = temp_r3_3;
    if ((u32) (temp_r3_3 << 0x18) > 0x3C000000U) {
        M2C_FIELD(temp_r7, u8 *, 0x295) = 0U;
    }
}
