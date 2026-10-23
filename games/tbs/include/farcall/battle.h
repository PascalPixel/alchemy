/// @file
/// Battle state, unit motion and presentation services.

/// Debug: runs the battle test menu, which never returns; nothing in the
/// source calls it.
void DebugBattle_RunMenuFar(void);
/// Readies the sprites of the units in `list` and releases those of units it
/// lacks; with `refresh`, every listed unit's pose is set again afterwards.
void BattleObject_SpawnObjectsForListFar(s16 *list, s32 refresh);
/// Points the battle camera at (x, depth, y) with a zoom in percent and lines
/// the background's perspective plane up with it.
void BattlePresentation_SetupTransitionViewFar(s32 x, s32 depth, s32 y,
                                               s32 zoom);
/// Points the battle camera at the ground between two units, with a zoom in
/// percent, for a paired action or a beam.
void BattlePresentation_SetupTransitionAtPairMidpointFar(s32 a, s32 b,
                                                         s32 zoom);
/// Switches the background's display mode and rebuilds its maps; a nonzero
/// `fade` also darkens or restores its palette.
void BattlePresentation_ConfigurePaletteFadeFar(s32 mode, s32 value, s32 fade);
/// Loads a battle background's picture, palette and maps, its palettes darkened
/// by `level`, as a battle opens or the Atalanta and Boreas effects show their
/// violet sky.
void BattleBackground_LoadFar(s32 mode, s32 resource, s32 level);
/// Copies the background's saved palette back, darkened by `level` (0 for
/// none), for effects that dim the background.
void BattlePresentation_SetPaletteLevelFar(s32 unused, s32 level);
/// Returns 1 for a class whose blow lands with SE_HEAVY_IMPACT instead of
/// SE_133, and 0 out of range; the beam effects ask it.
s32 BattleClass_HasHeavyImpactFar(s32 index);
/// Returns the height above a unit where its effects play, in 16.16 pixels.
s32 BattleUnit_GetEffectHeightFar(s32 id);
/// Sends a unit leaping three quarters of the way to its target, arriving in
/// `travel_divisor` frames, as an attacker closes in before it strikes.
void BattleMotion_ApproachTargetFar(s32 actorId, s32 targetId,
                                    s32 travel_divisor, s32 initial_velocity_y);
/// Knocks a unit back by one enum BattleKnockback variant and puts it in
/// POSE_DAMAGE, as an effect's blow lands.
void BattleMotion_ApplyVariantMotionFar(s32 id, s32 variant);
/// Lists the units still in the battle on the sides in `groups`, party first,
/// into `dst` if it's not null, and returns how many; HP isn't tested.
s32 BattleParty_ListActorIdsFar(s32 groups, u16 *dst);
/// Returns a unit's battle object, or null for a unit with no place
/// (BattleObject_Get); the effects find the sprites they move through it.
struct BattleObject *GetBattleObjectSlotFar(s32 objectId);
/// Fights a battle against an enemy formation from its opening to its end, and
/// returns the enemies defeated, ENCOUNTER_FLED or ENCOUNTER_PARTY_LOST.
s32 Battle_RunEncounterFar(s32 enemyGroup);
/// Projects a unit's position onto the screen, writing its pixel x and y and
/// its depth to `projected`; returns 0.
s32 BattleMotion_ProjectPositionFar(s32 id, s32 *projected);
/// Projects the point where a unit's effects play onto the screen, the height
/// scaled as the sprite is; returns 0.
s32 BattleMotion_ProjectScaledPositionFar(s32 id, s32 *projected);
/// Projects the point 24 pixels above a small enemy or 48 above another unit,
/// where the target cursor sits; returns 0.
s32 BattleMotion_ProjectConditionalPositionFar(s32 id, s32 *projected);
/// Lists the party members who fight, four or three in a link battle, into
/// `owners` if it's not null, marks them as the party's side and returns how
/// many.
s32 BattleParty_PrepareActiveOwnersFar(u16 *owners);
/// Sets the camera to the fixed view shown while commands are chosen, raised
/// by `pos` pixels.
void Camera_ConfigureViewFar(s32 pos);
/// Returns one sprite record of an object, or null when it has no such sprite.
void *GetMotionRecordFar(struct MotionObject *object, s32 record_index);
/// Highlights the units in `actors` by turning every other unit's sprites
/// semi-transparent, or stops the highlight with HIGHLIGHT_OFF.
void BattlePresentation_SetActorModesFar(u16 *actors, s32 mode);
/// Starts playback of the targets' results as an effect's blow lands, with
/// `cue` as the hit's sound (0 keeps the one set); see EFFECT_SHOW_DAMAGE.
void BattleEvent_StartPhaseFar(s32 cue);
/// Copies `count` colours with each channel scaled by `scale` (16.16, at most
/// 1), to darken a palette; returns 0.
s32 Graphics_ScaleRgb555ClampedFar(u16 *source, u16 *destination, s32 scale,
                                   s32 count);
/// Brings each recovering Djinni of a member with HP a turn nearer to being set
/// again, and returns 1 when any was; walking in the field ticks it too.
s32 DjinnRecovery_TickFar(void);
/// Returns the palette of a unit's battle object.
s32 BattleMotion_GetSlotPaletteFar(s32 id);
/// Sets every recovering Djinni of a member with HP at once, as the link lobby
/// and Vault's scene do; returns 0.
s32 DjinnRecovery_TickTwentyFar(void);
/// Debug: replaces the party with one of the preset parties, levels, Djinn,
/// abilities and items included; returns 1 past the table's end.
s32 DebugParty_LoadPresetFar(s32 preset);
/// Readies the sprite of every unit in the battle and sets its display mode and
/// motion afresh (BattleObject_Put), as after a call for help.
void BattleObject_PutFar(void);
#ifdef UBFIX
/// Shows the bobbing cursor at (x, y), in screen pixels, and waits for
/// KEY_ADVANCE; the battle command row waits on it.
void BattlePresentation_WaitForPromptAtFar(s32 x, s32 y);
#else
/// Declared s32 as the original is, though it returns no value.
s32 BattlePresentation_WaitForPromptAtFar(s32 x, s32 y);
#endif
/// Counts the standby Djinn of the units that fight on a unit's side, with each
/// element's count in `counts` if it's not null, for the Summon command.
s32 BattleParty_CountStandbyDjinnFar(u32 unitId, u8 *counts);
