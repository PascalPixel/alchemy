.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gGabombaStatue_PathDirections
gGabombaStatue_PathDirections:
	.space 0x00000020
	.global gGabombaStatue_PathXs
gGabombaStatue_PathXs:
	.space 0x00000020
	.global gGabombaStatue_PathZs
gGabombaStatue_PathZs:
	.space 0x00000020
	.global gGabombaStatue_PathKinds
gGabombaStatue_PathKinds:
	.space 0x00000020
	.global gGabombaStatue_PathTried
gGabombaStatue_PathTried:
	.space 0x00000020
	.global gGabombaStatue_PathFlags
gGabombaStatue_PathFlags:
