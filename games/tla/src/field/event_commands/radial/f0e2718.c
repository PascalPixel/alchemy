/// @file
/// Places the event work's listed blocks.
// Part of radial.c.

#include "../radial_internal.h"
#include "module/field/event_commands.h"

/* VIEW(f0e2718): variable gData_02001000 as extern u8 gData_02001000[]; f0e150c has extern struct TouchCopies gData_02001000; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/field/event_commands/radial.c:2361: incompatible types in initialization. */
extern u8 gData_02001000__f0e2718[] __asm__("gData_02001000");
/* VIEW(f0e2718): routine Func_080dbb78 as struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer); f0e150c has struct MapCell__f0e150c *Func_080dbb78(s32 x, s32 z, s32 layer); tried as the definition's struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer), it does not compile in tla-ja: games/tla/src/field/event_commands/radial.c:2357: conflicting types for `Func_080dbb78'. */
extern struct MapCell *Func_080dbb78__f0e2718(s32 x, s32 z, s32 layer) __asm__("Func_080dbb78");

/// For each entry of the event work's list at 0x174c, calls Func_080201f0 with
/// its bytes and clears the collision code of the cell at its position.
void Func_080e2718(void) {
  u8 *base = gData_02001000__f0e2718;
  s32 i;

  for (i = 0; i < (s8)gData_02001000__f0e2718[0x194c]; i++) {
    u8 *entry = base + 0x174c + i * 8;

    Func_080201f0(entry[0], entry[1], 1, 1, entry[2], entry[3]);
    Func_080dbb78__f0e2718(entry[4] << 20, entry[5] << 20, 2)->collisionCode = 0;
  }
}
