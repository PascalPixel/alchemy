	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_IceTileSheet
BattleFx_IceTileSheet:
	.incbin "games/common/src/graphics/battle_fx/ice_tile_00.gbapal"
	.incbin "games/common/src/graphics/battle_fx/ice_tile.parts.lz"
	.ifne GAME_TLA
	.byte 0x00, 0x01	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x106a
	.endif
