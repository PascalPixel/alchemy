@ BattleFx_OverlayImageA: 3 pictures (listed in OVERLAY_A.TSV) with no palette of their
@ own, drawn with BattleFx_BlueCrescentSheet's; joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_OverlayImageA
BattleFx_OverlayImageA:
	.incbin "GRAPHICS/BATTLE_FX/OVERLAY_A.parts.lz"
