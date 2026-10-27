#include "shrine_of_the_sea_god.h"

struct ShrineOfTheSeaGodEffectParams {
  u8 unk00[0x18];
  u16 kind;
  u8 unk1a[0xe];
};

struct ShrineOfTheSeaGodPartyExtra {
  u8 unk000[0x256];
  s16 value;
};

extern u32 gFrameCount;

s32 Random16Far(void);
void Func_02003b18(void);
void Func_02003b20(s32 a, s32 b, s32 c, s32 d);
void Func_02003988(void);
void Func_02000994(void);
void Func_02000ce8(void);
struct ShrineOfTheSeaGodObject *Func_02002f88(s32 id);
void Func_020000b8(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase,
                   s32 life, struct ShrineOfTheSeaGodEffectParams *params);
void Func_02003854(s32 id);

void Func_02003740(void) {
  Func_02003854(8);
  Func_02003854(9);
  Func_02003854(10);
  if (gPartyState.entrance == 3 && GameFlag_Test(265) == 0)
    Func_02000994();
}

void Func_0200377c(void) {
  if (gPartyState.entrance == 1 && GameFlag_Test(265) == 0)
    Func_02000ce8();
}

void Func_020037a4(struct ShrineOfTheSeaGodObject *object) {
  struct ShrineOfTheSeaGodEffectParams params;
  u32 phase = gFrameCount & 7;
  s32 dx;
  s32 dy;

  if (phase == 0) {
    {
      /* FAKEMATCH: the native builds the mask before the random draw. */
      s32 mask = 15;

      dx = mask & Random16Far();
    }
    dy = Random16Far() & 7;
    params.kind = 673;
    Func_020000b8(object->x + ((dx - 8) << 16), object->y + ((dy - 8) << 16),
                  object->z, 0, 0x8000, phase, 0x100000, &params);
  }
}

void Func_0200380c(void) {
  struct ShrineOfTheSeaGodObject *object = Object_GetById(64);
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer = &map->layers[2];

  layer->x += 0x800000;
  Func_02003988();
  WaitTicks(1);
  if (object != NULL) {
    object->motionFlags = 0;
    object->y = 0x420000;
    object->update = Func_020037a4;
  }
}

void Func_02003854(s32 id) {
  struct ShrineOfTheSeaGodObject *object = Object_GetById(id);

  ObjectMotion_SetActionVariant(id, 3);
  Func_02003aa0(id);
  object->priorityFlags |= 2;
  object->motionFlags = 0;
  object->field18 = 0xcccc;
  object->field1c = 0xe666;
}

s32 Func_02003894(void) {
  s32 angle;

  if (GameFlag_Test(0x340)) {
    struct ShrineOfTheSeaGodPartyExtra *party =
        (struct ShrineOfTheSeaGodPartyExtra *)&gPartyState;

    /* FAKEMATCH: loads the party base before it builds the field offset. */
    __asm__("");
    if (party->value == 1) {
      if (GameFlag_Test(0x341))
        ShrineOfTheSeaGod_ObjPlaceAt(14, 360 << 16, 216 << 16, 0);
      else if (GameFlag_Test(0x342)) {
        /* FAKEMATCH: the function-scope angle temporary preserves its early
         * constant construction; a direct argument through the unbound helper
         * differs 4. */
        angle = 0x8000;
        ShrineOfTheSeaGod_ObjPlaceTurn(14, 328 << 16, 216 << 16, angle);
      }
    }
    GameFlag_ClearBit(0x340);
    GameFlag_ClearBit(0x341);
    GameFlag_ClearBit(0x342);
  }
  return 0;
}
