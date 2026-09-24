#include "TYPES.H"

void Engine_GameFlagClear();
s32 Engine_GameFlagIsSet();
void Engine_ActorSetPosition();
void ActorPresentation_SetTwoSceneCells();
void Engine_ActorSetSpritePriority();
s32 Engine_ActorGet();
void Engine_ActorSetSpriteFlags();
void Main_08009188();
void Engine_ActorSetAnimation();
void FieldScene_RunPaletteRampSequence();
void Engine_GameFlagSet();
void FieldScene_RunComplexActorSequence();
void FieldScene_RunSupplementalSequenceOne();
void Main_0808a2c8();
void Main_0808a2d8();
void Engine_TaskWait();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Main_0808a2e0();
void Engine_MapRedraw();


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

s32 HaidiaBabi_RestoreEntryState(void)
{
    u32 i;
    s32 record;
    s32 base5_3001ebc;

    if (Data_02000240_t[225][0] == 19) {
        Call1(Engine_GameFlagClear, 0x12f);
        *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x209;
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x834) != 0) {
            Engine_ActorSetPosition(11, 0, 0);
            Engine_ActorSetPosition(12, 0, 0);
            Engine_ActorSetPosition(13, 0, 0);
            Engine_ActorSetPosition(14, 0, 0);
            Engine_ActorSetPosition(15, 0, 0);
            Engine_ActorSetPosition(16, 0, 0);
        } else {
            ActorPresentation_SetTwoSceneCells();
        }
        Engine_ActorSetSpritePriority(13, 1);
        if (Value1(Engine_GameFlagIsSet, 0x87a) != 0) {
            record = Engine_ActorGet(17);
            Engine_ActorSetSpriteFlags(record, 0);
            if (Data_02000240_t[225][0] != 6 && Data_02000240_t[225][0] != 7) {
                goto L_02000550;
            }
            if (Value1(Engine_GameFlagIsSet, 0x109) != 0) {
                record = Value1(Engine_GameFlagIsSet, 0x203);
                if (record == 0) {
                    goto L_02000550;
                }
                Main_08009188(12);
                goto L_02000550;
            }
            Main_08009188(11);
            record = Engine_ActorGet(8);
            Engine_ActorSetSpriteFlags(record, 0);
            Engine_ActorSetAnimation(8, 10);
        } else {
            if (Data_02000240_t[225][0] == 21) {
                FieldScene_RunPaletteRampSequence();
            } else {
                if (Data_02000240_t[225][0] == 20) {
                    Call1(Engine_GameFlagSet, 0x834);
                    FieldScene_RunComplexActorSequence();
                } else {
                    if (Data_02000240_t[225][0] == 22) {
                        FieldScene_RunSupplementalSequenceOne();
                    } else {
                        base5_3001ebc = 0x3001ebc;
                        *(s32 *)((*(s32 *)base5_3001ebc + 0x1c0)) = 0x209;
                        if (Value1(Engine_GameFlagIsSet, 0x834) != 0) {
                            Main_0808a2c8();
                            {
                                u16 *target = (u16 *)((*(s32 *)(base5_3001ebc + 12) + 0x1f84));
                                s32 shown = 1;
                            
                                *target = shown;
                            }
                            Main_0808a2d8();
                            Engine_TaskWait(30);
                            Engine_EventOpenScreen();
                            Engine_EventWaitForScreen();
                            Main_0808a2e0();
                        } else {
                            Engine_MapRedraw();
                            Engine_TaskWait(1);
                        }
                    }
                }
            }
        }
    }
    L_02000550:;
    return 0;
}
