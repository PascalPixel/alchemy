#include "jupiter_lighthouse_room_7.h"
#include "event_inline.h"

s32 Func_020005a0(struct Actor *object);
void Func_02002b54(s32 x, s32 z, s32 a, s32 b);

void Func_020005e8(s32 mode) {
  struct Actor *object;
  s32 i, frame;

  // Goto loops: as for loops, the stored constants (3, 4, 0x208000) are hoisted
  // out of the loops and 42 of 181 instructions differ in tla-en.
  if (!Event_GetFlag(0xa1a)) {
    MapGrid_CopyCellsLayered(80, 46, 1, 7, 72, 9);
    MapGrid_CopyCellsLayered(80, 53, 8, 5, 72, 16);
    MapGrid_CopyCellsLayered(78, 47, 1, 1, 8, 74);
    Func_02002b54(0x800000, 0xc00000, 0, 4);
    if (mode == 1) {
      frame = 0;
    L_0200864c:
      i = 0;
    L_0200864e:
      object = Actor_Get(i + 10);
      if (Func_020005a0(object)) {
        object->motionFlags = 0;
        object->y -= 0x20000;
      }
      if (++i <= 5)
        goto L_0200864e;
      System_Wait(1);
      frame++;
      if (frame <= 15)
        goto L_0200864c;
    }
    i = 0;
  L_02008684:
    object = Actor_Get(i + 10);
    object->motionFlags = 3;
    i++;
    object->y = object->terrainHeight;
    if (i <= 5)
      goto L_02008684;
    /* FAKEMATCH(dead): an empty once-only block keeps the loop's exit branch
     * from being threaded to the function's end; as the plain if/else, 4 of 175
     * instructions differ in tla-en. */
    do {
    } while (0);
  } else {
    MapGrid_CopyCellsLayered(72, 46, 1, 7, 72, 9);
    MapGrid_CopyCellsLayered(72, 53, 8, 5, 72, 16);
    MapGrid_CopyCellsLayered(70, 47, 1, 1, 8, 74);
    Func_02002b54(0x800000, 0xc00000, 0, 6);
    FieldActor_Stop(gMainRoutineWork.currentOwner);
    Actor_SetPosition(20, 0, 0);
    if (mode == 1) {
      frame = 0;
    L_0200870a:
      i = 0;
    L_0200870c:
      object = Actor_Get(i + 10);
      if (Func_020005a0(object)) {
        object->motionFlags = 0;
        object->y += 0x20000;
      }
      if (++i <= 5)
        goto L_0200870c;
      System_Wait(1);
      frame++;
      if (frame <= 15)
        goto L_0200870a;
    }
    i = 0;
  L_02008744:
    object = Actor_Get(i + 10);
    if (Func_020005a0(object)) {
      object->motionFlags = 4;
      object->y = 0x208000;
    }
    if (++i <= 5)
      goto L_02008744;
  }
}
