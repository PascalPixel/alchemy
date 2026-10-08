.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer WorldMap_EnterScene
	overlay_veneer WorldMap_GetEntrances
	overlay_veneer WorldMap_GetExits
	overlay_veneer StoryScene_SelectPlacementTable
	overlay_veneer WorldMap_GetEvents
	overlay_veneer WorldMap_GetRegions
