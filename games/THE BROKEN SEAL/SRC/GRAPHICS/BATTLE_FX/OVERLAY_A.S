	.section .rodata
	.global BattleFx_OverlayImageA
BattleFx_OverlayImageA:
	.incbin "GRAPHICS/BATTLE_FX/OVERLAY_A.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_FireBurstSheet at +0x43b */
	.byte 0x88
