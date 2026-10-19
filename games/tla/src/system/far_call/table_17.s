.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global BattleLayout_HighlightPartyPanelsFar
	.thumb_func
BattleLayout_HighlightPartyPanelsFar:
	overlay_veneer BattleLayout_HighlightPartyPanels
	.global UiIcon_BuildItemIconTilesFar
	.thumb_func
UiIcon_BuildItemIconTilesFar:
	overlay_veneer UiIcon_BuildItemIconTiles
	.global UiIcon_BuildAbilityIconTilesFar
	.thumb_func
UiIcon_BuildAbilityIconTilesFar:
	overlay_veneer UiIcon_BuildAbilityIconTiles
