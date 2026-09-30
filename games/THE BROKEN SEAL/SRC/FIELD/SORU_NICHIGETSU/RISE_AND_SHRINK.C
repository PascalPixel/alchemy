#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
extern u8 MsgSoruHa[];

/*
 * Exact 2026-09-23 (1,120 bytes), with two tagged fake matches for the
 * zero stores of the two presentations. Both rise-and-shrink loops of each
 * actor share one counter.
 */

void SetSolShindenActorStep();
s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Engine_ActorWalkToAndWait();
void Engine_ActorSetSpeed();
void Engine_ActorSetAnimation();
void Engine_ActorSetPosition();
void Engine_MapCopyCellsTo();
void Engine_EventWait();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_ActorFaceDirection();
void Map_ClearLayerEntryFlag();
void Engine_AudioPlayCue();
void Engine_ActorShowEmote();
void Object_LinkPair();
void Engine_TaskWait();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void ObjectGroup_ConfigureChildValue();
struct FieldActor *Engine_ActorGet();
void Engine_ActorJump();
void Engine_ActorRunRepeatedMotion();
void Engine_EventEnd();
void Engine_ColorBufferApplyTarget();
void Engine_EventRequestExit();
void Engine_EventCloseScreen();
void Engine_ColorBufferInterpolate();
void Engine_EventWaitForScreen();

void FieldScene_RunSanctumRiseAndShrink(void)
{
    u32 i;
    struct FieldActor *actor;
    s32 record;
    s32 base5_8010;
    s32 base5_0;
    struct FieldSprite *sprite;

    if (Engine_GameFlagIsSet(0x811) == 0) {
    } else {
        Engine_EventBegin();
        Call2(Engine_CameraSetSpeed, 0x10000, 0x2000);
        Call4(Engine_CameraMoveTo, 0x11f0000, -1, 0x940000, 1);
        Call3(Engine_ActorWalkToAndWait, 0, 0x120, 120);
        Engine_ActorSetAnimation(0, 0);
        Actor_Jump(ACTOR_PARTY_LEADER, 4, 30);
        Call3(Engine_ActorSetPosition, 16, 0x1200000, 0x780000);
        Call3(Engine_ActorSetSpeed, 16, 0x10000, 0x8000);
        Actor_WalkToAndWait(16, 0x114, 136);
        Actor_WalkTo(16, 0x108, 136);
        Call3(Engine_ActorWalkToAndWait, 0, 0x138, 136);
        Engine_ActorSetAnimation(0, 1);
        Engine_ActorSetAnimation(16, 1);
        Call3(Engine_ActorFaceDirection, 0, 0xb000, 0);
        Engine_ActorFaceDirection(16, 0xd000, 20);
        if (Value1(Engine_GameFlagIsSet, 0x819) == 0) {
            Engine_AudioPlayCue(220);
        }
        Engine_EventWait(40);
        if (GameFlag_IsSet(0x819) != 0) {
        } else {
            Map_CopyCellsTo(36, 62, 17, 36, 2, 3);
            Engine_MapCopyCellsTo(44, 59, 17, 38, 2, 1);
            Engine_EventWait(10);
            Engine_MapCopyCellsTo(38, 62, 17, 36, 2, 3);
            Engine_MapCopyCellsTo(44, 59, 17, 39, 2, 1);
            Engine_EventWait(10);
            Engine_MapCopyCellsTo(40, 62, 17, 36, 2, 3);
            Engine_MapCopyCellsTo(0, 32, 17, 39, 2, 1);
            Engine_MapCopyCellsTo(44, 59, 17, 40, 2, 1);
            Engine_EventWait(10);
            Call3(Engine_ActorShowEmote, 0, 0x100, 0);
            Call3(Engine_ActorShowEmote, 16, 0x100, 0);
            Engine_MapCopyCellsTo(42, 62, 17, 36, 2, 3);
            Engine_MapCopyCellsTo(0, 32, 17, 40, 2, 1);
            Engine_MapCopyCellsTo(44, 59, 17, 41, 2, 1);
            Event_Wait(10);
            Engine_MapCopyCellsTo(0, 32, 17, 41, 2, 1);
            Engine_MapCopyCellsTo(44, 59, 17, 42, 2, 1);
            Engine_EventWait(10);
            Map_CopyCellsTo(0, 32, 17, 42, 2, 3);
            Engine_EventWait(80);
            Map_ClearLayerEntryFlag(9);
            Map_ClearLayerEntryFlag(10);
            Engine_GameFlagSet(0x819);
        }
        Object_LinkPair(16, 0, 30);
        Engine_ActorSetAnimation(16, 3);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimation(16, 1);
        base5_8010 = 0x8010;
        Engine_ActorSetAnimation(0, 0);
        Engine_EventSetMessage((s32)MsgSoruHa);
        SetSolShindenActorStep(base5_8010, 6);
        Actor_SetAnimationAndWait(16, 3);
        Actor_SetAnimation(16, 1);
        Engine_EventShowMessage(base5_8010, 0);
        Engine_ActorSetAnimation(0, 3);
        Engine_EventWait(60);
        actor = Actor_Get(ACTOR_PARTY_LEADER);
        Engine_CameraSetSpeed(0x9999, 0x1333);
        Engine_CameraMoveTo(0x11f0000, -1, 0x720000, 1);
        Engine_ActorWalkToAndWait(0, 0x120, 120);
        Engine_EventWait(20);
        Call3(Engine_ActorFaceDirection, 0, 0xc000, 20);
        Call3(Engine_ActorSetSpeed, 0, 0x4ccc, 0x2666);
        actor->unknown_5a &= 254;
        /* FAKEMATCH: the zero goes through the sprite variable (r7). */
        sprite = 0;
        actor->motion_flags = (u32)sprite;
        Engine_AudioPlayCue(201);
        Call2(ObjectGroup_ConfigureChildValue, 0, 0x100);
        sprite = actor->sprite;
        base5_0 = 0;
        sprite->flags = 0;
        do {
            actor->y.fixed += 0x3333;
            Task_Wait(1);
            base5_0++;
        } while (base5_0 != 120);
        Engine_AudioPlayCue(190);
        base5_0 = 0;
        do {
            actor->y.fixed += 0x1999;
            sprite->scale += -0x400;
            Engine_TaskWait(1);
            base5_0++;
        } while (base5_0 != 60);
        Engine_ActorSetPosition(0, 0, 0);
        Engine_ActorJump(16, 4, 20);
        SetSolShindenActorStep(16, 6);
        Call3(Engine_ActorWalkToAndWait, 16, 0x120, 120);
        Engine_ActorRunRepeatedMotion(16, 2);
        Engine_EventWait(20);
        Call3(Engine_ActorFaceDirection, 16, 0xc000, 20);
        Engine_ActorSetSpeed(16, 0x4ccc, 0x2666);
        actor = Engine_ActorGet(16);
        /* FAKEMATCH: a mask temporary delays both byte stores past the zero. */
        {
            s32 m = 254;

            m &= actor->unknown_5a;
            base5_0 = 0;
            actor->unknown_5a = m;
            actor->motion_flags = base5_0;
        }
        Engine_AudioPlayCue(201);
        Call2(ObjectGroup_ConfigureChildValue, 16, 0x100);
        sprite = actor->sprite;
        sprite->flags = 0;
        do {
            actor->y.fixed += 0x3333;
            Engine_TaskWait(1);
            base5_0++;
        } while (base5_0 != 120);
        Engine_AudioPlayCue(190);
        base5_0 = 0;
        do {
            actor->y.fixed += 0x1999;
            sprite->scale += -0x400;
            Engine_TaskWait(1);
            base5_0++;
        } while (base5_0 != 60);
        Engine_ActorSetPosition(16, 0, 0);
        Engine_EventWait(80);
        gEventWork->start_transition = 0x203;
        gEventWork->transition_frames = 24;
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        Engine_ColorBufferApplyTarget(0, 0);
        Engine_ColorBufferInterpolate(1);
        Engine_TaskWait(1);
        Engine_EventRequestExit(7);
        Engine_EventEnd();
    }
}
