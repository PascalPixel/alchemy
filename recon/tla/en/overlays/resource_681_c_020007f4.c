// Draft, differ 27 of 79: four dust puffs scatter from person 9 with a sound.
#include "types.h"
#include "actor.h"

extern const u8 gData_020050f4[];

struct Actor *Actor_GetById(s32 id);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Func_02005014(s32 cue);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02004d9c(struct Actor *object, s32 mode);
void Func_02004dac(struct Actor *object, const u8 *script);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Effect_AdvanceMotionAtLeaderPriority(void);

struct DustSprite {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

s32 Func_020007f4(void) {
  struct Actor *source = Actor_GetById(9);
  s32 i;

  Func_02005014(202);
  i = 3;
loop:
  {
    struct Actor *dust =
        Import_ActorCreate(0x11d, source->x, source->y, source->z);

    if (dust != 0) {
      dust->motionFlags = 0;
      dust->collisionFlags = 0;
      Actor_SetSingleChildFlags(dust, 0);
      Func_02004d9c(dust, 1);
      Func_02004dac(dust, gData_020050f4);
      ((struct DustSprite *)dust->animation)->layer = 1;
      dust->bounce = Math_Cosine(((Random16Far() << 3) >> 16) << 14) >> 1;
      dust->unk4c = Math_Sine(((Random16Far() << 3) >> 16) << 14) >> 1;
      dust->gravity = 0;
      dust->action = ((Random16Far() << 9) >> 16) - 0x100;
      dust->speedLimit = 0;
      dust->acceleration = 0;
      dust->update = Effect_AdvanceMotionAtLeaderPriority;
    }
  }
  if (--i >= 0)
    goto loop;
  return 0;
}
