	.global Field_MagmaRockSlide
Field_MagmaRockSlide:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MagmaRockSlide, .LField_MagmaRockSlide_CELL_TILES
	map_block Field_MagmaRockSlide, .LField_MagmaRockSlide_SHAPES
	map_block Field_MagmaRockSlide, .LField_MagmaRockSlide_GRID
	map_block Field_MagmaRockSlide, .LField_MagmaRockSlide_SHAPE_GRID
	map_block Field_MagmaRockSlide, .LField_MagmaRockSlide_ANIMATION
	map_block Field_MagmaRockSlide, .LField_MagmaRockSlide_BLEND
	map_block Field_MagmaRockSlide, .LField_MagmaRockSlide_SCENE
.LField_MagmaRockSlide_CELL_TILES:
	.incbin "games/tla/src/map/magma_rock_slide/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MagmaRockSlide_SHAPES:
	.incbin "games/tla/src/map/magma_rock_slide/shapes.table.lz"
	.balign 4, 0
.LField_MagmaRockSlide_GRID:
	.incbin "games/tla/src/map/magma_rock_slide/map.grid.lz"
	.balign 4, 0
.LField_MagmaRockSlide_SHAPE_GRID:
	.incbin "games/tla/src/map/magma_rock_slide/shape_grid.plane.lz"
	.balign 4, 0
.LField_MagmaRockSlide_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_MagmaRockSlide_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MagmaRockSlide_SCENE:
	.incbin "games/tla/src/map/magma_rock_slide/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteMagmaRockSlide
Tileset_PaletteMagmaRockSlide:
	.incbin "games/tla/src/graphics/tileset/palettes/magma_rock_slide.gbapal.lz"
	.byte 0x38, 0xc8	@ PACKER: left in the buffer by Field_MagmaRock1+0x17a
