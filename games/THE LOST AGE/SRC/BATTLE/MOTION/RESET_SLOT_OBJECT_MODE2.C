#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_MOTION.H"
#include "FIXED_MATH.H"

u8 *Owner_GetState(s32);
void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);

extern s32 BattleMotion_VariantAcceleration[];
extern s32 BattleMotion_VariantSpeedLimit[];
extern s32 BattleMotion_VariantVelocityY[];
extern s32 BattleMotion_VariantDistancePercent[];

void BattleMotion_ApplyVariantMotion(s32 id, s32 variant);
void BattleMotion_ApproachTarget( s32 actor_id, s32 target_id, s32 travel_divisor, s32 initial_velocity_y );
void BattleMotion_ReservedNoOp83B0();

void BattleMotion_ResetSlotObjectMode2(s32 id)
{
    struct MotionObject *object;

    object = GetBattleObjectSlot(id)->object;
    Object_ResetMotion(object);
    Object_SetMode(object, 2);
}
