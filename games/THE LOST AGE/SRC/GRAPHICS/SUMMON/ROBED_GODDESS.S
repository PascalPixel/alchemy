@ Summon_RobedGoddessTiles: its 256-colour palette, then 16 8-bit sprites of
@ 32x32 and 480 zero bytes (ZERO_FILL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Summon_RobedGoddessTiles
Summon_RobedGoddessTiles:
	.incbin "GRAPHICS/SUMMON/ROBED_GODDESS.gbapal"
	.incbin "GRAPHICS/SUMMON/ROBED_GODDESS.parts.lz"
