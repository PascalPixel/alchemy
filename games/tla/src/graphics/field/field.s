	.section .rodata
@ The whirlwind's 52 tiles of 16 colours: 26 columns of 8x16, the top halves
@ in the first row and the bottom halves in the second, as they sit at
@ background tiles 0x140 and 0x160. Overlays 663 and 696 unpack them, copy
@ each row to its place and draw them with Field_WhirlwindMap less 0xc0.
@ Overlay 696 shows every tile in background palette 1, and the picture
@ carries that row of its first map's tileset palette (Tileset_Set100Palette).
	.global Field_WhirlwindTiles
Field_WhirlwindTiles:
	.incbin "games/tla/src/graphics/field/whirlwind_tiles.4bpp.lz"
	.byte 0x07, 0x12, 0x36	@ PACKER: left in the buffer by WorldMap_TilesD6+0x2f5
	.global Field_WhirlwindMap
Field_WhirlwindMap:
	.incbin "games/tla/src/graphics/field/whirlwind_map.table.lz"
	.byte 0xd9, 0xe1, 0xe4	@ PACKER: left in the buffer by Field_WhirlwindTiles+0xd9
@ The lightning bolt's 64 background tiles of 16 colours, drawn as
@ Field_LightningMap shows them: one bolt from the sky to the ground.
@ Overlays 663, 664 and 6a3 unpack them to background tiles 0 to 63 and draw
@ them with that map, its tile numbers less 0x200, in background palette 2;
@ the picture carries that row of the tileset palette of overlay 663's first
@ map (Tileset_Set40Palette).
	.global Field_LightningTiles
Field_LightningTiles:
	.incbin "games/tla/src/graphics/field/lightning_tiles.parts.lz"
	.byte 0x9f, 0xe5	@ PACKER: left in the buffer by Field_WhirlwindTiles+0x2ee
	.global Field_LightningMap
Field_LightningMap:
	.incbin "games/tla/src/graphics/field/lightning_map.table.lz"
	.byte 0xf1, 0x2e	@ PACKER: left in the buffer by Field_LightningTiles+0x92
	.global Field_WalkPathA
Field_WalkPathA:
	.incbin "games/tla/src/graphics/field/walk_path_a.table.lz"
	.byte 0x41, 0xad, 0x6c	@ PACKER: left in the buffer by WorldMap_TilesD3+0xbb1
	.global Field_WalkPathB
Field_WalkPathB:
	.incbin "games/tla/src/graphics/field/walk_path_b.table.lz"
	.byte 0xce, 0x03	@ PACKER: left in the buffer by WorldMap_TilesD3+0xcaa
	.global Field_WalkPathC
Field_WalkPathC:
	.incbin "games/tla/src/graphics/field/walk_path_c.table.lz"
	.byte 0xa0, 0x81	@ PACKER: left in the buffer by Field_WalkPathB+0xb02
	.global Field_WalkPathD
Field_WalkPathD:
	.incbin "games/tla/src/graphics/field/walk_path_d.table.lz"
	.byte 0xe6, 0xf4, 0x80	@ PACKER: left in the buffer by WorldMap_TilesD3+0xf3d
	.global Field_WalkPathE
Field_WalkPathE:
	.incbin "games/tla/src/graphics/field/walk_path_e.table.lz"
	.byte 0x1c, 0x38, 0x13	@ PACKER: left in the buffer by Field_WalkPathD+0x839
	.global Field_WalkPathF
Field_WalkPathF:
	.incbin "games/tla/src/graphics/field/walk_path_f.table.lz"
	.global Field_WalkPathG
Field_WalkPathG:
	.incbin "games/tla/src/graphics/field/walk_path_g.table.lz"
	.byte 0xa2, 0x38	@ PACKER: left in the buffer by Field_WalkPathF+0x882
	.global Field_WalkPathH
Field_WalkPathH:
	.incbin "games/tla/src/graphics/field/walk_path_h.table.lz"
	.byte 0x00, 0x5b	@ PACKER: left in the buffer by Field_WalkPathF+0xab6
	.global Field_WalkPathI
Field_WalkPathI:
	.incbin "games/tla/src/graphics/field/walk_path_i.table.lz"
	.global Field_WalkPathJ
Field_WalkPathJ:
	.incbin "games/tla/src/graphics/field/walk_path_j.table.lz"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
