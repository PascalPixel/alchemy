#include "FIELD_EVENT.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "TYPES.H"
#include "CALL.H"

extern u8 gMenuCtrlWork[];

/* The competitor's starting position and facing, kept in the overlay's
 * variables while a round runs. */
extern s32 Korosseo_CompetitorStartX;
extern s32 Korosseo_CompetitorStartZ;
extern s32 Korosseo_CompetitorStartAngle;

/* FAKEMATCH: the loop around the IME read preserves its saved-copy order;
 * the count store's cast preserves the queue entry scheduling. */
#define QUEUE_IO_WRITE(address, value, delay)                                \
    do { \
        /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling; see its retained draft. */ \
        volatile u16 *ime;                                                   \
        struct IoWriteQueue *q;                                              \
        u32 saved;                                                           \
        s32 count;                                                           \
                                                                             \
        q = &gIoWriteQueue;                                                  \
        do { \
            /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling; see its retained draft. */ \
            ime = &REG_IME;                                                  \
            saved = *ime;                                                    \
        } while (0);                                                         \
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

void Engine_ActorSetAnimation();
void Engine_ActorFaceDirection();
void Engine_EventWait();
void Engine_ActorSetSpriteFlags();
void Object_SetMode();
void Engine_TaskWait();

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

    state = *(struct CompetitorState **)gMenuCtrlWork;
    actor = Object_GetById(id);
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
    Object_SetMode(actor, 0);
    Object_SetMode(actor, 1);
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

/* The game state, read here as bytes: the byte at 498 is the retry flag. */

/* Colosso: put the competitor back at its stored start position and facing
 * after a round, replaying the fall animation unless the retry flag is set.
 * The same function sits in each of the three Colosso trial overlays. */
void Korosseo_RestoreCompetitor(s32 a0)
{
    u8 *rec7;
    u8 *p7;
    s32 zero;
    /* FAKEMATCH: a one-halfword aggregate holds the zero stored at +34, so
     * it loads as a halfword pool constant whose short range places the
     * literal pool before the epilogue. */
    struct Half {
        u16 v;
    } fall;

    p7 = *(u8 **)gMenuCtrlWork;
    rec7 = (u8 *)Object_GetById(a0);
    if (gGameState.movement_mode == 1) {
        gGameState.movement_mode = 0;
        Engine_ActorSetAnimation(a0, 1);
    } else {
        Call3(Engine_ActorFaceDirection, a0, 0x4000, 30);
        Engine_ActorSetAnimation(a0, 3);
        Engine_EventWait(30);
    }
    zero = 0;
    p7[7] = zero;
    p7[6] = 15;
    *(s32 *)((s32)rec7 + 8) = Korosseo_CompetitorStartX;
    *(s32 *)((s32)rec7 + 16) = Korosseo_CompetitorStartZ;
    *(u16 *)((s32)rec7 + 6) = Korosseo_CompetitorStartAngle;
    *(s32 *)((s32)rec7 + 56) = -0x80000000;
    *(s32 *)((s32)rec7 + 64) = -0x80000000;
    *(s32 *)((s32)rec7 + 36) = zero;
    *(s32 *)((s32)rec7 + 44) = zero;
    fall.v = 0;
    rec7[85] = 3;
    rec7[34] = fall.v;
    *(s32 *)((s32)rec7 + 12) = zero;
    *(s32 *)((s32)rec7 + 20) = zero;
    Engine_ActorSetSpriteFlags((s32)rec7, 1);
    Object_SetMode((s32)rec7, 0);
    Object_SetMode((s32)rec7, 1);
    Engine_TaskWait(1);
}
