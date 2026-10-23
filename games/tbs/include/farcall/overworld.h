/// @file
/// Map events, field Psynergy, objects and effect lifecycles.

/// Runs the field map the party entered by warp destination until it leaves,
/// and returns the warp kind for Map_TakeWarp, or WARP_NONE when gSaveBlock
/// already holds the next map.
s32 Map_RunFar(s32 warpDest);
/// The game's main loop, which never returns; mode, an enum GameRunMode,
/// picks the first map. System_Initialize starts it at power-on with
/// GAME_RUN_TITLE, and Game_Restart starts it again.
void Game_RunFar(s32 mode);
/// Waits the given number of frames, unless debug fast forward is on.
void FieldScript_WaitFramesFar(s32 frames);
/// Opens a script: closes any map name popup, stops the leader and resets the
/// message and timing state.
void FieldScript_StartFar(void);
/// Closes a script: gives the camera back to the leader and refreshes
/// FLAG_LURE_CAP.
void FieldScript_EndFar(void);
/// Spawns the object events of an object event template table after the last
/// object event in use, so a ship map can bring people in mid-scene.
void FieldScript_CallWithLastActiveObjectIdFar(
    struct ObjectEventTemplate *table);
/// Gives each unconditional object event template a condition that shows it
/// only while it lies inside the map's view bounds.
void Map_AssignViewFlagsFar(struct ObjectEventTemplate *entry);
/// Replaces the map's event table, the one the field checks as the leader
/// moves.
void FieldScript_SetMapEventsFar(const struct MapEvent *events);
/// Adds a character to the party; a nonzero value also shows
/// `<name> joined your party.`
void FieldScript_PrepareObjectAndApplyValueFar(s32 event_id, s32 value);
/// Adds two characters to the party and announces them in one message, as
/// Vale's map script does.
void FieldScript_PrepareTwoObjectsAndApplyFar(s32 first, s32 second);
/// Takes a character out of the party with HP and PP restored and poison
/// cured; if nobody left standing has HP, the leader keeps 1.
void Party_RemoveMemberRestoredFar(s32 owner);
/// Gives an item to the first member with room and says so, letting the
/// player drop something when every bag is full. Returns the member who got
/// it, or -1 when it was left behind.
s32 PartyInventory_GiveItemFar(s32 item, s32 unused);
/// Asks yes or no once the message on screen is read, with the speaker
/// nodding at yes and shaking its head at no. Returns 0 for yes, 1 for no.
s32 FieldScript_AskYesNoWithObjectEventAnimationFar(s32 localId,
                                                    s32 force_bottom);
/// Teaches a character a Psynergy ability with its fanfare and
/// `<unit> learned <ability>.`
void Party_LearnAbilityFar(s32 unit, s32 ability);
/// Returns the object event with this id, or NULL; the form of
/// ObjectEvent_GetByLocalId the other banks call.
struct ObjectEvent *ObjectEvent_GetByIdFar(s32 objectId);
/// Releases an object event and empties its slot in the field work's object
/// event table.
void ObjectEvent_DestroyByIdFar(s32 index);
/// Sets an object event's top speed and acceleration, in 16.16 pixels per
/// frame.
void ObjectMotion_SetSpeedParametersFar(u32 objectId, s32 speedLimit,
                                        s32 acceleration);
/// Starts a built-in action or a supplied script on an object event and makes
/// it face the way it moves (OBJ_EVENT_FACES_MOTION).
void ObjectMotion_EnableActionAndSetCallbackFar(u32 objectId, s32 action);
/// Waits, for at most 300 frames, until an object event's script reaches its
/// end.
void ObjectEvent_RefreshSelectorByIdFar(u32 objectId);
/// Stops an object event's action and puts it back on the idle script under
/// script control.
void ObjectMotion_EnableActionAndResetMotionFar(u32 objectId);
/// Starts an action or script on an object event, then waits up to 300 frames
/// for it to finish.
void ObjectEvent_SetActionCallbackAndRefreshByIdFar(u32 objectId, s32 action);
/// Sends an object event toward (x, z), in pixels, keeping its current
/// animation and unpausing it.
void ObjectMotion_ResetAndSetPositionFar(u32 objectId, s32 x, s32 z);
/// Sends an object event toward (x, z) and waits, at most 600 frames, for it to
/// get there.
void ObjectMotion_SetPositionAndCommitFar(u32 objectId, s32 x, s32 z);
/// Walks an object event toward (x, z), in pixels, in its walking animation.
void ObjectMotion_ResetAndSetPositionInMode2Far(u32 objectId, s32 x, s32 z);
/// Walks an object event to (x, z), waits for it to arrive and stands it still.
void ObjectMotion_SetPositionAndResetFar(u32 objectId, s32 x, s32 z);
/// Steps an object event sideways to the middle of its 16-pixel column, fixes
/// its sprite priority, then sends it z_offset pixels along z. Kalay's
/// interiors use it.
void ObjectMotion_CenterOnColumnFar(u32 objectId, s32 action, s32 z_offset);
/// Sends an object event toward a point offset in pixels from where it stands.
void ObjectMotion_OffsetPositionAndResetMotionFar(u32 objectId, s32 x_offset,
                                                  s32 z_offset);
/// Waits, at most 600 frames, for an object event to reach its destination,
/// then stands it still.
void ObjectMotion_CommitCurrentPositionAndActivateFar(u32 objectId);
/// Puts an object event at rest at (x, z), in 16.16 pixels; one that follows
/// the ground keeps the same height above it there.
void ObjectMotion_SetHorizontalPositionWithTerrainFar(u32 objectId, s32 x,
                                                      s32 z);
/// Sets an object event's animation: 1 stands, 2 walks, 3 nods, 4 shakes the
/// head.
void ObjectEvent_SetModeByIdFar(u32 objectId, s32 action);
/// Sets the animation timer steps of each of an object event's sprites, as the
/// Tolbi ship and Lamakan Desert maps do.
void ObjectEvent_SetActionByIdFar(u32 objectId, s32 action);
/// Plays an object event's animation and waits for it to end.
void ObjectMotion_SetAnimationAndWaitFar(u32 objectId, s32 action);
/// Waits, at most 90 frames, for an animated object event's animation to
/// change.
void ObjectMotion_WaitForAnimationChangeFar(u32 objectId);
/// Throws an object event upward at speed pixels per frame with a jump sound,
/// lets gravity bring it down, and waits event_id frames.
void ObjectMotion_LaunchFar(u32 objectId, s32 speed, s32 event_id);
/// Makes an object event squash and spring back one to three times, through
/// gObjectMotion_VariantScripts.
void ObjectMotion_SetVariantCallbackFar(u32 objectId, s32 variant);
/// Runs that squash-and-spring motion and waits for it to end.
void ObjectMotion_SetVarCbAndRefreshFar(u32 objectId, s32 variant);
/// Makes an object event hop down off the ledge in front of it and land 16
/// pixels south, playing the queued sound as it lands. The field runs it for
/// the leader at a ledge, and seven maps import it.
void ObjectEvent_JumpDownLedgeFar(s32 sequence_arg, s32 mode_or_frame,
                                  s32 optional_action);
/// Turns the first object event at once to face the second, then waits wait
/// frames.
void ObjectMotion_SetAngleTowardFar(s32 firstId, s32 second_id, s32 wait);
/// Turns two object events toward each other, then waits wait frames; talking
/// to someone turns the leader and them this way.
void ObjectEvent_LinkPairFar(s32 firstId, s32 second_id, s32 wait);
/// Gives every sprite part of an object event a param, or with bit 8 set makes
/// the object event blink until another param is given.
void ObjectEvent_ConfigureSpriteColorFar(s32 objectId, s32 value);
/// Sets the param byte of every animated part of an object event's sprite; maps
/// flash an object event by alternating 0 and 15.
void ObjectEvent_SetSpriteColorFar(struct ObjectEvent *objectEvent, s32 value);
/// Links an object event to a target and starts a script on it; unless
/// OBJ_EVENT_LINK_KEEP_MOTION is set it trails 40 pixels behind, fast enough to
/// keep up and unable to block the target.
void ObjectEvent_SetTargetAndCallbackFar(u32 objectId, s32 targetId,
                                         const void *callback);
/// Sets the number of the script's next message.
void FieldScript_SetMessageIdFar(s32 value);
/// Opens the script's current message in a speech window beside the speaker,
/// with its portrait, and moves on to the next message. Returns the window
/// once its text is typed out; the caller waits for it to be read.
struct Window *FieldMessage_ShowAtObjectFar(s32 arg, s32 unused);
/// Shows the current message at an object event and waits until it has been
/// read and its window has closed.
void FieldScript_ShowMessageAndWaitFar(s32 action, s32 flag);
/// Shows a message, then waits the given number of frames.
void FieldScript_ShowMessageThenWaitFar(s32 first, s32 second, s32 value);
/// Asks a question at an object event and shows the reply for yes or for no;
/// the script carries on past both replies. Returns 0 for yes, 1 for no.
s32 FieldScript_AskYesNoAndShowResponseFar(s32 objectId, s32 actionId);
/// Turns an object event toward an angle, 65536 to a full turn, then waits wait
/// frames.
void ObjectMotion_ArmCallbackFar(s32 objectId, s32 angle, s32 wait);
/// Two object events speak at once, each message and portrait at its own tile
/// position; waits for A, B, L or R, then closes both.
void FieldScript_ShowTwoMessagesAndWaitFar(s32 first, s32 first_x, s32 first_y,
                                           s32 first_arg, s32 first_extra,
                                           s32 second, s32 second_x,
                                           s32 second_y, s32 second_arg,
                                           s32 second_extra);
/// Gives the messages that follow the voice of an object event's sprite, at the
/// rate the message speed option picks; the top bit alone silences them.
void Message_SetVoiceFromObjectEventFar(s32 arg0);
/// Fixes the priority of an object event's sprite so it no longer follows its
/// depth.
void ObjectMotion_SetActionVariantFar(s32 objectId, s32 priority);
/// Shows an emote bubble over an object event, then waits wait frames; the
/// bubble rides the object event and pops after 46 frames whatever the wait.
void FieldEffect_SpawnLinkedFar(s32 id, s32 mode, s32 wait);
/// Attaches, removes or briefly shows the effect object on an object event, by
/// enum AttachedEffectFlag; the leader's step uses it on hazard ground.
void FieldEffect_ConfigureLinkedObjectFar(s32 id, s32 flags);
/// Makes the target object event's sprite show the source object event's
/// graphics; neither is checked for NULL. The Tolbi ship deck uses it.
void ObjectEvent_CopyGraphicsFar(u32 targetId, u32 sourceId);
/// Makes the camera follow an object event; unless flag is set it jumps there
/// at once and the map is redrawn around it.
void FieldCamera_FollowObjectEventFar(s32 id, s32 flag);
/// Sets the camera's top speed and acceleration, in 16.16 pixels per frame.
void FieldCamera_SetSpeedParametersFar(s32 first, s32 second);
/// Sends the camera to a point (-1 keeps an axis), kept inside the map so the
/// screen never shows past its edge; with use_setter 0 it jumps there.
void FieldCamera_MoveWithinMapBoundsFar(s32 requested_x, s32 requested_y,
                                        s32 requested_z, s32 use_setter);
/// Waits, at most 600 frames, for the camera to arrive, then 2 frames more.
void FieldCamera_WaitForArrivalFar(void);
/// Moves the camera to an object event's position, kept inside the map.
void ObjectEvent_PlaceCurrentWithinCameraBoundsFar(s32 arg0, s32 arg1);
/// Returns the camera object, the script's view centre.
struct ObjectEvent *FieldCamera_GetScriptObjectFar(void);
/// Sets the map rectangle the camera keeps the screen inside.
void MapGrid_SetViewBoundsFar(s32 minX, s32 minY, s32 maxX, s32 maxY);
/// Makes the script leave for a warp destination of a map; the title, the
/// Battle Arena and the debug menus use it as well as a few maps.
void FieldScript_SetDestinationFar(s32 first, s32 second);
/// Raises the field's warp request; 999 leaves for the map and warp destination
/// in gSaveBlock.
void FieldScript_SetWarpRequestFar(u16 value);
/// Sets up a battle with one formation of an encounter table, which starts
/// once the script that asked for it has returned.
void Field_RequestBattleFromEncounterTableFar(s32 zone_no, s32 slot_no);
/// Sets up the battle with a Djinni, its object event hopping first unless
/// DJINN_NO_HOP is set; on re-entering the map the Djinni joins or hides.
void Field_RequestDjinnBattleFar(s32 flag, s32 no);
/// Sets the map and warp destination the field goes to next.
void FieldScript_SetNextDestinationFar(s32 first, s32 second);
/// Sets where the party wakes up after losing a battle, ahead of a scripted
/// fight.
void FieldScript_SetDefeatDestinationFar(s32 first, s32 second);
/// Sets the destination and raises WARP_NONE, so the map loop leaves on its
/// next pass; only Tolbi's spring uses it.
void FieldScript_RequestDestinationFar(u16 first, u16 second);
/// Writes an object event's screen position in pixels, y at the top of its
/// sprite. Returns 0, or -1 when there is no such object event.
s32 ObjectEvent_GetScreenPositionFar(s32 objectId, s32 *position);
/// Returns the metatile behavior of the searchable cell 16 pixels ahead of the
/// leader, or 0 when there is nothing to search; the field menu's search
/// entry asks it.
s32 ObjectEvent_GetTriggerTileAheadOfCurrentFar(void);
/// Returns the sprite a character is drawn with, which depends on
/// FLAG_CHILDHOOD and FLAG_ISAAC_WEARS_TUNIC.
s32 Party_RemapCharacterIdByFlagsFar(s32 arg0);
/// Jumps the leader two cells forward when the map allows, chaining jumps.
/// Returns 0 after a jump, -1 when it can't.
s32 Field_TryJumpForwardFar(void);
/// Climbs the leader down onto a climbable wall, or drops it off one. Returns
/// 0, or -1 when there's nothing to climb.
s32 Field_ClimbDownFar(void);
/// Climbs the leader up onto a climbable wall, or over its top. Returns 0, or
/// -1 when there's nothing to climb.
s32 Field_ClimbUpFar(void);
/// Stops the leader where it stands at normal walking speed, in its standing
/// or climbing pose.
void Leader_StopMovingFar(void);
/// Puts 32 semi-transparent ground particles under the leader and queues
/// SE_170.
void Weather_StartRainFar(void);
/// Starts the storm's lightning, bursts that repeat every 600 frames over
/// Vale's storm maps.
void Weather_StartLightningFar(void);
/// Stops the lightning from flashing again.
void Weather_StopLightningFar(void);
/// One last burst of lightning, starting two frames from now.
void Weather_LastLightningFar(void);
/// Restarts the lightning, the next burst flashing 118 frames from now.
void Weather_ResumeLightningFar(void);
/// One burst of three palette flashes from one colour mode to another, as
/// Vale's eruption shows.
void Weather_StartLightningFlashFar(s32 from, s32 to);
/// Loads the sparkles' tiles and clears all 32.
void FieldEffect_InitializeSparklesFar(void);
/// Starts a window transition in a mode at value 0, for the caller to move;
/// Reveal and three maps use it.
void ScreenTransition_InitializeBattleEffectStateFar(s32 mode);
/// Sets the colours the palette fade moves toward.
void PaletteFade_SetTargetFar(s32 mode, s32 half);
/// Sets the colours the palette fade shows now.
void PaletteFade_SetSourceFar(s32 mode, s32 half);
/// Moves the shown colours to the target colours over the given number of
/// frames.
void PaletteFade_StartInterpolationFar(s32 frames);
/// Starts the script's screen transition.
void FieldScript_StartScreenTransitionFar(void);
/// Finishes the script's screen transition.
void FieldScript_FinishScreenTransitionFar(void);
/// Waits for as long as the screen transition runs.
void FieldScript_WaitTransitionFramesFar(void);
/// Starts a wave in each axis of the background's scroll, changed line by
/// line in the horizontal blank; Lamakan Desert starts it everywhere but its
/// cave.
void ScanlineEffect_InitWaveFar(s32 mode, s32 freqX, s32 stepX, s32 ampX,
                                s32 freqY, s32 stepY, s32 ampY);
/// Stops the scanline effect: removes its two callbacks and stops DMA 0.
void ScanlineEffect_StopFar(void);
/// Has the target object event present an item: SE_124, animation 4, then the
/// item's picture with sparkles. Returns the picture, or 0 when either is
/// missing.
s32 FieldEffect_PlayCueAndStartEmitterOnTargetFar(s32 effect, s32 target,
                                                  s32 mode);
/// Shows a found item's picture, with sparkles or raised above the leader as
/// flags ask, then returns the leader to standing.
void ItemDisplay_ShowPictureFar(void *object, s32 flags);
/// Holds an item's picture above the leader for 80 frames, the way a found
/// item is shown in most maps.
void FieldEffect_StartEffectObject22Far(s32 value, s32 flags);
/// Moves a treasure spot's object event to (x, z) on the ground, in 16.16
/// pixels; -1 keeps the centre of its cell on that axis.
void TreasureSpot_SetObjectEventPositionFar(s32 id, s32 x, s32 y);
/// Moves a leader standing within 16 pixels of a treasure spot cell's centre 20
/// pixels away from it.
void TreasureSpot_MoveLeaderAwayFar(void);
/// On the world map, freezes the perspective view and the camera while an
/// script or menu runs; elsewhere it does nothing.
void WorldMap_StartScriptFar(void);
/// Undoes WorldMap_StartScript, reloading the area's graphics.
void WorldMap_EndScriptFar(void);
/// Returns the object event in a slot, or NULL; map overlays look object events
/// up through it.
void *ObjectEvent_GetByLocalIdFar(u32 index);
/// Starts the fade overlay on an object (the leader when NULL), which lasts
/// until the next map change.
void FieldEffect_StartFadeOverlayFar(void *object);
/// Runs one step of the current Psynergy effect, an enum PsynergyEffectStep;
/// Goma Cave's entrance plays Whirlwind on the stump's ivy with it.
void Field_DispatchTypeHandlerFar(s32 kind);
/// Prepares a Psynergy effect's work and spark tiles for an ability; while an
/// effect resolves it only switches the ability.
void FieldEffect_LoadActionEffectResourcesFar(s32 abilityId, s32 mode);
/// Records an effect's caster and target; with no target the effect lands 16
/// pixels in front of the caster.
void FieldEffect_SetupObjectPairFar(s32 user_id, s32 targetId);
/// Stops the caster's per-frame hook, restores its colours and waits a frame.
void FieldEffect_StopCasterHookFar(void);
/// Ends a Psynergy effect, releasing its sprites, tiles and work and playing
/// the queued sound.
void FieldEffect_EndFar(void);
/// Puts the caster in its casting pose, with the flash, sound and ability
/// name the effect calls for.
void FieldEffect_StartCastFar(void);
/// Clears the field's busy flag and, while Reveal is active, ends it.
void Field_EndBusyAndRevealFar(void);
/// Starts Mind Read's wave of shifting colour, with x the caster and y the
/// target.
void FieldEffect_StartMindReadWaveFar(s32 x, s32 y);
/// Ends Mind Read's wave, restoring the caster's colours and the palettes.
void FieldEffect_FinishWaveAndReleaseHeapBlockFar(void);
/// Stops BG0's scanline scroll and restores BG palette 15.
void Ui_SetBank15PaletteAndClearRenderModeFar(void);
/// Returns whether the map has a Psynergy event of this group for what the
/// leader faces; nothing in ☀️ calls it.
s32 FieldEffect_HasMatchingEvent5Far(s32 group);
/// Returns whether the map has an event for this item and what the leader
/// faces; the item menu then closes and the item is used on the field.
u32 Map_HasPsynergyEventFar(u16 item);
/// Makes a sprite share another's VRAM block, so a chain of afterimages draws
/// from one copy. Returns the sprite whose block is shared from now on, or
/// NULL for no source.
struct Sprite *Sprite_ShareVramCacheEntryFar(struct Sprite *sourceSprite,
                                             struct Sprite *sharedSprite);
/// Returns the battle background chosen for the current map, which the
/// battle setup reads.
s16 FieldScript_GetSpecialValueFar(void);
/// Gives each party member PP; 999 restores it all, as a Psynergy Stone does.
void Party_AddPpFar(s32 amount, s32 unused);
/// Asks yes or no at the top of the screen, or the bottom when value is set.
/// Returns 0 for yes, 1 for no.
s32 FieldYesNo_AskFar(s32 value);
/// Starts the current map's field music.
void Map_PlayMusicFar(void);
/// A Djinni joins the member with the fewest Djinn, after its object event in
/// slot plays its element's capture effect (none for slot -1).
void Djinn_JoinPartyFar(s32 slot, s32 element, s32 djinn);
/// Updates one field effect sprite for a frame: its age, callback, motion and
/// sprite.
void FieldEffectSprite_UpdateFar(struct FieldEffectSprite *effect);
/// Returns 1 while a field effect sprite is still on its way to its target, 0
/// once it arrives or when it never stops.
u32 FieldEffectSprite_HasPendingTargetFar(struct FieldEffectSprite *effect);
/// Places a field effect sprite at (x, z), at rest with no target.
void FieldEffectSprite_SetPositionFar(struct FieldEffectSprite *effect, s32 x,
                                      s32 z);
/// Sets the animation of a field effect sprite's sprite.
void FieldEffectSprite_SetObjectModeFar(struct FieldEffectSprite *effect,
                                        s32 mode);
/// Gives a field effect sprite its per-frame callback, starting at state 0.
void FieldEffectSprite_SetCallbackFar(struct FieldEffectSprite *effect,
                                      EffectCallback callback);
/// Fills a field effect sprite with a new sprite of a kind at (x, z), which it
/// owns until it is cleared.
void FieldEffectSprite_InitializeFar(struct FieldEffectSprite *effect, s32 kind,
                                     s32 x, s32 z);
/// Frees a field effect sprite and its sprite; a callback ends by calling it on
/// its own field effect sprite.
void FieldEffect_ClearOwnedSlotFar(struct FieldEffectSprite *effect);
/// Converts a world position in 16.16 pixels to screen x and y, in place,
/// using the world map's perspective there.
void Camera_WorldToScreenFar(struct EffectVector *position);
/// Returns the blip sound that voices a sprite's messages, or 0 for none or
/// with the Speech option off.
s32 Speaker_GetVoiceFar(u32 id);
/// Clears each button shortcut whose Psynergy the party can no longer cast.
void FieldScript_ClearInvalidPackedValuesFar(void);
/// Allocates and clears the field effect sprites and starts their update; the
/// Djinn joining effects and Altmiller Cave use them.
void FieldEffect_InitializeSlotsFar(void);
/// Frees every field effect sprite and its work, then waits a frame.
void FieldEffect_ClearActiveSlotsAndScheduleUpdatesFar(void);
/// Places an object event on the leader and walks it out to an offset, then
/// gives it an action.
void ObjectMotion_LaunchFromFocusedObjectFar(u32 arg0, s32 arg1, s32 arg2,
                                             s32 arg3);
/// Walks an object event a given offset in pixels from where it stands.
void ObjectMotion_OffsetPositionAndResetFar(u32 objectId, s32 x_offset,
                                            s32 z_offset);
/// Walks an object event an offset in pixels, waits for it to arrive and stands
/// it still.
void ObjectMotion_CommitPositionAndActivateFar(u32 objectId, s32 x_offset,
                                               s32 z_offset);
/// Makes an object event follow another through the linked action script.
void ObjectEvent_LinkObjectAndSetCallbackFar(u32 objectId,
                                             u32 linked_object_id);
/// Pauses an object, keeping its update and pose in gSaveBlock for later;
/// Halt and the Colosso operators use it.
void FieldEffect_PauseObjectFar(s32 id);
/// Captures up to 32 object events into gObjectEventSnapshot, for
/// ObjectEvent_Restore (ObjectEvent_Snapshot); the save menu calls it.
void Save_CaptureObjectEventsFar(void);
/// Puts back the object events ObjectEvent_Snapshot saved and moves the camera
/// to the leader's height.
void ObjectEvent_RestoreFar(void);
/// Starts a zoom to a 16.16 factor over the given frames, on the world map.
void FieldEffect_ScheduleRatioTransitionFar(s32 zoom, s32 frames);
/// Waits, at most 300 frames, for the world map's zoom to end.
void FieldScript_WaitForZoomTransitionFar(void);
/// Returns the area name of a map and warp destination, an offset from
/// MsgDebugEntryName, or 0 when no rule matches.
s32 MapSec_GetNameIndexFar(s32 mapId, s32 warpDest);
/// Starts an object event's action script: an enum MovementType, or the script
/// itself.
void ObjectEvent_SetMovementTypeFar(struct Object *object, s32 kind);
/// Plays SE_STOP_SLOT5 and then sound_id (SONG_FADE_OUT_ACTION_SE for -1), and
/// remembers sound_id to play again later.
void Map_QueueMusicFar(s32 sound_id);
/// Plays the sound Map_QueueMusic remembered, if any.
void Map_PlayQueuedMusicFar(void);
/// Returns a character's sprite for a save file's summary, which records only
/// whether it was childhood.
s32 Party_RemapCharacterIdIfEnabledFar(s32 arg0, s32 arg1);
#if TBS_PATCH_2
/// While Reveal is active, destroys its sparkles and leaves its marks; the
/// field menu calls it after each choice.
void FieldEffect_ClearRandomParticlesFar(void);
#endif
