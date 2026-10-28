	.section .rodata
@ A 64x64 silhouette with no palette of its own. No code loads it: only the
@ resource directory names it. The PNG carries the palette of resource 0xB3,
@ the next resource and another silhouette, as a neighbour's, not as a use
	.global Resource_Data0B2
Resource_Data0B2:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0b2.bitmap.lz"
