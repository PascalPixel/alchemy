	.section .rodata
	.global gSpriteColorTables
gSpriteColorTables:
	.incbin "games/tbs/src/system/resource/colconv0.table"
	.incbin "games/tbs/src/system/resource/colconv1.table"
	.incbin "games/tbs/src/system/resource/colconv2.table"
	.incbin "games/tbs/src/system/resource/colconv3.table"
	.incbin "games/tbs/src/system/resource/colconv4.table"
