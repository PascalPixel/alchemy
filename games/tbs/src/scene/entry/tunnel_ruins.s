@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer FieldScene_InitializeActorGroups
	overlay_veneer Scene_GetEntrances
	overlay_veneer TunnelRuins_GetExits
	overlay_veneer SceneData_SelectAndApplyTableBySceneId
	overlay_veneer TunnelRuins_SelectEvents
	overlay_veneer Scene_GetRegions
