@ A 16-colour palette and one packed stream of 8,672 bytes. The game copies the
@ palette to OBJ palette 15 and the stream's first 8,192 bytes to video memory
@ as two groups of eight 32x32 sprites of 16 colours, 512 bytes a sprite. Side
@ by side each group is a strip 256 pixels wide, and the second continues the
@ first downwards: forest_canopy.png draws them as that one picture. The last
@ 480 bytes are zero.
	.section .rodata
	.global Summon_ForestCanopyTiles
Summon_ForestCanopyTiles:
	.incbin "graphics/summon/forest_canopy.gbapal"
	.incbin "graphics/summon/forest_canopy.parts.lz"
