/// @file
/// Battle state, unit motion and presentation services.

/// Debug: runs the battle test menu, which never returns; nothing in the
/// source calls it.
void DebugBattle_RunMenuFar(void);
/// Readies the sprites of the units in `list` and releases those of units it
/// lacks; with `refresh`, every listed unit's pose is set again afterwards.
void BattleObject_SpawnObjectsForListFar(s16 *list, s32 refresh);
/// Points the battle camera at (x, depth, y) with a zoom in percent and lines
/// the zoomed background up with it.
void BattleCamera_SetViewFar(s32 x, s32 depth, s32 y, s32 zoom);
/// Points the battle camera at the ground between two units, with a zoom in
/// percent, for a paired action or a beam.
void BattleCamera_SetViewAtPairMidpointFar(s32 a, s32 b, s32 zoom);
/// Switches the background's display mode and rebuilds its maps; a nonzero
/// `fade` also darkens or restores its palette.
void BattleBackground_SetModeFar(s32 mode, s32 value, s32 fade);
/// Loads a battle background's picture, palette and maps, its palettes darkened
/// by `level`, as a battle opens or the Atalanta and Boreas animations show
/// their violet sky.
void BattleBackground_LoadFar(s32 mode, s32 resource, s32 level);
/// Copies the background's saved palette back, darkened by `level` (0 for
/// none), for animations that dim the background.
void BattleBackground_SetPaletteLevelFar(s32 unused, s32 level);
/// Returns 1 for an enemy whose blow lands with SE_HEAVY_IMPACT instead of
/// SE_IMPACT, and 0 out of range; the beam animations ask it.
s32 BattleSprite_HasHeavyImpactFar(s32 index);
/// Returns the height above a unit where its animations play, in 16.16 pixels.
s32 BattleUnit_GetEffectHeightFar(s32 id);
/// Sends a unit leaping three quarters of the way to its target, arriving in
/// `travel_divisor` frames, as an attacker closes in before it strikes.
void BattleMotion_ApproachTargetFar(s32 actorId, s32 targetId,
                                    s32 travel_divisor, s32 initial_velocity_y);
/// Knocks a unit back by one enum BattleKnockback variant and puts it in
/// POSE_DAMAGE, as an animation's blow lands.
void BattleMotion_ApplyKnockbackFar(s32 id, s32 variant);
/// Lists the units still in the battle on the sides in `groups`, party first,
/// into `dst` if it's not null, and returns how many; HP isn't tested.
s32 BattleUnit_ListIdsFar(s32 groups, u16 *dst);
/// Returns a unit's battle object, or null for a unit with no place
/// (BattleObject_Get); the animations find the sprites they move through it.
struct BattleObject *GetBattleObjectSlotFar(s32 objectId);
/// Fights a battle against an enemy formation from its opening to its end, and
/// returns the enemies defeated, BATTLE_OUTCOME_FLED or BATTLE_OUTCOME_LOST.
s32 Battle_RunFar(s32 enemyGroup);
/// Projects a unit's position onto the screen, writing its pixel x and y and
/// its depth to `projected`; returns 0.
s32 BattleMotion_ProjectPositionFar(s32 id, s32 *projected);
/// Projects the point where a unit's animations play onto the screen, the
/// height scaled as the sprite is; returns 0.
s32 BattleMotion_ProjectScaledPositionFar(s32 id, s32 *projected);
/// Projects the point 24 pixels above a small enemy or 48 above another unit,
/// where the target cursor sits; returns 0.
s32 BattleMotion_ProjectConditionalPositionFar(s32 id, s32 *projected);
/// Lists the party members who fight, four or three in a link battle, into
/// `owners` if it's not null, marks them as the party's side and returns how
/// many.
s32 BattleParty_PrepareMembersFar(u16 *owners);
/// Sets the camera to the fixed view shown while commands are chosen, raised
/// by `pos` pixels.
void BattleCamera_SetCommandViewFar(s32 pos);
/// Returns one of an object's sprites, or null when it has no such sprite.
void *GetMotionRecordFar(struct Object *object, s32 record_index);
/// Highlights the units in `actors` by turning every other unit's sprites
/// semi-transparent, or stops the highlight with HIGHLIGHT_OFF.
void BattleHighlight_SetModesFar(u16 *actors, s32 mode);
/// Starts playback of the targets' results as an animation's blow lands, with
/// `cue` as the hit's sound (0 keeps the one set); see ANIM_SHOW_DAMAGE.
void BattlePlayback_StartFar(s32 cue);
/// Copies `count` colours with each channel scaled by `scale` (16.16, at most
/// 1), to darken a palette; returns 0.
s32 Graphics_ScaleRgb555ClampedFar(u16 *source, u16 *destination, s32 scale,
                                   s32 count);
/// Brings each recovering Djinni of a member with HP a turn nearer to being set
/// again, and returns 1 when any was; walking in the field ticks it too.
s32 DjinnRecovery_TickFar(void);
/// Returns the palette of a unit's battle object.
s32 BattleObject_GetPaletteFar(s32 id);
/// Sets every recovering Djinni of a member with HP at once, as the Battle
/// Arena and Vault's map do; returns 0.
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
void BattleMessage_WaitForAdvanceAtFar(s32 x, s32 y);
#else
/// Declared s32 as the original is, though it returns no value.
s32 BattleMessage_WaitForAdvanceAtFar(s32 x, s32 y);
#endif
/// Counts the standby Djinn of the units that fight on a unit's side, with each
/// element's count in `counts` if it's not null, for the Summon command.
s32 BattleDjinn_CountStandbyFar(u32 unitId, u8 *counts);
