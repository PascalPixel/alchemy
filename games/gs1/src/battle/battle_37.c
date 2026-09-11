#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "battle_party.h"
#include "battle_runtime.h"
#include "battle_target.h"

/* battle/motion/set_mode5_and_activate_slot.c */
struct Creature_080b8ec4 {
  u8 padding_00[0x38];
  s16 field_38;
};

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

s32 WaitFrames(s32);
s32 Object_InitializeMode(void *, s32);

struct Creature_080b8ec4 *Runtime_GetObject();
struct Runtime_080b8ec4 *GetBattleObjectSlot(s32);
s32 ActivateBattleObjectSlot(s32);
void BattleMotion_SetMode5AndActivateSlot(s32 arg0)
{
  struct ActorData_080b8ec4 *actor_data;
  struct Actor_080b8ec4 *object;
  if (Runtime_GetObject()->field_38 <= 0)
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

/* battle/target/replace_defeated.c */
u32 Random16(void);

s32 BattleTarget_ReplaceDefeated(const u8 *action)
{
    s16 living_units[14];
    s32 target_id;
    s32 living_count;

    target_id = ((const s16 *)action)[5];
    if (BattleUnit_Get(target_id)->hp != 0) {
        return target_id;
    }

    if (target_id > 0x7F) {
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_ENEMIES,
            living_units);
    } else {
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_PARTY,
            living_units);
    }

    if (living_count == 0) {
        return 0x100;
    }

    return living_units[(u32)(Random16() * living_count) >> 0x10];
}
