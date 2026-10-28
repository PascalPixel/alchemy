#include "types.h"
#include "field_effect.h"

/// One entry of the cell lists Func_020005ac walks.
struct TreasureIsleCell {
  s32 x; ///< in 16-pixel cells
  s32 z; ///< in 16-pixel cells
  s32 unk08;
  u16 flag; ///< set once the cell has been opened (Func_020003fc)
};

extern const s32 gData_02001128[];

void Func_02001004(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02001014(void);
void Func_02000fb4(s32 a, s32 b, s32 c);
void Func_0200106c(s32 sound);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void FieldEvent_Wait(s32 frames);
void Func_02000f9c(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);
void Func_020003fc(struct TreasureIsleCell *cells, s16 index);

static __inline__ void CameraBounds(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as a direct call, 13
   * of 163 instructions differ in tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetFade(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as direct calls, 11
   * of 163 instructions differ in tla-en. */
  Func_02000fb4(a, b, c);
}

static __inline__ void SetCells(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as direct calls, 24
   * of 163 instructions differ in tla-en. */
  Func_02000f9c(a, b, c, d, x, z);
}

/// Called by Func_020005ac for each listed cell whose flag is still clear:
/// points the camera at the cell, bursts three effects there, changes its
/// cell pictures twice and hands over to Func_020003fc, which sets its flag.
void Func_0200043c(struct TreasureIsleCell *cells, s16 index) {
  struct TreasureIsleCell *cell;
  struct EffectOptions options;

  Func_02001004(0x60000, 0xc000);
  // As &cells[index], the sum takes its operands the other way round.
  cell = (struct TreasureIsleCell *)((index << 4) + (s32)cells);
  CameraBounds((cell->x << 20) + 0x80000, -1, (cell->z << 20) + 0x80000, 1);
  Func_02001014();
  SetFade(0, 0x100000, 0x10000);
  SetFade(-1, -1, 0xe666);
  Func_0200106c(567);
  options.startScaleX = 0x13333;
  options.startScaleY = 0x18000;
  options.priority = 1;
  options.type = 0x115;
  options.script = gData_02001128;
  Func_0200015c((cell->x << 20) + 0x80000, 0, (cell->z << 20) + 0x140000, 0, 0,
                0, 0x3a0000, &options);
  options.startScaleX = 0x9999;
  {
    s32 left = -0x20000;

    /* FAKEMATCH(barrier): an empty asm holding the offset and the scale just
     * stored, so the offset's pool load comes before the cell's x is read; as
     * (cell->x << 20) - 0x20000, 2 of 163 instructions differ in tla-en. */
    asm("" : "+l"(left), "+m"(options.startScaleX));
    Func_0200015c((cell->x << 20) + left, 0, (cell->z << 20) + 0x80000, 0, 0, 0,
                  0x3a0000, &options);
  }
  Func_0200015c((cell->x << 20) + 0x120000, 0, (cell->z << 20) + 0x80000, 0, 0,
                0, 0x3a0000, &options);
  FieldEvent_Wait(10);
  SetCells(6, 100, 3, 4, cell->x + 63, cell->z - 2);
  FieldEvent_Wait(10);
  SetCells(3, 100, 3, 4, cell->x + 63, cell->z - 2);
  FieldEvent_Wait(10);
  Func_020003fc(cells, index);
}
