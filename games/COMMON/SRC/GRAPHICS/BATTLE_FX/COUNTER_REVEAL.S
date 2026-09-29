@ BattleFx_CounterRevealSheet: the 64-colour palette of its first picture, then its 3 pictures
@ (listed in COUNTER_REVEAL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_CounterRevealSheet
BattleFx_CounterRevealSheet:
	.incbin "GRAPHICS/BATTLE_FX/COUNTER_REVEAL_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/COUNTER_REVEAL.parts.lz"
