/*
 * Draft: BattleMotion_SetMode5AndActivateSlot, ported from its ☀️ twin; it
 * follows BattlePresentation_ApplyUnitDamage in SRC/BATTLE/PRESENTATION/
 * PRESENT12.C. Score 60: the listing sets the animation argument
 * (movs r1, #5) between the slot's two loads, where this C (and ☀️'s
 * build of it) sets it after the second load. A 30-second permuter run and
 * assigning the records inside the call found nothing better.
 */
#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_UNIT.H"
#include "SYSTEM.H"
#include "OWNER_STATE.H"

struct ActorData_080b8ec4 {
  u8 padding_00[5];
  s8 field_05;
  u8 padding_06[0x10];
  s8 field_16;
};

struct Actor_080b8ec4 {
  u8 padding_00[0x28];
  struct ActorData_080b8ec4 *field_28;
};

s32 Animation_ApplyChildArgumentFar(void *, s32);
s32 Map_RenderAnimatedTileFramesForObjectFar(void *);
s32 ActivateBattleObjectSlot(s32);

void BattleMotion_SetMode5AndActivateSlot(s32 arg0)
{
  struct ActorData_080b8ec4 *actor_data;
  struct Actor_080b8ec4 *object;
  if (((struct BattleUnit *)Owner_GetState(arg0))->hp <= 0)
  {
    object = GetBattleObjectSlot(arg0)->object->records;
    Animation_ApplyChildArgumentFar(object, 5);
    actor_data = object->field_28;
    actor_data->field_05 = 6;
    actor_data->field_16 = 0xFF;
    WaitFrames(4);
    Map_RenderAnimatedTileFramesForObjectFar(object);
    ActivateBattleObjectSlot(arg0);
  }
}
