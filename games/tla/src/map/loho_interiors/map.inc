	.global Field_LohoInteriors
Field_LohoInteriors:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_LohoInteriors, .LField_LohoInteriors_CELL_PICTURES
	map_block Field_LohoInteriors, .LField_LohoInteriors_SHAPES
	map_block Field_LohoInteriors, .LField_LohoInteriors_GRID
	map_block Field_LohoInteriors, .LField_LohoInteriors_SHAPE_GRID
	map_block Field_LohoInteriors, .LField_LohoInteriors_ANIMATION
	map_block Field_LohoInteriors, .LField_LohoInteriors_BLEND
	map_block Field_LohoInteriors, .LField_LohoInteriors_SCENE
.LField_LohoInteriors_CELL_PICTURES:
	.incbin "games/tla/src/map/loho_interiors/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_LohoInteriors_SHAPES:
	.incbin "games/tla/src/map/loho_interiors/shapes.table.lz"
	.balign 4, 0
.LField_LohoInteriors_GRID:
	.incbin "games/tla/src/map/loho_interiors/map.grid.lz"
	.balign 4, 0
.LField_LohoInteriors_SHAPE_GRID:
	.incbin "games/tla/src/map/loho_interiors/shape_grid.plane.lz"
	.balign 4, 0
.LField_LohoInteriors_ANIMATION:
	.incbin "games/tla/src/map/loho_interiors/animation.script.lz"
	.balign 4, 0
.LField_LohoInteriors_BLEND:
	.incbin "games/tla/src/map/daila_interiors_houses/blend.script.lz"
	.balign 4, 0
.LField_LohoInteriors_SCENE:
	.incbin "games/tla/src/map/loho_interiors/scene.scene.lz"
	.balign 4, 0
	.global Tileset_LohoInteriorsPalette
Tileset_LohoInteriorsPalette:
	.incbin "games/tla/src/graphics/tileset/loho_interiors/palette.gbapal.lz"
	.global Tileset_LohoInteriorsTilesA
Tileset_LohoInteriorsTilesA:
	.incbin "games/tla/src/graphics/tileset/loho_interiors/tiles_a.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_LohoInteriors+0x14fb
	.global Tileset_LohoInteriorsTilesB
Tileset_LohoInteriorsTilesB:
	.incbin "games/tla/src/graphics/tileset/loho_interiors/tiles_b.4bpp.mtf"
	.byte 0xb5	@ PACKER: left in the buffer by Field_LohoInteriors+0x1bc7
	.global Tileset_LohoInteriorsTilesC
Tileset_LohoInteriorsTilesC:
	.incbin "games/tla/src/graphics/tileset/loho_interiors/tiles_c.4bpp.mtf"
	.byte 0x95, 0x3b	@ PACKER: left in the buffer by Tileset_LohoInteriorsTilesB+0x1b7a
	.global Tileset_LohoInteriorsTilesD
Tileset_LohoInteriorsTilesD:
	.incbin "games/tla/src/graphics/tileset/loho_interiors/tiles_d.4bpp.mtf"
	.byte 0x82, 0x94, 0x06	@ PACKER: left in the buffer by Tileset_LohoInteriorsTilesC+0x449
