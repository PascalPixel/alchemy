#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "ENTRY_SETUP.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"
#include "SCENE_IDS.H"

void BattleFx_SetQueuedSoundAndPlay(s32 cue);

struct FieldActor;

static __inline__ void Rubble_SetScaleAndSpin(struct EffectOptions *opts)
{
    opts->start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
    opts->start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
    opts->spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
}

void Scene_RunPairedParticleWaveSequence(void)
{
    struct EffectOptions options;
    u32 repeat;
    u32 row;
    u32 i;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventBegin();
    Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
    Engine_ActorSetChildValue(0, 15);
    BattleFx_SetQueuedSoundAndPlay(170);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_AudioPlayCue(162);
    repeat = 0;
    for (row = 0; row < 10; row++) {
        options.start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    again:
        for (i = 0; i < 4 && row < 8; i++) {
            Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x3600000, 0,
                         0x300000 + row * 0x100000 + i * 0x40000, 0, 0, 0, 0x880000, &options);
        }
        Engine_TaskWait(3);
        if (row == 3 && repeat < 3) {
            repeat++;
            goto again;
        }
        Map_CopyCellsTo(48, row + 3, 54, row + 3, 3, 1);
    }
    Map_CopyCellsTo(111, 5, 117, 5, 5, 2);
    Map_CopyCellsTo(111, 10, 117, 10, 5, 2);
    Map_CopyCellsTo(111, 7, 111, 5, 5, 2);
    Map_CopyCellsTo(111, 7, 111, 10, 5, 2);
    for (row = 0; row < 10; row++) {
        Rubble_SetScaleAndSpin(&options);
        for (i = 0; i < 4 && row < 8; i++) {
            Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x3000000, 0,
                         0x300000 + row * 0x100000 + i * 0x40000, 0, 0, 0, 0x880000, &options);
        }
        Engine_TaskWait(3);
        Map_CopyCellsTo(55, row + 26, 48, row + 3, 3, 1);
    }
    Engine_AudioPlayCue(289);
    Engine_EventWait(60);
    Engine_EventRequestExit(21);
    Engine_EventEnd();
}

void VinasuHeya_SpawnRandomParticles(s32 x, s32 z)
{
    struct EffectOptions options;
    struct EffectOptions *p = &options;
    register struct EffectOptions *opts asm("r8"); /* FAKEMATCH: the options pointer is held in r8 once both scales are stored through p. 2026-10-02: an ordinary pointer puts options in r7 and x in r8, changing spin storage and both spawn calls. */
    s32 phase;
    u32 coin;

    p->start_scale_x = 0xb333;
    p->start_scale_y = 0xb333;
    opts = p;
    {
        s32 spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        u32 *frame = &gFrameCount; /* FAKEMATCH: the counter's address is loaded before the spin is stored */

        opts->spin = spin;
        phase = *frame & 3;
    }
    if (phase == 0) {
        coin = (u32)(Engine_RandomNext() << 1) >> 16;
        if (coin != 0) {
            s32 r = Engine_RandomNext();
            s32 vz = __divsi3((((u32)(Engine_RandomNext() * 5) >> 16) << 16) + 0x70000, 10);

            Effect_Spawn((x + (((u32)(r << 1) >> 16) << 4)) << 16, 0, z << 19, 0, phase, vz, 0x880000, opts);
        } else {
            s32 r = Engine_RandomNext();

            Effect_Spawn((x + ((u32)(r * 17) >> 16)) << 16, 0, (x << 19) - 0x40000, 0, coin, coin, 0x880000, opts);
        }
    }
}

void SceneState_CallWith432And32(void)
{
    VinasuHeya_SpawnRandomParticles(432, 32);
}

void FieldScene_CallWith560And44(void)
{
    VinasuHeya_SpawnRandomParticles(560, 44);
}

void Scene_RunParticleWaveSequence(void)
{
    struct EffectOptions options;
    u32 repeat;
        s32 zero;
    u32 row;
    s32 offset;
    s32 z;
    u32 i;
    struct EffectOptions *opts;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventBegin();
    Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
    Engine_ActorSetChildValue(0, 15);
    BattleFx_SetQueuedSoundAndPlay(170);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_AudioPlayCue(162);
    Engine_CameraSetSpeed(0x8000, 0x1000);
    Engine_CameraMoveTo(0x1b80000, -1, 0x1680000, 1);
    repeat = 0;
    row = 0;
    opts = &options;
    offset = 0;
    do {
        options.start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    again:
        z = 0xc00000;
        for (i = 0, zero = 0, z += offset; i <= 3; i++) {
            Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x1a00000, 0, z, 0, zero, zero, 0x880000, opts);
            z += 0x40000;
        }
        Engine_TaskWait(3);
        if (row == 3) {
            if (repeat <= 2) {
                repeat++;
                goto again;
            }
            if (repeat == 3)
                Engine_TaskAddCallback(SceneState_CallWith432And32, 3200);
        }
        Map_CopyCellsTo(53, row + 12, 26, row + 12, 3, 1);
        offset += 0x100000;
        row++;
    } while (row <= 12);
    Map_CopyCellsTo(81, 41, 89, 14, 9, 2);
    Engine_CameraWaitForMove();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_EventWait(60);
    Engine_GameFlagSet(0x306);
    Engine_EventRequestExit(19);
    Engine_EventEnd();
}

void Scene_RunEastParticleWaveSequence(void)
{
    struct EffectOptions options;
    u32 repeat;
        s32 zero;
    u32 row;
    s32 offset;
    s32 z;
    u32 i;
    struct EffectOptions *opts;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventBegin();
    Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
    Engine_ActorSetChildValue(0, 15);
    BattleFx_SetQueuedSoundAndPlay(170);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_AudioPlayCue(162);
    Engine_CameraSetSpeed(0x8000, 0x1000);
    Engine_CameraMoveTo(0x2380000, -1, 0x1680000, 1);
    repeat = 0;
    row = 0;
    opts = &options;
    offset = 0;
    do {
        options.start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    again:
        z = 0xc00000;
        for (i = 0, zero = 0, z += offset; i <= 3; i++) {
            Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x2200000, 0, z, 0, zero, zero, 0x880000, opts);
            z += 0x40000;
        }
        Engine_TaskWait(3);
        if (row == 3 && repeat <= 2) {
            repeat++;
            goto again;
        }
        Engine_TaskAddCallback(FieldScene_CallWith560And44, 3200);
        Map_CopyCellsTo(58, row + 12, 34, row + 12, 3, 1);
        offset += 0x100000;
        row++;
    } while (row <= 12);
    Map_CopyCellsTo(86, 41, 97, 14, 5, 2);
    Engine_CameraWaitForMove();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_EventWait(60);
    Engine_GameFlagSet(0x307);
    Engine_EventRequestExit(20);
    Engine_EventEnd();
}

s32 VinasuHeya_UpdateRisingSpray(struct FieldActor *actor)
{
    struct EffectOptions options;
    struct EffectOptions *o = &options;
    s32 phase;
    s32 x, y, z, rise;

    o->priority = 1;
    o->palette = 5;
    o->type = 286;
    o->script = gVinasuSprayScript;
    phase = gFrameCount & 3;
    if (phase == 0) {
        if ((gFrameCount & 7) == 0) {
            Engine_AudioPlayCue(246);
        }
        x = actor->x.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
        y = actor->y.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
        z = actor->z.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
        rise = (((u32)(Engine_RandomNext() * 4) >> 16) << 15) + 0x8000;
        Effect_Spawn(x, y, z, 0, rise, phase, 0x330000, o);
    }
    return 0;
}

void FieldScene_RunLeaderDropSequence(void)
{
    u32 i;
    struct FieldActor *leader;
    struct FieldActor *spray;

    leader = Object_GetById(ACTOR_PARTY_LEADER);
    spray = Object_GetById(20);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    leader->y.fixed = 130 << 16;
    leader->gravity = 0x8000;
    leader->bounce = 0;
    leader->motion_flags = 0;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Audio_PlayCue(204);
    Engine_EventWait(30);
    leader->motion_flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    Engine_EventWait(24);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 257);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 22);
    leader->motion_flags &= ~ACTOR_FOLLOWS_TERRAIN;
    spray->y.fixed += -3 << 16;
    leader->y.fixed += -3 << 16;
    leader->terrain_height += -3 << 16;
    Engine_TaskWait(2);
    spray->y.fixed += -2 << 16;
    leader->y.fixed += -2 << 16;
    leader->terrain_height += -2 << 16;
    Engine_TaskWait(10);
    spray->y.fixed += 2 << 16;
    leader->y.fixed += 2 << 16;
    leader->terrain_height += 2 << 16;
    Engine_TaskWait(4);
    spray->y.fixed += 2 << 16;
    leader->y.fixed += 2 << 16;
    leader->terrain_height += 2 << 16;
    Engine_TaskWait(4);
    spray->y.fixed += 1 << 16;
    leader->y.fixed += 1 << 16;
    leader->terrain_height += 1 << 16;
    leader->motion_flags = 0;
    spray->motion_flags = 0;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 256);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(40);
    leader->update = (void *)VinasuHeya_UpdateRisingSpray;
    Engine_EventWait(60);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetSpritePriority(20, 1);
    Audio_PlayCue(17);
    Audio_PlayCue(308);
    GameFlag_Set(0x101);
    for (i = 0; i < 128; i++) {
        leader->y.fixed += 1 << 16;
        leader->terrain_height += 1 << 16;
        spray->y.fixed += 1 << 16;
        Engine_TaskWait(1);
    }
    Engine_EventRequestExit(21);
}

struct TrackAnimation {
    s16 id;
};

struct TrackSprite {
    u8 unknown_00[36];
    u8 shape;
    u8 unknown_25[3];
    struct TrackAnimation *animations[4];
};

extern struct SwitchCell gVinasuPillarCells[];

s32 VinasuHeya_DispatchPushScript(struct FieldActor *actor)
{
    s32 pos[3];
    struct FieldActor *piece;
    struct TrackSprite *sprite;
    const u8 *shapes;
    u8 shape;
    u32 cell;
    s32 skip;
    const u8 *cursor;

    pos[0] = actor->x.fixed;
    pos[1] = actor->y.fixed - 0x100000;
    pos[2] = actor->z.fixed;
    piece = SceneData_FindSlotAtPosition(pos, 0);
    sprite = (struct TrackSprite *)piece->sprite;
    if (sprite->animations[0]->id == 256) {
        s32 vx = actor->velocity_x;
        s32 ax = vx < 0 ? -vx : vx;
        s32 vz = actor->velocity_z;
        s32 az = vz < 0 ? -vz : vz;

        if (ax > az) {
            if (vx / 0x10000 < 0) {
                shapes = gVinasuShapesPushedLeft;
            } else {
                shapes = gVinasuShapesPushedRight;
            }
        } else {
            if (vz / 0x10000 < 0) {
                shapes = gVinasuShapesPushedUp;
            } else {
                shapes = gVinasuShapesPushedDown;
            }
        }
        for (; *shapes != 0; shapes++) {
            if (sprite->shape == *shapes) {
                break;
            }
        }
        if (*shapes == 0) {
            Engine_ObjectSetScript(actor, gVinasuRefusedPushScript);
            return 0;
        }
        if (gGameState.scene == (s32)&SceneId_VinasuHeya5) {
            for (cell = 0; cell <= 3 && !(actor->x.fixed >> 20 == gVinasuSwitchCells[cell].x
                                          && actor->z.fixed >> 20 == gVinasuSwitchCells[cell].z);
                 cell++) {
            }
            for (skip = 0;; skip++) {
                cursor = gVinasuSwitchShapes[cell];
                shape = *cursor;
                if (shape == 0) {
                    Engine_ObjectSetScript(actor, gVinasuRefusedPushScript);
                    return 0;
                }
                if (shape == ((struct TrackSprite *)piece->sprite)->shape) {
                    break;
                }
                gVinasuSwitchShapes[cell] = cursor + 1;
            }
            Engine_ObjectSetScript(actor, gVinasuSwitchPushScripts[cell][skip]);
        } else {
            for (cell = 0; cell <= 7 && !(actor->x.fixed >> 20 == gVinasuPillarCells[cell].x
                                          && actor->z.fixed >> 20 == gVinasuPillarCells[cell].z);
                 cell++) {
            }
            for (skip = 0;; skip++) {
                cursor = gVinasuPillarShapes[cell];
                shape = *cursor;
                if (shape == 0) {
                    Engine_ObjectSetScript(actor, gVinasuRefusedPushScript);
                    return 0;
                }
                if (shape == ((struct TrackSprite *)piece->sprite)->shape) {
                    break;
                }
                gVinasuPillarShapes[cell] = cursor + 1;
            }
            Engine_ObjectSetScript(actor, gVinasuPillarPushScripts[cell][skip]);
        }
    } else {
        Engine_ObjectSetScript(actor, gVinasuRefusedPushScript);
    }
    return 0;
}
