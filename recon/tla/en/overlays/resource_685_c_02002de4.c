// Draft, differ 185 of 175: the frame callback of the glow: cycles thirty-two
// colours of the object palettes through a wave of greens and blues, places
// the glow's two sprites at its spot with their scales, and lets each sprite's
// second scale trail its first by a quarter.
#include "types.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Math_Sine(s32 angle);
void Func_020035c4(s32 *position);
void Func_02003604(u8 *sprite);

void Func_02002de4(void) {
  u8 *work = Ram_HeapSlots->menuRuntime;
  s32 position[3];
  u8 *sprite;
  s32 i;

  for (i = 0; i <= 31; i++) {
    s16 *frame = &FIELD_AT(work, s16, 4212);
    s32 green = ((Math_Sine((*frame + i) << 11) * 6) >> 16) + 20;
    s32 blue = ((Math_Sine(((*frame / 2 + i) << 11) + 0x4800) * 3) >> 16) + 3;
    u16 colour = blue << 10 | green << 5 | 31;

    if (i <= 15)
      ((u16 *)0x05000200)[111 - i] = colour;
    else
      ((u16 *)0x05000200)[255 - i] = colour;
  }
  position[0] = FIELD_AT(work, s32, 4096);
  position[1] = FIELD_AT(work, s32, 4100);
  position[2] = FIELD_AT(work, s32, 4104);
  Func_020035c4(position);
  sprite = work + 4108;
  FIELD_AT(sprite, s32, 12) = position[0];
  FIELD_AT(sprite, s32, 16) = position[2];
  FIELD_AT(sprite, s32, 20) = FIELD_AT(work, s32, 4196);
  FIELD_AT(sprite, s32, 24) = FIELD_AT(work, s32, 4200);
  Func_02003604(sprite);
  position[0] = FIELD_AT(work, s32, 4096);
  position[1] = FIELD_AT(work, s32, 4100);
  position[2] = FIELD_AT(work, s32, 4104);
  Func_020035c4(position);
  sprite = work + 4148;
  FIELD_AT(sprite, s32, 12) = position[0];
  FIELD_AT(sprite, s32, 16) = position[2];
  FIELD_AT(sprite, s32, 20) = FIELD_AT(work, s32, 4204);
  FIELD_AT(sprite, s32, 24) = FIELD_AT(work, s32, 4208);
  Func_02003604(sprite);
  FIELD_AT(work, s32, 4200) = FIELD_AT(work, s32, 4196) * 31 / 40;
  FIELD_AT(work, s32, 4208) = FIELD_AT(work, s32, 4204) * 31 / 40;
  FIELD_AT(work, u16, 4212)++;
}
