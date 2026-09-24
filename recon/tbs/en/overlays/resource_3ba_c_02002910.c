#include "TYPES.H"

/* Unit bindings for scoring (declare as absolute_symbols of a unit on
 * resource_3ba:02002910):
 *   Engine_ActorGet = 0x0200bcb8 (thumb)
 *   Engine_ActorSetSpritePriority = 0x0200bd58 (thumb)
 *   Engine_ActorSetSpriteFlags = 0x0200bc10 (thumb)
 *   Engine_ObjectSetAnimation = 0x0200bba8 (thumb)
 *   Engine_ActorSetPosition = 0x0200bcf8 (thumb)
 *   Engine_ActorFaceActor = 0x0200bd20 (thumb)
 *   Engine_AudioPlayCue = 0x0200bdf8 (thumb)
 *   Engine_TaskWait = 0x0200bb08 (thumb)
 *   Engine_EventWait = 0x0200bc98 (thumb)
 *   Engine_ActorSetAnimationAndWait = 0x0200bd08 (thumb)
 *   Korosseo_CompetitorStartX = 0x0200c804 (data)
 *   Korosseo_CompetitorStartZ = 0x0200c75c (data)
 *   Korosseo_CompetitorStartAngle = 0x0200c788 (data)
 */
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;
extern s32 Korosseo_CompetitorStartX;
extern s32 Korosseo_CompetitorStartZ;
extern s32 Korosseo_CompetitorStartAngle;

#define QUEUE_IO_WRITE(address, value, delay)                                \
    do {                                                                     \
        volatile u16 *ime;                                                   \
        struct IoWriteQueue *q;                                              \
        u32 saved;                                                           \
        s32 count;                                                           \
                                                                             \
        q = &gIoWriteQueue;                                                  \
        ime = &Data_04000208;                                                \
        saved = *ime;                                                        \
        *ime = (u16)(u32)ime;                                                \
        count = q->count;                                                    \
        if (count <= 31) {                                                   \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);            \
            q->count = count + 1;                                            \
            *destination++ = (value);                                        \
            *destination++ = (address);                                      \
            *destination = (delay);                                          \
        }                                                                    \
        *ime = saved;                                                        \
    } while (0)


u8 * Engine_ActorGet();
void Engine_ActorSetSpritePriority();
void Engine_ActorSetSpriteFlags();
void Engine_ObjectSetAnimation();
void Engine_ActorSetPosition();
void Engine_ActorFaceActor();
void Engine_AudioPlayCue();
void Engine_TaskWait();
void Engine_EventWait();
void Engine_ActorSetAnimationAndWait();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* NONMATCHING: 476 of 476 bytes, 47 halfword edits (2026-09-24). Saves the
 * competitor start the restore reads (FIELD/COMMON/KOROSSEO/RESTORE_COMPETITOR.C)
 * and fades the actor in through QUEUE_IO_WRITE. Twins: resource_3bb:02002ba8
 * (472) and resource_3bc:02003640. Residual: the sprite blend masks are built
 * as 0xf3 instead of -13 and the IO-queue count store is scheduled early. */
void Func_02002910(s32 id, s32 x, s32 z)
{
    u8 *state;
    u8 *actor;
    u8 *sprite;
    s32 i;

    state = *(u8 **)0x03001e68;
    actor = (u8 *)Engine_ActorGet(id);
    {
        s32 one = 1;

        state[6] = one;
    }
    state[7] = 4;
    Korosseo_CompetitorStartX = *(s32 *)(actor + 8);
    Korosseo_CompetitorStartZ = *(s32 *)(actor + 16);
    sprite = *(u8 **)(actor + 80);
    Korosseo_CompetitorStartAngle = *(u16 *)(actor + 6);
    Engine_ActorSetSpritePriority(id, 2);
    {
        u8 value = *(volatile u8 *)&actor[35];

        actor[35] = (u8)(value | 1);
    }
    {
        s32 facing = 0x4000;

        *(u16 *)(actor + 6) = facing;
    }
    Engine_ActorSetSpriteFlags(actor, 3);
    Engine_ObjectSetAnimation(actor, 0);
    Engine_ObjectSetAnimation(actor, 1);
    Engine_ActorSetPosition(id, x << 16, z << 16);
    Engine_ActorFaceActor(0, 0x4000, 0);
    QUEUE_IO_WRITE(0x4000050, 0xf00, 0x20000);
    sprite[5] = (sprite[5] & -13) | 4;
    sprite[17] = (sprite[17] & -13) | 4;
    Engine_AudioPlayCue(252);
    for (i = 0; i <= 15; i += 2) {
        *(s32 *)(actor + 24) = (i << 12) + 0x1000;
        *(s32 *)(actor + 28) = 0x1f000 - (i << 12);
        QUEUE_IO_WRITE(0x4000052, ((15 - i) << 8) | (i + 1), 0x20000);
        Engine_TaskWait(1);
    }
    QUEUE_IO_WRITE(0x4000052, 16, 0x20000);
    *(s32 *)(actor + 24) = 0x11000;
    *(s32 *)(actor + 28) = 0xf000;
    Engine_EventWait(1);
    *(s32 *)(actor + 24) = 0x10000;
    *(s32 *)(actor + 28) = 0x10000;
    Engine_EventWait(13);
    sprite[5] &= -13;
    sprite[17] &= -13;
    Engine_ActorSetAnimationAndWait(id, 3);
    Engine_EventWait(20);
}
