	.section .rodata
@ A copy of The Broken Seal's sSelectorTileOffsets
	.global gShop_SelectorOffsets
gShop_SelectorOffsets:
	.2byte 0x00, 0x01, 0x02, 0x03
	.2byte 0x20, 0x21, 0x22, 0x23
	.2byte 0x40, 0x41, 0x42, 0x43
	.2byte 0x60, 0x61, 0x62, 0x63
	.2byte 0x80, 0x81, 0x82, 0x83
	.2byte 0xa0, 0xa1, 0xa2, 0xa3
	.2byte 0xc0, 0xc1, 0xc2, 0xc3
	.2byte 0xe0, 0xe1
@ Where each digit of a price is drawn in its sprite, ones first
	.global gShop_GlyphRowOffsets
gShop_GlyphRowOffsets:
	.2byte 0xc0, 0x90, 0x80, 0x50, 0x40
@ A copy of The Broken Seal's sSpecialItemPrices
	.global sGameTicketCreditCosts
sGameTicketCreditCosts:
	.2byte 300, 300, 300, 300, 300
	.2byte 500, 500, 500, 500, 500
	.2byte 1000, 1000, 1000, 1000, 1000
	.2byte 2000, 2000, 2000, 2000, 2000
	.2byte 4000, 4000, 4000, 4000, 4000
	.2byte 8000, 8000, 8000, 8000, 8000
@ Gift items (same as The Broken Seal's gShop_GiftItemIds).
	.global gShop_GiftItemIds
gShop_GiftItemIds:
	.2byte 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
	.2byte 16, 195, 38, 84, 89, 90, 180, 181, 190, 0
@ Stock (0-terminated), special items, and type for 32 shops; like TBS's sStockTable.
	.global sStockTable
sStockTable:
	.incbin "games/tla/src/shop/stock.table"
@ Material rows: item, type, 8 results and 8 chances totaling 100; item -1 ends.
	.global gData_0810cc34
gData_0810cc34:
	.incbin "games/tla/src/shop/forge.table"
@ Party action sound effects (same as The Broken Seal's sPartyActionCues).
	.global sSanctumServiceCues
sSanctumServiceCues:
	.byte 0x52, 0x54, 0x58, 0x57
@ Inn price multipliers (same layout as TBS's sInnPriceMultipliers).
	.global gData_0810cf2c
gData_0810cf2c:
	.byte 2, 4, 7, 8, 8, 10, 12, 16, 14, 15, 18, 20, 20, 22, 25, 0
