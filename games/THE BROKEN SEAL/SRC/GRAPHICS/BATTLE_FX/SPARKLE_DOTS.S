@ BattleFx_SparkleDots: 9 pictures (listed in SPARKLE_DOTS.TSV) with no palette of their
@ own, drawn with BattleFx_IceShardSheet's; joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_SparkleDots
BattleFx_SparkleDots:
	.incbin "GRAPHICS/BATTLE_FX/SPARKLE_DOTS.parts.lz"
