// Near miss, differ 8 of 641 (tla-en, 2026-10-30): sched2 order in four
// pairs. The draft issues the stack spill of cursorEntry after cnt = 0 at
// entry, the spill of slot = 240 after the verticalScroll address in the row
// loop, and in the chosen-icon loop the u16 load of node->scale after its
// signed compare load, each a slot later than the listing; there the store of
// the new node->scale comes a slot earlier, before the transform's address.
// The sched2 dump ranks the listing's order lower in the first three (6
// against 7, 7 against 8, 1 against 8), which no source order changes; the
// fourth is a priority tie (61 and 61) that RTL order breaks the other way,
// because the reload copy of the transform's address sits just before its use.
// Measured without gain: cnt = 0 after the head load (12) or just before it
// (8), slot = 240 earlier in the loop (8 to 26), the three record pointers
// in other orders (22 to 26), the chosen-icon loop as a for loop (8) or with
// its sprite taken after the moves (12), its scale test reversed (18 and
// 101), its scale step chained into scaleX (8) or through a temporary (50),
// cnt and slot as other integer types (8 to 233), and a 4-minute permuter
// run (8).
// Earlier: differ 14 under -mthumb-slow-loads (2026-10-26), when no single
// empty asm, one-pass block, wrapping block or swap of neighbouring statements
// brought it closer (device search); before that differ 58 as The Broken
// Seal's SelectionMenu_Draw with this game's three-word projected position
// (frame 24), where assigning cursor and transfer after the head load, and an
// empty one-pass block after cnt = 0, measured 66 and 62.
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "select.h"

struct EffectPosition {
  s32 x;
  s32 y;
  s32 z;
};

extern u32 gVBlankCount;
extern s8 gMenu_CursorBobOffsets[];
extern u8 gData_0805c9c4[];

s32 BattleMotion_ProjectScaledPositionFar(s32 id,
                                          struct EffectPosition *projected);
s32 GameFlag_Test(s32 flag);
void Oam_AddSpriteAtDepth(s32 *entry, s32 slot);
struct SelectionNode *NodeChain_GetNodeAtCount(struct SelectionScreen *screen);
s32 VramCache_Load(s32 slot, s32 size, void *source);
void Func_0803e488(struct SelectionScreen *state, s32 index);

void SelectionMenu_Draw(void) {
  struct SelectionScreen *state = Ram_HeapSlots->menuWork;
  struct SelectionSprite *cursorEntry = &state->records[14].sprite;
  struct SelectionNode *cursor = &state->records[14];
  struct SelectionNode *transfer = &state->records[15];
  struct SelectionNode *node;
  struct SelectionSprite *e;
  struct SelectionSprite *te;
  s32 cnt = 0;
  s32 slot;
  s32 tmp;
  struct EffectPosition pos;

  node = state->head;
  while (node != 0) {
    e = &node->sprite;
    e->affine = 0;
    e->param = 0;
    e->x = node->x;
    e->y = node->y;
    slot = 240;
    if (state->verticalScroll != 0) {
      e->y += state->verticalScroll;
    } else if (node->x != node->targetX) {
      if (node->dx > 0) {
        if (node->x + node->dx > node->targetX) {
          node->x = node->targetX;
        } else {
          node->x = node->x + node->dx;
        }
      } else {
        if (node->x + node->dx < node->targetX) {
          node->x = node->targetX;
        } else {
          node->x = node->x + node->dx;
        }
      }
      e->x = node->x;
    } else if (cnt == state->cursorIndex) {
      slot = 241;
      if (cursor->kind != 0) {
        if (BattleMotion_ProjectScaledPositionFar(node->base, &pos) != -1) {
          cursor->targetX = pos.x;
          cursor->targetY = pos.y;
          if (cursor->scale == 0) {
            cursor->x = pos.x;
            cursor->y = pos.y;
            cursor->scale = 1;
          }
        }
      }
    }
    if (node->scale != 0) {
      if (GameFlag_Test(0x103) != 0) {
        if (state->records[14].kind == 1) {
          e->mode = 1;
        } else {
          e->mode = 0;
        }
        if (node->kind == 1) {
          e->mode = 1;
        }
      }
      Oam_AddSpriteAtDepth((s32 *)e, slot);
    }
    node = node->next;
    cnt++;
  }
  if (transfer->kind != 0) {
    node = NodeChain_GetNodeAtCount(state);
    te = &transfer->sprite;
    te->mode = 0;
    te->affine = 0;
    te->param = 0;
    te->mosaic = 0;
    te->colors = 1;
    te->size = 2;
    te->shape = 0;
    te->priority = 0;
    te->tile = transfer->tile;
    te->x = node->x - 4;
    te->y = node->y + (gMenu_CursorBobOffsets[(gVBlankCount >> 1) & 15] >> 1) - 4;
    if (transfer->scale != transfer->scaleEnd) {
      state->transform.scaleX = transfer->scale;
      state->transform.scaleY = transfer->scale;
      state->transform.angle = 0;
      te->param = AffineMatrix_BuildForEffect(&state->transform);
      te->affine = 3;
      te->x = te->x + 0xfff0;
      te->y = te->y + 0xf0;
      transfer->scale = transfer->scale + transfer->scaleStep;
    }
    if (GameFlag_Test(0x103) != 0) {
      te->mode = 1;
    }
    Oam_AddSpriteAtDepth((s32 *)te, 248);
  }
  Func_0803e488(state, 0);
  Func_0803e488(state, 1);
  node = state->path;
  while (node != 0) {
    e = &node->sprite;
    if (node->x != node->targetX) {
      node->x = node->x + node->dx;
    }
    if (node->y != node->targetY) {
      node->y = node->y + node->dy;
    }
    e->x = node->x;
    e->y = node->y;
    if (node->scale != node->scaleEnd) {
      node->scale = node->scale + node->scaleStep;
      state->transform.scaleX = node->scale;
      state->transform.scaleY = node->scale;
      state->transform.angle = 0;
      e->param = AffineMatrix_BuildForEffect(&state->transform);
      e->affine = 3;
      e->x = e->x + 0xfff8;
      e->y = e->y + 0xf8;
    } else {
      e->affine = 0;
      e->param = 0;
    }
    if (GameFlag_Test(0x103) != 0) {
      if (state->records[14].kind == 1) {
        e->mode = 1;
      } else {
        e->mode = 0;
      }
      if (node->kind == 1) {
        e->mode = 1;
      }
    }
    Oam_AddSpriteAtDepth((s32 *)e, 240);
    node = node->next;
  }
  if (cursor->kind != 0) {
    cnt = (s32)gData_0805c9c4;
    {
      void *src = (void *)(cnt + (((gVBlankCount >> 2) & 15) << 8));

      cursorEntry->tile = VramCache_Load(cursor->slot, 0x100, src);
    }
    if (cursor->targetX != cursor->x) {
      tmp = (cursor->targetX - cursor->x) >> 1;
      if (tmp != 0) {
        cursor->x = cursor->x + tmp;
      } else {
        cursor->x = cursor->targetX;
      }
    }
    if (cursor->targetY != cursor->y) {
      if ((tmp = (cursor->targetY - cursor->y) >> 1) != 0) {
        cursor->y = cursor->y + tmp;
      } else {
        cursor->y = cursor->targetY;
      }
    }
    cursorEntry->y =
        cursor->y + gMenu_CursorBobOffsets[(gVBlankCount >> 2) & 15] - 32;
    cursorEntry->x = cursor->x - 4;
    if (GameFlag_Test(0x103) != 0) {
      if (state->records[14].kind == 1) {
        cursorEntry->mode = 1;
      } else {
        cursorEntry->mode = 0;
      }
    }
    Oam_AddSpriteAtDepth((s32 *)cursorEntry, 248);
  }
  state->status += 1;
}
