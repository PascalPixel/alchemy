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

/* The far end of the bridge: the first time the leader steps on it, the
 * bridge rolls back the other way in a spray of dust. */
void VinasuHeya_RetractBridge(void)
{
    struct EffectOptions options;
    struct ScrollLayer *layer;
    struct FieldActor *leader;
    /* The leader's tile column, then the bridge column being drawn. */
    s32 x;
    s32 z;
    s32 countdown;
    u32 i;
    s32 dust_x;

    layer = &Data_03001e70->layer;
    leader = Engine_ActorGet(0);
    x = leader->x.part.pixel;
    z = leader->z.part.pixel;
    leader->y.fixed = 0;
    if (x >= 532 && x <= 539 && z >= 324 && z < 332) {
        leader->y.fixed = -0x20000;
        if (!Engine_GameFlagIsSet(0x306)) {
            Engine_EventBegin();
            Engine_MapCopyCellsTo(63, 29, 33, 20, 1, 1);
            Engine_AudioPlayCue(161);
            Engine_MapCopyCellsTo(44, 83, 44, 80, 3, 3);
            Engine_EventWait(30);
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
            Engine_AudioPlayCue(239);
            Engine_EventWait(20);
            dust_x = 0x2680000;
            x = 0;
            countdown = 60;
            for (i = 0; i <= 319; i++, countdown--) {
                layer->x += 0x3333;
                if (dust_x >= 0x2400000 && i > 40) {
                    dust_x += -0x3333;
                    options.priority = 2;
                    options.start_scale_x = ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
                    options.start_scale_y = ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
                    options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
                    Effect_Spawn(dust_x, 0, 0x1200000, 0, -(((gFrameCount & 1) * 3) << 16), 0, 0x8a0000, &options);
                    if (countdown == 0) {
                        countdown = 40;
                        x += 4;
                        Map_CopyCellsTo(x, 56, 36, 17, 3, 4);
                    }
                }
                Engine_TaskWait(1);
            }
            layer->x += 0x8000;
            layer->x = layer->x / 0x10000 << 16;
            Engine_AudioPlayCue(288);
            Engine_AudioPlayCue(188);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Engine_MapWaitWorkValuesBelow256();
            gEventWork->start_transition = 0x202;
            Engine_EventRequestExit(18);
            Engine_EventEnd();
        }
    }
}
