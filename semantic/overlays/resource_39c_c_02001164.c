typedef signed int s32;

/*
 * resource_39c placement dispatcher at 0x02001164
 * (476-byte whole-owner span, 30 static call sites).
 *
 * The 462-byte body ends at 0x02001332.  Alignment and three flag/effect pool
 * words extend the owner to the next prologue at 0x02001340.  It consumes the
 * reviewed six-word placement probe, dispatches actor slots 9, 10 and 11 by
 * their relevant grid coordinate, then runs the exact-C 0x020010c0 follow-up
 * on every successful probe.  A failed probe skips that follow-up but still
 * reaches the shared scene close.
 */

typedef struct PlacementResult_02001164 {
    s32 deltaIndex;
    s32 actorSlot;
    s32 x;
    s32 y;
    s32 z;
    void (*callback)(void);
} PlacementResult_02001164;

extern void Func_0808a018(void);
extern s32 Func_02000758(PlacementResult_02001164 *result);
extern void Func_020008ec(s32 deltaIndex, s32 actorSlot,
                          s32 x, s32 y, s32 z,
                          void (*callback)(void));
extern void Func_0808a010(s32 frames);
extern void Func_08009180(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_02003c48(s32 x, s32 y, s32 z, s32 scale);
extern void Func_080770c8(s32 flag);
extern void Func_080770d0(s32 flag);
extern s32 Func_080770c0(s32 flag);
extern void Func_020010c0(void);
extern void Func_0808a020(void);

void Func_02001164(void)
{
    PlacementResult_02001164 placement;

    Func_0808a018();

    if (Func_02000758(&placement) != 0) {
        if (placement.actorSlot == 9) {
            if ((placement.z >> 20) == 8) {
                Func_020008ec(placement.deltaIndex, 9,
                              placement.x, placement.y, placement.z,
                              placement.callback);
                Func_0808a010(20);
                Func_08009180(119, 9, 109, 11, 1, 1);
                Func_02003c48(0x02d60000, 0, 0xb40000, 0x8000);
                Func_080770c8(0x310);
            } else {
                Func_08009180(117, 9, 104, 7, 1, 1);
                Func_08009180(119, 8, 109, 11, 1, 1);
                Func_08009180(118, 8, 104, 13, 1, 1);
                Func_020008ec(placement.deltaIndex, placement.actorSlot,
                              placement.x, placement.y, placement.z,
                              placement.callback);
                Func_080770d0(0x310);
            }
        } else if (placement.actorSlot == 10) {
            if ((placement.z >> 20) == 12) {
                Func_020008ec(placement.deltaIndex, 10,
                              placement.x, placement.y, placement.z,
                              placement.callback);
                Func_0808a010(10);

                if (Func_080770c0(0x310) != 0) {
                    Func_08009180(118, 9, 104, 13, 1, 1);
                    Func_02003c48(0x02840000, 0, 0xd20000, 0x4000);
                }
                Func_080770c8(0x311);
            } else {
                Func_08009180(119, 8, 109, 11, 1, 1);
                if (Func_080770c0(0x310) != 0) {
                    Func_08009180(119, 9, 109, 11, 1, 1);
                    Func_08009180(118, 8, 104, 13, 1, 1);
                }
                Func_020008ec(placement.deltaIndex, placement.actorSlot,
                              placement.x, placement.y, placement.z,
                              placement.callback);
                Func_080770d0(0x311);
            }
        } else if (placement.actorSlot == 11) {
            if ((placement.x >> 20) == 40) {
                Func_020008ec(placement.deltaIndex, 11,
                              placement.x, placement.y, placement.z,
                              placement.callback);
                Func_080770c8(0x312);
            } else {
                Func_020008ec(placement.deltaIndex, 11,
                              placement.x, placement.y, placement.z,
                              placement.callback);
                Func_080770d0(0x312);
            }
        }

        Func_020010c0();
    }

    Func_0808a020();
}
