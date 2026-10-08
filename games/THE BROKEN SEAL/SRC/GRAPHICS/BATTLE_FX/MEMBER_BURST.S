	.section .rodata
	.global BattleFx_MemberBurstImage
BattleFx_MemberBurstImage:
	.incbin "GRAPHICS/BATTLE_FX/MEMBER_BURST.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_FireballSheet at +0x49d */
	.byte 0x19, 0x14, 0x56
