@ A 16-colour palette and one packed stream of 3,040 bytes. The game copies the
@ palette to OBJ palette 12 and the stream's first 2,560 bytes to video memory
@ as five 32x32 sprites of 16 colours (golden_scales.png). The last 480 bytes
@ are zero.
	.section .rodata
	.global Summon_GoldenScalesTiles
Summon_GoldenScalesTiles:
	.incbin "graphics/summon/golden_scales.gbapal"
	.incbin "graphics/summon/golden_scales.parts.lz"
