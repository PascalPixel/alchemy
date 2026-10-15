	.section .rodata
@ The Broken Seal's gResourceSlot_ConversionTables, byte for byte
	.global gResourceSlot_ConversionTables
gResourceSlot_ConversionTables:
	.incbin "games/common/src/system/resource/colconv0.table"
	.incbin "games/common/src/system/resource/colconv1.table"
	.incbin "games/common/src/system/resource/colconv2.table"
	.incbin "games/common/src/system/resource/colconv3.table"
	.incbin "games/common/src/system/resource/colconv4.table"
