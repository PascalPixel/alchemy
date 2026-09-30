.syntax unified
	.thumb
	.global Func_08029810
	.thumb_func
Func_08029810:
	push {r5, r6, lr}
	adds r6, r0, #0
	ldr r2, [r6, #80]
	movs r1, #128
	ldrh r3, [r2, #18]
	lsls r1, r1, #3
	adds r3, r3, r1
	strh r3, [r2, #18]
	bl Random16
	adds r5, r0, #0
	bl Random16
	ldr r3, [r6, #8]
	lsrs r0, r0, #1
	lsrs r5, r5, #1
	subs r5, r5, r0
	adds r3, r3, r5
	str r3, [r6, #8]
	pop {r5, r6, pc}
