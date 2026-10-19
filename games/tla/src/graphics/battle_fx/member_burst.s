	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_MemberBurstImage
BattleFx_MemberBurstImage:
	.incbin "games/tla/src/graphics/battle_fx/member_burst.bitmap.lz"
	.ifne GAME_TLA
	.byte 0x19, 0x14, 0x56	@ PACKER: left in the buffer by BattleFx_FireballSheet+0x49d
	.endif
