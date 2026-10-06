.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Audio_GetBackgroundCueFar
	.thumb_func
Audio_GetBackgroundCueFar:
	overlay_veneer Audio_GetBackgroundCue
	.global AudioCommand_WaitForCompletionFar
	.thumb_func
AudioCommand_WaitForCompletionFar:
	overlay_veneer AudioCommand_WaitForCompletion
