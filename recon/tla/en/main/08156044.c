#include "TYPES.H"
#include "MOTION_OBJECT.H"

/* Mode entries of the projectile volley effect. */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Object_SetMode(s32, s32);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32);
s32 ObjectDispatch_ApplyValueToChildrenFar(s32, s32);
s32 BattleFx_RunProjectileVolley(void *effect, s32 mode);

void BattleFx_RunMode9WithAction(void *effect)
{
    s32 object;

    object =
        (s32)GetBattleObjectSlotFar(FIELD_AT_OFFSET(effect, s32 *, 8))->object;
    Object_SetMode(object, 2);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
    BattleFx_RunProjectileVolley(effect, 9);
    ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}
