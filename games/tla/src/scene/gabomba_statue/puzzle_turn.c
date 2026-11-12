#include "types.h"
#include "actor.h"
#include "event_inline.h"

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
  s16 tileEntry;
  s8 redrawA;
  s8 redrawB;
  s16 originX;
  s16 originZ;
  s16 statuesReady;
  u8 unk172[2];
  s32 statueOffset;
  struct Sprite oam[10];
};

extern struct SceneState gSceneState;
extern const u8 gData_02005ad4[];
extern const u8 gData_02006ea8[];
void Func_02004288(s32 id, s32 value);
void Func_02005624(struct Actor *object, s32 mode);
u8 Func_02004380(s32 x, s32 z, s32 direction) {
  s32 i;

  {
    s32 column = x;

    /* FAKEMATCH(barrier): an empty asm on column that names the cell table, so
     * the table is loaded before x is added; without it, 12 of 13 instructions
     * differ in tla-en. */
    asm("" : "+l"(column) : "l"(gSceneState.cells));
    i = z * 11 + column;
  }
  gSceneState.cells[i].kind =
      gData_02006ea8[gSceneState.cells[i].kind * 2 + direction];
  return gSceneState.cells[i].kind;
}

void Func_020043a4(s32 value, s32 row) {
  struct Actor *object = Actor_Get(15);

  if (row <= 7)
    object->scaleY += -0x1f00;
  if (value == 1) {
    Func_02004288(gSceneState.firstId, 1);
    Func_02005624(object, 6);
    object->scaleY = 0x10000;
  }
}

#include "ram_buffer.h"

void Func_020043ec(s32 mode) {
  struct SceneState *state;
  struct Actor *object;
  struct PuzzleCell *cell;
  s32 x, z;
  u32 row, col;
  s32 found;

  if (mode != 1)
    return;
  state = &gSceneState;
  object = Actor_Get(*(s16 *)((u8 *)Ram_HeapSlots->effectWork + 26));
  x = (object->x >> 20) - state->originX;
  z = (object->z >> 20) - state->originZ;
  {
    s32 kind = state->cells[z * 11 + x].kind;

    cell = &state->cells[z * 11 + x];
    cell->occupied = mode;
    MapGrid_CopyCells(1, kind, 1, 1, state->originX + x, state->originZ + z);
  }
  object->x = 0;
  object->z = 0;
  found = 0;
  row = 0;
  for (row = 0; row <= 7; row++) {
    for (col = 0; col <= 10; col++) {
      if (state->cells[row * 11 + col].occupied == 2) {
        found = 1;
        break;
      }
    }
    if (found)
      return;
  }
  state->statuesReady = found;
}

void Func_020044b4(s32 mode) { Actor_SetModeById(gSceneState.firstId, mode); }

void Func_020056b4(void);
void Func_020056bc(void);

void Func_020044d0(s32 value) {
  struct SceneState *state = &gSceneState;
  s32 id;

  for (id = state->firstId; id < state->endId; id++) {
    struct Actor *object = Actor_Get(id);

    Func_02004380((object->x >> 20) - state->originX,
                  (object->z >> 20) - state->originZ, value);
  }
}

/// Redraws side A once; with the statues ready, moves their offset down by
/// 0x333 a frame for twenty frames, then clears it.
void Func_02004530(void) {
  struct SceneState *state = &gSceneState;
  s32 i;

  if (state->redrawA == 0) {
    MapGrid_CopyCellsLayered(1, 13, 1, 1, state->originX + 3,
                             state->originZ - 2);
    if (state->statuesReady != 0) {
      Event_PlaySound(248);
      Func_020056b4();
      FieldEvent_ResolvePendingActions(0);
      Func_020044b4(3);
      i = 19;
      do {
        s32 *offset = &state->statueOffset;

        *offset -= 0x333;
        System_Wait(1);
        i--;
      } while (i >= 0);
      Func_020044d0(0);
      Func_020056bc();
      state->statueOffset = 0;
    }
    state->redrawA = 1;
  }
}

void Func_020045d8(void) {
  if (gSceneState.redrawA != 0) {
    MapGrid_CopyCellsLayered(1, 12, 1, 1, gSceneState.originX + 3,
                             gSceneState.originZ - 2);
    gSceneState.redrawA = 0;
  }
}

/// Side B's twin of Func_02004530: the offset moves up instead.
void Func_02004620(void) {
  struct SceneState *state = &gSceneState;
  s32 i;

  if (state->redrawB == 0) {
    MapGrid_CopyCellsLayered(0, 13, 1, 1, state->originX + 7,
                             state->originZ - 2);
    if (state->statuesReady != 0) {
      Event_PlaySound(248);
      Func_020056b4();
      FieldEvent_ResolvePendingActions(0);
      Func_020044b4(2);
      i = 19;
      do {
        s32 *offset = &state->statueOffset;

        *offset += 0x333;
        System_Wait(1);
        i--;
      } while (i >= 0);
      Func_020044d0(1);
      Func_020056bc();
      state->statueOffset = 0;
    }
    state->redrawB = 1;
  }
}

void Func_020046c8(void) {
  if (gSceneState.redrawB != 0) {
    MapGrid_CopyCellsLayered(0, 12, 1, 1, gSceneState.originX + 7,
                             gSceneState.originZ - 2);
    gSceneState.redrawB = 0;
  }
}

#include "affine.h"
#include "graphics/vram_tab.h"

void Func_020055ec(struct Sprite *oam, s32 depth);

void Func_02004710(void) {
  s32 affine = 0;
  s32 *scroll = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0xe4);
  struct SceneState *state = &gSceneState;
  struct Sprite *oam = state->oam;
  s32 scrollX = scroll[0] & 0xffff0000;
  s32 scrollZ = scroll[1] & 0xffff0000;
  u16 tile = gResourceTableEntries[state->tileEntry].offset >> 5;
  struct AffineTransform effect;
  s32 id;

  if (state->statueOffset != 0) {
    effect.angle = state->statueOffset;
    effect.scaleX = 0x100;
    effect.scaleY = 0x100;
    affine = AffineMatrix_BuildForEffect(&effect);
  }
  for (id = state->firstId; id < state->endId; id++) {
    struct Actor *piece = Actor_Get(id);
    s32 px = piece->x;
    if (px != 0) {
      s32 x = px - scrollX;
      s32 y = piece->y + 0x100000;
      s32 pz = piece->z;
      s32 z = pz - scrollZ;
      s32 sy = z - y;
      s32 depth = ((y + z) >> 16) + 58;
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
      if (state->statueOffset != 0) {
        oam->mode = 1;
        oam->affine = affine;
      }
      Func_020055ec(oam, depth);
      oam++;
    }
  }
}

#include "iwram_call.h"
#include "task.h"

void Func_020038ac(void);
void Func_02003974(void);
s32 Func_02003a64(s32 value);

s32 Func_020048a4(s32 x, s32 z, s32 value, s32 id) {
  struct SceneState *state = &gSceneState;
  s32 count;

  if (Event_GetFlag(0x109)) {
    Func_020038ac();
    Func_02003974();
    count = state->count;
    Task_AddFrame(Func_02004710, TASK_PASS_VBLANK);
  } else {
    Iwram_ClearWords(state, sizeof(struct SceneState));
    state->originX = x;
    state->originZ = z;
    Func_020038ac();
    count = Func_02003a64(value);
    Func_02004288(id, 0);
    Task_AddFrame(Func_02004710, TASK_PASS_VBLANK);
  }
  Actor_Get(id)->unk5c[0] = 2;
  return count;
}

#include "call.h"

extern const u8 gData_02006f38[], gData_02006ec0[];
s32 Math_Cosine(s32 angle);
void Func_02005634(struct Actor *object, const u8 *script);

void Func_02004938(struct Actor *ball, s32 step) {
  s16 *colp = (s16 *)((u8 *)ball + 100);
  s32 col = *colp;
  s16 *rowp = (s16 *)((u8 *)ball + 102);
  s32 row = *rowp;
  u8 *headingp = (u8 *)ball + 98;
  s32 heading = *headingp;
  s32 angle = heading << 14;
  s32 cell = row * 11 + col;
  struct SceneState *board = &gSceneState;
  s32 piece = board->cells[cell].kind;
  s32 goal = board->cells[cell].occupied;
  s32 key = (heading << 8) + piece;
  s32 d = step - 8;
  s32 arc = step << 10;
  s32 s = Value2((s32 (*)())Iwram_MulQ16, 0x80000, Math_Sine(arc));
  s32 c = Value2((s32 (*)())Iwram_MulQ16, 0x80000, Math_Cosine(arc));
  s32 z;
  s32 x;

  x = ((board->originX + col) << 20) + 0x80000;
  z = ((board->originZ + row) << 20) + 0x80000;
  if (goal == 2) {
    heading = row = col = 0;
    if (((u8 *)ball)[99] == 1) {
      Sound_Play(204);
      Func_02005634(ball, gData_02006f38);
      ((u8 *)ball)[99] = goal;
    }
    goto done;
  }
  switch (key) {
  case 514:
  case 515:
    col--;
    x -= d << 16;
    break;
  case 2:
  case 3:
    col++;
    x += d << 16;
    break;
  case 257:
  case 259:
    row++;
    z += d << 16;
    break;
  case 769:
  case 771:
    row--;
    z -= d << 16;
    break;
  case 516:
  case 520:
    row++;
    heading = 1;
    x = x - s + 0x80000;
    z = z - c + 0x80000;
    angle = 0x8000 - arc;
    break;
  case 773:
  case 777:
    col--;
    heading = 2;
    x = x + c - 0x80000;
    z = z - s + 0x80000;
    angle = 0xc000 - arc;
    break;
  case 6:
  case 8:
    row--;
    heading = 3;
    x = x + s - 0x80000;
    z = z + c - 0x80000;
    angle = -arc;
    break;
  case 263:
  case 265:
    col++;
    heading = 0;
    x = x - c + 0x80000;
    z = z + s - 0x80000;
    angle = 0x4000 - arc;
    break;
  case 519:
  case 521:
    row--;
    heading = 3;
    x = x - s + 0x80000;
    z = z + c - 0x80000;
    angle = arc + 0x8000;
    break;
  case 262:
  case 264:
    col--;
    heading = 2;
    x = x + c - 0x80000;
    z = z + s - 0x80000;
    angle = arc + 0x4000;
    break;
  case 5:
  case 9:
    row++;
    heading = 1;
    x = x + s - 0x80000;
    z = z - c + 0x80000;
    angle = arc;
    break;
  case 772:
  case 776:
    col++;
    heading = 0;
    x = x - c + 0x80000;
    z = z - s + 0x80000;
    angle = arc + 0xc000;
    break;
  case 11:
    heading = row = col = 0;
    break;
  case 267:
    heading = row = col = 0;
    break;
  case 523:
    heading = row = col = 0;
    break;
  case 10:
  case 266:
  case 522:
  case 778:
    if (((u8 *)ball)[99] == 1) {
      ball->x = x;
      ball->y = 0;
      ball->z = z;
      ball->facing = angle;
      if (x >> 20 == 24 && z >> 20 == 14) {
        ((u8 *)ball)[99] = 7;
      } else {
        Sound_Play(204);
        Func_02005634(ball, gData_02006f38);
        ((u8 *)ball)[99] = 2;
      }
    }
    break;
  case 779:
    heading = row = col = 0;
    break;
  default:
    heading = row = col = 0;
    if (((u8 *)ball)[99] == 1) {
      Sound_Play(132);
      Func_02005634(ball, gData_02006ec0);
      ((u8 *)ball)[99] = 2;
    }
    break;
  }
done:
  if (step == 15) {
    *colp = col;
    *rowp = row;
    *headingp = heading;
  }
  if (((u8 *)ball)[99] == 1) {
    ball->y = 0;
    ball->x = x;
    ball->z = z;
    ball->facing = angle;
    ball->unk4c++;
  }
}
