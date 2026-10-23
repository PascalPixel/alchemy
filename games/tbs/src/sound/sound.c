#include "unit/sound/sound.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "audio_engine.h"
#include "audio_engine_types.h"
#include "audio_engine_symbols.h"
#include "types.h"
#include "io_reg.h"
#include "songs.h"

enum {
  /// Full volume, and the song's own tempo and pitch: 1.0 in the ramps' 8.8
  /// fixed point.
  MUSIC_UNITY_SCALE = 0x100,
  /// A ramp's step a VBlank: a full fade in or out takes 64 frames.
  MUSIC_DEFAULT_RAMP_STEP = 4,
  /// The track mask of every music command here. It selects tracks 0 to 7
  /// only, though the music player has 10 (sound/players.s) and MUS_006,
  /// MUS_022, MUS_028, MUS_029, MUS_040, MUS_041, MUS_WORLD_MAP,
  /// MUS_VENUS_LIGHTHOUSE and MUS_074 use 9 or more: their tracks 8 and 9
  /// keep their volume and pitch through the ramps and under a fanfare.
  MUSIC_ALL_TRACKS = 0xff,
  MUSIC_PLAYER_COUNT = 8, ///< gMusicPlayerVolumes' entries, one a player.
  /// VBlanks after a fanfare starts before the music waits for it to end.
  MUSIC_RESTORE_DELAY_FRAMES = 10,
  /// AudioCommand_WaitForCompletion gives up after this many frames, about
  /// five seconds.
  MUSIC_WAIT_TIMEOUT_FRAMES = 300
};

/// Plays a cue, the game's one way of playing a sound: a fade control, a sound
/// effect, a fanfare or music, as the README's "What a cue number means"
/// describes. The low 12 bits are the cue, AUDIO_CUE_START_SILENT the only flag
/// read. Reached through the sound far-call table as AudioCommand_PlayFar from
/// the whole game; it keeps the music's ramps and cue records
/// (gAudioSecondaryState, gMusicRestoreDelay) up to date.
void AudioCommand_Play(s32 id) {
  s32 flags;
  s32 player;

  flags = id & AUDIO_CUE_FLAGS_MASK;
  id &= AUDIO_CUE_ID_MASK;

  if (id == AUDIO_CUE_FADE_BGM) {
    // Fade the music out, once until the next music starts: 16 steps 7
    // frames apart, 112 frames. MUS_SILENCE is a song with no tracks,
    // so a later cue for the faded music starts it again.
    if (gMusicFadeRequested != 0)
      return;
    MusicPlayer_FadeOut(&gMusicPlayerBgm, 7);
    gMusicFadeRequested++;
    gAudioSecondaryState = MUS_SILENCE;
  } else if (id == AUDIO_CUE_FADE_SLOT3) {
    // The cue is also the number of the song table's row 289, so it fades
    // player 3 out over 48 frames (16 steps of 3) and that row never plays.
    // Only sound effects record their cue in gMusicPlayerVolumes; player 3's
    // is 0 after this fade.
    gMusicPlayerVolumes[3] = 0;
    MusicPlayer_FadeOut(&gSound_Player3, 3);
  } else if (id >= AUDIO_SONG_FIRST) {
    // A song on SOUND_PLAYER_EFFECT_LAST borrows a free effect player,
    // searching back to SOUND_PLAYER_EFFECT_FIRST.
    // A player is busy while the low byte of its status, the active bits of
    // its first eight tracks, is nonzero; with all four busy,
    // SOUND_PLAYER_EFFECT_LAST's sound is cut off.
    player = gSound_SongTable[id].slot;
    if (player == SOUND_PLAYER_EFFECT_LAST) {
    next:
      if (((u8 *)&gSound_PlayerSlots[player].player->status)[0] != 0) {
        player--;
        if (player >= SOUND_PLAYER_EFFECT_FIRST)
          goto next;
        player = SOUND_PLAYER_EFFECT_LAST;
      }
    }
    MusicPlayer_StartSong(gSound_PlayerSlots[player].player,
                          gSound_SongTable[id].header);
    gMusicPlayerVolumes[player] = id;
  } else if (id >= AUDIO_JINGLE_FIRST) {
    // A fanfare, on its own player (SOUND_PLAYER_FANFARE): the music's tracks 0
    // to 7 go silent at once but keep playing, and fade back in once the
    // fanfare has ended (MusicPlayer_StepVolumeAndPitchTowardTargets).
    MusicPlayer_SetVolume(&gMusicPlayerBgm, 255, 0);
    gMusicVolumeTarget = 0;
    gMusicVolume = 0;
    Audio_PlaySound(id);
    gMusicRestoreDelay = MUSIC_RESTORE_DELAY_FRAMES;
  } else {
    // Music: the keep cue, or the music cue last chosen (gAudioSecondaryState),
    // changes nothing, so a scene may name its music on every entry; a song
    // that has ended is not started again. The three songs below take mixer
    // record 3, the rest record 2 (the README's "The mixer's code is chosen per
    // song").
    if (id == AUDIO_CUE_KEEP_BGM)
      return;
    if (id == gAudioSecondaryState)
      return;
    gAudioSecondaryState = id;
    Sound_LoadPresetParameters(
        (id == MUS_TITLE || id == MUS_ENDING_LOGO || id == MUS_ENDING) ? 3 : 2);
    Audio_PlaySound(id);
    // A silent start fades the new music in over 64 frames.
    if (flags & AUDIO_CUE_START_SILENT)
      gMusicVolume = 0;
    else
      gMusicVolume = MUSIC_UNITY_SCALE;
    gMusicVolumeTarget = MUSIC_UNITY_SCALE;
    gMusicVolumeStep = MUSIC_DEFAULT_RAMP_STEP;
    gMusicFadeRequested = 0;
  }
}

/// Restores BGM after a fanfare and steps its volume and combined tempo/pitch
/// ramps once per VBlank, clamping each ramp when it crosses its target.
/// Called by System_VBlankHandler through the sound far-call table; it also
/// runs the PCM buffer's DMA restart count (Sound_TickDmaRestartTimer).
void MusicPlayer_StepVolumeAndPitchTowardTargets(void) {
  s32 delta;

  // After a fanfare starts, count down to 1, then hold there until the
  // fanfare's player has no track left and let the music fade back in.
  if (gMusicRestoreDelay != 0) {
    if (gMusicRestoreDelay == 1) {
      if ((u8)gMusicPlayerFanfare.status == 0) {
        gMusicRestoreDelay = 0;
        gMusicVolumeTarget = MUSIC_UNITY_SCALE;
      }
    } else {
      gMusicRestoreDelay -= 1;
    }
  }
  // A step that changes the sign of target - value has crossed the target,
  // and the value stops on it.
  if ((s16)gMusicVolumeTarget != (s16)gMusicVolume) {
    delta = (s16)gMusicVolumeTarget - (s16)gMusicVolume;
    if (delta > 0) {
      gMusicVolume += gMusicVolumeStep;
    } else {
      gMusicVolume -= gMusicVolumeStep;
    }
    if ((((s16)gMusicVolumeTarget - (s16)gMusicVolume) ^ delta) < 0) {
      gMusicVolume = gMusicVolumeTarget;
    }
    MusicPlayer_SetVolume(&gMusicPlayerBgm, MUSIC_ALL_TRACKS, gMusicVolume);
  }
  if ((s16)gMusicPitchTarget != (s16)gMusicPitch) {
    delta = (s16)gMusicPitchTarget - (s16)gMusicPitch;
    if (delta > 0) {
      gMusicPitch += gMusicPitchStep;
    } else {
      gMusicPitch -= gMusicPitchStep;
    }
    if ((((s16)gMusicPitchTarget - (s16)gMusicPitch) ^ delta) < 0) {
      gMusicPitch = gMusicPitchTarget;
    }
    // The tempo scale and the pitch move together, like a record played at
    // another speed, but in a straight line: 12 semitones for each 0x100 of
    // scale (pitch is in 1/256 semitone), so 0x200 plays an octave up and
    // 0x80 half an octave down.
    MusicPlayer_SetPitchAndUpdateFrequency(&gMusicPlayerBgm, gMusicPitch);
    MusicPlayer_SetPitch(&gMusicPlayerBgm, MUSIC_ALL_TRACKS,
                         (s16)gMusicPitch * 12 - 12 * MUSIC_UNITY_SCALE);
  }
  // Count the PCM buffer's frames down and restart the FIFO DMAs from its
  // start when they run out (SoundWork.transferCountdown).
  Sound_TickDmaRestartTimer();
}

/// Runs the audio test's three cue selectors and five mixer presets forever.
/// Up and Down choose a row, Right and Left change its cue by 1, R and L by
/// 10, and A plays it; B plays MUS_SILENCE, Start fades the music,
/// and Select both steps the mixer preset (0 to 4, 2 at first) and fades player
/// 3. Never returns. Reached only through the sound far-call table's veneer;
/// nothing in ☀️ names it.
void AudioTest_RunParameterController(void) {
  // One row each for music, fanfares (from AUDIO_JINGLE_FIRST) and sound
  // effects (from AUDIO_SONG_FIRST).
  s32 cues[3] = {0, 80, 100};
  s32 preset;
  s32 row;
  s32 timer;

  row = 0;
  preset = 2;
  /* FAKEMATCH(one-pass): the block keeps the state-address load after preset
   * setup and the state store before timer setup. Plain store: differ 8 or more
   * in nine statement orders. */
  // XXX: this overwrites the first four digits of the build stamp copy; why
  // the audio test clears them is not known.
  do {
    gBuildStamp[0] = 0;
  } while (0);
  timer = 20;

  for (;;) {
    // XXX: timer counts down from 20 and restarts while gSoundClipFlag is
    // set, but nothing reads it, and no code but this test names the flag;
    // what sets it, and what both were for, is not known. ⚓️'s copy of this
    // test (games/tla/src/sound/audio_test.c) only clears its flag too, so a
    // writer of the flag in either game would settle it.
    if (timer != 0)
      timer--;

    if (gSoundClipFlag != 0) {
      gSoundClipFlag = 0;
      timer = 20;
    }

    if (gKeysRepeat & KEY_SELECT) {
      preset = (preset + 1) % 5;
      Sound_LoadPresetParameters(preset);
    }
    if (gKeysRepeat & KEY_R)
      cues[row] += 10;
    if (gKeysRepeat & KEY_L)
      cues[row] -= 10;
    if (gKeysRepeat & KEY_RIGHT)
      cues[row] += 1;
    if (gKeysRepeat & KEY_LEFT)
      cues[row] -= 1;
    if (gKeysRepeat & KEY_UP) {
      if (row > 0)
        row--;
    }
    if (gKeysRepeat & KEY_DOWN) {
      if (row <= 1)
        row++;
    }
    if (gKeysRepeat & KEY_A)
      AudioCommand_Play(cues[row]);
    if (gKeysRepeat & KEY_B)
      AudioCommand_Play(MUS_SILENCE);
    if (gKeysRepeat & KEY_START)
      AudioCommand_Play(AUDIO_CUE_FADE_BGM);
    if (gKeysRepeat & KEY_SELECT)
      AudioCommand_Play(AUDIO_CUE_FADE_SLOT3);

    WaitFrames(1);
  }
}

/// Starts the sound driver, resets the BGM ramps (gMusicVolume and its kin)
/// and clears all player cue records. Called by System_Initialize through the
/// sound far-call table.
void Audio_InitializeRuntimeDefaults(void) {
  s16 *playerVolume;
  s32 remaining;

  Audio_Initialize();
  // No music cue is 0xff, so the first music cue always plays.
  gAudioSecondaryState = 0xff;
  gMusicRestoreDelay = 0;
  gMusicVolumeTarget = MUSIC_UNITY_SCALE;
  gMusicVolume = MUSIC_UNITY_SCALE;
  gMusicVolumeStep = MUSIC_DEFAULT_RAMP_STEP;
  gMusicPitchTarget = MUSIC_UNITY_SCALE;
  gMusicPitch = MUSIC_UNITY_SCALE;
  gMusicPitchStep = MUSIC_DEFAULT_RAMP_STEP;
  gMusicFadeRequested = 0;
  gAudio_CommandMask = 0;
  playerVolume = gMusicPlayerVolumes;
  // XXX: gEwram_Data1 is cleared here and named nowhere else in ☀️; what it
  // holds is not known. ⚓️'s twin (gData_02005804, also after
  // gMusicRestoreDelay) is only cleared as well; a read of either byte
  // would settle it.
  gEwram_Data1 = 0;
  remaining = MUSIC_PLAYER_COUNT - 1;
  do {
    remaining--;
    *playerVolume = 0;
    playerVolume += 1;
  } while (remaining >= 0);
}

/// Sets the BGM tempo scale directly; 0x100 preserves its native tempo. The
/// scale lasts until the next song starts (MusicPlayer_StartSong sets 0x100)
/// or the tempo and pitch ramp moves. Called only by the Lemurian ship scene,
/// which plays MUS_ENDING at 240 (0.94 of its tempo) as the ship sails off.
void MusicCommand_SetPitchAndUpdateFrequency(u16 tempoScale) {
  MusicPlayer_SetPitchAndUpdateFrequency(&gMusicPlayerBgm, tempoScale);
}

/// Applies a pitch offset, in 1/256 semitone, to the music's active tracks 0
/// to 7 (MUSIC_ALL_TRACKS). Reached only through the sound far-call table's
/// veneer.
void MusicCommand_SetPitch(u16 pitch) {
  MusicPlayer_SetPitch(&gMusicPlayerBgm, MUSIC_ALL_TRACKS, pitch);
}

/// Sets the target and per-frame step of the combined BGM tempo/pitch ramp
/// (0x100 the song's own). Reached only through the sound far-call table's
/// veneer.
void Audio_SetWorkPairB(u16 targetScale, u16 step) {
  gMusicPitchTarget = targetScale;
  gMusicPitchStep = step;
}

/// Sets BGM volume (0x100 full) immediately and cancels any outstanding
/// volume ramp. Reached only through the sound far-call table's veneer.
void MusicCommand_SetVolume(s16 volume) {
  MusicPlayer_SetVolume(&gMusicPlayerBgm, MUSIC_ALL_TRACKS, volume);
  gMusicVolumeTarget = volume;
  gMusicVolume = volume;
}

/// Sets the BGM volume target (0x100 full) and its per-frame step. Reached
/// only through the sound far-call table's veneer.
void Audio_SetMusicVolumeTargetAndStep(u16 targetVolume, u16 step) {
  gMusicVolumeTarget = targetVolume;
  gMusicVolumeStep = step;
}

/// Returns nonzero while a fanfare holds the music: from its start until its
/// player falls silent and the music begins to fade back in. Polled by the
/// Colosso scenes, the Djinni-joined messages (window/battle_command/joined.c),
/// the message windows' pause skip (window/window/messages.c) and
/// AudioCommand_WaitForStateByteClear (shop/shop/run.c), which the inn, the
/// sanctum, the Lemurian ship and Tolbi's spring use.
s32 AudioCommand_GetStateByte(void) { return gMusicRestoreDelay; }

/// Stops every sound player while preserving its state for a later resume.
/// Reached only through the sound far-call table's veneer.
void AudioCommand_StopAllPlayers(void) { Audio_StopAllPlayers(); }

/// Resumes every player, each from where Audio_StopAllPlayers stopped it.
/// Reached only through the sound far-call table's veneer.
void AudioCommand_ResumeAllPlayers(void) { Audio_ResumeAllPlayers(); }

/// Replaces the audio command mask, or toggles its requested bits when
/// flagged. Reached only through the sound far-call table's veneer; nothing
/// in ☀️ reads gAudio_CommandMask by name.
void AudioCommand_UpdateToggleMask(u32 command) {
  u32 toggle = command & AUDIO_COMMAND_TOGGLE;

  command &= AUDIO_COMMAND_MASK;
  if (toggle != 0)
    gAudio_CommandMask ^= command;
  else
    gAudio_CommandMask = command;
}

/// Returns the music cue AudioCommand_Play last chose: 0xff before any,
/// MUS_SILENCE after a fade, else the last music cue. Field_RunMap saves it
/// as gGameState.returnCue before the pause menu, so the music can be started
/// again when the party is put back where it was (Game_Run,
/// object/scene.c).
s32 AudioCommand_GetSecondaryStateByte(void) { return gAudioSecondaryState; }

/// Waits a frame at a time for fanfare restoration, giving up after 300
/// frames. Called only by Vale's sanctum, after the fanfare (MUS_086) for
/// accepting the Great Healer's quest.
void AudioCommand_WaitForCompletion(void) {
  s32 framesWaited = 0;

  do {
    if (gMusicRestoreDelay == 0)
      break;
    WaitFrames(1);
    framesWaited++;
  } while (framesWaited < MUSIC_WAIT_TIMEOUT_FRAMES);
}

/// Unused: returns the mixer record AudioCommand_Play gives music cue
/// `command`: 3 for MUS_ENDING, MUS_TITLE and MUS_ENDING_LOGO, 2 for the rest.
/// Nothing in ☀️ calls it.
UNUSED s32 AudioCommand_GetWidth(s32 command) {
  if (command == MUS_TITLE || command == MUS_ENDING_LOGO ||
      command == MUS_ENDING)
    return 3;
  return 2;
}
