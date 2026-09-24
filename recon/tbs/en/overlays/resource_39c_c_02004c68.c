/* NONMATCHING: 728 of 732 bytes, 50 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: the lift scene is written in full; the store of 0x4000000 after the lowering loop reuses the loop's compare constant (the reference rematerialises it), which shifts the rest by 4 bytes, and the second loop's speed update and a few argument orders differ (50 edits). */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_080091a0(void);

struct MapLayer {
    u8 unknown_00[12];
    s32 y;
    u8 unknown_10[12];
    s32 unknown_1c;
    u8 unknown_20[16];
};

struct MapWork {
    u8 unknown_00[20];
    struct MapLayer layers[8];
};

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Local_02004c68(void)
{
    struct MapLayer *layer;
    s32 speed;

    layer = &(*(struct MapWork **)0x03001e70)->layers[7];
    speed = 0x9c28;
    layer->y = 0x4890000;
    layer->unknown_1c = 0;
    Engine_ActorGet(0)->motion_flags = 0;
    Engine_ActorGet(0)->z.fixed += -0x890000;
    Engine_ActorGet(0)->target_z = Engine_ActorGet(0)->z.fixed;
    Engine_ActorGet(13)->motion_flags = 0;
    Engine_ActorSetPosition(13, 0x2a80000, 0x1b80000);
    Engine_ActorGet(13)->z.fixed += -0x890000;
    Engine_ActorGet(13)->target_z = Engine_ActorGet(13)->z.fixed;
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(60);
    Main_080091a0();
    Engine_AudioPlayCue(223);
lower:
    {
        layer->y -= speed;
        Engine_ActorGet(0)->z.fixed += speed;
        Engine_ActorGet(0)->target_z = Engine_ActorGet(0)->z.fixed;
        Engine_ActorGet(13)->z.fixed += speed;
        Engine_ActorGet(13)->target_z = Engine_ActorGet(13)->z.fixed;
        if (layer->y > 0x4000000) {
            if ((*(u32 *)0x03001e40 & 15) == 0 && speed > 0xccb) {
                speed += -0x560;
            }
            Engine_TaskWait(1);
            goto lower;
        }
    }
    layer->y = 0x4000000;
    Engine_MapRedraw();
    Engine_TaskWait(2);
    Engine_ActorGet(0)->motion_flags = 3;
    Engine_ActorGet(13)->z.fixed = 0x1b80000;
    Engine_ActorGet(13)->target_z = Engine_ActorGet(13)->z.fixed;
    Engine_EventWait(30);
    Call3(Engine_ActorWalkToAndWait, 0, 0x2c0, 0x248);
    *(s32 *)Engine_ActorGet(0)->unknown_44 = 0;
    Engine_ActorSetAnimation(0, 6);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(0, 7);
    Engine_ActorGet(0)->speed = 0x30000;
    Engine_ActorGet(0)->acceleration = 0x20000;
    Engine_AudioPlayCue(152);
    Engine_ActorGet(0)->velocity_y = 0x40000;
    Engine_ActorSetSpriteFlags(Engine_ActorGet(0), 0);
    Call3(Engine_ActorSetDestination, 0, 0x2e0, 0x248);
    Engine_ActorWaitForMove(0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(0), 1);
    *(s32 *)Engine_ActorGet(0)->unknown_44 = 0x4000;
    Engine_ActorSetAnimation(0, 6);
    Engine_EventWait(6);
    Engine_ActorFaceDirection(0, 0x8000, 30);
    Engine_AudioPlayCue(223);
    goto raise;
raise_wait:
    if ((*(u32 *)0x03001e40 & 7) == 0 && speed <= 0xcccc) {
        speed += 0x1999;
    }
    Engine_TaskWait(1);
raise:
    {
        layer->y += speed;
        Engine_ActorGet(13)->z.fixed -= speed;
        Engine_ActorGet(13)->target_z = Engine_ActorGet(13)->z.fixed;
        if (layer->y <= 0x488ffff) {
            goto raise_wait;
        }
    }
    layer->y = 0x4000000;
    Call6(Engine_MapCopyCellsTo, 45, 91, 40, 91, 5, 4);
    Call6(Engine_MapCopyCellAttributes, 104, 34, 5, 4, 40, 34);
    Engine_MapRedraw();
    Engine_TaskWait(2);
    Engine_ActorSetPosition(13, 0, 0);
    Engine_EventWait(30);
    Engine_ActorGet(0)->priority_flags |= 1;
}
