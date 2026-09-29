@ Summon_RobedGoddessTiles: 8-bit sprite tiles in a column 2108 pixels tall, their 256-colour
@ palette and then the tiles as a packed 8-bit bitmap.
	.section .rodata
	.global Summon_RobedGoddessTiles
Summon_RobedGoddessTiles:
	.incbin "GRAPHICS/SUMMON/ROBED_GODDESS.gbapal"
	.incbin "GRAPHICS/SUMMON/ROBED_GODDESS.bitmap.lz"
