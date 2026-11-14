.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.space 0x00000002
	.global gAquaRock_Streams
gAquaRock_Streams:
	.space 0x00000020
	.global gAquaRock_SecondStream
gAquaRock_SecondStream:
	.space 0x00000080
	.global gAquaRock_CellMarkers
gAquaRock_CellMarkers:
