#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"

/* Gather actors 16 to 31 at the crater, fling them out in a ring, then
 * reset them once they land. */
void SoruFunka_ThrowEruptionRing(void)
{
    struct FieldActor *actor;
    struct FieldActor *spun;
    u32 i;
    s32 angle;
    struct FieldSprite *sprite;
    s32 dx;
    s32 dz;

    for (i = 16; i <= 31; i++) {
        actor = Engine_ActorGet(i);
        Engine_ActorStop(i);
        Engine_ActorSetSpriteFlags(actor, 0);
        Engine_ObjectSetAnimation(actor, 2);
        actor->sprite->priority = 0;
        actor->motion_flags = 0;
        actor->speed = 0x80000;
        actor->acceleration = 0xc000;
        actor->scale_x = 0x1cccc;
        actor->scale_y = 0x1cccc;
        actor->x.fixed = 0xe80000;
        actor->y.fixed = 0x140000;
        actor->z.fixed = 0x840000;
    }
    Engine_AudioPlayCue(145);
    for (i = 0; i <= 15; i++) {
        spun = Engine_ActorGet(i + 16);
        sprite = spun->sprite;
        angle = i << 12;
        sprite->rotation = angle - 0x4000;
        dx = Iwram_MulQ16(Engine_MathCos(angle), 0x1000000);
        dz = Iwram_MulQ16(Engine_MathSin(angle), 0x1000000);
        Engine_ObjectSetPosition(spun, spun->x.fixed + dx, spun->y.fixed, spun->z.fixed + dz);
    }
    Engine_EventWait(20);
    Engine_ActorWaitForMove(16);
    for (i = 16; i <= 31; i++) {
        actor = Engine_ActorGet(i);
        Engine_ActorStop(i);
        actor->scale_x = 0x10000;
        actor->scale_y = 0x10000;
        actor->x.fixed = 0;
        actor->y.fixed = 0;
        actor->z.fixed = 0;
        actor->velocity_x = 0;
        actor->velocity_y = 0;
        actor->velocity_z = 0;
        actor->target_x = ACTOR_NO_TARGET;
        actor->target_y = ACTOR_NO_TARGET;
        actor->target_z = ACTOR_NO_TARGET;
    }
}
