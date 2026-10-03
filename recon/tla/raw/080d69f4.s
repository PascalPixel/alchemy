.syntax unified
	.thumb
	.global Func_080d69f4
	.thumb_func
Func_080d69f4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #116]
	sub sp, #16
	mov r9, r3
	movs r1, #0
	ldr r3, .L_080d6ad8
	movs r2, #4
	str r1, [sp, #0]
	mov r5, r9
	add r2, sp
	adds r5, #8
	mov r10, r2
	mov r11, r3
.L_080d6a1e:
	ldrh r1, [r5, #28]
	mov r8, r1
	cmp r1, #0
	bne .L_080d6a5c
	bl Random16
	movs r3, #200
	muls r3, r0
	movs r2, #144
	lsls r2, r2, #17
	adds r6, r3, r2
	bl Random16
	lsls r3, r0, #2
	adds r3, r3, r0
	movs r2, #160
	lsls r3, r3, #2
	lsls r2, r2, #15
	subs r7, r2, r3
	str r6, [r5, #12]
	str r7, [r5, #20]
	movs r0, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Map_GetTerrainHeightFar
	movs r3, #120
	strh r3, [r5, #28]
	mov r3, r8
	str r0, [r5, #16]
	str r3, [r5, #24]
.L_080d6a5c:
	ldr r2, [r5, #24]
	ldr r3, [r5, #12]
	mov r1, r10
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r10
	ldr r3, [r5, #16]
	str r3, [r1, #4]
	ldr r3, [r5, #20]
	str r3, [r1, #8]
	bl Camera_WorldToScreen
	mov r3, r10
	movs r2, #2
	ldrsh r6, [r3, r2]
	ldrh r2, [r5, #28]
	movs r1, #10
	ldrsh r3, [r3, r1]
	subs r3, r3, r2
	adds r7, r3, #0
	bl Random16
	ldr r3, [r5, #24]
	movs r2, #128
	adds r3, r3, r0
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r5, #24]
	adds r3, r6, #0
	adds r3, #16
	subs r7, #8
	cmp r3, #255
	bhi .L_080d6b66
	movs r3, #32
	negs r3, r3
	cmp r7, r3
	blt .L_080d6b66
	cmp r7, #159
	bgt .L_080d6b66
	ldrh r3, [r5, #28]
	cmp r3, #59
	bhi .L_080d6ac2
	mov r1, r9
	ldr r2, [r1, #4]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	adds r2, #16
	ands r2, r3
	ldrh r3, [r5, #8]
	b .L_080d6aea
.L_080d6ac2:
	cmp r3, #89
	bhi .L_080d6adc
	mov r3, r9
	ldr r2, [r3, #4]
	movs r1, #192
	ldrh r3, [r5, #8]
	lsls r1, r1, #2
	adds r1, #255
	adds r2, #8
	b .L_080d6ae8
	.2byte 0x0000
.L_080d6ad8:
	.4byte 0xfffffc00
.L_080d6adc:
	mov r3, r9
	ldr r2, [r3, #4]
	movs r1, #192
	ldrh r3, [r5, #8]
	lsls r1, r1, #2
	adds r1, #255
.L_080d6ae8:
	ands r2, r1
.L_080d6aea:
	mov r1, r11
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
	ldr r3, .L_080d6b58
	ldrh r1, [r5, #8]
	ldr r2, [r3]
	movs r3, #1
	lsrs r2, r2, #3
	ands r2, r3
	lsls r3, r1, #22
	lsls r2, r2, #2
	lsrs r3, r3, #22
	adds r3, r3, r2
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #255
	ands r3, r2
	mov r2, r11
	ands r1, r2
	orrs r1, r3
	ldr r3, .L_080d6b50
	ldr r2, .L_080d6b54
	ands r6, r3
	ldrh r3, [r5, #6]
	strh r1, [r5, #8]
	ands r3, r2
	orrs r3, r6
	strh r3, [r5, #6]
	ldrb r2, [r5, #5]
	movs r3, #63
	adds r1, r3, #0
	ands r1, r2
	ldrb r2, [r5, #7]
	strb r7, [r5, #4]
	ands r3, r2
	movs r2, #64
	orrs r3, r2
	strb r3, [r5, #7]
	ldrb r3, [r5, #9]
	movs r2, #12
	orrs r3, r2
	movs r2, #13
	negs r2, r2
	strb r3, [r5, #9]
	adds r3, r2, #0
	ands r1, r3
	movs r3, #4
	orrs r1, r3
	b .L_080d6b5c
	.2byte 0x0000
.L_080d6b50:
	.4byte 0x000001ff
.L_080d6b54:
	.4byte 0xfffffe00
.L_080d6b58:
	.4byte gFrameCount
.L_080d6b5c:
	strb r1, [r5, #5]
	adds r0, r5, #0
	movs r1, #240
	bl Func_080140d8
.L_080d6b66:
	ldrh r3, [r5, #28]
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	strh r3, [r5, #28]
	ldr r2, [sp, #0]
	adds r5, #32
	adds r2, #1
	str r2, [sp, #0]
	cmp r2, #63
	bhi .L_080d6b80
	b .L_080d6a1e
.L_080d6b80:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
