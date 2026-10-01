.syntax unified
	.thumb
	.global Func_08021a84
	.thumb_func
Func_08021a84:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	adds r7, r0, #0
	movs r0, #0
	str r0, [sp, #24]
	movs r5, #192
	lsls r5, r5, #18
	mov r10, r1
	ldr r1, [r5, #24]
	movs r2, #1
	str r1, [sp, #16]
	str r2, [sp, #8]
	ldr r3, [r5, #80]
	str r3, [sp, #12]
	cmp r3, #0
	bne .L_08021ada
	ldr r1, .L_08021e00
	movs r0, #80
	bl Runtime_AllocateHeapBlock
	ldr r2, .L_08021e04
	adds r1, r0, #0
	ldr r0, .L_08021e08
	movs r4, #132
	subs r2, r2, r0
	movs r3, #128
	lsls r4, r4, #24
	lsrs r2, r2, #2
	lsls r3, r3, #19
	adds r3, #212
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, [r5, #80]
	movs r4, #0
	str r5, [sp, #12]
	str r4, [sp, #8]
.L_08021ada:
	ldrb r2, [r7, #27]
	ldr r1, [sp, #24]
	movs r0, #0
	mov r8, r0
	cmp r1, r2
	blt .L_08021ae8
	b .L_08021cae
.L_08021ae8:
	mov r4, r8
	lsls r3, r4, #2
	adds r3, #40
	ldr r6, [r7, r3]
	cmp r6, #0
	bne .L_08021af6
	b .L_08021ca4
.L_08021af6:
	ldr r3, [r6, #16]
	cmp r3, #0
	bne .L_08021afe
	b .L_08021ca4
.L_08021afe:
	movs r0, #2
	ldrsh r3, [r6, r0]
	ldrh r2, [r6, #2]
	cmp r3, #0
	bgt .L_08021bce
	ldrb r3, [r6, #20]
	ldr r1, [r6, #16]
	adds r2, r3, #1
	strb r2, [r6, #20]
	lsls r3, r3, #24
	lsrs r3, r3, #24
	ldrb r0, [r1, r3]
	adds r3, r2, #1
	strb r3, [r6, #20]
	lsls r2, r2, #24
	adds r3, r0, #0
	lsrs r2, r2, #24
	subs r3, #239
	ldrb r5, [r1, r2]
	cmp r3, #16
	bhi .L_08021bc2
	ldr r2, .L_08021e0c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08021b30:
	.4byte .L_08021bb0
	.4byte .L_08021b96
	.4byte .L_08021b8c
	.4byte .L_08021b9e
	.4byte .L_08021b9a
	.4byte .L_08021afe
	.4byte .L_08021ba6
	.4byte .L_08021afe
	.4byte .L_08021afe
	.4byte .L_08021afe
	.4byte .L_08021afe
	.4byte .L_08021afe
	.4byte .L_08021bc2
	.4byte .L_08021b84
	.4byte .L_08021b80
	.4byte .L_08021b74
	.4byte .L_08021ba2
.L_08021b74:
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_08022acc
	strb r5, [r7, #24]
	b .L_08021afe
.L_08021b80:
	strb r5, [r6, #20]
	b .L_08021afe
.L_08021b84:
	adds r0, r5, #0
	bl Audio_PlayCue
	b .L_08021afe
.L_08021b8c:
	ldrb r3, [r6, #20]
	ldrb r0, [r6, #23]
	adds r3, #254
	strb r3, [r6, #20]
	b .L_08021bd6
.L_08021b96:
	strb r5, [r6, #4]
	b .L_08021afe
.L_08021b9a:
	strb r5, [r7, #22]
	b .L_08021afe
.L_08021b9e:
	strb r5, [r7, #23]
	b .L_08021afe
.L_08021ba2:
	movs r3, #255
	strb r3, [r6, #23]
.L_08021ba6:
	ldrh r3, [r6, #2]
	lsls r2, r5, #4
	adds r3, r3, r2
	strh r3, [r6, #2]
	b .L_08021afe
.L_08021bb0:
	movs r3, #255
	strb r3, [r6, #23]
	movs r3, #0
	str r3, [r6, #16]
	ldrb r3, [r7, #27]
	movs r0, #255
	adds r3, #255
	strb r3, [r7, #27]
	b .L_08021bd6
.L_08021bc2:
	ldrh r3, [r6, #2]
	lsls r2, r5, #4
	adds r3, r3, r2
	strb r0, [r6, #23]
	strh r3, [r6, #2]
	b .L_08021afe
.L_08021bce:
	ldrb r3, [r6, #21]
	ldrb r0, [r6, #23]
	subs r3, r2, r3
	strh r3, [r6, #2]
.L_08021bd6:
	ldrb r3, [r6, #4]
	subs r3, #1
	cmp r3, #21
	bhi .L_08021c7e
	ldr r2, .L_08021e10
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_08021be8:
	.4byte .L_08021c40
	.4byte .L_08021c48
	.4byte .L_08021c58
	.4byte .L_08021c7e
	.4byte .L_08021c62
	.4byte .L_08021c7e
	.4byte .L_08021c7e
	.4byte .L_08021c6c
	.4byte .L_08021c7e
	.4byte .L_08021c7e
	.4byte .L_08021c7e
	.4byte .L_08021c7e
	.4byte .L_08021c7e
	.4byte .L_08021c7e
	.4byte .L_08021c7e
	.4byte .L_08021c7e
	.4byte .L_08021c7e
	.4byte .L_08021c7e
	.4byte .L_08021c7e
	.4byte .L_08021c48
	.4byte .L_08021c7e
	.4byte .L_08021c50
.L_08021c40:
	mov r1, r10
	ldr r3, .L_08021e14
	lsrs r2, r1, #13
	b .L_08021c76
.L_08021c48:
	ldr r3, .L_08021e18
	mov r4, r10
	lsrs r2, r4, #13
	b .L_08021c76
.L_08021c50:
	mov r1, r10
	ldr r3, .L_08021e1c
	lsrs r2, r1, #13
	b .L_08021c76
.L_08021c58:
	ldr r3, .L_08021e20
	mov r4, r10
	lsrs r2, r4, #12
	movs r1, #15
	b .L_08021c78
.L_08021c62:
	mov r1, r10
	ldr r3, .L_08021e24
	lsrs r2, r1, #10
	movs r1, #63
	b .L_08021c78
.L_08021c6c:
	movs r2, #128
	lsls r2, r2, #5
	ldr r3, .L_08021e28
	add r2, r10
	lsrs r2, r2, #13
.L_08021c76:
	movs r1, #7
.L_08021c78:
	ands r2, r1
	ldrb r2, [r3, r2]
	b .L_08021c80
.L_08021c7e:
	movs r2, #0
.L_08021c80:
	movs r3, #7
	ands r3, r2
	adds r0, r0, r3
	mov r3, r8
	cmp r3, #0
	bne .L_08021c96
	lsrs r3, r2, #7
	cmp r3, #0
	beq .L_08021c96
	movs r4, #1
	str r4, [sp, #24]
.L_08021c96:
	ldrb r3, [r6, #22]
	cmp r3, r0
	beq .L_08021ca2
	movs r3, #1
	strb r0, [r6, #22]
	strb r3, [r7, #25]
.L_08021ca2:
	ldrb r2, [r7, #27]
.L_08021ca4:
	movs r0, #1
	add r8, r0
	cmp r8, r2
	bge .L_08021cae
	b .L_08021ae8
.L_08021cae:
	ldrb r2, [r7, #26]
	movs r3, #32
	ands r3, r2
	cmp r3, #0
	beq .L_08021cd4
	mov r2, r10
	ldr r1, .L_08021e18
	lsrs r3, r2, #13
	movs r2, #7
	ands r3, r2
	ldrb r2, [r1, r3]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	bne .L_08021cce
	b .L_08021fe0
.L_08021cce:
	movs r3, #1
	str r3, [sp, #24]
	b .L_08021fe0
.L_08021cd4:
	ldrb r3, [r7, #25]
	cmp r3, #0
	bne .L_08021cdc
	b .L_08021fe0
.L_08021cdc:
	ldrb r2, [r7, #20]
	ldrb r3, [r7, #21]
	adds r4, r3, #0
	muls r4, r2
	adds r0, r4, #0
	str r4, [sp, #20]
	bl Runtime_BumpAllocate
	ldr r3, .L_08021e2c
	ldr r1, [sp, #20]
	mov r9, r0
	mov lr, r3
	.2byte 0xf800
	ldrb r3, [r7, #27]
	movs r0, #1
	negs r0, r0
	subs r3, #1
	mov r10, r0
	mov r8, r3
	cmp r3, #0
	blt .L_08021d98
	lsls r3, r3, #2
	adds r3, r3, r7
	adds r3, #40
	movs r1, #0
	mov lr, r3
	mov r11, r1
.L_08021d12:
	mov r2, lr
	ldr r6, [r2]
	movs r3, #4
	negs r3, r3
	add lr, r3
	cmp r6, #0
	beq .L_08021d8c
	ldr r3, [r6, #8]
	cmp r3, #0
	beq .L_08021d8c
	ldrb r3, [r6, #22]
	cmp r3, #255
	beq .L_08021d8c
	ldrb r0, [r6, #6]
	cmp r0, #3
	bhi .L_08021d8c
	lsls r0, r0, #8
	mov r4, r8
	mov r5, r10
	orrs r0, r4
	cmp r5, #0
	blt .L_08021d72
	add r6, sp, #28
	lsls r2, r5, #1
	ldrh r3, [r6, r2]
	cmp r3, r0
	bls .L_08021d7a
	mov r1, r11
	strh r3, [r6, r1]
	adds r3, r2, r6
	mov r12, r6
	adds r4, r3, #2
	adds r1, r2, #0
.L_08021d54:
	subs r5, #1
	subs r1, #2
	cmp r5, #0
	blt .L_08021d7e
	adds r3, r1, #0
	mov r2, r12
	ldrh r2, [r3, r2]
	str r2, [sp, #4]
	cmp r2, r0
	bls .L_08021d80
	add r3, sp, #4
	ldrh r3, [r3]
	subs r4, #2
	strh r3, [r4]
	b .L_08021d54
.L_08021d72:
	mov r4, r10
	add r6, sp, #28
	lsls r3, r4, #1
	b .L_08021d80
.L_08021d7a:
	adds r3, r2, #0
	b .L_08021d80
.L_08021d7e:
	lsls r3, r5, #1
.L_08021d80:
	adds r3, #2
	strh r0, [r6, r3]
	movs r1, #1
	movs r0, #2
	add r11, r0
	add r10, r1
.L_08021d8c:
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r3, r8
	cmp r3, #0
	bge .L_08021d12
.L_08021d98:
	movs r4, #1
	movs r0, #0
	add r10, r4
	mov r8, r0
	cmp r8, r10
	bge .L_08021e6a
.L_08021da4:
	mov r1, r8
	lsls r3, r1, #1
	add r2, sp, #36
	adds r3, r3, r2
	subs r3, #8
	ldrb r3, [r3]
	lsls r3, r3, #2
	adds r3, #40
	ldr r6, [r7, r3]
	ldrb r3, [r6, #7]
	cmp r3, #1
	bne .L_08021dcc
	ldrb r3, [r6, #22]
	ldr r2, [r6, #8]
	lsls r3, r3, #2
	ldr r0, [r3, r2]
	mov r1, r9
	bl Resource_DecodeType01
	b .L_08021e62
.L_08021dcc:
	cmp r3, #3
	bne .L_08021e50
	ldrb r3, [r6, #5]
	cmp r3, #0
	beq .L_08021e30
	movs r0, #128
	lsls r0, r0, #3
	bl Runtime_BumpAllocate
	ldrb r3, [r6, #22]
	ldr r2, [r6, #8]
	adds r5, r0, #0
	lsls r3, r3, #2
	ldr r0, [r3, r2]
	adds r1, r5, #0
	bl Func_08021940
	ldrb r2, [r6, #5]
	mov r1, r9
	ldr r3, [sp, #12]
	mov lr, r3
	.2byte 0xf800
	adds r0, r5, #0
	bl Sys_Free
	b .L_08021e62
.L_08021e00:
	.4byte 0x000002c8
.L_08021e04:
	.4byte Render_DecodeFrameCopyEnd
.L_08021e08:
	.4byte Render_DecodeFrameCode
.L_08021e0c:
	.4byte .L_08021b30
.L_08021e10:
	.4byte .L_08021be8
.L_08021e14:
	.4byte Data_0802eac4
.L_08021e18:
	.4byte Data_0802eadc
.L_08021e1c:
	.4byte Data_0802ead4
.L_08021e20:
	.4byte Data_0802eae4
.L_08021e24:
	.4byte Data_0802eb04
.L_08021e28:
	.4byte Data_0802eb44
.L_08021e2c:
	.4byte IwramClearWords
.L_08021e30:
	ldrb r3, [r6, #22]
	ldr r2, [r6, #8]
	lsls r3, r3, #2
	ldr r0, [r3, r2]
	mov r1, r9
	ldr r3, .L_08021f50
	mov lr, r3
	.2byte 0xf800
	cmp r0, #0
	beq .L_08021e62
	mov r1, r9
	movs r2, #0
	ldr r4, [sp, #12]
	mov lr, r4
	.2byte 0xf800
	b .L_08021e62
.L_08021e50:
	ldrb r3, [r6, #22]
	ldr r2, [r6, #8]
	lsls r3, r3, #2
	ldr r0, [r3, r2]
	mov r1, r9
	ldrb r2, [r6, #5]
	ldr r3, [sp, #12]
	mov lr, r3
	.2byte 0xf800
.L_08021e62:
	movs r4, #1
	add r8, r4
	cmp r8, r10
	blt .L_08021da4
.L_08021e6a:
	ldrb r2, [r7, #26]
	movs r3, #14
	ands r3, r2
	cmp r3, #0
	bne .L_08021e76
	b .L_08021f9a
.L_08021e76:
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08021eb8
	ldr r5, .L_08021f54
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	adds r6, r0, #0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08021f58
	adds r1, r6, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldrb r0, [r7, #20]
	ldr r2, [sp, #16]
	ldrb r1, [r7, #21]
	ldrb r3, [r2, #6]
	ldrb r2, [r2, #7]
	str r2, [sp, #0]
	mov r2, r9
	mov lr, r6
	.2byte 0xf800
	adds r0, r6, #0
	bl Sys_Free
	ldrb r2, [r7, #26]
.L_08021eb8:
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	beq .L_08021f5c
	ldrb r3, [r7, #17]
	ldrb r5, [r7, #21]
	lsrs r0, r3, #2
	ldrb r6, [r7, #20]
	cmp r0, r5
	ble .L_08021ece
	adds r0, r5, #0
.L_08021ece:
	adds r3, r5, #0
	muls r3, r6
	mov r4, r9
	adds r2, r4, r3
	adds r3, r6, #0
	subs r2, #4
	cmp r3, #0
	beq .L_08021ef2
	movs r1, #0
	mov r8, r3
.L_08021ee2:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r4, r8
	str r1, [r2]
	subs r2, #4
	cmp r4, #0
	bne .L_08021ee2
.L_08021ef2:
	adds r4, r0, #0
	muls r4, r6
	adds r3, r4, #0
	cmp r4, #0
	bge .L_08021efe
	adds r3, r4, #3
.L_08021efe:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r1, r2, r3
	subs r3, r5, r0
	subs r3, #4
	muls r3, r6
	cmp r3, #0
	bge .L_08021f10
	adds r3, #3
.L_08021f10:
	asrs r3, r3, #2
	cmp r3, #0
	ble .L_08021f2c
	mov r8, r3
.L_08021f18:
	ldr r3, [r1]
	movs r0, #1
	negs r0, r0
	add r8, r0
	str r3, [r2]
	mov r3, r8
	subs r1, #4
	subs r2, #4
	cmp r3, #0
	bne .L_08021f18
.L_08021f2c:
	adds r3, r4, #0
	cmp r3, #0
	bge .L_08021f34
	adds r3, #3
.L_08021f34:
	movs r4, #0
	asrs r3, r3, #2
	mov r8, r4
	cmp r8, r3
	bge .L_08021f9a
	movs r1, #0
.L_08021f40:
	movs r0, #1
	add r8, r0
	str r1, [r2]
	subs r2, #4
	cmp r8, r3
	blt .L_08021f40
	b .L_08021f9a
	.2byte 0x0000
.L_08021f50:
	.4byte IwramDecompress
.L_08021f54:
	.4byte 0x000000f4
.L_08021f58:
	.4byte Data_08021824
.L_08021f5c:
	movs r3, #4
	ands r3, r2
	cmp r3, #0
	beq .L_08021f9a
	ldrb r3, [r7, #17]
	ldrb r2, [r7, #21]
	lsrs r3, r3, #2
	ldrb r0, [r7, #20]
	ldr r4, .L_08021ff4
	cmp r3, r2
	bls .L_08021f74
	adds r3, r2, #0
.L_08021f74:
	adds r1, r3, #0
	muls r1, r0
	subs r3, r2, r3
	muls r3, r0
	mov r0, r9
	adds r2, r0, r3
	cmp r1, #0
	beq .L_08021f9a
	mov r8, r1
.L_08021f86:
	ldrb r3, [r2]
	movs r1, #1
	ldrb r3, [r4, r3]
	negs r1, r1
	add r8, r1
	strb r3, [r2]
	mov r3, r8
	adds r2, #1
	cmp r3, #0
	bne .L_08021f86
.L_08021f9a:
	ldr r1, [sp, #20]
	movs r2, #0
	ldrb r0, [r7, #16]
	bl VramBlock_LoadCached
	ldr r4, .L_08021ff8
	adds r5, r0, #0
	movs r0, #192
	lsls r3, r5, #5
	lsls r0, r0, #18
	adds r3, r3, r4
	ldrb r1, [r7, #20]
	ldrb r2, [r7, #21]
	ldr r4, [r0, #84]
	mov r0, r9
	mov lr, r4
	.2byte 0xf800
	ldr r3, .L_08021ff0
	ldrh r2, [r7, #8]
	ands r5, r3
	ldr r3, .L_08021ffc
	ands r3, r2
	orrs r3, r5
	strh r3, [r7, #8]
	movs r3, #0
	strb r3, [r7, #25]
	ldr r0, [sp, #16]
	ldr r1, [sp, #20]
	ldrh r3, [r0]
	adds r2, r0, #0
	adds r3, r3, r1
	strh r3, [r2]
	mov r0, r9
	bl Sys_Free
.L_08021fe0:
	ldr r3, [sp, #8]
	cmp r3, #0
	bne .L_08022000
	movs r0, #80
	bl Runtime_ReleaseHeapBlock
	b .L_08022000
	.2byte 0x0000
.L_08021ff0:
	.4byte 0x000003ff
.L_08021ff4:
	.4byte Data_080209a8
.L_08021ff8:
	.4byte 0x06010000
.L_08021ffc:
	.4byte 0xfffffc00
.L_08022000:
	ldr r0, [sp, #24]
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
