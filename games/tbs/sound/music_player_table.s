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
	music_player MPlayInfoJingle, MPlayTrackJingle, 4
	music_player MPlayInfoBGM, MPlayTrackBGM, 10
	music_player MPlayInfoMenuSE, MPlayTrackMenuSE, 2
	music_player MPlayInfoActionSE, MPlayTrackActionSE, 2
	music_player MPlayInfoSE4, MPlayTrackSE4, 2
	music_player MPlayInfoSE5, MPlayTrackSE5, 2
	music_player MPlayInfoSE6, MPlayTrackSE6, 2
	music_player MPlayInfoSE7, MPlayTrackSE7, 2

@ The player count is this table's row count; the mixer has no lines.
	.global NumMusicPlayers
	.equ NumMusicPlayers, (. - MPlayTable) / 12
	.global MaxLines
	.equ MaxLines, 0
