/* resource_39d:0200b4bc..0200b628 (364 bytes with pool), still linked from
 * the listing. Remaining difference: the spark's random angle is masked with
 * 0x0ffff000 loaded from the literal pool as a link-time value; an integer
 * mask stored to the halfword angle narrows to a halfword load and changes the
 * register allocation that follows (362 bytes, 104 differ). */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "OVERLAY_OBJECT.H"

extern u8 *gCam;

struct LampWork {
    s16 unused;
    s16 height;
};

struct Half {
    u16 value;
};

/* The creator uses actor fields; its arc callback uses the object prefix. */
union ArcObject {
    struct FieldActor actor;
    struct OverlayObject arc;
};

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void MakyuriChojo_SpawnLampSpark(void)
{
    struct FieldActor *lamp;
    struct FieldActor *actor;
    union ArcObject *spark;
    struct FieldSprite *sprite;
    struct LampWork *work;
    s32 height;
    s32 scale;
    s32 tick;
    /* FAKEMATCH: a literal halfword zero shares the angle store's zero
     * producer and keeps the short-reach pool at the original boundary. */
    struct Half zero;

    lamp = Object_GetById(8);
    work = (struct LampWork *)(gCam + 0xe8);
    height = ((u32)(Engine_RandomNext() * 48) >> 16) << 16;
    if (work->height <= 129) {
        if (gFrameCount & 1) {
            Engine_ActorSetPosition(8, 0x1300000, 0x900000);
            actor = Object_GetById(8);
            /* FAKEMATCH: keep the scale load after the actor lookup. */
            do { scale = 0x10000; } while (0);
        } else {
            Engine_ActorSetPosition(8, 0x1300000, 0x970000);
            actor = Object_GetById(8);
            scale = 0x14ccc;
        }
        actor->scale_x = scale;
        Object_GetById(8)->scale_y = scale;
    } else {
        Call3(Engine_ActorSetPosition, 8, 0x80000, 0x80000);
    }
    if (lamp == NULL)
        return;
    tick = gFrameCount & 15;
    if (tick != 0)
        return;
    spark = (union ArcObject *)Object_Create(0x11c, lamp->x.fixed + 0x80000, lamp->y.fixed + height + 0x80000, lamp->z.fixed);
    height = Engine_MathDivide(height, 0x60000);
    height <<= 16;
    if (spark == NULL)
        return;
    sprite = spark->actor.sprite;
    Engine_ObjectSetScript(&spark->actor, (const s32 *)0x0200bc54);
    SetOverlayObjectSlot(&spark->actor, 3);
    spark->actor.motion_flags = tick;
    spark->arc.angle_64 = 0x0ffff000 & Engine_RandomNext();
    zero.value = 0;
    spark->actor.unknown_66 = tick;
    spark->arc.linked_object = (struct OverlayObject *)lamp;
    spark->actor.update = (void (*)(union FieldObject *))0x0200b461;
    spark->arc.field_30 = (Engine_MathSin((height & 0xfffff) >> 4) * 24) >> 16;
    *((u8 *)sprite + 38) = zero.value;
    sprite->priority = lamp->sprite->priority;
}
