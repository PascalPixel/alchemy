	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_BeamSequenceImage
BattleFx_BeamSequenceImage:
	.incbin "games/common/src/graphics/battle_fx/beam_sequence.parts.lz"
	.ifne GAME_TLA
	.byte 0x5f	@ PACKER: left in the buffer by BattleFx_LightningBoltSheet+0x37b
	.endif
