.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.balign 16
	.global gPaletteCycleBright
gPaletteCycleBright:
	.space 0x00000004
	.global gPaletteCycleSlots
gPaletteCycleSlots:
	.space 0x00000004
	.global gPaletteCycleTimer
gPaletteCycleTimer:
	.space 0x00000004
	.global gPaletteCycleDelay
gPaletteCycleDelay:
	.space 0x00000004
	.global gPaletteSlots
gPaletteSlots:
	.space 0x00000004
	.global gPaletteColors
gPaletteColors:
	.space 0x00000004
	.global gPaletteCycleColors
gPaletteCycleColors:
	.space 0x00000004
	.global gPaletteCycleLength
gPaletteCycleLength:
	.space 0x00000004
	.global gPaletteCycleIndex
gPaletteCycleIndex:
	.space 0x00000010
	.global gContigo_SparkleWork
gContigo_SparkleWork:
