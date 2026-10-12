@ Palettes, a packed 4bpp sheet and a packed table of 64 x 64 halfwords whose
@ meaning is not established; the sheet is drawn with the first palette row.
	.section .rodata
	.global Data_020020bc
Data_020020bc:
	.incbin "field/overlay_64c/pal_a.gbapal"
	.global Data_0200213c
Data_0200213c:
	.incbin "field/overlay_64c/pal_b.gbapal"
	.global Data_0200215c
Data_0200215c:
	.incbin "field/overlay_64c/pal_c.gbapal"
	.global Data_0200217c
Data_0200217c:
	.incbin "field/overlay_64c/tiles.4bpp.lz"
	.global Data_02002469
Data_02002469:
	.incbin "field/overlay_64c/cells.table.lz"
