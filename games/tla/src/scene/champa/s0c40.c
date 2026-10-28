#include "types.h"
#include "field_event.h"

extern const u8 gData_02003620[];

struct FieldActor *Func_020029ec(s32 type, s32 x, s32 y, s32 z);
void Func_020029dc(struct FieldActor *object, s32 mode);
void Func_020029e4(struct FieldActor *object, const void *script);
struct FieldActor *Actor_GetById(s32 id);
void Func_02002bc4(s32 cue);
void FieldEvent_WaitFrames(s32 frames);

void Func_02000c40(s32 x, s32 y, s32 z, s32 angle, s32 frames) {
  s16 value = angle;
  struct FieldActor *object;
  struct FieldSprite *sprite;

  object = Func_020029ec(0x1e8, x, y, z);
  if (object != 0) {
    sprite = object->sprite;
    Func_020029dc(object, 1);
    Func_020029e4(object, gData_02003620);
    object->motionFlags = 0;
    {
      /* FAKEMATCH: a 64-bit copy of frames reads it from the stack after the
       * pool zero is loaded (one reordered pair; as u32, u16 or s16 it stays
       * before). */
      u64 count = frames;

      ((struct {
        u8 unk00[94];
        u16 frames;
      } *)object)
          ->frames = count;
    }

    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}
