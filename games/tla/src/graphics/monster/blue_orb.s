	.section .rodata
	.global Monster_BlueOrbSprites
Monster_BlueOrbSprites:
	.incbin "games/tla/src/graphics/monster/blue_orb.frames.lz"
	.byte 0x0a	@ PACKER: left in the buffer by Monster_GreenMageSprites+0x108b
