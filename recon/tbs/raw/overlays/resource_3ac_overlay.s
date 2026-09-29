.syntax unified
	.thumb
	.section .text.x020081d8,"ax",%progbits
	.balign 4
	.global ItemMerchant_ReadMind
	.thumb_func
ItemMerchant_ReadMind:
	push {lr}
	ldr r0, [pc, #60]
	bl 0x02008498
	cmp r0, #0
	beq .L_020001d8_0
	bl 0x020084b0
	ldr r0, [pc, #48]
	bl 0x020084c8
	movs r0, #16
	movs r1, #0
	bl 0x020084d0
	bl 0x020084b8
	b .L_020001d8_1
.L_020001d8_0:
	bl 0x020084b0
	ldr r0, [pc, #28]
	bl 0x020084c8
	movs r0, #16
	movs r1, #0
	bl 0x020084d0
	bl 0x020084b8
.L_020001d8_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000941
	.4byte 0x000024fa
	.4byte 0x00001be0
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
