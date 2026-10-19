	.macro music_player player:req, tracks:req, count:req
	.4byte \player, \tracks
	.byte \count, 0
	.2byte 0
	.endm

@ Each music player's work area, its tracks and how many it has
	.section .rodata
	.balign 4
	.global gSound_PlayerSlots
gSound_PlayerSlots:
	music_player gMusicPlayerFanfare, Sound_Tracks0, 4
	music_player gMusicPlayerBgm, Sound_Tracks1, 10
	music_player Sound_Player2, Sound_Tracks2, 2
	music_player gSound_Player3, Sound_Tracks3, 2
	music_player Sound_Player4, Sound_Tracks4, 2
	music_player gSound_Player5, Sound_Tracks5, 2
	music_player Sound_Player6, Sound_Tracks6, 2
	music_player Sound_Player7, Sound_Tracks7, 2

@ The player count is this table's row count; the mixer has at most 0 lines
	.global Sound_PlayerCount
	.equ Sound_PlayerCount, (. - gSound_PlayerSlots) / 12
	.global Sound_MaxLines
	.equ Sound_MaxLines, 0
