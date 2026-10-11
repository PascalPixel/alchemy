@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	import Engine_MapCopyCellAttributes, Map_CopyCellAttributeRectFar
	import Engine_ActorSetSpriteFlags, ObjectDispatch_SetSingleChildFlagsFar
	import Engine_GameFlagIsSet, GameFlag_TestFar
	import Object_GetById, Object_GetByIdFar
	import Engine_ActorSetPosition, ObjectMotion_SetHorizontalPositionWithTerrainFar
	import Engine_ActorSetAnimation, Object_SetModeByIdFar
