// Draft, differ 49 of 116: seventeen dust clouds burst outward around the
// leader on the leader's sprite layer. Overlay 68F's Func_02002b04 and 691's
// Func_020035e0 are copies with small changes.
// Measured against its copies: overlay 68F's Func_02002b04 (differ 49 of 116),
// overlay 691's Func_020035e0 (differ 49 of 116).
#include "types.h"
#include "actor.h"
#include "adventure.h"

extern const u8 gData_02001570[];

struct Actor *Actor_GetById(s32 id);
u32 Random16Far(void);
struct Actor *Func_02001438(s32 type, s32 x, s32 y, s32 z);
s32 Func_020014f0(void *sprite, s32 frame);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02001428(struct Actor *object, s32 mode);
void Func_02001430(struct Actor *object, const u8 *script);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Effect_AdvanceMotionAtLeaderPriority(void);

struct DustSprite {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

void Func_02000aa8(void) {
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  s32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
loop:
  {
    struct Actor *dust =
        Func_02001438(0x11d, leader->x, leader->y, leader->z);

    if (dust != 0) {
      s32 angle;

      frame = Func_020014f0(dust->animation, frame);
      dust->motionFlags = 0;
      dust->collisionFlags = 0;
      Actor_SetSingleChildFlags(dust, 0);
      Func_02001428(dust, 2);
      Func_02001430(dust, gData_02001570);
      angle = i << 12;
      dust->bounce = Math_Cosine(angle);
      dust->gravity = 0;
      dust->unk4c = Math_Sine(angle);
      dust->bounce += dust->bounce >> 1;
      dust->bounce += ((Random16Far() * 3 << 14) >> 16) - 0x6000;
      dust->unk4c += ((Random16Far() * 3 << 13) >> 16) - 0x3000;
      dust->action = ((Random16Far() << 12) >> 16) - 0x800;
      dust->speedLimit = 0;
      dust->acceleration = 0;
      dust->update = Effect_AdvanceMotionAtLeaderPriority;
      ((struct DustSprite *)dust->animation)->layer =
          ((struct DustSprite *)leader->animation)->layer;
    }
  }
  if (++i <= 16)
    goto loop;
}
