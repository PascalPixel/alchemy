	.global Field_NaribweInteriors
Field_NaribweInteriors:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_NaribweInteriors, .LField_NaribweInteriors_CELL_PICTURES
	map_block Field_NaribweInteriors, .LField_NaribweInteriors_SHAPES
	map_block Field_NaribweInteriors, .LField_NaribweInteriors_GRID
	map_block Field_NaribweInteriors, .LField_NaribweInteriors_SHAPE_GRID
	map_block Field_NaribweInteriors, .LField_NaribweInteriors_ANIMATION
	map_block Field_NaribweInteriors, .LField_NaribweInteriors_BLEND
	map_block Field_NaribweInteriors, .LField_NaribweInteriors_SCENE
.LField_NaribweInteriors_CELL_PICTURES:
	.incbin "games/tla/src/map/naribwe_interiors/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_NaribweInteriors_SHAPES:
	.incbin "games/tla/src/map/naribwe_interiors/shapes.table.lz"
	.balign 4, 0
.LField_NaribweInteriors_GRID:
	.incbin "games/tla/src/map/naribwe_interiors/map.grid.lz"
	.balign 4, 0
.LField_NaribweInteriors_SHAPE_GRID:
	.incbin "games/tla/src/map/naribwe_interiors/shape_grid.plane.lz"
	.balign 4, 0
.LField_NaribweInteriors_ANIMATION:
	.incbin "games/tla/src/map/naribwe_interiors/animation.script.lz"
	.balign 4, 0
.LField_NaribweInteriors_BLEND:
	.incbin "games/tla/src/map/daila_interiors_houses/blend.script.lz"
	.balign 4, 0
.LField_NaribweInteriors_SCENE:
	.incbin "games/tla/src/map/naribwe_interiors/scene.scene.lz"
	.balign 4, 0
	.global Tileset_NaribweInteriorsPalette
Tileset_NaribweInteriorsPalette:
	.incbin "games/tla/src/graphics/tileset/naribwe_interiors/palette.gbapal.lz"
	.global Tileset_NaribweInteriorsTilesA
Tileset_NaribweInteriorsTilesA:
	.incbin "games/tla/src/graphics/tileset/naribwe_interiors/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_NaribweInteriorsTilesB
Tileset_NaribweInteriorsTilesB:
	.incbin "games/tla/src/graphics/tileset/naribwe_interiors/tiles_b.4bpp.mtf"
	.byte 0x71, 0x76, 0xf0	@ PACKER: left in the buffer by Tileset_NaribweInteriorsTilesA+0x1969
	.global Tileset_NaribweInteriorsTilesC
Tileset_NaribweInteriorsTilesC:
	.incbin "games/tla/src/graphics/tileset/naribwe_interiors/tiles_c.4bpp.mtf"
	.global Tileset_NaribweInteriorsTilesD
Tileset_NaribweInteriorsTilesD:
	.incbin "games/tla/src/graphics/tileset/naribwe_interiors/tiles_d.4bpp.mtf"
	.byte 0x3e, 0x8d	@ PACKER: left in the buffer by Tileset_NaribweInteriorsTilesC+0xf1e
