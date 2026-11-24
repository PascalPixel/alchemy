@ Each music player's work area, its tracks and how many it has.
	.macro music_player player:req, tracks:req, count:req
	.4byte \player, \tracks
	.byte \count, 0
	.2byte 0
	.endm

	.section .rodata
	.balign 4
	.global gMPlayTable
gMPlayTable:
	music_player gMPlayInfo_Jingle, gMPlayTrack_Jingle, 4
	music_player gMPlayInfo_BGM, gMPlayTrack_BGM, 10
	music_player gMPlayInfo_MenuSE, gMPlayTrack_MenuSE, 2
	music_player gMPlayInfo_ActionSE, gMPlayTrack_ActionSE, 2
	music_player gMPlayInfo_SE4, gMPlayTrack_SE4, 2
	music_player gMPlayInfo_SE5, gMPlayTrack_SE5, 2
	music_player gMPlayInfo_SE6, gMPlayTrack_SE6, 2
	music_player gMPlayInfo_SE7, gMPlayTrack_SE7, 2

@ The player count is this table's row count; the mixer has no lines.
	.global gNumMusicPlayers
	.equ gNumMusicPlayers, (. - gMPlayTable) / 12
	.global gMaxLines
	.equ gMaxLines, 0
