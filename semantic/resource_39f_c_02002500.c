#include "types.h"

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
s32 Func_020052d0();
void Func_020052f0();
void Func_0200523e();
void Func_0200537e();
void Func_0200535c();
void Func_020053a8();
void Func_02005342();
void Func_02005364();
void Func_02005420();
void Func_0200542c();
void Func_02005382();
void Func_02005338();
void Func_020053ec();
void Func_020053bc();
void Func_02005352();
void Func_02003354();
void Func_020033e2();
void Func_020053f6();
u8 *Func_02005394();
void Func_0200533a();
void Func_02005378();
void Func_02005386();
void Func_0200539a();
s32 Func_02005380();
s32 Func_0200538e();
void Func_02005434();
void Func_02005412();
u8 *Func_020053e2();
void Func_02005428();
u8 *Func_020053f6_b();
s32 Func_020053d0();
u8 *Func_0200540a();
void Func_02005346();
void Func_0200545c();
s32 Func_020053fa();
void Func_02005284();
void Func_020054a4();
void Func_020054ac();
u8 *Func_0200544a();
void Func_020053f0();
u8 *Func_02005456();
void Func_020053fc();
void Func_02002f66();
s32 Func_0200543a();
void Func_02002f78();
void Func_020053ac();
void Func_020054c2();
void Func_020054d2();
void Func_02005436();
u8 *Func_020054ac_b();
s32 Func_020054b4();
void Func_02005558();
u8 *Func_020054f6();
void Func_0200549c();
s32 Func_020054d4();
void Func_0200557a();
s32 Func_020054ec();
void Func_0200302a();
void Func_0200545e();
void Func_02005574();
void Func_02005584();
u8 *Func_0200554a();
void Func_020054f4();
s32 Func_02002a00();
void Func_02003082();
void Func_02003088();
u8 *Func_0200558a();
u8 *Func_02005592();
s32 Func_02002a32();
void Func_020054e0();
void Func_02005620();
u8 *Func_020055be();
s32 Func_0200559e();
void Func_02005644();
u8 *Func_020055e2();
void Func_02005588();
s32 Func_020055be_b();
void Func_0200563a();
void Func_02005648();
void Func_020055d4();
s32 Func_0200567a();
void Func_0200571e();
u8 *Func_020056bc();
void Func_02005662();
u8 *Func_020056c8();
void Func_0200566e();
s32 Func_020056a6();
void Func_0200574a();
u8 *Func_020056e8();
void Func_0200568e();
u8 *Func_020056f4();
void Func_0200569a();
s32 Func_020056d0();
void Func_02005776();
u8 *Func_02005714();
void Func_020056ba();
u8 *Func_02005720();
void Func_020056c6();
u8 *Func_0200572e();
u8 *Func_02005736();
s32 Func_02002bd6();
void Func_020057be();
void Func_0200568c();
u8 *Func_02005762();
void Func_02003276();
void Func_02003414();
void Func_02003420();
void Func_0200329a();
void Func_020032a0();
s32 Func_02005772();
void Func_020032b0();
void Func_020056e4();
void Func_020057fc();
void Func_02005800();
u8 *Func_020057ce();
void Func_0200577c();
void Func_020057cc();
void Func_0200583a();
u8 *Func_02005808();
void Func_020057ae();
s32 Func_020057e4();
void Func_02003322();
void Func_02005756();
void Func_0200586e();
void Func_02005870();
u8 *Func_0200583e();
void Func_020057e8();
u8 *Func_0200585e();
u8 *Func_02005866();
s32 Func_02002d06();
void Func_020058ee();
void Func_020057bc();
u8 *Func_02005892();
s32 Func_02005872();
void Func_02005916();
u8 *Func_020058b4();
void Func_0200585a();
s32 Func_02005890();
void Func_0200590a();
void Func_02005918();
void Func_0200475c();
void Func_020058fe();
s32 Func_020058c4();
s32 Func_020058ce();
void Func_020058e0();
void Func_020058e8();

                     
                               /* ROM dispatch[0] -> Func_080030f8 */

                     

                     

                     

                     

                     
/* Four register arguments plus two stack words. */

                     

                     

/* Byte-exact: (layer, x, z, width, height, value). */

s32 Func_02002500(void)
{
    s32 step;
    s32 releaseSlot;
    s32 x;
    s32 z;
    u8 *record;

    s32 permuted_12;
    *(s32 *)(Data_03001ebc + 448) = 516;      /* 129 << 2 */

    step = Data_02000240[225];

    if (Data_02000240[224] == 0x44) {
        if (step < 1) return 0;

        if (step <= 4) {
            if (Func_020052d0(0x89c) == 0) {
                Func_020052f0();
                Func_0200523e(1);
                Func_0200537e(10, 1);
                Func_0200535c(10, 184 << 15, 240 << 15);
                Func_020053a8(10, 0xd000, 0);        /* 208 << 8 */
                Func_02005342(0, 0x6666, 0x3333);
                Func_02005364(0, 136, 64);
                Func_02005420();
                Func_0200542c();
                Func_02005382(0);
                Func_02005338(30);
                Func_020053ec(10, 256, 0);
                Func_020053bc(10, 2);
                Func_02005352(30);
                Func_02003354(10, 136, 116, 0x70000);   /* 224 << 11 */
                Func_020033e2(10);
                Func_020053f6(10, 15);
                Func_0200533a(Func_02005394(10), 0);
                Func_02005378(0x89c);
                Func_02005386(60);
                Func_0200539a();
            }

            if (Func_02005380(0x109) == 0) return 0;
            if (Func_0200538e(768) != 0) return 0;      /* 192 << 2 */
            Func_02005434(10, 15);
            return 0;
            Func_02005412(10, 136 << 16, 232 << 15);
        }

        if (step > 9 || step < 7) return 0;

        /* steps 7-9 */
        record = Func_020053e2(0);
        if (record != 0) {
            Func_02005428(16, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        *(u32 *)(Func_020053f6_b(16) + 108) = 0;

        if (Func_020053d0(0x109) != 0) {
            *(s32 *)(Func_0200540a(16) + 12) = 0x200000;    /* 128 << 14 */
        }

        Func_02005346(1);
        Func_0200545c(16, 158 << 18, 220 << 17);

        if (Func_020053fa(0xfd4) == 0) Func_02005284(16);

        Func_020054a4(11, 15);
        Func_020054ac(12, 15);
        Func_020053f0(Func_0200544a(11), 0);
        Func_020053fc(Func_02005456(12), 0);
        Func_02002f66(8);

        if (Func_0200543a(784) == 0) {                       /* 196 << 2 */
            Func_02002f78(9);
            return 0;
        }

        Func_020053ac(1);
        Func_020054c2(9, 132 << 18, 204 << 17);
        Func_020054d2(9, 4);
        Func_02005436(38, 27, 4, 2, 31, 25);
        Func_020054ac_b(9)[35] = 2;
        return 0;
    }

    if (Data_02000240[224] == 0x45) {
        if (step < 3) return 0;

        if (step <= 6) {
            if (Func_020054b4(0x303) == 0) {
                Func_02005558(12, 15);
                Func_0200549c(Func_020054f6(12), 0);
            }
            if (Func_020054d4(772) != 0) return 0;           /* 193 << 2 */
            Func_0200557a(13, 15);
            /* The assembly jumps into the shared two-call tail at 0x02002962
             * with r0 already holding 13, so this is the SAME call site as the
             * one spelled below for slot 21, not a second one. */
            releaseSlot = 13;
            goto release;
        }

        if (step > 12 || step < 10) return 0;

        /* steps 10-12 */
        if (Func_020054ec(0x311) == 0) {
            Func_0200302a(10);
        } else {
            Func_0200545e(1);
            Func_02005574(10, 138 << 18, 255 << 17);
            Func_02005584(10, 4);
            Func_0200554a(10)[35] = 2;
            Func_020054f4(44, 30, 2, 4, 34, 30);
            Func_02002a00(0, 35, 29, 1, 4, 0);
        }

        Func_02003082(8);
        Func_02003088(9);

        permuted_12 = *(s32 *)(Func_0200558a(11) + 8) >> 20;
        z = *(s32 *)(Func_02005592(11) + 16) >> 20;
        x  = permuted_12;
        Func_02002a32(2, x, z, 1, 1, 0xff);

        Func_020054e0(1);
        Func_02005620(11, 6);
        Func_020055be(8)[89] |= 8;

        if (Func_0200559e(0x306) != 0) return 0;
        Func_02005644(14, 15);
        Func_02005588(Func_020055e2(14), 0);

        if (Func_020055be_b(0x305) == 0) return 0;
        Func_0200563a(14, 212 << 17, 240 << 17);
        Func_02005648(17, 212 << 17, 240 << 17);
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
        Func_020055d4(1);
        if (Func_0200567a(0x307) == 0) {
            Func_0200571e(15, 15);
            Func_02005662(Func_020056bc(15), 0);
            Func_0200566e(Func_020056c8(19), 0);
        }
        if (Func_020056a6(776) == 0) {                       /* 194 << 2 */
            Func_0200574a(16, 15);
            Func_0200568e(Func_020056e8(16), 0);
            Func_0200569a(Func_020056f4(20), 0);
        }
        if (Func_020056d0(0x309) != 0) return 0;
        Func_02005776(17, 15);
        Func_020056ba(Func_02005714(17), 0);
        releaseSlot = 21;
release:
        Func_020056c6(Func_02005720(releaseSlot), 0);
        return 0;

    case 7:
        x = *(s32 *)(Func_0200572e(13) + 8) >> 20;
        z = *(s32 *)(Func_02005736(13) + 16) >> 20;
        Func_02002bd6(2, x, z, 1, 1, 0xff);
        Func_020057be(13, 6);
        Func_0200568c(1);
        Func_02005762(8)[89] |= 8;
        Func_02003276(8);
        return 0;

    case 8:
    case 9:
    case 10:
    case 11:
        /* Both banners are placed at the same height, 185 << 17. */
        Func_02003414(0x02de0000, 0, 185 << 17, 223);
        Func_02003420(0x02f20000, 0, 185 << 17, 223);
        Func_0200329a(10);
        Func_020032a0(12);

        if (Func_02005772(0x312) == 0) {
            Func_020032b0(9);
        } else {
            Func_020056e4(1);
            Func_020057fc(9, 4);
            Func_02005800(9, 0x02ba0000, 199 << 17);
            Func_020057ce(9)[35] |= 2;
            Func_0200577c(26, 20, 2, 4, 42, 23);
            Func_020057cc(532);                              /* 133 << 2 */
            Func_0200583a(14, 158 << 18, 220 << 17);
            Func_020057ae(Func_02005808(14), 0);
        }

        if (Func_020057e4(0x313) == 0) {
            Func_02003322(11);
        } else {
            Func_02005756(1);
            Func_0200586e(11, 4);
            Func_02005870(11, 0x029a0000, 0x02260000);
            Func_0200583e(11)[35] = 2;
            Func_020057e8(26, 20, 2, 4, 40, 32);
        }

        x = *(s32 *)(Func_0200585e(14) + 8) >> 20;
        z = *(s32 *)(Func_02005866(14) + 16) >> 20;
        Func_02002d06(2, x, z, 1, 1, 0xff);
        Func_020058ee(14, 6);
        Func_020057bc(1);
        Func_02005892(9)[89] |= 8;

        if (Func_02005872(0x30b) == 0) {
            Func_02005916(18, 15);
            Func_0200585a(Func_020058b4(18), 0);
            if (Func_02005890(0x30a) != 0) {
                Func_0200590a(22, 186 << 18, 252 << 17);
                Func_02005918(18, 186 << 18, 252 << 17);
            }
        }

        Func_0200475c();
        return 0;

    case 12:
    case 13:
        Func_020058fe(18, Data_0200b084);
        if (Func_020058c4(0x893) == 0) return 0;
        if (Func_020058ce(0x89e) == 0) return 0;
        Func_020058e0(0x88f);
        return 0;

    case 15:
        Func_020058e8(0x89e);
        return 0;

    default:
        /* step 14: the table entry points straight at the common exit. */
        return 0;
    }
}
