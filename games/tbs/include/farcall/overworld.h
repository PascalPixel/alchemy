/// @file
/// Triggers, field Psynergy, actors and effect lifecycles.

/// Runs the scene the party entered by entrance until it leaves,
/// and returns the exit kind for Scene_TakeExit, or EXIT_KEEP_DESTINATION when
/// gSequenceParentWork already holds the next scene.
s32 SceneSequence_RunFar(s32 entrance);
/// The sequence parent, which never returns; mode, an enum GameStart,
/// picks the first scene. System_Initialize starts it at power-on with
/// GAME_START_TITLE, and Game_RestartQuest starts it again.
void SequenceParent_RunFar(s32 mode);
/// Waits the given number of frames, unless debug fast forward is on.
void Event_WaitFar(s32 frames);
/// Opens an event: closes any area name window, stops the leader and resets the
/// message and timing state.
void Event_StartFar(void);
/// Closes an event: gives the camera back to the leader and refreshes
/// FLAG_LURE_CAP.
void Event_EndFar(void);
/// Spawns the actors of an actor placement table after the last
/// actor in use, so a ship scene can bring people in mid-event.
void Event_CallWithLastActiveActorIdFar(struct ActorPlacement *table);
/// Gives each unconditional actor placement a condition that shows it
/// only while it lies inside the map's view bounds.
void Scene_AssignViewFlagsFar(struct ActorPlacement *entry);
/// Replaces the scene's trigger table, the one the field checks as the leader
/// moves.
void Event_SetTriggersFar(const struct Trigger *triggers);
/// Adds a character to the party; a nonzero value also shows
/// `<name> joined your party.`
void Event_PrepareActorAndApplyValueFar(s32 event_id, s32 value);
/// Adds two characters to the party and announces them in one message, as
/// Vale's event does.
void Event_PrepareTwoActorsAndApplyFar(s32 first, s32 second);
/// Takes a character out of the party with HP and PP restored and poison
/// cured; if nobody left standing has HP, the leader keeps 1.
void Party_RemoveMemberRestoredFar(s32 owner);
/// Gives an item to the first party member with room and says so, letting the
/// player drop something when every bag is full. Returns the party member who
/// got it, or -1 when it was left behind.
s32 PartyInventory_GiveItemFar(s32 item, s32 unused);
/// Asks yes or no once the message on screen is read, with the speaker
/// nodding at yes and shaking its head at no. Returns 0 for yes, 1 for no.
s32 Event_AskYesNoWithActorAnimationFar(s32 actorId, s32 force_bottom);
/// Teaches a character a Psynergy ability with its jingle and
/// `<unit> learned <ability>.`
void Party_LearnAbilityFar(s32 unit, s32 ability);
/// Returns the actor with this id, or NULL; the form of
/// Actor_Lookup the other banks call.
struct Actor *Actor_GetByIdFar(s32 actorId);
/// Releases an actor and empties its slot in the field work's
/// actor table.
void Actor_ReleaseByIdFar(s32 index);
/// Sets an actor's top speed and acceleration, in 16.16 pixels per
/// frame.
void Actor_SetSpeedParametersFar(u32 actorId, s32 speedLimit, s32 acceleration);
/// Starts a movement type or a supplied actor script on an actor and makes
/// it face the way it moves (ACTOR_FACES_MOTION).
void Actor_SetMovementTypeByIdFar(u32 actorId, s32 action);
/// Waits, for at most 300 frames, until an actor's script reaches its
/// end.
void Actor_RefreshSelectorByIdFar(u32 actorId);
/// Stops an actor's movement type and puts it back on the idle script under
/// script control.
void Actor_StopFar(u32 actorId);
/// Starts a movement type or actor script on an actor, then waits up to 300
/// frames for it to finish.
void Actor_SetMovementTypeAndWaitByIdFar(u32 actorId, s32 action);
/// Sends an actor toward (x, z), in pixels, keeping its current
/// animation and unpausing it.
void Actor_ResetAndSetPositionFar(u32 actorId, s32 x, s32 z);
/// Sends an actor toward (x, z) and waits, at most 600 frames, for it to
/// get there.
void Actor_SetPositionAndCommitFar(u32 actorId, s32 x, s32 z);
/// Walks an actor toward (x, z), in pixels, in its walking animation.
void Actor_ResetAndSetPositionInMode2Far(u32 actorId, s32 x, s32 z);
/// Walks an actor to (x, z), waits for it to arrive and stands it still.
void Actor_SetPositionAndResetFar(u32 actorId, s32 x, s32 z);
/// Steps an actor sideways to the middle of its 16-pixel column, fixes
/// its sprite priority, then sends it z_offset pixels along z. Kalay's
/// interiors use it.
void Actor_CenterOnColumnFar(u32 actorId, s32 action, s32 z_offset);
/// Sends an actor toward a point offset in pixels from where it stands.
void Actor_OffsetPositionAndResetMotionFar(u32 actorId, s32 x_offset,
                                           s32 z_offset);
/// Waits, at most 600 frames, for an actor to reach its destination,
/// then stands it still.
void Actor_WaitForMoveByIdFar(u32 actorId);
/// Puts an actor at rest at (x, z), in 16.16 pixels; one that follows
/// the ground keeps the same height above it there.
void Actor_SetHorizontalPositionWithTerrainFar(u32 actorId, s32 x, s32 z);
/// Sets an actor's animation: 1 stands, 2 walks, 3 nods, 4 shakes the
/// head.
void Actor_SetModeByIdFar(u32 actorId, s32 action);
/// Sets the animation timer steps of each of an actor's sprites, as the
/// Tolbi ship and Lamakan Desert scenes do.
void Actor_SetAnimationSpeedByIdFar(u32 actorId, s32 action);
/// Plays an actor's animation and waits for it to end.
void Actor_SetAnimationAndWaitFar(u32 actorId, s32 action);
/// Waits, at most 90 frames, for an animated actor's animation to
/// change.
void Actor_WaitForAnimationChangeFar(u32 actorId);
/// Throws an actor upward at speed pixels per frame with a jump sound,
/// lets gravity bring it down, and waits event_id frames.
void Actor_JumpFar(u32 actorId, s32 speed, s32 event_id);
/// Makes an actor squash and spring back one to three times, through
/// gActor_VariantScripts.
void Actor_StartRepeatedMotionFar(u32 actorId, s32 variant);
/// Runs that squash-and-spring motion and waits for it to end.
void Actor_RunRepeatedMotionFar(u32 actorId, s32 variant);
/// Makes an actor hop down off the ledge in front of it and land 16
/// pixels south, playing the queued sound as it lands. The field runs it for
/// the leader at a ledge, and seven scenes import it.
void Actor_JumpDownLedgeFar(s32 actorId, s32 pose, s32 param);
/// Turns the first actor at once to face the second, then waits the given
/// number of frames.
void Actor_SetAngleTowardFar(s32 firstId, s32 second_id, s32 frames);
/// Turns two actors toward each other, then waits the given number of frames;
/// talking to someone turns the leader and them this way.
void Actor_TurnToFaceEachOtherFar(s32 firstId, s32 secondId, s32 frames);
/// Gives every sprite part of an actor a param, or with bit 8 set makes
/// the actor blink until another param is given.
void Actor_ConfigureSpriteColorFar(s32 actorId, s32 value);
/// Sets the param byte of every animated part of an actor's sprite; scenes
/// flash an actor by alternating 0 and 15.
void Actor_SetSpriteColorFar(struct Actor *actor, s32 value);
/// Makes an actor watch a target and starts a script on it; unless
/// ACTOR_WATCH_KEEP_MOTION is set it trails 40 pixels behind, fast enough to
/// keep up and unable to block the target.
void Actor_SetTargetAndCallbackFar(u32 actorId, s32 targetId,
                                   const void *callback);
/// Sets the number of the event's next message.
void Event_SetMessageIdFar(s32 value);
/// Opens the event's current message in a speech window beside the speaker,
/// with its portrait, and moves on to the next message. Returns the window
/// once its text is typed out; the caller waits for it to be read.
struct Window *FieldMessage_ShowAtActorFar(s32 arg, s32 unused);
/// Shows the current message at an actor and waits until it has been
/// read and its window has closed.
void Event_ShowMessageAndWaitFar(s32 action, s32 flag);
/// Shows a message, then waits the given number of frames.
void Event_ShowMessageThenWaitFar(s32 first, s32 second, s32 value);
/// Asks a question at an actor and shows the reply for yes or for no;
/// the event carries on past both replies. Returns 0 for yes, 1 for no.
s32 Event_AskYesNoAndShowResponseFar(s32 actorId, s32 flags);
/// Turns an actor toward an angle, 65536 to a full turn, then waits the given
/// number of frames.
void Actor_ArmCallbackFar(s32 actorId, s32 angle, s32 frames);
/// Two actors speak at once, each message and portrait at its own tile
/// position; waits for A, B, L or R, then closes both.
void Event_ShowTwoMessagesAndWaitFar(s32 first, s32 first_x, s32 first_y,
                                     s32 first_arg, s32 first_extra, s32 second,
                                     s32 second_x, s32 second_y, s32 second_arg,
                                     s32 second_extra);
/// Gives the messages that follow the voice of an actor's sprite, at the
/// rate the message speed option picks; the top bit alone silences them.
void Message_SetVoiceFromActorFar(s32 actorId);
/// Fixes the priority of an actor's sprite so it no longer follows its
/// depth.
void Actor_SetSpritePriorityFar(s32 actorId, s32 priority);
/// Shows an emote bubble over an actor, then waits the given number of frames;
/// the bubble rides the actor and pops after 46 frames however long it waits.
void FieldEffect_SpawnAttachedFar(s32 id, s32 mode, s32 frames);
/// Attaches, removes or briefly shows an attached effect on an actor, by
/// enum AttachedEffectFlag; the leader's walking update uses it on hazard
/// ground.
void FieldEffect_SetAttachedFar(s32 id, s32 flags);
/// Makes the target actor's sprite show the source actor's
/// graphics; neither is checked for NULL. The Tolbi ship deck uses it.
void Actor_CopyGraphicsFar(u32 targetId, u32 sourceId);
/// Makes the camera follow an actor; unless flag is set it jumps there
/// at once and the map is redrawn around it.
void FieldCamera_FollowActorFar(s32 id, s32 flag);
/// Sets the camera's top speed and acceleration, in 16.16 pixels per frame.
void FieldCamera_SetSpeedParametersFar(s32 first, s32 second);
/// Sends the camera to a point (-1 keeps an axis), kept inside the map so the
/// screen never shows past its edge; with use_setter 0 it jumps there.
void FieldCamera_MoveWithinMapBoundsFar(s32 requested_x, s32 requested_y,
                                        s32 requested_z, s32 use_setter);
/// Waits, at most 600 frames, for the camera to arrive, then 2 frames more.
void FieldCamera_WaitForArrivalFar(void);
/// Moves the camera to an actor's position, kept inside the map.
void Actor_PlaceCurrentWithinCameraBoundsFar(s32 actorId, s32 glide);
/// Returns the camera actor, the event's view centre.
struct Actor *FieldCamera_GetActorFar(void);
/// Sets the map rectangle the camera keeps the screen inside.
void MapGrid_SetCameraBoundsFar(s32 minX, s32 minY, s32 maxX, s32 maxY);
/// Makes the event leave for an entrance of a scene; the Title, the
/// Battle Arena and the debug menus use it as well as a few scenes.
void Event_SetDestinationFar(s32 first, s32 second);
/// Raises the field's exit request; 999 leaves for the scene and entrance
/// in gSequenceParentWork.
void Event_SetExitRequestFar(u16 value);
/// Sets up a battle with one formation of an encounter table, which starts
/// once the event that asked for it has returned.
void Field_RequestBattleFromEncounterTableFar(s32 zone_no, s32 slot_no);
/// Sets up the battle with a Djinni, its actor hopping first unless
/// DJINN_NO_HOP is set; on re-entering the scene the Djinni joins or hides.
void Field_RequestDjinnBattleFar(s32 flag, s32 no);
/// Sets the scene and entrance the field goes to next.
void Event_SetAfterBattleDestinationFar(s32 first, s32 second);
/// Sets where the party wakes up after losing a battle, ahead of a fight
/// an event starts.
void Event_SetDefeatDestinationFar(s32 first, s32 second);
/// Sets the destination and raises EXIT_KEEP_DESTINATION, so the scene sequence
/// leaves on its next pass; only Tolbi's spring uses it.
void Event_LeaveForFar(u16 first, u16 second);
/// Writes an actor's screen position in pixels, y at the top of its
/// sprite. Returns 0, or -1 when there is no such actor.
s32 Actor_GetScreenPositionFar(s32 actorId, s32 *position);
/// Returns the cell behavior of the searchable cell 16 pixels ahead of the
/// leader, or 0 when there is nothing to search; the field menu's search
/// entry asks it.
s32 Actor_GetTriggerCellAheadOfCurrentFar(void);
/// Returns the sprite a character is drawn with, which depends on
/// FLAG_CHILDHOOD and FLAG_ISAAC_WEARS_TUNIC.
s32 Party_RemapCharacterIdByFlagsFar(s32 character);
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
/// SE_AMBIENT_WATER.
void Weather_StartRainFar(void);
/// Starts the storm's lightning, bursts that repeat every 600 frames over
/// Vale's storm scenes.
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
/// Reveal and three scenes use it.
void ScreenTransition_InitializeBattleEffectStateFar(s32 mode);
/// Sets the colours the palette fade moves toward.
void PaletteFade_SetTargetFar(s32 mode, s32 half);
/// Sets the colours the palette fade shows now.
void PaletteFade_SetSourceFar(s32 mode, s32 half);
/// Moves the shown colours to the target colours over the given number of
/// frames.
void PaletteFade_StartInterpolationFar(s32 frames);
/// Starts the event's screen transition.
void Event_StartScreenTransitionFar(void);
/// Finishes the event's screen transition.
void Event_FinishScreenTransitionFar(void);
/// Waits for as long as the screen transition runs.
void Event_WaitTransitionFramesFar(void);
/// Starts a wave in each axis of the background's scroll, changed line by
/// line in the horizontal blank; Lamakan Desert starts it everywhere but its
/// cave.
void LineScroll_InitWaveFar(s32 mode, s32 freqX, s32 stepX, s32 ampX, s32 freqY,
                            s32 stepY, s32 ampY);
/// Stops the line scroll: removes its two tasks and stops DMA 0.
void LineScroll_StopFar(void);
/// Has the target actor present an item: SE_CHEST_OPEN, animation 4,
/// then the item's picture with sparkles. Returns the picture, or 0 when either
/// is missing.
s32 FieldEffect_PlayCueAndStartEmitterOnTargetFar(s32 effect, s32 target,
                                                  s32 mode);
/// Shows a found item's picture, with sparkles or raised above the leader as
/// flags ask, then returns the leader to standing.
void ItemDisplay_ShowPictureFar(void *object, s32 flags);
/// Holds an item's picture above the leader for 80 frames, the way a found
/// item is shown in most scenes.
void ItemDisplay_ShowItemFar(s32 value, s32 flags);
/// Moves a treasure spot's actor to (x, z) on the ground, in 16.16
/// pixels; -1 keeps the centre of its cell on that axis.
void TreasureSpot_SetActorPositionFar(s32 id, s32 x, s32 y);
/// Moves a leader standing within 16 pixels of a treasure spot cell's centre 20
/// pixels away from it.
void TreasureSpot_MoveLeaderAwayFar(void);
/// On the World Map, freezes the perspective view and the camera while an
/// event or menu runs; elsewhere it does nothing.
void WorldMap_FreezeFar(void);
/// Undoes WorldMap_Freeze, reloading the area's graphics.
void WorldMap_UnfreezeFar(void);
/// Returns the actor in a slot, or NULL; scene overlays look actors
/// up through it.
void *Actor_LookupFar(u32 index);
/// Starts the fade overlay on an actor (the leader when NULL), which lasts
/// until the next scene change.
void FieldEffect_StartFadeOverlayFar(void *object);
/// Runs one step of the current field Psynergy, an enum FieldPsynergyHandler;
/// Goma Cave's entrance plays Whirlwind on the stump's ivy with it.
void FieldPsynergy_RunHandlerFar(s32 kind);
/// Prepares a field Psynergy's work and spark tiles for an ability; while
/// one resolves it only switches the ability.
void FieldPsynergy_LoadResourcesFar(s32 abilityId, s32 mode);
/// Records a field Psynergy's caster and target; with no target it lands 16
/// pixels in front of the caster.
void FieldPsynergy_SetupActorPairFar(s32 user_id, s32 targetId);
/// Stops the caster's update routine, restores its colours and waits a frame.
void FieldPsynergy_StopCasterUpdateFar(void);
/// Ends a field Psynergy, releasing its sprites, tiles and work and playing
/// the queued sound.
void FieldPsynergy_EndFar(void);
/// Puts the caster in its casting pose, with the flash, sound and ability
/// name the Psynergy calls for.
void FieldPsynergy_StartCastFar(void);
/// Clears the field's busy flag and, while Reveal is active, ends it.
void Field_EndBusyAndRevealFar(void);
/// Starts Mind Read's wave of shifting colour, with x the caster and y the
/// target.
void FieldPsynergy_StartMindReadWaveFar(s32 x, s32 y);
/// Ends Mind Read's wave, restoring the caster's colours and the palettes.
void FieldPsynergy_FinishWaveAndReleaseHeapBlockFar(void);
/// Stops BG0's line scroll and restores BG palette 15.
void FieldPsynergy_RestoreMindReadPaletteFar(void);
/// Returns whether the scene has a Psynergy trigger of this group for what the
/// leader faces; nothing in ☀️ calls it.
s32 Scene_HasFieldPsynergyTriggerFar(s32 group);
/// Returns whether the scene has an event for this item and what the leader
/// faces; the item menu then closes and the item is used on the field.
u32 Scene_HasPsynergyTriggerFar(u16 item);
/// Makes a sprite share another's VRAM block, so a chain of afterimages draws
/// from one copy. Returns the sprite whose block is shared from now on, or
/// NULL for no source.
struct Sprite *Sprite_ShareVramCacheEntryFar(struct Sprite *sourceSprite,
                                             struct Sprite *sharedSprite);
/// Returns the battle background chosen for the current scene, which the
/// battle setup reads.
s16 Event_GetSpecialValueFar(void);
/// Gives each party member PP; 999 restores it all, as a Psynergy Stone does.
void Party_AddPpFar(s32 amount, s32 unused);
/// Asks yes or no at the top of the screen, or the bottom when value is set.
/// Returns 0 for yes, 1 for no.
s32 FieldYesNo_AskFar(s32 value);
/// Starts the current scene's field music.
void Scene_PlayMusicFar(void);
/// A Djinni joins the party member with the fewest Djinn, after its actor in
/// slot plays its element's capture effect (none for slot -1).
void Djinn_JoinPartyFar(s32 slot, s32 element, s32 djinn);
/// Updates one field effect sprite for a frame: its age, callback, motion and
/// sprite.
void FieldParticle_UpdateFar(struct FieldParticle *effect);
/// Returns 1 while a field effect sprite is still on its way to its target, 0
/// once it arrives or when it never stops.
u32 FieldParticle_HasPendingTargetFar(struct FieldParticle *effect);
/// Places a field effect sprite at (x, z), at rest with no target.
void FieldParticle_SetPositionFar(struct FieldParticle *effect, s32 x, s32 z);
/// Sets the animation of a field effect sprite's sprite.
void FieldParticle_SetActorModeFar(struct FieldParticle *effect, s32 mode);
/// Gives a field effect sprite its per-frame callback, starting at state 0.
void FieldParticle_SetCallbackFar(struct FieldParticle *effect,
                                  EffectCallback callback);
/// Fills a field effect sprite with a new sprite of a kind at (x, z), which it
/// owns until it is cleared.
void FieldParticle_InitializeFar(struct FieldParticle *effect, s32 kind, s32 x,
                                 s32 z);
/// Frees a field effect sprite and its sprite; a callback ends by calling it on
/// its own field effect sprite.
void FieldEffect_ClearOwnedSlotFar(struct FieldParticle *effect);
/// Converts a world position in 16.16 pixels to screen x and y, in place,
/// using the World Map's perspective there.
void Camera_WorldToScreenFar(struct Vector3 *position);
/// Returns the blip sound that voices a sprite's messages, or 0 for none or
/// with the Speech option off.
s32 Speaker_GetVoiceFar(u32 id);
/// Clears each button shortcut whose Psynergy the party can no longer cast.
void Event_ClearInvalidPackedValuesFar(void);
/// Allocates and clears the field effect sprites and starts their update; the
/// Djinn joining effects and Altmiller Cave use them.
void FieldEffect_InitializeSlotsFar(void);
/// Frees every field effect sprite and its work, then waits a frame.
void FieldEffect_ClearActiveSlotsAndScheduleUpdatesFar(void);
/// Places an actor on the leader and walks it out to an offset, then
/// gives it a movement type.
void Actor_LaunchFromLeaderFar(u32 actorId, s32 xOffset, s32 zOffset,
                               s32 action);
/// Walks an actor a given offset in pixels from where it stands.
void Actor_OffsetPositionAndResetFar(u32 actorId, s32 x_offset, s32 z_offset);
/// Walks an actor an offset in pixels, waits for it to arrive and stands
/// it still.
void Actor_CommitPositionAndActivateFar(u32 actorId, s32 x_offset,
                                        s32 z_offset);
/// Makes an actor follow another through the watched actor script.
void Actor_WatchAndSetScriptFar(u32 actorId, u32 watchedActorId);
/// Pauses an actor, keeping its update and pose in gSequenceParentWork for
/// later; Halt and the Colosso operators use it.
void FieldPsynergy_HaltActorFar(s32 id);
/// Captures up to 32 actors into gActorSnapshot, for
/// Actor_Restore (Actor_Snapshot); the save menu calls it.
void Save_CaptureActorsFar(void);
/// Puts back the actors Actor_Snapshot saved and moves the camera
/// to the leader's height.
void Actor_RestoreFar(void);
/// Starts a zoom to a 16.16 factor over the given frames, on the World Map.
void FieldEffect_ScheduleRatioTransitionFar(s32 zoom, s32 frames);
/// Waits, at most 300 frames, for the World Map's zoom to end.
void Event_WaitForZoomTransitionFar(void);
/// Returns the area name of a scene and entrance, an offset from
/// MsgDebugEntryName, or 0 when no rule matches.
s32 Area_GetNameIndexFar(s32 sceneId, s32 entrance);
/// Starts an actor's movement type: an enum MovementType, or the actor script
/// itself.
void Actor_SetMovementTypeFar(struct Actor *actor, s32 kind);
/// Plays SE_STOP_MUSIC_PLAYER_5 and then sound_id (SOUND_FADE_OUT_ACTION_SE for
/// -1), and remembers sound_id to play again later.
void Scene_QueueMusicFar(s32 sound_id);
/// Plays the sound Scene_QueueMusic remembered, if any.
void Scene_PlayQueuedMusicFar(void);
/// Returns a character's sprite for a save file's summary, which records only
/// whether it was childhood.
s32 Party_RemapCharacterIdIfEnabledFar(s32 character, s32 childhood);
#if TBS_PATCH_2
/// While Reveal is active, releases its sparkles and leaves its marks; the
/// field menu calls it after each choice.
void FieldEffect_ClearRandomParticlesFar(void);
#endif
