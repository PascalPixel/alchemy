/// @file
/// The glow's frame callback.

#include "types.h"
#include "call.h"
#include "ram_buffer.h"

struct GlowSprite {
  u8 unk_0[12];
  s32 x;
  s32 z;
  s32 scale;
  s32 trailingScale;
  u8 unk_1C[12];
};

struct Glow {
  u8 unk_0[4096];
  s32 position[3];
  struct GlowSprite sprites[2];
  u8 unk_105C[8];
  s32 scale;
  s32 trailingScale;
  s32 scale2;
  s32 trailingScale2;
  s16 frame;
};

s32 Math_Sine(s32 angle);
void Func_020035c4(s32 *position);
void Func_02003604(struct GlowSprite *sprite);

/// Cycles thirty-two colours of the object palettes through a wave of greens
/// and blues, places the glow's two sprites at its spot with their scales, and
/// lets each sprite's second scale trail its first at 31/40.
void Func_02002de4(void) {
  struct Glow *work = (struct Glow *)Ram_HeapSlots->menuRuntime;
  s32 position[3];
  struct GlowSprite *sprite;
  s32 i;

  i = 0;
loop:
  {
    s32 red = 31;
    s16 *frame = &work->frame;
    s32 wave = Math_Sine((*frame + i) << 11) * 6;
    s32 blue = Math_Sine(((*frame / 2 + i) << 11) + 0x4800) * 3;
    s32 colour;
    wave >>= 16;
    blue >>= 16;
    wave += 20;
    blue += 3;
    if (i <= 15) {
      colour = blue << 10 | wave << 5 | red;
      ((u16 *)0x05000200)[111 - i] = colour;
    } else {
      colour = blue << 10 | wave << 5 | red;
      ((u16 *)0x05000200)[255 - i] = colour;
    }
    i++;
  }
  if (i <= 31)
    goto loop;
  position[0] = work->position[0];
  position[1] = work->position[1];
  position[2] = work->position[2];
  sprite = &work->sprites[0];
  Func_020035c4(position);
  sprite->x = position[0];
  sprite->z = position[2];
  sprite->scale = work->scale;
  sprite->trailingScale = work->trailingScale;
  Func_02003604(sprite);
  position[0] = work->position[0];
  position[1] = work->position[1];
  position[2] = work->position[2];
  sprite = &work->sprites[1];
  Func_020035c4(position);
  sprite->x = position[0];
  sprite->z = position[2];
  sprite->scale = work->scale2;
  sprite->trailingScale = work->trailingScale2;
  Func_02003604(sprite);
  work->trailingScale = work->scale * 31 / 40;
  work->trailingScale2 = work->scale2 * 31 / 40;
  work->frame++;
}
