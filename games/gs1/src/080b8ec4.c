#include "types.h"

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

s32 Func_080030f8(s32);
s32 Func_08009020(void *, s32);
s32 Func_08009100(void *);
struct Creature_080b8ec4 *Func_08077008();
struct Runtime_080b8ec4 *Func_080b7dd0(s32);
s32 Func_080b7e60(s32);
void Func_080b8ec4(s32 arg0)
{
  struct ActorData_080b8ec4 *actor_data;
  struct Actor_080b8ec4 *object;
  if (Func_08077008()->field_38 <= 0)
  {
    object = Func_080b7dd0(arg0)->field_00->field_50;
    Func_08009020(object, 5);
    actor_data = object->field_28;
    actor_data->field_05 = 6;
    actor_data->field_16 = 0xFF;
    Func_080030f8(4);
    Func_08009100(object);
    Func_080b7e60(arg0);
  }
}
