#include "GROUP_DEPARTURE.H"

void ActorPresentation_SetEightSceneCells(void)
{
    s32 a = 15;
    s32 d = 0x35;
    s32 e;
    s32 b;
    s32 c;
    s32 f;

    Map_CopyCellAttributes(29, 23, 1, 1, a, d);
    b = 14;
    Map_CopyCellAttributes(29, 23, 1, 1, b, d);
    c = 13;
    Map_CopyCellAttributes(29, 23, 1, 1, c, d);
    Map_CopyCellAttributes(26, 20, 2, 1, b, 0x34);
    e = 0x36;
    Map_CopyCellAttributes(25, 21, 1, 1, c, e);
    Map_CopyCellAttributes(25, 21, 1, 1, a, e);
    Map_CopyCellAttributes(14, 0x35, 1, 1, b, e);
    f = 0x37;
    Map_CopyCellAttributes(13, 0x37, 1, 1, a, f);
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    if (GameFlag_IsSet(0x312) == 0) {
        Event_Begin();
        if (GameFlag_IsSet(0x832) == 0) {
            struct FieldActor *actor = Actor_Get(13);
            struct FieldActor *leader = Actor_Get(ACTOR_PARTY_LEADER);
            u16 priority = leader->sprite->priority;
            u8 flags = leader->priority_flags;

            Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
            Audio_PlayCue(141);
            Task_Wait(40);
            Audio_PlayCue(145);
            ObjectMotion_SetActionVariant(ACTOR_PARTY_LEADER, 3);
            Actor_Get(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
            Actor_SetPosition(13, 0, 0x2bf0000);
            actor->speed = 0x18000;
            actor->acceleration = 0x18000;
            actor->y.fixed += 0x500000;
            *(s32 *)((u8 *)actor + 0x3c) = actor->y.fixed;
            *(s32 *)((u8 *)actor + 0x44) = 0x8000;
            Actor_WalkToAndWait(13, 64, 0x2bf);
            Event_Wait(40);
            Audio_PlayCue(0x121);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Engine_MapWaitWorkValuesBelow256();
            BattleFx_PlayQueuedSound();
            GameFlag_Set(0x832);
            ObjectMotion_SetActionVariant(ACTOR_PARTY_LEADER, priority);
            Actor_Get(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
            leader->priority_flags = flags;
        }
        SceneState_ApplyFourRects();
        GameFlag_Set(0x312);
        if (GameFlag_IsSet(0x837) != 0) {
            if (GameFlag_IsSet(0x841) == 0) {
                if (GameFlag_IsSet(0x30c) == 0) {
                    if (Actor_Get(ACTOR_PARTY_LEADER)->z.fixed <= 0x2b4ffff) {
                        SceneActor_RunActor22PlacementSequence(62, 0x29d);
                        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 27, 0x273);
                    } else {
                        SceneActor_RunActor22PlacementSequence(75, 0x2cb);
                        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 67, 0x2f5);
                    }
                    GameFlag_Set(0x30c);
                }
            }
        }
        Event_End();
    }
}

void SceneState_ApplyFourRects(void)
{

    s32 a = 0x2a;
    s32 b;

    Map_CopyCellAttributes(29, 22, 1, 1, 3, a);
    b = 2;
    Map_CopyCellAttributes(29, 21, 1, 1, b, a);
    Map_CopyCellAttributes(29, 21, 1, 1, 4, a);
    Map_CopyCellAttributes(23, 20, 3, 1, b, 0x2b);
}
