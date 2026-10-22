/// @file
/// Battle-effect dispatch, drawing and presentation setup.

/// Writes a canvas drawing routine for one clip, mirroring and blend mode into
/// heap slot `slot`, for the star field and the colour effects of other banks.
s32 BattleFx_BuildBlitterFar(s32 slot, s32 width_shift, s32 height_shift,
                             s32 flags, u32 mode);
/// Runs an effect from the second range of kinds: the casting impacts, the beam
/// sequences, and the ring and spark scenes.
void BattleFx_DispatchByIdRangeFar(struct BattleEffectArgument *effect);
/// Runs one of the large effects, the summons' scenes, with its sheet, work and
/// canvas taken for it and freed after.
void BattleFx_RunSceneEffectFar(struct BattleEffectArgument *effect);
/// Runs an ordinary effect, the gBattleFx_EffectHandlers entry for its kind,
/// with its sheet, work and canvas taken for it and freed after.
void BattleFx_RunTableEffectFar(struct BattleEffectArgument *effect);
/// Stands a pillar of light in an element's colours at a unit's feet for 56
/// frames, with motes spiralling up around it.
void BattleFx_RunElementPillarFar(struct BattleEffectArgument *effect);
/// Shatters a white screen into falling pieces as a battle on the Venus aerie
/// storm backdrop opens, for 128 frames.
void BattleFx_RunShatteringOpeningFar(struct BattleEffectArgument *effect);
/// Flares sparks in an element's colours at the four corners around a unit for
/// 56 frames, then plays a burst of light over it.
void BattleFx_RunElementBurstFar(struct BattleEffectArgument *effect);
/// Starts the streaks in an element's colours and returns at once;
/// ElementGlow_Play ends them with BattleFx_EndElementStreaksFar.
void BattleFx_StartElementStreaksFar(s32 element);
/// Ends the element streaks, stopping their callbacks and freeing their work
/// and canvas.
void BattleFx_EndElementStreaksFar(void);
