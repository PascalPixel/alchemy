	.global Field_SuhallaGateCheckpoint
Field_SuhallaGateCheckpoint:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_CELL_PICTURES
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_SHAPES
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_GRID
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_ANIMATION
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_BLEND
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_MARKERS
.LField_SuhallaGateCheckpoint_CELL_PICTURES:
	.incbin "games/tbs/src/map/suhalla_gate_checkpoint/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_SHAPES:
	.incbin "games/tbs/src/map/suhalla_gate_checkpoint/shapes_es.table.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_GRID:
	.incbin "games/tbs/src/map/suhalla_gate_checkpoint/map_es.grid.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_ANIMATION:
	.incbin "games/tbs/src/map/bilibin/tileset_anims.script.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_CELL_PICTURES
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_SHAPES
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_GRID
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_ANIMATION
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_BLEND
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_MARKERS
.LField_SuhallaGateCheckpoint_CELL_PICTURES:
	.incbin "games/tbs/src/map/suhalla_gate_checkpoint/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_SHAPES:
	.incbin "games/tbs/src/map/suhalla_gate_checkpoint/shapes_es.table.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_GRID:
	.incbin "games/tbs/src/map/suhalla_gate_checkpoint/map_es.grid.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_ANIMATION:
	.incbin "games/tbs/src/map/bilibin/tileset_anims.script.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_CELL_PICTURES
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_SHAPES
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_GRID
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_ANIMATION
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_BLEND
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_MARKERS
.LField_SuhallaGateCheckpoint_CELL_PICTURES:
	.incbin "games/tbs/src/map/suhalla_gate_checkpoint/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_SHAPES:
	.incbin "games/tbs/src/map/suhalla_gate_checkpoint/shapes_es.table.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_GRID:
	.incbin "games/tbs/src/map/suhalla_gate_checkpoint/map_es.grid.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_ANIMATION:
	.incbin "games/tbs/src/map/bilibin/tileset_anims.script.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_MARKERS:
	treasure_spots_end
	.else
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_CELL_PICTURES
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_SHAPES
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_GRID
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_ANIMATION
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_BLEND
	map_offset Field_SuhallaGateCheckpoint, .LField_SuhallaGateCheckpoint_MARKERS
.LField_SuhallaGateCheckpoint_CELL_PICTURES:
	.incbin "games/tbs/src/map/suhalla_gate_checkpoint/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_SHAPES:
	.incbin "games/tbs/src/map/suhalla_gate_checkpoint/shapes.table.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_GRID:
	.incbin "games/tbs/src/map/suhalla_gate_checkpoint/map.grid.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_ANIMATION:
	.incbin "games/tbs/src/map/bilibin/tileset_anims.script.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_SuhallaGateCheckpoint_MARKERS:
	treasure_spots_end
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_PaletteSuhallaGate
Tileset_PaletteSuhallaGate:
	.incbin "games/tbs/src/graphics/tileset/palettes/suhalla_gate.gbapal.lz"
	.byte 0xfc, 0x22, 0x8f	@ PACKER: left in the buffer by Field_SuhallaGateCheckpoint+0x171
	.balign 4, 0
