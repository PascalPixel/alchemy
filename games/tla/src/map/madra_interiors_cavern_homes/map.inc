	.global Field_MadraInteriorsCavernHomes
Field_MadraInteriorsCavernHomes:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraInteriorsCavernHomes, .LField_MadraInteriorsCavernHomes_CELL_PICTURES
	map_block Field_MadraInteriorsCavernHomes, .LField_MadraInteriorsCavernHomes_SHAPES
	map_block Field_MadraInteriorsCavernHomes, .LField_MadraInteriorsCavernHomes_GRID
	map_block Field_MadraInteriorsCavernHomes, .LField_MadraInteriorsCavernHomes_SHAPE_GRID
	map_block Field_MadraInteriorsCavernHomes, .LField_MadraInteriorsCavernHomes_ANIMATION
	map_block Field_MadraInteriorsCavernHomes, .LField_MadraInteriorsCavernHomes_BLEND
	map_block Field_MadraInteriorsCavernHomes, .LField_MadraInteriorsCavernHomes_SCENE
.LField_MadraInteriorsCavernHomes_CELL_PICTURES:
	.incbin "games/tla/src/map/madra_interiors_cavern_homes/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MadraInteriorsCavernHomes_SHAPES:
	.incbin "games/tla/src/map/madra_interiors_cavern_homes/shapes.table.lz"
	.balign 4, 0
.LField_MadraInteriorsCavernHomes_GRID:
	.incbin "games/tla/src/map/madra_interiors_cavern_homes/map.grid.lz"
	.balign 4, 0
.LField_MadraInteriorsCavernHomes_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_interiors_cavern_homes/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraInteriorsCavernHomes_ANIMATION:
	.incbin "games/tla/src/map/madra_interiors_cavern_homes/animation.script.lz"
	.balign 4, 0
.LField_MadraInteriorsCavernHomes_BLEND:
	.incbin "games/tla/src/map/daila_interiors_houses/blend.script.lz"
	.balign 4, 0
.LField_MadraInteriorsCavernHomes_SCENE:
	.incbin "games/tla/src/map/madra_interiors_cavern_homes/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MadraInteriorsCavernHomesPalette
Tileset_MadraInteriorsCavernHomesPalette:
	.incbin "games/tla/src/graphics/tileset/madra_interiors_cavern_homes/palette.gbapal.lz"
	.byte 0xc7	@ PACKER: left in the buffer by Field_MadraInteriorsCavernHomes+0x177
	.global Tileset_MadraInteriorsCavernHomesTilesA
Tileset_MadraInteriorsCavernHomesTilesA:
	.incbin "games/tla/src/graphics/tileset/madra_interiors_cavern_homes/tiles_a.4bpp.mtf"
	.byte 0x08, 0x0a	@ PACKER: left in the buffer by Field_MadraInteriorsCavernHomes+0x14be
	.global Tileset_MadraInteriorsCavernHomesTilesB
Tileset_MadraInteriorsCavernHomesTilesB:
	.incbin "games/tla/src/graphics/tileset/madra_interiors_cavern_homes/tiles_b.4bpp.mtf"
	.byte 0x3c, 0x00	@ PACKER: left in the buffer by Field_MadraInteriorsCavernHomes+0x1802
	.global Tileset_MadraInteriorsCavernHomesTilesC
Tileset_MadraInteriorsCavernHomesTilesC:
	.incbin "games/tla/src/graphics/tileset/madra_interiors_cavern_homes/tiles_c.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0x1d, 0x1e	@ PACKER: left in the buffer by Field_MadraInteriorsCavernHomes+0x1976
	.else
	.byte 0x01, 0x00	@ PACKER: unused post-stream residue; Decompress_MoveToFrontLz uses decoded RAM and output length; writer unknown
	.endif
	.global Tileset_MadraInteriorsCavernHomesTilesD
Tileset_MadraInteriorsCavernHomesTilesD:
	.incbin "games/tla/src/graphics/tileset/madra_interiors_cavern_homes/tiles_d.4bpp.mtf"
	.byte 0x22, 0xb9	@ PACKER: left in the buffer by Tileset_MadraInteriorsCavernHomesTilesC+0xf1e
