#include "GLOBAL.H"
#include "FIXED_MATH.H"
#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "GAME_STATE.H"
#include "FIELDOBJ.H"


void Animation_ApplyChildValuesFar(void *, s32);
void ObjectDispatch_ApplyValueToChildrenFar(void *, s32);

void BattleEffect_SetRandomTableValueOnObject(s32 arg0)
{
    s8 *table = BattleFx_RandomChildValues;
    s32 index = Random16();
    Animation_ApplyChildValuesFar((void *)arg0, table[(u32)(index * 8) >> 16]);
}

void BattleEffect_PauseObject(s32 id)
{
    struct FieldActor *object;
    u8 *entry;

    object = ObjectTable_Get(id);
    if (object != NULL) {
        gGameState.paused_update = (u32)object->update;
        gGameState.paused_pose = 0;
        if (object->active == 1) {
            entry = *(u8 **)((u8 *)object->sprite + 0x28);
            if (entry != NULL) {
                gGameState.paused_pose = entry[5];
            }
        }
        object->update = (void (*)(union FieldObject *))BattleEffect_SetRandomTableValueOnObject;
        object->paused = 1;
        ObjectDispatch_ApplyValueToChildrenFar(object, 0);
    }
}

void BattleFx_ResumeObject(void)
{
    struct FieldActor *object = ObjectTable_Get();
    if (object != 0) {
        if (object->update == (void (*)(union FieldObject *))BattleEffect_SetRandomTableValueOnObject) {
            struct GameState *state = &gGameState;
            object->update = (void (*)(union FieldObject *))state->paused_update;
            state->paused_update = 0;
            Animation_ApplyChildValuesFar(object, state->paused_pose);
        }
        object->paused = 0;
        ObjectDispatch_ApplyValueToChildrenFar(object, 16);
    }
}
