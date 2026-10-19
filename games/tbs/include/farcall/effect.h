/// @file
/// Battle-effect dispatch, drawing and presentation setup.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/EFFECT.S.

/// Far call to BattleFx_BuildBlitter.
s32 BattleEffect_LoadWorkFar(s32 slot, s32 width_shift, s32 height_shift,
                             s32 flags, u32 mode);
/// Far call to BattleFx_DispatchByIdRange.
void BattleFx_DispatchByIdRangeFar(struct BattleEffectArgument *effect);
/// Far call to BattleFx_RunSceneEffect.
void BattleFx_RunSceneEffectFar(struct BattleEffectArgument *effect);
/// Far call to BattleFx_RunTableEffect.
void BattleFx_RunTableEffectFar(struct BattleEffectArgument *effect);
/// Far call to BattleFx_RunElementPillar.
void BattleFx_RunElementPillarFar(struct BattleEffectArgument *effect);
/// Far call to BattleFx_RunShatteringOpening.
void BattleFx_RunShatteringOpeningFar(struct BattleEffectArgument *effect);
/// Far call to BattleFx_RunElementBurst.
void BattleFx_RunElementBurstFar(struct BattleEffectArgument *effect);
/// Far call to BattleFx_StartElementStreaks.
void BattleFx_StartElementStreaksFar(s32 element);
/// Far call to BattleFx_EndElementStreaks.
void BattleFx_EndElementStreaksFar(void);
