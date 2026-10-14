	.section .rodata
	.global Data_080b1f2c
Data_080b1f2c:
	.byte 15, 16, 10, 11, 12
	.byte 1, 8, 15, 16, 17
	.byte 5, 6, 7, 8, 9
	.byte 0, 1, 2, 3, 4
@ 品物ごとの数え番号 + 1 (0 は数えない)。Item_AdjustCounter が品物番号で引く、512 個
	.global Data_080b1f40
Data_080b1f40:
	.incbin "games/tla/src/game/inventory/cntidx.table"
	.global Data_080b2340
Data_080b2340:
	.4byte 0, 1, 2, 3, 4, 5, 6, 7, -1
