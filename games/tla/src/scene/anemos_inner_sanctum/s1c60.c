#include "types.h"
#include "adventure.h"

extern char SceneId_Ov694Room1;

struct Adventure2c2 {
  u8 unk000[0x2c2];
  u8 value;
};

void FieldEvent_WaitFrames(s32 frames);
void Func_020000f4(void);
void Func_02000158(void);
void Func_02003448(s32 scene, s32 entrance);
void Func_020034b0(void);

void Func_02001c60(void) {
  Func_020000f4();
  Func_020034b0();
  {
    struct Adventure2c2 *party = (struct Adventure2c2 *)&gAdventure;

    /* FAKEMATCH: a one-pass block loads the adventure before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    party->value = 2;
  }
  Func_02003448((s32)&SceneId_Ov694Room1, 99);
  FieldEvent_WaitFrames(40);
  Func_02000158();
}
