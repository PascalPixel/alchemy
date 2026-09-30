/* The Suhara desert: the selected actor's progress and the middle steps. */
#include "SABAKU.H"

void SuharaSabaku_SyncSelectedActorProgress(void)
{
    struct Actor_02000400 *actor;
    struct SceneWork_02000400 *scene;
    s32 progress;

    actor = Actor_Get(((struct Selection_02000400 *)((s16 *)&gGameState))->actor_id);
    scene = *(struct SceneWork_02000400 **)((u8 *)&gEventWork);
    actor->presentation = (u16)(gFrameCount << 12);

    progress = GameFlag_GetByte(0x210);
    if (progress != 0) {
        if (progress == 1) {
            scene->state_one_marker = 99;
        } else if (GameFlag_IsSet(0x106) == 0) {
            progress -= 1;
        }
    }
    GameFlag_SetByte(0x210, progress);
}

void FieldScene_RunMiddleAuxiliarySequence(s32 a0)
{
    s32 p10;
    s32 rec2;
    u8 *rec7;
    s32 record;
    u8 *p6;
    u8 *base;

    base = (u8 *)((s16 *)&gGameState);
    p6 = *(u8 **)(base + 500);
    p10 = a0;
    rec7 = Engine_ActorGet((s32)p6);
    Actor_Get(p10);
    rec2 = GameFlag_IsSet(0x20f);
    if (rec2 == 0) {
        Event_Begin();
        Actor_SetAttachedEffect((s32)p6, 0x101);
        Actor_SetAnimation((s32)p6, 9);
        record = Engine_ActorGet(p10);
        if (record != 0) {
            Actor_SetDestination((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove((s32)p6);
        Audio_PlayCue(244);
        Engine_TaskAddCallback((s32)SuharaSabaku_SyncSelectedActorProgress, 0xc80);
        rec7[85] = rec2;
        Engine_ObjectSetPosition((s32)rec7, *(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12) + 0x200000, *(s32 *)(rec7 + 16));
        Actor_WaitForMove((s32)p6);
        *(s32 *)(rec7 + 40) = rec2;
        rec7[85] = 4;
        *(u8 *)(base + 498) = 2;
        GameFlag_Set(0x20f);
        GameFlag_SetByte(0x218, p10);
        GameFlag_SetByte(0x210, 180);
        Event_End();
        *(u16 *)(*(u8 **)((u8 *)&gEventWork) + 0x17c) = rec2;
    }
}

void FieldScene_RunActor8Step(void) { FieldScene_RunMiddleAuxiliarySequence(8); }
void FieldScene_RunActor9Step(void) { FieldScene_RunMiddleAuxiliarySequence(9); }
void FieldScene_RunActor10Step(void) { FieldScene_RunMiddleAuxiliarySequence(10); }
void FieldScene_RunActor11Step(void) { FieldScene_RunMiddleAuxiliarySequence(11); }
void FieldScene_RunActor12Step(void) { FieldScene_RunMiddleAuxiliarySequence(12); }
