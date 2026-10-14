	.section .rodata
@ The Broken Seal's ResourceSlot_ConversionTables, byte for byte
	.global ResourceSlot_ConversionTables
ResourceSlot_ConversionTables:
	.incbin "games/common/src/system/resource/colconv0.table"
	.incbin "games/common/src/system/resource/colconv1.table"
	.incbin "games/common/src/system/resource/colconv2.table"
	.incbin "games/common/src/system/resource/colconv3.table"
	.incbin "games/common/src/system/resource/colconv4.table"
