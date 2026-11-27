#ifndef ALCHEMY_SOUND_DRIVER_H
#define ALCHEMY_SOUND_DRIVER_H

/* The sound driver in one include: its types and registers
   (sound_driver_types.h), what each game sets for it (sdk_config.h), and its
   work, tables and functions, under the names pret gives them. */

#include "types.h"
#include "sdk/sound_driver_types.h"
#include "sdk_config.h"

/* The driver's work: its state, the CGB channels' notes and the bytes the songs
   share. The sound driver's work, which m4aSoundInit sets up through
   SoundInit. */
extern struct SoundInfo SoundInfoWork;
/* The CGB channels' notes, which SoundInit hands the driver
   (SoundInfo.cgbNotes). */
extern struct CgbChannel CgbChans[4];
/* The 16 work bytes every music player shares (MusicPlayerInfo.workBytes), so
   one sound can test what another has set (ply_memacc). */
extern u8 MPlayMemAccArea[];
/* The song commands' handlers and the driver's helpers, which SoundInit copies
   from MPlayJumpTableTemplate (MPLAY_JUMP_); the driver's core reaches it as
   RamMPlayJumpTable. */
extern MPlayFunc MPlayJumpTable[36];

/* The driver's tables: its music players, every sound by number, the functions
   of the extended commands, and its pitch and rate tables. Each music player's
   work area, its tracks and how many it has, by enum MusicPlayerId
   (sound/music_player_table.s). */
extern const struct MusicPlayer MPlayTable[];
/* Each sound's header and music player, by sound id (sound/song_table.s). */
extern const struct Song SongTable[];
/* The extended commands' handlers, by the byte after 0xCD (ply_xcmd). */
extern const MPlayFunc XcmdTable[];
extern const u8		   Cgb3Vol[];
extern const s16	   CgbFreqTable[];
extern const u8		   CgbScaleTable[];
extern const u16	   PcmSamplesPerVBlankTable[];
extern const u8		   NoiseTable[];
extern const u32	   FreqTable[];
extern const u8		   ScaleTable[];
/* The music player count, MPlayTable's rows, as this symbol's address
   (sound/music_player_table.s). */
extern char NumMusicPlayers[];
/* The mixer's line limit, 0 for none, as this symbol's address
   (sound/music_player_table.s). */
extern char MaxLines[];

/* What the driver calls outside it, which each game's system library provides:
   the BIOS's CpuSet and the compiler's signed division. */
void BiosCpuSet(const void *src, void *dst, u32 control);
s32	 __divsi3(s32 numerator, s32 denominator);

/* Starting the driver and running it each VBlank. */
void m4aSoundInit(void);
void SoundInit(struct SoundInfo *work);
void MPlayExtender(struct CgbChannel *notes);
void m4aSoundMode(u32 mode);
void SampleFreqSet(u32 mode);
void m4aSoundVSyncOn(void);
void m4aSoundVSyncOff(void);
/* Restarts the two sound DMAs at the start of the mix buffer once it has played
   through; called each VBlank. */
void m4aSoundVSync(void);
/* The sound driver's work each VBlank: steps the music players (the MPlayMain
   chain), then mixes the next buffer through SoundMainRAM in IWRAM. */
void SoundMain(void);
/* The mixer's code as the ROM holds it, which m4aSoundInit copies to IWRAM
   (RamSoundMixer) to run from there. */
void sySoundMainRAMRom(void);
/* The callback that does nothing, for the driver's hooks until they are set. */
void DummyFunc(void);
/* The rate a PCM sample plays at for a key and a fine pitch. */
s32 MidiKeyToFreq(struct WaveData *wave, u8 key, u8 pitch);
/* The high word of the 64-bit product of two words: a 32-bit fraction's share
   of a value, as MidiKeyToFreq takes the step between two keys. */
u32 umul3232H32(u32 left, u32 right);

/* Songs and music players. */
void m4aSongNumStart(u16 id);
void MPlayOpen(
	struct MusicPlayerInfo *player, struct MusicPlayerTrack *tracks, u8 cnt);
void MPlayStart(
	struct MusicPlayerInfo *player, const struct SongHeader *header);
void MPlayContinue(struct MusicPlayerInfo *player);
void MPlayFadeOut(struct MusicPlayerInfo *player, u16 period);
void FadeOutBody(struct MusicPlayerInfo *player);
void m4aMPlayStop(struct MusicPlayerInfo *player);
void m4aMPlayAllStop(void);
void m4aMPlayAllContinue(void);
void m4aMPlayFadeOut(struct MusicPlayerInfo *player, u16 period);
void m4aMPlayTempoControl(struct MusicPlayerInfo *player, u16 scale);
void m4aMPlayVolumeControl(
	struct MusicPlayerInfo *player, u16 mask, u16 volume);
void m4aMPlayPitchControl(struct MusicPlayerInfo *player, u16 mask, s16 pitch);
void m4aMPlayPanpotControl(struct MusicPlayerInfo *player, u16 mask, s8 pan);
void m4aMPlayModDepthSet(struct MusicPlayerInfo *player, s32 mask, s32 value);
void m4aMPlayLFOSpeedSet(struct MusicPlayerInfo *player, u32 mask, u32 value);
/* Advances one music player by its tempo's ticks for this VBlank, running the
   commands of each track that falls due; MPlayOpen links it into the chain. */
void MPlayMain(struct MusicPlayerInfo *player);

/* Tracks, notes and the CGB channels. */
void TrkVolPitSet(
	struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* Silences a track: every note still on its chain stops at once, a CGB channel
   switched off with it. */
void TrackStop(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ClearModM(struct MusicPlayerTrack *track);
void ClearChain(struct SoundChannel *note);
void Clear64byte(void *block);
/* Takes a note off its track's chain (ClearChain, through the jump table). */
void RealClearChain(struct SoundChannel *note);
/* Clears 64 bytes (Clear64byte, through the jump table). */
void SoundMainBTM(void *block);
/* Copies the driver's jump table, MPlayJumpTableTemplate, into table. */
void MPlayJumpTableCopy(MPlayFunc *tbl);
void CgbSound(void);
void CgbOscOff(u8 channel);
void CgbModVol(struct CgbChannel *note);
s32	 MidiKeyToCgbFreq(u8 kind, u8 key, u8 fine);

/* The song commands (MPlayJumpTableTemplate) and the extended commands
   (XcmdTable), each called with the music player and the track. A note command:
   keys a note on the track's voice for the command's length
   (SoundInfo.noteOn). */
void ply_note(u32 command, struct MusicPlayerInfo *player,
	struct MusicPlayerTrack *track);
/* FINE: ends the track. */
void ply_fine(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* GOTO: jumps to another place in the track. */
void ply_goto(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* PATT: plays a pattern, returning at its PEND. */
void ply_patt(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* PEND: returns from a pattern. */
void ply_pend(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* REPT: jumps back a number of times. */
void ply_rept(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* PRIO: the track's priority for a free channel. */
void ply_prio(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* TEMPO: the song's tempo; its byte is half the beats a minute. */
void ply_tempo(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* KEYSH: transposes the track, in semitones. */
void ply_keysh(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* VOICE: the track's instrument from the song's voice group. */
void ply_voice(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* VOL: the track's volume. */
void ply_vol(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* PAN: the track's place between the speakers. */
void ply_pan(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* BEND: the track's pitch bend. */
void ply_bend(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* BENDR: how far a full bend goes, in semitones. */
void ply_bendr(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* LFODL: how long a note plays before its vibrato starts. */
void ply_lfodl(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* MODT: what the vibrato moves: pitch, volume or pan. */
void ply_modt(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* TUNE: fine tuning, within a semitone. */
void ply_tune(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* PORT: writes a byte to a sound register. */
void ply_port(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* The versions of LFOS, MOD and EOT MPlayExtender installs: LFOS and MOD set
   the track's vibrato speed and depth, a 0 stopping the vibrato at once; EOT
   releases the track's tied note of the key the command names, or of its last
   key. */
void ply_lfos(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_mod(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_endtie(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* MEMACC, which MPlayExtender installs: works on the shared work bytes. */
void ply_memacc(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
/* XCMD, which MPlayExtender installs: runs the extended command the next byte
   names. */
void ply_xcmd(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xxx(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xwave(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xtype(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xatta(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xdeca(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xsust(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xrele(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xiecv(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xiecl(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xleng(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_xswee(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);

#endif
