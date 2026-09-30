.syntax unified
	.thumb
	.global Func_080e1448
	.thumb_func
Func_080e1448:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #248
	ldr r6, [r3]
	ldr r2, .L_080e1504
	adds r3, r6, #0
	adds r3, #164
	movs r1, #0
	ldrsh r3, [r3, r1]
	sub sp, #4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	movs r2, #166
	lsrs r3, r3, #5
	adds r2, r2, r6
	mov r11, r3
	movs r1, #0
	ldrsh r3, [r2, r1]
	mov r9, r0
	mov r8, r3
	lsls r3, r3, #2
	add r3, r8
	lsls r3, r3, #4
	mov r10, r2
	adds r5, r3, r6
	movs r7, #1
.L_080e148c:
	mov r2, r11
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #16
	movs r2, #16
	ldr r3, .L_080e1508
	bl Func_080eaf98
	ldrb r3, [r5, #5]
	movs r1, #33
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r5, #9]
	mov r0, r9
	bl Func_080db9c0
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r5, #9]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	subs r7, #1
	strb r3, [r5, #9]
	adds r5, #40
	cmp r7, #0
	bge .L_080e148c
	mov r1, r8
	lsls r3, r1, #1
	movs r2, #0
	adds r3, #160
	strh r2, [r6, r3]
	adds r3, r6, #0
	mov r2, r9
	adds r3, #168
	str r2, [r3]
	mov r1, r10
	ldrh r3, [r1]
	ldr r2, .L_080e1500
	add sp, #4
	eors r3, r2
	mov r2, r10
	strh r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e1500:
	.4byte 0x00000001
.L_080e1504:
	.4byte ResourceTableEntries
.L_080e1508:
	.4byte 0x80004000
