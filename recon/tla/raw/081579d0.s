.syntax unified
	.thumb
	.global Func_081579d0
	.thumb_func
Func_081579d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #96]
	sub sp, #44
	mov r11, r0
	ldr r0, [r3, #92]
	str r1, [sp, #32]
	mov r9, r0
	ldr r3, [r3, #100]
	movs r0, #0
	str r3, [sp, #24]
	bl Func_081435e0
	ldr r3, .L_08157a38
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	mov r2, sp
	adds r2, #36
	adds r1, r2, #0
	movs r0, #0
	str r2, [sp, #20]
	bl Func_08144aac
	ldr r0, .L_08157a3c
	ldr r1, [sp, #24]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_08157a40
	add r1, r9
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r1, #162
	lsls r1, r1, #4
	ldr r0, .L_08157a44
	add r1, r9
	b .L_08157a48
	.2byte 0x0000
.L_08157a38:
	.4byte 0x00001010
.L_08157a3c:
	.4byte 0x00000134
.L_08157a40:
	.4byte 0x00000153
.L_08157a44:
	.4byte 0x0000014c
.L_08157a48:
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	movs r3, #75
	add r2, r9
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_08157ce0
	lsls r1, r1, #4
	bl Func_080145a8
	movs r3, #0
	mov r10, r3
	movs r6, #63
	mov r5, r9
.L_08157a78:
	bl Random16
	ands r0, r6
	adds r0, #64
	str r0, [r5]
	bl Random16
	movs r7, #1
	ands r0, r6
	subs r0, #80
	add r10, r7
	str r0, [r5, #4]
	mov r0, r10
	adds r5, #28
	cmp r0, #32
	bne .L_08157a78
	ldr r3, .L_08157ce4
	movs r1, #0
	movs r2, #128
	mov r10, r1
	lsls r2, r2, #2
	subs r1, #1
.L_08157aa4:
	movs r7, #1
	add r10, r7
	str r1, [r3]
	adds r3, #28
	cmp r10, r2
	bne .L_08157aa4
	movs r0, #171
	bl Audio_PlayCue
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	str r0, [sp, #28]
	str r1, [sp, #16]
.L_08157ac0:
	ldr r2, [sp, #28]
	cmp r2, #56
	bne .L_08157acc
	movs r0, #133
	bl Func_08118088 + 0x60
.L_08157acc:
	ldr r3, [sp, #28]
	cmp r3, #95
	bgt .L_08157b0e
	ldr r0, [sp, #16]
	bl Trig_Sin
	ldr r7, [sp, #28]
	movs r5, #64
	lsls r3, r7, #1
	subs r5, r5, r3
	adds r6, r5, #0
	muls r6, r0
	ldr r0, [sp, #16]
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r2, #20
	asrs r6, r6, #17
	movs r1, #224
	asrs r3, r3, #16
	adds r6, #86
	str r2, [sp, #0]
	lsls r1, r1, #3
	movs r2, #40
	str r2, [sp, #4]
	adds r3, #28
	ldr r4, [sp, #36]
	ldr r0, [sp, #32]
	add r1, r9
	adds r2, r6, #0
	mov lr, r4
	.2byte 0xf800
.L_08157b0e:
	movs r0, #0
	str r0, [sp, #12]
	mov r10, r0
	mov r8, r9
.L_08157b16:
	mov r1, r10
	ldr r2, [sp, #28]
	lsls r3, r1, #2
	adds r3, #8
	cmp r2, r3
	blt .L_08157c08
	mov r7, r8
	ldr r3, [r7, #4]
	cmp r3, #95
	bgt .L_08157c08
	movs r1, #40
	ldr r2, [r7]
	str r1, [sp, #0]
	movs r1, #64
	str r1, [sp, #4]
	movs r1, #162
	lsls r1, r1, #4
	subs r3, #32
	subs r2, #20
	ldr r4, [sp, #36]
	ldr r0, [sp, #32]
	add r1, r9
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r7]
	subs r3, #6
	str r3, [r7]
	ldr r3, [r7, #4]
	adds r3, #12
	str r3, [r7, #4]
	cmp r3, #95
	ble .L_08157c08
	ldr r0, [sp, #12]
	ldr r1, .L_08157ce8
	movs r4, #0
	adds r7, r0, r1
.L_08157b5e:
	str r4, [sp, #8]
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	ands r5, r0
	mov r0, r8
	ldr r3, [r0]
	movs r2, #128
	lsls r3, r3, #16
	str r3, [r7]
	lsls r2, r2, #1
	ldr r3, [r0, #4]
	adds r0, r6, #0
	lsls r3, r3, #16
	str r3, [r7, #4]
	adds r5, r5, r2
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	ldr r4, [sp, #8]
	movs r3, #15
	ands r3, r0
	adds r3, #32
	adds r4, #1
	str r3, [r7, #24]
	adds r7, #28
	cmp r4, #32
	bne .L_08157b5e
	movs r0, #133
	bl Audio_PlayCue
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r9
	movs r2, #4
	str r2, [r3]
	mov r1, r11
	ldr r3, [r1, #20]
	movs r4, #0
	cmp r3, #0
	beq .L_08157c08
	movs r5, #36
.L_08157bde:
	mov r2, r11
	ldrsh r0, [r5, r2]
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r4, #0
	movs r2, #5
	mov r7, r11
	str r4, [sp, #8]
	bl Func_0814cd48
	ldrsh r0, [r5, r7]
	movs r1, #6
	bl Func_08118088
	ldr r4, [sp, #8]
	ldr r3, [r7, #20]
	adds r4, #1
	adds r5, #2
	cmp r4, r3
	bne .L_08157bde
.L_08157c08:
	ldr r3, [sp, #12]
	movs r7, #224
	movs r0, #1
	lsls r7, r7, #2
	add r10, r0
	movs r2, #28
	adds r3, r3, r7
	mov r1, r10
	add r8, r2
	str r3, [sp, #12]
	cmp r1, #8
	beq .L_08157c22
	b .L_08157b16
.L_08157c22:
	ldr r5, .L_08157ce8
	ldr r6, .L_08157cec
	movs r2, #0
	mov r10, r2
.L_08157c2a:
	ldr r0, [r5, #24]
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_08157c7a
	cmp r0, #0
	bge .L_08157c3a
	adds r0, #15
.L_08157c3a:
	asrs r0, r0, #4
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r7, [sp, #24]
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	adds r1, r7, r1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #20]
	ldr r4, [r0, #4]
	ldr r0, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r5, #0
	movs r1, #62
	lsls r2, r2, #6
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_08157c7a:
	movs r1, #1
	movs r2, #128
	add r10, r1
	lsls r2, r2, #2
	adds r5, #28
	cmp r10, r2
	bne .L_08157c2a
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r7, .L_08157cf0
	ldr r3, [sp, #16]
	ldr r0, [sp, #28]
	adds r3, r3, r7
	adds r0, #1
	str r3, [sp, #16]
	str r0, [sp, #28]
	cmp r0, #96
	beq .L_08157cba
	b .L_08157ac0
.L_08157cba:
	ldr r0, .L_08157ce0
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08157ce0:
	.4byte Func_08143000
.L_08157ce4:
	.4byte Data_02010018
.L_08157ce8:
	.4byte gMapCellBuffer
.L_08157cec:
	.4byte Data_08197410
.L_08157cf0:
	.4byte 0xfffff800
