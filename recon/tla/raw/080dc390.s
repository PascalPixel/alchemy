.syntax unified
	.thumb
	.global Camera_WorldToScreen
	.thumb_func
Camera_WorldToScreen:
	push {r5, r6, lr}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r1, #197
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	sub sp, #12
	adds r6, r0, #0
	cmp r3, #3
	bne .L_080dc3c0
	mov r5, sp
	adds r1, r5, #0
	bl Render_ProjectPoint
	ldr r3, [r5]
	lsls r3, r3, #16
	str r3, [r6]
	ldr r3, [r5, #4]
	lsls r3, r3, #16
	b .L_080dc3e0
.L_080dc3c0:
	ldr r2, [r2, #32]
	adds r3, r2, #0
	adds r3, #228
	adds r2, #232
	ldr r1, [r3]
	ldr r0, [r2]
	ldr r3, .L_080dc3ec
	ldr r2, [r6, #4]
	ands r1, r3
	ands r0, r3
	ldr r3, [r6]
	subs r3, r3, r1
	str r3, [r6]
	ldr r3, [r6, #8]
	subs r3, r3, r2
	subs r3, r3, r0
.L_080dc3e0:
	str r3, [r6, #8]
	movs r3, #0
	str r3, [r6, #4]
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080dc3ec:
	.4byte 0xffff0000
