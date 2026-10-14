@ A 16-colour palette and one packed stream of 3,040 bytes. The game copies the
@ palette to OBJ palette 13 and the stream's first 2,560 bytes to video memory
@ as five 32x32 sprites of 16 colours (purple_armour.png). It reads no more of
@ the stream. The last 480 bytes are no picture of this summon: Camelot's
@ converter left them in its buffer, and they are bytes 15,904 to 16,383 of
@ Summon_ChainedDemonTiles' unpacked 256-colour tiles
@ (purple_armour_leftover.tsv).
	.section .rodata
	.global Summon_PurpleArmourTiles
Summon_PurpleArmourTiles:
	.incbin "games/tla/src/graphics/summon/purple_armour.gbapal"
	.incbin "games/tla/src/graphics/summon/purple_armour.parts.lz"
