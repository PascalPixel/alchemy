#include "trial_road.h"

/// The head of a mover's slot, as Func_02004224 fills it.
struct TrialRoadMover {
  s16 id;
  s16 value;
  s16 unk04;
  s16 unk06;
  s16 unk08;
  s16 unk0a;
};

/// A view of the map work's byte 23.
struct MapWorkByte23 {
  u8 unk00[0x17];
  u8 bits0 : 2;
  // XXX: what bits 2 and 3 select; the map code that reads them would settle
  // it.
  u8 bits2 : 2;
  u8 bits4 : 4;
};

void Func_02004188(s32 a, s32 b);
struct TrialRoadMover *Func_02004224(s32 index, s32 id, s32 value, s32 low,
                                     s32 high);

/// Sets up the Trial Road room with five movers: objects 16 to 20 in slots 0
/// to 4, the first three over a range of 64 and the last two over 32, each
/// with 240, 120 and 150; objects 16 to 18 are put away (Func_0200078c) once
/// flags 0x363 to 0x365 are set and placed otherwise; then object 15.
void Func_02001c6c(void) {
  struct TrialRoadMover *mover;

  Func_02004188(0, 240);
  mover = Func_02004224(0, 16, 0, 0, 64);
  mover->unk06 = 240;
  mover->unk08 = 120;
  mover->unk0a = 150;
  mover = Func_02004224(1, 17, 0, 0, 64);
  mover->unk06 = 240;
  mover->unk08 = 120;
  mover->unk0a = 150;
  mover = Func_02004224(2, 18, 0, 0, 64);
  mover->unk06 = 240;
  mover->unk08 = 120;
  mover->unk0a = 150;
  mover = Func_02004224(3, 19, 0, 0, 32);
  mover->unk06 = 240;
  mover->unk08 = 120;
  mover->unk0a = 150;
  mover = Func_02004224(4, 20, 0, 0, 32);
  mover->unk06 = 240;
  mover->unk08 = 120;
  mover->unk0a = 150;
  if (!GameFlag_Test(0x363))
    Func_020058e0(16, 0, 0);
  else
    Func_0200078c(16);
  if (!GameFlag_Test(0x364))
    Func_020058e0(17, 0, 0);
  else
    Func_0200078c(17);
  if (!GameFlag_Test(0x365))
    Func_020058e0(18, 0, 0);
  else
    Func_0200078c(18);
  ((struct MapWorkByte23 *)Ram_HeapSlots->mapWork)->bits2 = 1;
  Func_020006b4(15);
  Func_0200062c(15);
}
