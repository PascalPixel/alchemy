typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080091d8(void *, s32 *);
void *Func_080092a0(void *, s32 *);
s32 Func_080092a8(u8);
void Func_08097384(void);
void *Func_08098070(void *);
void Func_080981b0(void *);

/* Runs the interactive destination-selection and movement scene. */
void Func_08097c3c(void) {
    s8 *sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    void *sp14;
    void *sp18;
    s32 sp1C[3];
    s32 sp28[3];
    void *temp_r0_3;
    s32 temp_r0_4;
    s32 temp_r4;
    s32 temp_r5;
    u16 temp_r0_2;
    void *temp_r0;
    void *temp_r0_5;
    void *temp_r0_6;
    void *temp_r3;
    void *temp_r6;

    temp_r3 = *(void **)0x03001F30;
    sp18 = temp_r3;
    sp14 = M2C_FIELD(temp_r3, void **, 0x10);
    temp_r6 = M2C_FIELD(temp_r3, void **, 0x14);
    sp8 = M2C_FIELD(temp_r3, s32 *, 0) + 0x8000;
    sp4 = 0;
    if (temp_r6 == 0) {
        return;
    }
    Func_08097384();
    M2C_FIELD(sp14, void **, 0x68) = temp_r6;
    Func_08009098(sp14, 0x0809F0BC);
    temp_r0 = Func_08098070(sp14);
    if (temp_r0 == 0) {
        Func_0809748c();
        return;
    }
    M2C_FIELD(temp_r0, void **, 0x68) = temp_r6;
    M2C_FIELD(&sp28, s32 *, 0) = M2C_FIELD(temp_r6, s32 *, 8);
    M2C_FIELD(&sp28, s32 *, 4) = (s32) (M2C_FIELD(temp_r6, s32 *, 0xC) + 0x100000);
    M2C_FIELD(&sp28, s32 *, 8) = (s32) M2C_FIELD(temp_r6, s32 *, 0x10);
    Func_0800447c(0x100000, sp8, &sp28);
    Func_08009150(temp_r0, M2C_FIELD(&sp28, s32 *, 0), M2C_FIELD(&sp28, s32 *, 4), M2C_FIELD(&sp28, s32 *, 8));
    Func_08098184(temp_r0);
    M2C_FIELD(temp_r0, s32 *, 0x30) = 0x40000;
    M2C_FIELD(temp_r0, s32 *, 0x34) = 0x8000;
    sp0 = temp_r0 + 0x55;
    M2C_FIELD(temp_r0, s8 *, 0x55) = 4;
    M2C_FIELD(temp_r6, s32 *, 0x6C) = 0x08096B89;
    M2C_FIELD(temp_r6, s32 *, 0x30) = 0x6666;
    M2C_FIELD(temp_r6, s32 *, 0x34) = 0x3333;
    M2C_FIELD(temp_r6, u8 *, 0x5A) = (u8) sp4;
    M2C_FIELD(temp_r6, s8 *, 0x22) = 2;
loop_21:
    Func_080030f8(1U);
    temp_r5 = *(s32 *)0x03001C94 & 0x303;
    if (temp_r5 == 0) {
        temp_r0_2 = Func_08097b54();
        if (temp_r0_2 == 0xFFFF) {
            M2C_FIELD(&sp28, s32 *, 0) = M2C_FIELD(temp_r6, s32 *, 8);
            M2C_FIELD(&sp28, s32 *, 4) = (s32) (M2C_FIELD(temp_r6, s32 *, 0xC) + 0x100000);
            M2C_FIELD(&sp28, s32 *, 8) = (s32) M2C_FIELD(temp_r6, s32 *, 0x10);
            Func_0800447c(0x100000, sp8, &sp28);
            Func_08009150(temp_r0, M2C_FIELD(&sp28, s32 *, 0), M2C_FIELD(&sp28, s32 *, 4), M2C_FIELD(&sp28, s32 *, 8));
            Func_08009080((s32) temp_r0, 1);
            M2C_FIELD(temp_r0, s32 *, 0x24) = temp_r5;
            M2C_FIELD(temp_r0, s32 *, 0x28) = temp_r5;
            M2C_FIELD(temp_r0, s32 *, 0x2C) = temp_r5;
            goto loop_21;
        }
        M2C_FIELD(&sp28, s32 *, 0) = M2C_FIELD(temp_r6, s32 *, 8);
        M2C_FIELD(&sp28, s32 *, 4) = (s32) (M2C_FIELD(temp_r6, s32 *, 0xC) + 0x100000);
        M2C_FIELD(&sp28, s32 *, 8) = (s32) M2C_FIELD(temp_r6, s32 *, 0x10);
        Func_0800447c(0x100000, sp8, &sp28);
        Func_0800447c(0x20000, (s32) temp_r0_2, &sp28);
        Func_08009150(temp_r0, M2C_FIELD(&sp28, s32 *, 0), M2C_FIELD(&sp28, s32 *, 4), M2C_FIELD(&sp28, s32 *, 8));
        Func_08009158(temp_r0);
        M2C_FIELD(&sp28, s32 *, 0) = M2C_FIELD(temp_r6, s32 *, 8);
        M2C_FIELD(&sp28, s32 *, 4) = (s32) M2C_FIELD(temp_r6, s32 *, 0xC);
        M2C_FIELD(&sp28, s32 *, 8) = (s32) M2C_FIELD(temp_r6, s32 *, 0x10);
        Func_0800447c(0x100000, (s32) temp_r0_2, &sp28);
        M2C_FIELD(&sp1C, s32 *, 0) = M2C_FIELD(temp_r6, s32 *, 8);
        M2C_FIELD(&sp1C, s32 *, 4) = (s32) M2C_FIELD(temp_r6, s32 *, 0xC);
        M2C_FIELD(&sp1C, s32 *, 8) = (s32) M2C_FIELD(temp_r6, s32 *, 0x10);
        Func_0800447c(0x200000, (s32) temp_r0_2, &sp1C);
        if (Func_080091d8(temp_r6, sp28) <= 0) {
            temp_r0_3 = Func_080092a0(temp_r6, sp28);
            if (temp_r0_3 != 0) {
                if ((temp_r0_3 == sp14) && ((temp_r0_4 = M2C_FIELD(sp14, s32 *, 8) & 0xFFF00000, temp_r4 = M2C_FIELD(sp14, s32 *, 0x10) & 0xFFF00000, (temp_r0_4 != (M2C_FIELD(&sp28, s32 *, 0) & 0xFFF00000))) || (temp_r4 != (M2C_FIELD(&sp28, s32 *, 8) & 0xFFF00000)))) {
                    if ((temp_r0_4 == (M2C_FIELD(&sp1C, s32 *, 0) & 0xFFF00000)) && (temp_r4 == (M2C_FIELD(&sp1C, s32 *, 8) & 0xFFF00000))) {
                        if (Func_080092a8(M2C_FIELD(sp14, u8 *, 0x22)) != 0) {
                            goto block_15;
                        }
                        sp4 = 1;
                        goto block_18;
                    }
                    goto block_18;
                }
                goto block_15;
            }
block_18:
            Func_080f9010(0xAF);
            sp10 = M2C_FIELD(&sp28, s32 *, 0);
            spC = M2C_FIELD(&sp28, s32 *, 8);
            Func_08009080((s32) temp_r0, (s32) M2C_FIELD(((u32) ((sp8 - temp_r0_2) << 0x10) >> 0x1E), u8 *, 0x0809F118));
            Func_080030f8(0xFU);
            M2C_FIELD(temp_r6, s8 *, 0x5B) = 0;
            M2C_FIELD(temp_r6, s32 *, 0x30) = 0x3333;
            M2C_FIELD(temp_r6, s32 *, 0x34) = 0x3333;
            Func_08009150(temp_r6, M2C_FIELD(&sp28, s32 *, 0), M2C_FIELD(&sp28, s32 *, 4), M2C_FIELD(&sp28, s32 *, 8));
            *sp0 = 0;
            M2C_FIELD(temp_r0, s32 *, 0x30) = 0x3333;
            M2C_FIELD(temp_r0, s32 *, 0x34) = 0x3333;
            Func_0800447c(0x100000, (s32) temp_r0_2, &sp28);
            Func_08009150(temp_r0, M2C_FIELD(&sp28, s32 *, 0), M2C_FIELD(&sp28, s32 *, 4) + 0x100000, M2C_FIELD(&sp28, s32 *, 8));
            if (sp4 == 1) {
                temp_r0_5 = (void *) Func_08092054();
                M2C_FIELD(temp_r0_5, u8 *, 0x5A) = (u8) (0xFE & M2C_FIELD(temp_r0_5, u8 *, 0x5A));
                M2C_FIELD(sp14, s32 *, 0x30) = 0x3333;
                M2C_FIELD(sp14, s32 *, 0x34) = 0x3333;
                Func_08009150(sp14, M2C_FIELD(&sp1C, s32 *, 0), M2C_FIELD(&sp1C, s32 *, 4), M2C_FIELD(&sp1C, s32 *, 8));
            }
            Func_08009158(temp_r6);
            M2C_FIELD(temp_r6, s32 *, 8) = sp10;
            M2C_FIELD(temp_r6, s32 *, 0x10) = spC;
            M2C_FIELD(temp_r6, s32 *, 0x24) = 0;
            M2C_FIELD(temp_r6, s32 *, 0x2C) = 0;
        } else {
block_15:
            Func_08009080((s32) temp_r0, 4);
            if (!(*(s32 *)0x03001E40 & 0xF)) {
                Func_080f9010(0x72);
            }
            goto loop_21;
        }
    }
    Func_08009240((s32) temp_r6, (s32) M2C_FIELD(sp18, u8 *, 0x44));
    Func_08009098(temp_r6, M2C_FIELD(sp18, s32 *, 0x3C));
    M2C_FIELD(temp_r6, s32 *, 0x6C) = (s32) M2C_FIELD(sp18, s32 *, 0x38);
    Func_08097174();
    if (sp4 == 1) {
        temp_r0_6 = (void *) Func_08092054();
        M2C_FIELD(temp_r0_6, u8 *, 0x5A) = (u8) (1 | M2C_FIELD(temp_r0_6, u8 *, 0x5A));
    }
    Func_0809748c();
    Func_080981b0(temp_r0);
}
