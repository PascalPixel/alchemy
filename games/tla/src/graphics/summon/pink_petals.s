@ A 16-colour palette and one packed stream of 4,320 bytes. Three routines load
@ it, and each reads the stream's first 1,920 bytes as fifteen 16x16 sprites of
@ 16 colours (pink_petals.png): two copy them to OBJ video memory 128 bytes a
@ sprite, one with the palette in OBJ palette 15 and one with that row filled
@ white, and the third unpacks them into a 256-colour canvas under BG palette
@ 0. None reads further. The next 1,920 bytes are no picture of this summon:
@ Camelot's converter left them in its buffer, and they are bytes 6,104 to
@ 8,023 of Summon_SeaSerpentTiles' unpacked 256-colour tiles
@ (pink_petals_leftover.tsv). The last 480 bytes are zero.
	.section .rodata
	.global Summon_PinkPetalsTiles
Summon_PinkPetalsTiles:
	.incbin "games/tla/src/graphics/summon/pink_petals.gbapal"
	.incbin "games/tla/src/graphics/summon/pink_petals.parts.lz"
	.byte 0xe1, 0xb1	@ PACKER: left in the buffer by Summon_SeaSerpentTiles+0x77a
