	.section .rodata
	.balign 2
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
	.incbin "menu/shop/stock.table"
@ 仲間の動作ごとの効果音
	.global Shop_PartyActionCues
Shop_PartyActionCues:
	.byte 0x52, 0x54, 0x58, 0x57
@ 宿代の倍率
	.global Inn_PriceMultipliers
Inn_PriceMultipliers:
	.byte 2, 4, 7, 8, 8, 10, 12, 15, 20, 0, 18, 25
