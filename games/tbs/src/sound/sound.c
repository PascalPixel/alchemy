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
  MUSIC_UNITY_SCALE = 0x100,
  MUSIC_DEFAULT_RAMP_STEP = 4,
  MUSIC_ALL_TRACKS = 0xff,
  MUSIC_PLAYER_COUNT = 8,
  MUSIC_RESTORE_DELAY_FRAMES = 10,
  MUSIC_WAIT_TIMEOUT_FRAMES = 300
};

/// Dispatches a packed cue: fade controls, allocated sound effects, fanfares,
/// or background music with an optional silent start.
void AudioCommand_Play(s32 id) {
  s32 flags;
  s32 player;

  flags = id & AUDIO_CUE_FLAGS_MASK;
  id &= AUDIO_CUE_ID_MASK;

  if (id == AUDIO_CUE_FADE_BGM) {
    if (gMusicFadeRequested != 0)
      return;
    MusicPlayer_FadeOut(&gMusicPlayerBgm, 7);
    gMusicFadeRequested++;
    gAudioSecondaryState = MUS_019;
  } else if (id == AUDIO_CUE_FADE_SLOT3) {
    gMusicPlayerVolumes[3] = 0;
    MusicPlayer_FadeOut(&gSound_Player3, 3);
  } else if (id >= AUDIO_SONG_FIRST) {
    // Slot 7 borrows a free effect player, searching backwards through 4..7.
    player = gSound_SongTable[id].slot;
    if (player == 7) {
    next:
      if (((u8 *)&gSound_PlayerSlots[player].player->status)[0] != 0) {
        player--;
        if (player > 3)
          goto next;
        player = 7;
      }
    }
    MusicPlayer_StartSong(gSound_PlayerSlots[player].player,
                          gSound_SongTable[id].header);
    gMusicPlayerVolumes[player] = id;
  } else if (id >= AUDIO_JINGLE_FIRST) {
    MusicPlayer_SetVolume(&gMusicPlayerBgm, 255, 0);
    gMusicVolumeTarget = 0;
    gMusicVolume = 0;
    Audio_PlaySound(id);
    gMusicRestoreDelay = MUSIC_RESTORE_DELAY_FRAMES;
  } else {
    if (id == AUDIO_CUE_KEEP_BGM)
      return;
    if (id == gAudioSecondaryState)
      return;
    gAudioSecondaryState = id;
    Sound_LoadPresetParameters(
        (id == MUS_070 || id == MUS_075 || id == MUS_067) ? 3 : 2);
    Audio_PlaySound(id);
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
void MusicPlayer_StepVolumeAndPitchTowardTargets(void) {
  s32 delta;

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
    MusicPlayer_SetPitchAndUpdateFrequency(&gMusicPlayerBgm, gMusicPitch);
    MusicPlayer_SetPitch(&gMusicPlayerBgm, MUSIC_ALL_TRACKS,
                         (s16)gMusicPitch * 12 - 12 * MUSIC_UNITY_SCALE);
  }
  Sound_TickDmaRestartTimer();
}

/// Runs the audio test's three cue selectors and five mixer presets forever.
/// Buttons play, fade or adjust the selected cue while frames keep advancing.
void AudioTest_RunParameterController(void) {
  s32 cues[3] = {0, 80, 100};
  s32 preset;
  s32 row;
  s32 timer;

  row = 0;
  preset = 2;
  /* FAKEMATCH: the one-pass block keeps the state-address load after preset
   * setup and the state store before timer setup. Plain store: differ 8 or more
   * in nine statement orders. */
  do {
    gIwramHi_Data1 = 0;
  } while (0);
  timer = 20;

  for (;;) {
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
      AudioCommand_Play(MUS_019);
    if (gKeysRepeat & KEY_START)
      AudioCommand_Play(AUDIO_CUE_FADE_BGM);
    if (gKeysRepeat & KEY_SELECT)
      AudioCommand_Play(AUDIO_CUE_FADE_SLOT3);

    WaitFrames(1);
  }
}

/// Starts the sound driver, resets BGM ramps and clears all player cue records.
void Audio_InitializeRuntimeDefaults(void) {
  s16 *playerVolume;
  s32 remaining;

  Audio_Initialize();
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
  gEwram_Data1 = 0;
  remaining = MUSIC_PLAYER_COUNT - 1;
  do {
    remaining--;
    *playerVolume = 0;
    playerVolume += 1;
  } while (remaining >= 0);
}

/// Sets the BGM tempo scale directly; 0x100 preserves its native tempo.
void MusicCommand_SetPitchAndUpdateFrequency(u16 tempoScale) {
  MusicPlayer_SetPitchAndUpdateFrequency(&gMusicPlayerBgm, tempoScale);
}

/// Applies a pitch offset to every active BGM track.
void MusicCommand_SetPitch(u16 pitch) {
  MusicPlayer_SetPitch(&gMusicPlayerBgm, MUSIC_ALL_TRACKS, pitch);
}

/// Sets the target and per-frame step of the combined BGM tempo/pitch ramp.
void Audio_SetWorkPairB(u16 targetScale, u16 step) {
  gMusicPitchTarget = targetScale;
  gMusicPitchStep = step;
}

/// Sets BGM volume immediately and cancels any outstanding volume ramp.
void MusicCommand_SetVolume(s16 volume) {
  MusicPlayer_SetVolume(&gMusicPlayerBgm, MUSIC_ALL_TRACKS, volume);
  gMusicVolumeTarget = volume;
  gMusicVolume = volume;
}

/// Sets the BGM volume target and its per-frame adjustment.
void Audio_SetMusicVolumeTargetAndStep(u16 targetVolume, u16 step) {
  gMusicVolumeTarget = targetVolume;
  gMusicVolumeStep = step;
}

/// Returns the remaining delay before BGM can be restored after a fanfare.
s32 AudioCommand_GetStateByte(void) { return gMusicRestoreDelay; }

/// Stops every sound player while preserving its state for a later resume.
void AudioCommand_StopAllPlayers(void) { Audio_StopAllPlayers(); }

/// Resumes the sound players after AudioCommand_StopAllPlayers.
void AudioCommand_ResumeAllPlayers(void) { Audio_ResumeAllPlayers(); }

/// Replaces the audio command mask, or toggles its requested bits when flagged.
void AudioCommand_UpdateToggleMask(u32 command) {
  u32 toggle = command & AUDIO_COMMAND_TOGGLE;

  command &= AUDIO_COMMAND_MASK;
  if (toggle != 0)
    gAudio_CommandMask ^= command;
  else
    gAudio_CommandMask = command;
}

/// Returns the last BGM cue or fade state selected by AudioCommand_Play.
s32 AudioCommand_GetSecondaryStateByte(void) { return gAudioSecondaryState; }

/// Waits for fanfare restoration, giving up after 300 frames.
void AudioCommand_WaitForCompletion(void) {
  s32 framesWaited = 0;

  do {
    if (gMusicRestoreDelay == 0)
      break;
    WaitFrames(1);
    framesWaited++;
  } while (framesWaited < MUSIC_WAIT_TIMEOUT_FRAMES);
}

/// Returns the mixer preset chosen by the cue dispatcher.
s32 AudioCommand_GetWidth(s32 command) {
  if (command == MUS_070 || command == MUS_075 || command == MUS_067)
    return 3;
  return 2;
}
