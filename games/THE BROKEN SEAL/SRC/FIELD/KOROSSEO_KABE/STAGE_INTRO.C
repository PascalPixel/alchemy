#include "TYPES.H"

void Local_02001df4();
void Engine_EventBegin();
s32 run_state_interaction();
void Engine_EventSetMessage();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventShowMessage();
s32 Local_02002ba8();
void Engine_ActorSetSpeed();
s32 SceneActor_PlaceWithScale14000();
void Engine_EventWait();
s32 Engine_ActorGet();
void Engine_ActorSetAnimation();
void Engine_ActorSetAttachedEffect();
void Korosseo_RestoreCompetitor();
void Engine_CameraFollowActor();
void Local_020020b8();
s32 FieldScene_RunMiddleSequence();
void Engine_EventEnd();


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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Colosso wall stage: unless the stage is already cleared, walk the player
 * along the wall, show the introduction and hand over to the stage. */
void KorosseoKabe_RunStageIntro(s32 a0)
{
    s32 rec;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Local_02001df4();
    } else {
        Engine_EventBegin();
        rec = Value2(run_state_interaction, a0, 1);
        if (rec == 0) {
            Call1(Engine_EventSetMessage, 0x209e);
            Call2(Engine_CameraSetSpeed, 0x30000, 0x6000);
            Call4(Engine_CameraMoveTo, 0x4c80000, -1, 0xb80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventShowMessage(a0, 0);
            Value3(Local_02002ba8, 0, 0x4f8, 168);
            Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
            SceneActor_PlaceWithScale14000(0, 0x508, 184);
            SceneActor_PlaceWithScale14000(0, 0x508, 216);
            SceneActor_PlaceWithScale14000(0, 0x4c8, 216);
            Engine_EventShowMessage(a0, 0);
            SceneActor_PlaceWithScale14000(0, 0x4c8, 248);
            Call3(SceneActor_PlaceWithScale14000, 0, 0x4a8, 248);
            Engine_EventWait(3);
            record = Engine_ActorGet(0);
            *(s32 *)(record + 40) = 0x40000;
            Engine_ActorSetAnimation(0, 28);
            Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
            Engine_EventWait(30);
            Engine_EventShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(0, 0);
            Local_020020b8(a0, 1);
        } else {
            if (rec == 1) {
                Call1(Engine_EventSetMessage, 0x209d);
                Engine_EventShowMessage(a0, 0);
            }
        }
        Value3(FieldScene_RunMiddleSequence, rec, a0, 1);
        Engine_EventEnd();
    }
}
