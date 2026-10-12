	.section .rodata
	.balign 2
@ 店ごとの品物 (0 で終わり), 特別な品物, 種類
	.global Shop_StockTable
Shop_StockTable:
	.incbin "menu/shop/stock.table"
