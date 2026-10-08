@ 名前入力の文字盤: 日本語版の名前入力 (NameEntry_KeyboardTilemap) と同じ
@ 圧縮タイルマップ。どのコードも参照しないが、全六版のこのオーバーレイに残る。
	.section .data
	.balign 4
	.global Clear_KeyboardTilemap
Clear_KeyboardTilemap:
	.incbin "graphics/window/keybd_ja.table.lz"
	.balign 4, 0
