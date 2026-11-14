/// @file
/// The game's own sound code over the sound driver: Sound_Play, which takes
/// every sound number, the music's volume and tempo ramps, the music's return
/// after a jingle, and a sound test nothing in the game names.

#include "sound/sound.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "sound/sound_driver.h"
#include "system/io_reg.h"
#include "system/system.h"

enum {
  /// Full volume, and the song's own tempo and pitch: 1.0 in the ramps' 8.8
  /// fixed point.
  BGM_UNITY_SCALE = 256,
  /// A ramp's step a VBlank: a full fade in or out takes 64 VBlanks.
  BGM_DEFAULT_RAMP_STEP = 4,
  /// The track mask of every music command here.
  TRACKS_ALL = 0xff,
  MUSIC_PLAYER_COUNT = 8, ///< gMusicPlayerLastSound' entries, one a player.
  /// VBlanks after a jingle starts before the music waits for it to end.
  BGM_RESTORE_DELAY_VBLANKS = 10,
  /// Sound_WaitForBgmRestore gives up after this many frames, about
  /// five seconds.
  BGM_WAIT_TIMEOUT_FRAMES = 300
};

/// Plays a sound number, the game's one way of playing a sound: a fade control,
/// a sound effect, a jingle or music, as the README's "What a sound number
/// means" describes.
void Sound_Play(s32 id) {
  s32 flags;
  s32 player;

  flags = id & SOUND_FLAGS_MASK;
  id &= SOUND_NUM_MASK;

  if (id == SOUND_FADE_OUT_BGM) {
    // Fade the music out, once until the next music starts: 16 steps 7 VBlanks
    // apart, 112 VBlanks.
    if (gBgmFadeRequested != 0)
      return;
    m4aMPlayFadeOut(&gMPlayInfo_BGM, 7);
    gBgmFadeRequested++;
    gCurrentBgm = MUS_SILENCE;
  } else if (id == SOUND_FADE_OUT_ACTION_SE) {
    // The control is also the number of the song table's row 289, so it fades
    // MUSIC_PLAYER_ACTION_SE out over 48 VBlanks (16 steps of 3) and that row
    // never plays.
    gMusicPlayerLastSound[MUSIC_PLAYER_ACTION_SE] = 0;
    m4aMPlayFadeOut(&gMPlayInfo_ActionSE, 3);
  } else if (id >= SOUND_SE_FIRST) {
    // A sound on MUSIC_PLAYER_SE_LAST borrows a free effect player, searching
    // back to MUSIC_PLAYER_SE_FIRST.
    player = gSongTable[id].ms;
    if (player == MUSIC_PLAYER_SE_LAST) {
      while ((u8)gMPlayTable[player].info->status != 0) {
        player--;
        if (player < MUSIC_PLAYER_SE_FIRST) {
          player = MUSIC_PLAYER_SE_LAST;
          break;
        }
      }
    }
    MPlayStart(gMPlayTable[player].info, gSongTable[id].header);
    gMusicPlayerLastSound[player] = id;
  } else if (id >= SOUND_JINGLE_FIRST) {
    // A jingle, on its own player (MUSIC_PLAYER_JINGLE): the music's tracks 0
    // to 7 go silent at once but keep playing, and fade back in once the
    // jingle has ended (Sound_UpdateBgmRamps).
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0);
    gBgmVolumeTarget = 0;
    gBgmVolume = 0;
    m4aSongNumStart(id);
    gBgmRestoreDelay = BGM_RESTORE_DELAY_VBLANKS;
  } else {
    // Music: the keep control, or the music last chosen (gCurrentBgm), changes
    // nothing, so a scene may name its music on every entry; music that has
    // ended is not started again.
    if (id == SOUND_KEEP_BGM)
      return;
    if (id == gCurrentBgm)
      return;
    gCurrentBgm = id;
    Sound_LoadReverbCode(
        (id == MUS_TITLE || id == MUS_ENDING_LOGO || id == MUS_ENDING)
            ? SOUND_REVERB_LONG
            : SOUND_REVERB_NORMAL);
    m4aSongNumStart(id);
    // A silent start fades the new music in over 64 VBlanks.
    if (flags & SOUND_FLAG_FADE_IN)
      gBgmVolume = 0;
    else
      gBgmVolume = BGM_UNITY_SCALE;
    gBgmVolumeTarget = BGM_UNITY_SCALE;
    gBgmVolumeStep = BGM_DEFAULT_RAMP_STEP;
    gBgmFadeRequested = 0;
  }
}

/// Restores BGM after a jingle and steps its volume and combined tempo/pitch
/// ramps once per VBlank, clamping each ramp when it crosses its target.
void Sound_UpdateBgmRamps(void) {
  s32 delta;

  // After a jingle starts, count down to 1, then hold there until the
  // jingle's player has no track left and let the music fade back in.
  if (gBgmRestoreDelay != 0) {
    if (gBgmRestoreDelay == 1) {
      if ((u8)gMPlayInfo_Jingle.status == 0) {
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
    // another speed, but in a straight line: 12 semitones for each 256 of
    // scale (pitch is in 1/256 semitone), so 512 plays an octave up and
    // 128 half an octave down.
    m4aMPlayTempoControl(&gMPlayInfo_BGM, gBgmTempoPitch);
    m4aMPlayPitchControl(&gMPlayInfo_BGM, TRACKS_ALL,
                         (s16)gBgmTempoPitch * 12 - 12 * BGM_UNITY_SCALE);
  }
  // Count down the VBlanks left in the PCM buffer and restart the FIFO DMAs
  // from its start when they run out (SoundInfo.transferCountdown).
  m4aSoundVSync();
}

/// Runs the sound test's three sound selectors and five reverb codes forever.
void SoundTest_Run(void) {
  // One row each for music, jingles and sound effects.
  s32 cues[3] = {0, SOUND_JINGLE_FIRST, SOUND_SE_FIRST};
  s32 reverbCode;
  s32 row;
  s32 clipTimer;

  row = 0;
  reverbCode = SOUND_REVERB_NORMAL;
  /* FAKEMATCH(dead): a once-only block. */
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
      Sound_Play(cues[row]);
    if (gKeysRepeat & KEY_B)
      Sound_Play(MUS_SILENCE);
    if (gKeysRepeat & KEY_START)
      Sound_Play(SOUND_FADE_OUT_BGM);
    if (gKeysRepeat & KEY_SELECT)
      Sound_Play(SOUND_FADE_OUT_ACTION_SE);

    wait(1);
  }
}

/// Starts the sound driver, resets the BGM ramps (gBgmVolume and its kin) and
/// clears every player's last sound.
void Sound_Initialize(void) {
  s32 i;

  m4aSoundInit();
  // No music is sound 0xff, so the first music always plays.
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
  gSound_UnreadByte = 0;
  for (i = 0; i < MUSIC_PLAYER_COUNT; i++)
    gMusicPlayerLastSound[i] = 0;
}

/// Sets the BGM tempo scale directly; BGM_UNITY_SCALE preserves its native
/// tempo.
void Sound_SetBgmTempo(u16 tempoScale) {
  m4aMPlayTempoControl(&gMPlayInfo_BGM, tempoScale);
}

/// Applies a pitch offset, in 1/256 semitone, to the music's active tracks 0
/// to 7 (TRACKS_ALL).
void Sound_SetBgmPitch(u16 pitch) {
  m4aMPlayPitchControl(&gMPlayInfo_BGM, TRACKS_ALL, pitch);
}

/// Sets the target and per-VBlank step of the combined BGM tempo/pitch ramp
/// (BGM_UNITY_SCALE the music's own).
void Sound_SetBgmTempoPitchTarget(u16 targetScale, u16 step) {
  gBgmTempoPitchTarget = targetScale;
  gBgmTempoPitchStep = step;
}

/// Sets BGM volume (BGM_UNITY_SCALE full) immediately and cancels any
/// outstanding volume ramp.
void Sound_SetBgmVolume(s16 volume) {
  m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, volume);
  gBgmVolumeTarget = volume;
  gBgmVolume = volume;
}

/// Sets the BGM volume target (BGM_UNITY_SCALE full) and its per-VBlank step.
void Sound_SetBgmVolumeTarget(u16 targetVolume, u16 step) {
  gBgmVolumeTarget = targetVolume;
  gBgmVolumeStep = step;
}

/// Returns nonzero while a jingle holds the music: from its start until its
/// player falls silent and the music begins to fade back in.
s32 Sound_GetBgmRestoreDelay(void) { return gBgmRestoreDelay; }

/// Stops every sound player while preserving its state for a later resume.
void Sound_StopAllMusicPlayers(void) { m4aMPlayAllStop(); }

/// Resumes every player, each from where m4aMPlayAllStop stopped it.
void Sound_ResumeAllMusicPlayers(void) { m4aMPlayAllContinue(); }

/// Replaces the audio command mask, or toggles its requested bits when
/// flagged.
void Sound_UpdateCommandMask(u32 command) {
  u32 toggle = command & SOUND_COMMAND_MASK_TOGGLE;

  command &= SOUND_COMMAND_MASK_BITS;
  if (toggle != 0)
    gSoundCommandMask ^= command;
  else
    gSoundCommandMask = command;
}

/// Returns the music Sound_Play last chose: 0xff before any, MUS_SILENCE after
/// a fade, else the last music.
s32 Sound_GetCurrentBgm(void) { return gCurrentBgm; }

/// Waits a frame at a time for jingle restoration, giving up after 300 frames.
void Sound_WaitForBgmRestore(void) {
  s32 framesWaited = 0;

  do {
    if (gBgmRestoreDelay == 0)
      break;
    wait(1);
    framesWaited++;
  } while (framesWaited < BGM_WAIT_TIMEOUT_FRAMES);
}

/// Unused: returns the reverb code Sound_Play gives music `command`:
/// SOUND_REVERB_LONG for MUS_ENDING, MUS_TITLE and MUS_ENDING_LOGO,
/// SOUND_REVERB_NORMAL for the rest.
UNUSED s32 Sound_GetReverbCode(s32 command) {
  if (command == MUS_TITLE || command == MUS_ENDING_LOGO ||
      command == MUS_ENDING)
    return SOUND_REVERB_LONG;
  return SOUND_REVERB_NORMAL;
}
