#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "event_work.h"

void Actor_SetModeById(s32 id, s32 mode);
void Func_02000db0(void);

/// The scene's first entry: sets screen effect 256 and event mode 2, puts
/// objects 16 to 19 in mode 5 and 30 to 33 in mode 6, and runs
/// Func_02000db0 when the party came in by entrance 99.
s32 Func_02000688(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  /* FAKEMATCH: the effect's address pinned to r3 and an empty asm load it
   * before the screen effect is stored, as the native does; plain, 2
   * instructions differ in tla-en. */
  register s32 *effect asm("r3") = &work->screenEffect;
  struct Adventure *party = &gAdventure;

  asm volatile("" : "+r"(party));
  *effect = 256;
  party->eventMode = 2;
  Actor_SetModeById(16, 5);
  Actor_SetModeById(17, 5);
  Actor_SetModeById(18, 5);
  Actor_SetModeById(19, 5);
  Actor_SetModeById(30, 6);
  Actor_SetModeById(31, 6);
  Actor_SetModeById(32, 6);
  Actor_SetModeById(33, 6);
  if (party->entrance == 99)
    Func_02000db0();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
