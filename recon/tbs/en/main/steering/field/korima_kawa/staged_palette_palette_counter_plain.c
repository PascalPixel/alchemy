/* NONMATCHING: 2026-10-01 brief Wave2 palette-counter plain-source attempt.
 * Removing this one source device changes SceneEffect_AdjustPaletteColors.
 * First remaining difference: SceneEffect_AdjustPaletteColors: add	r3, r6, r2 => mov	r3, #223 (54/53 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * This reduced draft preserves the affected function and its declarations.
 * Production retains the measured device with its FAKEMATCH reason.
 */
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


s32 Math_DivideSigned();

u8 *MapStagedScene_SelectPrimaryData(void) ;

s32 MapStagedScene_GetEmptyData(void)
;

u8 *MapStagedScene_SelectSecondaryData(void) ;

u8 *MapStagedScene_SelectTertiaryData(void) ;

/*
 * Placement query followed by the tile-(10,12) scene transition.  The
 * six-word result is one aggregate and its two-word tail is forwarded by
 * value.  Keeping `zero' live across the draw is load-bearing: it lets the
 * dead result pointer be reused for the following stack slot.
 */

void FieldScene_RunActorTenPlacementScene(void)
;

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request)
;

void SceneActor_ApplyOffsetObjectPosition(void)
;

u8 *SceneData_GetTable9098(void) ;

/* Set workspace word 448 to 516, then run the scene's beat sequence. */
s32 SceneState_SetRuntimeWord448To516(void)
;

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
            x += 0x10000;
            if (x > 0xdf0000) {
                break;
            }
        }
    } while (1);
    KorimaPalette_Capture();
    KorimaPalette_SaveSecond();
    BattleFx_ApplyColorToTargetBuffer(0x10000, 0);
}

u16 SceneEffect_AdjustColorChannels(u16 color, s32 adj)
;
