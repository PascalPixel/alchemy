@ The advance arrow beside a battle message: eight pictures of a 256-colour
@ sprite.
	.section .rodata
	.balign 4
	.global gBattleMessage_AdvanceArrowTiles
gBattleMessage_AdvanceArrowTiles:
	.incbin "games/tbs/src/battle/advance_arrow.8bpp8x16"
