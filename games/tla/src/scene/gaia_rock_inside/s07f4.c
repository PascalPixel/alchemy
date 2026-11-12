/// @file
/// Scene code of overlay 0x681, in ROM order, from s07f4's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "event_inline.h"
#include "main_routine_work.h"

extern const u8 gData_020050f4[];

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02004d9c(struct Actor *object, s32 mode);
void Func_02004dac(struct Actor *object, const u8 *script);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Effect_AdvanceMotionAtLeaderPriority(void);

struct DustSprite {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

s32 Func_020007f4(void) {
  struct Actor *source = Actor_Get(9);
  s32 i;

  Event_PlaySound(202);
  i = 3;
  do {
    {
      struct Actor *dust = (FieldActor_Create(
          0x11d, source->x, source->y, source->z));

      if (dust != 0) {
        dust->motionFlags = 0;
        dust->collisionFlags = 0;
        Actor_SetSingleChildFlags(dust, 0);
        Func_02004d9c(dust, 1);
        Func_02004dac(dust, gData_020050f4);
        ((struct DustSprite *)dust->animation)->layer = 1;
        dust->bounce = Math_Cosine(((Random16Far() << 3) >> 16) << 14) >> 1;
        dust->unk4c = Math_Sin(((Random16Far() << 3) >> 16) << 14) >> 1;
        dust->gravity = 0;
        dust->action = ((Random16Far() << 9) >> 16) - 0x100;
        dust->speedLimit = 0;
        dust->acceleration = 0;
        dust->update = Effect_AdvanceMotionAtLeaderPriority;
      }
    }
  } while (--i >= 0);
  return 0;
}

extern const u8 gData_020056c4[];
extern const u8 gData_02005728[];

s32 Func_020008b4(void) {
  struct Actor *object = Actor_Get(9);
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  if (leader->z >> 19 <= 45 && leader->x >> 19 <= 57)
    Func_02004dac(object, gData_02005728);
  else
    Func_02004dac(object, gData_020056c4);
  return 0;
}
