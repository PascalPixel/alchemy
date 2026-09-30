@ Far-call stub table at 0x081c0000: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Resource_FarCall010
Resource_FarCall010:
	.global AudioEngine_RunMixerFar
	.thumb_func
AudioEngine_RunMixerFar:
	overlay_veneer AudioEngine_RunMixer
	.global Func_081c0008
	.thumb_func
Func_081c0008:
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
	.global Func_081c0030
	.thumb_func
Func_081c0030:
	overlay_veneer Func_081c0f70
	.global MusicCommand_SetVolumeFar
	.thumb_func
MusicCommand_SetVolumeFar:
	overlay_veneer MusicCommand_SetVolume
