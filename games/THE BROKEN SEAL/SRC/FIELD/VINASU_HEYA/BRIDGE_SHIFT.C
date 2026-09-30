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

extern struct MapWork *gMapWork;

/* The first bridge slides beneath the leader while its map columns are
 * copied into place and dust trails along the moving edge. */
void VinasuHeya_ShiftBridge(void)
{
    struct EffectOptions options;
    struct ScrollLayer *layer;
    struct FieldActor *leader;
    s32 x;
    s32 z;
    s32 countdown;
    u32 i;
    s32 dust_x;

    layer = &gMapWork->layer;
    leader = Object_GetById(0);
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
            x = 29;
            countdown = 40;
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
                        Map_CopyCellsTo(x, 50, 15, 32, 3, 4);
                    } else {
                        x += 4;
                        Map_CopyCellsTo(x, 45, 9, 32, 3, 4);
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
            Engine_MapRenderWaitForValues();
            gEventWork->start_transition = 0x202;
            Engine_EventRequestExit(11);
            Engine_EventEnd();
        }
    }
}
