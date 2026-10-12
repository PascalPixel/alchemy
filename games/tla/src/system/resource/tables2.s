	.section .rodata
@ 色変換表とバイト置換表の二つ目の写し (ResourceSlot_ConversionTables から
@ Data_080209a8 の終わりまでと同じ並び)
	.global Data_080c89cc
Data_080c89cc:
	.incbin "common/system/resource/colconv0.table"
	.incbin "common/system/resource/colconv1.table"
	.incbin "common/system/resource/colconv2.table"
	.incbin "common/system/resource/colconv3.table"
	.incbin "common/system/resource/colconv4.table"
	.incbin "common/field/common/remap0.table"
	.incbin "common/field/common/remap1.table"
	.incbin "common/field/common/remap2.table"
	.incbin "common/field/common/remap3.table"
	.incbin "field/common/remap4.table"
