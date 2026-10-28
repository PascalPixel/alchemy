	.section .rodata
@ {x, y} screen offsets of a ground particle, in pixels, indexed by its
@ remaining frame count; read by Func_080d6674 (The Broken Seal's
@ FieldEffect_UpdateGroundParticles), which has no such table.
	.global Data_080f0b50
Data_080f0b50:
	.2byte -32, -16
	.2byte -48, -23
	.2byte -64, -34
	.2byte -80, -43
	.2byte -96, -52
	.2byte -112, -61
	.2byte -128, -70
	.2byte -144, -77
	.2byte -160, -84
	.2byte -176, -91
	.2byte -192, -98
