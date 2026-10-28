#include "types.h"
#include "field_event.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern const u8 gData_02002dec[];

struct FieldActor *Func_02002804(s32 type, s32 x, s32 y, s32 z);
void Func_020027f4(struct FieldActor *object, s32 mode);
void Func_020027fc(struct FieldActor *object, const void *script);
struct FieldActor *Actor_GetById(s32 id);
void Func_02002924(s32 cue);
void FieldEvent_WaitFrames(s32 frames);

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

struct SpawnedActor {
  u8 unk00[0x5e];
  u16 timer;
};

void Func_0200128c(s32 x, s32 y, s32 z, s16 rotation, s32 timer) {
  struct FieldActor *object;
  struct AnimatedSprite *sprite;

  object = Func_02002804(0x1e8, x, y, z);
  if (object != 0) {
    sprite = (struct AnimatedSprite *)object->sprite;
    Func_020027f4(object, 1);
    Func_020027fc(object, gData_02002dec);
    object->motionFlags = 0;
    {
      s32 timerValue = timer;

      /* FAKEMATCH: the native loads the timer argument before the zero for
       * nextPicture (differ 2 without the temporary and barrier). */
      __asm__("");
      ((struct SpawnedActor *)object)->timer = timerValue;
    }
    sprite->nextPicture = 0;
    sprite->rotation = rotation;
  }
}
