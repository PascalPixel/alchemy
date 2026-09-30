.syntax unified
	.thumb
	.global Func_08014ae0
	.thumb_func
Func_08014ae0:
	push {r5, lr}
	ldr r3, .L_08014b20
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08014b1c
	ldr r4, .L_08014b24
	ldrb r3, [r0]
	ldr r2, [r4]
	movs r1, #0
	adds r0, #1
	cmp r3, #0
	beq .L_08014b1a
	ldr r4, .L_08014b28
	movs r5, #240
	lsls r5, r5, #8
.L_08014afe:
	orrs r3, r5
	strh r3, [r2]
	adds r2, #2
	cmp r2, r4
	bne .L_08014b0a
	ldr r2, .L_08014b2c
.L_08014b0a:
	adds r1, #1
	cmp r1, #31
	bhi .L_08014b18
	ldrb r3, [r0]
	adds r0, #1
	cmp r3, #0
	bne .L_08014afe
.L_08014b18:
	ldr r4, .L_08014b24
.L_08014b1a:
	str r2, [r4]
.L_08014b1c:
	pop {r5, pc}
	.2byte 0x0000
.L_08014b20:
	.4byte Data_03001110
.L_08014b24:
	.4byte Data_030011c4
.L_08014b28:
	.4byte 0x06002500
.L_08014b2c:
	.4byte 0x06002000
