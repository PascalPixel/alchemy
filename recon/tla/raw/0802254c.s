.syntax unified
	.thumb
	.global Func_0802254c
	.thumb_func
Func_0802254c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #40
	str r1, [sp, #12]
	adds r6, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #24]
	adds r7, r0, #0
	ldrh r3, [r3, #4]
	mov r9, r2
	cmp r3, #0
	beq .L_08022572
	b .L_08022772
.L_08022572:
	add r1, sp, #28
	ldr r0, [sp, #12]
	mov r10, r1
	bl Render_ProjectPoint
	mov r2, r10
	ldr r3, [r2, #8]
	adds r5, r0, #0
	cmp r3, #0
	bne .L_08022588
	b .L_08022772
.L_08022588:
	ldr r3, [r2]
	movs r1, #152
	adds r3, #32
	lsls r1, r1, #1
	cmp r3, r1
	bls .L_08022596
	b .L_08022772
.L_08022596:
	ldr r3, [r2, #4]
	adds r3, #32
	cmp r3, #240
	bls .L_080225a0
	b .L_08022772
.L_080225a0:
	ldrb r2, [r7, #17]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080225ae
	movs r5, #128
	lsls r5, r5, #9
.L_080225ae:
	ldr r2, .L_080226d0
	ldr r1, [r7, #12]
	mov r8, r2
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_08021a84
	asrs r5, r5, #10
	str r0, [sp, #8]
	adds r5, #1
	asrs r5, r5, #1
	mov r3, r9
	lsls r5, r5, #11
	ldr r1, [r3]
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	mov r2, r9
	adds r6, r0, #0
	ldr r1, [r2, #4]
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	movs r3, #252
	lsls r3, r3, #9
	adds r5, r0, #0
	cmp r6, r3
	ble .L_080225f0
	adds r6, r3, #0
.L_080225f0:
	cmp r5, r3
	ble .L_080225f6
	adds r5, r3, #0
.L_080225f6:
	ldrb r3, [r7, #21]
	movs r0, #23
	ldrsb r0, [r7, r0]
	lsrs r3, r3, #1
	subs r0, r0, r3
	adds r1, r5, #0
	mov r11, r3
	mov lr, r8
	.2byte 0xf800
	str r0, [sp, #0]
	ldrb r3, [r7, #20]
	lsrs r3, r3, #1
	mov r9, r3
	movs r3, #128
	lsls r3, r3, #9
	cmp r6, r3
	bgt .L_0802261c
	cmp r5, r3
	ble .L_0802262e
.L_0802261c:
	mov r1, r9
	mov r2, r11
	movs r3, #3
	lsls r1, r1, #1
	lsls r2, r2, #1
	str r3, [sp, #4]
	mov r9, r1
	mov r11, r2
	b .L_08022644
.L_0802262e:
	movs r1, #1
	str r1, [sp, #4]
	cmp r6, r3
	bne .L_08022644
	ldrh r3, [r7, #18]
	cmp r3, #0
	bne .L_08022644
	cmp r5, r6
	bne .L_08022644
	movs r2, #0
	str r2, [sp, #4]
.L_08022644:
	ldr r3, [sp, #8]
	cmp r3, #0
	beq .L_0802264c
	negs r6, r6
.L_0802264c:
	ldr r1, [sp, #4]
	cmp r1, #0
	beq .L_08022660
	ldrh r0, [r7, #18]
	asrs r1, r6, #8
	asrs r2, r5, #8
	bl Func_0801440c
	mov r8, r0
	b .L_08022666
.L_08022660:
	lsrs r2, r6, #31
	lsls r2, r2, #3
	mov r8, r2
.L_08022666:
	cmp r6, #0
	beq .L_080226e0
	cmp r5, #0
	beq .L_080226e0
	ldr r3, .L_080226d0
	adds r1, r6, #0
	movs r0, #22
	ldrsb r0, [r7, r0]
	mov lr, r3
	.2byte 0xf800
	ldr r2, [r7, #4]
	ldr r1, [sp, #4]
	ldr r3, .L_080226d4
	ands r2, r3
	lsls r3, r1, #8
	mov r1, r8
	orrs r2, r3
	lsls r3, r1, #25
	mov r1, r10
	orrs r2, r3
	ldr r3, [r1]
	mov r1, r9
	subs r3, r3, r1
	adds r3, r3, r0
	lsls r3, r3, #23
	lsrs r3, r3, #7
	mov r1, r10
	orrs r2, r3
	ldr r3, [r1, #4]
	mov r1, r11
	subs r3, r3, r1
	ldr r1, [sp, #0]
	adds r3, r3, r1
	lsls r3, r3, #24
	lsrs r3, r3, #24
	orrs r2, r3
	str r2, [r7, #4]
	ldr r2, [sp, #72]
	cmp r2, #0
	bne .L_080226d8
	mov r1, r10
	ldr r3, [r1, #8]
	movs r2, #192
	asrs r3, r3, #1
	lsls r2, r2, #1
	subs r1, r2, r3
	cmp r1, #0
	bgt .L_080226c8
	movs r1, #1
.L_080226c8:
	adds r0, r7, #0
	bl Func_080140d8
	b .L_080226e0
.L_080226d0:
	.4byte IwramMulQ16
.L_080226d4:
	.4byte 0xc000fc00
.L_080226d8:
	adds r0, r7, #0
	ldr r1, [sp, #72]
	bl Func_080140d8
.L_080226e0:
	ldrb r2, [r7, #26]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08022790
	ldr r2, [sp, #12]
	add r0, sp, #16
	ldr r3, [r2]
	mov r1, r10
	str r3, [r0]
	movs r3, #0
	str r3, [r0, #4]
	ldr r3, [r2, #8]
	str r3, [r0, #8]
	bl Render_ProjectPoint
	ldr r3, [sp, #4]
	movs r4, #8
	cmp r3, #3
	bne .L_0802270a
	movs r4, #16
.L_0802270a:
	mov r1, r10
	ldr r2, [r1]
	ldr r3, .L_0802274c
	adds r0, r7, #0
	adds r0, #28
	subs r2, r2, r4
	ldrh r1, [r0, #6]
	ands r2, r3
	ldr r3, .L_08022750
	ands r3, r1
	orrs r3, r2
	mov r2, r10
	strh r3, [r0, #6]
	ldr r3, [r2, #4]
	lsrs r2, r4, #1
	subs r3, r3, r2
	adds r3, #2
	strb r3, [r0, #4]
	ldrb r2, [r0, #5]
	ldr r1, [sp, #4]
	movs r3, #4
	negs r3, r3
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #5]
	mov r2, r8
	movs r3, #31
	ands r2, r3
	lsls r1, r2, #1
	ldrb r2, [r0, #7]
	movs r3, #63
	b .L_08022754
	.2byte 0x0000
.L_0802274c:
	.4byte 0x000001ff
.L_08022750:
	.4byte 0xfffffe00
.L_08022754:
	negs r3, r3
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #7]
	ldr r3, [sp, #72]
	cmp r3, #0
	bne .L_0802276a
	movs r1, #0
	bl Func_080140d8
	b .L_08022790
.L_0802276a:
	ldr r1, [sp, #72]
	bl Func_080140d8
	b .L_08022790
.L_08022772:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	cmp r3, #0
	bne .L_08022790
	ldrb r2, [r7, #17]
	movs r5, #1
	adds r3, r5, #0
	ands r3, r2
	cmp r3, #0
	bne .L_08022790
	ldrb r0, [r7, #16]
	bl Resource_ActivateEntry
	strb r5, [r7, #25]
.L_08022790:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
