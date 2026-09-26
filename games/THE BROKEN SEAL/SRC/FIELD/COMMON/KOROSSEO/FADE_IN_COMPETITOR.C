#include "FIELD_EVENT.H"

struct CompetitorIoQueue {
    u16 count;
    u16 pad;
    u32 entries[32][3];
};

extern struct CompetitorIoQueue gCompetitorIoQueue;

extern volatile u16 gInterruptMasterEnable;
extern s32 Korosseo_CompetitorStartX;
extern s32 Korosseo_CompetitorStartZ;
extern s32 Korosseo_CompetitorStartAngle;

/* FAKEMATCH: the loop around the IME read preserves its saved-copy order;
 * the count store's cast preserves the queue entry scheduling. */
#define QUEUE_IO_WRITE(address, value, delay)                                \
    do {                                                                     \
        volatile u16 *ime;                                                   \
        struct CompetitorIoQueue *q;                                              \
        u32 saved;                                                           \
        s32 count;                                                           \
                                                                             \
        q = &gCompetitorIoQueue;                                                  \
        do {                                                                \
            ime = &gInterruptMasterEnable;                                            \
            saved = *ime;                                                    \
        } while (0);                                                        \
        *ime = (u16)(u32)ime;                                                \
        count = q->count;                                                    \
        if (count <= 31) {                                                   \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);            \
            *(u16 *)&q->count = count + 1;                                    \
            *destination++ = (value);                                        \
            *destination++ = (address);                                      \
            *destination = (delay);                                          \
        }                                                                    \
        *ime = saved;                                                        \
    } while (0)

/* Save the competitor's starting position and fade in its sprite. */
void Korosseo_FadeInCompetitor(s32 id, s32 x, s32 z)
{
    struct CompetitorState {
        u8 unknown_00[6];
        u8 mode;
        u8 stage;
    } *state;
    struct FieldActor *actor;
    struct CompetitorSprite {
        u8 unknown_00[4];
        u16 y : 8;
        u16 affine : 2;
        u16 blend : 2;
        u16 unused : 4;
        u8 unknown_06[10];
        u16 second_y : 8;
        u16 second_affine : 2;
        u16 second_blend : 2;
        u16 second_unused : 4;
    } *sprite;
    s32 i;

    state = *(struct CompetitorState **)0x03001e68;
    actor = Engine_ActorGet(id);
    {
        /* FAKEMATCH: a word temporary keeps 1 out of a halfword pool. */
        s32 one = 1;

        state->mode = one;
    }
    state->stage = 4;
    Korosseo_CompetitorStartX = actor->x.fixed;
    Korosseo_CompetitorStartZ = actor->z.fixed;
    sprite = (struct CompetitorSprite *)actor->sprite;
    Korosseo_CompetitorStartAngle = actor->facing;
    Engine_ActorSetSpritePriority(id, 2);
    {
        /* FAKEMATCH: the byte view prevents sharing a dead bitfield value. */
        u8 value = *(volatile u8 *)&actor->priority_flags;

        *(u8 *)&actor->priority_flags = (u8)(value | 1);
    }
    {
        /* FAKEMATCH: a word temporary keeps the facing value immediate. */
        s32 facing = 0x4000;

        actor->facing = facing;
    }
    Engine_ActorSetSpriteFlags(actor, 3);
    Engine_ObjectSetAnimation(actor, 0);
    Engine_ObjectSetAnimation(actor, 1);
    Engine_ActorSetPosition(id, x << 16, z << 16);
    Engine_ActorFaceActor(0, 0x4000, 0);
    QUEUE_IO_WRITE(0x4000050, 0xf00, 0x20000);
    sprite->blend = 1;
    sprite->second_blend = 1;
    Engine_AudioPlayCue(252);
    for (i = 0; i <= 15; i += 2) {
        actor->scale_x = (i << 12) + 0x1000;
        actor->scale_y = 0x1f000 - (i << 12);
        QUEUE_IO_WRITE(0x4000052, ((15 - i) << 8) | (i + 1), 0x20000);
        Engine_TaskWait(1);
    }
    QUEUE_IO_WRITE(0x4000052, 16, 0x20000);
    actor->scale_x = 0x11000;
    actor->scale_y = 0xf000;
    Engine_EventWait(1);
    actor->scale_x = 0x10000;
    actor->scale_y = 0x10000;
    Engine_EventWait(13);
    sprite->blend = 0;
    sprite->second_blend = 0;
    Engine_ActorSetAnimationAndWait(id, 3);
    Engine_EventWait(20);
}
