.syntax unified
	.thumb
	.section .text.x020083dc,"ax",%progbits
	.balign 4
	.global Scene_Initialize
	.thumb_func
Scene_Initialize:
	push {r5, r6, lr}
	ldr r3, [pc, #96]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	ldr r2, [pc, #92]
	adds r3, r3, r1
	ldr r5, [pc, #92]
	str r2, [r3]
	subs r2, #71
	adds r3, r5, r2
	movs r1, #0
	ldrsh r6, [r3, r1]
	cmp r6, #10
	bne .L_020003dc_0
	ldr r0, [pc, #80]
	bl 0x020084a8
	movs r1, #226
	ldr r2, [pc, #76]
	lsls r1, r1, #1
	adds r3, r5, r1
	strh r2, [r3]
	movs r2, #227
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r6, [r3]
.L_020003dc_0:
	movs r0, #23
	bl 0x020084c0
	movs r1, #0
	bl 0x02008490
	movs r0, #24
	bl 0x020084c0
	movs r1, #0
	bl 0x02008490
	movs r0, #25
	bl 0x020084c0
	movs r1, #0
	bl 0x02008490
	movs r0, #0
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000209
	.4byte 0x02000240
	.4byte 0x0000012f
	.4byte 0x00000069
