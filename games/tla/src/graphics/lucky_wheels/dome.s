	.section .rodata
	.global Slots_DomePicture
Slots_DomePicture:
	.incbin "games/tla/src/graphics/lucky_wheels/dome.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/dome.8bpp.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x9b, 0x65	@ PACKER: left in the buffer by Ending_TheEndPicture+0x3f12
	.else
	.ifdef TLA_EDITION_EN
	.byte 0x4b, 0x5a	@ PACKER: left in the buffer by Ending_TheEndPicture+0x3f12
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x00, 0x00	@ PACKER: left in the buffer by Ending_TheEndPicture+0x3f12
	.else
	.ifdef TLA_EDITION_IT
	.byte 0xd3, 0xd3	@ PACKER: left in the buffer by Ending_TheEndPicture+0x3f12
	.else
	.byte 0x71, 0xa4	@ PACKER: left in the buffer by Ending_TheEndPicture+0x3f12
	.endif
	.endif
	.endif
	.endif
