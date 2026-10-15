	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_MushroomSprites
Monster_MushroomSprites:
	.incbin "games/common/src/graphics/monster/mushroom.frames.lz"
	.ifeq GAME_TLA
	.byte 0x47, 0x7d, 0x5a	@ PACKER: left in the buffer by Monster_BlueWispSprites+0xd09
	.else
	.byte 0x47, 0x7d, 0x5a	@ PACKER: left in the buffer by Monster_BlueWispSprites+0xd09
	.endif
