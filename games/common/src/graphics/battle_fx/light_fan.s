	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_LightFanSheet
BattleFx_LightFanSheet:
	.incbin "games/common/src/graphics/battle_fx/light_fan.gbapal"
	.incbin "games/common/src/graphics/battle_fx/light_fan.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x09, 0x82, 0x76	@ PACKER: left in the buffer by BattleFx_IceBlockSheet+0x59d
	.else
	.byte 0x09, 0x82, 0x76	@ PACKER: left in the buffer by BattleFx_IceBlockSheet+0x59d
	.endif
