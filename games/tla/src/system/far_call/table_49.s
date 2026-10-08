.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Audio_GetBackgroundCueFar
	.thumb_func
Audio_GetBackgroundCueFar:
	overlay_veneer Audio_GetBackgroundCue
	.global AudioCommand_WaitForCompletionFar
	.thumb_func
AudioCommand_WaitForCompletionFar:
	overlay_veneer AudioCommand_WaitForCompletion
