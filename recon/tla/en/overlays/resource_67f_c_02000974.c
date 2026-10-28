// Draft, differ 79 of 122: eight dust clouds in a row across the leader, thrown
// out in a ring when spread is set and straight up otherwise. Overlay 68F's
// Func_020029d0 and 691's Func_020034ac are copies with small changes.
// The ring's outward speed is the cosine scaled by Iwram_MulQ16 by 0x13333,
// which the earlier draft left out. Left, allocation: the native keeps the
// leader in r7, spread in r8 and the count in r10 (here r8, r10 and r7), and
// its bitfield mask comes from r0 through a copy.
#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "iwram_call.h"

extern const u8 gData_02001540[];

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

void Func_02000974(s32 spread) {
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  s32 frame = 0;
  u32 i;

  i = 0;
loop:
  {
    struct Actor *dust = Func_02001438(
        0x11d, (i << 17) + leader->x - 0x80000,
        leader->y - (((Random16Far() * 14) >> 16) << 16) - 0x20000, leader->z);

    if (dust != 0) {
      frame = Func_020014f0(dust->animation, frame);
      dust->motionFlags = 0;
      dust->collisionFlags = 0;
      Actor_SetSingleChildFlags(dust, 0);
      Func_02001428(dust, 2);
      Func_02001430(dust, gData_02001540);
      dust->scaleX = 0xb333;
      dust->scaleY = 0xb333;
      ((struct DustSprite *)dust->animation)->layer = 1;
      if (spread != 0) {
        s32 angle = i << 13;

        dust->bounce = Iwram_MulQ16(Math_Cosine(angle), 0x13333);
        dust->unk4c = Math_Sine(angle);
      } else {
        dust->bounce = spread;
        dust->unk4c = spread;
      }
      dust->gravity = -(((Random16Far() << 14) >> 16) + 0x6000);
      dust->action = ((Random16Far() << 9) >> 16) - 0x100;
      dust->speedLimit = -0x800;
      dust->acceleration = -0x600;
      dust->update = Effect_AdvanceMotionAtLeaderPriority;
    }
  }
  if (++i <= 7)
    goto loop;
}
