#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "animspr.h"


void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh)
{
    Func_0811b75c(list, refresh, 1);
}

struct AnimationEntryGroup {
    u8 unknown_00[0x28];
    struct AnimationEntry *entries[4];
};

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
