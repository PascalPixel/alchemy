#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;
#define REG_IME Data_04000208

void Main_0808a5e0(s32 cue);

/* QueueIoWriteDelay2 (SYSTEM/IO_WRITE_QUEUE.C) written out in place, with
 * that function's one-pass loops around the IME read and the queue update. */
#define QUEUE_IO_WRITE(address, value)                                      \
    q = &gIoWriteQueue;                                                     \
    do {                                                                    \
        do {                                                                \
            ime = &REG_IME;                                                 \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        limit = 31;                                                         \
        count = q->count;                                                   \
        if (count <= limit) {                                               \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
                                                                            \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = 0x20000;                                         \
        }                                                                   \
        *ime = saved;                                                       \
    } while (0)

/* Raises flags 0x206-0x20a for each of flags 0x301-0x305 already set,
 * resets the scale of actors 8 to 12 unless flag 0x109 is set, then queues
 * the blend setup: full fade-in weights when flag 0x340 is set.
 *
 * FAKEMATCH: the queue bound is a variable, so the comparison stays signed
 * on the u16 count as the ROM's bgt has it, and the queue writes keep the
 * one-pass loops of QueueIoWriteDelay2. */
void SuharaSabaku_Func02000f50(void)
{
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u32 saved;
    u16 count;
    s32 limit;
    struct FieldActor *actor;
    s32 id;
    s32 alpha = 0;

    if (Engine_GameFlagIsSet(0x301)) {
        Engine_GameFlagSet(0x206);
    }
    if (Engine_GameFlagIsSet(0x302)) {
        Engine_GameFlagSet(0x207);
    }
    if (Engine_GameFlagIsSet(0x303)) {
        Engine_GameFlagSet(0x208);
    }
    if (Engine_GameFlagIsSet(0x304)) {
        Engine_GameFlagSet(0x209);
    }
    if (Engine_GameFlagIsSet(0x305)) {
        Engine_GameFlagSet(0x20a);
    }
    for (id = 8; id <= 12; id++) {
        actor = Engine_ActorGet(id);
        if (actor != 0) {
            if (!Engine_GameFlagIsSet(0x109)) {
                actor->scale_x = 0x800;
                actor->scale_y = 0x800;
            }
            actor->sprite->flags = 0;
        }
    }
    QUEUE_IO_WRITE(0x04000050, 0x3f42);
    if (Engine_GameFlagIsSet(0x340)) {
        alpha = 16;
        Main_0808a5e0(244);
    }
    QUEUE_IO_WRITE(0x04000052, ((16 - alpha) << 8) | alpha);
}
