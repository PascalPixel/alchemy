// Under -Os, 5 November: differ 101 of 115 (tla-en); older figures below predate it.
// Draft, differ 144 of 115, skeleton gap 24 (tla-en, 2026-11-01)
// The cell grid of s48a4.c, written from the listing. Two things are left, and
// the statements are otherwise the native's: a scratch compile of this source
// with both changes below, by cc1 -fno-caller-saves, equals the listing line
// for line with branch and pool operands blanked. That option is a diagnostic
// only and is not proposed here; the plain source under it still differs by 93
// lines, and the real compile with change 1 alone gives differ 100.
// 1. Argument order. The native loads kind, then originX, then originZ into
// registers and stores both stack arguments together (read, read, add, add,
// str, str), so all the arguments are evaluated before the call. This source
// stores each as it goes. These both give the native's order:
//   static inline void Draw(s32 t, s32 k, s32 x, s32 z) {
//     Func_0200566c(t, k, 1, 1, x, z);
//   }
//   #define DRAW(t, k, a, b) do { s32 k_ = (k); s32 x_ = (a); s32 z_ = (b);
//     Func_0200566c(t, k_, 1, 1, x_, z_); } while (0)
// Either is left out because it is a wrapper or a one-pass block with forced
// temporaries (S2), and this round adds no device. Whether the macro counts as
// S2's matching do-while macro is Pascal's call. Locals x and z in each arm
// keep kind last, 132.
// 2. Allocation. The native has col r8 and row r10 and never touches r4. Here
// col takes r4, saved around calls, and row r8. col has 29 refs and row 25
// across 4 calls, so 4 * 4 < refs and find_reg tries r4 before the high
// registers. At depth 2 each ref weighs 3, so no form gets under 16: only depth
// 0 would (about 11 flat refs). The r4 saves also rotate reloads and stop the
// first two arms sharing their call only with each other. Whether the native's
// compiler weighted goto loops at all is a compiler question and out of scope;
// 02004288 shows the same with row. Pinning the two as a diagnostic gives 101.
// No effect, 144: `register` on both, swapped declarations, `col + originX`,
// `col + row * 11`, u8 occupied, an s32 return for Func_0200566c. Worse: for
// and do-while loops 167 (loop.c hoists row * 11), cell->kind in the arms 146.
#include "types.h"

struct PuzzleCell {
  u8 kind;
  u8 blocked;
  u8 state;
  u8 occupied;
};

struct SceneState {
  struct PuzzleCell cells[88];
  s32 count;
  s16 firstId;
  s16 endId;
  u16 tileEntry;
  s8 redrawA;
  s8 redrawB;
  s16 originX;
  s16 originZ;
};

extern struct SceneState gSceneState;
s32 GameFlag_Test(s32 flag);
void Func_0200566c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02003974(void) {
  struct SceneState *state = &gSceneState;
  u32 row;
  u32 col;

  row = 0;
rows:
  col = 0;
cols:
  {
    struct PuzzleCell *cell = &state->cells[row * 11 + col];
    s32 occupied = cell->occupied;

    if (GameFlag_Test(0x8fb) && cell->occupied == 2) {
      cell->occupied = 1;
      occupied = 1;
    }
    if (occupied == 2)
      Func_0200566c(0, 11, 1, 1, state->originX + col, state->originZ + row);
    else if (occupied == 1)
      Func_0200566c(1, state->cells[row * 11 + col].kind, 1, 1,
                    state->originX + col, state->originZ + row);
    else
      Func_0200566c(occupied, state->cells[row * 11 + col].kind, 1, 1,
                    state->originX + col, state->originZ + row);
  }
  if (++col <= 10)
    goto cols;
  if (++row <= 7)
    goto rows;
}
