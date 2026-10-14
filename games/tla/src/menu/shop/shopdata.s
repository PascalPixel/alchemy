	.section .rodata
@ 贈り物の品物 (The Broken Seal の Shop_GiftItemIds と同じ)
	.global Data_0810c3ca
Data_0810c3ca:
	.2byte 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
	.2byte 16, 195, 38, 84, 89, 90, 180, 181, 190, 0
@ 店ごとの品物 (0 で終わり), 特別な品物, 種類 (The Broken Seal の Shop_StockTable と同じ形、32 店)
	.global Data_0810c3f4
Data_0810c3f4:
	.incbin "games/tla/src/menu/shop/stock.table"
@ 素材ごとの行: 品物, 種類, 出来る品物 8 個, その確率 8 個 (合計 100)。品物 -1 で終わり
	.global Data_0810cc34
Data_0810cc34:
	.incbin "games/tla/src/menu/shop/forge.table"
@ 仲間の動作ごとの効果音 (The Broken Seal の Shop_PartyActionCues と同じ)
	.global Data_0810cf28
Data_0810cf28:
	.byte 0x52, 0x54, 0x58, 0x57
@ 宿代の倍率 (The Broken Seal の Inn_PriceMultipliers と同じ形)
	.global Data_0810cf2c
Data_0810cf2c:
	.byte 2, 4, 7, 8, 8, 10, 12, 16, 14, 15, 18, 20, 20, 22, 25, 0
