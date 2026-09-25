.syntax unified
	.thumb
	.global Unnamed_080f7460
	.global Func_080f7460
	.thumb_func
Unnamed_080f7460:
Func_080f7460:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, [pc, #120]
	movs r0, #41
	sub sp, #116
	bl 0x080048b0
	movs r1, #128
	str r0, [sp, #44]
	lsls r1, r1, #8
	movs r0, #40
	bl 0x080048b0
	ldr r1, [pc, #100]
	str r0, [sp, #40]
	movs r0, #39
	bl 0x080048f4
	ldr r1, [pc, #96]
	str r0, [sp, #36]
	movs r0, #45
	bl 0x080048f4
	str r0, [sp, #32]
	ldr r0, [pc, #88]
	ldr r5, [pc, #88]
	bl 0x08002f3c
	movs r0, #144
	lsls r0, r0, #1
	adds r5, r5, r0
	movs r3, #255
	strb r3, [r5]
	ldr r2, [sp, #32]
	movs r3, #0
	adds r2, #162
	strh r3, [r2]
	ldr r1, [sp, #32]
	movs r3, #1
	adds r1, #152
	str r1, [sp, #28]
	str r3, [r1]
	bl 0x080040e8
	ldr r5, [pc, #24]
	ldr r3, [pc, #52]
	movs r0, #0
	strb r5, [r3]
	ldr r1, [pc, #52]
	ldr r7, [pc, #16]
	ldr r4, [pc, #20]
	mov r8, r0
	movs r6, #0
.L4:
	movs r2, #0
	mov r10, r2
	b .L0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0000a1a6
	.4byte 0x0000a1a8
	.4byte 0x0000060e
	.4byte 0x0000782c
	.4byte 0x0000061c
	.4byte 0x0000000c
	.4byte 0x0200024c
	.4byte 0x03001d18
	.4byte 0x06002800
.L0:
	mov r3, r10
	subs r3, #5
	cmp r3, #19
	bhi .L1
	cmp r6, #2
	ble .L1
	cmp r6, #13
	bgt .L1
	mov r2, r8
	adds r3, r2, r1
	strh r7, [r3]
	b .L2
.L1:
	mov r3, r10
	cmp r3, #29
	ble .L3
	mov r2, r8
	adds r3, r2, r1
	strh r5, [r3]
	b .L2
.L3:
	mov r3, r8
	adds r2, r3, r1
	adds r3, r0, r4
	strh r3, [r2]
	adds r0, #1
.L2:
	movs r2, #1
	add r10, r2
	movs r3, #2
	mov r2, r10
	add r8, r3
	cmp r2, #32
	bne .L0
	adds r6, #1
	cmp r6, #20
	bne .L4
	ldr r0, [pc, #172]
	bl 0x08002f40
	ldr r1, [sp, #44]
	bl 0x08005340
	ldr r0, [pc, #164]
	bl 0x08002f40
	ldr r3, [pc, #164]
	adds r4, r0, #0
	ldr r1, [pc, #164]
	ldr r2, [pc, #164]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r4, #32
	adds r0, r4, #0
	ldr r1, [pc, #160]
	bl 0x08005340
	ldr r0, [pc, #152]
	movs r3, #0
	ldr r7, [pc, #152]
	mov r8, r3
	movs r6, #0
	movs r5, #0
	mov r12, r0
.L8:
	movs r1, #0
	lsls r3, r5, #6
	mov r2, r12
	mov r10, r1
	adds r4, r3, r2
.L7:
	mov r3, r10
	subs r3, #5
	cmp r3, #19
	bhi .L5
	cmp r6, #2
	ble .L5
	cmp r6, #13
	ble .L6
.L5:
	mov r2, r8
	adds r1, r2, r7
	ldr r3, [pc, #96]
	adds r0, r4, #0
	ldr r2, [pc, #100]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #32
	add r8, r3
.L6:
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r4, #32
	cmp r1, #30
	bne .L7
	adds r6, #1
	adds r5, #15
	cmp r6, #20
	bne .L8
	movs r1, #192
	ldr r3, [pc, #80]
	ldr r0, [pc, #84]
	lsls r1, r1, #2
	bl 0x080072f0
	ldr r3, [pc, #40]
	movs r2, #0
	movs r4, #2
	ldr r5, [pc, #72]
	mov r8, r2
	movs r6, #0
	mov r12, r3
	negs r4, r4
	movs r0, #0
.L12:
	movs r7, #0
	adds r2, r0, #0
	mov r10, r7
	adds r1, r4, #0
	adds r2, #148
.L11:
	cmp r1, #14
	bls .L9
	mov r7, r8
	adds r3, r7, r5
	mov r7, r12
	strh r7, [r3]
	b .L10
	.4byte 0x000000bf
	.4byte 0x00000076
	.4byte 0x0000003f
	.4byte 0x040000d4
	.4byte 0x05000140
	.4byte 0x84000008
	.4byte 0x02010000
	.4byte 0x0600b500
	.4byte 0x03000164
	.4byte 0x06002d00
	.4byte 0x06003000
.L9:
	mov r7, r8
	adds r3, r7, r5
	strh r2, [r3]
.L10:
	movs r3, #1
	add r10, r3
	movs r7, #2
	mov r3, r10
	adds r2, #1
	add r8, r7
	cmp r3, #32
	bne .L11
	adds r6, #1
	adds r4, #1
	adds r0, #32
	cmp r6, #20
	bne .L12
	ldr r2, [pc, #72]
	ldr r3, [pc, #48]
	strh r3, [r2]
	ldr r3, [pc, #48]
	adds r2, #2
	strh r3, [r2]
	ldr r3, [pc, #64]
	movs r1, #0
	strh r1, [r3]
	strh r1, [r3, #2]
	strh r1, [r3, #4]
	strh r1, [r3, #6]
	strh r1, [r3, #8]
	strh r1, [r3, #10]
	ldr r3, [pc, #28]
	adds r2, #60
	strh r3, [r2]
	ldr r3, [pc, #28]
	adds r2, #2
	strh r3, [r2]
	ldr r3, [pc, #24]
	adds r2, #6
	strh r3, [r2]
	ldr r3, [pc, #24]
	adds r2, #2
	b .L13
	.4byte 0x00000509
	.4byte 0x00000680
	.4byte 0x00003737
	.4byte 0x00002727
	.4byte 0x00003f44
	.4byte 0x00001010
	.4byte 0x0400000a
	.4byte 0x03001ad0
.L13:
	strh r3, [r2]
	ldr r3, [pc, #76]
	ldr r2, [pc, #52]
	strh r1, [r3]
	adds r3, #4
	strh r1, [r3]
	subs r3, #2
	strh r2, [r3]
	adds r3, #4
	strh r2, [r3]
	ldr r0, [pc, #60]
	ldr r3, [pc, #40]
	ldr r4, [pc, #60]
	strh r3, [r0]
	ldr r2, [pc, #36]
	ldr r3, [pc, #40]
	strh r3, [r4]
	strh r2, [r0]
	ldr r3, [pc, #52]
	ldr r0, [pc, #36]
	strh r0, [r4]
	strh r2, [r3]
	adds r3, #4
	strh r0, [r3]
	ldr r7, [sp, #36]
	ldr r2, [sp, #32]
	movs r0, #128
	lsls r0, r0, #2
	b .L14
	.2byte 0x0000
	.4byte 0x0000ff60
	.4byte 0x000028c8
	.4byte 0x000000f0
	.4byte 0x00001878
	.4byte 0x000000a0
	.4byte 0x04000014
	.4byte 0x04000040
	.4byte 0x04000044
	.4byte 0x04000042
.L14:
	adds r0, r7, r0
	adds r2, #140
	str r0, [sp, #24]
	str r2, [sp, #20]
	str r1, [r2]
	ldr r3, [sp, #32]
	adds r3, #144
	str r1, [r3]
	ldr r3, [sp, #32]
	ldr r0, [pc, #96]
	adds r3, #148
	str r1, [r3]
	adds r3, r7, r0
	str r1, [r3]
	ldr r2, [sp, #32]
	adds r2, #168
	str r2, [sp, #16]
	str r1, [r2]
	ldr r0, [pc, #80]
	bl 0x08002f40
	movs r1, #160
	ldr r3, [pc, #76]
	lsls r1, r1, #19
	ldr r2, [pc, #76]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, [pc, #76]
	ldr r3, [pc, #48]
	strh r3, [r2]
	ldr r3, [pc, #48]
	adds r2, #2
	strh r3, [r2]
	ldr r0, [pc, #68]
	bl 0x08002f40
	ldr r3, [pc, #48]
	adds r4, r0, #0
	ldr r1, [pc, #60]
	ldr r2, [pc, #64]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #240
	lsls r3, r3, #1
	adds r4, r4, r3
	adds r0, r4, #0
	ldr r1, [pc, #52]
	bl 0x08005340
	ldr r3, [pc, #24]
	ldr r0, [pc, #44]
	ldr r1, [pc, #48]
	ldr r2, [pc, #48]
	b .L15
	.4byte 0x00002f8b
	.4byte 0x00005bf6
	.4byte 0x0000778c
	.4byte 0x0000008f
	.4byte 0x040000d4
	.4byte 0x84000020
	.4byte 0x05000080
	.4byte 0x00000040
	.4byte 0x05000200
	.4byte 0x84000078
	.4byte 0x02010000
	.4byte 0x06010000
	.4byte 0x84001b30
.L15:
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, [pc, #156]
	bl 0x08002f40
	ldr r3, [pc, #156]
	adds r4, r0, #0
	ldr r1, [pc, #156]
	ldr r2, [pc, #156]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r4, #32
	adds r0, r4, #0
	ldr r1, [pc, #152]
	bl 0x08005340
	ldr r3, [pc, #132]
	ldr r0, [pc, #144]
	ldr r1, [pc, #144]
	ldr r2, [pc, #148]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl 0x08015000
	bl 0x080f731c
	movs r7, #0
	mov r10, r7
	ldr r7, [sp, #32]
.L17:
	movs r3, #8
	str r3, [r7]
	movs r3, #0
	strb r3, [r7, #25]
	movs r3, #255
	strb r3, [r7, #26]
	movs r6, #0
	adds r5, r7, #4
.L16:
	bl 0x08004458
	movs r1, #5
	bl 0x08002304
	adds r6, #1
	strb r0, [r5]
	adds r5, #1
	cmp r6, #21
	bne .L16
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r7, #28
	cmp r1, #5
	bne .L17
	ldr r3, [sp, #32]
	movs r2, #0
	add r7, sp, #84
	mov r10, r2
	adds r4, r7, #0
	mov r9, r3
.L23:
	movs r0, #0
	mov r8, r0
	movs r5, #0
.L20:
	str r4, [sp, #8]
	bl 0x08004458
	movs r1, #21
	bl 0x08002304
	mov r1, r8
	str r0, [r5, r7]
	movs r6, #0
	ldr r4, [sp, #8]
	cmp r1, #0
	beq .L18
	ldr r3, [r7]
	cmp r0, r3
	bne .L19
	movs r2, #1
	negs r2, r2
	subs r5, #4
	add r8, r2
	b .L18
	.4byte 0x00000041
	.4byte 0x040000d4
	.4byte 0x050003e0
	.4byte 0x84000008
	.4byte 0x02010000
	.4byte 0x06016e00
	.4byte 0x84000480
.L19:
	adds r6, #1
	cmp r6, r8
	beq .L18
	lsls r3, r6, #2
	ldr r2, [r5, r4]
	ldr r3, [r4, r3]
	cmp r2, r3
	bne .L19
	movs r3, #1
	negs r3, r3
	subs r5, #4
	add r8, r3
.L18:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #4
	cmp r1, #8
	bne .L20
	movs r6, #0
	adds r1, r7, #0
.L22:
	adds r2, r6, #0
	cmp r6, #5
	ble .L21
	movs r2, #5
.L21:
	ldmia r1!, {r3}
	mov r0, r9
	adds r3, #4
	adds r6, #1
	strb r2, [r0, r3]
	cmp r6, #8
	bne .L22
	movs r2, #1
	add r10, r2
	movs r1, #28
	mov r3, r10
	add r9, r1
	cmp r3, #5
	bne .L23
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #8
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl 0x080c9000
	ldr r5, [pc, #172]
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	movs r1, #8
	str r3, [sp, #48]
	movs r2, #7
	movs r3, #3
	movs r0, #47
	str r3, [sp, #0]
	bl 0x080c9000
	adds r5, #188
	ldr r3, [r5]
	mov r7, sp
	adds r7, #48
	str r7, [sp, #12]
	movs r1, #128
	str r3, [r7, #4]
	ldr r0, [sp, #40]
	ldr r3, [pc, #136]
	lsls r1, r1, #8
	movs r2, #0
	bl 0x080072f0
	ldr r3, [pc, #132]
	ldr r0, [sp, #40]
	ldr r1, [pc, #132]
	ldr r2, [pc, #132]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #160
	lsls r0, r0, #19
	ldr r1, [sp, #36]
	ldr r2, [pc, #124]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, [sp, #24]
	ldr r0, [pc, #120]
	ldr r2, [pc, #116]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r5, #128
	lsls r5, r5, #1
	ldr r1, [pc, #108]
	ldr r0, [sp, #24]
	movs r2, #0
	adds r3, r5, #0
	bl 0x080f6038
	movs r1, #160
	movs r2, #0
	adds r3, r5, #0
	lsls r1, r1, #19
	ldr r0, [sp, #36]
	bl 0x080f6038
	movs r2, #128
	ldr r3, [pc, #52]
	lsls r2, r2, #19
	strh r3, [r2]
	movs r0, #228
	bl 0x080772e0
	cmp r0, #1
	bne .L24
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #18
	movs r3, #3
	movs r0, #6
	bl 0x08015010
	movs r2, #153
	adds r1, r0, #0
	ldr r0, [sp, #32]
	lsls r2, r2, #3
	adds r3, r0, r2
	str r1, [r3]
	ldr r0, [pc, #40]
	movs r2, #0
	movs r3, #0
	bl 0x08015080
	b .L25
	.4byte 0x00003740
	.4byte 0x03001e50
	.4byte 0x03000168
	.4byte 0x040000d4
	.4byte 0x06003500
	.4byte 0x84002000
	.4byte 0x84000080
	.4byte 0x05000200
	.4byte 0x00000909
.L25:
	b .L26
.L24:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #26
	movs r3, #4
	movs r0, #2
	bl 0x08015010
	movs r7, #153
	ldr r3, [sp, #32]
	ldr r5, [pc, #596]
	lsls r7, r7, #3
	adds r1, r0, #0
	adds r6, r3, r7
	adds r0, r5, #0
	str r1, [r6]
	movs r2, #0
	movs r3, #0
	adds r5, #1
	bl 0x08015080
	ldr r1, [r6]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #8
	bl 0x08015080
.L26:
	ldr r0, [sp, #36]
	ldr r1, [pc, #564]
	movs r5, #144
	adds r2, r0, r1
	movs r3, #0
	lsls r5, r5, #3
	str r3, [r2]
	adds r1, r5, #0
	ldr r0, [pc, #552]
	bl 0x080041d8
	ldr r0, [pc, #552]
	adds r1, r5, #0
	bl 0x080041d8
	ldr r7, [sp, #20]
	ldr r3, [r7]
	movs r2, #0
	mov r11, r2
	cmp r3, #10
	bne .L27
	b .L28
.L27:
	mov r0, r11
	cmp r0, #16
	bgt .L29
	movs r6, #128
	lsls r5, r0, #12
	lsls r6, r6, #1
	ldr r1, [pc, #520]
	adds r2, r5, #0
	adds r3, r6, #0
	ldr r0, [sp, #24]
	bl 0x080f6038
	movs r1, #160
	ldr r0, [sp, #36]
	lsls r1, r1, #19
	adds r2, r5, #0
	adds r3, r6, #0
	bl 0x080f6038
.L29:
	ldr r1, [sp, #20]
	ldr r3, [r1]
	cmp r3, #3
	beq .L30
	b .L31
.L30:
	mov r0, r11
	movs r1, #80
	bl 0x080022fc
	cmp r0, #15
	bgt .L32
	ldr r0, [pc, #476]
	bl 0x080f61e8
	b .L33
.L32:
	cmp r0, #31
	bgt .L34
	ldr r0, [pc, #468]
	bl 0x080f61e8
	b .L33
.L34:
	cmp r0, #47
	bgt .L35
	ldr r0, [pc, #460]
	bl 0x080f61e8
	b .L33
.L35:
	cmp r0, #63
	bgt .L36
	ldr r0, [pc, #452]
	bl 0x080f61e8
	b .L33
.L36:
	ldr r0, [pc, #448]
	bl 0x080f61e8
.L33:
	ldr r2, [sp, #16]
	ldr r3, [r2]
	cmp r3, #15
	bgt .L37
	bl 0x080f6148
	ldr r7, [sp, #16]
	ldr r3, [r7]
.L37:
	cmp r3, #16
	ble .L38
	movs r3, #7
	mov r0, r11
	ands r3, r0
	cmp r3, #0
	bne .L38
	bl 0x08004458
	movs r3, #127
	ands r3, r0
	adds r5, r3, #0
	bl 0x08004458
	movs r3, #31
	ands r3, r0
	adds r1, r3, #0
	mov r2, r11
	adds r5, #56
	adds r1, #48
	cmp r2, #0
	bge .L39
	adds r2, #7
.L39:
	movs r3, #3
	asrs r2, r2, #3
	ands r2, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r2, [pc, #376]
	lsls r3, r3, #10
	adds r7, r3, r2
	lsls r5, r5, #16
	movs r3, #0
	lsls r1, r1, #16
	mov r8, r3
	mov r9, r5
	mov r10, r1
.L40:
	bl 0x08004458
	movs r5, #255
	ands r5, r0
	bl 0x08004458
	ldr r3, [pc, #348]
	adds r6, r0, #0
	ands r6, r3
	mov r1, r10
	mov r0, r9
	str r1, [r7, #4]
	str r0, [r7]
	adds r0, r6, #0
	bl 0x08002322
	adds r5, #64
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl 0x0800231c
	adds r3, r5, #0
	muls r3, r0
	negs r3, r3
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl 0x08004458
	movs r3, #15
	ands r3, r0
	adds r3, #16
	str r3, [r7, #24]
	movs r2, #1
	movs r3, #128
	add r8, r2
	lsls r3, r3, #1
	adds r7, #28
	cmp r8, r3
	bne .L40
.L38:
	movs r7, #0
	mov r8, r7
	ldr r7, [pc, #268]
.L45:
	ldr r0, [r7, #24]
	cmp r0, #0
	ble .L41
	ldr r3, [r7]
	ldr r1, [pc, #268]
	subs r0, #1
	str r0, [r7, #24]
	cmp r3, r1
	bhi .L42
	ldr r6, [r7, #4]
	ldr r2, [pc, #260]
	cmp r6, r2
	bgt .L42
	cmp r6, #0
	blt .L42
	movs r1, #12
	asrs r5, r3, #16
	bl 0x080022ec
	adds r0, #1
	lsls r4, r0, #1
	mov r3, r8
	ldr r1, [pc, #240]
	movs r2, #1
	ands r2, r3
	asrs r6, r6, #16
	subs r3, r4, #2
	ldrh r1, [r1, r3]
	subs r5, r5, r0
	ldr r3, [sp, #44]
	subs r6, r6, r0
	str r4, [sp, #0]
	ldr r0, [sp, #12]
	str r4, [sp, #4]
	lsls r2, r2, #2
	ldr r4, [r2, r0]
	adds r1, r3, r1
	ldr r0, [sp, #40]
	adds r3, r6, #0
	adds r2, r5, #0
	bl 0x080072f4
	ldr r3, [r7]
.L42:
	ldr r2, [r7, #12]
	adds r3, r3, r2
	str r3, [r7]
	ldr r1, [r7, #16]
	ldr r3, [r7, #4]
	adds r3, r3, r1
	str r3, [r7, #4]
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #2
	cmp r3, #0
	bge .L43
	adds r3, #63
.L43:
	asrs r3, r3, #6
	str r3, [r7, #12]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r3, r3, #2
	cmp r3, #0
	bge .L44
	adds r3, #63
.L44:
	asrs r3, r3, #6
	str r3, [r7, #16]
.L41:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #3
	adds r7, #28
	cmp r8, r2
	bne .L45
.L31:
	ldr r7, [sp, #20]
	ldr r3, [r7]
	cmp r3, #0
	beq .L46
	cmp r3, #2
	beq .L46
	b .L47
.L46:
	movs r0, #0
	add r5, sp, #56
	mov r8, r0
	movs r2, #0
	adds r3, r5, #0
.L48:
	movs r1, #1
	add r8, r1
	mov r7, r8
	stmia r3!, {r2}
	cmp r7, #7
	bne .L48
	ldr r0, [sp, #20]
	ldr r3, [r0]
	cmp r3, #0
	bne .L49
	movs r2, #1
	str r2, [r5, #12]
	ldr r1, [sp, #28]
	ldr r3, [r1]
	cmp r3, #1
	ble .L50
	str r2, [r5, #16]
	str r2, [r5, #8]
	ldr r3, [r1]
.L50:
	cmp r3, #2
	ble .L51
	str r2, [r5, #20]
	str r2, [r5, #4]
	ldr r7, [sp, #28]
	ldr r3, [r7]
.L51:
	cmp r3, #3
	ble .L52
	str r2, [r5, #24]
	str r2, [r5]
	b .L52
	.4byte 0x00000908
	.4byte 0x00007824
	.4byte 0x080f6441
	.4byte 0x080f60a1
	.4byte 0x05000200
	.4byte 0x00000091
	.4byte 0x00000093
	.4byte 0x000000b4
	.4byte 0x000000a0
	.4byte 0x0000008f
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x00ffffff
	.4byte 0x007fffff
	.4byte 0x080f86f8
.L49:
	mov r3, r11
	mov r0, r8
	ands r3, r0
	cmp r3, #3
	bgt .L52
	ldr r2, [sp, #32]
	movs r1, #0
	mov r8, r1
	adds r0, r5, #0
	adds r2, #172
.L53:
	ldmia r2!, {r3}
	str r3, [r1, r0]
	movs r3, #1
	add r8, r3
	mov r7, r8
	adds r1, #4
	cmp r7, #7
	bne .L53
.L52:
	movs r0, #0
	mov r8, r0
.L61:
	movs r2, #1
	mov r1, r8
	eors r2, r1
	negs r3, r2
	orrs r3, r2
	lsrs r6, r3, #31
	movs r3, #65
	subs r6, r3, r6
	ldr r3, [r5, #4]
	cmp r3, #0
	beq .L54
	mov r3, r8
	adds r3, #19
	movs r0, #20
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl 0x080f62b8
.L54:
	ldr r3, [r5, #8]
	cmp r3, #0
	beq .L55
	mov r3, r8
	adds r3, #35
	movs r0, #28
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl 0x080f62b8
.L55:
	ldr r3, [r5, #12]
	cmp r3, #0
	beq .L56
	mov r3, r8
	adds r3, #51
	movs r0, #20
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl 0x080f62b8
.L56:
	ldr r3, [r5, #16]
	cmp r3, #0
	beq .L57
	mov r3, r8
	adds r3, #67
	movs r0, #28
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl 0x080f62b8
.L57:
	ldr r3, [r5, #20]
	cmp r3, #0
	beq .L58
	mov r3, r8
	adds r3, #83
	movs r0, #20
	adds r1, r3, #0
	movs r2, #200
	str r6, [sp, #0]
	bl 0x080f62b8
.L58:
	ldr r3, [r5]
	cmp r3, #0
	beq .L59
	mov r1, r8
	mov r3, r8
	adds r1, #5
	adds r3, #91
	movs r0, #28
	movs r2, #200
	str r6, [sp, #0]
	bl 0x080f62b8
.L59:
	ldr r3, [r5, #24]
	cmp r3, #0
	beq .L60
	mov r1, r8
	mov r3, r8
	adds r1, #97
	adds r3, #11
	movs r0, #28
	movs r2, #200
	str r6, [sp, #0]
	bl 0x080f62b8
.L60:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #3
	bne .L61
.L47:
	ldr r7, [sp, #36]
	ldr r0, [pc, #152]
	movs r2, #1
	adds r3, r7, r0
	str r2, [r3]
	movs r0, #1
	bl 0x080030f8
	ldr r2, [sp, #20]
	ldr r3, [r2]
	movs r1, #1
	add r11, r1
	cmp r3, #10
	beq .L28
	b .L27
.L28:
	movs r3, #0
	movs r6, #128
	mov r8, r3
	lsls r6, r6, #1
.L62:
	mov r7, r8
	movs r5, #128
	lsls r3, r7, #12
	lsls r5, r5, #9
	subs r5, r5, r3
	ldr r0, [sp, #24]
	ldr r1, [pc, #108]
	adds r2, r5, #0
	adds r3, r6, #0
	bl 0x080f6038
	movs r1, #160
	lsls r1, r1, #19
	ldr r0, [sp, #36]
	adds r2, r5, #0
	adds r3, r6, #0
	bl 0x080f6038
	movs r0, #1
	bl 0x080030f8
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #17
	bne .L62
	movs r0, #47
	bl 0x08002dd8
	movs r0, #46
	bl 0x08002dd8
	ldr r0, [pc, #60]
	bl 0x08004278
	ldr r0, [pc, #56]
	bl 0x08004278
	movs r0, #45
	bl 0x08002dd8
	movs r0, #40
	bl 0x08002dd8
	movs r0, #39
	bl 0x08002dd8
	movs r0, #41
	bl 0x08002dd8
	sub sp, #-116
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00007824
	.4byte 0x05000200
	.4byte 0x080f60a1
	.4byte 0x080f6441
