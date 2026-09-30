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
	overlay_veneer Func_081c0c1c
	.global Audio_PlayCue
	.thumb_func
Audio_PlayCue:
	overlay_veneer Func_081c0cb0
	overlay_veneer Func_081c0f3c
	overlay_veneer Func_081c0f44
	overlay_veneer Func_081c0f58
	overlay_veneer Func_081c0f70
	overlay_veneer Func_081c0f84
