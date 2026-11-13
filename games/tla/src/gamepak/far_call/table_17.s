.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global PartyStatus_HighlightMembersFar
	.thumb_func
PartyStatus_HighlightMembersFar:
	overlay_veneer PartyStatus_HighlightMembers
	.global Icon_BuildItemIconTilesFar
	.thumb_func
Icon_BuildItemIconTilesFar:
	overlay_veneer Icon_BuildItemIconTiles
	.global Icon_BuildAbilityIconTilesFar
	.thumb_func
Icon_BuildAbilityIconTilesFar:
	overlay_veneer Icon_BuildAbilityIconTiles
