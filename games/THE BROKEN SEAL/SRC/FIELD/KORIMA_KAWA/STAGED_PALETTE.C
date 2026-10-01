/*
 * resource_393 scene script: staged-actor motion, scene beats, and the
 * overlay's palette adjustment.
 */
#include "TYPES.H"

#include "STAGED_ACTOR.H"
#include "STAGED_ACTOR_EFFECT.H"
#include "RESOURCE_393.H"

extern u8 *gWork;
extern u8 Data_02008fc8[];
extern u8 Data_02009028[];
extern u8 Data_02009038[];
extern u8 Data_02009098[];

struct SceneBeatSubject {
    u8 unknown_00[0x23];
    u8 marker;
};

void Battle_Reset(void);
void Object_SetModeById(s32, s32); void ObjectMotion_OffsetPositionAndResetMotion(s32, s32, s32);
void Battle_WaitMode0(s32); u8 *Object_GetById();
void Map_CopyCellAttributeRect(s32, s32, s32, s32, s32, s32);
void GameFlag_SetBit(s32); void ObjectDispatch_SetSingleChildField26(u8 *, s32);
void BattleFx_FinishAction(void);
s32 Object_CheckMovementCollision(struct StagedActorEffect *actor,
                         struct StagedActorEffectRequest *request);
s32 GameFlag_Test();
void WaitFrames(s32 actor_index);
void Object_SetMode(struct StagedActorEffect *actor, s32 mode);
void KorimaPalette_SaveFirst(void);
void KorimaPalette_Capture(void);
void KorimaPalette_SaveSecond(void);
void BattleFx_ApplyColorToTargetBuffer(s32, s32);

/*
 * Six-argument draw wrapper.  Inlining it here preserves the reference's
 * r2-before-r3 stacked-literal order at the call site.
 */

void SceneEffect_AdjustPaletteColors(s32 a);

static __inline__ void DrawPlacement(s32 left, s32 top, s32 width, s32 height,
                                     s32 tile, s32 palette)
{
    void Actor_SetAnimation(s32, s32); u8 *Object_GetById(s32);

    Map_CopyCellAttributeRect(left, top, width, height, tile, palette);
}

static __inline__ void DrawSceneBeat(s32 left, s32 top, s32 width, s32 height,
                                     s32 tile, s32 palette)
{

    Map_CopyCellAttributeRect(left, top, width, height, tile, palette);
}

s32 Math_DivideSigned();

u8 *MapStagedScene_SelectPrimaryData(void) { return Data_02008fc8; }

s32 MapStagedScene_GetEmptyData(void)
{
    return 0;
}

u8 *MapStagedScene_SelectSecondaryData(void) { return Data_02009028; }

u8 *MapStagedScene_SelectTertiaryData(void) { return Data_02009038; }

/*
 * Placement query followed by the tile-(10,12) scene transition.  The
 * six-word result is one aggregate and its two-word tail is forwarded by
 * value.  Keeping `zero' live across the draw is load-bearing: it lets the
 * dead result pointer be reused for the following stack slot.
 */

void FieldScene_RunActorTenPlacementScene(void)
{
    void Object_SetModeById(s32, s32); u8 *Object_GetById(s32);

    struct StagedActorProbe result;
    Battle_Reset();
    if (StagedActor_FindClearPosition(&result)) {
        SceneActor_MoveAndRedraw(result);
        if (result.actor_slot == 10 && (result.position_x >> 20) == 12) {
            u8 *actor;
            s32 zero;
            Object_SetModeById(10, 3);
            ObjectMotion_OffsetPositionAndResetMotion(10, -18, 6);
            Battle_WaitMode0(30);
            Audio_PlayCue(240);
            Object_SetModeById(10, 8);
            Object_GetById(10)[35] = 2;
            zero = 0;
            DrawPlacement(32, 20, 2, 4, 11, 16);
            StagedActor_FillGridAttributeRectangle(2, 12, 16, 1, 4, zero);
            GameFlag_SetBit(0x201);
            actor = Object_GetById(10);
            ObjectDispatch_SetSingleChildField26(actor, 0);
        }
    }
    BattleFx_FinishAction();
}

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request)
{
    s32 ObjectMotion_SetPositionAndCommit();

    struct StagedActorEffect *actor = Object_GetById(0);
    u8 *flags = &actor->motion_flags;
    u8 saved = *flags;
    s32 result = Object_CheckMovementCollision(actor, request);

    if (result == 0) {
        Battle_Reset();
        Object_SetMode(actor, 6);
        WaitFrames(6);
        Audio_PlayCue(152);
        Object_SetMode(actor, 7);
        actor->move_rate_x = 0x30000;
        actor->move_rate_z = 0x20000;
        actor->elevation_rate = 0x40000;
        *flags &= 0x7e;
        ObjectDispatch_SetSingleChildField26(actor, 0);
        ObjectMotion_SetPositionAndCommit(0, request->cell_x, request->cell_z);
        Object_SetMode(actor, 6);
        ObjectDispatch_SetSingleChildField26(actor, 1);
        *flags = (u8)result;
        Object_SetModeById(10, 7);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        WaitFrames(2);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        WaitFrames(10);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        WaitFrames(4);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        *flags = saved;
        BattleFx_FinishAction();
        return 1;
    }
    return 0;
}

void SceneActor_ApplyOffsetObjectPosition(void)
{
    struct Resource393Position pos;
    struct Resource393Object *obj = Object_GetById(Data_02000240.object_id);
    u32 xb = obj->position_x & 0xfff00000;

    pos.x = xb + 0x80000;
    pos.y = obj->position_y;
    pos.z = (obj->position_z & 0xfff00000) + 0x80000;
    pos.x = xb + 0x280000;
    StagedActor_RunStepEffect(&pos);
}

u8 *SceneData_GetTable9098(void) { return Data_02009098; }

/* Set workspace word 448 to 516, then run the scene's beat sequence. */
s32 SceneState_SetRuntimeWord448To516(void)
{

    u8 *work = gWork;

    *(s32 *)(work + 448) = 516;
    FieldScene_RedrawActorFootprint(10);

    if (GameFlag_Test(0x201) != 0) {
        struct SceneBeatSubject *subj = Object_GetById(10);

        subj->marker = 2;
        DrawSceneBeat(32, 20, 2, 4, 11, 16);
        StagedActor_FillGridAttributeRectangle(2, 12, 16, 1, 4, 0);
        ObjectDispatch_SetSingleChildField26(Object_GetById(10), 0);
    }

    FieldScene_RedrawActorFootprint(8);
    FieldScene_RedrawActorFootprint(9);

    if (GameFlag_Test(0x845) == 0) {
        SceneEffect_AdjustPaletteColors(6);
    }
    return 0;
}

void SceneEffect_AdjustPaletteColors(s32 a)
{

    u32 x;

    KorimaPalette_SaveFirst();
    x = 0;
    do {
        u32 idx = x >> 16;
        if (x + 0xffef0000 > 0x60000 && (idx + 0xff3f) << 16 > 0x70000) {
            u16 *pal = (u16 *)(0x5000000 + idx * 2);
            *pal = SceneEffect_AdjustColorChannels(*pal, a);
        }
        {
            u32 nx = x + 0x10000;
            x = nx;
            if (nx > 0xdf0000) {
                break;
            }
        }
    } while (1);
    KorimaPalette_Capture();
    KorimaPalette_SaveSecond();
    BattleFx_ApplyColorToTargetBuffer(0x10000, 0);
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

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
