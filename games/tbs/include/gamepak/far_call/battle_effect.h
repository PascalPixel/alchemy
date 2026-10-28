/// @file
/// Battle effect dispatch, drawing and presentation setup.

/// Writes a canvas drawing routine for one clip, mirroring and blend mode into
/// heap slot `slot`, for the star field and the colour effects of other banks.
s32 BattleEffect_BuildBlitterFar(s32 slot, s32 width_shift, s32 height_shift,
                                 s32 flags, u32 mode);
/// Runs an effect from the second range of kinds: the casting impacts, the
/// beams, and the ring and spark effects.
void BattleEffect_RunByIdRangeFar(struct BattleEffectParams *args);
/// Runs one of the large effects, the summons' effects, with its sheet,
/// work and canvas taken for it and freed after.
void BattleEffect_RunSummonFar(struct BattleEffectParams *args);
/// Runs an ordinary effect, the gBattleEffect_Handlers entry for its
/// kind, with its sheet, work and canvas taken for it and freed after.
void BattleEffect_RunFromTableFar(struct BattleEffectParams *args);
/// Stands a pillar of light in an element's colours at a unit's feet for 56
/// frames, with motes spiralling up around it.
void BattleEffect_RunElementPillarFar(struct BattleEffectParams *args);
/// Shatters a white screen into falling pieces as a battle on the Venus aerie
/// storm background opens, for 128 frames.
void BattleEffect_RunShatteringOpeningFar(struct BattleEffectParams *args);
/// Flares sparks in an element's colours at the four corners around a unit for
/// 56 frames, then plays a burst of light over it.
void BattleEffect_RunElementBurstFar(struct BattleEffectParams *args);
/// Starts the streaks in an element's colours and returns at once;
/// ElementGlow_Play ends them with BattleEffect_EndElementStreaksFar.
void BattleEffect_StartElementStreaksFar(s32 element);
/// Ends the element streaks, stopping their tasks and freeing their work
/// and canvas.
void BattleEffect_EndElementStreaksFar(void);
