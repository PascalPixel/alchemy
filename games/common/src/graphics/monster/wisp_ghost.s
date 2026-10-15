	.section .rodata
	.global Monster_WispGhostSprites
Monster_WispGhostSprites:
	.incbin "games/common/src/graphics/monster/wisp_ghost.frames.lz"
	.byte 0x3e, 0xca	@ PACKER: left in the buffer by Monster_OgreSprites+0xe46
