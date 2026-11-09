#include "room.h"

void Func_020003b4(void) {
  if (Actor_Get(15)->z >> 20 == 14) {
    Func_020027ac(0, 65);
    Func_020027ac(2, 19);
  } else {
    Func_020027ac(0, 17);
    Func_020027ac(2, 67);
  }
  if (Actor_Get(16)->x >> 20 == 6) {
    Func_020027ac(1, 65);
    Func_020027ac(3, 3);
  } else {
    Func_020027ac(1, 49);
    Func_020027ac(3, 67);
  }
}

void Func_02000418(void) {
  struct Box box;

  if (Func_02004a8c(&box)) {
    Func_02004d10(box);
    Func_020003b4();
  }
}

void Func_02000444(s32 state) {
  if (state == 1) {
    MapGrid_CopyCells(29, 56, 3, 4, 10, 50);
    MapGrid_CopyCellsLayered(93, 56, 3, 4, 74, 50);
    Event_SetFlag(0xa44);
  }
}

void Func_02000480(s32 state) {
  if (state == 1) {
    MapGrid_CopyCells(29, 56, 3, 4, 18, 50);
    MapGrid_CopyCellsLayered(93, 56, 3, 4, 82, 50);
    Event_SetFlag(0xa45);
  }
}

void Func_020004bc(s32 state) {
  if (state == 1) {
    MapGrid_CopyCells(29, 56, 3, 4, 14, 50);
    MapGrid_CopyCellsLayered(93, 56, 3, 4, 78, 50);
    Event_SetFlag(0xa46);
  }
}

void Func_020004f8(void) {
  Event_PlaySound(0x248);
  Func_020027ac(0, 3);
  Event_SetFlag(0x220);
}

void Func_02000514(void) {
  struct Actor *object = Actor_Get(9);

  Actor_SetPosition(10, 0, 0);
  Actor_SetPosition(9, S16_AT(object, 100) << 16, S16_AT(object, 102) << 16);
  Event_SetFlag(0xa42);
  Event_PlaySound(136);
}

void Func_02000554(void) { Func_02001448(); }
