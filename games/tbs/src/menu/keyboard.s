@ 名前入力の文字盤: 日本語版の名前入力 (NameEntry_KeyboardTilemap) と同じ
@ 圧縮タイルマップ。どのコードも参照しないが、全六版のこのオーバーレイに残る。
	.section .rodata
	.balign 4
	.global Clear_KeyboardTilemap
Clear_KeyboardTilemap:
	.incbin "games/common/src/graphics/window/keyboard_ja.table.lz"
	.balign 4, 0
