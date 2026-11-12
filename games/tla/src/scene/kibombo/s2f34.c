/// @file
/// Scene code of overlay 0x670, in ROM order, from s2f34's offset up to the next object the overlay links.

#include "kibombo.h"

s32 Math_Cosine(s32 angle);
void Func_02005a2c(struct KibomboObject *object, const void *script);
void Effect_AdvanceMotion(struct KibomboObject *object);
extern u32 gData_02005ca4[];

void Func_02002f34(void) {
  s32 phase = gFrameCount & 3;

  if (phase == 0) {
    struct KibomboObject *object =
        Import_ActorCreate(0x16e, 0x1080000, 0x200000, 0x13c0000);
    s32 value;

    if (object != 0) {
      object->field44 = Math_Cosine(((Random16Far() << 13) >> 16) + 0x3000);
      object->field48 = 0x10000;
      object->field4c = 0x40000 - (u16)Random16Far();
      value = (u16)Random16Far() + 0xffff8000;
      object->field64 = value;
      object->motionFlags = 0;
      Actor_SetSingleChildFlags(object, 0);
      Func_02005a24(object, 2);
      Func_02005a2c(object, gData_02005ca4);
      object->sprite->priority = 1;
      object->field64 = 0;
      *(s32 *)&object->unk2c[4] = 0;
      *(s32 *)&object->unk2c[8] = 0;
      object->update = Effect_AdvanceMotion;
    }
  }
}

void Func_02002fec(struct KibomboObject *object) {
  object->x += object->field64 << 8;

  object->y += 0x8000;

  object->scaleX += 0x51e;
  object->scaleY += 0x51e;
  object->field64 += 2;
  if (--object->field68 == 0)
    Func_02005a3c(object);
}

void Func_02003030(void) {
  if ((gFrameCount & 63) == 0) {
    struct KibomboObject *object =
        Import_ActorCreate(0x11d, 0x580000, 0x100000, 0x13a0000);
    u8 *part;
    s32 mask;

    if (object != 0) {
      object->field5c = 2;
      object->motionFlags = 0;
      object->field64 = 0;
      object->priorityFlags &= ~1;
      mask = 13;
      part = (u8 *)object->sprite;
      part[9] = (part[9] & -mask) | 8;
      Actor_SetSingleChildFlags(object, 0);
      object->scaleX = 0x8000;
      object->scaleY = 0x8000;
      object->field68 = 60;
      object->update = Func_02002fec;
      Func_02005a24(object, 5);
      Animation_SetStateFlags(object, 1);
    }
  }
}

void Func_020030b8(struct KibomboObject *object) {
  if (object->field64 != 0) {
    object->field64--;
  } else {
    object->field64 = ((Random16Far() * 10) >> 16) + 10;
    if (object->field66 != 0) {
      object->scaleX = 0x18000;
      object->field66 = 0;
    } else {
      object->scaleX = 0xfffe8000;
      object->field66 = 1;
    }
  }
}

void Func_02003104(s32 x, s32 y, s32 z) {
  struct KibomboObject *object = Import_ActorCreate(0x106, x, y, z);
  struct FieldSprite *sprite;

  if (object != 0) {
    object->motionFlags = 0;
    object->priorityFlags &= ~1;
    object->scaleY = 0x20000;
    object->field64 = ((Random16Far() * 10) >> 16) + 10;
    object->field66 = 0;
    object->update = Func_020030b8;
    sprite = object->sprite;
    sprite->priority = 1;
    Actor_SetSingleChildFlags(object, 0);
    Func_02005a24(object, 5);
    Animation_SetStateFlags(object, 1);
  }
}

#include "io_reg.h"

struct KibomboEffectWork {
  u8 unk000[0x7c4];
  s16 field7c4;
};

static inline void Kibombo_Spawn(s32 x, s32 y, s32 z) {
  Func_02003104(x, y, z);
}

void Func_02003184(void) {
  s32 x = 0x1b80000;

  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  Kibombo_Spawn(x, 0x280000, 0x1c20000);
  Kibombo_Spawn(x, 0x2e0000, 0x1b60000);
  Kibombo_Spawn(x, 0x340000, 0x1aa0000);
  Kibombo_Spawn(x, 0x3a0000, 0x19e0000);
  /* FAKEMATCH(dead): an empty once-only block before the last spawn, which
   * shifts x's shift after the second register store as the native has it;
   * without it, 2 of 43 instructions differ in tla-en. */
  do {
  } while (0);
  Func_02003104(x, 0x400000, 0x1920000);
}

#include "scene_tables.h"

void Func_020031f0(s32 id) {
  Func_02005b8c(id);
  Actor_SetPartAttribute(Actor_GetById(id), 15);
  Actor_SetSpritePriority(id, 1);
  Actor_GetById(id)->priorityFlags |= 2;
}

void Func_02003220(void) {
  if (GameFlag_Test(265)) {
    if (GameFlag_Test(0x200) == 0)
      Func_0200057c();
  } else {
    GameFlag_SetBit(0x200);
  }
}

void Func_0200324c(void) {
  u32 i;
  s32 zero;

  for (i = 0; i <= 1; i++) {
    s32 id = i + 15;
    struct KibomboObject *object = Actor_GetById(id);

    Func_020031f0(id);
    zero = 0;
    object->motionFlags = zero;
  }
  Actor_GetById(15)->y = PIXELS(32);
  Actor_GetById(16)->y = zero;
  Func_02003220();
}

void Func_02003288(void) {
  u32 i;

  for (i = 0; i <= 1; i++) {
    s32 id = i + 26;
    struct KibomboObject *object = Actor_GetById(id);

    Func_020031f0(id);
    object->motionFlags = 0;
  }
  Func_02003220();
}

void Func_020032b0(void) {
  u32 i;

  for (i = 0; i <= 7; i++)
    Actor_SetModeById(i + 18, 5);
}

void Func_020032c8(void) {
  u32 i;

  for (i = 0; i <= 7; i++)
    Actor_SetModeById(i + 18, 1);
}

void Func_020032e0(void) {
  Func_02005c14(143, 1);
  Func_02005c1c(17, 27);
  Func_02005c34();
  Field_DispatchTypeHandler(1);
  Func_02005c24();
  Func_02005c2c();
}
