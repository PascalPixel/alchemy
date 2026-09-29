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