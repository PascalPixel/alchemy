#include "TYPES.H"
#include "FIELD_EVENT.H"

struct Pulse {
    u8 unknown_00[0x64];
    s16 timer;
    s16 beat;
};

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Keep actor 27's glow on actor 19: step the screen work values as 19's timer runs down, and on odd frames pulse the glow's scale by 19's beat. */
void HaidiaArashi_UpdatePulsingGlow(void)
{
    struct FieldActor *source;
    struct FieldActor *glow;
    struct FieldSprite *sprite;
    s16 *timer;

    source = Engine_ActorGet(19);
    glow = Engine_ActorGet(27);
    sprite = glow->sprite;
    timer = &((struct Pulse *)source)->timer;
    if (*timer != 0) {
        if (*timer == 60) {
            Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
        }
        if (*timer == 40) {
            Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
        }
        if (*timer == 30) {
            Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
        }
        if (*timer == 20) {
            Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
        }
        (*timer)--;
    }
    glow->x.fixed = source->x.fixed;
    glow->z.fixed = source->z.fixed;
    glow->target_x = glow->x.fixed;
    ((u8 *)sprite)[35] = 10;
    if (gFrameCount & 1) {
        switch (((struct Pulse *)source)->beat) {
        case 1:
        case 5:
            glow->scale_x += 0xa3d;
            glow->scale_y += 0xa3d;
            break;
        case 4:
            glow->scale_x += 0x51e;
            glow->scale_y += 0x51e;
            break;
        case 2:
        case 3:
        case 6:
        case 7:
        case 8:
        case 9:
            glow->scale_x += -0x7ae;
            glow->scale_y += -0x7ae;
            break;
        }
        sprite->scale = glow->scale_x;
    } else {
        sprite->scale = 0;
    }
}
