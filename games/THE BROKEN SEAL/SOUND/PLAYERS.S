	.macro music_player player:req, tracks:req, count:req
	.4byte \player, \tracks
	.byte \count, 0
	.2byte 0
	.endm

@ ミュージックプレイヤーごとの作業領域とトラック数
	.section .rodata
	.balign 4
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	music_player gMusicPlayerFanfare, Sound_Tracks0, 4
	music_player gMusicPlayerBgm, Sound_Tracks1, 10
	music_player Sound_Player2, Sound_Tracks2, 2
	music_player Sound_Player3, Sound_Tracks3, 2
	music_player Sound_Player4, Sound_Tracks4, 2
	music_player Sound_Player5, Sound_Tracks5, 2
	music_player Sound_Player6, Sound_Tracks6, 2
	music_player Sound_Player7, Sound_Tracks7, 2

@ プレイヤー数はこの表の行数、ミキサーの最大ライン数は 0
	.global Sound_PlayerCount
	.equ Sound_PlayerCount, (. - Sound_PlayerSlots) / 12
	.global Sound_MaxLines
	.equ Sound_MaxLines, 0
