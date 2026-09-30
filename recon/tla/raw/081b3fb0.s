.syntax unified
	.thumb
	.global Func_081b3fb0
	.thumb_func
Func_081b3fb0:
	push {r5, r6, lr}
	ldr r3, .L_081b3ff0
	movs r1, #136
	ldr r2, [r3]
	lsls r1, r1, #7
	adds r1, #4
	adds r3, r2, r1
	ldr r3, [r3]
	adds r6, r0, #0
	movs r0, #0
	cmp r3, #0
	beq .L_081b3fec
	movs r3, #136
	lsls r3, r3, #7
	adds r3, #60
	adds r4, r2, r3
	subs r3, #52
	adds r5, r2, r1
	adds r1, r2, r3
.L_081b3fd6:
	ldrb r3, [r1]
	ldr r2, [r4]
	adds r0, #1
	strb r3, [r6, r2]
	adds r1, #1
	ldr r3, [r4]
	adds r3, #1
	str r3, [r4]
	ldr r3, [r5]
	cmp r0, r3
	bne .L_081b3fd6
.L_081b3fec:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_081b3ff0:
	.4byte Flash_Handler3
