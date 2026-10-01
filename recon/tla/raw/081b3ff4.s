.syntax unified
	.thumb
	.global Func_081b3ff4
	.thumb_func
Func_081b3ff4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_081b419c
	ldr r3, .L_081b41a0
	sub sp, #44
	str r0, [sp, #40]
	movs r0, #0
	str r1, [sp, #36]
	str r0, [sp, #32]
	str r3, [r5]
	adds r6, r2, #0
	bl AudioTrack_ResetSlotBuckets
	ldr r2, [r5]
	movs r1, #136
	lsls r1, r1, #7
	adds r1, #64
	adds r3, r2, r1
	str r6, [r3]
	movs r6, #136
	ldr r4, [sp, #32]
	lsls r6, r6, #7
	movs r7, #136
	adds r6, #52
	lsls r7, r7, #7
	movs r0, #136
	adds r3, r2, r6
	adds r7, #56
	lsls r0, r0, #7
	str r4, [r3]
	adds r0, #60
	adds r3, r2, r7
	str r4, [r3]
	adds r3, r2, r0
	str r4, [r3]
	add r7, sp, #32
	movs r3, #208
	movs r4, #136
	lsls r3, r3, #6
	ldrb r7, [r7]
	lsls r4, r4, #7
	adds r1, r2, r3
	adds r4, #8
	movs r3, #128
	str r3, [r1]
	subs r0, #56
	adds r3, r2, r4
	strb r7, [r3]
	adds r2, r2, r0
	movs r3, #1
	str r3, [r2]
	movs r1, #168
	lsls r1, r1, #2
	ldr r2, [sp, #40]
	movs r0, #0
	bl Func_081b3ed8
	ldr r2, [r5]
	movs r1, #208
	ldr r3, [r2, r6]
	lsls r1, r1, #6
	lsls r3, r3, #2
	adds r1, #4
	adds r3, r3, r1
	ldr r3, [r2, r3]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_081b408a
	b .L_081b482c
.L_081b408a:
	ldr r0, .L_081b419c
	movs r4, #136
	ldr r1, [r0]
	lsls r4, r4, #7
	adds r4, #52
	adds r3, r1, r4
	ldr r3, [r3]
	movs r5, #136
	lsls r5, r5, #7
	adds r5, #48
	mov r12, r3
	adds r2, r1, r5
	movs r3, #1
	str r3, [r2]
	movs r2, #208
	mov r7, r12
	lsls r2, r2, #6
	lsls r3, r7, #2
	adds r2, #4
	adds r3, r3, r2
	ldr r3, [r1, r3]
	movs r4, #1
	negs r4, r4
	cmp r3, r4
	bne .L_081b40be
	b .L_081b4260
.L_081b40be:
	movs r5, #192
	lsls r3, r3, #2
	lsls r5, r5, #6
	adds r3, r3, r5
	ldr r6, [r1, r3]
	movs r7, #192
	movs r3, #136
	lsls r3, r3, #1
	lsls r7, r7, #2
	add r3, r12
	adds r7, #255
	cmp r3, r7
	ble .L_081b41a4
	cmp r6, #0
	bne .L_081b40de
	b .L_081b4260
.L_081b40de:
	ldr r2, [r6, #8]
	movs r1, #192
	mov r0, r12
	lsls r1, r1, #2
	subs r7, r0, r2
	adds r1, #255
	ands r7, r1
	subs r3, r7, #1
	cmp r3, #62
	bhi .L_081b4192
	ldr r3, .L_081b419c
	movs r0, #192
	adds r5, r2, #0
	lsls r0, r0, #2
	mov r2, r12
	ldr r1, [r3]
	adds r0, #255
	mov lr, r3
	adds r2, #1
	movs r3, #208
	ands r2, r0
	lsls r3, r3, #6
	lsls r2, r2, #2
	adds r3, #4
	adds r3, r3, r2
	mov r8, r3
	movs r2, #208
	adds r3, r5, #1
	ands r3, r0
	lsls r2, r2, #6
	lsls r3, r3, #2
	adds r2, #4
	adds r2, r2, r3
	mov r10, r2
	mov r3, r8
	ldr r2, [r1, r3]
	mov r3, r10
	ldr r1, [r1, r3]
	movs r4, #1
	cmp r2, r1
	bne .L_081b416c
.L_081b4130:
	movs r1, #16
	adds r4, #1
	adds r1, #255
	cmp r4, r1
	bgt .L_081b416c
	mov r2, lr
	mov r3, r12
	ldr r1, [r2]
	adds r2, r3, r4
	movs r3, #208
	ands r2, r0
	lsls r3, r3, #6
	lsls r2, r2, #2
	adds r3, #4
	adds r3, r3, r2
	mov r8, r3
	movs r2, #208
	adds r3, r5, r4
	ands r3, r0
	lsls r2, r2, #6
	lsls r3, r3, #2
	adds r2, #4
	adds r2, r2, r3
	mov r10, r2
	mov r3, r8
	ldr r2, [r1, r3]
	mov r3, r10
	ldr r1, [r1, r3]
	cmp r2, r1
	beq .L_081b4130
.L_081b416c:
	ldr r5, .L_081b419c
	movs r0, #136
	ldr r2, [r5]
	lsls r0, r0, #7
	adds r0, #48
	adds r1, r2, r0
	ldr r3, [r1]
	cmp r3, r4
	bge .L_081b4192
	movs r5, #136
	lsls r5, r5, #7
	adds r5, #44
	adds r3, r2, r5
	str r7, [r3]
	movs r7, #136
	lsls r7, r7, #1
	str r4, [r1]
	cmp r4, r7
	beq .L_081b4260
.L_081b4192:
	ldr r6, [r6]
	cmp r6, #0
	bne .L_081b40de
	b .L_081b4260
	.2byte 0x0000
.L_081b419c:
	.4byte Flash_Handler3
.L_081b41a0:
	.4byte gMapCellBuffer
.L_081b41a4:
	cmp r6, #0
	beq .L_081b4260
	movs r4, #208
	mov r2, r12
	lsls r4, r4, #6
	lsls r3, r2, #2
	adds r4, #8
	movs r0, #192
	ldr r1, .L_081b4398
	adds r4, r3, r4
	lsls r0, r0, #2
	str r4, [sp, #8]
	adds r0, #255
	adds r2, r3, #0
	mov r9, r0
	mov r10, r1
	mov r11, r2
.L_081b41c6:
	ldr r0, [r6, #8]
	mov r7, r12
	subs r5, r7, r0
	mov r1, r9
	ands r5, r1
	subs r3, r5, #1
	cmp r3, #62
	bhi .L_081b425a
	mov r2, r10
	ldr r1, [r2]
	mov r7, r9
	adds r3, r0, #1
	movs r2, #208
	ands r3, r7
	lsls r2, r2, #6
	ldr r7, [sp, #8]
	adds r2, #4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r3, [r1, r3]
	ldr r2, [r1, r7]
	movs r4, #1
	cmp r2, r3
	bne .L_081b4234
	ldr r1, .L_081b4398
	adds r7, r0, #0
	movs r2, #192
	movs r0, #208
	lsls r2, r2, #2
	lsls r0, r0, #6
	adds r2, #255
	adds r0, #8
	mov r8, r1
	mov lr, r2
	add r0, r11
.L_081b420c:
	movs r3, #16
	adds r4, #1
	adds r3, #255
	adds r0, #4
	cmp r4, r3
	bgt .L_081b4234
	mov r2, r8
	ldr r1, [r2]
	adds r3, r7, r4
	mov r2, lr
	ands r3, r2
	movs r2, #208
	lsls r2, r2, #6
	adds r2, #4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r3, [r1, r3]
	ldr r2, [r1, r0]
	cmp r2, r3
	beq .L_081b420c
.L_081b4234:
	mov r3, r10
	ldr r2, [r3]
	movs r7, #136
	lsls r7, r7, #7
	adds r7, #48
	adds r1, r2, r7
	ldr r3, [r1]
	cmp r3, r4
	bge .L_081b425a
	movs r0, #136
	lsls r0, r0, #7
	adds r0, #44
	adds r3, r2, r0
	str r5, [r3]
	str r4, [r1]
	movs r1, #136
	lsls r1, r1, #1
	cmp r4, r1
	beq .L_081b4260
.L_081b425a:
	ldr r6, [r6]
	cmp r6, #0
	bne .L_081b41c6
.L_081b4260:
	ldr r2, [sp, #32]
	cmp r2, #0
	beq .L_081b4268
	b .L_081b4680
.L_081b4268:
	ldr r3, .L_081b4398
	movs r4, #136
	ldr r2, [r3]
	lsls r4, r4, #7
	adds r4, #48
	adds r1, r2, r4
	ldr r5, [r1]
	str r5, [sp, #28]
	cmp r5, #1
	bgt .L_081b427e
	b .L_081b4764
.L_081b427e:
	movs r7, #136
	lsls r7, r7, #7
	adds r7, #44
	adds r3, r2, r7
	ldr r3, [r3]
	movs r0, #136
	str r3, [sp, #24]
	str r5, [sp, #20]
	lsls r0, r0, #7
	adds r0, #52
	adds r3, r2, r0
	ldr r3, [r3]
	movs r4, #192
	lsls r4, r4, #2
	adds r3, #1
	adds r4, #255
	ands r3, r4
	mov r12, r3
	movs r7, #208
	movs r3, #1
	str r3, [r1]
	mov r5, r12
	lsls r7, r7, #6
	lsls r3, r5, #2
	adds r7, #4
	adds r3, r3, r7
	ldr r3, [r2, r3]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_081b42be
	b .L_081b4458
.L_081b42be:
	movs r1, #192
	lsls r3, r3, #2
	lsls r1, r1, #6
	adds r3, r3, r1
	ldr r6, [r2, r3]
	movs r3, #136
	lsls r3, r3, #1
	add r3, r12
	cmp r3, r4
	ble .L_081b439c
	cmp r6, #0
	bne .L_081b42d8
	b .L_081b4458
.L_081b42d8:
	ldr r2, [r6, #8]
	movs r4, #192
	mov r3, r12
	lsls r4, r4, #2
	subs r7, r3, r2
	adds r4, #255
	ands r7, r4
	subs r3, r7, #1
	cmp r3, #62
	bhi .L_081b438e
	ldr r5, .L_081b4398
	movs r0, #192
	mov lr, r5
	adds r5, r2, #0
	mov r2, lr
	ldr r1, [r2]
	lsls r0, r0, #2
	mov r2, r12
	adds r0, #255
	adds r2, #1
	movs r3, #208
	ands r2, r0
	lsls r3, r3, #6
	lsls r2, r2, #2
	adds r3, #4
	adds r3, r3, r2
	mov r8, r3
	movs r2, #208
	adds r3, r5, #1
	ands r3, r0
	lsls r2, r2, #6
	lsls r3, r3, #2
	adds r2, #4
	adds r2, r2, r3
	mov r10, r2
	mov r3, r8
	ldr r2, [r1, r3]
	mov r3, r10
	ldr r1, [r1, r3]
	movs r4, #1
	cmp r2, r1
	bne .L_081b4368
.L_081b432c:
	movs r1, #16
	adds r4, #1
	adds r1, #255
	cmp r4, r1
	bgt .L_081b4368
	mov r2, lr
	mov r3, r12
	ldr r1, [r2]
	adds r2, r3, r4
	movs r3, #208
	ands r2, r0
	lsls r3, r3, #6
	lsls r2, r2, #2
	adds r3, #4
	adds r3, r3, r2
	mov r8, r3
	movs r2, #208
	adds r3, r5, r4
	ands r3, r0
	lsls r2, r2, #6
	lsls r3, r3, #2
	adds r2, #4
	adds r2, r2, r3
	mov r10, r2
	mov r3, r8
	ldr r2, [r1, r3]
	mov r3, r10
	ldr r1, [r1, r3]
	cmp r2, r1
	beq .L_081b432c
.L_081b4368:
	ldr r5, .L_081b4398
	movs r0, #136
	ldr r2, [r5]
	lsls r0, r0, #7
	adds r0, #48
	adds r1, r2, r0
	ldr r3, [r1]
	cmp r3, r4
	bge .L_081b438e
	movs r5, #136
	lsls r5, r5, #7
	adds r5, #44
	adds r3, r2, r5
	str r7, [r3]
	movs r7, #136
	lsls r7, r7, #1
	str r4, [r1]
	cmp r4, r7
	beq .L_081b4458
.L_081b438e:
	ldr r6, [r6]
	cmp r6, #0
	bne .L_081b42d8
	b .L_081b4458
	.2byte 0x0000
.L_081b4398:
	.4byte Flash_Handler3
.L_081b439c:
	cmp r6, #0
	beq .L_081b4458
	movs r4, #208
	mov r2, r12
	lsls r4, r4, #6
	lsls r3, r2, #2
	adds r4, #8
	movs r0, #192
	ldr r1, .L_081b4584
	adds r4, r3, r4
	lsls r0, r0, #2
	str r4, [sp, #4]
	adds r0, #255
	adds r2, r3, #0
	mov r9, r0
	mov r10, r1
	mov r11, r2
.L_081b43be:
	ldr r0, [r6, #8]
	mov r7, r12
	subs r5, r7, r0
	mov r1, r9
	ands r5, r1
	subs r3, r5, #1
	cmp r3, #62
	bhi .L_081b4452
	mov r2, r10
	ldr r1, [r2]
	mov r7, r9
	adds r3, r0, #1
	movs r2, #208
	ands r3, r7
	lsls r2, r2, #6
	ldr r7, [sp, #4]
	adds r2, #4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r3, [r1, r3]
	ldr r2, [r1, r7]
	movs r4, #1
	cmp r2, r3
	bne .L_081b442c
	ldr r1, .L_081b4584
	adds r7, r0, #0
	movs r2, #192
	movs r0, #208
	lsls r2, r2, #2
	lsls r0, r0, #6
	adds r2, #255
	adds r0, #8
	mov r8, r1
	mov lr, r2
	add r0, r11
.L_081b4404:
	movs r3, #16
	adds r4, #1
	adds r3, #255
	adds r0, #4
	cmp r4, r3
	bgt .L_081b442c
	mov r2, r8
	ldr r1, [r2]
	adds r3, r7, r4
	mov r2, lr
	ands r3, r2
	movs r2, #208
	lsls r2, r2, #6
	adds r2, #4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r3, [r1, r3]
	ldr r2, [r1, r0]
	cmp r2, r3
	beq .L_081b4404
.L_081b442c:
	mov r3, r10
	ldr r2, [r3]
	movs r7, #136
	lsls r7, r7, #7
	adds r7, #48
	adds r1, r2, r7
	ldr r3, [r1]
	cmp r3, r4
	bge .L_081b4452
	movs r0, #136
	lsls r0, r0, #7
	adds r0, #44
	adds r3, r2, r0
	str r5, [r3]
	str r4, [r1]
	movs r1, #136
	lsls r1, r1, #1
	cmp r4, r1
	beq .L_081b4458
.L_081b4452:
	ldr r6, [r6]
	cmp r6, #0
	bne .L_081b43be
.L_081b4458:
	ldr r3, .L_081b4584
	movs r4, #136
	ldr r2, [r3]
	lsls r4, r4, #7
	adds r4, #48
	adds r1, r2, r4
	ldr r3, [r1]
	cmp r3, #2
	bgt .L_081b446c
	b .L_081b4664
.L_081b446c:
	movs r5, #136
	lsls r5, r5, #7
	adds r3, #1
	adds r5, #52
	str r3, [sp, #12]
	adds r3, r2, r5
	ldr r3, [r3]
	ldr r7, [sp, #28]
	movs r4, #208
	adds r3, r3, r7
	mov r12, r3
	movs r3, #192
	lsls r3, r3, #2
	mov r0, r12
	adds r3, #255
	ands r0, r3
	movs r3, #1
	str r3, [r1]
	lsls r4, r4, #6
	lsls r3, r0, #2
	adds r4, #4
	adds r3, r3, r4
	ldr r3, [r2, r3]
	movs r5, #1
	negs r5, r5
	mov r12, r0
	cmp r3, r5
	bne .L_081b44a6
	b .L_081b4644
.L_081b44a6:
	movs r7, #192
	lsls r3, r3, #2
	lsls r7, r7, #6
	adds r3, r3, r7
	ldr r6, [r2, r3]
	movs r0, #192
	movs r3, #136
	lsls r3, r3, #1
	lsls r0, r0, #2
	add r3, r12
	adds r0, #255
	cmp r3, r0
	ble .L_081b4588
	cmp r6, #0
	bne .L_081b44c6
	b .L_081b4644
.L_081b44c6:
	ldr r2, [r6, #8]
	movs r3, #192
	mov r1, r12
	lsls r3, r3, #2
	adds r3, #255
	subs r7, r1, r2
	ands r7, r3
	subs r3, r7, #1
	cmp r3, #62
	bhi .L_081b457c
	ldr r5, .L_081b4584
	movs r0, #192
	mov lr, r5
	adds r5, r2, #0
	mov r2, lr
	ldr r1, [r2]
	lsls r0, r0, #2
	mov r2, r12
	adds r0, #255
	adds r2, #1
	movs r3, #208
	ands r2, r0
	lsls r3, r3, #6
	lsls r2, r2, #2
	adds r3, #4
	adds r3, r3, r2
	mov r8, r3
	movs r2, #208
	adds r3, r5, #1
	ands r3, r0
	lsls r2, r2, #6
	lsls r3, r3, #2
	adds r2, #4
	adds r2, r2, r3
	mov r10, r2
	mov r3, r8
	ldr r2, [r1, r3]
	mov r3, r10
	ldr r1, [r1, r3]
	movs r4, #1
	cmp r2, r1
	bne .L_081b4556
.L_081b451a:
	movs r1, #16
	adds r4, #1
	adds r1, #255
	cmp r4, r1
	bgt .L_081b4556
	mov r2, lr
	mov r3, r12
	ldr r1, [r2]
	adds r2, r3, r4
	movs r3, #208
	ands r2, r0
	lsls r3, r3, #6
	lsls r2, r2, #2
	adds r3, #4
	adds r3, r3, r2
	mov r8, r3
	movs r2, #208
	adds r3, r5, r4
	ands r3, r0
	lsls r2, r2, #6
	lsls r3, r3, #2
	adds r2, #4
	adds r2, r2, r3
	mov r10, r2
	mov r3, r8
	ldr r2, [r1, r3]
	mov r3, r10
	ldr r1, [r1, r3]
	cmp r2, r1
	beq .L_081b451a
.L_081b4556:
	ldr r5, .L_081b4584
	movs r0, #136
	ldr r2, [r5]
	lsls r0, r0, #7
	adds r0, #48
	adds r1, r2, r0
	ldr r3, [r1]
	cmp r3, r4
	bge .L_081b457c
	movs r5, #136
	lsls r5, r5, #7
	adds r5, #44
	adds r3, r2, r5
	str r7, [r3]
	movs r7, #136
	lsls r7, r7, #1
	str r4, [r1]
	cmp r4, r7
	beq .L_081b4644
.L_081b457c:
	ldr r6, [r6]
	cmp r6, #0
	bne .L_081b44c6
	b .L_081b4644
.L_081b4584:
	.4byte Flash_Handler3
.L_081b4588:
	cmp r6, #0
	beq .L_081b4644
	movs r4, #208
	mov r2, r12
	lsls r4, r4, #6
	lsls r3, r2, #2
	adds r4, #8
	movs r0, #192
	ldr r1, .L_081b4700
	adds r4, r3, r4
	lsls r0, r0, #2
	str r4, [sp, #0]
	adds r0, #255
	adds r2, r3, #0
	mov r9, r0
	mov r10, r1
	mov r11, r2
.L_081b45aa:
	ldr r0, [r6, #8]
	mov r7, r12
	subs r5, r7, r0
	mov r1, r9
	ands r5, r1
	subs r3, r5, #1
	cmp r3, #62
	bhi .L_081b463e
	mov r2, r10
	ldr r1, [r2]
	mov r7, r9
	adds r3, r0, #1
	movs r2, #208
	ands r3, r7
	lsls r2, r2, #6
	ldr r7, [sp, #0]
	adds r2, #4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r3, [r1, r3]
	ldr r2, [r1, r7]
	movs r4, #1
	cmp r2, r3
	bne .L_081b4618
	ldr r1, .L_081b4700
	adds r7, r0, #0
	movs r2, #192
	movs r0, #208
	lsls r2, r2, #2
	lsls r0, r0, #6
	adds r2, #255
	adds r0, #8
	mov r8, r1
	mov lr, r2
	add r0, r11
.L_081b45f0:
	movs r3, #16
	adds r4, #1
	adds r3, #255
	adds r0, #4
	cmp r4, r3
	bgt .L_081b4618
	mov r2, r8
	ldr r1, [r2]
	adds r3, r7, r4
	mov r2, lr
	ands r3, r2
	movs r2, #208
	lsls r2, r2, #6
	adds r2, #4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r3, [r1, r3]
	ldr r2, [r1, r0]
	cmp r2, r3
	beq .L_081b45f0
.L_081b4618:
	mov r3, r10
	ldr r2, [r3]
	movs r7, #136
	lsls r7, r7, #7
	adds r7, #48
	adds r1, r2, r7
	ldr r3, [r1]
	cmp r3, r4
	bge .L_081b463e
	movs r0, #136
	lsls r0, r0, #7
	adds r0, #44
	adds r3, r2, r0
	str r5, [r3]
	str r4, [r1]
	movs r1, #136
	lsls r1, r1, #1
	cmp r4, r1
	beq .L_081b4644
.L_081b463e:
	ldr r6, [r6]
	cmp r6, #0
	bne .L_081b45aa
.L_081b4644:
	ldr r2, .L_081b4700
	movs r4, #136
	ldr r3, [r2]
	lsls r4, r4, #7
	adds r4, #48
	adds r3, r3, r4
	ldr r3, [r3]
	ldr r5, [sp, #28]
	ldr r7, [sp, #12]
	adds r3, r3, r5
	str r3, [sp, #16]
	cmp r7, r3
	blt .L_081b4664
	movs r0, #1
	str r0, [sp, #20]
	str r0, [sp, #32]
.L_081b4664:
	ldr r1, .L_081b4700
	movs r4, #136
	ldr r2, [r1]
	ldr r5, [sp, #24]
	lsls r4, r4, #7
	adds r4, #44
	adds r3, r2, r4
	str r5, [r3]
	movs r7, #136
	lsls r7, r7, #7
	ldr r0, [sp, #20]
	adds r7, #48
	adds r2, r2, r7
	str r0, [r2]
.L_081b4680:
	ldr r1, .L_081b4700
	movs r2, #136
	ldr r5, [r1]
	lsls r2, r2, #7
	adds r2, #48
	adds r6, r5, r2
	ldr r3, [r6]
	cmp r3, #1
	ble .L_081b4764
	movs r3, #0
	movs r4, #136
	str r3, [sp, #32]
	lsls r4, r4, #7
	movs r7, #208
	adds r4, #8
	lsls r7, r7, #6
	adds r1, r5, r4
	adds r3, r5, r7
	ldr r3, [r3]
	ldrb r2, [r1]
	orrs r3, r2
	strb r3, [r1]
	ldr r0, [r6]
	cmp r0, #16
	bgt .L_081b4708
	movs r1, #136
	lsls r1, r1, #7
	adds r1, #44
	adds r3, r5, r1
	ldr r2, [r3]
	ldr r3, .L_081b46f8
	lsls r1, r2, #4
	ands r1, r3
	movs r3, #255
	ands r2, r3
	orrs r1, r2
	ldr r2, .L_081b46fc
	subs r3, r0, #1
	lsls r3, r3, #8
	ands r3, r2
	movs r2, #136
	lsls r2, r2, #7
	adds r2, #4
	adds r0, r5, r2
	ldr r2, [r0]
	orrs r1, r3
	movs r7, #136
	lsls r1, r1, #16
	lsls r7, r7, #7
	adds r3, r2, r4
	adds r7, #8
	adds r2, #1
	asrs r4, r1, #16
	lsrs r1, r1, #24
	strb r1, [r5, r3]
	str r2, [r0]
	adds r3, r2, r7
	adds r2, #1
	strb r4, [r5, r3]
	b .L_081b4704
.L_081b46f8:
	.4byte 0xfffff000
.L_081b46fc:
	.4byte 0x00000f00
.L_081b4700:
	.4byte Flash_Handler3
.L_081b4704:
	str r2, [r0]
	b .L_081b479e
.L_081b4708:
	movs r0, #136
	lsls r0, r0, #7
	adds r0, #44
	adds r3, r5, r0
	ldr r1, [r3]
	ldr r2, .L_081b4750
	lsls r3, r1, #4
	ands r3, r2
	movs r2, #255
	ands r1, r2
	orrs r3, r1
	movs r1, #136
	lsls r1, r1, #7
	adds r1, #4
	adds r0, r5, r1
	lsls r3, r3, #16
	ldr r2, [r0]
	asrs r4, r3, #16
	movs r3, #136
	lsls r3, r3, #7
	movs r7, #136
	adds r3, #8
	lsls r7, r7, #7
	adds r1, r2, r3
	adds r7, #8
	adds r2, #1
	lsrs r3, r4, #8
	strb r3, [r5, r1]
	adds r3, r2, r7
	adds r1, r2, #1
	str r2, [r0]
	strb r4, [r5, r3]
	str r1, [r0]
	movs r3, #136
	lsls r3, r3, #7
	b .L_081b4754
.L_081b4750:
	.4byte 0xfffff000
.L_081b4754:
	adds r3, #9
	adds r2, r2, r3
	ldr r3, [r6]
	adds r1, #1
	subs r3, #17
	strb r3, [r5, r2]
	str r1, [r0]
	b .L_081b479e
.L_081b4764:
	ldr r4, .L_081b4884
	movs r5, #136
	ldr r2, [r4]
	lsls r5, r5, #7
	adds r5, #4
	adds r4, r2, r5
	ldr r1, [r4]
	movs r7, #136
	adds r5, #48
	adds r3, r2, r5
	lsls r7, r7, #7
	ldr r3, [r3]
	adds r7, #8
	adds r0, r1, r7
	movs r7, #208
	lsls r7, r7, #6
	lsls r3, r3, #2
	adds r7, #4
	adds r3, r3, r7
	ldr r3, [r2, r3]
	adds r1, #1
	strb r3, [r2, r0]
	movs r0, #136
	lsls r0, r0, #7
	adds r0, #48
	adds r2, r2, r0
	movs r3, #1
	str r1, [r4]
	str r3, [r2]
.L_081b479e:
	ldr r7, .L_081b4884
	movs r5, #136
	ldr r3, [r7]
	lsls r5, r5, #7
	adds r5, #52
	ldr r0, [r3, r5]
	movs r6, #136
	movs r1, #168
	lsls r6, r6, #7
	lsls r1, r1, #2
	adds r6, #48
	adds r0, r0, r1
	ldr r2, [sp, #40]
	ldr r1, [r3, r6]
	bl Func_081b3ed8
	ldr r1, [r7]
	adds r5, r1, r5
	ldr r2, [r1, r6]
	ldr r3, [r5]
	adds r3, r3, r2
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #255
	ands r3, r2
	str r3, [r5]
	movs r3, #208
	lsls r3, r3, #6
	adds r1, r1, r3
	ldr r3, [r1]
	asrs r5, r3, #1
	str r5, [r1]
	cmp r5, #0
	bne .L_081b480a
	ldr r0, [sp, #36]
	bl Func_081b3fb0
	ldr r2, [r7]
	movs r4, #208
	lsls r4, r4, #6
	adds r1, r2, r4
	movs r3, #128
	movs r0, #136
	str r3, [r1]
	lsls r0, r0, #7
	movs r1, #136
	adds r0, #8
	lsls r1, r1, #7
	adds r3, r2, r0
	adds r1, #4
	strb r5, [r3]
	adds r2, r2, r1
	movs r3, #1
	str r3, [r2]
.L_081b480a:
	ldr r2, [r7]
	movs r4, #136
	lsls r4, r4, #7
	adds r4, #52
	adds r3, r2, r4
	ldr r3, [r3]
	movs r5, #208
	lsls r5, r5, #6
	lsls r3, r3, #2
	adds r5, #4
	adds r3, r3, r5
	ldr r3, [r2, r3]
	movs r7, #1
	negs r7, r7
	cmp r3, r7
	beq .L_081b482c
	b .L_081b408a
.L_081b482c:
	ldr r6, .L_081b4884
	movs r0, #136
	ldr r4, [r6]
	lsls r0, r0, #7
	movs r2, #208
	adds r0, #8
	lsls r2, r2, #6
	adds r1, r4, r0
	adds r3, r4, r2
	ldr r3, [r3]
	ldrb r2, [r1]
	movs r5, #0
	orrs r3, r2
	strb r3, [r1]
	movs r3, #136
	lsls r3, r3, #7
	adds r3, #4
	adds r1, r4, r3
	ldr r3, [r1]
	adds r2, r3, r0
	adds r3, #1
	strb r5, [r4, r2]
	adds r0, r3, r0
	str r3, [r1]
	adds r3, #1
	strb r5, [r4, r0]
	str r3, [r1]
	ldr r0, [sp, #36]
	bl Func_081b3fb0
	ldr r3, [r6]
	movs r4, #136
	lsls r4, r4, #7
	adds r4, #60
	adds r3, r3, r4
	ldr r0, [r3]
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081b4884:
	.4byte Flash_Handler3
