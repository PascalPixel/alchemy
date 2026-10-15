@ A 16-colour palette and one packed stream of 4,576 bytes. The game copies the
@ palette to OBJ palette 14 and at once fills that row with white, and copies
@ the stream's first 2,048 bytes to video memory as one 64x64 sprite of 16
@ colours (white_wisps.png). It reads no more of the stream. The next 2,048
@ bytes are no picture of this summon: Camelot's converter left them in its
@ buffer, and they are bytes 6,488 to 8,535 of Summon_CrimsonBeastTiles' unpacked
@ 256-colour tiles (white_wisps_leftover.tsv). The last 480 bytes are zero.
	.section .rodata
	.global Summon_WhiteWispsTiles
Summon_WhiteWispsTiles:
	.incbin "games/tla/src/graphics/summon/white_wisps.gbapal"
	.incbin "games/tla/src/graphics/summon/white_wisps.parts.lz"
	.byte 0xfc	@ PACKER: left in the buffer by Summon_RedCrescentTiles+0x577
