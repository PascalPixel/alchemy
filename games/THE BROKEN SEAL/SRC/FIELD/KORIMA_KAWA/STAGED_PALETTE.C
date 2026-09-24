/*
 * resource_393 scene script: staged-actor motion, scene beats, and the
 * overlay's palette adjustment.
 */
#include "TYPES.H"

/*
 * Several aliases below resolve to the same relocation.  The spellings are
 * call-site evidence and are deliberately kept distinct.
 */
#define GetStagedActorEffect Func_0200193e
#define CanStartStagedActorEffect Func_02001916
#define BeginStagedActorEffect Func_02001948
#define SetStagedActorEffectMode Func_02001908
#define SelectStagedActorEffectSlot Func_02001906
#define SetStagedActorMotionMode Func_0200191c
#define PrepareStagedActorEffect Func_02001966
#define TestStagedActorEffectCell Func_020019b4_a
#define StartStagedActorEffectMove Func_02001954
#define SetStagedActorEffectTransition Func_02001984
#define WaitStagedActorEffect Func_020019e8
#define AdvanceStagedActorEffect Func_02001974
#define WaitSceneFrames Func_02001986
#define RestoreStagedActorEffect Func_0200199c
#define FinishStagedActorEffect_02000aac Func_02001a08
#define GetResource393Object Func_02001a38
#define ApplyResource393Position Func_0200168a
#define Resource393SharedWork Data_02000240
#define SCENE_WORKSPACE (*(u8 **)0x03001ebc)
#define RunSceneBeat10 Func_020014ce
#define IsSceneFlag0201Set Func_02001a70
#define GetSceneBeatSubject Func_02001aa2
#define DrawSceneBeatRectangle Func_02001a7c
#define FillSceneTileAttributes Func_02000e90
#define GetScenePresentationSubject Func_02001ad6
#define SetScenePresentationMode Func_02001aac
#define RunSceneBeat8 Func_0200151e
#define RunSceneBeat9 Func_02001524
#define IsSceneFlag0845Set Func_02001ac6
#define RunPhase516Followup SceneEffect_AdjustPaletteColors

#include "STAGED_ACTOR.H"
#include "STAGED_ACTOR_EFFECT.H"
#include "RESOURCE_393.H"

struct PlacementTail {
    s32 fifth;
    void (*callback)(void);
};

struct PlacementResult {
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
    struct PlacementTail tail;
};

struct SceneBeatSubject {
    u8 unknown_00[0x23];
    u8 marker;
};

void Func_02001872(void);
s32 Func_02000e7a(struct PlacementResult *result);
void Func_02001026(struct PlacementResult result);
void Func_020018e0(s32, s32); void Func_020018dc(s32, s32, s32);
void Func_020018b2(s32); u8 *Func_02001918();
void Func_020018b8(s32, s32, s32, s32, s32, s32);
s32 Func_02000ccc(s32, s32, s32, s32, s32, s32);
void Func_020018f2(s32); void Func_020018ee(u8 *, s32);
void Func_0200191a(void);
struct StagedActorEffect *Func_0200193e(s32 actor_index);
s32 Func_02001916(struct StagedActorEffect *actor,
                         struct StagedActorEffectRequest *request);
void Func_02001948(void);
void Func_02001908(struct StagedActorEffect *actor, s32 mode);
void Func_02001906(s32 actor_index);
void Func_0200191c(struct StagedActorEffect *actor, s32 mode);
void Func_02001966(struct StagedActorEffect *actor, s32 mode);
void Func_02001954(struct StagedActorEffect *actor, s32 mode);
void Func_02001984(struct StagedActorEffect *actor, s32 mode);
void Func_020019e8(s32 frames, s32 mode);
void Func_02001974(s32 mode);
void Func_02001986(s32 frames);
void Func_0200199c(s32 mode);
void Func_02001a08(void);
s32 Func_020014ce();
s32 Func_02001a70();
struct SceneBeatSubject *Func_02001aa2();
void Func_02001a7c();
s32 Func_02000e90();
struct SceneBeatSubject *Func_02001ad6();
void Func_02001aac();
void Func_0200151e();
void Func_02001524();
s32 Func_02001ac6();
void Func_020019ee(void);
void Func_02001a72(void);
void Func_02001a56(void);
void Func_02001b98(s32, s32);

/*
 * Six-argument draw wrapper.  Inlining it here preserves the reference's
 * r2-before-r3 stacked-literal order at the call site.
 */

void SceneEffect_AdjustPaletteColors(s32 a);

static __inline__ void DrawPlacement(s32 left, s32 top, s32 width, s32 height,
                                     s32 tile, s32 palette)
{
    void Actor_SetAnimation(s32, s32); u8 *Func_020018de(s32);

    Func_020018b8(left, top, width, height, tile, palette);
}

static __inline__ void DrawSceneBeat(s32 left, s32 top, s32 width, s32 height,
                                     s32 tile, s32 palette)
{

    DrawSceneBeatRectangle(left, top, width, height, tile, palette);
}

/*
 * One symbol per call site, named at the site's own address.  All three
 * reach the same ARM-mode IWRAM helper that scales a channel by the
 * adjustment, and each still needs its own name.  The sites are 0x02000d14,
 * 0x02000d22 and 0x02000d30.
 */

s32 Func_02001b32();

s32 Func_02001b40();

s32 Func_02001b4e();

u8 *MapStagedScene_SelectPrimaryData(void) { return (u8 *)0x02008fc8; }

s32 MapStagedScene_GetEmptyData(void)
{
    return 0;
}

u8 *MapStagedScene_SelectSecondaryData(void) { return (u8 *)0x02009028; }

u8 *MapStagedScene_SelectTertiaryData(void) { return (u8 *)0x02009038; }

/*
 * Placement query followed by the tile-(10,12) scene transition.  The
 * six-word result is one aggregate and its two-word tail is forwarded by
 * value.  Keeping `zero' live across the draw is load-bearing: it lets the
 * dead result pointer be reused for the following stack slot.
 */

void FieldScene_RunActorTenPlacementScene(void)
{
    void Engine_ActorSetAnimation(s32, s32); u8 *Func_020018de(s32);

    struct PlacementResult result;
    Func_02001872();
    if (Func_02000e7a(&result)) {
        Func_02001026(result);
        if (result.second == 10 && (result.third >> 20) == 12) {
            u8 *actor;
            s32 zero;
            Func_020018e0(10, 3);
            Func_020018dc(10, -18, 6);
            Func_020018b2(30);
            Audio_PlayCue(240);
            Engine_ActorSetAnimation(10, 8);
            Func_020018de(10)[35] = 2;
            zero = 0;
            DrawPlacement(32, 20, 2, 4, 11, 16);
            Func_02000ccc(2, 12, 16, 1, 4, zero);
            Func_020018f2(0x201);
            actor = Func_02001918(10);
            Func_020018ee(actor, 0);
        }
    }
    Func_0200191a();
}

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request)
{
    s32 Func_020019b4_a();

    struct StagedActorEffect *actor = GetStagedActorEffect(0);
    u8 *flags = &actor->motion_flags;
    u8 saved = *flags;
    s32 result = CanStartStagedActorEffect(actor, request);

    if (result == 0) {
        BeginStagedActorEffect();
        SetStagedActorEffectMode(actor, 6);
        SelectStagedActorEffectSlot(6);
        Audio_PlayCue(152);
        SetStagedActorMotionMode(actor, 7);
        actor->move_rate_x = 0x30000;
        actor->move_rate_z = 0x20000;
        actor->elevation_rate = 0x40000;
        *flags &= 0x7e;
        PrepareStagedActorEffect(actor, 0);
        TestStagedActorEffectCell(0, request->cell_x, request->cell_z);
        StartStagedActorEffectMove(actor, 6);
        SetStagedActorEffectTransition(actor, 1);
        *flags = (u8)result;
        WaitStagedActorEffect(10, 7);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        AdvanceStagedActorEffect(2);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        WaitSceneFrames(10);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        RestoreStagedActorEffect(4);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        *flags = saved;
        FinishStagedActorEffect_02000aac();
        return 1;
    }
    return 0;
}

void SceneActor_ApplyOffsetObjectPosition(void)
{
    struct Resource393Position pos;
    struct Resource393Object *obj = GetResource393Object(Resource393SharedWork.object_id);
    u32 xb = obj->position_x & 0xfff00000;

    pos.x = xb + 0x80000;
    pos.y = obj->position_y;
    pos.z = (obj->position_z & 0xfff00000) + 0x80000;
    pos.x = xb + 0x280000;
    ApplyResource393Position(&pos);
}

u8 *SceneData_GetTable9098(void) { return (u8 *)0x02009098; }

/* Set workspace word 448 to 516, then run the scene's beat sequence. */
s32 SceneState_SetRuntimeWord448To516(void)
{

    u8 *work = SCENE_WORKSPACE;

    *(s32 *)(work + 448) = 516;
    RunSceneBeat10(10);

    if (IsSceneFlag0201Set(0x201) != 0) {
        struct SceneBeatSubject *subj = GetSceneBeatSubject(10);

        subj->marker = 2;
        DrawSceneBeat(32, 20, 2, 4, 11, 16);
        FillSceneTileAttributes(2, 12, 16, 1, 4, 0);
        SetScenePresentationMode(GetScenePresentationSubject(10), 0);
    }

    RunSceneBeat8(8);
    RunSceneBeat9(9);

    if (IsSceneFlag0845Set(0x845) == 0) {
        RunPhase516Followup(6);
    }
    return 0;
}

void SceneEffect_AdjustPaletteColors(s32 a)
{

    u32 x;

    Func_020019ee();
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
    Func_02001a72();
    Func_02001a56();
    Func_02001b98(0x10000, 0);
}

/*
 * resource_393 owner at 0x02000cf4, 104 bytes: the asymmetric RGB555 colour
 * adjustment.  Red rises while green and blue fall, each through the same
 * per-channel scale.
 */
u16 SceneEffect_AdjustColorChannels(u16 color, s32 adj)
{

    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Func_02001b32(
        red,
        (s32)((u32)adj << 2)
    ));
    green = (s16)(green - Func_02001b40(green, adj));
    blue = (s16)(blue - Func_02001b4e(blue, adj));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
