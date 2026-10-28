	.section .rodata
	.global BattleEffect_OverlayImageA
BattleEffect_OverlayImageA:
	.incbin "games/tla/src/graphics/battle_effects/overlay_a.parts.lz"
	.byte 0x88	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x43b
