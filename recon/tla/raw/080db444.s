.syntax unified
	.thumb
	.global Func_080db444
	.thumb_func
Func_080db444:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_080db484
	adds r3, r0, #0
	adds r3, #85
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080db478
	ldr r3, [r0, #16]
	ldr r2, .L_080db488
	movs r1, #0
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r0, #12]
	adds r0, r5, #0
	bl Func_080daecc
	b .L_080db484
.L_080db478:
	cmp r6, #0
	beq .L_080db484
	adds r0, r5, #0
	movs r1, #0
	bl Func_080daecc
.L_080db484:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080db488:
	.4byte 0xfff00000
