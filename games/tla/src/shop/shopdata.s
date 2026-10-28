	.section .rodata
@ Gift items (same as The Broken Seal's gShop_GiftItemIds).
	.global Data_0810c3ca
Data_0810c3ca:
	.2byte 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
	.2byte 16, 195, 38, 84, 89, 90, 180, 181, 190, 0
@ Stock (0-terminated), special items, and type for 32 shops; like TBS's sStockTable.
	.global gData_0810c3f4
gData_0810c3f4:
	.incbin "games/tla/src/shop/stock.table"
@ Material rows: item, type, 8 results and 8 chances totaling 100; item -1 ends.
	.global gData_0810cc34
gData_0810cc34:
	.incbin "games/tla/src/shop/forge.table"
@ Party action sound effects (same as The Broken Seal's sPartyActionCues).
	.global Data_0810cf28
Data_0810cf28:
	.byte 0x52, 0x54, 0x58, 0x57
@ Inn price multipliers (same layout as TBS's sInnPriceMultipliers).
	.global gData_0810cf2c
gData_0810cf2c:
	.byte 2, 4, 7, 8, 8, 10, 12, 16, 14, 15, 18, 20, 20, 22, 25, 0
