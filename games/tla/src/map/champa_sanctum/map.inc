	.global Field_ChampaSanctum
Field_ChampaSanctum:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ChampaSanctum, .LField_ChampaSanctum_CELL_PICTURES
	map_block Field_ChampaSanctum, .LField_ChampaSanctum_SHAPES
	map_block Field_ChampaSanctum, .LField_ChampaSanctum_GRID
	map_block Field_ChampaSanctum, .LField_ChampaSanctum_SHAPE_GRID
	map_block Field_ChampaSanctum, .LField_ChampaSanctum_ANIMATION
	map_block Field_ChampaSanctum, .LField_ChampaSanctum_BLEND
	map_block Field_ChampaSanctum, .LField_ChampaSanctum_SCENE
.LField_ChampaSanctum_CELL_PICTURES:
	.incbin "games/tla/src/map/champa_sanctum/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ChampaSanctum_SHAPES:
	.incbin "games/tla/src/map/champa_sanctum/shapes.table.lz"
	.balign 4, 0
.LField_ChampaSanctum_GRID:
	.incbin "games/tla/src/map/champa_sanctum/map.grid.lz"
	.balign 4, 0
.LField_ChampaSanctum_SHAPE_GRID:
	.incbin "games/tla/src/map/champa_sanctum/shape_grid.plane.lz"
	.balign 4, 0
.LField_ChampaSanctum_ANIMATION:
	.incbin "games/tla/src/map/map_041/animation.script.lz"
	.balign 4, 0
.LField_ChampaSanctum_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_ChampaSanctum_SCENE:
	.incbin "games/tla/src/map/champa_sanctum/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteChampaSanctum
Tileset_PaletteChampaSanctum:
	.incbin "games/tla/src/graphics/tileset/palettes/champa_sanctum.gbapal.lz"
	.byte 0x84, 0x03, 0x15	@ PACKER: left in the buffer by Field_ChampaSanctum+0x13d
