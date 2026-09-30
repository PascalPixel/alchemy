.syntax unified
	.thumb
	.global Func_08022010
	.thumb_func
Func_08022010:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	mov r8, r2
	ldrb r2, [r6, #20]
	sub sp, #4
	lsrs r0, r2, #1
	ldrb r2, [r6, #21]
	mov r9, r3
	mov r12, r2
	lsrs r4, r2, #1
	movs r2, #8
	mov r11, r2
	movs r2, #4
	ldr r3, [sp, #40]
	str r2, [sp, #0]
	movs r2, #1
	mov r10, r2
	ldmia r3!, {r2}
	ldr r5, [r3]
	movs r3, #128
	lsls r3, r3, #9
	cmp r2, r3
	bgt .L_0802204e
	cmp r5, r3
	ble .L_0802205e
.L_0802204e:
	movs r3, #3
	mov r10, r3
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #16
	lsls r0, r0, #1
	lsls r4, r4, #1
	mov r11, r2
.L_0802205e:
	asrs r1, r1, #16
	subs r7, r1, r0
	mov r2, r8
	mov r0, r9
	mov lr, r1
	subs r1, r0, r2
	movs r2, #23
	ldrsb r2, [r6, r2]
	mov r0, r12
	lsrs r3, r0, #1
	subs r3, r3, r2
	muls r3, r5
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
	asrs r1, r1, #16
	subs r1, r1, r4
	asrs r3, r3, #16
	ldrb r2, [r6, #5]
	subs r5, r1, r3
	movs r1, #4
	negs r1, r1
	mov r12, r10
	adds r3, r1, #0
	mov r0, r12
	ands r3, r2
	ldr r2, .L_080220d8
	orrs r3, r0
	ldr r4, .L_080220d4
	ldrh r0, [r6, #6]
	strb r3, [r6, #5]
	adds r3, r2, #0
	ands r7, r4
	ands r3, r0
	orrs r3, r7
	strh r3, [r6, #6]
	mov r0, r11
	mov r3, lr
	subs r7, r3, r0
	ldr r3, [sp, #36]
	mov r0, r9
	subs r3, r0, r3
	ldr r0, [sp, #0]
	strb r5, [r6, #4]
	asrs r3, r3, #16
	subs r5, r3, r0
	adds r3, r6, #0
	adds r3, #28
	ldrb r0, [r3, #5]
	ands r7, r4
	ands r1, r0
	mov r0, r12
	orrs r1, r0
	strb r1, [r3, #5]
	ldrh r1, [r3, #6]
	strb r5, [r3, #4]
	b .L_080220dc
	.2byte 0x0000
.L_080220d4:
	.4byte 0x000001ff
.L_080220d8:
	.4byte 0xfffffe00
.L_080220dc:
	ands r2, r1
	orrs r2, r7
	strh r2, [r3, #6]
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
