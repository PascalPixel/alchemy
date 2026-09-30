.syntax unified
	.thumb
	.global Func_0814cbcc
	.thumb_func
Func_0814cbcc:
	push {r5, r6, lr}
	ldr r6, .L_0814cbfc
	ldr r5, .L_0814cbf8
	ldr r1, .L_0814cc00
	ldr r4, .L_0814cc04
	movs r0, #0
.L_0814cbd8:
	adds r3, r0, #0
	subs r3, #8
	cmp r3, #127
	bhi .L_0814cc08
	ldrh r2, [r6]
	ldrb r3, [r4]
	subs r2, r2, r3
	cmp r2, #0
	bge .L_0814cbec
	movs r2, #0
.L_0814cbec:
	cmp r2, #240
	ble .L_0814cbf2
	movs r2, #240
.L_0814cbf2:
	strh r2, [r1]
	b .L_0814cc0a
	.2byte 0x0000
.L_0814cbf8:
	.4byte 0x0000fff1
.L_0814cbfc:
	.4byte gMapCellBuffer
.L_0814cc00:
	.4byte Data_02010082
.L_0814cc04:
	.4byte Data_0200fffa
.L_0814cc08:
	strh r5, [r1]
.L_0814cc0a:
	adds r0, #1
	adds r1, #2
	adds r4, #1
	cmp r0, #160
	bne .L_0814cbd8
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r1, [r3, #10]
	movs r2, #197
	lsls r2, r2, #8
	adds r2, #255
	ands r2, r1
	strh r2, [r3, #10]
	movs r2, #254
	ldrh r1, [r3, #10]
	lsls r2, r2, #7
	adds r2, #255
	ands r2, r1
	strh r2, [r3, #10]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r3, #10]
	ldr r0, .L_0814cc44
	adds r1, #64
	ldr r2, .L_0814cc48
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	pop {r5, r6, pc}
.L_0814cc44:
	.4byte Data_02010082
.L_0814cc48:
	.4byte 0xa2600001
