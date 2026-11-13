/// @file
/// The game's own sound code over the sound driver, as The Broken Seal's:
/// Sound_Play, which takes every sound number, the music's volume and tempo
/// ramps, and the music's return after a jingle.

#include "types.h"
#include "sound_driver_symbols.h"

enum {
  /// Full volume, and the song's own tempo and pitch: 1.0 in the ramps' 8.8
  /// fixed point.
  BGM_UNITY_SCALE = 256,
  /// A ramp's step a VBlank: a full fade in or out takes 64 VBlanks.
  BGM_DEFAULT_RAMP_STEP = 4,
  /// The track mask of every music command here: tracks 0 to 7.
  TRACKS_ALL = 0xff,
  MUSIC_PLAYER_COUNT = 8, ///< gMusicPlayerLastSound's entries, one a player.
  /// VBlanks after a jingle starts before the music waits for it to end.
  BGM_RESTORE_DELAY_VBLANKS = 10,
  /// Sound_WaitForBgmRestore gives up after this many frames, about
  /// five seconds.
  BGM_WAIT_TIMEOUT_FRAMES = 300
};

extern u16 gCurrentBgm;
extern u16 gBgmTempoPitchTarget;
extern u16 gBgmTempoPitch;
extern u16 gBgmTempoPitchStep;
extern u8 gBgmFadeRequested;
extern u8 gSound_UnreadByte;
extern u16 gMusicPlayerLastSound[MUSIC_PLAYER_COUNT];

void m4aSoundInit(void);
void Sound_SetFixedSampleFreq(u32 mode);

void Func_081c0c18(void) {}

/// Starts the sound driver, resets the BGM ramps (gBgmVolume and its kin),
/// clears every player's last sound and sets the fixed output rate. As The
/// Broken Seal's, but no music is sound 0 here.
void Sound_Initialize(void) {
  u16 *playerVolume;
  s32 remaining;

  m4aSoundInit();
  gCurrentBgm = 0;
  gBgmRestoreDelay = 0;
  gBgmVolumeTarget = BGM_UNITY_SCALE;
  gBgmVolume = BGM_UNITY_SCALE;
  gBgmVolumeStep = BGM_DEFAULT_RAMP_STEP;
  gBgmTempoPitchTarget = BGM_UNITY_SCALE;
  gBgmTempoPitch = BGM_UNITY_SCALE;
  gBgmTempoPitchStep = BGM_DEFAULT_RAMP_STEP;
  gBgmFadeRequested = 0;
  gSoundCommandMask = 0;
  // XXX (no evidence): what gSound_UnreadByte holds; no code in either game
  // reads it or its Broken Seal twin.
  gSound_UnreadByte = 0;
  playerVolume = gMusicPlayerLastSound;
  remaining = MUSIC_PLAYER_COUNT - 1;
  do {
    remaining--;
    *playerVolume = 0;
    playerVolume += 1;
  } while (remaining >= 0);
  Sound_SetFixedSampleFreq(0x90000);
}

#include "sound_driver.h"

extern struct SoundPlayer gMPlayInfo_ActionSE;
extern struct SoundPlayer gMPlayInfo_SE5;
extern struct SoundPlayer gMPlayInfo_Jingle;

void SoundFlag_Set(s32 sound);
void m4aSongNumStart(u16 sound);
void Sound_LoadReverbCode(s32 code);

/// Returns the reverb code Sound_Play gives music `sound`: SOUND_REVERB_LONG
/// for every one, where The Broken Seal's gives it to three.
s32 Sound_GetReverbCode(s32 sound) { return SOUND_REVERB_LONG; }

/// Plays a sound number, the game's one way of playing a sound: a fade control,
/// a sound effect, a jingle or music, as The Broken Seal's with this game's
/// ranges (enum SoundControl). Every sound is first marked as heard
/// (SoundFlag_Set), and none plays while the frame wait runs the driver's
/// update.
void Sound_Play(s32 id) {
  s32 flags;
  s32 player;

  SoundFlag_Set(id);
  flags = id & SOUND_FLAGS_MASK;
  if (gSoundVBlankState.updating != 0)
    return;
  id &= SOUND_NUM_MASK;

  if (id == SOUND_FADE_OUT_BGM) {
    // Fade the music out, once until the next music starts: 16 steps 7
    // VBlanks apart, 112 VBlanks.
    if (gBgmFadeRequested != 0)
      return;
    m4aMPlayFadeOut(&gMPlayInfo_BGM, 7);
    gBgmFadeRequested++;
    gCurrentBgm = 0;
  } else if (id == SOUND_FADE_OUT_ACTION_SE) {
    gMusicPlayerLastSound[MUSIC_PLAYER_ACTION_SE] = 0;
    m4aMPlayFadeOut(&gMPlayInfo_ActionSE, 3);
    gMusicPlayerLastSound[MUSIC_PLAYER_SE5] = 0;
    m4aMPlayFadeOut(&gMPlayInfo_SE5, 3);
  } else if (id == SOUND_FADE_OUT_SE5) {
    gMusicPlayerLastSound[MUSIC_PLAYER_SE5] = 0;
    m4aMPlayFadeOut(&gMPlayInfo_SE5, 3);
  } else if (id >= SOUND_SE_END) {
    goto background;
  } else if (id >= SOUND_SE_FIRST) {
    // A sound on MUSIC_PLAYER_SE_LAST borrows a free effect player,
    // searching back to MUSIC_PLAYER_SE_FIRST; with all four busy,
    // MUSIC_PLAYER_SE_LAST's sound is cut off.
    player = gSongTable[id].slot;
    if (player == MUSIC_PLAYER_SE_LAST) {
    next:
      if (((u8 *)&gSound_PlayerSlots[player].player->status)[0] != 0) {
        player--;
        if (player >= MUSIC_PLAYER_SE_FIRST)
          goto next;
        player = MUSIC_PLAYER_SE_LAST;
      }
    }
    MPlayStart(gSound_PlayerSlots[player].player, gSongTable[id].header);
    gMusicPlayerLastSound[player] = id;
  } else if (id >= SOUND_JINGLE_FIRST) {
    // A jingle, on its own player: the music's tracks 0 to 7 go silent at
    // once but keep playing, and fade back in once the jingle has ended
    // (Sound_UpdateBgmRamps).
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0);
    gBgmVolumeTarget = 0;
    gBgmVolume = 0;
    m4aSongNumStart(id);
    gBgmRestoreDelay = BGM_RESTORE_DELAY_VBLANKS;
  } else {
    // Music: the keep control, or the music last chosen, changes nothing.
    if (id == SOUND_KEEP_BGM)
      return;
  background:
    if (id == gCurrentBgm)
      return;
    gCurrentBgm = id;
    Sound_LoadReverbCode(Sound_GetReverbCode(id));
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
/// ramps once per VBlank, clamping each ramp when it crosses its target, as
/// The Broken Seal's without the DMA restart count (Sound_VSync here).
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
    if (delta > 0)
      gBgmVolume += gBgmVolumeStep;
    else
      gBgmVolume -= gBgmVolumeStep;
    if ((((s16)gBgmVolumeTarget - (s16)gBgmVolume) ^ delta) < 0)
      gBgmVolume = gBgmVolumeTarget;
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, gBgmVolume);
  }
  if ((s16)gBgmTempoPitchTarget != (s16)gBgmTempoPitch) {
    delta = (s16)gBgmTempoPitchTarget - (s16)gBgmTempoPitch;
    if (delta > 0)
      gBgmTempoPitch += gBgmTempoPitchStep;
    else
      gBgmTempoPitch -= gBgmTempoPitchStep;
    if ((((s16)gBgmTempoPitchTarget - (s16)gBgmTempoPitch) ^ delta) < 0)
      gBgmTempoPitch = gBgmTempoPitchTarget;
    // The tempo scale and the pitch move together: 12 semitones for each 256
    // of scale (pitch is in 1/256 semitone).
    m4aMPlayTempoControl(&gMPlayInfo_BGM, gBgmTempoPitch);
    m4aMPlayPitchControl(&gMPlayInfo_BGM, TRACKS_ALL,
                         (s16)gBgmTempoPitch * 12 - 12 * BGM_UNITY_SCALE);
  }
}

void SoundDriver_Update(void);

/// Runs the sound driver's VBlank work: the DMA restart count The Broken
/// Seal's Sound_UpdateBgmRamps ran at its end. Reached through the far-call
/// slot that routine had.
void Sound_VSync(void) { SoundDriver_Update(); }

/// Sets the BGM tempo scale directly; BGM_UNITY_SCALE preserves its native
/// tempo.
void Sound_SetBgmTempo(u16 tempoScale) {
  m4aMPlayTempoControl(&gMPlayInfo_BGM, tempoScale);
}

/// Applies a pitch offset, in 1/256 semitone, to the music's tracks 0 to 7.
void Sound_SetBgmPitch(u16 pitch) {
  m4aMPlayPitchControl(&gMPlayInfo_BGM, TRACKS_ALL, pitch);
}

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"

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

/// Stops every sound player while preserving its state for a later resume.
void Sound_StopAllMusicPlayers(void) { m4aMPlayAllStop(); }

/// Resumes every player, each from where m4aMPlayAllStop stopped it.
void Sound_ResumeAllMusicPlayers(void) { m4aMPlayAllContinue(); }

/// Returns nonzero while a jingle holds the music: from its start until its
/// player falls silent and the music begins to fade back in.
u8 Sound_GetBgmRestoreDelay(void) { return gBgmRestoreDelay; }

/// Waits a frame at a time for jingle restoration, giving up after 300
/// frames.
void Sound_WaitForBgmRestore(void) {
  s32 framesWaited = 0;

  do {
    if (gBgmRestoreDelay == 0)
      break;
    wait(1);
    framesWaited++;
  } while (framesWaited < BGM_WAIT_TIMEOUT_FRAMES);
}

/// Stops the sound DMA and its timer; the soft reset calls it first.
void Sound_SuspendDirectSound(void) { m4aSoundVSyncOff(); }
