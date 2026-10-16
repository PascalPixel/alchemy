// Battle-effect dispatch, drawing and presentation setup.
// Far-call interfaces; their ordered jump stubs live in
// games/tbs/src/system/far_call/EFFECT.S.
s32 BattleEffect_LoadWorkFar(s32 slot, s32 width_shift, s32 height_shift,
                             s32 flags, u32 mode);
void BattleFx_DispatchByIdRangeFar(struct BattleEffectArgument *effect);
void BattleFx_InitializeModeFar(struct BattleEffectArgument *effect);
void BattleFx_DispatchModeFar(struct BattleEffectArgument *effect);
void BattleFx_RunIceFleckBurstFar(struct BattleEffectArgument *effect);
void BattleEffect_RunTileAndPaletteAnimationFar(
    struct BattleEffectArgument *effect);
void BattleFx_RunGatheringBurstFar(struct BattleEffectArgument *effect);
void BattlePresentation_PrepareSceneFar(s32 kind);
void BattleFx_ScheduleCallbacksAndReleaseBlocksFar(void);
