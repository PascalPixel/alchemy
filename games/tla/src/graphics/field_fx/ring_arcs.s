@ Two quarters of a ring, 64x64 each, one under the other: 128 background
@ tiles of 256 colours, drawn in colours 243 to 255. Func_080e82cc unpacks
@ them and copies all 8,192 bytes to background tiles 0 to 127. Its
@ callback Func_080e807c writes sixteen colours it computes to background
@ and OBJ palette 15 each frame, from a counter that starts at 0: the picture
@ carries the colours of the first frame, and black for the 240 it does not
@ use.
	.section .rodata
	.global FieldEffect_RingArcs
FieldEffect_RingArcs:
	.incbin "games/tla/src/graphics/field_fx/ring_arcs.8bpp.lz"
