	.section .rodata
	.global BattleFx_ArcFragmentsSheet
BattleFx_ArcFragmentsSheet:
	.incbin "games/tla/src/graphics/battle_fx/arc_fragments.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/arc_fragments.bitmap.lz"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data0FD+0x643
