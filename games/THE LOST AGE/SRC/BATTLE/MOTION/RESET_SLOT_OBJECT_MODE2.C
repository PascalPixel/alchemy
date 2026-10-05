#include "PROTO.H"
#include "GLOBAL.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_MOTION.H"
#include "FIXED_MATH.H"

void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);


void BattleMotion_ReservedNoOp83B0();

void BattleMotion_ResetSlotObjectMode2(s32 id)
{
    struct MotionObject *object;

    object = GetBattleObjectSlot(id)->object;
    Object_ResetMotion(object);
    Object_SetMode(object, 2);
}
