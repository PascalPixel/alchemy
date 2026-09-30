.syntax unified
	.thumb
	.global Func_08022fe8
	.thumb_func
Func_08022fe8:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r6, #0
.L_08022fee:
	adds r1, r6, #0
	adds r0, r5, #0
	bl Func_08022f64
	adds r1, r6, #1
	adds r0, r5, #0
	bl Func_08022f64
	adds r1, r6, #2
	adds r0, r5, #0
	bl Func_08022f64
	adds r1, r6, #3
	adds r0, r5, #0
	bl Func_08022f64
	adds r6, #4
	movs r0, #1
	bl WaitFrames
	cmp r6, #127
	bls .L_08022fee
	pop {r5, r6, pc}
