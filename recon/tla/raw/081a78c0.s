.syntax unified
	.thumb
	.global Func_081a78c0
	.thumb_func
Func_081a78c0:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	ldr r5, [r3]
	movs r2, #192
	movs r1, #224
	lsls r2, r2, #6
	lsls r1, r1, #5
	adds r2, #1
	adds r4, r5, r1
	adds r1, r5, r2
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r3, #0
	bne .L_081a78e2
	b .L_081a7a18
.L_081a78e2:
	adds r2, #1
	adds r3, r5, r2
	ldrb r2, [r3]
	adds r2, #1
	strb r2, [r3]
	lsls r2, r2, #24
	movs r3, #0
	ldrsb r3, [r1, r3]
	asrs r2, r2, #24
	cmp r2, r3
	bge .L_081a791a
	movs r3, #128
	movs r6, #160
	lsls r3, r3, #3
	lsls r6, r6, #3
	adds r1, r5, r3
	movs r0, #0
	adds r6, #255
.L_081a7906:
	ldrh r3, [r1]
	ldrh r2, [r4]
	adds r0, #1
	adds r3, r3, r2
	strh r3, [r1]
	adds r4, #2
	adds r1, #2
	cmp r0, r6
	ble .L_081a7906
	b .L_081a793e
.L_081a791a:
	movs r3, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #5
	lsls r2, r2, #3
	lsls r3, r3, #19
	adds r0, r5, r1
	adds r3, #212
	adds r1, r5, r2
	ldr r2, .L_081a7974
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #192
	lsls r3, r3, #6
	adds r3, #1
	adds r2, r5, r3
	movs r3, #0
	strb r3, [r2]
.L_081a793e:
	movs r1, #192
	lsls r1, r1, #6
	adds r3, r5, r1
	ldrb r2, [r3]
	movs r3, #1
	eors r3, r2
	lsls r3, r3, #10
	movs r2, #160
	adds r3, r5, r3
	lsls r2, r2, #6
	adds r4, r3, r2
	ldr r7, .L_081a796c
	movs r3, #248
	movs r2, #128
	ldr r6, .L_081a7970
	lsls r3, r3, #7
	movs r0, #128
	lsls r2, r2, #3
	mov r12, r3
	lsls r0, r0, #2
	adds r1, r5, r2
	b .L_081a7978
	.2byte 0x0000
.L_081a796c:
	.4byte 0x000003e0
.L_081a7970:
	.4byte 0x0000001f
.L_081a7974:
	.4byte 0x84000300
.L_081a7978:
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
	ands r3, r6
	orrs r2, r3
	strh r2, [r4]
	adds r4, #2
	cmp r0, #0
	bne .L_081a7978
	movs r3, #192
	lsls r3, r3, #6
	adds r1, r5, r3
	ldrb r3, [r1]
	movs r2, #1
	eors r3, r2
	strb r3, [r1]
	ldrb r3, [r1]
	movs r1, #160
	lsls r3, r3, #10
	adds r0, r5, r3
	ldr r5, .L_081a7a1c
	lsls r1, r1, #6
	adds r6, r0, r1
	ldr r4, .L_081a7a20
	ldrh r3, [r4]
	adds r1, r3, #0
	strh r4, [r4]
	ldrh r2, [r5]
	cmp r2, #31
	bgt .L_081a79e6
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
	adds r2, #128
	str r2, [r3]
.L_081a79e6:
	strh r1, [r4]
	ldrh r3, [r4]
	adds r6, r3, #0
	strh r4, [r4]
	ldrh r2, [r5]
	cmp r2, #31
	bgt .L_081a7a16
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r1, #168
	adds r2, #1
	adds r3, r3, r5
	lsls r1, r1, #6
	adds r3, #4
	strh r2, [r5]
	adds r2, r0, r1
	stmia r3!, {r2}
	ldr r2, .L_081a7a24
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #128
	str r2, [r3]
.L_081a7a16:
	strh r6, [r4]
.L_081a7a18:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081a7a1c:
	.4byte gIoWriteQueue
.L_081a7a20:
	.4byte 0x04000208
.L_081a7a24:
	.4byte 0x05000200
