@ A 16-colour palette and one packed stream of 1,504 bytes. The game copies the
@ palette to OBJ palette 15 and the stream's first 512 bytes to video memory as
@ sixteen 8x8 sprites of 16 colours (white_arrows.png). It reads no more of the stream. The next 512 bytes are no
@ picture of this summon: Camelot's converter left them in its buffer, and they
@ are bytes 1,880 to 2,391 of Summon_PinkPetalsTiles' unpacked stream, the end
@ of its petals and the start of its own leftover
@ (white_arrows_leftover.tsv). The last 480 bytes are zero.
	.section .rodata
	.global Summon_WhiteArrowsTiles
Summon_WhiteArrowsTiles:
	.incbin "graphics/summon/white_arrows.gbapal"
	.incbin "graphics/summon/white_arrows.parts.lz"
