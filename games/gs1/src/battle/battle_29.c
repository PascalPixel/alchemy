#include "types.h"
#include "scene.h"
#include "object_effect.h"

/* battle/effects/object_control/commit_position_and_wait.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
s32 Object_CommitPosition(s32);
void Battle_WaitMode0(s32 arg0);

void BattleFx_CommitObjectPositionAndWait(void)
{
    Object_CommitPosition(FIELD_AT_OFFSET(Runtime_AllocateBlock(0x1B, 0xCCC), s32 *, 0x1E0));
    Battle_WaitMode0(2);
}

/* battle/get_work_object_1e0.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

s32 Battle_GetWorkObject1e0(void)
{
    return FIELD_AT_OFFSET(Runtime_AllocateBlock(0x1B, 0xCCC), s32 *, 0x1E0);
}

/* battle/effects/object_control/link_object_to_target.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void BattleFx_LinkObjectToTarget(void *target, s32 keep_current_position)
{
    void *object;

    object = FIELD_AT_OFFSET(Runtime_AllocateBlock(0x1B, 0xCCC), void **, 0x1E0);
    if (target != NULL) {
        Battle_Run(object, NULL);
        FIELD_AT_OFFSET(object, void **, 0x68) = target;
        if (keep_current_position == 0) {
            FIELD_AT_OFFSET(object, s32 *, 8) = FIELD_AT_OFFSET(target, s32 *, 8);
            FIELD_AT_OFFSET(object, s32 *, 0xC) = FIELD_AT_OFFSET(target, s32 *, 0xC);
            FIELD_AT_OFFSET(object, s32 *, 0x10) = FIELD_AT_OFFSET(target, s32 *, 0x10);
        }
    }
}
