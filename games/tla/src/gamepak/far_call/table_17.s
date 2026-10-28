.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global BattleLayout_HighlightPartyPanelsFar
	.thumb_func
BattleLayout_HighlightPartyPanelsFar:
	overlay_veneer BattleLayout_HighlightPartyPanels
	.global Icon_BuildItemIconTilesFar
	.thumb_func
Icon_BuildItemIconTilesFar:
	overlay_veneer Icon_BuildItemIconTiles
	.global Icon_BuildAbilityIconTilesFar
	.thumb_func
Icon_BuildAbilityIconTilesFar:
	overlay_veneer Icon_BuildAbilityIconTiles
