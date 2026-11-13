.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gKandoreanTemple_SteamCounter
gKandoreanTemple_SteamCounter:
	.space 0x00000004
	.global gKandoreanTemple_LeftVentBlowing
gKandoreanTemple_LeftVentBlowing:
	.space 0x00000004
	.global gKandoreanTemple_RightVentBlowing
gKandoreanTemple_RightVentBlowing:
	.space 0x00000004
	.global gKandoreanTemple_LeftTimer
gKandoreanTemple_LeftTimer:
	.space 0x00000004
	.global gKandoreanTemple_RightTimer
gKandoreanTemple_RightTimer:
	.space 0x00000004
	.global gKandoreanTemple_LeftBobPhase
gKandoreanTemple_LeftBobPhase:
	.space 0x00000002
	.global gKandoreanTemple_RightBobPhase
gKandoreanTemple_RightBobPhase:
	.space 0x00000002
	.global gKandoreanTemple_LashPebble
gKandoreanTemple_LashPebble:
