@ Four pictures of a 32x32 sprite of 256 colours, one under another: two of an
@ orb of rings in colours 224 to 234, then two filled with colour 2.
@ Func_080e9af4 unpacks them into one block of OBJ video memory and makes a
@ 32x32 sprite that shows the first two in turn. Its callback Func_080e9940
@ writes sixteen colours it computes from the table gData_080f0fe0 to OBJ
@ palette 14 each frame, from a counter that starts at 0. The picture
@ carries the shared OBJ palette, those colours of the first frame, and black
@ for palette 15, which it does not use.
	.section .rodata
	.global FieldEffect_RingOrb
FieldEffect_RingOrb:
	.incbin "games/tla/src/graphics/field_fx/ring_orb.8bpp.lz"
