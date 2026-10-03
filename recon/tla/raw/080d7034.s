.syntax unified
	.thumb
	.global Func_080d7034
	.thumb_func
Func_080d7034:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #116]
	sub sp, #28
	movs r1, #0
	str r0, [sp, #24]
	str r1, [sp, #20]
	adds r7, r0, #0
	ldr r3, [r3, #32]
	str r1, [sp, #4]
	str r3, [sp, #12]
	adds r3, #228
	str r3, [sp, #8]
	str r1, [sp, #0]
	str r1, [sp, #16]
	adds r7, #8
	mov r9, r1
	mov r11, r1
	mov r8, r1
.L_080d7068:
	ldrh r3, [r7, #28]
	cmp r3, #0
	bne .L_080d7070
	b .L_080d717c
.L_080d7070:
	ldr r2, [sp, #8]
	ldr r3, [sp, #8]
	ldr r2, [r2]
	ldr r3, [r3, #4]
	movs r0, #179
	lsls r0, r0, #1
	mov r8, r2
	mov r10, r3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080d708e
	ldrh r3, [r7, #28]
	adds r3, #1
	strh r3, [r7, #28]
.L_080d708e:
	ldrh r3, [r7, #28]
	ldr r2, .L_080d7138
	lsrs r3, r3, #1
	lsls r3, r3, #2
	adds r6, r3, r2
	bl Random16
	adds r5, r0, #0
	bl Random16
	ldr r2, [r7, #12]
	movs r3, #1
	ands r0, r3
	mov r1, r8
	ands r3, r5
	subs r2, r2, r1
	adds r3, r3, r0
	asrs r2, r2, #16
	lsrs r3, r3, #1
	adds r1, r2, r3
	subs r2, r1, #1
	mov r11, r2
	ldr r3, [r7, #20]
	ldr r2, [r7, #16]
	mov r0, r10
	subs r3, r3, r2
	subs r3, r3, r0
	cmp r3, #0
	bge .L_080d70d0
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
.L_080d70d0:
	movs r0, #0
	ldrsh r2, [r6, r0]
	asrs r3, r3, #16
	adds r3, r3, r2
	mov r8, r3
	adds r3, r1, #0
	adds r3, #15
	adds r6, #2
	cmp r3, #255
	bhi .L_080d7166
	movs r1, #32
	negs r1, r1
	cmp r8, r1
	blt .L_080d7166
	mov r2, r8
	cmp r2, #159
	bgt .L_080d7166
	ldr r3, [sp, #24]
	ldr r2, .L_080d7128
	ldr r1, [r3, #4]
	ldrh r3, [r6]
	mov r0, r8
	adds r1, r1, r3
	ldr r3, .L_080d712c
	strb r0, [r7, #4]
	ands r1, r3
	ldrh r3, [r7, #8]
	movs r0, #63
	ands r3, r2
	orrs r3, r1
	strh r3, [r7, #8]
	ldr r3, .L_080d7130
	mov r1, r11
	ands r1, r3
	ldr r2, .L_080d7134
	ldrh r3, [r7, #6]
	negs r0, r0
	ands r3, r2
	ldrb r2, [r7, #5]
	orrs r3, r1
	movs r1, #63
	strh r3, [r7, #6]
	b .L_080d713c
	.2byte 0x0000
.L_080d7128:
	.4byte 0xfffffc00
.L_080d712c:
	.4byte 0x000003ff
.L_080d7130:
	.4byte 0x000001ff
.L_080d7134:
	.4byte 0xfffffe00
.L_080d7138:
	.4byte Data_080f0b7c
.L_080d713c:
	adds r3, r1, #0
	ands r3, r2
	strb r3, [r7, #5]
	ldrb r3, [r7, #7]
	ldr r2, .L_080d7174
	ands r1, r3
	movs r3, #64
	orrs r1, r3
	ldr r3, .L_080d7178
	ldr r3, [r3]
	lsrs r3, r3, #1
	ands r3, r2
	adds r2, r0, #0
	lsls r3, r3, #4
	ands r1, r2
	orrs r1, r3
	strb r1, [r7, #7]
	adds r0, r7, #0
	movs r1, #240
	bl Func_080140d8
.L_080d7166:
	ldrh r3, [r7, #28]
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	strh r3, [r7, #28]
	b .L_080d717c
.L_080d7174:
	.4byte 0x00000001
.L_080d7178:
	.4byte gFrameCount
.L_080d717c:
	ldr r2, [sp, #20]
	cmp r2, #3
	bhi .L_080d721e
	ldrh r3, [r7, #28]
	cmp r3, #0
	bne .L_080d721e
	ldr r0, [sp, #24]
	movs r1, #129
	lsls r1, r1, #4
	adds r3, r0, r1
	ldr r5, [r3]
	cmp r5, #0
	bne .L_080d721e
	mov r2, r9
	cmp r2, #0
	beq .L_080d71cc
	ldr r3, [sp, #4]
	mov r1, r11
	str r3, [r7, #12]
	ldr r0, [sp, #0]
	mov r2, r8
	str r0, [r7, #20]
	movs r0, #0
	bl Map_GetTerrainHeightFar
	ldr r3, .L_080d71c8
	mov r1, r9
	subs r3, r3, r1
	strh r3, [r7, #28]
	str r0, [r7, #16]
	str r5, [r7, #24]
	ldr r2, [sp, #20]
	movs r3, #4
	adds r2, #1
	str r2, [sp, #20]
	add r9, r3
	b .L_080d721e
	.2byte 0x0000
.L_080d71c8:
	.4byte 0x0000003e
.L_080d71cc:
	bl Random16
	movs r3, #255
	ands r0, r3
	cmp r0, #0
	bne .L_080d721e
	ldr r0, [sp, #12]
	ldr r5, .L_080d723c
	ldr r6, [r0]
	bl Random16
	ldr r3, [r6]
	lsls r0, r0, #8
	adds r3, r3, r0
	adds r3, r3, r5
	str r3, [sp, #4]
	bl Random16
	ldr r3, [r6, #8]
	ldr r1, [sp, #4]
	lsls r0, r0, #8
	adds r3, r3, r0
	adds r3, r3, r5
	str r3, [sp, #0]
	mov r2, r8
	str r1, [r7, #12]
	str r3, [r7, #20]
	movs r0, #0
	mov r1, r11
	bl Map_GetTerrainHeightFar
	movs r3, #30
	mov r2, r9
	str r0, [r7, #16]
	strh r3, [r7, #28]
	str r2, [r7, #24]
	ldr r3, [sp, #20]
	movs r0, #4
	adds r3, #1
	str r3, [sp, #20]
	mov r9, r0
.L_080d721e:
	ldr r1, [sp, #16]
	adds r7, #32
	adds r1, #1
	str r1, [sp, #16]
	cmp r1, #63
	bhi .L_080d722c
	b .L_080d7068
.L_080d722c:
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d723c:
	.4byte 0xff800000
