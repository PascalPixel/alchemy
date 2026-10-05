#include "TYPES.H"
#include "ANIMSPR.H"

void Func_0811b75c(s16 *list, s32 refresh, s32 spawn);

/* ⚓️ spawns the list's objects through a three-argument routine that ☀️
   does not have; this keeps ☀️'s two-argument entry. */
void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh)
{
    Func_0811b75c(list, refresh, 1);
}

/* The four animation entries ⚓️ keeps 0x28 into an animation object. */
struct AnimationEntryGroup {
    u8 unknown_00[0x28];
    struct AnimationEntry *entries[4];
};

/* Stops the scripts of an animation object's four entries. */
void ResetMotionRecordGroup(void *owner)
{
    struct AnimationEntryGroup *object = owner;
    s32 i;

    if (object != NULL) {
        for (i = 0; i < 4; i++) {
            if (object->entries[i] != NULL)
                object->entries[i]->script = NULL;
        }
    }
}
