.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall006
Resource_FarCall006:
	.global SceneSequence_RunFar
	.thumb_func
SceneSequence_RunFar:
	overlay_veneer SceneSequence_Run
