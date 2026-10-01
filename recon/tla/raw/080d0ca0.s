.syntax unified
	.thumb
	.global Func_080d0ca0
	.thumb_func
Func_080d0ca0:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	ldr r6, [r3]
	movs r1, #196
	movs r0, #169
	lsls r1, r1, #5
	lsls r0, r0, #1
	adds r5, r6, r1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080d0cbe
	b .L_080d0e0c
.L_080d0cbe:
	movs r2, #168
	lsls r2, r2, #6
	adds r2, #1
	adds r1, r6, r2
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r3, #0
	bne .L_080d0cd0
	b .L_080d0e0c
.L_080d0cd0:
	adds r2, #1
	adds r3, r6, r2
	ldrb r2, [r3]
	adds r2, #1
	strb r2, [r3]
	lsls r2, r2, #24
	movs r3, #0
	ldrsb r3, [r1, r3]
	asrs r2, r2, #24
	cmp r2, r3
	bge .L_080d0d08
	movs r3, #224
	movs r4, #136
	lsls r3, r3, #2
	lsls r4, r4, #3
	adds r1, r6, r3
	movs r0, #0
	adds r4, #255
.L_080d0cf4:
	ldrh r3, [r1]
	ldrh r2, [r5]
	adds r0, #1
	adds r3, r3, r2
	strh r3, [r1]
	adds r5, #2
	adds r1, #2
	cmp r0, r4
	ble .L_080d0cf4
	b .L_080d0d2a
.L_080d0d08:
	movs r1, #224
	movs r2, #224
	lsls r1, r1, #2
	lsls r2, r2, #4
	adds r0, r6, r1
	adds r1, r6, r2
	movs r2, #168
	ldr r3, .L_080d0d64
	lsls r2, r2, #4
	mov lr, r3
	.2byte 0xf800
	movs r3, #168
	lsls r3, r3, #6
	adds r3, #1
	adds r2, r6, r3
	movs r3, #0
	strb r3, [r2]
.L_080d0d2a:
	movs r1, #168
	lsls r1, r1, #6
	adds r3, r6, r1
	ldrb r3, [r3]
	movs r2, #1
	eors r2, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #7
	movs r2, #140
	adds r3, r6, r3
	lsls r2, r2, #6
	adds r4, r3, r2
	ldr r7, .L_080d0d5c
	movs r3, #248
	movs r2, #224
	ldr r5, .L_080d0d60
	lsls r3, r3, #7
	movs r0, #224
	lsls r2, r2, #2
	mov r12, r3
	lsls r0, r0, #1
	adds r1, r6, r2
	b .L_080d0d68
	.2byte 0x0000
.L_080d0d5c:
	.4byte 0x000003e0
.L_080d0d60:
	.4byte 0x0000001f
.L_080d0d64:
	.4byte IwramCopyWords
.L_080d0d68:
	ldrh r3, [r1]
	mov r2, r12
	ands r2, r3
	ldrh r3, [r1, #2]
	subs r0, #1
	lsls r3, r3, #16
	asrs r3, r3, #21
	ands r3, r7
	orrs r2, r3
	ldrh r3, [r1, #4]
	adds r1, #6
	lsls r3, r3, #16
	asrs r3, r3, #26
	ands r3, r5
	orrs r2, r3
	strh r2, [r4]
	adds r4, #2
	cmp r0, #0
	bne .L_080d0d68
	movs r3, #168
	lsls r3, r3, #6
	adds r1, r6, r3
	ldrb r3, [r1]
	movs r2, #1
	eors r3, r2
	strb r3, [r1]
	ldr r5, .L_080d0e10
	ldrb r2, [r1]
	movs r1, #140
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #7
	adds r0, r6, r3
	lsls r1, r1, #6
	adds r6, r0, r1
	ldr r4, .L_080d0e14
	ldrh r3, [r4]
	adds r1, r3, #0
	strh r4, [r4]
	ldrh r2, [r5]
	cmp r2, #31
	bgt .L_080d0dda
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r5
	adds r3, #4
	adds r2, #1
	stmia r3!, {r6}
	strh r2, [r5]
	movs r2, #160
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #112
	str r2, [r3]
.L_080d0dda:
	strh r1, [r4]
	ldrh r3, [r4]
	adds r6, r3, #0
	strh r4, [r4]
	ldrh r2, [r5]
	cmp r2, #31
	bgt .L_080d0e0a
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r1, #147
	adds r2, #1
	adds r3, r3, r5
	lsls r1, r1, #6
	adds r3, #4
	strh r2, [r5]
	adds r2, r0, r1
	stmia r3!, {r2}
	ldr r2, .L_080d0e18
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #112
	str r2, [r3]
.L_080d0e0a:
	strh r6, [r4]
.L_080d0e0c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d0e10:
	.4byte gIoWriteQueue
.L_080d0e14:
	.4byte 0x04000208
.L_080d0e18:
	.4byte 0x05000200
