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
  BGM_UNITY_SCALE = 0x100,
  /// A ramp's step a VBlank: a full fade in or out takes 64 frames.
  BGM_DEFAULT_RAMP_STEP = 4,
  /// The track mask of every music command here. It selects tracks 0 to 7
  /// only, though the music player has 10 (sound/music_player_table.s) and
  /// MUS_KALAY, MUS_ELEMENTAL_STARS, MUS_DUNGEON, MUS_CONFRONTATION,
  /// MUS_UNUSED, MUS_TOLBI_BOUND_SHIP, MUS_WORLD_MAP, MUS_VENUS_LIGHTHOUSE and
  /// MUS_FILE_SCREEN use 9 or more: their tracks 8 and 9 keep their volume and
  /// pitch through the ramps and under a fanfare.
  TRACKS_ALL = 0xff,
  MUSIC_PLAYER_COUNT = 8, ///< gMusicPlayerLastSong' entries, one a player.
  /// VBlanks after a fanfare starts before the music waits for it to end.
  BGM_RESTORE_DELAY_FRAMES = 10,
  /// Sound_WaitForSong gives up after this many frames, about
  /// five seconds.
  BGM_WAIT_TIMEOUT_FRAMES = 300
};

/// Plays a song number, the game's one way of playing a sound: a fade control,
/// a sound effect, a fanfare or music, as the README's "What a song number
/// means" describes. The low 12 bits are the song, SONG_FLAG_FADE_IN the only
/// flag read. Reached through the sound far-call table as Sound_PlaySongFar
/// from the whole game; it keeps the music's ramps and song state (gCurrentBgm,
/// gBgmRestoreDelay) up to date.
void Sound_PlaySong(s32 id) {
  s32 flags;
  s32 player;

  flags = id & SONG_FLAGS_MASK;
  id &= SONG_NUM_MASK;

  if (id == SONG_FADE_OUT_BGM) {
    // Fade the music out, once until the next music starts: 16 steps 7
    // frames apart, 112 frames. MUS_SILENCE is a song with no tracks,
    // so a later request for the faded music starts it again.
    if (gBgmFadeRequested != 0)
      return;
    m4aMPlayFadeOut(&gMPlayInfo_BGM, 7);
    gBgmFadeRequested++;
    gCurrentBgm = MUS_SILENCE;
  } else if (id == SONG_FADE_OUT_ACTION_SE) {
    // The control is also the number of the song table's row 289, so it fades
    // MUSIC_PLAYER_ACTION_SE out over 48 frames (16 steps of 3) and that row
    // never plays. Only sound effects record their song in
    // gMusicPlayerLastSong; MUSIC_PLAYER_ACTION_SE's is 0 after this fade.
    gMusicPlayerLastSong[MUSIC_PLAYER_ACTION_SE] = 0;
    m4aMPlayFadeOut(&gMPlayInfo_ActionSE, 3);
  } else if (id >= SONG_SE_FIRST) {
    // A song on MUSIC_PLAYER_SE_LAST borrows a free effect player,
    // searching back to MUSIC_PLAYER_SE_FIRST.
    // A player is busy while the low byte of its status, the active bits of
    // its first eight tracks, is nonzero; with all four busy,
    // MUSIC_PLAYER_SE_LAST's sound is cut off.
    player = gSongTable[id].ms;
    if (player == MUSIC_PLAYER_SE_LAST) {
      while (((u8 *)&gMPlayTable[player].info->status)[0] != 0) {
        player--;
        if (player < MUSIC_PLAYER_SE_FIRST) {
          player = MUSIC_PLAYER_SE_LAST;
          break;
        }
      }
    }
    MPlayStart(gMPlayTable[player].info, gSongTable[id].header);
    gMusicPlayerLastSong[player] = id;
  } else if (id >= SONG_FANFARE_FIRST) {
    // A fanfare, on its own player (MUSIC_PLAYER_FANFARE): the music's tracks 0
    // to 7 go silent at once but keep playing, and fade back in once the
    // fanfare has ended (Sound_UpdateBgmRamps).
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0);
    gBgmVolumeTarget = 0;
    gBgmVolume = 0;
    m4aSongNumStart(id);
    gBgmRestoreDelay = BGM_RESTORE_DELAY_FRAMES;
  } else {
    // Music: the keep control, or the music last chosen (gCurrentBgm),
    // changes nothing, so a scene may name its music on every entry; a song
    // that has ended is not started again. The three songs below take the long
    // reverb code, the rest the normal one (enum SoundReverbCode).
    if (id == SONG_KEEP_BGM)
      return;
    if (id == gCurrentBgm)
      return;
    gCurrentBgm = id;
    Sound_LoadReverbCode(
        (id == MUS_TITLE || id == MUS_ENDING_LOGO || id == MUS_ENDING)
            ? SOUND_REVERB_LONG
            : SOUND_REVERB_NORMAL);
    m4aSongNumStart(id);
    // A silent start fades the new music in over 64 frames.
    if (flags & SONG_FLAG_FADE_IN)
      gBgmVolume = 0;
    else
      gBgmVolume = BGM_UNITY_SCALE;
    gBgmVolumeTarget = BGM_UNITY_SCALE;
    gBgmVolumeStep = BGM_DEFAULT_RAMP_STEP;
    gBgmFadeRequested = 0;
  }
}

/// Restores BGM after a fanfare and steps its volume and combined tempo/pitch
/// ramps once per VBlank, clamping each ramp when it crosses its target. After
/// a fanfare starts it counts gBgmRestoreDelay down, waits until the fanfare's
/// player has no track playing, and sets the music's target volume back to
/// BGM_UNITY_SCALE, so the code that played the fanfare does nothing more.
/// Called by System_VBlankHandler through the sound far-call table; it also
/// runs the PCM buffer's DMA restart count (m4aSoundVSync).
void Sound_UpdateBgmRamps(void) {
  s32 delta;

  // After a fanfare starts, count down to 1, then hold there until the
  // fanfare's player has no track left and let the music fade back in.
  if (gBgmRestoreDelay != 0) {
    if (gBgmRestoreDelay == 1) {
      if ((u8)gMPlayInfo_Fanfare.status == 0) {
        gBgmRestoreDelay = 0;
        gBgmVolumeTarget = BGM_UNITY_SCALE;
      }
    } else {
      gBgmRestoreDelay -= 1;
    }
  }
  // A step that changes the sign of target - value has crossed the target,
  // and the value stops on it.
  if ((s16)gBgmVolumeTarget != (s16)gBgmVolume) {
    delta = (s16)gBgmVolumeTarget - (s16)gBgmVolume;
    if (delta > 0) {
      gBgmVolume += gBgmVolumeStep;
    } else {
      gBgmVolume -= gBgmVolumeStep;
    }
    if ((((s16)gBgmVolumeTarget - (s16)gBgmVolume) ^ delta) < 0) {
      gBgmVolume = gBgmVolumeTarget;
    }
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, gBgmVolume);
  }
  if ((s16)gBgmTempoPitchTarget != (s16)gBgmTempoPitch) {
    delta = (s16)gBgmTempoPitchTarget - (s16)gBgmTempoPitch;
    if (delta > 0) {
      gBgmTempoPitch += gBgmTempoPitchStep;
    } else {
      gBgmTempoPitch -= gBgmTempoPitchStep;
    }
    if ((((s16)gBgmTempoPitchTarget - (s16)gBgmTempoPitch) ^ delta) < 0) {
      gBgmTempoPitch = gBgmTempoPitchTarget;
    }
    // The tempo scale and the pitch move together, like a record played at
    // another speed, but in a straight line: 12 semitones for each 0x100 of
    // scale (pitch is in 1/256 semitone), so 0x200 plays an octave up and
    // 0x80 half an octave down.
    m4aMPlayTempoControl(&gMPlayInfo_BGM, gBgmTempoPitch);
    m4aMPlayPitchControl(&gMPlayInfo_BGM, TRACKS_ALL,
                         (s16)gBgmTempoPitch * 12 - 12 * BGM_UNITY_SCALE);
  }
  // Count the PCM buffer's frames down and restart the FIFO DMAs from its
  // start when they run out (SoundInfo.transferCountdown).
  m4aSoundVSync();
}

/// Runs the audio test's three song selectors and five reverb codes forever.
/// Up and Down choose a row, Right and Left change its song by 1, R and L by
/// 10, and A plays it; B plays MUS_SILENCE, Start fades the music,
/// and Select both steps the reverb code (SOUND_REVERB_NORMAL at first) and
/// fades MUSIC_PLAYER_ACTION_SE. Never returns. Reached only through the sound
/// far-call table's veneer; nothing in ☀️ names it.
void SoundTest_Run(void) {
  // One row each for music, fanfares (from SONG_FANFARE_FIRST) and sound
  // effects (from SONG_SE_FIRST).
  s32 cues[3] = {0, 80, 100};
  s32 reverbCode;
  s32 row;
  s32 clipTimer;

  row = 0;
  reverbCode = SOUND_REVERB_NORMAL;
  /* FAKEMATCH(one-pass): the block keeps the state-address load after
   * reverbCode setup and the state store before clipTimer setup. Plain store:
   * differ 8 or more in nine statement orders. */
  // XXX (no evidence): why the audio test clears the first four digits of the
  // build stamp copy; nothing in the game reads that copy.
  do {
    gBuildStamp[0] = 0;
  } while (0);
  clipTimer = 20;

  for (;;) {
    // A 20-frame countdown, restarted whenever the mixer has clipped a sample
    // since the last frame; nothing reads it.
    if (clipTimer != 0)
      clipTimer--;

    if (gSoundMixerClipped != 0) {
      gSoundMixerClipped = 0;
      clipTimer = 20;
    }

    if (gKeysRepeat & KEY_SELECT) {
      reverbCode = (reverbCode + 1) % 5;
      Sound_LoadReverbCode(reverbCode);
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
      Sound_PlaySong(cues[row]);
    if (gKeysRepeat & KEY_B)
      Sound_PlaySong(MUS_SILENCE);
    if (gKeysRepeat & KEY_START)
      Sound_PlaySong(SONG_FADE_OUT_BGM);
    if (gKeysRepeat & KEY_SELECT)
      Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);

    WaitTicks(1);
  }
}

/// Starts the sound driver, resets the BGM ramps (gBgmVolume and its kin)
/// and clears every player's last song. Called by System_Initialize through the
/// sound far-call table.
void Sound_Initialize(void) {
  s16 *playerVolume;
  s32 remaining;

  m4aSoundInit();
  // No music is song 0xff, so the first music always plays.
  gCurrentBgm = 0xff;
  gBgmRestoreDelay = 0;
  gBgmVolumeTarget = BGM_UNITY_SCALE;
  gBgmVolume = BGM_UNITY_SCALE;
  gBgmVolumeStep = BGM_DEFAULT_RAMP_STEP;
  gBgmTempoPitchTarget = BGM_UNITY_SCALE;
  gBgmTempoPitch = BGM_UNITY_SCALE;
  gBgmTempoPitchStep = BGM_DEFAULT_RAMP_STEP;
  gBgmFadeRequested = 0;
  gSoundCommandMask = 0;
  playerVolume = gMusicPlayerLastSong;
  // XXX (no evidence): what gSound_UnreadByte holds; no code in either game
  // reads it or its Lost Age twin (gData_02005804, also after
  // gBgmRestoreDelay), and no literal in the ROM but this one points at it.
  gSound_UnreadByte = 0;
  remaining = MUSIC_PLAYER_COUNT - 1;
  do {
    remaining--;
    *playerVolume = 0;
    playerVolume += 1;
  } while (remaining >= 0);
}

/// Sets the BGM tempo scale directly; 0x100 preserves its native tempo. The
/// scale lasts until the next song starts (MPlayStart sets 0x100)
/// or the tempo and pitch ramp moves. Called only by the Lemurian ship event,
/// which plays MUS_ENDING at 240 (0.94 of its tempo) as the ship sails off.
void Sound_SetBgmTempo(u16 tempoScale) {
  m4aMPlayTempoControl(&gMPlayInfo_BGM, tempoScale);
}

/// Applies a pitch offset, in 1/256 semitone, to the music's active tracks 0
/// to 7 (TRACKS_ALL). Reached only through the sound far-call table's
/// veneer.
void Sound_SetBgmPitch(u16 pitch) {
  m4aMPlayPitchControl(&gMPlayInfo_BGM, TRACKS_ALL, pitch);
}

/// Sets the target and per-frame step of the combined BGM tempo/pitch ramp
/// (0x100 the song's own). Reached only through the sound far-call table's
/// veneer.
void Sound_SetBgmTempoPitchTarget(u16 targetScale, u16 step) {
  gBgmTempoPitchTarget = targetScale;
  gBgmTempoPitchStep = step;
}

/// Sets BGM volume (0x100 full) immediately and cancels any outstanding
/// volume ramp. Reached only through the sound far-call table's veneer.
void Sound_SetBgmVolume(s16 volume) {
  m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, volume);
  gBgmVolumeTarget = volume;
  gBgmVolume = volume;
}

/// Sets the BGM volume target (0x100 full) and its per-frame step. Reached
/// only through the sound far-call table's veneer.
void Sound_SetBgmVolumeTarget(u16 targetVolume, u16 step) {
  gBgmVolumeTarget = targetVolume;
  gBgmVolumeStep = step;
}

/// Returns nonzero while a fanfare holds the music: from its start until its
/// player falls silent and the music begins to fade back in. Polled by the
/// Colosso scenes, the Djinni-joined messages (window/screens/joined.c),
/// the message windows' pause skip (window/window/messages.c) and
/// Sound_WaitForFanfare (shop/shop/run.c), which the inn, the
/// sanctum, the Lemurian ship and Tolbi's spring use.
s32 Sound_GetBgmRestoreDelay(void) { return gBgmRestoreDelay; }

/// Stops every sound player while preserving its state for a later resume.
/// Reached only through the sound far-call table's veneer.
void Sound_StopAllMusicPlayers(void) { m4aMPlayAllStop(); }

/// Resumes every player, each from where m4aMPlayAllStop stopped it.
/// Reached only through the sound far-call table's veneer.
void Sound_ResumeAllMusicPlayers(void) { m4aMPlayAllContinue(); }

/// Replaces the audio command mask, or toggles its requested bits when
/// flagged. Reached only through the sound far-call table's veneer; nothing
/// in ☀️ reads gSoundCommandMask by name.
void Sound_UpdateCommandMask(u32 command) {
  u32 toggle = command & SOUND_COMMAND_MASK_TOGGLE;

  command &= SOUND_COMMAND_MASK_BITS;
  if (toggle != 0)
    gSoundCommandMask ^= command;
  else
    gSoundCommandMask = command;
}

/// Returns the music Sound_PlaySong last chose: 0xff before any,
/// MUS_SILENCE after a fade, else the last music. SceneManager_Run saves it
/// as gSaveBlock.returnMusic before the Pause menu, so the music can be started
/// again when the party is put back where it was (SequenceManager_Run,
/// overworld/overworld/sequence_manager.c).
s32 Sound_GetCurrentBgm(void) { return gCurrentBgm; }

/// Waits a frame at a time for fanfare restoration, giving up after 300
/// frames. Called only by Vale's sanctum, after the fanfare (MUS_INN) for
/// accepting the Great Healer's quest.
void Sound_WaitForSong(void) {
  s32 framesWaited = 0;

  do {
    if (gBgmRestoreDelay == 0)
      break;
    WaitTicks(1);
    framesWaited++;
  } while (framesWaited < BGM_WAIT_TIMEOUT_FRAMES);
}

/// Unused: returns the reverb code Sound_PlaySong gives music `command`:
/// SOUND_REVERB_LONG for MUS_ENDING, MUS_TITLE and MUS_ENDING_LOGO,
/// SOUND_REVERB_NORMAL for the rest. Nothing in ☀️ calls it.
UNUSED s32 Sound_GetReverbCode(s32 command) {
  if (command == MUS_TITLE || command == MUS_ENDING_LOGO ||
      command == MUS_ENDING)
    return SOUND_REVERB_LONG;
  return SOUND_REVERB_NORMAL;
}
