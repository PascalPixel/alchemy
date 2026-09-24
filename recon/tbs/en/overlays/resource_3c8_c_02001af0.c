/* NONMATCHING: 596 of 600 bytes, 196 halfword edits (2026-09-24). Same script
 * as the matched bridge events F_0290C.C and F_026F8.C. Remaining: the
 * reference hoists the map-copy height 4 into fp and reloads the 0xcccc scale
 * base from the pool at each use; ours hoists 0xcccc into fp, which shifts
 * the loop registers (layer sl, i r7, x r5). Needs gFrameCount and gEventWork
 * bound as data. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"

struct ScrollLayer {
    u8 unknown_00[8];
    s32 x;
};

struct MapWork {
    u8 unknown_00[0x164];
    struct ScrollLayer layer;
};

extern struct MapWork *Data_03001e70;

void Local_02001af0(void)
{
    struct EffectOptions options;
    struct ScrollLayer *layer;
    struct FieldActor *leader;
    s32 x;
    s32 z;
    s32 countdown;
    u32 i;
    s32 dust_x;
    s32 height;

    layer = &Data_03001e70->layer;
    leader = Engine_ActorGet(0);
    x = leader->x.part.pixel;
    z = leader->z.part.pixel;
    leader->y.fixed = 0;
    if (x >= 308 && x <= 315 && z >= 532 && z < 540) {
        leader->y.fixed = -0x20000;
        if (!GameFlag_IsSet(0x300)) {
            Engine_EventBegin();
            Engine_AudioPlayCue(161);
            GameFlag_Set(0x300);
            Engine_MapCopyCellsTo(26, 33, 19, 33, 1, 1);
            Engine_EventWait(30);
            Engine_AudioPlayCue(239);
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
            Engine_EventWait(20);
            dust_x = 0x1200000;
            countdown = 40;
            height = 4;
            x = 29;
            for (i = 0; i <= 479; i++, countdown--) {
                layer->x += 0x3333;
                dust_x += -0x3333;
                options.priority = 2;
                options.start_scale_x = ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
                options.start_scale_y = ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
                options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
                Effect_Spawn(dust_x, 0, 0x2100000, 0, -(((gFrameCount & 1) * 3) << 16), 0, 0x8a0000, &options);
                if (i == 240) {
                    dust_x += -0x300000;
                }
                if (countdown == 0) {
                    countdown = 40;
                    if (i <= 240) {
                        x -= 4;
                        Engine_MapCopyCellsTo(x, 50, 15, 32, 3, height);
                    } else {
                        x += 4;
                        Engine_MapCopyCellsTo(x, 45, 9, 32, 3, height);
                    }
                }
                Engine_TaskWait(1);
            }
            layer->x += 0x8000;
            layer->x = layer->x / 0x10000 << 16;
            Engine_MapCopyCellAttributes(15, 32, 3, 1, 9, 32);
            Engine_MapCopyCellAttributes(12, 32, 3, 1, 15, 32);
            Engine_AudioPlayCue(288);
            Engine_AudioPlayCue(188);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Engine_MapWaitWorkValuesBelow256();
            gEventWork->start_transition = 0x202;
            Engine_EventRequestExit(11);
            Engine_EventEnd();
        }
    }
}
