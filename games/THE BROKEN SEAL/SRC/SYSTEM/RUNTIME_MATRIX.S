@ The runtime's current transform: a 3x3 matrix of 16.16 coefficients and
@ a translation, the identity with no translation in this image.
@ IwramTransformVector and IwramTransformMatrix in RUNTIME_TRANSFORM.S read
@ it (add r0, pc, #0xf0; add r1, pc, #0x7c; ldmia), and the matrix
@ transform writes its product back over it (stmdb r1, {...}). A run-time
@ variable carries no credit.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08000ac0
	.set AlchemyRunAddress, 0x03000350

@ 三行の単位変換係数と平行移動成分。
.L_unit_matrix:
	.4byte 0x00010000, 0, 0, 0
	.4byte 0x00010000, 0, 0, 0
	.4byte 0x00010000, 0, 0, 0
