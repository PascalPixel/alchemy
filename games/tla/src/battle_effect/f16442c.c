#include "types.h"
#include "ram_buffer.h"
#include "battle_fx_work.h"

struct FormationScale {
  s32 x;
  s32 y;
};

struct ScaledSprite;

extern const struct FormationScale gData_08196e5c;
extern const struct FormationScale gData_08196e64;
extern const u8 gData_08198984[];
extern const u8 gData_08198990[];
extern const u8 gData_0819899c[];
extern const u8 gData_081989a4[];
extern const u8 gData_081989ac[];
extern const u8 gData_081989b4[];
extern const u8 gData_081989bc[];
extern const u8 gData_081989c4[];
extern const s8 gData_081989cc[];
extern const u8 gData_081989d7[];

void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite,
                                       s32 *position, s32 *scale, s32 mode);

/// The Broken Seal's BattleEffect_PlaceFormationObjects with The Lost Age's
/// three more formations (4: eleven objects from signed columns, 5: twelve in
/// a 4x3 grid, 6: eight more), in the order 0, 5, 1, 2, 6, 3, 4.
void Func_0816442c(s32 formation, s32 x, s32 z) {
  struct BattleFxWork *work =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
  struct FormationScale normal = gData_08196e5c;
  struct FormationScale big = gData_08196e64;
  s32 pos[4];
  s32 i;
  void **objects;

  pos[3] = 0;
  pos[1] = 255 << 16;
  switch (formation) {
  case 0:
    objects = work->objects;
    for (i = 0; i != 9; i++) {
      pos[0] = (i % 3 << 21) + x;
      pos[2] = (i / 3 << 21) + z;
      Render_ApplyProjectedPlacementFar(*objects++, pos, &normal.x, 0);
    }
    break;
  case 5:
    for (i = 0; i != 12; i++) {
      pos[0] = (i % 4 << 21) + x;
      pos[2] = (i / 4 << 21) + z;
      Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
    }
    break;
  case 1:
    for (i = 0; i != 12; i++) {
      pos[0] = (gData_08198984[i] << 16) + x - (16 << 16);
      pos[2] = (gData_08198990[i] << 16) + z - (32 << 16);
      Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
    }
    break;
  case 2:
    for (i = 0; i != 8; i++) {
      pos[0] = (gData_0819899c[i] << 16) + x + (16 << 16);
      pos[2] = (gData_081989a4[i] << 16) + z;
      Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
    }
    break;
  case 6:
    for (i = 0; i != 8; i++) {
      pos[0] = (gData_081989ac[i] << 16) + x;
      pos[2] = (gData_081989b4[i] << 16) + z;
      Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
    }
    break;
  case 3:
    for (i = 0; i != 8; i++) {
      pos[0] = (gData_081989bc[i] << 16) + x;
      pos[2] = (gData_081989c4[i] << 16) + z;
      Render_ApplyProjectedPlacementFar(work->objects[i], pos, &big.x, 0);
    }
    break;
  case 4:
    for (i = 0; i != 11; i++) {
      pos[0] = (gData_081989cc[i] << 16) + x;
      pos[2] = (gData_081989d7[i] << 16) + z;
      Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
    }
    break;
  }
}
