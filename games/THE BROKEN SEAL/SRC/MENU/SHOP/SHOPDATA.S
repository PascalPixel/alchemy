	.section .rodata
	.balign 4
@ 選択枠のタイル位置
	.global Shop_SelectorTileOffsets
Shop_SelectorTileOffsets:
	.2byte 0x00, 0x01, 0x02, 0x03
	.2byte 0x20, 0x21, 0x22, 0x23
	.2byte 0x40, 0x41, 0x42, 0x43
	.2byte 0x60, 0x61, 0x62, 0x63
	.2byte 0x80, 0x81, 0x82, 0x83
	.2byte 0xa0, 0xa1, 0xa2, 0xa3
	.2byte 0xc0, 0xc1, 0xc2, 0xc3
	.2byte 0xe0, 0xe1
@ 数字の転送先
	.global Shop_GlyphOffsets
Shop_GlyphOffsets:
	.2byte 0xc0, 0x90, 0x80, 0x50, 0x40
@ 贈り物の値段
	.global Shop_SpecialItemPrices
Shop_SpecialItemPrices:
	.2byte 300, 300, 300, 300, 300
	.2byte 500, 500, 500, 500, 500
	.2byte 1000, 1000, 1000, 1000, 1000
	.2byte 2000, 2000, 2000, 2000, 2000
	.2byte 4000, 4000, 4000, 4000, 4000
	.2byte 8000, 8000, 8000, 8000, 8000
	.global Shop_GiftItemIds
Shop_GiftItemIds:
	.2byte 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
	.2byte 16, 195, 38, 84, 89, 90, 180, 181, 190, 0
@ 店ごとの品物 (0 で終わり), 特別な品物, 種類
	.global Shop_StockTable
Shop_StockTable:
	.incbin "MENU/SHOP/STOCK.table"
@ 仲間の動作ごとの効果音
	.global Shop_PartyActionCues
Shop_PartyActionCues:
	.byte 0x52, 0x54, 0x58, 0x57
@ 宿代の倍率
	.global Inn_PriceMultipliers
Inn_PriceMultipliers:
	.byte 2, 4, 7, 8, 8, 10, 12, 15, 20, 0, 18, 25
