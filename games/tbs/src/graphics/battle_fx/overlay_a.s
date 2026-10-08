	.section .rodata
	.global BattleFx_OverlayImageA
BattleFx_OverlayImageA:
	.incbin "graphics/battle_fx/overlay_a.parts.lz"
	.byte 0x88	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x43b
