// Pin-free trial (tla-en): listing 206 instructions, draft 206, common 186,
// differ 40. Earlier register-allocation notes describe the prior pinned form.

#include "types.h"

struct StepNode {
  u8 unk00[4];
  struct StepNode *next;
  u8 unk08[2];
  u16 id;
  u8 unk0c[4];
  s16 y;
  u8 unk12[2];
  u16 speed;
  u8 unk16[2];
  s16 targetY;
  u16 targetZ;
};

struct StepMenu {
  u8 unk000[8];
  u16 scrollUp;
  u16 moreAbove;
  u8 unk00c[0x30];
  u16 scrollDown;
  u16 moreBelow;
  u8 unk040[0x308];
  struct StepNode *nodes;
  u8 unk34c[8];
  u16 entryIds[16];
  u16 entryKinds[16];
  u16 count;
  u16 baseY;
  u16 baseZ;
  u8 unk39a[2];
  u16 top;
  u16 cursor;
  u8 unk3a0[2];
  u16 status;
};

void WaitFrames(s32 frames);
void Menu_ReloadNodeResource(struct StepMenu *state, u32 index);
void Menu_LoadSelectionNodeResource(struct StepMenu *state, u32 index);
void Menu_ScrollSelectionList(struct StepMenu *state, u32 mode);
void MenuSelection_SetupEntry(u32 id, u32 kind, struct StepNode *node,
                              u32 flag);
void Menu_OpenSelectionWindow(s32 type, u32 value);

void Menu_StepLeft(struct StepMenu *state) {
  struct StepNode *node;
  u16 *ids;
  s32 y; /* Earlier pinned trial: keeps the row offset in r1 */
  s32 i;

  Menu_ReloadNodeResource(state, state->cursor);
  state->status = 33;
  WaitFrames(1);
  if (state->count > 5) {
    if ((state->cursor | state->top) != 0) {
      if (state->cursor == 1 && state->top != 0) {
        state->scrollUp = 8;
        state->top--;
        Menu_ScrollSelectionList(state, 0);
        if (state->top == 0)
          state->moreAbove = 0;
        state->moreBelow = 1;
      } else {
        state->cursor--;
      }
    } else {
      node = state->nodes;
      y = 64;
      {
        s32 z = 0; /* Earlier pinned trial: the zero goes through r0 */
        state->moreBelow = i = z;
      }
      while (node->next != NULL) {
        node->targetY = node->y + y;
        node->speed = 12;
        node = node->next;
        y -= 16;
      }
      node = state->nodes;
      while (node->y != node->targetY)
        WaitFrames(1);
      i = 0;
      while (i != state->count - 5)
        i++;
      node = state->nodes;
      state->top = i;
      state->cursor = 4;
      if (node != NULL) {
        /* FAKEMATCH: the index is scaled and added to the base before the field
         * offset */
        ids = (u16 *)((i * 2 + (s32)state) +
                      (s32)((struct StepMenu *)0)->entryIds);
        do {
          MenuSelection_SetupEntry(ids[0], ids[16], node, 1);
          node = node->next;
          ids++;
        } while (node != NULL);
      }
      node = state->nodes;
      y = state->baseY;
      while (node->next != NULL) {
        node->targetY = y;
        node->speed = 0xfff4;
        node = node->next;
        y += 16;
      }
      state->moreAbove = 1;
    }
  } else if (state->cursor != 0) {
    state->cursor--;
  } else {
    state->cursor = state->count - 1;
  }
  state->status = 1;
  Menu_LoadSelectionNodeResource(state, state->cursor);
  WaitFrames(1);
  Menu_OpenSelectionWindow(state->nodes->id, 0);
  WaitFrames(1);
}
