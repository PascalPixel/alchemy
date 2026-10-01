.syntax unified
	.thumb
	.global UiMenu_SlideCursor
	.thumb_func
UiMenu_SlideCursor:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080a1aec
	ldr r3, [r3]
	ldr r2, .L_080a1af0
	mov r10, r3
	add r2, r10
	ldrh r3, [r2]
	mov r8, r1
	movs r1, #2
	sub sp, #4
	mov r9, r1
	cmp r3, #0
	beq .L_080a1af4
	movs r3, #0
	strh r3, [r2]
	b .L_080a1bb6
.L_080a1aec:
	.4byte gMenuWork
.L_080a1af0:
	.4byte 0x00000222
.L_080a1af4:
	mov r2, r10
	ldr r7, [r2, #20]
	ldrh r3, [r7, #22]
	lsls r3, r3, #23
	lsrs r3, r3, #23
	adds r3, #64
	ldrb r2, [r7, #20]
	strh r3, [r7, #6]
	adds r2, #64
	strh r2, [r7, #8]
	ldrh r2, [r7, #6]
	movs r3, #64
	add r8, r3
	adds r3, r2, #0
	subs r3, #8
	adds r0, #64
	cmp r3, #0
	ble .L_080a1b1e
	ldr r1, .L_080a1b94
	adds r3, r2, r1
	strh r3, [r7, #6]
.L_080a1b1e:
	ldrh r6, [r7, #8]
	adds r3, r6, #0
	subs r3, #8
	cmp r3, #0
	ble .L_080a1b30
	ldr r2, .L_080a1b94
	adds r3, r6, r2
	strh r3, [r7, #8]
	ldrh r6, [r7, #8]
.L_080a1b30:
	ldrh r5, [r7, #6]
	lsls r0, r0, #4
	lsls r5, r5, #4
	subs r0, r0, r5
	movs r1, #2
	adds r0, #1
	bl __divsi3
	mov r3, r8
	mov r11, r0
	lsls r6, r6, #4
	lsls r0, r3, #4
	subs r0, r0, r6
	adds r0, #1
	movs r1, #2
	bl __divsi3
	ldr r4, .L_080a1b88
	mov r8, r0
.L_080a1b56:
	mov r2, r10
	ldr r0, [r2, #16]
	ldrh r3, [r0, #12]
	add r5, r11
	lsls r3, r3, #3
	asrs r1, r5, #4
	adds r1, r1, r3
	subs r1, #56
	ldr r3, .L_080a1b8c
	strh r1, [r7, #6]
	ands r1, r4
	ands r1, r3
	ldr r2, .L_080a1b90
	ldrh r3, [r7, #22]
	ands r3, r2
	orrs r3, r1
	strh r3, [r7, #22]
	ldrh r3, [r0, #14]
	add r6, r8
	lsls r3, r3, #3
	asrs r2, r6, #4
	adds r2, r2, r3
	movs r3, #1
	b .L_080a1b98
	.2byte 0x0000
.L_080a1b88:
	.4byte 0x0000ffff
.L_080a1b8c:
	.4byte 0x000001ff
.L_080a1b90:
	.4byte 0xfffffe00
.L_080a1b94:
	.4byte 0x0000fff8
.L_080a1b98:
	negs r3, r3
	subs r2, #56
	add r9, r3
	strh r2, [r7, #8]
	mov r1, r9
	ands r2, r4
	strb r2, [r7, #20]
	cmp r1, #0
	beq .L_080a1bb6
	movs r0, #1
	str r4, [sp, #0]
	bl WaitFrames
	ldr r4, [sp, #0]
	b .L_080a1b56
.L_080a1bb6:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
