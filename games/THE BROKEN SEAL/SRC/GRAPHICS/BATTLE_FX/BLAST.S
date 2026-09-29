@ BattleFx_BlastSheet: the 64-colour palette of its first picture, then its 31 pictures
@ (listed in BLAST.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_BlastSheet
BattleFx_BlastSheet:
	.incbin "GRAPHICS/BATTLE_FX/BLAST_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLAST.parts.lz"
