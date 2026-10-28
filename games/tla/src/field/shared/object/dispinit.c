#include "types.h"
#include "actor_view.h"
#include "adventure.h"

void WaitFrames(s32 frames);

extern const s32 gData_0802f098[];
extern const s32 gData_0802f0b0[];
extern const s32 gData_0802f0c8[];
extern const s32 gData_0802f0e0[];
extern const s32 gData_0802f0f8[];
extern const s32 gData_0802f110[];
extern const s32 gData_0802f128[];
extern const s32 gData_0802f140[];
extern const s32 gData_0802f158[];
extern const s32 gData_0802f170[];
extern const s32 gData_0802f188[];
extern const s32 gData_0802f1a0[];
extern const s32 gData_0802f1b8[];
extern const s32 gData_0802f1e8[];

void Func_08023510(struct ActorView *object, s32 argument) {
  Actor_StartScript(object, gData_0802f1e8);
  object->argument = argument;
}

/// Starts the dispatch script for the adventure's render mode, waiting a
/// frame first in mode 9.
void Func_08023524(struct ActorView *object) {
  const s32 *script;
  struct Adventure *party = &gAdventure;

  /* FAKEMATCH: ⚓️ loads the adventure's address before it builds the field
   * offset 0x212 in r2; the empty asm holds r2 until the address is loaded.
   * Plainly the offset is started first (differ 2). */
  asm("" : "+l"(party) : : "r2");
  switch (party->renderMode) {
  case 0:
    script = gData_0802f140;
    break;
  case 1:
    script = gData_0802f158;
    break;
  case 2:
    script = gData_0802f0b0;
    break;
  case 3:
    script = gData_0802f0c8;
    break;
  case 4:
    script = gData_0802f0e0;
    break;
  case 5:
    script = gData_0802f0f8;
    break;
  case 6:
    script = gData_0802f170;
    break;
  case 7:
    script = gData_0802f188;
    break;
  case 8:
    script = gData_0802f1a0;
    break;
  case 9:
    WaitFrames(1);
    script = gData_0802f1b8;
    break;
  case 10:
    script = gData_0802f110;
    break;
  case 11:
    script = gData_0802f128;
    break;
  case 12:
    script = gActorScript_EndScript;
    break;
  default:
    script = gData_0802f098;
    break;
  }
  Actor_StartScript(object, script);
}
