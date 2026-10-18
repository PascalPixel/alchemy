// Battle-effect dispatch, drawing and presentation setup.
// Far-call interfaces; their ordered jump stubs live in
// games/tbs/src/system/far_call/EFFECT.S.
s32 BattleEffect_LoadWorkFar(s32 slot, s32 width_shift, s32 height_shift,
                             s32 flags, u32 mode);
void BattleFx_DispatchByIdRangeFar(struct BattleEffectArgument *effect);
void BattleFx_RunSceneEffectFar(struct BattleEffectArgument *effect);
void BattleFx_RunTableEffectFar(struct BattleEffectArgument *effect);
void BattleFx_RunElementPillarFar(struct BattleEffectArgument *effect);
void BattleFx_RunShatteringOpeningFar(struct BattleEffectArgument *effect);
void BattleFx_RunElementBurstFar(struct BattleEffectArgument *effect);
void BattleFx_StartElementStreaksFar(s32 kind);
void BattleFx_EndElementStreaksFar(void);
