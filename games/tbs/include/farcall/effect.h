/// @file
/// Battle-effect dispatch, drawing and presentation setup.

/// Writes a canvas drawing routine for one clip, mirroring and blend mode into
/// heap slot `slot`, for the star field and the colour effects of other banks.
s32 BattleAnim_BuildBlitterFar(s32 slot, s32 width_shift, s32 height_shift,
                               s32 flags, u32 mode);
/// Runs an effect from the second range of kinds: the casting impacts, the beam
/// sequences, and the ring and spark scenes.
void BattleAnim_DispatchByIdRangeFar(struct BattleAnimArgs *effect);
/// Runs one of the large effects, the summons' scenes, with its sheet, work and
/// canvas taken for it and freed after.
void BattleAnim_RunSummonEffectFar(struct BattleAnimArgs *effect);
/// Runs an ordinary effect, the gBattleAnim_Handlers entry for its
/// kind, with its sheet, work and canvas taken for it and freed after.
void BattleAnim_RunTableEffectFar(struct BattleAnimArgs *effect);
/// Stands a pillar of light in an element's colours at a unit's feet for 56
/// frames, with motes spiralling up around it.
void BattleAnim_RunElementPillarFar(struct BattleAnimArgs *effect);
/// Shatters a white screen into falling pieces as a battle on the Venus aerie
/// storm backdrop opens, for 128 frames.
void BattleAnim_RunShatteringOpeningFar(struct BattleAnimArgs *effect);
/// Flares sparks in an element's colours at the four corners around a unit for
/// 56 frames, then plays a burst of light over it.
void BattleAnim_RunElementBurstFar(struct BattleAnimArgs *effect);
/// Starts the streaks in an element's colours and returns at once;
/// ElementGlow_Play ends them with BattleAnim_EndElementStreaksFar.
void BattleAnim_StartElementStreaksFar(s32 element);
/// Ends the element streaks, stopping their callbacks and freeing their work
/// and canvas.
void BattleAnim_EndElementStreaksFar(void);
