.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FieldScene_InitializeActorGroups
	overlay_veneer Scene_GetEntrances
	overlay_veneer BabiChika_GetExits
	overlay_veneer SceneData_SelectAndApplyTableBySceneId
	overlay_veneer BabiChika_SelectEvents
	overlay_veneer Scene_GetRegions
