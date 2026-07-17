@ Reconstructed thumb disassembly of a call-graph-reached code region
@ (code interleaved with its data), verified byte-identical by build_asm.
.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_0800307c, 0x0800307c
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_08009008, 0x08009008
	.set sub_080ec728, 0x080ec728
	.set sub_080ec72a, 0x080ec72a
	.set sub_080ec752, 0x080ec752
	.global Region_080ec474
Region_080ec474:
	ldr r4, [r3, #4]
	ldr r3, [r3, #0]
	str r3, [sp, #72]
	str r4, [sp, #76]
	cmp r7, #83
	bgt .L_080ec4d8
	cmp r7, #48
	ble .L_080ec496
	ldr r0, [sp, #48]
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_080ec492
	adds r3, #63
.L_080ec492:
	asrs r3, r3, #6
	str r3, [sp, #48]
.L_080ec496:
	ldr r1, [sp, #44]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_080ec4a4
	adds r3, #63
.L_080ec4a4:
	asrs r3, r3, #6
	str r3, [sp, #44]
	ldr r2, [sp, #52]
	movs r3, #132
	lsls r3, r3, #16
	cmp r2, r3
	ble .L_080ec4bc
	ldr r4, [sp, #44]
	ldr r6, [pc, #572]
	adds r4, r4, r6
	str r4, [sp, #44]
	b .L_080ec4c6
.L_080ec4bc:
	ldr r7, [sp, #44]
	movs r0, #128
	lsls r0, r0, #6
	adds r7, r7, r0
	str r7, [sp, #44]
.L_080ec4c6:
	ldr r1, [sp, #56]
	ldr r3, [sp, #52]
	ldr r2, [sp, #48]
	ldr r4, [sp, #44]
	adds r1, r1, r2
	adds r3, r3, r4
	str r1, [sp, #56]
	str r3, [sp, #52]
	b .L_080ec540
.L_080ec4d8:
	mov r6, r10
	cmp r6, #170
	bgt .L_080ec540
	cmp r6, #107
	bgt .L_080ec4ec
	ldr r7, [sp, #48]
	movs r0, #128
	lsls r0, r0, #6
	adds r7, r7, r0
	str r7, [sp, #48]
.L_080ec4ec:
	ldr r1, [sp, #48]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_080ec4fa
	adds r3, #63
.L_080ec4fa:
	ldr r2, [sp, #56]
	asrs r3, r3, #6
	adds r2, r2, r3
	str r3, [sp, #48]
	mov r3, r10
	str r2, [sp, #56]
	cmp r3, #117
	bgt .L_080ec540
	movs r6, #138
	ldr r4, [sp, #52]
	lsls r6, r6, #16
	cmp r4, r6
	ble .L_080ec51e
	ldr r7, [sp, #44]
	ldr r0, [pc, #476]
	adds r7, r7, r0
	str r7, [sp, #44]
	b .L_080ec528
.L_080ec51e:
	ldr r1, [sp, #44]
	movs r2, #128
	lsls r2, r2, #6
	adds r1, r1, r2
	str r1, [sp, #44]
.L_080ec528:
	ldr r4, [sp, #44]
	lsls r3, r4, #3
	subs r3, r3, r4
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_080ec536
	adds r3, #63
.L_080ec536:
	ldr r6, [sp, #52]
	asrs r3, r3, #6
	adds r6, r6, r3
	str r3, [sp, #44]
	str r6, [sp, #52]
.L_080ec540:
	add r2, sp, #100
	movs r3, #0
	ldr r5, [pc, #432]
	str r3, [r2, #12]
	str r3, [r2, #4]
	mov r8, r3
	add r7, sp, #72
	adds r6, r2, #0
	add r5, r11
.L_080ec552:
	ldr r3, [pc, #424]
	mov r0, r8
	ldrb r2, [r3, r0]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r1, [sp, #56]
	asrs r3, r3, #1
	ldr r2, [pc, #412]
	lsls r3, r3, #16
	adds r3, r3, r1
	adds r3, r3, r2
	str r3, [r6, #0]
	ldr r3, [pc, #408]
	ldrb r2, [r3, r0]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r4, [sp, #52]
	asrs r3, r3, #1
	ldr r0, [pc, #400]
	lsls r3, r3, #16
	adds r3, r3, r4
	adds r3, r3, r0
	str r3, [r6, #8]
	adds r1, r6, #0
	adds r2, r7, #0
	ldmia r5!, {r0}
	movs r3, #0
	bl sub_08009008
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #8
	bne .L_080ec552
	mov r3, r10
	cmp r3, #76
	bne .L_080ec61a
	movs r4, #0
	ldr r7, [pc, #364]
	mov r8, r4
.L_080ec5a2:
	movs r6, #1
	ldr r3, [r7, #24]
	negs r6, r6
	cmp r3, r6
	bne .L_080ec60c
	bl sub_08004458
	movs r3, #255
	adds r6, r0, #0
	ands r6, r3
	bl sub_08004458
	ldr r3, [pc, #340]
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl sub_08002322
	adds r3, r6, #0
	muls r3, r0
	movs r0, #128
	lsls r0, r0, #15
	asrs r3, r3, #3
	adds r3, r3, r0
	str r3, [r7, #0]
	adds r0, r5, #0
	bl sub_0800231c
	adds r3, r6, #0
	muls r3, r0
	movs r1, #192
	lsls r1, r1, #15
	asrs r3, r3, #4
	adds r3, r3, r1
	str r3, [r7, #4]
	bl sub_08004458
	movs r2, #63
	ands r2, r0
	movs r3, #32
	subs r3, r3, r2
	lsls r3, r3, #12
	str r3, [r7, #12]
	bl sub_08004458
	movs r3, #31
	ands r3, r0
	negs r3, r3
	subs r3, #16
	lsls r3, r3, #13
	str r3, [r7, #16]
	movs r3, #0
	str r3, [r7, #24]
.L_080ec60c:
	movs r2, #1
	movs r3, #128
	add r8, r2
	lsls r3, r3, #3
	adds r7, #28
	cmp r8, r3
	bne .L_080ec5a2
.L_080ec61a:
	mov r4, r10
	cmp r4, #152
	bne .L_080ec63a
	ldr r2, [pc, #240]
	movs r0, #2
	movs r1, #96
	bl sub_0800307c
	ldr r2, [pc, #236]
	movs r3, #24
	add r2, r11
	str r3, [r2, #0]
	ldr r2, [pc, #232]
	movs r3, #0
	add r2, r11
	str r3, [r2, #0]
.L_080ec63a:
	movs r6, #152
	negs r6, r6
	add r6, r10
	mov r9, r6
	cmp r6, #87
	bhi .L_080ec6b6
	movs r6, #225
	movs r7, #0
	lsls r6, r6, #7
	mov r8, r7
	add r6, r11
.L_080ec650:
	mov r3, r8
	cmp r3, #0
	bge .L_080ec658
	adds r3, #3
.L_080ec658:
	asrs r2, r3, #2
	adds r3, r2, #0
	adds r3, #152
	cmp r10, r3
	blt .L_080ec6aa
	adds r3, #32
	cmp r10, r3
	bge .L_080ec6aa
	movs r0, #2
	ldrsh r2, [r6, r0]
	mov r4, r8
	movs r0, #3
	ands r0, r4
	adds r0, #5
	lsls r5, r0, #1
	ldr r4, [pc, #168]
	movs r1, #6
	ldrsh r3, [r6, r1]
	subs r1, r5, #2
	ldrh r1, [r4, r1]
	ldr r7, [pc, #160]
	adds r2, #112
	adds r3, #62
	add r1, r11
	subs r2, r2, r0
	subs r3, r3, r0
	adds r1, r1, r7
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #80]
	ldr r0, [sp, #68]
	bl sub_080072f4
	ldr r2, [r6, #0]
	ldr r3, [r6, #12]
	adds r2, r2, r3
	str r2, [r6, #0]
	ldr r3, [r6, #16]
	ldr r2, [r6, #4]
	adds r2, r2, r3
	str r2, [r6, #4]
.L_080ec6aa:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #28
	cmp r1, #32
	bne .L_080ec650
.L_080ec6b6:
	mov r2, r10
	cmp r2, #222
	bne sub_080ec752
	movs r5, #225
	movs r3, #0
	lsls r5, r5, #7
	mov r8, r3
	movs r6, #15
	add r5, r11
	bl sub_08004458
	ands r0, r6
	subs r0, #8
	str r0, [r5, #0]
	bl sub_08004458
	ldr r3, [r5, #0]
	ands r0, r6
	subs r0, #8
	str r0, [r5, #4]
	cmp r3, #0
	bge .L_080ec6e6
	subs r3, #4
	b .L_080ec6e8
.L_080ec6e6:
	adds r3, #4
.L_080ec6e8:
	str r3, [r5, #0]
	ldr r3, [r5, #4]
	cmp r3, #0
	bge sub_080ec728
	subs r3, #4
	b sub_080ec72a
