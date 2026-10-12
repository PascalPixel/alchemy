	.section .rodata
	.balign 2
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
