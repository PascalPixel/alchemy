@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_08007304, 0x08007304
	.set sub_08009038, 0x08009038
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080e155c, 0x080e155c
	.set sub_080e1e4e, 0x080e1e4e
	.set sub_080e3908, 0x080e3908
	.set sub_080e6d3c, 0x080e6d3c
	.set sub_080e6eac, 0x080e6eac
	.set sub_080ed408, 0x080ed408
	.global Region_080e21e8
Region_080e21e8:
	bl sub_080072f4
	movs r0, #47
	bl sub_08002dd8
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #7
	movs r0, #47
	bl sub_080ed408
	ldr r0, [pc, #760]
	str r5, [sp, #4]
	str r6, [sp, #0]
	ldr r1, [sp, #56]
	ldr r2, [sp, #60]
	ldr r4, [r0, #0]
	mov r3, r10
	ldr r0, [sp, #124]
	bl sub_080072f4
	movs r0, #47
	bl sub_08002dd8
	movs r1, #2
	str r1, [sp, #0]
	movs r2, #7
	movs r1, #7
	movs r3, #11
	movs r0, #47
	bl sub_080ed408
	ldr r2, [pc, #716]
	str r5, [sp, #4]
	str r6, [sp, #0]
	ldr r1, [sp, #56]
	ldr r4, [r2, #0]
	ldr r0, [sp, #124]
	mov r2, r8
	mov r3, r11
	bl sub_080072f4
	movs r0, #47
	bl sub_08002dd8
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #15
	movs r0, #47
	bl sub_080ed408
	str r5, [sp, #4]
	str r6, [sp, #0]
	ldr r5, [pc, #672]
	ldr r0, [sp, #124]
	ldr r4, [r5, #0]
	ldr r1, [sp, #56]
	ldr r2, [sp, #60]
	mov r3, r11
	bl sub_080072f4
	movs r0, #47
	bl sub_08002dd8
	ldr r7, [pc, #652]
	movs r0, #0
	mov r8, r0
	mov r9, r0
.L_080e2278:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_080e22d4
	bl sub_08004458
	movs r5, #63
	ands r5, r0
	bl sub_08004458
	ldr r3, [pc, #632]
	ldr r1, [sp, #60]
	adds r6, r0, #0
	mov r2, r11
	ands r6, r3
	lsls r3, r1, #16
	str r3, [r7, #0]
	lsls r3, r2, #16
	str r3, [r7, #4]
	adds r0, r6, #0
	bl sub_08002322
	adds r5, #64
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl sub_0800231c
	adds r3, r5, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl sub_08004458
	movs r3, #7
	ands r3, r0
	adds r3, #16
	str r3, [r7, #24]
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #4
	beq .L_080e22e0
.L_080e22d4:
	movs r5, #1
	ldr r0, [pc, #560]
	add r9, r5
	adds r7, #28
	cmp r9, r0
	bne .L_080e2278
.L_080e22e0:
	movs r1, #2
	str r1, [sp, #0]
	movs r2, #7
	movs r3, #15
	movs r0, #47
	movs r1, #7
	bl sub_080ed408
	ldr r2, [pc, #520]
	ldr r2, [r2, #0]
	movs r3, #0
	ldr r5, [pc, #520]
	mov r8, r2
	mov r9, r3
.L_080e22fc:
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_080e2364
	subs r3, #1
	str r3, [r5, #24]
	adds r0, r5, #0
	movs r1, #60
	movs r2, #0
	bl sub_080e3908
	movs r4, #240
	ldr r3, [r5, #4]
	lsls r4, r4, #15
	cmp r3, r4
	ble .L_080e2328
	ldr r3, [r5, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #16]
	b .L_080e2364
.L_080e2328:
	ldr r2, [r5, #0]
	ldr r0, [pc, #480]
	cmp r2, r0
	bhi .L_080e2364
	cmp r3, #0
	blt .L_080e2364
	ldr r0, [r5, #24]
	asrs r6, r2, #16
	asrs r7, r3, #16
	cmp r0, #0
	bge .L_080e2340
	adds r0, #15
.L_080e2340:
	asrs r0, r0, #4
	adds r0, #1
	lsls r4, r0, #1
	ldr r2, [pc, #456]
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #120]
	adds r1, r2, r1
	lsrs r2, r0, #31
	adds r2, r0, r2
	asrs r2, r2, #1
	subs r3, r7, r0
	str r0, [sp, #0]
	subs r2, r6, r2
	str r4, [sp, #4]
	ldr r0, [sp, #124]
	bl sub_08007304
.L_080e2364:
	movs r3, #1
	ldr r4, [pc, #416]
	add r9, r3
	adds r5, #28
	cmp r9, r4
	bne .L_080e22fc
	movs r0, #47
	bl sub_08002dd8
	ldr r5, [sp, #116]
	cmp r5, #15
	ble .L_080e23cc
	cmp r5, #32
	bne .L_080e238a
	movs r0, #128
	ldr r1, [pc, #400]
	lsls r0, r0, #11
	str r0, [sp, #72]
	str r1, [sp, #68]
.L_080e238a:
	ldr r2, [sp, #116]
	cmp r2, #31
	ble .L_080e23c2
	ldr r4, [sp, #72]
	ldr r3, [sp, #80]
	adds r3, r3, r4
	ldr r5, [sp, #76]
	str r3, [sp, #80]
	ldr r0, [sp, #68]
	lsls r3, r4, #4
	subs r3, r3, r4
	adds r5, r5, r0
	lsls r3, r3, #2
	str r5, [sp, #76]
	cmp r3, #0
	bge .L_080e23ac
	adds r3, #63
.L_080e23ac:
	ldr r1, [sp, #68]
	asrs r3, r3, #6
	str r3, [sp, #72]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_080e23be
	adds r3, #63
.L_080e23be:
	asrs r3, r3, #6
	str r3, [sp, #68]
.L_080e23c2:
	movs r0, #0
	ldr r1, [sp, #80]
	ldr r2, [sp, #76]
	bl sub_080e6d3c
.L_080e23cc:
	ldr r3, [sp, #116]
	subs r3, #16
	cmp r3, #127
	bhi .L_080e247c
	ldr r3, [sp, #80]
	ldr r4, [pc, #320]
	ldr r5, [pc, #320]
	movs r2, #0
	asrs r3, r3, #17
	mov r9, r2
	mov r11, r3
	movs r7, #3
	mov r10, r4
	mov r8, r5
.L_080e23e8:
	mov r4, r9
	ands r4, r7
	str r4, [sp, #8]
	bl sub_08004458
	ldr r3, [pc, #272]
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl sub_08002322
	ldr r4, [sp, #8]
	adds r6, r0, #0
	mov r0, r10
	ldrb r3, [r0, r4]
	lsls r6, r6, #3
	asrs r6, r6, #16
	lsrs r3, r3, #1
	adds r0, r5, #0
	add r6, r11
	subs r6, r6, r3
	bl sub_0800231c
	ldr r4, [sp, #8]
	mov r1, r8
	lsls r5, r0, #2
	ldrb r3, [r1, r4]
	adds r5, r5, r0
	lsls r5, r5, #3
	lsrs r3, r3, #1
	asrs r5, r5, #16
	subs r5, r5, r3
	bl sub_08004458
	ldr r3, [pc, #240]
	ands r0, r7
	ldrb r2, [r3, r0]
	adds r3, r7, #0
	orrs r3, r2
	movs r2, #2
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #47
	bl sub_080ed408
	ldr r4, [sp, #8]
	ldr r2, [pc, #220]
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r3, [pc, #216]
	mov r0, r10
	adds r1, r1, r3
	ldrb r3, [r0, r4]
	str r3, [sp, #0]
	mov r2, r8
	ldrb r3, [r2, r4]
	str r3, [sp, #4]
	ldr r3, [pc, #156]
	adds r5, #56
	ldr r4, [r3, #0]
	ldr r0, [sp, #124]
	adds r3, r5, #0
	adds r2, r6, #0
	bl sub_080072f4
	movs r0, #47
	bl sub_08002dd8
	movs r4, #1
	add r9, r4
	mov r5, r9
	cmp r5, #3
	bne .L_080e23e8
.L_080e247c:
	ldr r0, [sp, #116]
	cmp r0, #31
	bgt .L_080e248c
	movs r0, #8
	movs r1, #16
	bl sub_080e155c
	b .L_080e2494
.L_080e248c:
	movs r0, #4
	movs r1, #4
	bl sub_080e155c
.L_080e2494:
	bl sub_080cd52c
	ldr r3, [pc, #144]
	ldr r1, [sp, #128]
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2, #0]
	movs r0, #1
	bl sub_080030f8
	ldr r4, [sp, #116]
	adds r4, #1
	str r4, [sp, #116]
	cmp r4, #192
	beq .L_080e24b4
	b sub_080e1e4e
.L_080e24b4:
	movs r0, #46
	bl sub_08002dd8
	ldr r0, [pc, #116]
	bl sub_08004278
	movs r0, #0
	ldr r1, [sp, #80]
	ldr r2, [sp, #76]
	bl sub_080e6eac
	ldr r1, [pc, #104]
	ldr r0, [sp, #128]
	movs r5, #0
	mov r9, r5
	adds r5, r0, r1
.L_080e24d4:
	ldmia r5!, {r0}
	bl sub_08009038
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #9
	bne .L_080e24d4
	bl sub_080cdbc0
	add sp, #336
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
