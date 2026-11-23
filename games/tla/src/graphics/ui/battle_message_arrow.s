@ The battle message's advance arrow: a second copy of page_arrow.png's sprite
@ sheet, linked among the battle's objects.
	.section .rodata
	.global gBattleMessage_AdvanceArrowTiles
gBattleMessage_AdvanceArrowTiles:
	.incbin "games/tla/src/graphics/ui/page_arrow.8bpp8x16"
