@ The lightning bolt's 64 background tiles of 16 colours, drawn as
@ Field_LightningMap shows them: one bolt from the sky to the ground.
@ Overlays 663, 664 and 6a3 unpack them to background tiles 0 to 63 and draw
@ them with that map, its tile numbers less 0x200, in background palette 2;
@ the picture carries that row of the tileset palette of overlay 663's first
@ map (Tileset_Set40Palette).
	.section .rodata
	.global Field_LightningTiles
Field_LightningTiles:
	.incbin "games/tla/src/graphics/field/lightning_tiles.parts.lz"
	.byte 0x9f, 0xe5	@ PACKER: left in the buffer by Field_WhirlwindTiles+0x2ee
