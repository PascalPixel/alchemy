#include "ENTRANCE.H"


s32 MakyuriIriguchi_TrailSparks(s32 a0)
{
    s32 value;
    s32 magic;
    u8 storage[40];
    u8 *rec = storage;

    FIELD(rec, s32, 4) = 7;
    if ((*(volatile s32 *)&gFrameCount & 1) == 0) {
        FIELD(rec, s32, 4) = 5;
    }
    FIELD(rec, s32, 8) = 0xcccc;
    FIELD(rec, s32, 12) = 0xcccc;
    FIELD(rec, s32, 0) = 0;
    value = Random_Next();
    magic = -(s32)(((u32)(value << 3) >> 16) * 0x3333);
    Effect_Spawn((*(s32 *)(a0 + 8) + ((8 - (*(volatile s32 *)&gFrameCount & 15)) << 16)), (*(s32 *)(a0 + 12) + 0x1a0000), *(s32 *)(a0 + 16), 0, magic, 0, 0xb0000, rec);
    return 0;
}

/* Play the footprint-motion completion cue. */
s32 SceneAudio_PlayCue118AndReturnZero(void)
{
    Audio_PlayCue(118);
    return 0;
}

/* Keep this object facing actor 0 while the actor remains near ground level. */
s32 SceneActor_FaceLeaderWhileGrounded(u8 *object)
{
    u8 *leader = Actor_Get(ACTOR_PARTY_LEADER);

    if ((*(s32 *)(leader + 16) >> 19) <= 22) {
        *(u16 *)(object + 6) = ArcTan2(
            *(s32 *)(leader + 16) - *(s32 *)(object + 16),
            *(s32 *)(leader + 8) - *(s32 *)(object + 8));
    } else if (*(u16 *)(object + 6) != 0xc000) {
        Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    }
    return 0;
}
