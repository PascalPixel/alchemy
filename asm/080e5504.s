@ Reconstructed thumb disassembly of a call-graph-reached code region
@ (code interleaved with its data), verified byte-identical by build_asm.
.syntax unified
	.thumb
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_080072f4, 0x080072f4
	.set sub_080b5070, 0x080b5070
	.set sub_080b5098, 0x080b5098
	.set sub_080d6888, 0x080d6888
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080e46f0, 0x080e46f0
	.set sub_080e57b4, 0x080e57b4
	.set sub_080e640e, 0x080e640e
	.global Region_080e5504
Region_080e5504:
	ldr r0, [pc, #648]
	bl sub_080e46f0
	b .L_080e551a
	ldr r0, [pc, #644]
	bl sub_080e46f0
	b .L_080e551a
	ldr r0, [pc, #640]
	bl sub_080e46f0
.L_080e551a:
	ldr r1, [sp, #96]
	cmp r1, #11
	beq .L_080e5608
	cmp r1, #8
	beq .L_080e5608
	cmp r1, #32
	beq .L_080e5608
	ldr r2, [sp, #84]
	subs r2, #4
	cmp r2, #11
	bhi .L_080e556a
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	lsls r1, r3, #4
	subs r1, r1, r3
	ldr r2, [sp, #92]
	ldr r5, [sp, #60]
	lsls r1, r1, #6
	adds r1, r2, r1
	movs r3, #162
	ldr r2, [r5, #0]
	lsls r3, r3, #7
	ldr r0, [sp, #56]
	adds r1, r1, r3
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r0, #4]
	movs r0, #20
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	ldr r5, [sp, #52]
	asrs r2, r2, #1
	subs r2, #8
	subs r3, #24
	ldr r4, [r5, #4]
	ldr r0, [sp, #88]
	bl sub_080072f4
.L_080e556a:
	bl sub_080049ac
	ldr r1, [sp, #80]
	ldr r0, [sp, #80]
	adds r1, #12
	bl sub_080051d8
	ldr r0, [sp, #84]
	cmp r0, #3
	ble .L_080e5608
	movs r2, #112
	movs r1, #0
	add r2, sp
	mov r10, r1
	mov r9, r2
.L_080e5588:
	mov r5, r10
	lsrs r3, r5, #31
	add r3, r10
	asrs r7, r3, #1
	lsls r3, r7, #3
	subs r3, r3, r7
	ldr r0, [sp, #92]
	lsls r3, r3, #2
	movs r1, #225
	adds r3, r0, r3
	lsls r1, r1, #7
	adds r1, r1, r3
	ldr r5, [r1, #24]
	mov r8, r1
	cmp r5, #0
	ble .L_080e55fe
	mov r6, r9
	adds r1, r6, #0
	mov r0, r8
	bl sub_080e3944
	ldr r2, [r6, #0]
	asrs r5, r5, #4
	lsrs r3, r2, #31
	adds r5, #1
	adds r2, r2, r3
	lsls r4, r5, #1
	asrs r2, r2, #1
	ldr r1, [pc, #472]
	str r2, [r6, #0]
	subs r3, r4, #2
	ldrh r1, [r1, r3]
	ldr r3, [sp, #76]
	adds r1, r3, r1
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r6, #4]
	movs r0, #1
	subs r3, r3, r5
	str r5, [sp, #0]
	str r4, [sp, #4]
	ldr r5, [sp, #52]
	ands r0, r7
	lsls r0, r0, #2
	ldr r4, [r0, r5]
	ldr r0, [sp, #88]
	bl sub_080072f4
	mov r0, r8
	movs r1, #60
	ldr r2, [pc, #428]
	bl sub_080e38b8
	mov r0, r8
	ldr r3, [r0, #24]
	subs r3, #1
	str r3, [r0, #24]
.L_080e55fe:
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #128
	bne .L_080e5588
.L_080e5608:
	ldr r3, [sp, #96]
	cmp r3, #7
	beq .L_080e561c
	cmp r3, #13
	beq .L_080e561c
	cmp r3, #18
	beq .L_080e561c
	cmp r3, #19
	beq .L_080e561c
	b sub_080e57b4
.L_080e561c:
	ldr r5, [sp, #84]
	cmp r5, #50
	bne .L_080e563c
	ldr r0, [sp, #92]
	ldr r1, [pc, #380]
	adds r3, r0, r1
	ldr r3, [r3, #0]
	ldr r0, [r3, #8]
	movs r3, #1
	movs r2, #0
	negs r3, r3
	str r2, [sp, #0]
	movs r1, #7
	adds r2, r3, #0
	bl sub_080d6888
.L_080e563c:
	ldr r2, [sp, #84]
	cmp r2, #79
	bne .L_080e565c
	ldr r0, [pc, #352]
	ldr r5, [sp, #92]
	adds r3, r5, r0
	ldr r3, [r3, #0]
	ldr r0, [r3, #8]
	movs r3, #1
	movs r2, #0
	negs r3, r3
	str r2, [sp, #0]
	movs r1, #0
	adds r2, r3, #0
	bl sub_080d6888
.L_080e565c:
	ldr r1, [sp, #84]
	cmp r1, #12
	bne .L_080e56ae
	movs r2, #0
	ldr r5, [pc, #320]
	mov r10, r2
	movs r6, #255
.L_080e566a:
	ldr r0, [sp, #28]
	ldr r3, [r0, #8]
	str r3, [r5, #0]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	str r3, [r5, #8]
	bl sub_08004458
	ands r0, r6
	subs r0, #128
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl sub_08004458
	ands r0, r6
	subs r0, #128
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl sub_08004458
	movs r1, #1
	ands r0, r6
	subs r0, #128
	add r10, r1
	lsls r0, r0, #10
	movs r3, #0
	mov r2, r10
	str r0, [r5, #20]
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #64
	bne .L_080e566a
.L_080e56ae:
	ldr r3, [sp, #84]
	cmp r3, #11
	bgt .L_080e56b8
	bl sub_080e640e
.L_080e56b8:
	ldr r0, [sp, #92]
	ldr r1, [pc, #232]
	adds r5, r0, r1
	ldr r3, [r5, #0]
	ldr r0, [r3, #8]
	bl sub_080b5098
	ldr r3, [r5, #0]
	ldr r0, [r0, #0]
	mov r8, r0
	ldr r0, [r3, #8]
	bl sub_080b5070
	lsrs r3, r0, #31
	adds r0, r0, r3
	movs r3, #112
	asrs r0, r0, #1
	movs r2, #0
	add r3, sp
	ldr r7, [pc, #200]
	mov r11, r0
	mov r10, r2
	mov r9, r3
.L_080e56e6:
	ldr r3, [r7, #24]
	cmp r3, #0
	blt .L_080e577e
	mov r6, r9
	mov r0, r10
	movs r5, #1
	ands r5, r0
	adds r1, r6, #0
	adds r0, r7, #0
	bl sub_080e3944
	ldr r2, [r6, #0]
	adds r5, #6
	lsls r0, r5, #1
	asrs r2, r2, #1
	ldr r1, [pc, #148]
	str r2, [r6, #0]
	subs r3, r0, #2
	ldrh r1, [r1, r3]
	ldr r3, [sp, #76]
	adds r1, r3, r1
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, [r6, #4]
	ldr r4, [sp, #104]
	subs r3, r3, r5
	str r5, [sp, #0]
	str r0, [sp, #4]
	ldr r0, [sp, #88]
	bl sub_080072f4
	adds r0, r7, #0
	movs r1, #62
	movs r2, #0
	bl sub_080e38b8
	mov r3, r10
	ldr r5, [sp, #84]
	adds r3, #22
	cmp r5, r3
	ble .L_080e577e
	mov r1, r8
	ldr r0, [r1, #8]
	ldr r3, [r7, #0]
	ldr r1, [r1, #12]
	subs r0, r0, r3
	ldr r3, [r7, #4]
	mov r2, r8
	add r1, r11
	subs r1, r1, r3
	ldr r3, [r2, #16]
	ldr r2, [r7, #8]
	subs r3, r3, r2
	asrs r4, r3, #8
	ldr r3, [r7, #12]
	asrs r0, r0, #8
	adds r3, r3, r0
	str r3, [r7, #12]
	ldr r3, [r7, #16]
	asrs r1, r1, #8
	adds r3, r3, r1
	str r3, [r7, #16]
	ldr r3, [r7, #20]
	adds r3, r3, r4
	str r3, [r7, #20]
	ldr r3, [pc, #64]
	ldr r2, [pc, #68]
	adds r0, r0, r3
	cmp r0, r2
	bhi .L_080e577e
	adds r3, r4, r3
	cmp r3, r2
	bhi .L_080e577e
	movs r3, #1
	negs r3, r3
	str r3, [r7, #24]
.L_080e577e:
	movs r5, #1
	add r10, r5
	mov r0, r10
	adds r7, #28
	cmp r0, #32
	bne .L_080e56e6
	bl sub_080e640e
	lsls r0, r0, #0
