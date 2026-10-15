/* FarCall_EffectTable: one entry per 8-byte stub, in table order. */
FAR_CALL(BattleEffect_LoadWorkFar, BattleEffect_LoadWork, s32,
         (s32 slot, s32 width_shift, s32 height_shift, s32 flags, u32 mode))
FAR_CALL(BattleFx_DispatchByIdRangeFar, BattleFx_DispatchByIdRange, void,
         (struct BattleEffectArgument * effect))
FAR_CALL(BattleFx_InitializeModeFar, BattleFx_InitializeMode, void, (s32 *arg))
FAR_CALL(BattleFx_DispatchModeFar, BattleFx_DispatchMode, void, (s32 *arg))
FAR_CALL(BattleFx_RunIceFleckBurstFar, BattleFx_RunIceFleckBurst, void,
         (struct BattleEffectArgument * effect))
FAR_CALL(BattleEffect_RunTileAndPaletteAnimationFar,
         BattleEffect_RunTileAndPaletteAnimation, void,
         (struct BattleEffectArgument * effect))
FAR_CALL(BattleFx_RunGatheringBurstFar, BattleFx_RunGatheringBurst, void,
         (struct BattleEffectArgument * effect))
FAR_CALL(BattlePresentation_PrepareSceneFar, BattlePresentation_PrepareScene,
         void, (s32 kind))
FAR_CALL(BattleFx_ScheduleCallbacksAndReleaseBlocksFar,
         BattleFx_ScheduleCallbacksAndReleaseBlocks, void, (void))
