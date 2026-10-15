@ A 24 by 13 window tilemap holding a kana grid, in every edition, and the
@ table of cell numbers that follows it.
	.section .rodata
	.balign 4
	.global Data_02002504
Data_02002504:
	.incbin "games/tla/src/menu/clear/tilemap.table.lz"

	.global gData_0200265e
gData_0200265e:
	.ifdef TLA_EDITION_JA
	@ 20 cells a row; 99 is an empty cell
	.byte  0,  1,  2,  3,  4, 20, 21, 22, 23, 24, 38, 39, 40, 41, 42, 56, 99, 99, 57, 58
	.byte  5,  6,  7,  8,  9, 25, 26, 27, 28, 29, 43, 99, 45, 99, 44, 59, 60, 61, 62, 63
	.byte 10, 11, 12, 13, 14, 30, 31, 32, 33, 34, 46, 47, 48, 49, 50, 99, 99, 99, 99, 99
	.byte 15, 16, 17, 18, 19, 35, 99, 36, 99, 37, 51, 52, 53, 54, 55, 99, 99, 99, 99, 99
	.else
	.byte  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15
	.byte 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
	.byte 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47
	.byte 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63
	.endif
