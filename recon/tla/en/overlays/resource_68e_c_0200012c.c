// Matches (tla-en, 2026-11-08, -mtune=gba); differ 0 in all six editions.
// An empty once-only block in place of the empty asm between the two blend
// stores (4 to 0; 8 with neither), so no device is left. BLDALPHA's 0x80c through a
// one-constant local gives the native's r8 and r7 (29 without).
// Earlier: the scene start that sets the event work's 0x1ac
// word to 0x100, the leader's bit 5 at 0x23, blending 0x3f42 and 0x80c, modes
// 2 for objects 24 and 25 and, when gMainRoutineWork's 0x1e2 halfword is 1 and
// flag 0x200 is set, alpha 0x1000.
// The blend value as one local set at its first store places it in r10 as the
// native does (33 to 27). Left: the native gives BLDCNT's address r8 and
// BLDALPHA's r7 (here the other way round), and loads gMainRoutineWork before the
// 0x100 store. Tried: register pointers in either order (47), BLDCNT pinned to
// r8 (25, with its copies then reversed), u16 or volatile stores.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Actor_GetById();
s32 Actor_SetModeById();
s32 Event_StartScreenTransition();
s32 Func_020010f4();
s32 GameFlag_Test();
extern u8 gMainRoutineWork[];

s32 Func_0200012c(void) {
  s32 t0;
  s32 t3;
  s32 t4;
  s32 t7;
  s32 blend;
  s32 alpha;

  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x100;
  t0 = Value1(Actor_GetById, FIELD_AT(gMainRoutineWork, s32, 532));
  FIELD_AT((t0 + 0x23), u8, 0) = (0x20 | FIELD_AT((t0 + 0x23), u8, 0));
  blend = 0x3f42;
  FIELD_AT(0x4000050, u16, 0) = blend;
  // A once-only block: its end holds the order around it (as the plain
  // statements, 8 of 80 differ in tla-en).
  do {
  } while (0);
  alpha = 0x80c;
  FIELD_AT(0x4000052, u16, 0) = alpha;
  Call2(Actor_SetModeById, 0x18, 2);
  Call2(Actor_SetModeById, 0x19, 2);
  t3 = Value1(Actor_GetById, 0x18);
  FIELD_AT((t3 + 0x23), u8, 0) = 2;
  t4 = Value1(Actor_GetById, 0x19);
  FIELD_AT((t4 + 0x23), u8, 0) = 2;
  Event_StartScreenTransition();
  if (FIELD_AT(gMainRoutineWork, s16, 482) == 1) {
    Func_020010f4();
    t7 = Value1(GameFlag_Test, 0x200);
    if (t7 != 0) {
      FIELD_AT(0x4000050, u16, 0) = blend;
      FIELD_AT(0x4000052, u16, 0) = 0x1000;
    }
  }
  return 0;
}
