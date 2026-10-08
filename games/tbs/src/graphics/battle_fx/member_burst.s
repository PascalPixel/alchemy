	.section .rodata
	.global BattleFx_MemberBurstImage
BattleFx_MemberBurstImage:
	.incbin "graphics/battle_fx/member_burst.bitmap.lz"
	.byte 0x19, 0x14, 0x56	@ PACKER: left in the buffer by BattleFx_FireballSheet+0x49d
