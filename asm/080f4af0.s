@ Reconstructed thumb disassembly of a call-graph-reached code region
@ (code interleaved with its data), verified byte-identical by build_asm.
.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08005268, 0x08005268
	.set sub_080072f0, 0x080072f0
	.set sub_080f4f04, 0x080f4f04
	.set sub_080f4fea, 0x080f4fea
	.set sub_080f5260, 0x080f5260
	.global Region_080f4af0
Region_080f4af0:
	mov r5, r10
	lsls r0, r5, #2
	adds r3, r0, #4
	mov r6, lr
	str r4, [r3, r6]
	ldr r1, [sp, #68]
	movs r7, #0
	lsls r3, r1, #3
	movs r0, #240
	movs r2, #204
	adds r2, r2, r3
	mov r11, r7
	add r0, sp
	adds r3, #200
	ldr r7, [sp, #44]
	add r6, sp, #200
	mov r9, r0
	mov r10, r2
	mov r8, r3
.L_080f4b16:
	ldr r2, [r7, #0]
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r4, [sp, #112]
	lsls r3, r3, #2
	movs r5, #225
	adds r3, r4, r3
	lsls r5, r5, #7
	adds r4, r3, r5
	adds r0, r4, #0
	adds r1, r6, #0
	str r4, [sp, #8]
	bl sub_08005268
	ldr r2, [r6, #8]
	ldr r0, [pc, #916]
	ldr r4, [sp, #8]
	cmp r2, r0
	bgt .L_080f4b44
	movs r3, #175
	lsls r3, r3, #1
	str r3, [r6, #8]
	adds r2, r3, #0
.L_080f4b44:
	ldr r3, [pc, #904]
	cmp r2, r3
	ble .L_080f4b4e
	str r3, [r6, #8]
	adds r2, r3, #0
.L_080f4b4e:
	ldr r3, [r7, #0]
	ldr r1, [pc, #896]
	lsls r3, r3, #2
	adds r2, r2, r1
	mov r5, r9
	str r2, [r5, r3]
	ldr r5, [r7, #0]
	mov r0, r9
	lsls r3, r5, #2
	ldr r1, [r0, r3]
	lsrs r3, r1, #31
	adds r1, r1, r3
	asrs r1, r1, #1
	movs r0, #128
	adds r1, #128
	lsls r0, r0, #4
	str r4, [sp, #8]
	bl sub_080022ec
	ldr r3, [r6, #0]
	subs r2, r3, r0
	ldr r3, [r6, #4]
	subs r0, r3, r0
	adds r3, r0, #0
	movs r1, #16
	adds r3, #96
	negs r1, r1
	ldr r4, [sp, #8]
	cmp r3, r1
	ble .L_080f4bd8
	lsls r1, r5, #25
	movs r5, #158
	lsls r5, r5, #2
	adds r3, r2, r5
	ldr r2, [pc, #836]
	ands r3, r2
	lsls r3, r3, #16
	subs r2, #159
	orrs r1, r3
	adds r3, r0, r2
	movs r2, #255
	ands r3, r2
	orrs r1, r3
	ldr r3, [pc, #820]
	orrs r1, r3
	ldr r3, [sp, #108]
	mov r5, r8
	str r1, [r3, r5]
	ldr r0, [r4, #24]
	ldr r2, [pc, #812]
	cmp r0, #0
	bge .L_080f4bba
	ldr r1, [pc, #812]
	adds r0, r0, r1
.L_080f4bba:
	asrs r3, r0, #10
	ldrb r3, [r2, r3]
	movs r2, #128
	lsls r2, r2, #3
	lsls r3, r3, #3
	orrs r3, r2
	ldr r2, [sp, #108]
	mov r4, r10
	str r3, [r2, r4]
	ldr r0, [sp, #68]
	adds r0, #1
	movs r5, #8
	str r0, [sp, #68]
	add r10, r5
	add r8, r5
.L_080f4bd8:
	movs r1, #1
	add r11, r1
	mov r2, r11
	adds r7, #4
	cmp r2, #2
	bne .L_080f4b16
	ldr r3, [sp, #96]
	cmp r3, #0
	bne .L_080f4ca6
	ldr r2, [sp, #68]
	movs r4, #0
	ldr r7, [sp, #44]
	mov r6, sp
	ldr r0, [sp, #112]
	movs r1, #225
	lsls r3, r2, #3
	mov r11, r4
	movs r5, #200
	adds r6, #240
	lsls r1, r1, #7
	movs r4, #204
	add r5, sp
	str r7, [sp, #24]
	adds r0, r0, r1
	adds r4, r4, r3
	adds r7, r3, #0
	str r6, [sp, #60]
	mov r9, r5
	mov r10, r0
	mov r8, r4
	adds r7, #200
.L_080f4c16:
	mov r1, r9
	mov r0, r10
	bl sub_08005268
	ldr r6, [sp, #24]
	ldmia r6!, {r3}
	adds r5, r6, #0
	ldr r0, [sp, #60]
	str r5, [sp, #24]
	lsls r3, r3, #2
	ldr r6, [r0, r3]
	lsrs r3, r6, #31
	adds r6, r6, r3
	asrs r6, r6, #1
	adds r6, #128
	movs r0, #128
	adds r1, r6, #0
	lsls r0, r0, #4
	bl sub_080022ec
	mov r1, r9
	ldr r5, [r1, #0]
	subs r5, r5, r0
	movs r0, #192
	adds r1, r6, #0
	lsls r0, r0, #2
	bl sub_080022ec
	mov r3, r9
	ldr r2, [r3, #4]
	movs r4, #158
	ldr r3, [pc, #640]
	lsls r4, r4, #2
	movs r6, #176
	adds r2, r2, r0
	adds r5, r5, r4
	lsls r6, r6, #1
	ands r5, r3
	adds r2, r2, r6
	movs r3, #255
	ands r2, r3
	lsls r5, r5, #16
	ldr r3, [pc, #636]
	orrs r5, r2
	ldr r0, [sp, #108]
	orrs r5, r3
	mov r1, r11
	str r5, [r0, r7]
	cmp r1, #1
	bne .L_080f4c84
	ldr r3, [r0, r7]
	movs r2, #128
	lsls r2, r2, #21
	orrs r3, r2
	str r3, [r0, r7]
.L_080f4c84:
	ldr r2, [sp, #108]
	ldr r3, [pc, #612]
	mov r4, r8
	str r3, [r2, r4]
	ldr r6, [sp, #68]
	movs r1, #1
	add r11, r1
	movs r5, #8
	adds r6, #1
	movs r0, #28
	mov r2, r11
	add r8, r5
	adds r7, #8
	str r6, [sp, #68]
	add r10, r0
	cmp r2, #2
	bne .L_080f4c16
.L_080f4ca6:
	ldr r0, [sp, #112]
	movs r6, #128
	movs r1, #225
	ldr r2, [sp, #68]
	movs r3, #0
	lsls r6, r6, #19
	lsls r1, r1, #7
	mov r11, r3
	mov r9, r6
	lsls r3, r2, #3
	adds r6, r0, r1
	movs r0, #200
	adds r0, r0, r3
	adds r3, #204
	add r4, sp, #212
	add r5, sp, #200
	add r7, sp, #248
	mov r10, r0
	mov r8, r3
.L_080f4ccc:
	ldr r3, [r6, #0]
	movs r1, #0
	str r1, [r4, #4]
	str r3, [r4, #0]
	ldr r3, [r6, #8]
	adds r0, r4, #0
	str r3, [r4, #8]
	adds r1, r5, #0
	str r4, [sp, #8]
	bl sub_08005268
	ldr r3, [r5, #8]
	ldr r2, [pc, #484]
	ldr r4, [sp, #8]
	cmp r3, r2
	bgt .L_080f4cf2
	movs r3, #175
	lsls r3, r3, #1
	str r3, [r5, #8]
.L_080f4cf2:
	ldr r2, [pc, #476]
	cmp r3, r2
	ble .L_080f4cfc
	str r2, [r5, #8]
	adds r3, r2, #0
.L_080f4cfc:
	ldr r0, [pc, #468]
	adds r3, r3, r0
	str r3, [r7, #0]
	movs r1, #6
	ldrsh r2, [r6, r1]
	adds r3, r3, r2
	str r3, [r7, #0]
	ldr r1, [r5, #4]
	movs r0, #16
	adds r3, r1, #0
	adds r3, #88
	negs r0, r0
	ldr r2, [r5, #0]
	cmp r3, r0
	ble .L_080f4d54
	movs r0, #156
	lsls r0, r0, #2
	adds r3, r2, r0
	ldr r2, [pc, #436]
	movs r0, #172
	ands r3, r2
	lsls r0, r0, #1
	mov r2, r9
	lsls r3, r3, #16
	orrs r3, r2
	adds r2, r1, r0
	movs r1, #255
	ands r2, r1
	orrs r3, r2
	ldr r2, [pc, #420]
	ldr r1, [sp, #108]
	orrs r3, r2
	mov r2, r10
	str r3, [r1, r2]
	movs r3, #137
	mov r0, r8
	lsls r3, r3, #4
	str r3, [r1, r0]
	ldr r2, [sp, #68]
	adds r2, #1
	movs r1, #8
	str r2, [sp, #68]
	add r10, r1
	add r8, r1
.L_080f4d54:
	movs r0, #1
	movs r3, #128
	add r11, r0
	lsls r3, r3, #18
	mov r1, r11
	add r9, r3
	adds r7, #4
	adds r6, #28
	cmp r1, #2
	bne .L_080f4ccc
	ldr r2, [sp, #96]
	cmp r2, #1
	beq .L_080f4d70
	b sub_080f5260
.L_080f4d70:
	ldr r4, [sp, #36]
	movs r3, #0
	ldr r5, [sp, #112]
	movs r6, #225
	lsls r6, r6, #7
	str r3, [sp, #56]
	str r4, [sp, #32]
	str r3, [sp, #28]
	mov r11, r3
	adds r7, r5, r6
	ldr r1, [r7, #0]
	ldr r3, [r7, #12]
	ldr r2, [r7, #16]
	adds r1, r1, r3
	ldr r3, [r7, #4]
	adds r3, r3, r2
	str r3, [r7, #4]
	ldr r2, [r7, #20]
	ldr r3, [r7, #8]
	adds r3, r3, r2
	str r3, [r7, #8]
	str r1, [r7, #0]
	ldr r2, [sp, #112]
	movs r3, #225
	lsls r3, r3, #7
	movs r0, #0
	adds r2, r2, r3
	mov r10, r0
	mov r9, r2
.L_080f4daa:
	cmp r11, r10
	beq .L_080f4e1c
	mov r4, r9
	ldr r3, [r4, #0]
	subs r3, r1, r3
	asrs r6, r3, #16
	ldr r2, [r7, #4]
	ldr r3, [r4, #4]
	mov r5, r9
	subs r2, r2, r3
	asrs r4, r2, #16
	ldr r3, [r5, #8]
	ldr r2, [r7, #8]
	subs r2, r2, r3
	asrs r2, r2, #16
	mov r8, r2
	adds r3, r4, #0
	muls r3, r4
	adds r2, r6, #0
	muls r2, r6
	mov r0, r8
	adds r2, r2, r3
	mov r3, r8
	muls r3, r0
	adds r0, r2, r3
	ldr r2, [pc, #272]
	cmp r0, r2
	bgt .L_080f4e1c
	str r4, [sp, #8]
	ldr r3, [pc, #268]
	bl sub_080072f0
	adds r5, r0, #0
	adds r1, r5, #0
	lsls r0, r6, #15
	bl sub_080022ec
	ldr r3, [r7, #12]
	ldr r4, [sp, #8]
	adds r3, r3, r0
	str r3, [r7, #12]
	lsls r0, r4, #15
	adds r1, r5, #0
	bl sub_080022ec
	ldr r3, [r7, #16]
	adds r3, r3, r0
	str r3, [r7, #16]
	mov r3, r8
	lsls r0, r3, #15
	adds r1, r5, #0
	bl sub_080022ec
	ldr r3, [r7, #20]
	adds r3, r3, r0
	str r3, [r7, #20]
	ldr r1, [r7, #0]
.L_080f4e1c:
	movs r5, #1
	add r10, r5
	movs r4, #28
	mov r6, r10
	add r9, r4
	cmp r6, #2
	bne .L_080f4daa
	ldr r0, [pc, #204]
	cmp r1, r0
	bge .L_080f4e48
	ldr r2, [r7, #12]
	negs r2, r2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #4
	str r0, [r7, #0]
	cmp r3, #0
	bge .L_080f4e42
	adds r3, #63
.L_080f4e42:
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r1, r0, #0
.L_080f4e48:
	movs r3, #200
	lsls r3, r3, #16
	cmp r1, r3
	ble .L_080f4e66
	ldr r2, [r7, #12]
	negs r2, r2
	str r3, [r7, #0]
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #4
	cmp r3, #0
	bge .L_080f4e62
	adds r3, #63
.L_080f4e62:
	asrs r3, r3, #6
	str r3, [r7, #12]
.L_080f4e66:
	ldr r2, [r7, #8]
	ldr r1, [pc, #144]
	cmp r2, r1
	bge .L_080f4e86
	ldr r2, [r7, #20]
	negs r2, r2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #4
	str r1, [r7, #8]
	cmp r3, #0
	bge .L_080f4e80
	adds r3, #63
.L_080f4e80:
	asrs r3, r3, #6
	str r3, [r7, #20]
	adds r2, r1, #0
.L_080f4e86:
	ldr r3, [r7, #4]
	movs r0, #180
	lsls r0, r0, #16
	adds r3, r3, r0
	cmp r2, r3
	ble .L_080f4eb8
	ldr r1, [r7, #20]
	str r3, [r7, #8]
	cmp r1, #0
	ble .L_080f4eb8
	lsrs r3, r1, #31
	ldr r2, [r7, #16]
	adds r3, r1, r3
	asrs r3, r3, #1
	adds r2, r2, r3
	str r2, [r7, #16]
	negs r2, r1
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r2, r3, #4
	cmp r2, #0
	bge .L_080f4eb4
	adds r2, #63
.L_080f4eb4:
	asrs r3, r2, #6
	str r3, [r7, #20]
.L_080f4eb8:
	ldr r3, [r7, #16]
	ldr r1, [pc, #68]
	adds r2, r3, r1
	ldr r3, [r7, #4]
	str r2, [r7, #16]
	cmp r3, #0
	ble .L_080f4ec8
	b sub_080f4fea
.L_080f4ec8:
	b sub_080f4f04
