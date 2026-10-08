#include "entrance.h"

s32 MakyuriIriguchi_TrailSparks(struct FieldActor *a0)
{
    s32 value;
    s32 magic;
    u8 storage[40];
    u8 *rec = storage;

    FIELD(rec, s32, 4) = 7;
    if ((gFrameCount & 1) == 0) {
        FIELD(rec, s32, 4) = 5;
    }
    FIELD(rec, s32, 8) = 0xcccc;
    FIELD(rec, s32, 12) = 0xcccc;
    FIELD(rec, s32, 0) = 0;
    value = Engine_RandomNext();
    magic = -(s32)(((u32)(value << 3) >> 16) * 0x3333);
    Effect_Spawn((a0->x.fixed + ((8 - (gFrameCount & 15)) << 16)), (a0->y.fixed + 0x1a0000), a0->z.fixed, 0, magic, 0, 0xb0000, rec);
    return 0;
}

s32 SceneAudio_PlayCue118AndReturnZero(void)
{
    Engine_AudioPlayCue(118);
    return 0;
}

s32 SceneActor_FaceLeaderWhileGrounded(struct FieldActor *object)
{
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    if ((leader->z.fixed >> 19) <= 22) {
        object->facing = ArcTan2(
            leader->z.fixed - object->z.fixed,
            leader->x.fixed - object->x.fixed);
    } else if (object->facing != 0xc000) {
        Engine_ActorFaceDirection(MEARI, 0xc000, 0);
    }
    return 0;
}
