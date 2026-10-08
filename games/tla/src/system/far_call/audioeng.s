.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall010
Resource_FarCall010:
	.global AudioEngine_RunMixerFar
	.thumb_func
AudioEngine_RunMixerFar:
	overlay_veneer AudioEngine_RunMixer
	.global Audio_InitializeRuntimeDefaultsFar
	.thumb_func
Audio_InitializeRuntimeDefaultsFar:
	overlay_veneer Func_081c0c1c
	.global Audio_PlayCue
	.thumb_func
Audio_PlayCue:
	overlay_veneer Func_081c0cb0
	.global Func_081c0018
	.thumb_func
Func_081c0018:
	overlay_veneer Func_081c0f3c
	.global Func_081c0020
	.thumb_func
Func_081c0020:
	overlay_veneer Func_081c0f44
	.global MusicCommand_SetPitchFar
	.thumb_func
MusicCommand_SetPitchFar:
	overlay_veneer MusicCommand_SetPitch
	.global Audio_SetWorkPairBFar
	.thumb_func
Audio_SetWorkPairBFar:
	overlay_veneer Func_081c0f70
	.global MusicCommand_SetVolumeFar
	.thumb_func
MusicCommand_SetVolumeFar:
	overlay_veneer MusicCommand_SetVolume
