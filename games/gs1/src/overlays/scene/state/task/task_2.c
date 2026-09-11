#include "types.h"
#include "scene.h"

/* overlays/scene/state/task/initialize_mode_task.c */
/*
 * Mode task setup for resource_3bb.  It seeds a second per-instance task's
 * state and picks which of five addresses that task dispatches through.  The
 * 144-byte owner includes its 15-word literal pool.
 */

extern u16 gOv;
extern u16 gOv2;
extern u16 gOv3;
extern s32 gOv4;
extern u16 gOv5;
extern u16 gOv6;
extern s32 gOv7;

extern s32 gOv8;   /* Default handler. */
extern s32 gOv9;   /* Handler for mode 2. */
extern s32 gOv10;   /* Handler for mode 4. */
extern s32 gOv11;   /* Handler for mode 3 with param set. */
extern s32 gOv12;   /* Handler for mode 3 with param zero. */

/*
 * 0x0200a451 is the installed callback, a routine in this overlay.  The branch
 * chain picks one of five addresses by mode, consulting param only when mode is
 * 3; whether those five are code or per-mode records is not resolved, so they
 * are typed as plain addresses.  The four stores that follow reset the rest of
 * the task's state block, three as halfwords and one as a word.
 */
void State_Run(u32 mode, u32 param)
{
    s32 handler;

    gOv = (u16)mode;
    gOv2 = (u16)(param << 4);

    {
        s32 budget = 0xc80;
        s32 task = 0x0200a451;
        State_Apply(task, budget);
    }

    handler = (s32)&gOv8;
    if (mode == 2) {
        handler = (s32)&gOv9;
    }
    if (mode == 4) {
        handler = (s32)&gOv10;
    }
    if (mode == 3) {
        if (param != 0) {
            handler = (s32)&gOv11;
        } else {
            handler = (s32)&gOv12;
        }
    }

    gOv3 = 0;
    gOv4 = handler;
    gOv5 = 0;
    gOv6 = 0;
    gOv7 = 0;
}

/* overlays/scene/state/task/run_scripted_transition.c */
/*
 * resource_3bb scripted transition owner at 0x020029d0, 268 bytes including
 * alignment and its three-word pool.  Mode zero is the short opening; every
 * other mode runs the complete multi-stage transition and publishes flag
 * 0x123 when it closes.
 *
 * Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including every repeated State_Run,
 * Audio_PlayCue, State_unk2_4, State_unk3_4, State_unk4_4/360/370/020
 * call) -- declared/named as the literal per-site targets, not the shared
 * ultimate-destination symbol.
 */

extern void State_unk5_4(void);              /* State_unk4_4 veneer #1 (mode==0 arm) */
extern void State_unk6_4(void);              /* State_unk7_4 veneer #1 */
extern void State_unk8_4(void);              /* State_unk9_4 veneer #1 */
extern void State_unk10_4(s32 frames);        /* State_Run veneer #1 */
extern void State_unk11_4(s32 cue);           /* Audio_PlayCue veneer #1 */
extern void State_unk12_4(s32 mode);          /* State_unk2_4 veneer #1 */
extern void State_unk13_4(s32 style, s32 variant); /* State_unk3_4 veneer #1 */
extern void State_unk14_4(s32 frames);        /* State_Run veneer #2 */
extern void State_unk15_4(void);              /* State_unk16_4 veneer #1 */

extern void State_unk17_4(s32 cue);           /* Audio_PlayCue veneer #2 (main arm) */
extern void State_unk18_4(void);              /* State_unk4_4 veneer #2 */
extern void State_unk19_4(void);              /* State_unk7_4 veneer #2 */
extern void State_unk20_4(void);              /* State_unk9_4 veneer #2 */
extern void State_unk21_4(s32 frames);        /* State_Run veneer #3 */
extern void State_unk22_4(s32 cue);           /* Audio_PlayCue veneer #3 */
extern void State_unk23_4(s32 mode);          /* State_unk2_4 veneer #2 */
extern void State_unk24_4(s32 style, s32 variant); /* State_unk3_4 veneer #2 */
extern void State_unk25_4(s32 frames);        /* State_Run veneer #4 */

extern void State_unk26_4(s32 frames);        /* State_unk27_4 veneer (loop body) */
extern s32 State_unk28_4(void);               /* State_unk29_4 veneer (loop check) */

extern void State_unk30_4(s32 cue);           /* Audio_PlayCue veneer #4 */
extern void State_unk31_4(s32 mode);          /* State_unk2_4 veneer #3 */
extern void State_unk32_4(s32 style, s32 variant); /* State_unk3_4 veneer #3 */
extern void State_unk33_4(s32 cue);           /* Audio_PlayCue veneer #5 */
extern void State_unk34_4(s32 frames);        /* State_Run veneer #5 */
extern void State_unk35_4(s32 style, s32 variant); /* State_unk3_4 veneer #4 */
extern void State_unk36_4(s32 cue);           /* Audio_PlayCue veneer #6 */
extern void State_unk37_4(s32 frames);        /* State_Run veneer #6 */
extern void State_unk38_4(s32 mode);          /* State_unk2_4 veneer #4 */
extern void State_unk39_4(s32 style, s32 variant); /* State_unk3_4 veneer #5 */
extern void State_unk40_4(s32 cue);           /* Audio_PlayCue veneer #7 */
extern void State_unk41_3(s32 frames);        /* State_Run veneer #7 */
extern void State_unk42_3(s32 mode);          /* State_unk2_4 veneer #5 */
extern void State_unk43_3(s32 style, s32 variant); /* State_unk3_4 veneer #6 */
extern void State_unk44_3(s32 cue);           /* Audio_PlayCue veneer #8 */
extern void State_unk45_3(void);              /* State_unk46_3 veneer */
extern void State_unk47_3(void);              /* State_unk16_4 veneer #2 */
extern void State_unk48_3(s32 flag);          /* GameFlag_Set veneer */

        State_unk6_4();
        State_unk8_4();
        State_unk10_4(30);
        State_unk11_4(0x59);
        State_unk12_4(0);
        State_unk13_4(1, 0);
        State_unk14_4(120);
        State_unk15_4();
        return;
    }

    State_unk17_4(0xf7);
    State_unk18_4();
    State_unk19_4();
    State_unk20_4();
    {
        s16 *base = (s16 *)0x0200ca1e;
        *(s16 *)((u8 *)base + 30) = (s16)(mode * 60);
    }
    State_unk21_4(30);
    State_unk22_4(mode + 0x5a);
    State_unk23_4(mode);
    State_unk24_4(1, 0);
    State_unk25_4(120);

    goto check_transition;
wait_transition:
    State_unk26_4(1);
check_transition:
    if (State_unk28_4() != 0)
        goto wait_transition;

    State_unk30_4(0x121);
    State_unk31_4(5);
    State_unk32_4(2, 0);
    State_unk33_4(0xec);
    State_unk34_4(60);
    State_unk35_4(2, 1);
    State_unk36_4(0xec);
    State_unk37_4(60);
    State_unk38_4(6);
    State_unk39_4(2, 0);
    State_unk40_4(0xec);
    State_unk41_3(60);
    State_unk42_3(7);
    State_unk43_3(4, 0);
    State_unk44_3(0xed);
    State_unk45_3();
    State_unk47_3();
    State_unk48_3(0x123);
}

/* overlays/scene/state/task/run_scene_late_sequence.c */
extern u8 gVal[];

/* The two mode records the entry point seeds; the halfword at +26 holds the
 * per-mode span in sixtieths. */
struct ModeRecord {
    u8 pad[26];
    u16 span;
};
extern struct ModeRecord gOv;
extern struct ModeRecord gOv2;
extern u8 gCell[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gWork[];
extern u8 gCell2[][2];

u8 *State_unk2_4();

u8 *State_unk3_4();

void Scene_RunLateSequence(s32 a0)
{
    s32 kind;

    State_unk4_4(247);
    State_unk5_4();
    State_unk6_4();
    gOv.span = a0 * 60;
    gOv2.span = (a0 < 0 ? -a0 : a0) * 60;
    if (a0 < 0) {
        State_unk7_4(30);
        State_unk8_4(86);
        State_unk9_4(8);
        State_Check(3, 1);
        State_unk10_4(-a0 * 60 + 60);
        kind = 0;
    } else {
        State_unk11_4(30);
        State_unk12_4(a0 + 90);
        State_unk13_4(4);
        State_unk2(3, 0);
        State_unk14_4(a0 * 60 + 60);
        kind = 8;
    }
    State_Place(kind, 0x105, 0);
    while (State_Run()!= 0) {
        State_unk15_4(1);
    }
    State_unk16_4(19);
    State_unk17_4(30);
    State_Do(0x121);
    State_unk18_4();
    State_unk19_4();
}

void State_unk20_4(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p8;
    s32 p8b;
    u8 *p9;
    s32 p9b;
    u8 *rec;
    s32 record;
    s32 v5;
    s32 base6_4000208;
    s32 v1;
    s32 base4_2002090;
    s32 v4;
    s32 v0;
    s32 slot0;
    u8 *p6;

    p6 = *(volatile s32 *)0x03001e68;
    p11 = a0;
    p8 = a1;
    p10 = a2;
    rec = State_unk2_4();
    p6[6] = 1;
    p6[7] = 4;
    *(volatile s32 *)gOv6 = *(volatile s32 *)((s32)rec + 8);
    *(volatile s32 *)gOv4 = *(volatile s32 *)((s32)rec + 16);
    p9 = *(volatile s32 *)((s32)rec + 80);
    *(volatile s32 *)gOv5 = *(volatile u16 *)((s32)rec + 6);
    v5 = 1;
    State_unk21_4(p11, 2);
    {
        u8 value = *(volatile u8 *)&rec[35];

        rec[35] = (u8)(value | v5);
    }
    {
        s32 shown = 0x4000;

        *(volatile u16 *)((s32)rec + 6) = shown;
    }
    State_unk22_4((s32)rec, 3);
    State_unk23_4((s32)rec, 0);
    State_unk24_4((s32)rec, 1);
    p10b = ((s32)p10 << 16);
    p8b = ((s32)p8 << 16);
    State_unk25_4(p11, p8b, p10b);
    State_unk26_4(0, 0x4000, 0);
    base6_4000208 = 0x4000208;
    base4_2002090 = (s32)gOv3;
    v1 = *(volatile u16 *)base6_4000208;
    *(volatile u16 *)base6_4000208 = base6_4000208;
    if (*(volatile u16 *)base4_2002090 <= 31) {
        *(volatile u16 *)base4_2002090 += 1;
        *(volatile s32 *)((((((*(volatile u16 *)base4_2002090 << 1) + *(volatile u16 *)base4_2002090) << 2) + base4_2002090) + 4)) = 0xf00;
        *(volatile s32 *)(((((((*(volatile u16 *)base4_2002090 << 1) + *(volatile u16 *)base4_2002090) << 2) + base4_2002090) + 4) + 4)) = 0x4000050;
        *(volatile s32 *)(((((((*(volatile u16 *)base4_2002090 << 1) + *(volatile u16 *)base4_2002090) << 2) + base4_2002090) + 4) + 4) + 4) = 0x20000;
    }
    *(volatile u16 *)base6_4000208 = v1;
    p9[5] = ((s32)(-13 & p9[5]) | 4);
    p9[17] = ((s32)(-13 & p9[17]) | 4);
    slot0 = base4_2002090;
    v5 = 0;
    State_unk27_4(252);
    v4 = slot0;
    do {
        *(volatile s32 *)((s32)rec + 24) = ((v5 << 12) + 0x1000);
        *(volatile s32 *)((s32)rec + 28) = (0x1f000 - (v5 << 12));
        v0 = *(volatile u16 *)base6_4000208;
        *(volatile u16 *)base6_4000208 = base6_4000208;
        if (*(volatile u16 *)(v4) <= 31) {
            *(volatile u16 *)(v4) += 1;
            *(volatile s32 *)(((v4 + (((*(volatile u16 *)(v4) << 1) + *(volatile u16 *)(v4)) << 2)) + 4)) = (((15 - v5) << 8) | (v5 + 1));
            *(volatile s32 *)((((v4 + (((*(volatile u16 *)(v4) << 1) + *(volatile u16 *)(v4)) << 2)) + 4) + 4)) = 0x4000052;
            *(volatile s32 *)((((v4 + (((*(volatile u16 *)(v4) << 1) + *(volatile u16 *)(v4)) << 2)) + 4) + 4) + 4) = 0x20000;
        }
        *(volatile u16 *)base6_4000208 = v0;
        slot0 = v4;
        State_unk28_4(1);
        v5 = (v5 + 2);
        v4 = slot0;
    } while (v5 <= 15);
    v4 = *(volatile u16 *)0x04000208;
    *(volatile u16 *)0x04000208 = 0x4000208;
    if (*(volatile u16 *)gOv3 <= 31) {
        *(volatile u16 *)gOv3 += 1;
        *(volatile s32 *)((((((*(volatile u16 *)gOv3 << 1) + *(volatile u16 *)gOv3) << 2) + 0x2002090) + 4)) = 16;
        *(volatile s32 *)(((((((*(volatile u16 *)gOv3 << 1) + *(volatile u16 *)gOv3) << 2) + 0x2002090) + 4) + 4)) = 0x4000052;
        *(volatile s32 *)(((((((*(volatile u16 *)gOv3 << 1) + *(volatile u16 *)gOv3) << 2) + 0x2002090) + 4) + 4) + 4) = 0x20000;
    }
    *(volatile u16 *)0x04000208 = v4;
    *(volatile s32 *)((s32)rec + 24) = 0x11000;
    *(volatile s32 *)((s32)rec + 28) = 0xf000;
    State_unk29_4(1);
    *(volatile s32 *)((s32)rec + 24) = 0x10000;
    *(volatile s32 *)((s32)rec + 28) = 0x10000;
    State_unk30_4(13);
    p9[5] &= -13;
    p9[17] &= -13;
    State_unk31_4(p11, 3);
    State_unk32_4(20);
    p9b = v5;
}

void State_unk33_4(s32 a0)
{
    u32 i;
    u8 *rec7;
    s32 record;
    u8 *p7;

    p7 = *(volatile s32 *)0x03001e68;
    rec7 = State_unk3_4();
    if (gCell2[249][0] == 1) {
        gCell2[249][0] = 0;
        State_unk34_4(a0, 1);
    } else {
        State_unk2_3(a0, 0x4000, 30);
        State_unk35_4(a0, 3);
        State_unk36_4(30);
    }
    p7[7] = 0;
    p7[6] = 15;
    *(volatile s32 *)((s32)rec7 + 8) = *(volatile s32 *)gOv6;
    *(volatile s32 *)((s32)rec7 + 16) = *(volatile s32 *)gOv4;
    *(volatile u16 *)((s32)rec7 + 6) = *(volatile s32 *)gOv5;
    *(volatile s32 *)((s32)rec7 + 56) = -0x80000000;
    *(volatile s32 *)((s32)rec7 + 64) = -0x80000000;
    *(volatile s32 *)((s32)rec7 + 36) = 0;
    *(volatile s32 *)((s32)rec7 + 44) = 0;
    rec7[85] = 3;
    rec7[34] = (s32)gVal;
    *(volatile s32 *)((s32)rec7 + 12) = 0;
    *(volatile s32 *)((s32)rec7 + 20) = 0;
    State_unk37_4((s32)rec7, 1);
    State_unk38_4((s32)rec7, 0);
    State_unk39_4((s32)rec7, 1);
    State_unk8_4(1);
}
