@ The battle transition's curtain: its eight variants, then its tiles.
	.section .rodata
	.global gBattleTransition_CurtainVariants
gBattleTransition_CurtainVariants:
	.incbin "games/tbs/src/battle/transition/curtain_variants.4bpp"
	.global gBattleTransition_CurtainTiles
gBattleTransition_CurtainTiles:
	.incbin "games/tbs/src/battle/transition/curtain.4bpp"
