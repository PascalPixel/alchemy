#include "TYPES.H"

void Engine_MapClearLayerEntryFlag();
void Engine_GameFlagSet();
s32 Engine_GameFlagIsSet();
s32 Engine_GameFlagClear();
void FieldScene_RunLongBranchingChoreography();
void FieldScene_RunScene38d_0200179c();
void RunEventScript02();
void Engine_MapCopyCellAttributes();
void Engine_ActorSetPosition();
s32 Engine_ActorGet();
void Engine_ActorSetSpriteFlags();
void Local_020021bc();


extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* McCoy's Palace entry: record the arrival, clear two layer flags and, by
 * the story flags and the entrance, run the matching scene or restore the
 * room (the four guards at row 0x2d8, the opened doorway cells). */
s32 BiribinoKyuden_ApplyEntryState(s32 a0, s32 a1)
{
    u32 i;
    s32 rec8;
    s32 record;

    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x209;
    Engine_MapClearLayerEntryFlag(1);
    Engine_MapClearLayerEntryFlag(2);
    Call1(Engine_GameFlagSet, 0x84b);
    if (Value1(Engine_GameFlagIsSet, 0x109) != 0) {
        Call1(Engine_GameFlagClear, 0x200);
    }
    if (Value1(Engine_GameFlagIsSet, 0x84f) == 0) {
        record = Value1(Engine_GameFlagIsSet, 0x845);
        if (record != 0) {
            goto L_02000758;
        }
        if (Data_02000240_t[225][0] == 29) {
            FieldScene_RunLongBranchingChoreography();
            goto L_02000888;
        }
        if (Data_02000240_t[225][0] != 9) {
            goto L_02000888;
        }
        if (Value1(Engine_GameFlagIsSet, 0x321) == 0) {
            goto L_02000888;
        }
        FieldScene_RunScene38d_0200179c();
    } else {
        L_02000758:;
        rec8 = Value1(Engine_GameFlagIsSet, 0x84e);
        if (rec8 != 0) {
        } else {
            if (Data_02000240_t[225][0] == 29) {
                if (Value1(Engine_GameFlagIsSet, 0x85e) != 0) {
                    goto L_02000888;
                }
                record = Value1(Engine_GameFlagIsSet, 0x845);
                if (record == 0) {
                    goto L_02000888;
                }
                RunEventScript02();
            } else {
                if (Data_02000240_t[225][0] == 28) {
                    if (Value1(Engine_GameFlagIsSet, 0x322) != 0) {
                        if (Value1(Engine_GameFlagIsSet, 0x109) != 0) {
                            Call6(Engine_MapCopyCellAttributes, 38, 55, 4, 1, 38, 45);
                            Call6(Engine_MapCopyCellAttributes, 42, 55, 4, 1, 38, 46);
                            Call3(Engine_ActorSetPosition, 21, 0x2680000, 0x2d80000);
                            Call3(Engine_ActorSetPosition, 22, 0x2780000, 0x2d80000);
                            Call3(Engine_ActorSetPosition, 23, 0x2880000, 0x2d80000);
                            Call3(Engine_ActorSetPosition, 24, 0x2980000, 0x2d80000);
                            record = Engine_ActorGet(21);
                            Engine_ActorSetSpriteFlags(record, 0);
                            record = Engine_ActorGet(22);
                            Engine_ActorSetSpriteFlags(record, 0);
                            record = Engine_ActorGet(23);
                            Engine_ActorSetSpriteFlags(record, 0);
                            record = Engine_ActorGet(24);
                            Engine_ActorSetSpriteFlags(record, 0);
                            *(u8 *)(Engine_ActorGet(21) + 85) = rec8;
                            *(u8 *)(Engine_ActorGet(22) + 85) = rec8;
                            *(u8 *)(Engine_ActorGet(23) + 85) = rec8;
                            *(u8 *)(Engine_ActorGet(24) + 85) = rec8;
                            record = Engine_ActorGet(21);
                            *(s32 *)(record + 12) = -0x40000;
                            record = Value1(Engine_ActorGet, 22);
                            *(s32 *)(record + 12) = -0x40000;
                            record = Value1(Engine_ActorGet, 23);
                            *(s32 *)(record + 12) = -0x40000;
                            record = Value1(Engine_ActorGet, 24);
                            *(s32 *)(record + 12) = -0x40000;
                        } else {
                            Local_020021bc();
                        }
                    }
                }
            }
        }
    }
    L_02000888:;
    return 0;
}
