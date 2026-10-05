.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_081c0070
	.thumb_func
Func_081c0070:
	overlay_veneer Audio_GetBackgroundCue
	.global AudioCommand_WaitForCompletionFar
	.thumb_func
AudioCommand_WaitForCompletionFar:
	overlay_veneer AudioCommand_WaitForCompletion
