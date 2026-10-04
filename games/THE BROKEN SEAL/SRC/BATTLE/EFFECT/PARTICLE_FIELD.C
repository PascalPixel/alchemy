#include "RUNTIME_MEM.H"
#include "HEAP_STATE.H"
#include "CANVAS.H"
#include "RESOURCE.H"
#include "BATTLE_PRESENTATION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

extern u8 gBattleFxWork[];

/* Heap-allocation cache: gWorkSlot[kind] holds kind's block address. */

void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

extern u16 ParticleStreams_CellOffsets[];
extern u8 ParticleField_Counts[][3];
extern u16 ParticleField_PuffCells[];
extern u16 ParticleField_PuffSizes[];

void BattleFx_RunParticleField(struct BattleEffectArgument *effect, s32 mode);

/* Mode entries of the particle field effect. */

void BattleFx_RunParticleFieldMode1(struct BattleEffectArgument *effect)
{
    BattleFx_RunParticleField(effect, 1);
}

void BattleFx_RunParticleFieldMode0(struct BattleEffectArgument *effect)
{
    BattleFx_RunParticleField(effect, 0);
}
