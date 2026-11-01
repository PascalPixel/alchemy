// Draft, differ 117 of 190, skeleton gap 9 (tla-en, 2026-11-01)
// Draws the Gabomba Statue grid's movable pieces (actors first to end - 1):
// each live piece gets an OAM entry at state->oam on, its picture by the kind
// of the grid cell under it (four bytes a cell, 11 cells a row from the origin),
// and, while rotation is set, the rotation matrix built from it; Func_020055ec
// draws it at its depth.
// Left: the native sets tile (r11) before the rotation test and keeps the matrix
// at sp+0. Priority decides that: as u32, tile has 3 refs over 102 insns and the
// matrix 4 over 202, so tile is spilled instead (skeleton 14, r0 to r2 otherwise
// close). u16 tile puts the matrix on the stack, because the zero extension is
// hoisted to a 75-insn pseudo, but sets r11 after the test and takes r5 until
// then, which moves the temporaries (r5 for r0 and so on, and -63 comes from
// `subs #59` on the -4). Wanted: tile at exactly 4 refs. A two-step shift or
// index then tile in the one local gives 5, which puts tile above the scroll
// and breaks r9 to r11 (gap 12). Order: depth before sy gets pz r0, but x and
// z's registers need sy first as the native has it (r1, r2, gap 11, sy in r0).
// Settled: the effect is the 16-bit bitfield, written z, x, y; the sprite's two
// byte fields; the first words through a post-incremented pointer; x and sy
// masked before the first store. A stack struct for the matrix and the effect
// loses the bitfield read-modify-writes; a goto loop changes no refs.
#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "graphics/vram_tab.h"

struct Effect {
  u32 x : 16;
  u32 y : 16;
  u32 z : 16;
};

struct Sprite {
  u8 b0, b1, b2, b3, b4;
  u8 mode : 2;
  u8 b5rest : 6;
  u8 b6;
  u8 b7a : 1;
  u8 affine : 5;
  u8 b7b : 2;
  u32 w8;
};

struct Cell {
  u8 kind, b1, b2, b3;
};

struct SceneState {
  struct Cell cells[88];
  u8 unk160[4];
  s16 first;
  s16 end;
  s16 block;
  u8 unk16a[2];
  s16 originX;
  s16 originZ;
  u8 unk170[4];
  s32 rotation;
  struct Sprite oam[1];
};

extern struct SceneState gSceneState;
struct Actor *Actor_GetById(u32 id);
s32 AffineMatrix_BuildForEffect(struct Effect *effect);
void Func_020055ec(struct Sprite *oam, s32 depth);

void Func_02004710(void) {
  s32 affine = 0;
  s32 *scroll = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0xe4);
  struct SceneState *state = &gSceneState;
  struct Sprite *oam = state->oam;
  s32 scrollX = scroll[0] & 0xffff0000;
  s32 scrollZ = scroll[1] & 0xffff0000;
  u16 tile = gResourceTableEntries[state->block].offset >> 5;
  struct Effect effect;
  s32 id;

  if (state->rotation != 0) {
    effect.z = state->rotation;
    effect.x = 0x100;
    effect.y = 0x100;
    affine = AffineMatrix_BuildForEffect(&effect);
  }
  for (id = state->first; id < state->end; id++) {
    struct Actor *piece = Actor_GetById(id);
    s32 px = piece->x;
    if (px != 0) {
      s32 x = px - scrollX;
      s32 y = piece->y + 0x100000;
      s32 pz = piece->z;
      s32 z = pz - scrollZ;
      s32 depth = ((y + z) >> 16) + 58;
      s32 sy = z - y;
      s32 cellX = (px >> 20) - state->originX;
      s32 cellZ = (pz >> 20) - state->originZ;
      s32 kind = state->cells[cellZ * 11 + cellX].kind;
      u32 *p;
      x = ((x >> 16) - 8) & 0x1ff;
      sy = ((sy >> 16) - 8) & 255;
      p = (u32 *)oam;
      *p++ = 0;
      *p++ = sy | (x << 16) | 0x40002000;
      *p = (tile + (kind << 3)) | 0x800;
      if (state->rotation != 0) {
        oam->mode = 1;
        oam->affine = affine;
      }
      Func_020055ec(oam, depth);
      oam++;
    }
  }
}
