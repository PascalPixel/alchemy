#include "TYPES.H"

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
s32 AnimationObjects_SelectAnimationFar(void *, s32);
s32 Map_RenderAnimatedTileFramesForObjectFar(void *);
struct Creature_080b8ec4 *Owner_GetStateFar();
struct Runtime_080b8ec4 *GetBattleObjectSlot(s32);
s32 ActivateBattleObjectSlot(s32);
void BattleMotion_SetMode5AndActivateSlot(s32 arg0)
{
  struct ActorData_080b8ec4 *actor_data;
  struct Actor_080b8ec4 *object;
  if (Owner_GetStateFar()->field_38 <= 0)
  {
    object = GetBattleObjectSlot(arg0)->field_00->field_50;
    AnimationObjects_SelectAnimationFar(object, 5);
    actor_data = object->field_28;
    actor_data->field_05 = 6;
    actor_data->field_16 = 0xFF;
    WaitFrames(4);
    Map_RenderAnimatedTileFramesForObjectFar(object);
    ActivateBattleObjectSlot(arg0);
  }
}
