.syntax unified
	.thumb
	.global Func_080409f0
	.thumb_func
Func_080409f0:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	sub sp, #4
	ldr r6, [r3]
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #3
	movs r2, #15
	movs r3, #9
	movs r0, #7
	bl UiWindow_Create
	adds r5, r0, #0
	bl Func_08044460
	movs r1, #128
	movs r3, #0
	adds r2, r5, #0
	lsls r1, r1, #23
	str r3, [sp, #0]
	bl RenderOutput_Create
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #164
	adds r6, r6, r3
	str r0, [r6]
	adds r0, r6, #0
	movs r3, #12
	ldrsh r1, [r5, r3]
	movs r3, #14
	ldrsh r2, [r5, r3]
	lsls r1, r1, #3
	lsls r2, r2, #3
	subs r1, #4
	adds r2, #12
	bl Func_08108048
	adds r0, r5, #0
	add sp, #4
	pop {r5, r6, pc}
	.2byte 0x0000
