	.global Field_MadraCatacombsStoneDoor
Field_MadraCatacombsStoneDoor:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraCatacombsStoneDoor, .LField_MadraCatacombsStoneDoor_CELL_TILES
	map_block Field_MadraCatacombsStoneDoor, .LField_MadraCatacombsStoneDoor_SHAPES
	map_block Field_MadraCatacombsStoneDoor, .LField_MadraCatacombsStoneDoor_GRID
	map_block Field_MadraCatacombsStoneDoor, .LField_MadraCatacombsStoneDoor_SHAPE_GRID
	map_block Field_MadraCatacombsStoneDoor, .LField_MadraCatacombsStoneDoor_ANIMATION
	map_block Field_MadraCatacombsStoneDoor, .LField_MadraCatacombsStoneDoor_BLEND
	map_block Field_MadraCatacombsStoneDoor, .LField_MadraCatacombsStoneDoor_SCENE
.LField_MadraCatacombsStoneDoor_CELL_TILES:
	.incbin "games/tla/src/map/madra_catacombs_stone_door/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MadraCatacombsStoneDoor_SHAPES:
	.incbin "games/tla/src/map/madra_catacombs_stone_door/shapes.table.lz"
	.balign 4, 0
.LField_MadraCatacombsStoneDoor_GRID:
	.incbin "games/tla/src/map/madra_catacombs_stone_door/map.grid.lz"
	.balign 4, 0
.LField_MadraCatacombsStoneDoor_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_catacombs_stone_door/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraCatacombsStoneDoor_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_MadraCatacombsStoneDoor_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MadraCatacombsStoneDoor_SCENE:
	.incbin "games/tla/src/map/madra_catacombs_stone_door/scene.scene.lz"
	.global Tileset_PaletteMadraCatacombsStoneDoor
Tileset_PaletteMadraCatacombsStoneDoor:
	.incbin "games/tla/src/graphics/tileset/palettes/madra_catacombs_stone_door.gbapal.lz"
	.byte 0xe3, 0x56	@ PACKER: left in the buffer by Field_MadraCatacombs4+0x176
