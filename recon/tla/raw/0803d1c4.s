.syntax unified
	.thumb
	.global Func_0803d1c4
	.thumb_func
Func_0803d1c4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, [r0]
	movs r2, #255
	lsrs r3, r5, #8
	ands r5, r2
	ldr r2, .L_0803d2cc
	lsls r3, r3, #3
	ldr r1, [r2, r3]
	adds r3, #4
	ldr r2, [r2, r3]
	lsls r3, r5, #1
	ldrh r3, [r3, r2]
	mov r10, r0
	adds r1, r1, r3
	ldr r6, [r0, #4]
	ldr r0, [r0, #8]
	subs r2, r1, #1
	movs r3, #0
	movs r5, #128
	movs r4, #1
	mov r9, r2
	mov lr, r3
	movs r7, #1
	mov r8, r5
	b .L_0803d25e
.L_0803d202:
	adds r2, r0, #0
	movs r3, #1
	ands r2, r3
	asrs r0, r0, #1
	cmp r2, #0
	beq .L_0803d25e
	cmp r0, #0
	bne .L_0803d220
	ldrb r0, [r6]
	adds r6, #1
	adds r2, r0, #0
	ands r2, r3
	asrs r0, r0, #1
	mov r3, r8
	orrs r0, r3
.L_0803d220:
	cmp r2, #0
	beq .L_0803d25e
	movs r5, #1
	movs r2, #128
	movs r3, #0
	mov r11, r5
	mov r12, r2
.L_0803d22e:
	adds r2, r4, #0
	mov r5, r11
	ands r2, r5
	asrs r4, r4, #1
	cmp r2, #0
	beq .L_0803d250
	cmp r4, #0
	bne .L_0803d24c
	ldrb r4, [r1]
	adds r1, #1
	adds r2, r4, #0
	ands r2, r5
	asrs r4, r4, #1
	mov r5, r12
	orrs r4, r5
.L_0803d24c:
	cmp r2, #0
	bne .L_0803d254
.L_0803d250:
	adds r3, #1
	b .L_0803d25a
.L_0803d254:
	movs r2, #1
	add lr, r2
	subs r3, #1
.L_0803d25a:
	cmp r3, #0
	bge .L_0803d22e
.L_0803d25e:
	adds r2, r4, #0
	ands r2, r7
	asrs r4, r4, #1
	cmp r2, #0
	beq .L_0803d202
	cmp r4, #0
	bne .L_0803d27a
	ldrb r4, [r1]
	mov r3, r8
	adds r2, r4, #0
	asrs r4, r4, #1
	adds r1, #1
	ands r2, r7
	orrs r4, r3
.L_0803d27a:
	cmp r2, #0
	beq .L_0803d202
	mov r5, lr
	lsls r3, r5, #1
	adds r1, r3, r5
	lsls r3, r1, #2
	movs r2, #7
	ands r3, r2
	cmp r3, #0
	bne .L_0803d2a2
	mov r2, r9
	lsrs r3, r1, #1
	subs r3, r2, r3
	ldrb r2, [r3]
	subs r3, #1
	lsls r5, r2, #4
	ldrb r2, [r3]
	lsrs r3, r2, #4
	orrs r5, r3
	b .L_0803d2b6
.L_0803d2a2:
	lsrs r3, r1, #1
	mov r5, r9
	subs r3, r5, r3
	ldrb r2, [r3]
	subs r3, #1
	movs r1, #15
	ldrb r5, [r3]
	ands r2, r1
	lsls r2, r2, #8
	orrs r5, r2
.L_0803d2b6:
	mov r2, r10
	str r0, [r2, #8]
	str r5, [r2]
	str r6, [r2, #4]
	adds r0, r5, #0
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0803d2cc:
	.4byte Text_MessageContexts
