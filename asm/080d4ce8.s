@ Reconstructed thumb disassembly of a call-graph-reached code region
@ (code interleaved with its data), verified byte-identical by build_asm.
.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002f40, 0x08002f40
	.set sub_080041d8, 0x080041d8
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd594, 0x080cd594
	.set sub_080d5094, 0x080d5094
	.set sub_080d50a6, 0x080d50a6
	.set sub_080d50b4, 0x080d50b4
	.set sub_080d50b6, 0x080d50b6
	.set sub_080d512e, 0x080d512e
	.set sub_080e0524, 0x080e0524
	.set sub_080e3944, 0x080e3944
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Region_080d4ce8
Region_080d4ce8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, [pc, #92]
	adds r3, r2, #0
	ldmia r3!, {r1}
	ldr r3, [r3, #0]
	sub sp, #64
	str r3, [sp, #40]
	ldr r5, [pc, #84]
	mov r11, r1
	ldr r2, [r2, #8]
	add r5, r11
	str r2, [sp, #28]
	str r0, [r5, #0]
	movs r0, #1
	bl sub_080cd594
	ldr r2, [pc, #68]
	ldr r3, [pc, #56]
	ldr r0, [pc, #68]
	strh r3, [r2, #0]
	mov r1, r11
	movs r2, #1
	movs r3, #1
	bl sub_080e0524
	movs r3, #0
	ldr r0, [pc, #56]
	ldr r1, [sp, #28]
	movs r2, #0
	bl sub_080e0524
	ldr r3, [r5, #0]
	ldr r3, [r3, #24]
	cmp r3, #0
	bne .L_080d4d72
	ldr r0, [pc, #44]
	bl sub_08002f40
	adds r1, r0, #0
	movs r0, #160
	ldr r3, [pc, #36]
	lsls r0, r0, #19
	movs r2, #128
	bl sub_080072f0
	b .L_080d4d70
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x00000085
	.4byte 0x00000073
	.4byte 0x00000086
	.4byte 0x03001388
.L_080d4d70:
	b .L_080d4d8a
.L_080d4d72:
	cmp r3, #2
	bne .L_080d4d8a
	ldr r0, [pc, #732]
	bl sub_08002f40
	adds r1, r0, #0
	movs r0, #160
	ldr r3, [pc, #724]
	lsls r0, r0, #19
	movs r2, #128
	bl sub_080072f0
.L_080d4d8a:
	movs r6, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r6, [sp, #0]
	bl sub_080ed408
	ldr r5, [pc, #704]
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3, #0]
	movs r1, #7
	str r3, [sp, #44]
	movs r2, #7
	movs r3, #7
	movs r0, #47
	str r6, [sp, #0]
	bl sub_080ed408
	adds r5, #188
	ldr r3, [r5, #0]
	mov r2, sp
	adds r2, #44
	str r2, [sp, #24]
	str r3, [r2, #4]
	movs r3, #0
	mov r8, r3
	movs r2, #128
	ldr r3, [pc, #664]
	movs r1, #0
	lsls r2, r2, #3
.L_080d4dca:
	movs r5, #1
	add r8, r5
	str r1, [r3, #0]
	adds r3, #28
	cmp r8, r2
	bne .L_080d4dca
	ldr r3, [pc, #652]
	add r3, r11
	ldr r3, [r3, #0]
	movs r7, #36
	ldrsh r0, [r3, r7]
	bl sub_080b5098
	movs r5, #225
	ldr r6, [r0, #0]
	lsls r5, r5, #7
	movs r0, #0
	ldr r7, [pc, #632]
	mov r8, r0
	add r5, r11
.L_080d4df2:
	bl sub_08004458
	movs r3, #15
	ands r3, r0
	adds r3, #72
	lsls r2, r3, #16
	movs r3, #0
	str r3, [r5, #4]
	ldr r3, [pc, #608]
	add r3, r11
	str r2, [r5, #0]
	ldr r3, [r3, #0]
	ldr r3, [r3, #24]
	lsls r3, r3, #2
	add r3, r8
	ldrsb r3, [r7, r3]
	lsls r3, r3, #16
	str r3, [r5, #8]
	ldr r3, [r6, #8]
	cmp r3, #0
	bge .L_080d4e20
	negs r3, r2
	str r3, [r5, #0]
.L_080d4e20:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #4
	bne .L_080d4df2
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2, #0]
	ldr r2, [pc, #564]
	movs r3, #50
	add r2, r11
	movs r1, #144
	str r3, [r2, #0]
	ldr r0, [pc, #556]
	lsls r1, r1, #3
	bl sub_080041d8
	movs r3, #0
	str r3, [sp, #36]
	ldr r3, [pc, #548]
	ldr r5, [r3, #0]
	ldr r3, [pc, #528]
	add r3, r11
	ldr r2, [r3, #0]
	ldr r3, [r2, #24]
	cmp r3, #2
	bne .L_080d4e74
	ldr r7, [sp, #36]
	cmp r7, #63
	bgt .L_080d4e74
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_080d4e6e
	ldrh r3, [r5, #54]
	adds r3, #192
	b .L_080d4e72
.L_080d4e6e:
	ldrh r3, [r5, #54]
	subs r3, #192
.L_080d4e72:
	strh r3, [r5, #54]
.L_080d4e74:
	ldr r0, [sp, #36]
	cmp r0, #16
	bne .L_080d4e80
	movs r0, #134
	bl sub_080b50e8
.L_080d4e80:
	bl sub_080049ac
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl sub_080051d8
	ldr r1, [sp, #36]
	cmp r1, #63
	ble .L_080d4e96
	b sub_080d512e
.L_080d4e96:
	movs r2, #0
	ldr r3, [pc, #456]
	str r2, [sp, #32]
	add r3, r11
	ldr r3, [r3, #0]
	ldr r2, [pc, #468]
	ldr r3, [r3, #24]
	ldrb r3, [r2, r3]
	cmp r3, #0
	bne .L_080d4eac
	b sub_080d512e
.L_080d4eac:
	adds r5, r1, #0
	movs r2, #221
	movs r1, #225
	movs r7, #1
	ldr r0, [pc, #452]
	lsls r1, r1, #7
	lsls r2, r2, #4
	movs r3, #52
	ands r5, r7
	add r1, r11
	add r2, r11
	add r3, sp
	str r5, [sp, #20]
	str r0, [sp, #12]
	str r1, [sp, #8]
	str r2, [sp, #16]
	mov r9, r3
	ldr r0, [sp, #8]
	mov r1, r9
	bl sub_080e3944
	mov r5, r9
	ldr r3, [r5, #0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #0]
	ldr r3, [r5, #4]
	subs r3, #8
	str r3, [r5, #4]
	ldr r7, [sp, #12]
	ldr r0, [sp, #36]
	ldrb r3, [r7, #0]
	cmp r0, r3
	bne .L_080d4efa
	movs r0, #145
	bl sub_080f9010
	ldrb r3, [r7, #0]
.L_080d4efa:
	ldr r1, [sp, #36]
	adds r3, #4
	cmp r1, r3
	blt .L_080d4fc6
	ldr r2, [sp, #32]
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #3
	adds r3, r3, r2
	lsls r0, r1, #4
	adds r0, r0, r3
	movs r1, #104
	bl sub_080022fc
	ldr r5, [sp, #32]
	movs r3, #1
	ands r5, r3
	ldr r3, [sp, #56]
	ldr r7, [sp, #24]
	mov r8, r0
	subs r3, r3, r0
	movs r6, #34
	movs r0, #104
	str r0, [sp, #4]
	ldr r2, [sp, #52]
	str r6, [sp, #0]
	lsls r5, r5, #2
	adds r5, r5, r7
	subs r2, #17
	subs r3, #104
	ldr r4, [r5, #0]
	ldr r0, [sp, #40]
	mov r1, r11
	bl sub_080072f4
	mov r1, r8
	str r1, [sp, #4]
	ldr r2, [sp, #52]
	ldr r3, [sp, #56]
	str r6, [sp, #0]
	subs r2, #17
	subs r3, r3, r1
	ldr r4, [r5, #0]
	ldr r0, [sp, #40]
	mov r1, r11
	bl sub_080072f4
	ldr r2, [sp, #20]
	cmp r2, #0
	beq .L_080d4f90
	ldr r2, [sp, #52]
	ldr r3, [sp, #56]
	movs r5, #20
	movs r7, #37
	subs r2, #20
	subs r3, #24
	ldr r4, [sp, #44]
	ldr r1, [sp, #16]
	str r5, [sp, #0]
	str r7, [sp, #4]
	ldr r0, [sp, #40]
	bl sub_080072f4
	ldr r0, [sp, #24]
	ldr r3, [sp, #56]
	str r5, [sp, #0]
	str r7, [sp, #4]
	ldr r2, [sp, #52]
	ldr r4, [r0, #4]
	subs r3, #24
	ldr r0, [sp, #40]
	ldr r1, [sp, #16]
	bl sub_080072f4
	b .L_080d4fc6
.L_080d4f90:
	ldr r5, [pc, #236]
	ldr r2, [sp, #52]
	ldr r3, [sp, #56]
	add r5, r11
	movs r1, #20
	movs r7, #37
	subs r2, #20
	subs r3, #24
	str r1, [sp, #0]
	ldr r4, [sp, #44]
	adds r1, r5, #0
	str r7, [sp, #4]
	ldr r0, [sp, #40]
	bl sub_080072f4
	movs r0, #20
	str r0, [sp, #0]
	ldr r1, [sp, #24]
	ldr r3, [sp, #56]
	str r7, [sp, #4]
	ldr r2, [sp, #52]
	ldr r4, [r1, #4]
	subs r3, #24
	ldr r0, [sp, #40]
	adds r1, r5, #0
	bl sub_080072f4
.L_080d4fc6:
	ldr r5, [sp, #12]
	ldr r7, [sp, #36]
	ldrb r3, [r5, #0]
	ldr r2, [pc, #172]
	cmp r7, r3
	beq .L_080d4fd8
	adds r3, #16
	cmp r7, r3
	blt sub_080d50b6
.L_080d4fd8:
	movs r0, #0
	ldr r7, [pc, #168]
	mov r10, r0
	mov r8, r0
	ldr r3, [r7, #24]
	cmp r3, #0
	bne sub_080d50a6
	bl sub_08004458
	ldr r6, [pc, #156]
	ands r6, r0
	bl sub_08004458
	mov r2, r9
	ldr r3, [r2, #0]
	lsls r3, r3, #8
	str r3, [r7, #0]
	ldr r5, [pc, #144]
	ldr r3, [r2, #4]
	ldr r1, [pc, #144]
	ands r5, r0
	movs r0, #128
	lsls r0, r0, #5
	lsls r3, r3, #8
	adds r5, r5, r1
	adds r3, r3, r0
	str r3, [r7, #4]
	adds r0, r5, #0
	bl sub_08002322
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #15
	str r3, [r7, #8]
	adds r0, r5, #0
	bl sub_0800231c
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #15
	str r3, [r7, #16]
	ldr r2, [sp, #12]
	ldr r5, [sp, #36]
	ldrb r3, [r2, #0]
	movs r1, #1
	add r10, r1
	cmp r5, r3
	bne sub_080d5094
	bl sub_08004458
	movs r1, #7
	ands r0, r1
	adds r0, #48
	mov r2, r10
	str r0, [r7, #24]
	cmp r2, #200
	bne sub_080d50a6
	b sub_080d50b4
