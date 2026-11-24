@ Why assembly: WorldMap_UpdateAffineLines (world_map_lines.c) calls its target in ip through this
@ stub. The return address it sets has the Thumb bit, so a target that returns
@ by pop {pc} comes back in Thumb, to the bx pc that switches to ARM again: a
@ mode switch inside a function, in a linkage no procedure-call standard
@ produces.
.syntax unified
	.arm
	.global Matrix_CallInThumb
	.type Matrix_CallInThumb, %function
Matrix_CallInThumb:
	stmfd	sp!, {lr}
	add	lr, pc, #1
	bx	ip
	.thumb
	bx	pc
	movs	r0, r0
	.arm
	ldmfd	sp!, {pc}
