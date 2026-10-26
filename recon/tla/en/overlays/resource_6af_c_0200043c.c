// Draft, differ 2 of 163: rewritten from the listing. Treasure Isle's opening
// of one cell of a list (Func_020005ac): it points the camera at the cell,
// fades, plays its sound, bursts three effects there, changes the cell's
// metatiles twice and hands over to Func_020003fc. The engine calls go through
// forwarding inlines, as scene_call.h describes (called directly they share -1
// and r3: 43). Left: the pool load of -0x20000 for the second burst comes one
// instruction late (after the cell's x load, where the native issues it right
// after the 0x9999 store); a temporary, a reversed sum, an empty asm after the
// store and a one-pass block round the store and the constant change nothing or
// add differences, and permute found nothing in 4 minutes.
#include "types.h"
#include "field_effect.h"

struct TreasureIsleCell {
  s32 x; ///< in 16-pixel cells
  s32 z; ///< in 16-pixel cells
  s32 unk08;
  u16 flag;
};

extern const s32 gData_02001128[];

void Func_02001004(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02001014(void);
void Func_02000fb4(s32 a, s32 b, s32 c);
void Func_0200106c(s32 sound);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void FieldEvent_WaitFrames(s32 frames);
void Func_02000f9c(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);
void Func_020003fc(struct TreasureIsleCell *cells, s16 index);

static __inline__ void CameraBounds(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH(wrapper): forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetFade(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarded inline; see scene_call.h. */
  Func_02000fb4(a, b, c);
}

static __inline__ void SetCells(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarded inline; see scene_call.h. */
  Func_02000f9c(a, b, c, d, x, z);
}

void Func_0200043c(struct TreasureIsleCell *cells, s16 index) {
  struct TreasureIsleCell *cell;
  struct EffectOptions options;

  Func_02001004(0x60000, 0xc000);
  // As &cells[index], the sum takes its operands the other way round.
  cell = (struct TreasureIsleCell *)((index << 4) + (s32)cells);
  CameraBounds((cell->x << 20) + 0x80000, -1,
               (cell->z << 20) + 0x80000, 1);
  Func_02001014();
  SetFade(0, 0x100000, 0x10000);
  SetFade(-1, -1, 0xe666);
  Func_0200106c(0x237);
  options.startScaleX = 0x13333;
  options.startScaleY = 0x18000;
  options.priority = 1;
  options.type = 0x115;
  options.script = gData_02001128;
  Func_0200015c((cell->x << 20) + 0x80000, 0, (cell->z << 20) + 0x140000, 0, 0,
                0, 0x3a0000, &options);
  options.startScaleX = 0x9999;
  Func_0200015c((cell->x << 20) - 0x20000, 0, (cell->z << 20) + 0x80000, 0, 0,
                0, 0x3a0000, &options);
  Func_0200015c((cell->x << 20) + 0x120000, 0, (cell->z << 20) + 0x80000, 0, 0,
                0, 0x3a0000, &options);
  FieldEvent_WaitFrames(10);
  SetCells(6, 100, 3, 4, cell->x + 63, cell->z - 2);
  FieldEvent_WaitFrames(10);
  SetCells(3, 100, 3, 4, cell->x + 63, cell->z - 2);
  FieldEvent_WaitFrames(10);
  Func_020003fc(cells, index);
}
