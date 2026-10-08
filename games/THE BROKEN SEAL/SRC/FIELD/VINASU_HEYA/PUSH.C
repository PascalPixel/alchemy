#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"
#include "CALL.H"

struct MapWork {
    u8 unknown_00[0x164];
    struct ScrollLayer layer;
};

void Engine_MapCopyCellAttributes();
s32 OverlayObject_PrepareObjectWithCommand15();
void OverlayObject_WaitUntilIdle();
void Object_RefreshSelectorById();

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
            gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
            Engine_EventRequestExit(11);
            Engine_EventEnd();
        }
    }
}

void VinasuHeya_RunCellPushScene(void)
{
    u8 *flags;
    s32 z;
    s32 x;
    s32 set;
    struct FieldActor *leader;
    struct FieldActor *block;
    struct FieldActor *actor;
    s32 row;
    s32 marker;

    marker = 0;
    block = Object_GetById(9);
    leader = Object_GetById(0);
    row = 45;
    Engine_EventBegin();
    Engine_MapCopyCellAttributes(109, 43, 7, 5, row, 43);
    flags = &block->priority_flags;
    if ((2 & block->priority_flags) != 0) {
        Engine_MapCopyCellAttributes(45, 45, 1, 1, 46, row);
    } else {
        Map_CopyCellAttributes(48, 42, 1, 1, block->x.fixed >> 20, block->z.fixed >> 20);
    }
    x = block->x.fixed >> 20;
    if (x != 46) {
    } else {
        z = block->z.fixed >> 20;
        if (z != 45) {
        } else {
            set = Engine_GameFlagIsSet(0x301);
            if (set != 0) {
            } else {
                if ((leader->z.fixed >> 20) <= 45) {
                    marker = OverlayObject_PrepareObjectWithCommand15(0x2e80000, 0, 0x2c00000, 20);
                    Engine_ActorSetSpritePriority(0, 3);
                }
                actor = Object_GetById(9);
                Engine_ActorSetSpriteFlags(actor, 0);
                block->terrain_id = set;
                block->motion_flags = 3;
                block->gravity = 0x1999;
                block->bounce = set;
                Engine_MapCopyCellAttributes(43, 45, 1, 1, x, z);
                OverlayObject_WaitUntilIdle(block);
                Engine_AudioPlayCue(188);
                block->motion_flags = set;
                block->y.fixed = -0x100000;
                Engine_ActorSetSpritePriority(9, 3);
                {
                    u8 two = 2;

                    *flags = two;
                }
                Engine_MapCopyCellAttributes(45, 45, 1, 1, x, z);
                Object_GetById(0)->priority_flags |= 1;
                Engine_ObjectDispatchRelease(marker);
                block->collision_flags = set;
                *flags |= 2;
                Object_GetById(10)->collision_flags = set;
                Object_GetById(10)->priority_flags |= 2;
                Engine_ActorSetPosition(10, 0x3280000, 0x2d80000);
                Engine_ActorEnableActionCallback(10, gVinasuPushScript);
                Object_RefreshSelectorById(10);
                Engine_EventWait(30);
                Engine_AudioPlayCue(158);
                Engine_MapAnimateCells(gVinasuPushCells, 110, 41);
                Map_CopyCellAttributes(46, 41, 1, 1, x, 42);
                Engine_GameFlagSet(0x301);
            }
        }
    }
    Engine_EventEnd();
}
