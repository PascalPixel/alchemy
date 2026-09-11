#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/task/run_scripted_transition.h"

/*
 * resource_3bb scripted transition owner at 0x020029d0, 268 bytes including
 * alignment and its three-word pool.  Mode zero is the short opening; every
 * other mode runs the complete multi-stage transition and publishes flag
 * 0x123 when it closes.
 *
 * Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including every repeated State_Run,
 * Audio_PlayCue, State_Run2, State_Run3, State_Run4/360/370/020
 * call) -- declared/named as the literal per-site targets, not the shared
 * ultimate-destination symbol.
 */

extern void State_Run5(void);              /* State_Run4 veneer #1 (mode==0 arm) */
extern void State_Run6(void);              /* State_Run7 veneer #1 */
extern void State_Run8(void);              /* State_Run9 veneer #1 */
extern void State_Run10(s32 frames);        /* State_Run veneer #1 */
extern void State_Run11(s32 cue);           /* Audio_PlayCue veneer #1 */
extern void State_Run12(s32 mode);          /* State_Run2 veneer #1 */
extern void State_Run13(s32 style, s32 variant); /* State_Run3 veneer #1 */
extern void State_Run14(s32 frames);        /* State_Run veneer #2 */
extern void State_Run15(void);              /* State_Run16 veneer #1 */

extern void State_Run17(s32 cue);           /* Audio_PlayCue veneer #2 (main arm) */
extern void State_Run18(void);              /* State_Run4 veneer #2 */
extern void State_Run19(void);              /* State_Run7 veneer #2 */
extern void State_Run20(void);              /* State_Run9 veneer #2 */
extern void State_Run21(s32 frames);        /* State_Run veneer #3 */
extern void State_Run22(s32 cue);           /* Audio_PlayCue veneer #3 */
extern void State_Run23(s32 mode);          /* State_Run2 veneer #2 */
extern void State_Run24(s32 style, s32 variant); /* State_Run3 veneer #2 */
extern void State_Run25(s32 frames);        /* State_Run veneer #4 */

extern void State_Run26(s32 frames);        /* State_Run27 veneer (loop body) */
extern s32 State_Run28(void);               /* State_Run29 veneer (loop check) */

extern void State_Run30(s32 cue);           /* Audio_PlayCue veneer #4 */
extern void State_Run31(s32 mode);          /* State_Run2 veneer #3 */
extern void State_Run32(s32 style, s32 variant); /* State_Run3 veneer #3 */
extern void State_Run33(s32 cue);           /* Audio_PlayCue veneer #5 */
extern void State_Run34(s32 frames);        /* State_Run veneer #5 */
extern void State_Run35(s32 style, s32 variant); /* State_Run3 veneer #4 */
extern void State_Run36(s32 cue);           /* Audio_PlayCue veneer #6 */
extern void State_Run37(s32 frames);        /* State_Run veneer #6 */
extern void State_Run38(s32 mode);          /* State_Run2 veneer #4 */
extern void State_Run39(s32 style, s32 variant); /* State_Run3 veneer #5 */
extern void State_Run40(s32 cue);           /* Audio_PlayCue veneer #7 */
extern void State_Run41(s32 frames);        /* State_Run veneer #7 */
extern void State_Run42(s32 mode);          /* State_Run2 veneer #5 */
extern void State_Run43(s32 style, s32 variant); /* State_Run3 veneer #6 */
extern void State_Run44(s32 cue);           /* Audio_PlayCue veneer #8 */
extern void State_Run45(void);              /* State_Run46 veneer */
extern void State_Run47(void);              /* State_Run16 veneer #2 */
extern void State_Run48(s32 flag);          /* GameFlag_Set veneer */

        State_Run6();
        State_Run8();
        State_Run10(30);
        State_Run11(0x59);
        State_Run12(0);
        State_Run13(1, 0);
        State_Run14(120);
        State_Run15();
        return;
    }

    State_Run17(0xf7);
    State_Run18();
    State_Run19();
    State_Run20();
    {
        s16 *base = (s16 *)0x0200ca1e;
        *(s16 *)((u8 *)base + 30) = (s16)(mode * 60);
    }
    State_Run21(30);
    State_Run22(mode + 0x5a);
    State_Run23(mode);
    State_Run24(1, 0);
    State_Run25(120);

    goto check_transition;
wait_transition:
    State_Run26(1);
check_transition:
    if (State_Run28() != 0)
        goto wait_transition;

    State_Run30(0x121);
    State_Run31(5);
    State_Run32(2, 0);
    State_Run33(0xec);
    State_Run34(60);
    State_Run35(2, 1);
    State_Run36(0xec);
    State_Run37(60);
    State_Run38(6);
    State_Run39(2, 0);
    State_Run40(0xec);
    State_Run41(60);
    State_Run42(7);
    State_Run43(4, 0);
    State_Run44(0xed);
    State_Run45();
    State_Run47();
    State_Run48(0x123);
}
