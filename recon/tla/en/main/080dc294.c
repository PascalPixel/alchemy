// Near miss, differ 7 of 102 (2026-10-15), The Broken Seal's
// BattleEffect_InitializeSharedScene. ☀️ copies the palettes into the target
// buffer itself and picks the tint from the frame counter, overridden by
// flags 0x148-0x14f; ⚓️ adds a tint fixed by the animation for its newer
// kinds and leaves the copy to Field_BeginColorTransition (palfade.c).
// Left: the last animation test. The native reads the animation twice at
// the top, signed for the chain (ldrsh) and unsigned (ldrh), and tests the
// unsigned copy's low half against 30 (lsls r3, r3, #16; cmp with 30 << 16);
// this tests a halfword load against 30 (cmp r3, #30).
// Tried: (s16)*(u16 *)&scene->animation, (s16)scene->action and a u16 local
// read before the chain all fold into the signed load (differ 30-32); the
// chain on a u16 or s16 local, with (u16) or (s16) casts, differ 18-29.
#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "fx_scene.h"

s32 GameFlag_Test(s32 flag);
void Field_BeginColorTransition(s32 frames, s32 color);

// The eight tints a field Psynergy washes the screen in (BGR555, fxdata.c).
extern const u32 gData_080f3954[8];

// Flags 0x148-0x14f each force one tint, the last set one winning.
#define FLAG_TINT_FIRST 0x148

// Tint the field for a Psynergy animation: a tint chosen by the frame counter
// (so each cast looks a little different), fixed for some animations, and
// forced by the tint flags; then fade towards it over eight frames.
void BattleEffect_InitializeSharedScene(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  s32 tint;
  s32 animation;

  tint = gFrameCount & 7;
  animation = scene->animation;
  if (animation == 18)
    tint = 0;
  if (animation == 26)
    tint = 2;
  if (animation == 23)
    tint = 7;
  if (animation == 25)
    tint = 0;
  if (animation == 20)
    tint = 5;
  if (animation == 29)
    tint = 0;
  if (animation == 22)
    tint = 7;
  if (animation == 19)
    tint = 6;
  if (animation == 27)
    tint = 0;
  if (animation == 21)
    tint = 7;
  if ((u16)scene->animation == 30)
    tint = 6;

  if (GameFlag_Test(FLAG_TINT_FIRST + 0))
    tint = 0;
  if (GameFlag_Test(FLAG_TINT_FIRST + 1))
    tint = 1;
  if (GameFlag_Test(FLAG_TINT_FIRST + 2))
    tint = 2;
  if (GameFlag_Test(FLAG_TINT_FIRST + 3))
    tint = 3;
  if (GameFlag_Test(FLAG_TINT_FIRST + 4))
    tint = 4;
  if (GameFlag_Test(FLAG_TINT_FIRST + 5))
    tint = 5;
  if (GameFlag_Test(FLAG_TINT_FIRST + 6))
    tint = 6;
  if (GameFlag_Test(FLAG_TINT_FIRST + 7))
    tint = 7;
  Field_BeginColorTransition(8, gData_080f3954[tint]);
}
