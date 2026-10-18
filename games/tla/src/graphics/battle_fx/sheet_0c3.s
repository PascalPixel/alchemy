	.section .rodata
@ A 64x64 ring with no palette of its own: BattleFx_RunCharon maps it as a 64 by
@ 64 texture. That effect colours it with a ramp it computes; the PNG carries
@ BattleFx_BoltSheet's palette, the one it loads most often
	.global Resource_Data0C3
Resource_Data0C3:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0c3.bitmap.lz"
	.byte 0x08, 0x81	@ PACKER: left in the buffer by Resource_Data0C2+0x2e6
