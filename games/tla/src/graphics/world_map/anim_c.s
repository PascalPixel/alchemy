	.section .rodata
@ World map C has no animated tiles.
	.global Resource_Data1AD
Resource_Data1AD:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x12, 0x40	@ PACKER: unused post-stream residue; Resource_DecodeType01 uses decoded RAM and output length; writer unknown
	.endif
