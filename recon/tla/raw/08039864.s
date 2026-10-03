.syntax unified
	.thumb
	.global Func_08039864
	.thumb_func
Func_08039864:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	ldr r1, .L_08039ba4
	mov r8, r3
	ldr r3, [r1]
	ldr r3, .L_08039ba8
	adds r6, r0, #0
	movs r0, #139
	lsls r0, r0, #2
	adds r3, r3, r0
	ldrb r3, [r3]
	ldr r2, .L_08039bac
	sub sp, #52
	ldrb r2, [r2, r3]
	str r2, [sp, #32]
	mov r2, r8
	ldrb r3, [r2, #5]
	cmp r3, #0
	beq .L_080398b4
	ldr r3, .L_08039bb0
	ldrh r3, [r3]
	adds r2, r3, #0
	cmp r2, #0
	bge .L_080398a6
	movs r2, #0
.L_080398a6:
	cmp r2, #3
	ble .L_080398ac
	movs r2, #3
.L_080398ac:
	lsls r3, r2, #2
	adds r3, r3, r2
	adds r3, #3
	str r3, [sp, #32]
.L_080398b4:
	ldrh r3, [r6, #28]
	cmp r3, #0
	beq .L_080398ca
	movs r0, #1
	bl Func_0803975c
	ldrh r3, [r6, #28]
	movs r0, #0
	subs r3, #1
	strh r3, [r6, #28]
	b .L_08039ec2
.L_080398ca:
	ldr r3, [r1]
	cmp r3, #0
	bne .L_080398e4
	ldrh r2, [r6, #34]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080398e4
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r2, r4
	strh r3, [r6, #34]
	b .L_08039ec0
.L_080398e4:
	ldrh r3, [r6, #32]
	movs r7, #0
	cmp r3, #0
	bne .L_080398fa
	ldrh r3, [r6, #18]
	movs r0, #244
	lsls r3, r3, #1
	lsls r0, r0, #4
	adds r3, r3, r0
	mov r1, r8
	ldrh r7, [r1, r3]
.L_080398fa:
	cmp r7, #30
	bls .L_08039906
	cmp r7, #176
	beq .L_08039904
	b .L_08039d00
.L_08039904:
	b .L_08039cf0
.L_08039906:
	ldr r2, .L_08039bb4
	lsls r3, r7, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_08039910:
	.4byte .L_08039ce4
	.4byte .L_080399d8
	.4byte .L_08039bbc
	.4byte .L_0803998c
	.4byte .L_08039c16
	.4byte .L_08039bee
	.4byte .L_08039bf8
	.4byte .L_08039ca4
	.4byte .L_08039c2e
	.4byte .L_08039c60
	.4byte .L_08039c88
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cbc
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039cf0
	.4byte .L_08039ce4
.L_0803998c:
	ldrh r3, [r6, #30]
	strh r3, [r6, #4]
	ldr r3, [r6]
	ldrh r2, [r3, #22]
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	beq .L_080399be
	movs r3, #192
	ldrh r2, [r6, #6]
	lsls r3, r3, #4
	adds r3, #255
	cmp r2, r3
	bls .L_080399b4
	adds r0, r6, #0
	bl Func_08039754
	movs r2, #1
	str r2, [sp, #32]
	b .L_08039cf0
.L_080399b4:
	movs r4, #208
	lsls r4, r4, #4
	adds r3, r2, r4
	strh r3, [r6, #6]
	b .L_08039cf0
.L_080399be:
	ldrh r3, [r6, #6]
	movs r0, #240
	ldrh r2, [r6, #16]
	lsls r0, r0, #4
	adds r3, r3, r0
	strh r3, [r6, #6]
	adds r3, r2, #0
	cmp r3, #2
	bls .L_080399d2
	b .L_08039cf0
.L_080399d2:
	adds r3, r2, #1
	strh r3, [r6, #16]
	b .L_08039cf0
.L_080399d8:
	ldrh r1, [r6, #20]
	cmp r1, #0
	bne .L_080399fa
	ldr r3, .L_08039ba8
	movs r4, #139
	lsls r4, r4, #2
	adds r3, r3, r4
	ldr r2, .L_08039bb8
	ldrb r3, [r3]
	mov r0, r8
	ldrb r3, [r2, r3]
	strh r3, [r6, #20]
	ldrb r3, [r0, #4]
	cmp r3, #0
	beq .L_080399fa
	ldr r3, .L_08039ba4
	str r1, [r3, #28]
.L_080399fa:
	adds r0, r6, #0
	bl Func_0803cdb4
	cmp r0, #0
	bne .L_08039a40
	ldr r0, [r6]
	ldrh r3, [r0, #8]
	cmp r3, #0
	bne .L_08039a0e
	b .L_08039cf0
.L_08039a0e:
	ldrh r3, [r0, #10]
	cmp r3, #0
	bne .L_08039a16
	b .L_08039cf0
.L_08039a16:
	movs r7, #152
	lsls r7, r7, #5
	adds r7, #136
	add r7, r8
	ldrb r3, [r7]
	cmp r3, #0
	beq .L_08039a26
	b .L_08039cf0
.L_08039a26:
	ldrh r2, [r0, #8]
	ldrh r3, [r0, #10]
	lsls r2, r2, #2
	lsls r3, r3, #3
	movs r5, #1
	subs r2, #8
	subs r3, #16
	movs r1, #1
	str r5, [sp, #0]
	bl Func_0803bde4
	strb r5, [r7]
	b .L_08039cf0
.L_08039a40:
	ldr r5, [r6]
	ldrh r4, [r6, #18]
	movs r1, #12
	ldrsh r3, [r5, r1]
	str r4, [sp, #12]
	str r3, [sp, #48]
	movs r2, #14
	ldrsh r3, [r5, r2]
	movs r2, #152
	str r3, [sp, #44]
	ldrh r3, [r5, #8]
	lsls r2, r2, #5
	str r3, [sp, #28]
	adds r2, #136
	ldrh r0, [r5, #10]
	movs r3, #0
	add r2, r8
	str r0, [sp, #24]
	strb r3, [r2]
	adds r0, r5, #0
	bl RenderOutput_PrepareForRedraw
	ldrh r3, [r6, #36]
	ldr r4, [sp, #12]
	cmp r3, #0
	bne .L_08039a90
	ldrh r2, [r5, #8]
	ldrh r3, [r5, #10]
	orrs r3, r2
	cmp r3, #0
	beq .L_08039a90
	movs r1, #12
	ldrsh r0, [r5, r1]
	movs r2, #14
	ldrsh r1, [r5, r2]
	ldrh r3, [r5, #10]
	ldrh r2, [r5, #8]
	bl UiWindow_EraseBorderRect
	ldr r4, [sp, #12]
.L_08039a90:
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	adds r4, #1
	ands r4, r3
	movs r0, #244
	lsls r3, r4, #1
	lsls r0, r0, #4
	adds r3, r3, r0
	mov r1, r8
	ldrh r3, [r1, r3]
	cmp r3, #0
	beq .L_08039b76
	ldrh r2, [r5, #8]
	ldrh r3, [r5, #10]
	orrs r3, r2
	cmp r3, #0
	beq .L_08039b76
	ldrh r7, [r6, #36]
	cmp r7, #0
	beq .L_08039acc
	movs r2, #12
	ldrsh r0, [r5, r2]
	movs r3, #14
	ldrsh r1, [r5, r3]
	ldrh r2, [r5, #8]
	ldrh r3, [r5, #10]
	bl UiWindow_EraseBorderRect
	b .L_08039b66
.L_08039acc:
	add r0, sp, #48
	mov r1, sp
	mov r2, sp
	mov r11, r0
	adds r1, #44
	adds r2, #40
	movs r3, #36
	movs r0, #8
	add r3, sp
	str r1, [sp, #20]
	str r2, [sp, #16]
	adds r0, r0, r6
	str r3, [sp, #0]
	str r0, [sp, #4]
	mov r1, r11
	mov r9, r3
	mov r10, r0
	ldr r3, [sp, #16]
	adds r0, r4, #0
	ldr r2, [sp, #20]
	str r4, [sp, #12]
	str r7, [sp, #8]
	bl UiWindow_FitOnScreen
	ldrh r1, [r5, #22]
	movs r3, #128
	ands r3, r1
	ldr r4, [sp, #12]
	cmp r3, #0
	beq .L_08039b20
	ldr r2, [sp, #36]
	ldr r3, [sp, #24]
	cmp r3, r2
	beq .L_08039b18
	subs r2, r2, r3
	ldr r3, [sp, #44]
	subs r3, r3, r2
	str r3, [sp, #44]
.L_08039b18:
	ldr r3, [sp, #44]
	cmp r3, #0
	bge .L_08039b20
	str r7, [sp, #44]
.L_08039b20:
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r1
	cmp r3, #0
	bne .L_08039b56
	ldr r3, [sp, #40]
	ldr r0, [sp, #28]
	subs r3, r0, r3
	cmp r3, #0
	bge .L_08039b36
	adds r3, #3
.L_08039b36:
	ldr r2, [sp, #48]
	asrs r3, r3, #2
	adds r2, r2, r3
	mov r1, r9
	movs r3, #2
	str r2, [sp, #48]
	mov r2, r10
	str r1, [sp, #0]
	str r2, [sp, #4]
	str r3, [sp, #8]
	adds r0, r4, #0
	mov r1, r11
	ldr r2, [sp, #20]
	ldr r3, [sp, #16]
	bl UiWindow_FitOnScreen
.L_08039b56:
	ldr r3, [sp, #48]
	strh r3, [r5, #12]
	ldr r3, [sp, #44]
	strh r3, [r5, #14]
	ldr r3, [sp, #40]
	strh r3, [r5, #8]
	ldr r3, [sp, #36]
	strh r3, [r5, #10]
.L_08039b66:
	movs r3, #12
	ldrsh r0, [r5, r3]
	movs r4, #14
	ldrsh r1, [r5, r4]
	ldrh r2, [r5, #8]
	ldrh r3, [r5, #10]
	bl Func_0803a084
.L_08039b76:
	ldrh r3, [r6, #30]
	movs r5, #152
	lsls r5, r5, #5
	strh r3, [r6, #4]
	adds r5, #70
	movs r3, #15
	movs r2, #0
	strh r3, [r6, #22]
	adds r0, r6, #0
	movs r3, #10
	add r5, r8
	strh r3, [r6, #26]
	strh r2, [r6, #6]
	strh r2, [r6, #16]
	strh r2, [r6, #24]
	bl Func_0803972c
	ldrh r0, [r5]
	bl Resource_ResetEntry
	movs r3, #99
	strh r3, [r5]
	b .L_08039cf0
.L_08039ba4:
	.4byte gInput
.L_08039ba8:
	.4byte gPartyState
.L_08039bac:
	.4byte Data_080aa0e2
.L_08039bb0:
	.4byte gLagFramesShown
.L_08039bb4:
	.4byte .L_08039910
.L_08039bb8:
	.4byte Data_080aa0df
.L_08039bbc:
	ldrh r1, [r6, #20]
	cmp r1, #0
	bne .L_08039bde
	ldr r3, .L_08039c54
	movs r0, #139
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r2, .L_08039c58
	ldrb r3, [r3]
	ldrb r3, [r2, r3]
	mov r2, r8
	strh r3, [r6, #20]
	ldrb r3, [r2, #4]
	cmp r3, #0
	beq .L_08039bde
	ldr r3, .L_08039c5c
	str r1, [r3, #28]
.L_08039bde:
	adds r0, r6, #0
	bl Func_0803cdb4
	cmp r0, #0
	bne .L_08039bea
	b .L_08039cf0
.L_08039bea:
	movs r0, #9
	b .L_08039ec2
.L_08039bee:
	ldrh r3, [r6, #20]
	cmp r3, #0
	bne .L_08039c02
	movs r3, #20
	b .L_08039c00
.L_08039bf8:
	ldrh r3, [r6, #20]
	cmp r3, #0
	bne .L_08039c02
	movs r3, #120
.L_08039c00:
	strh r3, [r6, #20]
.L_08039c02:
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #134
	add r2, r8
	movs r3, #0
	strh r3, [r2]
	adds r0, r6, #0
	bl Func_0803cd5c
	b .L_08039cf0
.L_08039c16:
	ldrh r3, [r6, #20]
	cmp r3, #0
	bne .L_08039c20
	movs r3, #60
	strh r3, [r6, #20]
.L_08039c20:
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #134
	add r2, r8
	movs r3, #0
	strh r3, [r2]
	b .L_08039cf0
.L_08039c2e:
	ldrh r3, [r6, #18]
	ldr r2, .L_08039c50
	adds r3, #1
	ands r3, r2
	strh r3, [r6, #18]
	movs r4, #244
	ldrh r3, [r6, #18]
	lsls r4, r4, #4
	lsls r3, r3, #1
	mov r0, r8
	adds r3, r3, r4
	ldrh r3, [r0, r3]
	adds r0, r6, #0
	strh r3, [r6, #22]
	bl Func_0803972c
	b .L_08039cf0
.L_08039c50:
	.4byte 0x000001ff
.L_08039c54:
	.4byte gPartyState
.L_08039c58:
	.4byte Data_080aa0df
.L_08039c5c:
	.4byte gInput
.L_08039c60:
	ldrh r3, [r6, #18]
	ldr r2, .L_08039c84
	adds r3, #1
	ands r3, r2
	strh r3, [r6, #18]
	movs r1, #244
	ldrh r3, [r6, #18]
	lsls r1, r1, #4
	lsls r3, r3, #1
	adds r3, r3, r1
	mov r2, r8
	ldrh r3, [r2, r3]
	adds r0, r6, #0
	strh r3, [r6, #24]
	bl Func_0803972c
	b .L_08039cf0
	.2byte 0x0000
.L_08039c84:
	.4byte 0x000001ff
.L_08039c88:
	ldrh r3, [r6, #18]
	ldr r2, .L_08039cb8
	adds r3, #1
	ands r3, r2
	strh r3, [r6, #18]
	movs r4, #244
	ldrh r3, [r6, #18]
	lsls r4, r4, #4
	lsls r3, r3, #1
	mov r0, r8
	adds r3, r3, r4
	ldrh r3, [r0, r3]
	adds r0, r6, #0
	b .L_08039cb0
.L_08039ca4:
	movs r3, #0
	movs r2, #15
	strh r3, [r6, #24]
	adds r0, r6, #0
	movs r3, #10
	strh r2, [r6, #22]
.L_08039cb0:
	strh r3, [r6, #26]
	bl Func_0803972c
	b .L_08039cf0
.L_08039cb8:
	.4byte 0x000001ff
.L_08039cbc:
	ldrh r3, [r6, #18]
	ldr r0, .L_08039cec
	adds r3, #1
	ands r3, r0
	strh r3, [r6, #18]
	movs r4, #244
	ldrh r2, [r6, #18]
	lsls r4, r4, #4
	lsls r3, r2, #1
	adds r3, r3, r4
	mov r4, r8
	ldrh r3, [r4, r3]
	ldr r1, [r6]
	adds r2, #1
	strh r3, [r1, #18]
	ands r2, r0
	movs r3, #11
	strh r3, [r6, #20]
	strh r2, [r6, #18]
	b .L_08039cf0
.L_08039ce4:
	movs r3, #1
	strh r3, [r6, #32]
	movs r0, #8
	b .L_08039ec2
.L_08039cec:
	.4byte 0x000001ff
.L_08039cf0:
	mov r0, r8
	ldrb r3, [r0, #5]
	cmp r3, #0
	beq .L_08039cfa
	b .L_08039e8c
.L_08039cfa:
	movs r1, #1
	str r1, [sp, #32]
	b .L_08039e8c
.L_08039d00:
	ldrh r3, [r6, #4]
	adds r2, r3, #0
	adds r2, #128
	cmp r2, #0
	bge .L_08039d10
	movs r4, #128
	adds r4, #255
	adds r2, r3, r4
.L_08039d10:
	ldrh r3, [r6, #6]
	asrs r5, r2, #8
	adds r0, r3, #0
	adds r0, #128
	cmp r0, #0
	bge .L_08039d22
	movs r1, #128
	adds r1, #255
	adds r0, r3, r1
.L_08039d22:
	ldr r3, .L_08039d64
	movs r4, #139
	lsls r4, r4, #2
	adds r3, r3, r4
	ldrb r3, [r3]
	ldr r2, .L_08039d68
	asrs r0, r0, #8
	mov r12, r0
	mov r0, r8
	ldrb r2, [r2, r3]
	ldrb r3, [r0, #4]
	mov r10, r2
	ldrh r2, [r6, #18]
	cmp r3, #0
	beq .L_08039d42
	adds r5, #8
.L_08039d42:
	adds r3, r2, #1
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	ands r3, r2
	movs r1, #244
	lsls r3, r3, #1
	lsls r1, r1, #4
	adds r3, r3, r1
	mov r4, r8
	ldrh r0, [r4, r3]
	cmp r0, #222
	bne .L_08039d6c
	movs r3, #128
	lsls r3, r3, #7
	b .L_08039d74
	.2byte 0x0000
.L_08039d64:
	.4byte gPartyState
.L_08039d68:
	.4byte Data_080aa0e5
.L_08039d6c:
	cmp r0, #223
	bne .L_08039d7e
	movs r3, #128
	lsls r3, r3, #8
.L_08039d74:
	orrs r7, r3
	ldrh r3, [r6, #18]
	adds r3, #1
	ands r3, r2
	strh r3, [r6, #18]
.L_08039d7e:
	ldr r4, [r6]
	movs r3, #8
	ldrh r2, [r4, #22]
	ands r3, r2
	cmp r3, #0
	bne .L_08039dc8
	cmp r7, #32
	bls .L_08039dc8
	cmp r0, #32
	bls .L_08039dc8
	ldr r1, .L_08039dc4
	adds r3, r7, #0
	adds r2, r0, #0
	subs r3, #32
	subs r2, #32
	lsls r3, r3, #5
	lsls r2, r2, #5
	ldrh r3, [r1, r3]
	ldrh r2, [r1, r2]
	movs r1, #240
	adds r3, r3, r2
	lsls r3, r3, #16
	lsls r1, r1, #12
	cmp r3, r1
	bhi .L_08039dc8
	lsls r3, r0, #8
	orrs r7, r3
	ldrh r3, [r6, #18]
	ldr r2, .L_08039dc0
	adds r3, #1
	ands r3, r2
	strh r3, [r6, #18]
	b .L_08039dc8
.L_08039dc0:
	.4byte 0x000001ff
.L_08039dc4:
	.4byte UiText_Glyphs
.L_08039dc8:
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #140
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_08039df4
	ldrh r3, [r6, #38]
	cmp r3, #0
	beq .L_08039df4
	ldrh r0, [r6, #6]
	movs r1, #160
	adds r0, #128
	lsls r1, r1, #1
	str r4, [sp, #12]
	bl __divsi3
	ldr r4, [sp, #12]
	subs r0, #3
	mov r12, r0
.L_08039df4:
	movs r3, #0
	str r3, [sp, #0]
	adds r0, r4, #0
	adds r2, r5, #0
	mov r3, r12
	adds r1, r7, #0
	bl Func_0803bde4
	ldr r3, .L_08039e54
	adds r4, r0, #0
	movs r0, #139
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r2, .L_08039e58
	ldrb r3, [r3]
	ldrb r3, [r2, r3]
	strh r3, [r6, #34]
	cmp r4, #0
	bne .L_08039e1c
	b .L_08039bea
.L_08039e1c:
	movs r1, #152
	lsls r1, r1, #5
	adds r1, #132
	add r1, r8
	ldrh r3, [r1]
	cmp r3, #0
	beq .L_08039e66
	movs r5, #152
	lsls r5, r5, #5
	adds r5, #134
	add r5, r8
	ldrh r2, [r5]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_08039e5c
	cmp r7, #32
	beq .L_08039e66
	ldrh r0, [r1]
	movs r3, #3
	ands r3, r7
	adds r0, r0, r3
	str r4, [sp, #12]
	bl Audio_PlayCue
	mov r1, r10
	strh r1, [r5]
	ldr r4, [sp, #12]
	b .L_08039e66
.L_08039e54:
	.4byte gPartyState
.L_08039e58:
	.4byte Data_080aa0dc
.L_08039e5c:
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r5]
.L_08039e66:
	lsls r0, r4, #8
	cmp r7, #32
	bne .L_08039e76
	ldrh r3, [r6, #16]
	lsls r3, r3, #1
	adds r3, #8
	ldrh r3, [r6, r3]
	adds r0, r0, r3
.L_08039e76:
	ldrh r3, [r6, #4]
	adds r3, r3, r0
	strh r3, [r6, #4]
	cmp r7, #32
	bne .L_08039e8c
	mov r1, r8
	ldrb r3, [r1, #5]
	cmp r3, #0
	bne .L_08039e8c
	movs r2, #1
	str r2, [sp, #32]
.L_08039e8c:
	ldrh r2, [r6, #20]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_08039ea4
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r2, r4
	strh r3, [r6, #20]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_08039eae
.L_08039ea4:
	ldrh r3, [r6, #18]
	ldr r2, .L_08039ebc
	adds r3, #1
	ands r3, r2
	strh r3, [r6, #18]
.L_08039eae:
	ldr r0, [sp, #32]
	subs r0, #1
	str r0, [sp, #32]
	cmp r0, #0
	beq .L_08039eba
	b .L_080398e4
.L_08039eba:
	b .L_08039ec0
.L_08039ebc:
	.4byte 0x000001ff
.L_08039ec0:
	movs r0, #0
.L_08039ec2:
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
