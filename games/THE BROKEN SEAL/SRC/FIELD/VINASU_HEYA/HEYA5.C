#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"
#include "CALL.H"

struct ScrollLayer {
    u8 unknown_00[8];
    s32 x;
};

struct MapWork {
    u8 unknown_00[0x164];
    struct ScrollLayer layer;
};


extern u8 gVinasuPushScript[];
extern u8 gVinasuPushCells[];
void Engine_EventBegin();
void Engine_MapCopyCellAttributes();
s32 Engine_GameFlagIsSet();
s32 OverlayObject_PrepareObjectWithCommand15();
void Engine_ActorSetSpritePriority();
void Engine_ActorSetSpriteFlags();
void OverlayObject_WaitUntilIdle();
void Engine_AudioPlayCue();
void Engine_ObjectDispatchRelease();
void Engine_ActorEnableActionCallback();
void Engine_ActorStartAction();
void Engine_EventWait();
void Engine_MapAnimateCells();
void Engine_EventEnd();

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

    layer = &((struct MapWork *)gMapWork[0])->layer;
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

/* Venus Lighthouse room: when actor 9 stands on cell (46, 45) and flag 0x301
 * is clear, push it through, shift the map cells, release actor 10 and set
 * the flag. */
void VinasuHeya_RunCellPushScene(void)
{
    u32 i;
    u8 *p10;
    s32 p8;
    s32 p9;
    s32 rec;
    s32 rec5;
    u8 *rec8;
    u8 *record;
    s32 v5;
    s32 slot8;

    slot8 = 0;
    rec8 = (s32)Object_GetById(9);
    rec5 = (s32)Object_GetById(0);
    v5 = 45;
    Engine_EventBegin();
    Engine_MapCopyCellAttributes(109, 43, 7, 5, v5, 43);
    p10 = (s32)rec8 + 35;
    if ((2 & rec8[35]) != 0) {
        Engine_MapCopyCellAttributes(45, 45, 1, 1, 46, v5);
    } else {
        Call6(Engine_MapCopyCellAttributes, 48, 42, 1, 1, (*(s32 *)((s32)rec8 + 8) >> 20), (*(s32 *)((s32)rec8 + 16) >> 20));
    }
    p9 = (*(s32 *)((s32)rec8 + 8) >> 20);
    if (p9 != 46) {
    } else {
        p8 = (*(s32 *)((s32)rec8 + 16) >> 20);
        if (p8 != 45) {
        } else {
            rec = Engine_GameFlagIsSet(0x301);
            if (rec != 0) {
            } else {
                if ((*(s32 *)(rec5 + 16) >> 20) <= 45) {
                    record = OverlayObject_PrepareObjectWithCommand15(0x2e80000, 0, 0x2c00000, 20);
                    slot8 = (s32)record;
                    Engine_ActorSetSpritePriority(0, 3);
                }
                record = (s32)Object_GetById(9);
                Engine_ActorSetSpriteFlags((s32)record, 0);
                rec8[34] = rec;
                rec8[85] = 3;
                *(s32 *)((s32)rec8 + 72) = 0x1999;
                *(s32 *)((s32)rec8 + 68) = rec;
                Engine_MapCopyCellAttributes(43, 45, 1, 1, p9, p8);
                OverlayObject_WaitUntilIdle((s32)rec8);
                Engine_AudioPlayCue(188);
                rec8[85] = rec;
                *(s32 *)((s32)rec8 + 12) = -0x100000;
                Engine_ActorSetSpritePriority(9, 3);
                { s32 two = 2; *p10 = two; } /* FAKEMATCH: a block-local word temporary keeps the 2 from being shared with the later |= 2 */
                Engine_MapCopyCellAttributes(45, 45, 1, 1, p9, p8);
                {
                    u8 *record = (s32)Object_GetById(0);
                    u8 value = *(volatile u8 *)&record[35];
                
                    record[35] = (u8)(value | 1);
                }
                Engine_ObjectDispatchRelease(slot8);
                rec8[89] = rec;
                *p10 |= 2;
                *(u8 *)((s32)Object_GetById(10) + 89) = rec;
                {
                    u8 *record = (s32)Object_GetById(10);
                    u8 value = *(volatile u8 *)&record[35];
                
                    record[35] = (u8)(value | 2);
                }
                Engine_ActorSetPosition(10, 0x3280000, 0x2d80000);
                Engine_ActorEnableActionCallback(10, (s32)gVinasuPushScript);
                Engine_ActorStartAction(10);
                Engine_EventWait(30);
                Engine_AudioPlayCue(158);
                Engine_MapAnimateCells((s32)gVinasuPushCells, 110, 41);
                Call6(Engine_MapCopyCellAttributes, 46, 41, 1, 1, p9, 42);
                Engine_GameFlagSet(0x301);
            }
        }
    }
    Engine_EventEnd();
}
