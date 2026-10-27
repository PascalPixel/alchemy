// 26 October 2026, under -mthumb-slow-loads: differ 8 of 360: the frame's sub
// sp and its store at the start, and the native issues a load one slot later
// after a store, where alchemy-gcc issues it at once; alchemy permute (5 to 6
// minutes) and an empty asm or one-pass block after any one statement do not
// move it.
// Draft, differ 8 of 360: the Shrine of the Sea God's descent: the map's
// three layers and the party move down while the blend steps from 0x80c to
// 0x1000 and Func_0200094c draws the steps. Left: the frame's sub sp and
// the third layer's spill one load later than the native, and the
// leader's id loaded after the 1 of its SetActionVariant call.
#include "../../../../games/tla/src/scene/shrine_of_the_sea_god/shrine_of_the_sea_god.h"
#include "io_reg.h"
#include "iowrite.h"
#include "call.h"

void Func_02003988(void);
void Func_020039a0(void);
void Func_020039b8(s32 a, s32 b, s32 c);
void Func_020039c0(void);
void Func_02003854(s32 actor);
void Func_0200087c(void);
void Func_0200092c(void);
void Func_0200094c(s32 x, s32 y);

void Func_02000994(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  struct ShrineOfTheSeaGodMapLayer *layer2 = &map->layers[2];
  struct ShrineOfTheSeaGodObject *leader =
      Object_GetById(gPartyState.currentOwner);
  struct ShrineOfTheSeaGodObject *ride = Func_02003ad0();
  struct ShrineOfTheSeaGodObject *a = Object_GetById(8);
  struct ShrineOfTheSeaGodObject *b = Object_GetById(9);
  struct ShrineOfTheSeaGodObject *c = Object_GetById(10);
  u32 i;

  Func_020039d8();
  Call1((void (*)())Func_02003b08, 0);
  ShrineOfTheSeaGod_CamMove(-1, -1, -1, 0);
  Object_GetById(gPartyState.currentOwner)->motionFlags = 0;
  Call2((void (*)())ObjectMotion_SetActionVariant, gPartyState.currentOwner, 1);
  Call2((void (*)())ObjectMotion_SetActionVariant, 8, 1);
  Call2((void (*)())ObjectMotion_SetActionVariant, 9, 1);
  Call2((void (*)())ObjectMotion_SetActionVariant, 10, 1);
  Func_020039a0();
  Call1((void (*)())WaitFrames, 1);
  ShrineOfTheSeaGod_CopyRect(46, 51, 6, 56, 18, 25);
  ShrineOfTheSeaGod_CopyRect(84, 100, 6, 56, 18, 6);
  ShrineOfTheSeaGod_CopyRect(59, 124, 9, 12, 12, 3);
  Io_Write16(0x80c, &REG_BLDALPHA);
  Call1((void (*)())WaitFrames, 1);
  layer0->y += -0x440000;
  layer1->y += -0x140000;
  layer2->y += 0xc0000;
  leader->z += -0xc0000;
  ride->y += -0xc0000;
  a->z += -0xc0000;
  b->z += -0xc0000;
  c->z += -0xc0000;
  Func_02003988();
  Call1((void (*)())WaitFrames, 4);
  ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 0x209;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call3((void (*)())Func_020039b8, 0x10000, 0x10000, 0x10000);
  Call1((void (*)())Func_02003b48, 245);
  i = 0;
  do {
    i++;
    Func_0200087c();
  } while (i <= 24);
  i = 0;
  do {
    i++;
    Func_0200092c();
  } while (i <= 24);
  Call2((void (*)())Func_0200094c, 84, 93);
  Call2((void (*)())Func_0200094c, 84, 86);
  Io_Write16(0x90b, &REG_BLDALPHA);
  Call2((void (*)())Func_0200094c, 84, 79);
  Call2((void (*)())Func_0200094c, 84, 72);
  Io_Write16(0xa0a, &REG_BLDALPHA);
  Call2((void (*)())Func_0200094c, 84, 65);
  Call2((void (*)())Func_0200094c, 84, 58);
  Io_Write16(0xb09, &REG_BLDALPHA);
  Call2((void (*)())Func_0200094c, 84, 51);
  Call2((void (*)())Func_0200094c, 65, 100);
  Io_Write16(0xc08, &REG_BLDALPHA);
  Call2((void (*)())Func_0200094c, 65, 93);
  Call2((void (*)())Func_0200094c, 65, 86);
  Io_Write16(0xd07, &REG_BLDALPHA);
  Call2((void (*)())Func_0200094c, 65, 79);
  Call2((void (*)())Func_0200094c, 65, 72);
  Io_Write16(0xe06, &REG_BLDALPHA);
  Call2((void (*)())Func_0200094c, 65, 65);
  Call2((void (*)())Func_0200094c, 65, 58);
  Io_Write16(0xf05, &REG_BLDALPHA);
  Call2((void (*)())Func_0200094c, 65, 51);
  Call3((void (*)())Func_020039b8, 0, 0x10000, 0x10000);
  Io_Write16(0x1004, &REG_BLDALPHA);
  Call1((void (*)())WaitFrames, 10);
  Call1((void (*)())Func_02003b48, 390);
  Call3((void (*)())Func_020039b8, -1, -1, 0xe666);
  Io_Write16(0x1003, &REG_BLDALPHA);
  Call1((void (*)())WaitFrames, 10);
  Io_Write16(0x1002, &REG_BLDALPHA);
  Call1((void (*)())WaitFrames, 10);
  Io_Write16(0x1001, &REG_BLDALPHA);
  Call1((void (*)())WaitFrames, 10);
  Io_Write16(0x1000, &REG_BLDALPHA);
  ShrineOfTheSeaGod_CopyRect(103, 51, 6, 56, 18, 25);
  ShrineOfTheSeaGod_CopyRect(46, 108, 9, 12, 12, 3);
  Call1((void (*)())WaitFrames, 10);
  Func_020039c0();
  ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 0x204;
  Call1((void (*)())Func_02003854, 8);
  Call1((void (*)())Func_02003854, 9);
  Call1((void (*)())Func_02003854, 10);
  Object_GetById(gPartyState.currentOwner)->priorityFlags |= 1;
  leader->motionFlags = 3;
  leader->y = 0;
  leader->height = 0;
  Func_020039e0();
}
