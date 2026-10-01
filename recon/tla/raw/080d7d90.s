.syntax unified
	.thumb
	.global Func_080d7d90
	.thumb_func
Func_080d7d90:
	push {lr}
	ldr r1, .L_080d7db8
	ldr r3, [r0, #28]
	ldr r2, [r0, #24]
	adds r3, r3, r1
	str r3, [r0, #28]
	ldrh r3, [r0, #6]
	adds r2, r2, r1
	movs r1, #128
	lsls r1, r1, #6
	adds r3, r3, r1
	strh r3, [r0, #6]
	movs r3, #192
	lsls r3, r3, #6
	str r2, [r0, #24]
	cmp r2, r3
	bge .L_080d7db6
	bl Object_Destroy
.L_080d7db6:
	pop {pc}
.L_080d7db8:
	.4byte 0xfffffe40
