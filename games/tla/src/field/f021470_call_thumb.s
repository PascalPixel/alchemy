@ Why assembly: Func_08021470 (f021470.c) calls its target in ip through this
@ stub. The return address it sets has the Thumb bit, so a target that returns
@ by pop {pc} comes back in Thumb, to the bx pc that switches to ARM again: a
@ mode switch inside a function, in a linkage no procedure-call standard
@ produces.
.syntax unified
	.arm
	.global Func_080216f0
	.type Func_080216f0, %function
Func_080216f0:
	stmfd	sp!, {lr}
	add	lr, pc, #1
	bx	ip
	.thumb
	bx	pc
	movs	r0, r0
	.arm
	ldmfd	sp!, {pc}
