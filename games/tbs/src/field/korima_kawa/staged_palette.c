#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "types.h"
#include "tbs_edition.h"

#include "staged_actor.h"
#include "staged_actor_effect.h"
#include "resource_393.h"
#include "fieldobj.h"

void Engine_ActorSetSpriteFlags(struct DispatchObject *object, u32 value);

void Engine_MapCopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y);

void Engine_ColorBufferApplyTarget(s32 value, s32 mode);

void Engine_AudioPlayCue(s32 cue);

extern struct GameState gGameState;
#ifdef MODERN
// called before its definition below, which modern GCC refuses for an
// implicit int declaration of a u16 function
u16 SceneEffect_AdjustColorChannels(u16 color, s32 adj);
#endif

#define Engine_ActorSetSpriteFlags ((void (*)(u8*,s32))Engine_ActorSetSpriteFlags)


struct SceneBeatSubject {
    u8 unknown_00[0x23];
    u8 marker;
};

void Engine_EventBegin(void);
void Engine_ActorSetAnimation(s32, s32);
void Engine_ActorSetDestinationOffset(s32, s32, s32);
void Engine_EventWait(s32);
u8 *Object_GetById();
void Engine_GameFlagSet(s32);
void Engine_EventEnd(void);
s32 Object_CheckMovementCollision(struct StagedActorEffect *actor, struct StagedActorEffectRequest *request);
s32 Engine_GameFlagIsSet();
void Engine_TaskWait(s32 actor_index);
void Object_SetMode(struct StagedActorEffect *actor, s32 mode);
void KorimaPalette_SaveFirst(void);
void KorimaPalette_Capture(void);
void KorimaPalette_SaveSecond(void);


static __inline__ void DrawPlacement(s32 left, s32 top, s32 width, s32 height,
                                     s32 tile, s32 palette)
{
    /* FAKEMATCH: a direct call builds both stacked literals in r3; the reference builds palette 16 in r2 first. */
    void Engine_ActorSetAnimation(s32, s32); u8 *Object_GetById(s32);

    Engine_MapCopyCellAttributes(left, top, width, height, tile, palette);
}

static __inline__ void DrawSceneBeat(s32 left, s32 top, s32 width, s32 height,
                                     s32 tile, s32 palette)
{
    /* FAKEMATCH: a direct call builds both stacked literals in r3; the reference builds palette 16 in r2 first. */
    Engine_MapCopyCellAttributes(left, top, width, height, tile, palette);
}

s32 Math_DivideSigned();

u8 *MapStagedScene_SelectPrimaryData(void) { return Data_02008fc8; }

s32 MapStagedScene_GetEmptyData(void)
{
    return 0;
}

u8 *MapStagedScene_SelectSecondaryData(void) { return Data_02009028; }

u8 *MapStagedScene_SelectTertiaryData(void) { return Data_02009038; }

void FieldScene_RunActorTenPlacementScene(void)
{
    /* FAKEMATCH: replacing the live zero below with 0 changes r5 allocation and the outgoing stack store order. */
    void Engine_ActorSetAnimation(s32, s32); u8 *Object_GetById(s32);

    struct StagedActorProbe result;
    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&result)) {
        SceneActor_MoveAndRedraw(result);
        if (result.actor_slot == 10 && (result.position_x >> 20) == 12) {
            struct FieldActor *actor;
            s32 zero;
            Engine_ActorSetAnimation(10, 3);
            Engine_ActorSetDestinationOffset(10, -18, 6);
            Engine_EventWait(30);
            Engine_AudioPlayCue(240);
            Engine_ActorSetAnimation(10, 8);
            Object_GetById(10)[35] = 2;
            zero = 0;
            DrawPlacement(32, 20, 2, 4, 11, 16);
            StagedActor_FillGridAttributeRectangle(2, 12, 16, 1, 4, zero);
            Engine_GameFlagSet(0x201);
            actor = Object_GetById(10);
            Engine_ActorSetSpriteFlags(actor, 0);
        }
    }
    Engine_EventEnd();
}

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request)
{
    s32 ObjectMotion_SetPositionAndCommit();

    struct StagedActorEffect *actor = Object_GetById(0);
    u8 *flags = &actor->motion_flags;
    u8 saved = *flags;
    s32 result = Object_CheckMovementCollision(actor, request);

    if (result == 0) {
        Engine_EventBegin();
        Object_SetMode(actor, 6);
        Engine_TaskWait(6);
        Engine_AudioPlayCue(152);
        Object_SetMode(actor, 7);
        actor->move_rate_x = 0x30000;
        actor->move_rate_z = 0x20000;
        actor->elevation_rate = 0x40000;
        *flags &= 0x7e;
        Engine_ActorSetSpriteFlags(actor, 0);
        ObjectMotion_SetPositionAndCommit(0, request->cell_x, request->cell_z);
        Object_SetMode(actor, 6);
        Engine_ActorSetSpriteFlags(actor, 1);
        *flags = (u8)result;
        Engine_ActorSetAnimation(10, 7);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        Engine_TaskWait(2);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        Engine_TaskWait(10);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        Engine_TaskWait(4);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        *flags = saved;
        Engine_EventEnd();
        return 1;
    }
    return 0;
}

void SceneActor_ApplyOffsetObjectPosition(void)
{
    struct Resource393Position pos;
#if !EDITION_INTERNATIONAL
    pos.x = ((struct Resource393Object *)Object_GetById(0))->position_x + 0x1e0000;
    pos.y = ((struct Resource393Object *)Object_GetById(0))->position_y;
    pos.z = ((struct Resource393Object *)Object_GetById(0))->position_z;
#else
    struct Resource393Object *obj = Object_GetById(((struct Resource393Work *)&gGameState)->object_id);
    u32 xb = obj->position_x & 0xfff00000;

    pos.x = xb + 0x80000;
    pos.y = obj->position_y;
    pos.z = (obj->position_z & 0xfff00000) + 0x80000;
    pos.x = xb + 0x280000;
#endif
    StagedActor_RunStepEffect(&pos);
}

u8 *KorimaKawa_GetEvents(void) { return Data_02009098; }

s32 KorimaKawa_PrepareWindowTransition(void)
{
    struct EventWork *work = gEventWork;

    work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    FieldScene_RedrawActorFootprint(10);

    if (Engine_GameFlagIsSet(0x201) != 0) {
        struct SceneBeatSubject *subj = Object_GetById(10);

        subj->marker = 2;
        DrawSceneBeat(32, 20, 2, 4, 11, 16);
        StagedActor_FillGridAttributeRectangle(2, 12, 16, 1, 4, 0);
        Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
    }

    FieldScene_RedrawActorFootprint(8);
    FieldScene_RedrawActorFootprint(9);

    if (Engine_GameFlagIsSet(0x845) == 0) {
        SceneEffect_AdjustPaletteColors(6);
    }
    return 0;
}

void SceneEffect_AdjustPaletteColors(s32 a)
{
    u16 i;

    KorimaPalette_SaveFirst();
    for (i = 0; i < 224; i++) {
        if ((i < 17 || i > 23) && (i < 193 || i > 200)) {
            u16 *pal = (u16 *)(0x5000000 + i * 2);
            *pal = SceneEffect_AdjustColorChannels(*pal, a);
        }
    }
    KorimaPalette_Capture();
    KorimaPalette_SaveSecond();
    Engine_ColorBufferApplyTarget(0x10000, 0);
}

u16 SceneEffect_AdjustColorChannels(u16 color, s32 adj)
{

    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Math_DivideSigned(
        red,
        (s32)((u32)adj << 2)
    ));
    green = (s16)(green - Math_DivideSigned(green, adj));
    blue = (s16)(blue - Math_DivideSigned(blue, adj));

    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
