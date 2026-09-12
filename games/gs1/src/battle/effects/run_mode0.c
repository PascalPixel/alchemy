#include "types.h"
#include "scene.h"
#include "b5_context.h"
#include "motion_object.h"

/* battle/effects/runtime/mode/run_mode_0.c */
void BattleFx_RunMode0(s32 arg0)
{
    Battle_Apply(arg0, 0);
}

/* battle/effects/runtime/mode/run_mode_1.c */
void BattleFx_RunMode1(s32 arg0)
{
    Battle_Apply(arg0, 1);
}

/* battle/effects/runtime/mode/run_mode_2.c */
void BattleFx_RunMode2(s32 arg0)
{
    Battle_Apply(arg0, 2);
}

/* battle/effects/runtime/mode/run_mode_3.c */
void BattleFx_RunMode3(s32 arg0)
{
    Battle_Apply(arg0, 3);
}

/* battle/effects/runtime/mode/run_mode_11.c */
void BattleFx_RunMode11(s32 arg0)
{
    Battle_Apply(arg0, 0xB);
}

/* battle/effects/runtime/mode/run_mode_4.c */
void BattleFx_RunMode4(s32 arg0)
{
    Battle_Apply(arg0, 4);
}

/* battle/effects/runtime/mode/run_mode_5_with_action.c */
void Object_SetMode(struct MotionObject *, s32);
s32 Object_SetAction(s32, s32);
struct B5Context *Battle_Run(s32);

void BattleFx_RunMode5WithAction(void *effect)
{
    s32 object;

    object =
        (s32)Battle_Run(FIELD_AT_OFFSET(effect, s32 *, 8))->object;
    Object_SetMode(object, 2);
    Object_SetAction(object, 0x30);
    Battle_Apply(effect, 5);
    Object_SetAction(object, 0x10);
}
