	.section .rodata
	.global Data_080b1f2c
Data_080b1f2c:
	.byte 15, 16, 10, 11, 12
	.byte 1, 8, 15, 16, 17
	.byte 5, 6, 7, 8, 9
	.byte 0, 1, 2, 3, 4
@ 512 counter indices plus one (0 means uncounted); Item_AdjustCounter indexes by item.
	.global gData_080b1f40
gData_080b1f40:
	.incbin "games/tla/src/game/inventory/cntidx.table"
	.global gData_080b2340
gData_080b2340:
	.4byte 0, 1, 2, 3, 4, 5, 6, 7, -1
