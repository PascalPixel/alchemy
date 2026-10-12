	.section .rodata
@ The Broken Seal's ResourceSlot_ConversionTables, byte for byte
	.global ResourceSlot_ConversionTables
ResourceSlot_ConversionTables:
	.incbin "common/system/resource/colconv0.table"
	.incbin "common/system/resource/colconv1.table"
	.incbin "common/system/resource/colconv2.table"
	.incbin "common/system/resource/colconv3.table"
	.incbin "common/system/resource/colconv4.table"
