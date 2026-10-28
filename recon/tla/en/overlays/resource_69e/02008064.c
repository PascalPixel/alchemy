// Func_02000148: differ 2, the native loads the step 0xfffff800 from the pool
// before it reads the timer (same priority; the scheduler prefers the read,
// which has one more dependent). The step through a "+l" asm operand or a
// one-pass block, a pointer to the timer and every statement order were tried.
// Func_020001a4: differ 20, the native shares one tail between the two
// Random16Far branches (cross-jump from the lsrs on), each branch loading
// center->y before its shift.
#include "types.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"
#include "field_event.h"
#include "field_sprite.h"

extern u8 gData_02002fb8[];
extern u8 gData_02003214[];
extern s32 gData_02003274;
extern s32 gData_02003278;
extern u8 gData_02002f70[];
extern u8 gData_02002f94[];
extern u32 gFrameCount;

void Func_020029d0();
void Func_020029d8();
struct FieldActor *Actor_GetById();
s32 GameFlag_Test();
void GameFlag_SetBit();
void wait();
void Func_02000e04();
u32 __umodsi3();
void Animation_ApplyChildValues();
s32 Math_Cosine();
s32 Math_Sine();
u32 Random16Far();
struct FieldActor *Func_02002828();
void Func_02002820();
void Actor_SetPartAttribute();
void Func_02002a40();
void Func_02001650();

struct SpritePictures {
  u8 unk00[5];
  u8 picture;
};

struct AnimatedSprite {
  u8 unk00[0x19];
  u8 picture;
  u8 nextPicture;
  u8 unk1b[0xd];
  struct SpritePictures *pictures;
};

void Func_02000054(void) { Func_020029d0(gData_02002fb8); }

void Func_020000c8(struct FieldActor *object) {
  struct AnimatedSprite *sprite = (struct AnimatedSprite *)object->sprite;
  struct SpritePictures *pictures = sprite->pictures;

  switch (object->timer & 3) {
  case 0:
    pictures->picture = 7;
    sprite->picture = 1;
    sprite->nextPicture = 2;
    break;
  case 1:
    sprite->picture = 1;
    sprite->nextPicture = 0;
    break;
  case 2:
    pictures->picture = 7;
    sprite->picture = 1;
    sprite->nextPicture = 0;
    break;
  case 3:
    pictures->picture = 0;
    sprite->picture = 1;
    sprite->nextPicture = 0;
    break;
  }
  object->timer++;
}

void Func_02000120(struct FieldActor *object) {
  u32 frameCount = gFrameCount;

  if (frameCount & 1)
    Animation_ApplyChildValues(object, (frameCount >> 1) % 6);
}

void Func_02000148(struct FieldActor *object) {
  struct FieldActor *center;
  u16 angle;

  center = Actor_GetById(23);
  angle = object->timer;
  object->x = center->x + Math_Cosine(angle) * (object->speed + 3);
  {
    s32 sine = Math_Sine(angle);
    s32 z = center->z;
    s32 x = object->x;

    /* FAKEMATCH: the barrier keeps both loads before the doubling (differ 4
     * without it). */
    __asm__("");
    object->z = z + sine * 2;
    /* FAKEMATCH: and the height store before the target store. */
    __asm__("");
    object->targetX = x;
    object->targetZ = object->z;
    object->timer -= 0x800;
  }
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

static inline s32 FlagCheck(s32 flag) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameter keeps each flag's
   * constant */
  return GameFlag_Test(flag);
}

#define Rand(range) ((Random16Far() * (range)) >> 16)

void Func_020001a4(void) {
  struct FieldActor *center;
  struct FieldActor *spark;
  struct FieldSprite *sprite;
  s32 angle;

  if (!FlagCheck(0x201) && gFrameCount % 3 != 0)
    return;
  center = Actor_GetById(23);
  if (FlagCheck(0x201))
    spark = Func_02002828(672, center->x,
                          (s32)((u32)(Random16Far() << 8) >> 16 << 16) +
                              center->y - 0x1c0000,
                          center->z);
  else
    spark = Func_02002828(672, center->x,
                          (s32)((u32)(Random16Far() << 6) >> 16 << 16) +
                              center->y - 0x1c0000,
                          center->z);
  if (spark == NULL)
    return;
  sprite = spark->sprite;
  Func_02002820(spark, gData_02003214);
  Actor_SetPartAttribute(spark, 1);
  spark->motionFlags = 0;
  angle = Random16Far() & 0xffff000;
  spark->timer = angle;
  HALF_AT(spark, 102) = 0;
  WORD_AT(spark, 108) = (s32)Func_02000148;
  spark->speed = (Math_Sine((Random16Far() * 0xffff) >> 20) * 24) >> 16;
  BYTE_AT(sprite, 26) = 0;
  sprite->priority = 1;
}

void Func_02000294(void) {
  if (gData_02003278 != 0)
    Func_02002a40(201);
  Func_02001650(gData_02003274);
}
