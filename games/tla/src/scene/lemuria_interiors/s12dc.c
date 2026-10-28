#include "types.h"
#include "field_event.h"

void Func_0200128c();
struct FieldActor *Actor_GetById(s32 id);
void Func_02002924(s32 cue);
void FieldEvent_Wait(s32 frames);

struct SpritePictures {
  u8 unk00[5];
  u8 picture;
};

struct AnimatedSprite {
  u8 unk00[0x12];
  s16 rotation;
  u8 unk14[5];
  u8 picture;
  u8 nextPicture;
  u8 unk1b[0xd];
  struct SpritePictures *pictures;
};

void Func_020012dc(s32 id) {
  struct FieldActor *object = Actor_GetById(id);

  Func_0200128c(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000,
                15);
  Func_02002924(151);
  FieldEvent_Wait(15);
  Func_0200128c(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000,
                15);
  Func_02002924(151);
  FieldEvent_Wait(15);
  Func_0200128c(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000,
                15);
  Func_02002924(151);
  FieldEvent_Wait(15);
  Func_0200128c(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000,
                15);
  Func_02002924(151);
  FieldEvent_Wait(15);
}
