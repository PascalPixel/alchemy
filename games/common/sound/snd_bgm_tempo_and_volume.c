/* sySndSetBGMTempo to sySndSetBGMVolumeTarget, the same in both games. The
   Broken Seal's sound/sound.c and The Lost Age's sound/sound_play.c include
   it where the functions stand. */

/* Plays the music at a tempo scale at once, BGM_UNITY_SCALE its own; its pitch
   stays. In The Broken Seal the Lemurian ship slows it to fifteen sixteenths.
 */
void sySndSetBGMTempo(Uint16 tempoScale)
{
	m4aMPlayTempoControl(&MPlayInfoBGM, tempoScale);
}

/* Transposes the music's tracks 0 to 7 by `pitch`, in 1/256 semitone. In The
   Broken Seal only a bare stub, which no code names, leads here. */
void sySndSetBGMPitch(Uint16 pitch)
{
	m4aMPlayPitchControl(&MPlayInfoBGM, TRACKS_ALL, pitch);
}

/* Ramps the music's tempo and pitch together toward `tgtScale` (BGM_UNITY_SCALE
   its own) by `step` a VBlank. In The Broken Seal only a bare stub, which no
   code names, leads here. */
void sySndSetBGMTempoPitchTarget(Uint16 tgtScale, Uint16 step)
{
	BgmTempoPitchTarget = tgtScale;
	BgmTempoPitchStep = step;
}

/* Sets the music's volume at once (BGM_UNITY_SCALE full), ending any fade
   toward another. In The Broken Seal only a bare stub, which no code names,
   leads here. */
void sySndSetBGMVolume(Sint16 volume)
{
	m4aMPlayVolumeControl(&MPlayInfoBGM, TRACKS_ALL, volume);
	BgmVolumeTarget = volume;
	BgmVolume = volume;
}

/* Fades the music toward `tgtVolume` (BGM_UNITY_SCALE full) by `step` a VBlank.
   In The Broken Seal only a bare stub, which no code names, leads here. */
void sySndSetBGMVolumeTarget(Uint16 tgtVolume, Uint16 step)
{
	BgmVolumeTarget = tgtVolume;
	BgmVolumeStep = step;
}
