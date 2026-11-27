@ Each music player's work area, its tracks and how many it has.
	.macro music_player player:req, tracks:req, count:req
	.4byte \player, \tracks
	.byte \count, 0
	.2byte 0
	.endm

	.section .rodata
	.balign 4
	.global MPlayTable
MPlayTable:
	music_player MPlayInfo_Jingle, MPlayTrack_Jingle, 4
	music_player MPlayInfo_BGM, MPlayTrack_BGM, 10
	music_player MPlayInfo_MenuSE, MPlayTrack_MenuSE, 2
	music_player MPlayInfo_ActionSE, MPlayTrack_ActionSE, 2
	music_player MPlayInfo_SE4, MPlayTrack_SE4, 2
	music_player MPlayInfo_SE5, MPlayTrack_SE5, 2
	music_player MPlayInfo_SE6, MPlayTrack_SE6, 2
	music_player MPlayInfo_SE7, MPlayTrack_SE7, 2

@ The player count is this table's row count; the mixer has no lines.
	.global NumMusicPlayers
	.equ NumMusicPlayers, (. - MPlayTable) / 12
	.global MaxLines
	.equ MaxLines, 0
