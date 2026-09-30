.syntax unified
	.thumb
	.global Func_080e70f8
	.thumb_func
Func_080e70f8:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	movs r1, #200
	ldr r3, [r3, #92]
	lsls r1, r1, #5
	adds r1, #48
	adds r3, r3, r1
	adds r6, r0, #0
	ldr r5, [r3]
	bl Object_GetById
	adds r1, r0, #0
	cmp r5, #0
	beq .L_080e7124
	ldr r3, [r5, #8]
	str r3, [r1, #8]
	ldr r3, [r5, #12]
	str r3, [r1, #12]
	ldr r3, [r5, #16]
	str r3, [r1, #16]
.L_080e7124:
	movs r2, #197
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080e7142
	adds r0, r1, #0
	adds r0, #85
	movs r2, #0
	movs r3, #2
	strb r3, [r0]
	str r2, [r1, #20]
	b .L_080e714a
.L_080e7142:
	adds r2, r1, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
.L_080e714a:
	movs r3, #184
	lsls r3, r3, #5
	adds r3, #10
	str r3, [r1, #72]
	movs r0, #50
	bl WaitFrames
	ldr r3, .L_080e7168
	movs r1, #176
	lsls r1, r1, #1
	adds r2, r7, r1
	orrs r6, r3
	strh r6, [r2]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e7168:
	.4byte 0x00001000
