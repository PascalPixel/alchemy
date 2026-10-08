.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer FieldScene_InitializeActorGroups
	overlay_veneer Scene_GetEntrances
	overlay_veneer BabiChika_GetExits
	overlay_veneer SceneData_SelectAndApplyTableBySceneId
	overlay_veneer BabiChika_SelectEvents
	overlay_veneer Scene_GetRegions
