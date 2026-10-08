	.section .rodata
	.global BattleFx_OverlayImageA
BattleFx_OverlayImageA:
	.incbin "GRAPHICS/BATTLE_FX/OVERLAY_A.parts.lz"
	.byte 0x88	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x43b
