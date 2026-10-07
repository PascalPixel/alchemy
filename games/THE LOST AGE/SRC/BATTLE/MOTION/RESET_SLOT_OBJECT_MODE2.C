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

void BattleMotion_NoOp(void *position, s32 speed)
{
}

void BattleMotion_NoOp(void *, s32);

void BattleMotion_ApplyPairMidpoint(s32 first_id, s32 second_id)
{
    struct BattleObjectSlot *second_slot;
    struct MotionObject *first;
    struct MotionObject *second;
    s32 x1;
    s32 x2;
    s32 z1;
    s32 z2;
    struct {
        s32 x;
        s32 y;
        s32 z;
    } pos;

    first = GetBattleObjectSlot(first_id)->object;
    second_slot = GetBattleObjectSlot(second_id);


    x1 = first->target_x;
    /* FAKEMATCH: ⚓️ loads the first target before it builds the unset marker */
    __asm__ volatile("" : "+l"(x1));
    second = second_slot->object;
    if (x1 == (s32)0x80000000) {
        x1 = first->x;
    }
    z1 = first->target_z;
    if (z1 == (s32)0x80000000) {
        z1 = first->z;
    }
    x2 = second->target_x;
    if (x2 == (s32)0x80000000) {
        x2 = second->x;
    }
    z2 = second->target_z;
    if (z2 == (s32)0x80000000) {
        z2 = second->z;
    }

    pos.x = (x1 + x2) / 2;
    pos.y = 0;
    pos.z = (z1 + z2) / 2;

    BattleMotion_NoOp(&pos, 0x1000);
}
