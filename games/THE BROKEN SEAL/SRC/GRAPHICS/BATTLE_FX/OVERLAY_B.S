@ BattleFx_OverlayImageB: 7 pictures (listed in OVERLAY_B.TSV) with no palette of their
@ own, drawn with BattleFx_BlueCrescentSheet's; joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_OverlayImageB
BattleFx_OverlayImageB:
	.incbin "GRAPHICS/BATTLE_FX/OVERLAY_B.parts.lz"
