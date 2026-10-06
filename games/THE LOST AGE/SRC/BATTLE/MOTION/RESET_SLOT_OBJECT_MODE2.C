#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_MOTION.H"
#include "FIXED_MATH.H"

#define Object_ResetMotion ((void (*)(struct MotionObject *))Object_ResetMotion)
#define Object_SetPosition ((void (*)(struct MotionObject *, s32, s32, s32))Object_SetPosition)
#define Object_SetMode ((void (*)(struct MotionObject *, s32))Object_SetMode)



void BattleMotion_ReservedNoOp83B0();

void BattleMotion_ResetSlotObjectMode2(s32 id)
{
    struct MotionObject *object;

    object = GetBattleObjectSlot(id)->object;
    Object_ResetMotion(object);
    Object_SetMode(object, 2);
}
