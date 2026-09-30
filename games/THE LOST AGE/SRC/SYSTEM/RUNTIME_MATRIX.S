@ The current transform of TLA's resident runtime: a 3x3 matrix of 16.16
@ coefficients and a translation, the identity with no translation in this
@ image. The vector and matrix transforms of IWRAM_RUNTIME_TRANSFORM.S read
@ it (add r0, pc, #0x110; add r5, pc, #144; add r1, pc, #64; ldm), and the
@ matrix transform writes its product back over it (stmdb r1, {...}). A
@ run-time variable carries no credit.
.syntax unified
	.section .data,"aw",%progbits
	.arm
	.global gTransform
gTransform:
	.4byte	0x00010000
	.4byte	0x00000000
	.4byte	0x00000000
	.4byte	0
	.4byte	0x00010000
	.4byte	0
	.4byte	0
	.4byte	0
	.4byte	0x00010000
	.global gTransformTranslation
gTransformTranslation:
	.4byte	0x00000000
	.4byte	0x00000000
	.4byte	0x00000000
