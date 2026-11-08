#include "types.h"
#include "graphics.h"
#include "window.h"
#include "ram_buffer.h"

extern const s16 gPortrait_CharacterMap[];
extern const s16 gPortrait_SpriteMap[];

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 flags);
s32 CreateSideObject(s32 id, s32 kind, s32 slot, struct Window *window, s32 x,
                     s32 y);

s32 Portrait_GetForSpeaker(u32 value) {
  s32 result = -1;
  s32 i = 0;

  switch (value) {
  case 47:
    value = 4;
    break;
  case 49:
    value = 6;
    break;
  case 48:
    value = 5;
    break;
  case 50:
    value = 7;
    break;
  case 51:
    value = 0;
    break;
  case 52:
    value = 1;
    break;
  case 53:
    value = 2;
    break;
  case 54:
    value = 3;
    break;
  }
  if (value < 16) {
    for (;;) {
      s32 key = gPortrait_CharacterMap[i];

      if (key == -1)
        break;
      if (key == value) {
        i++;
        result = gPortrait_CharacterMap[i];
        break;
      }
      i += 2;
    }
  } else {
    for (;;) {
      s32 key = gPortrait_SpriteMap[i];

      if (key == -1)
        break;
      if (key == value) {
        i++;
        result = gPortrait_SpriteMap[i];
        result += 128;
        break;
      }
      i += 2;
    }
  }
  return result;
}

struct Window *Func_0803d3c0(s32 arg0, s32 arg1, s32 x, s32 y) {
  struct Window *win;
  s32 minusFour;
  s32 offset;
  struct WindowWork *work;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  if (Portrait_GetForSpeaker(arg0) == -1) {
    return 0;
  }
  minusFour = -4;
  offset = minusFour;
  if (work->mode != 0) {
    win = Window_Create(x, y, 6, 5, 2);
    offset = 0;
  } else {
    win = Window_Create(x, y, 5, 5, 2);
  }
  if (win != 0) {
    CreateSideObject(arg0, arg1, -1, win, offset, minusFour);
  }
  return win;
}

void Func_0803d450(s32 id) {
  struct WindowWork *state;
  struct Window *work;
  s32 index;
  s32 i;
  struct WindowSprite *entity;

  state = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  work = state->windows;
  id = Portrait_GetForSpeaker(id);
  if (id == -1)
    return;

  if (state->resourceIds[1] == id) {
    index = 1;
  } else if (state->resourceIds[0] == id) {
    index = 0;
  } else {
    return;
  }

  id = state->outputIds[index];

  for (i = 0; i != UI_WINDOW_COUNT; i++, work++) {
    entity = work->output.head;
    if ((u8)entity->kind == 2 && (u8)entity->index == id) {
      Window_Close(work, 2);
      return;
    }
  }
}
