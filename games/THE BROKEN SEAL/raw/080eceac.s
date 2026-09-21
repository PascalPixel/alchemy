.syntax unified
	.thumb
	.set Func_080b5078, 0x080b5078
	.set Func_080030f8, 0x080030f8
	.set Func_080d4604, 0x080d4604
	.global Object_WaitTwelveFramesThenSetField18
	.thumb_func
Object_WaitTwelveFramesThenSetField18:
	push {r5, lr}
	adds r5, r0, #0
	movs r3, #36
	ldrsh r1, [r5, r3]
	ldr r0, [r5, #8]
	ldr r3, [pc, #32]
	movs r2, #24
	bl Func_080b5078
	movs r0, #12
	bl Func_080030f8
	movs r3, #3
	adds r0, r5, #0
	movs r1, #2
	str r3, [r5, #24]
	bl Func_080d4604
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00073333
