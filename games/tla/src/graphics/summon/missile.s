@ A 16-colour palette and one packed stream of 608 bytes. The game copies the
@ palette to OBJ palette 14 and the stream's first 256 bytes to video memory, and
@ shows them through 16x8 sprites of 16 colours that all name the first two
@ tiles: the stream's first 64 bytes (missile.png). The other 544 bytes are no
@ picture: Camelot's converter left them in its buffer, and they are bytes of
@ the packed stream of Summon_ForestCanopyTiles, the 64 from its byte 1,056 and
@ then the 480 from its byte 1,960 (missile_leftover.tsv).
	.section .rodata
	.global Summon_MissileTiles
Summon_MissileTiles:
	.incbin "games/tla/src/graphics/summon/missile.gbapal"
	.incbin "games/tla/src/graphics/summon/missile.parts.lz"
