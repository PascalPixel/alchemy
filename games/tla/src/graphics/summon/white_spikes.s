@ A 16-colour palette and one packed stream of 3,552 bytes. The game copies the
@ palette to OBJ palette 14 and the stream's first 3,072 bytes to video memory
@ as six 32x32 sprites of 16 colours, drawn here as the three spikes they make
@ in pairs, upper half then lower (white_spikes.png). The last 480 bytes are
@ zero.
	.section .rodata
	.global Summon_WhiteSpikesTiles
Summon_WhiteSpikesTiles:
	.incbin "games/tla/src/graphics/summon/white_spikes.gbapal"
	.incbin "games/tla/src/graphics/summon/white_spikes.parts.lz"
