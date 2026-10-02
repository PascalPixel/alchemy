/* NONMATCHING: the lamp spark at Mercury Lighthouse's aerie, 364 bytes; 52 of 153 lines
 * differ. A plain rewrite: the spark's angle and turn are 16-bit bit-fields of a word, so
 * the random angle keeps its whole 0x0ffff000 mask, and the two stores of the frame
 * counter's low bits are stores of the variable, which is what leaves the zero the
 * reference loads from its pool.
 * Remaining: reload takes r2 where the reference takes r1 for the lamp's copies (its
 * first choice of spill register, from then on); the masked angle lands in r0 with the
 * address in r3 where the reference has r3 and r2; the turn's address is the angle's
 * plus 2 where the reference computes it from the object again. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"

extern u8 *gMapWork;
extern const s32 Data_02003c54[];

struct LampWork {
    s16 unused;
    s16 height;
};

/* The spark's object record, as its creator fills it. */
struct LampSpark {
    u8 unknown_00[0x30];
    s32 swing;
    u8 unknown_34[0x1c];
    struct FieldSprite *sprite;
    u8 unknown_54;
    u8 motion_flags;
    u8 unknown_56[0x0e];
    u32 angle : 16;
    u32 turn : 16;
    struct FieldActor *source;
    void (*update)();
};

void SceneEffect_UpdateArcPosition();
void SetOverlayObjectSlot(struct FieldActor *object, s32 slot);


void MakyuriChojo_FlickerActorEight(void)
{
    struct FieldActor *lamp;
    struct LampSpark *spark;
    struct FieldSprite *sprite;
    struct LampWork *work;
    s32 height;
    s32 scale;
    s32 tick;

    lamp = Actor_Get(8);
    work = (struct LampWork *)(gMapWork + 0xe8);
    height = ((u32)(Random_Next() * 48) >> 16) << 16;
    if (work->height <= 129) {
        if (gFrameCount & 1) {
            Engine_ActorSetPosition(8, 0x1300000, 0x900000);
            scale = 0x10000;
            Actor_Get(8)->scale_x = scale;
        } else {
            Engine_ActorSetPosition(8, 0x1300000, 0x970000);
            scale = 0x14ccc;
            Actor_Get(8)->scale_x = scale;
        }
        Actor_Get(8)->scale_y = scale;
    } else {
        Actor_SetPosition(8, 0x80000, 0x80000);
    }
    if (lamp == NULL) {
        return;
    }
    tick = gFrameCount & 15;
    if (tick != 0) {
        return;
    }
    spark = (struct LampSpark *)Object_Create(0x11c, lamp->x.fixed + 0x80000,
                                              lamp->y.fixed + height + 0x80000, lamp->z.fixed);
    height = height / 0x60000;
    height <<= 16;
    if (spark == NULL) {
        return;
    }
    sprite = spark->sprite;
    Object_SetScript((struct FieldActor *)spark, Data_02003c54);
    SetOverlayObjectSlot((struct FieldActor *)spark, 3);
    spark->motion_flags = tick;
    spark->angle = Random_Next() & 0x0ffff000;
    spark->turn = tick;
    spark->source = lamp;
    spark->update = SceneEffect_UpdateArcPosition;
    spark->swing = (Math_Sin((height & 0xfffff) >> 4) * 24) >> 16;
    sprite->flags = 0;
    sprite->priority = lamp->sprite->priority;
}
