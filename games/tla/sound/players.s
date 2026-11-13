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
	music_player gMPlayInfo_Jingle, Sound_Tracks0, 4
	music_player gMPlayInfo_BGM, Sound_Tracks1, 10
	music_player gMPlayInfo_MenuSE, Sound_Tracks2, 2
	music_player gMPlayInfo_ActionSE, Sound_Tracks3, 2
	music_player gMPlayInfo_SE4, Sound_Tracks4, 2
	music_player gMPlayInfo_SE5, Sound_Tracks5, 2
	music_player gMPlayInfo_SE6, Sound_Tracks6, 2
	music_player gMPlayInfo_SE7, Sound_Tracks7, 2

@ The player count is this table's row count; the mixer has at most 0 lines
	.global Sound_PlayerCount
	.equ Sound_PlayerCount, (. - gSound_PlayerSlots) / 12
	.global Sound_MaxLines
	.equ Sound_MaxLines, 0
