// Draft, differ 38 of 458 in tla-en under -mthumb-slow-loads (2026-10-26). A
// device search (each statement followed by an empty asm or a one-pass block,
// wrapped in one, or swapped with the next, kept when it helps) reaches 27
// with `sprite->affine = 0;` moved after `sprite->affineIndex = 0;`, and finds
// nothing closer.
// Draft, differ 56 of 458 (2026-10-18): a dozen load-earlier schedules (the
// lane's near misses), and the branch offsets they move: the native
// function is four bytes longer, its page-arrow case placing its literal pool
// after the frame-tick load, where this places it before.
// ☀️'s UiWork_AnimateSpriteSlots with this game's sprite modes (no
// pulse; four hops; an off-screen test for mode 19).
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "affine.h"

struct AnimSprite {
  struct AnimSprite *next;
  u8 y;
  u8 affine : 2;
  u8 mode : 2;
  u8 mosaic : 1;
  u8 colors : 1;
  u8 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
};

struct AnimOutput {
  struct AnimOutput *next;
  u8 kind;
  u8 active;
  u16 x;
  u16 y;
  u16 unk0a;
  u16 animStep;
  u8 index;
  u8 sentinel;
  struct AnimSprite attr;
};

struct PopTransform {
  u32 scaleX : 16;
  u32 scaleY : 16;
  u32 angle : 16;
  u32 unk06 : 16;
};

extern u32 gFrameTick;
extern const u8 Data_0805ec84[];
extern const u8 gUiSprite_CursorBobOffsets[];
extern const s8 gUiSprite_WobbleOffsets[][2];
extern const s8 gUiSprite_BounceOffsets[];

u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
s32 VramCache_Load(s32 slot, s32 size, const void *data);
void Runtime_PushSlotEntry(void *entry, s32 priority);
void Func_0803c40c(struct AnimOutput *output);

void UiWork_AnimateSpriteSlots(void) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  struct UiWindow *window = work->windows;
  s32 no;
  union {
    struct AffineTransform plain;
    struct PopTransform packed;
  } effect;

  for (no = 0; no != UI_WINDOW_COUNT; window++, no++) {
    struct AnimOutput *output;
    u32 phase;

    if (!(window->flags & 1))
      continue;
    output = (struct AnimOutput *)window->output.head;
    while (phase = (gFrameTick >> 2) & 7, output != NULL) {
      struct AnimSprite *sprite = &output->attr;

      switch (output->active) {
      case 2:
        if (work->glyphResource != 0x60) {
          const u8 *tiles = &Data_0805ec84[phase * 128];

          sprite->tile = VramCache_Load(work->glyphResource, 128, tiles);
          output->index = sprite->tile;
          sprite->mode = 0;
          sprite->mosaic = 0;
          sprite->colors = 1;
          sprite->size = 0;
          sprite->shape = 2;
          sprite->y =
              output->y + gUiSprite_CursorBobOffsets[gFrameTick % 80] + 2;
          sprite->affine = 0;
          sprite->affineIndex = 0;
        }
        break;
      case 5:
        if (gFrameTick & 1) {
          sprite->x =
              output->x +
              (s32)(((Random16() * 3 >> 16) + (Random16() * 3 >> 16)) >> 1) - 1;
          sprite->y =
              output->y +
              (s32)(((Random16() * 3 >> 16) + (Random16() * 3 >> 16)) >> 1) - 1;
        }
        break;
      case 6:
        if (output->animStep != 0) {
          effect.packed.scaleX = 512;
          effect.packed.scaleY = 512;
          effect.packed.angle = 0;
          sprite->affineIndex = AffineMatrix_BuildForEffect(&effect.plain);
          sprite->affine = 3;
          sprite->x = output->x - 5;
          sprite->y = output->y - 5;
          output->animStep--;
        } else {
          sprite->affineIndex = 0;
          sprite->affine = 0;
          sprite->x = output->x;
          sprite->y = output->y;
        }
        break;
      case 7:
        effect.plain.scaleX = 256;
        effect.plain.scaleY = 256;
        output->animStep += 768;
        effect.plain.angle = output->animStep;
        sprite->affineIndex = AffineMatrix_BuildForEffect(&effect.plain);
        sprite->affine = 1;
        sprite->x = output->x - (Trig_Sin(effect.plain.angle + 0xe800) >> 14) - 2;
        sprite->y = output->y - (Trig_Cos(effect.plain.angle + 0x6800) >> 14) - 2;
        break;
      case 4:
        if (gFrameTick & 1)
          output->animStep++;
        sprite->x =
            output->x + gUiSprite_WobbleOffsets[output->animStep % 20][0];
        sprite->y =
            output->y + gUiSprite_WobbleOffsets[output->animStep % 20][1] - 2;
        break;
      case 17:
        output->animStep++;
        sprite->y = output->y - gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case 15:
        output->animStep++;
        sprite->y = output->y + gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case 16:
        output->animStep++;
        sprite->x = output->x - gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case 14:
        output->animStep++;
        sprite->x = output->x + gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case 18:
        output->animStep++;
        sprite->x = output->x - gUiSprite_BounceOffsets[output->animStep & 15];
        sprite->y = output->y + gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case 9:
      case 10:
      case 11:
      case 12:
        Func_0803c40c(output);
        break;
      }
      if (output->active == 2) {
        if (work->glyphResource != 0x60)
          Runtime_PushSlotEntry(sprite, 255);
      } else if (output->active == 19) {
        s32 x = (s16)output->x;
        s32 y = (s16)output->y;

        if ((u32)(x + 64) < 304 && (u32)(y + 64) < 224)
          Runtime_PushSlotEntry(sprite, output->sentinel);
      } else if (output->active != 13) {
        Runtime_PushSlotEntry(sprite, output->sentinel);
      }
      output = output->next;
    }
  }
}
