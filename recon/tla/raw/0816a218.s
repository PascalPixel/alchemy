.syntax unified
	.thumb
	.global Func_0816a218
	.thumb_func
Func_0816a218:
	push {r5, lr}
	adds r5, r0, #0
	movs r3, #36
	ldrsh r1, [r5, r3]
	ldr r0, [r5, #8]
	ldr r3, .L_0816a240
	movs r2, #24
	bl Func_08118078
	movs r0, #29
	bl WaitFrames
	movs r3, #4
	adds r0, r5, #0
	movs r1, #2
	str r3, [r5, #24]
	bl Func_08149bac
	pop {r5, pc}
	.2byte 0x0000
.L_0816a240:
	.4byte 0x000c3333
