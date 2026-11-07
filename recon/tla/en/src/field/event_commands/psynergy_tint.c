#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "field_psynergy_work.h"

s32 GameFlag_Test(s32 flag);
void Field_StartColorTransition(s32 frames, s32 color);

/// The eight tints a field Psynergy washes the screen in (BGR555, fxdata.c).
extern const u32 gData_080f3954[8];

/// Flags 0x148-0x14f each force one tint, the last set one winning.
#define FLAG_TINT_FIRST 0x148

/// Tints the field for a Psynergy animation: a tint chosen by the frame
/// counter (so each cast looks a little different), fixed for some
/// animations, and forced by the tint flags; then fades towards it over eight
/// frames. The palettes are copied by Field_StartColorTransition (palfade.c).
// ☀️'s function of the same name also copies the palettes.
void BattleEffect_InitializeSharedScene(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  s32 tint;
  s32 animation;
  s32 raw;

  tint = gFrameCount & 7;
  raw = *(u16 *)&scene->fieldKind;
  /* FAKEMATCH: the barrier keeps the unsigned (ldrh) and signed (ldrsh) reads
   * of the animation apart; without it, 7 instructions differ in tla-en. */
  asm("" : "+r"(raw));
  animation = scene->fieldKind;
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
  if ((s16)raw == 30)
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
  Field_StartColorTransition(8, gData_080f3954[tint]);
}
