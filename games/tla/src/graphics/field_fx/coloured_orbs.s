	.section .rodata
	.global FieldFx_ColouredOrbs
FieldFx_ColouredOrbs:
	.incbin "games/tla/src/graphics/field_fx/coloured_orbs.8bpp.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x39, 0x96	@ PACKER: left in the buffer by FieldFx_WhitePuff+0x152
	.else
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
