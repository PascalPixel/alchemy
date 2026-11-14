.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.space 0x00000004
	.global gKibombo_FireColors
gKibombo_FireColors:
	.space 0x00000008
	.global gKibombo_BlackCrystal
gKibombo_BlackCrystal:
