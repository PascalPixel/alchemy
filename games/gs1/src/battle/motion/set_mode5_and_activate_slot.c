#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "battle_party.h"
#include "battle_runtime.h"
#include "battle_target.h"

/* battle/motion/set_mode5_and_activate_slot.c */
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

struct ObjectHolder_080b8ec4 {
  u8 padding_00[0x50];
  struct Actor_080b8ec4 *field_50;
};

struct Runtime_080b8ec4 {
  struct ObjectHolder_080b8ec4 *field_00;
};

void WaitFrames(s32);
s32 Object_InitializeMode(void *, s32);

struct BattleUnit *Runtime_GetObject();
struct Runtime_080b8ec4 *GetBattleObjectSlot(s32);
s32 ActivateBattleObjectSlot(s32);
void BattleMotion_SetMode5AndActivateSlot(s32 arg0)
{
  struct ActorData_080b8ec4 *actor_data;
  struct Actor_080b8ec4 *object;
  if (Runtime_GetObject()->hp <= 0)
  {
    object = GetBattleObjectSlot(arg0)->field_00->field_50;
    Object_InitializeMode(object, 5);
    actor_data = object->field_28;
    actor_data->field_05 = 6;
    actor_data->field_16 = 0xFF;
    WaitFrames(4);
    Battle_Check(object);
    ActivateBattleObjectSlot(arg0);
  }
}
