typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02002500, 1640 bytes as inventoried
 * (0x02002500-0x02002b67): code 0x02002500-0x02002b5f, the embedded jump table
 * at 0x020028b0-0x020028e3, and literal pools at 0x02002858-0x0200288b and
 * 0x02002b60 onwards.  The trailing pool actually runs to 0x02002ba3, four
 * words past the inventoried row; 0x02002ba4 is the next function's prologue.
 * Only the row's own 1640 bytes are claimed here.
 *
 * This is the overlay's scene state machine.  It is entered every time the
 * scene is re-evaluated, publishes a fixed workspace value, and then dispatches
 * on (chapter, step) = Data_02000240[224], Data_02000240[225].  Chapters 0x44
 * and 0x45 use compare chains; chapter 0x46 uses the embedded 13-entry jump
 * table.  Every arm reduces to "if this scene flag is not yet set, run the beat
 * and set it", which is why Func_080770c0 (test) and Func_080770c8 (set) appear
 * throughout.
 *
 * All 145 assembly `bl` sites in the row are placed; they reach 28 distinct
 * callees.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 * This owner supplies the two remaining classic witnesses for the 0x02008000
 * base:
 *   - the dispatcher at 0x020028a6 loads its jump-table base as 0x0200a8b0
 *     while the table is physically embedded at offset 0x28b0, and every one of
 *     its thirteen entries is 0x8000 above the case body it selects;
 *   - the installed descriptor 0x0200b084 passed to Func_0808a098 is the
 *     in-image data at offset 0x3084.
 * Consistently, Data_03001ebc and Data_02000240 lie *below* 0x02008000 and are
 * therefore genuine RAM globals - the same two symbols the byte-exact sources
 * in assets/code use, with Data_02000240[224] being the very halfword
 * resource_39f_c_02000ee0.c switches on.
 *
 * The pooled 0x02xx0000 constants are 16.16 coordinates, not addresses: this
 * module's own image ends at 0x0200be2c, so 0x02de0000 cannot be one, and
 * 0x02de0000 >> 16 = 734 is an ordinary tile position.
 *
 * The epilogue is `add sp, #8 / pop {r5} / pop {r1} / bx r1`, so r0 is the
 * result; it is zeroed on the single common exit, so the owner always
 * returns 0.
 *
 * The byte-exact sources spell the chapter constants as the addresses of
 * link-time symbols Value_00000044/45/46; they are written as plain 0x44, 0x45
 * and 0x46 here.
 */

extern u8 *Data_03001ebc;      /* workspace pointer */
extern s16 Data_02000240[];    /* engine state; [224] chapter, [225] step */
extern u8 Data_0200b084[];     /* in-image descriptor at file offset 0x3084 */

/* Scene flags: test and set. */
s32 Func_080770c0();
void Func_080770c8();

void Func_0808a018();
void Func_080000c0();          /* ROM dispatch[0] -> Func_080030f8 */
void Func_0808a158();
void Func_0808a0f0();
void Func_0808a1b8();
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a360();
void Func_0808a370();
void Func_0808a0e8();
void Func_0808a010();
void Func_0808a1e8();
void Func_0808a138();
void Func_0808a100();
void Func_0808a098();
u8 *Func_0808a080();
void Func_080091e0();
/* Four register arguments plus two stack words. */
void Func_080091c0();
void Func_0808a020();

void Func_02000d90();
void Func_02000e18();
void Func_02002c0c();
void Func_020008c0();
void Func_02000a4c();
void Func_02001c34();
/* Byte-exact: (layer, x, z, width, height, value). */
s32 Func_02000244();

s32 Func_02002500(void)
{
    s32 step;
    s32 releaseSlot;
    s32 x;
    s32 z;
    u8 *record;

    *(s32 *)(Data_03001ebc + 448) = 516;      /* 129 << 2 */

    step = Data_02000240[225];

    if (Data_02000240[224] == 0x44) {
        if (step < 1) return 0;

        if (step <= 4) {
            if (Func_080770c0(0x89c) == 0) {
                Func_0808a018();
                Func_080000c0(1);
                Func_0808a158(10, 1);
                Func_0808a0f0(10, 184 << 15, 240 << 15);
                Func_0808a1b8(10, 0xd000, 0);        /* 208 << 8 */
                Func_0808a090(0, 0x6666, 0x3333);
                Func_0808a0c8(0, 136, 64);
                Func_0808a360();
                Func_0808a370();
                Func_0808a0e8(0);
                Func_0808a010(30);
                Func_0808a1e8(10, 256, 0);
                Func_0808a138(10, 2);
                Func_0808a010(30);
                Func_02000d90(10, 136, 116, 0x70000);   /* 224 << 11 */
                Func_02000e18(10);
                Func_0808a158(10, 15);
                Func_080091e0(Func_0808a080(10), 0);
                Func_080770c8(0x89c);
                Func_0808a010(60);
                Func_0808a020();
            }

            if (Func_080770c0(0x109) == 0) return 0;
            if (Func_080770c0(768) != 0) return 0;      /* 192 << 2 */
            Func_0808a158(10, 15);
            Func_0808a0f0(10, 136 << 16, 232 << 15);
            return 0;
        }

        if (step > 9 || step < 7) return 0;

        /* steps 7-9 */
        record = Func_0808a080(0);
        if (record != 0) {
            Func_0808a0f0(16, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        *(u32 *)(Func_0808a080(16) + 108) = 0;

        if (Func_080770c0(0x109) != 0) {
            *(s32 *)(Func_0808a080(16) + 12) = 0x200000;    /* 128 << 14 */
        }

        Func_080000c0(1);
        Func_0808a0f0(16, 158 << 18, 220 << 17);

        if (Func_080770c0(0xfd4) == 0) Func_02002c0c(16);

        Func_0808a158(11, 15);
        Func_0808a158(12, 15);
        Func_080091e0(Func_0808a080(11), 0);
        Func_080091e0(Func_0808a080(12), 0);
        Func_020008c0(8);

        if (Func_080770c0(784) == 0) {                       /* 196 << 2 */
            Func_020008c0(9);
            return 0;
        }

        Func_080000c0(1);
        Func_0808a0f0(9, 132 << 18, 204 << 17);
        Func_0808a100(9, 4);
        Func_080091c0(38, 27, 4, 2, 31, 25);
        Func_0808a080(9)[35] = 2;
        return 0;
    }

    if (Data_02000240[224] == 0x45) {
        if (step < 3) return 0;

        if (step <= 6) {
            if (Func_080770c0(0x303) == 0) {
                Func_0808a158(12, 15);
                Func_080091e0(Func_0808a080(12), 0);
            }
            if (Func_080770c0(772) != 0) return 0;           /* 193 << 2 */
            Func_0808a158(13, 15);
            /* The assembly jumps into the shared two-call tail at 0x02002962
             * with r0 already holding 13, so this is the SAME call site as the
             * one spelled below for slot 21, not a second one. */
            releaseSlot = 13;
            goto release;
        }

        if (step > 12 || step < 10) return 0;

        /* steps 10-12 */
        if (Func_080770c0(0x311) == 0) {
            Func_020008c0(10);
        } else {
            Func_080000c0(1);
            Func_0808a0f0(10, 138 << 18, 255 << 17);
            Func_0808a100(10, 4);
            Func_0808a080(10)[35] = 2;
            Func_080091c0(44, 30, 2, 4, 34, 30);
            Func_02000244(0, 35, 29, 1, 4, 0);
        }

        Func_020008c0(8);
        Func_020008c0(9);

        x = *(s32 *)(Func_0808a080(11) + 8) >> 20;
        z = *(s32 *)(Func_0808a080(11) + 16) >> 20;
        Func_02000244(2, x, z, 1, 1, 0xff);

        Func_080000c0(1);
        Func_0808a158(11, 6);
        Func_0808a080(8)[89] |= 8;

        if (Func_080770c0(0x306) != 0) return 0;
        Func_0808a158(14, 15);
        Func_080091e0(Func_0808a080(14), 0);

        if (Func_080770c0(0x305) == 0) return 0;
        Func_0808a0f0(14, 212 << 17, 240 << 17);
        Func_0808a0f0(17, 212 << 17, 240 << 17);
        return 0;
    }

    if (Data_02000240[224] != 0x46) return 0;

    /* Chapter 0x46 dispatches through the embedded table at 0x020028b0.  Its
     * thirteen entries map steps 3-15 onto five case bodies; step 14 selects
     * the common exit directly. */
    if ((u32)(step - 3) > 12) return 0;

    switch (step) {
    case 3:
    case 4:
    case 5:
    case 6:
        Func_080000c0(1);
        if (Func_080770c0(0x307) == 0) {
            Func_0808a158(15, 15);
            Func_080091e0(Func_0808a080(15), 0);
            Func_080091e0(Func_0808a080(19), 0);
        }
        if (Func_080770c0(776) == 0) {                       /* 194 << 2 */
            Func_0808a158(16, 15);
            Func_080091e0(Func_0808a080(16), 0);
            Func_080091e0(Func_0808a080(20), 0);
        }
        if (Func_080770c0(0x309) != 0) return 0;
        Func_0808a158(17, 15);
        Func_080091e0(Func_0808a080(17), 0);
        releaseSlot = 21;
release:
        Func_080091e0(Func_0808a080(releaseSlot), 0);
        return 0;

    case 7:
        x = *(s32 *)(Func_0808a080(13) + 8) >> 20;
        z = *(s32 *)(Func_0808a080(13) + 16) >> 20;
        Func_02000244(2, x, z, 1, 1, 0xff);
        Func_0808a158(13, 6);
        Func_080000c0(1);
        Func_0808a080(8)[89] |= 8;
        Func_020008c0(8);
        return 0;

    case 8:
    case 9:
    case 10:
    case 11:
        /* Both banners are placed at the same height, 185 << 17. */
        Func_02000a4c(0x02de0000, 0, 185 << 17, 223);
        Func_02000a4c(0x02f20000, 0, 185 << 17, 223);
        Func_020008c0(10);
        Func_020008c0(12);

        if (Func_080770c0(0x312) == 0) {
            Func_020008c0(9);
        } else {
            Func_080000c0(1);
            Func_0808a100(9, 4);
            Func_0808a0f0(9, 0x02ba0000, 199 << 17);
            Func_0808a080(9)[35] |= 2;
            Func_080091c0(26, 20, 2, 4, 42, 23);
            Func_080770c8(532);                              /* 133 << 2 */
            Func_0808a0f0(14, 158 << 18, 220 << 17);
            Func_080091e0(Func_0808a080(14), 0);
        }

        if (Func_080770c0(0x313) == 0) {
            Func_020008c0(11);
        } else {
            Func_080000c0(1);
            Func_0808a100(11, 4);
            Func_0808a0f0(11, 0x029a0000, 0x02260000);
            Func_0808a080(11)[35] = 2;
            Func_080091c0(26, 20, 2, 4, 40, 32);
        }

        x = *(s32 *)(Func_0808a080(14) + 8) >> 20;
        z = *(s32 *)(Func_0808a080(14) + 16) >> 20;
        Func_02000244(2, x, z, 1, 1, 0xff);
        Func_0808a158(14, 6);
        Func_080000c0(1);
        Func_0808a080(9)[89] |= 8;

        if (Func_080770c0(0x30b) == 0) {
            Func_0808a158(18, 15);
            Func_080091e0(Func_0808a080(18), 0);
            if (Func_080770c0(0x30a) != 0) {
                Func_0808a0f0(22, 186 << 18, 252 << 17);
                Func_0808a0f0(18, 186 << 18, 252 << 17);
            }
        }

        Func_02001c34();
        return 0;

    case 12:
    case 13:
        Func_0808a098(18, Data_0200b084);
        if (Func_080770c0(0x893) == 0) return 0;
        if (Func_080770c0(0x89e) == 0) return 0;
        Func_080770c8(0x88f);
        return 0;

    case 15:
        Func_080770c8(0x89e);
        return 0;

    default:
        /* step 14: the table entry points straight at the common exit. */
        return 0;
    }
}
