.syntax unified
	.thumb
	.global Func_08101860
	.thumb_func
Func_08101860:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	mov r8, r1
	movs r1, #0
	str r2, [sp, #8]
	str r3, [sp, #4]
	str r1, [sp, #0]
	mov r2, r8
	ldrh r3, [r2]
	mov r12, r0
	mov r10, r1
	mov r11, r1
	cmp r3, #0
	beq .L_081018f8
	movs r3, #252
	ldr r5, [sp, #8]
	lsls r3, r3, #6
	adds r3, #255
	mov lr, r3
	mov r0, r8
	subs r5, #2
.L_08101896:
	ldrh r2, [r0]
	mov r3, lr
	ands r3, r2
	strh r3, [r5, #2]
	movs r1, #1
	add r10, r1
	mov r1, r12
	ldrh r2, [r1]
	ldrh r3, [r0]
	adds r5, #2
	eors r3, r2
	mov r2, lr
	ands r3, r2
	movs r4, #0
	cmp r3, #0
	beq .L_081018ce
	ldr r7, .L_081018e0
	adds r6, r0, #0
.L_081018ba:
	adds r4, #1
	cmp r4, #31
	bgt .L_081018ce
	adds r1, #4
	ldrh r3, [r6]
	ldrh r2, [r1]
	eors r3, r2
	ands r3, r7
	cmp r3, #0
	bne .L_081018ba
.L_081018ce:
	cmp r4, #32
	bne .L_081018e8
	movs r3, #1
	add r11, r3
	ldr r2, .L_081018e4
	ldrh r3, [r5]
	orrs r3, r2
	strh r3, [r5]
	b .L_081018e8
.L_081018e0:
	.4byte 0x00003fff
.L_081018e4:
	.4byte 0x00008000
.L_081018e8:
	mov r3, r8
	adds r0, #4
	adds r3, #124
	cmp r0, r3
	bgt .L_081018f8
	ldrh r3, [r0]
	cmp r3, #0
	bne .L_08101896
.L_081018f8:
	mov r2, r12
	ldrh r3, [r2]
	movs r1, #0
	mov r9, r1
	cmp r3, #0
	beq .L_08101988
	ldr r2, [sp, #8]
	mov r1, r10
	lsls r3, r1, #1
	mov lr, r12
	adds r0, r3, r2
	movs r7, #0
.L_08101910:
	mov r1, r12
	ldrh r3, [r7, r1]
	mov r1, r8
	ldrh r2, [r1]
	movs r4, #0
	eors r3, r2
	movs r2, #252
	lsls r2, r2, #6
	adds r2, #255
	ands r3, r2
	cmp r3, #0
	beq .L_08101948
	ldr r6, .L_08101944
	mov r5, lr
.L_0810192c:
	adds r4, #1
	cmp r4, #31
	bgt .L_08101948
	adds r1, #4
	ldrh r3, [r5]
	ldrh r2, [r1]
	eors r3, r2
	ands r3, r6
	cmp r3, #0
	bne .L_0810192c
	b .L_08101948
	.2byte 0x0000
.L_08101944:
	.4byte 0x00003fff
.L_08101948:
	cmp r4, #32
	bne .L_0810196a
	ldr r3, [sp, #0]
	mov r1, r12
	adds r3, #1
	str r3, [sp, #0]
	ldrh r3, [r7, r1]
	movs r2, #252
	lsls r2, r2, #6
	adds r2, #255
	ands r2, r3
	ldr r3, .L_08101984
	orrs r2, r3
	strh r2, [r0]
	movs r2, #1
	adds r0, #2
	add r10, r2
.L_0810196a:
	movs r1, #1
	add r9, r1
	movs r3, #4
	mov r2, r9
	adds r7, #4
	add lr, r3
	cmp r2, #31
	bgt .L_08101988
	mov r1, r12
	ldrh r3, [r7, r1]
	cmp r3, #0
	bne .L_08101910
	b .L_08101988
.L_08101984:
	.4byte 0x00004000
.L_08101988:
	ldr r3, [sp, #4]
	mov r2, r11
	str r2, [r3]
	ldr r1, [sp, #0]
	ldr r3, [sp, #44]
	mov r0, r10
	str r1, [r3]
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
