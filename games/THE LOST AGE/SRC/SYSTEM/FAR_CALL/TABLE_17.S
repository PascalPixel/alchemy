.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global BattleLayout_HighlightPartyPanelsFar
	.thumb_func
BattleLayout_HighlightPartyPanelsFar:
	overlay_veneer BattleLayout_HighlightPartyPanels
	.global Func_080382a8
	.thumb_func
Func_080382a8:
	overlay_veneer Func_0803d5c4
	.global UiIcon_BuildAbilityIconTilesFar
	.thumb_func
UiIcon_BuildAbilityIconTilesFar:
	overlay_veneer UiIcon_BuildAbilityIconTiles
