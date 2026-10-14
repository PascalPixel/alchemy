@ Four frames of one 16x16 sprite of 16 colours, 512 bytes. Func_080e82cc
@ unpacks them, loads them as one block of OBJ video memory and makes 32
@ sprites of them, each with the 256-colour bit cleared and OBJ palette 15.
@ Its callback Func_080e807c steps every sprite through the four frames, four
@ ticks a frame, and each tick writes sixteen colours it computes to OBJ
@ palette 15. The game stores no palette for this sheet: r1ed.png carries the
@ first row of the shared OBJ palette, which the 256-colour sheets of this
@ folder carry whole.
	.section .rodata
	.global Resource_Data1ED
Resource_Data1ED:
	.incbin "games/tla/src/graphics/field_fx/r1ed.4bpp16x16.lz"
