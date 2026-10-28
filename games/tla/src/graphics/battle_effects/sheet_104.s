	.section .rodata
@ A 64x32 bar with a rounded end and no palette of its own: BattleEffect_RunCruelRuin
@ maps it as a 64 by 32 texture and shows it in a grey ramp it writes in
@ code; the PNG carries BattleEffect_CornerRuneASheet's palette, the one 64-colour
@ palette that effect loads
	.global Resource_Data104
Resource_Data104:
	.incbin "games/tla/src/graphics/battle_effects/sheet_104.bitmap.lz"
	.byte 0x89, 0x22, 0x69	@ PACKER: left in the buffer by Resource_Data103+0xfd
