	.include "games/common/include/game/ed_asm.h"
	.section .bss,"aw",%nobits
	.space 12
	.global KorimaPalette_First
KorimaPalette_First:
	.space 1792
	.global KorimaPalette_Second
KorimaPalette_Second:
	.space 896
