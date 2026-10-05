@ Far-call stub table at 0x081c0040: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Audio_SetMusicVolumeTargetAndStepFar
	.thumb_func
Audio_SetMusicVolumeTargetAndStepFar:
	overlay_veneer Audio_SetMusicVolumeTargetAndStep
	.global Audio_Check
	.thumb_func
Audio_Check:
	overlay_veneer AudioCommand_GetStateByte
	.global Func_081c0050
	.thumb_func
Func_081c0050:
	overlay_veneer Func_081c1014
	.global AudioCommand_StopAllPlayersFar
	.thumb_func
AudioCommand_StopAllPlayersFar:
	overlay_veneer AudioCommand_StopAllPlayers
	.global AudioCommand_ResumeAllPlayersFar
	.thumb_func
AudioCommand_ResumeAllPlayersFar:
	overlay_veneer AudioCommand_ResumeAllPlayers
	.global AudioCommand_UpdateToggleMaskFar
	.thumb_func
AudioCommand_UpdateToggleMaskFar:
	overlay_veneer AudioCommand_UpdateToggleMask
