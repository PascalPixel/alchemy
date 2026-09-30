.syntax unified
	.thumb
	.global Func_0804297c
	.thumb_func
Func_0804297c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #64]
	ldr r6, [r3, #36]
	ldr r3, [r3, #60]
	sub sp, #48
	movs r1, #0
	mov r10, r0
	ldr r0, [r5]
	str r3, [sp, #28]
	str r1, [sp, #20]
	mov r9, r0
	ldrb r3, [r3, #5]
	mov r8, r1
	cmp r3, #0
	beq .L_080429fa
	movs r0, #0
	bl Func_08118088 + 0x40
	movs r2, #1
	negs r2, r2
	movs r7, #0
	str r0, [sp, #24]
	str r2, [sp, #20]
	cmp r7, r0
	bcs .L_08042a46
	mov r3, sp
	adds r3, #36
	str r3, [sp, #12]
	movs r3, #88
	ldrh r3, [r6, r3]
	ldr r4, [sp, #12]
	movs r0, #255
	strh r3, [r4]
	lsls r0, r0, #16
	lsls r3, r3, #16
	cmp r3, r0
	beq .L_08042a46
	ldr r0, [sp, #12]
	adds r2, r6, #0
	adds r2, #88
	movs r1, #0
.L_080429de:
	ldr r3, [sp, #24]
	adds r7, #1
	adds r1, #2
	cmp r7, r3
	bcs .L_08042a46
	adds r2, #2
	ldrh r3, [r2]
	movs r4, #255
	strh r3, [r1, r0]
	lsls r4, r4, #16
	lsls r3, r3, #16
	cmp r3, r4
	bne .L_080429de
	b .L_08042a46
.L_080429fa:
	bl Func_080ad0f0
	str r0, [sp, #24]
	cmp r0, #4
	bls .L_08042a08
	movs r0, #4
	str r0, [sp, #24]
.L_08042a08:
	ldr r1, [sp, #24]
	movs r7, #0
	cmp r7, r1
	bcs .L_08042a38
	ldr r3, .L_08042a34
	mov r2, sp
	movs r4, #134
	adds r2, #36
	lsls r4, r4, #2
	str r2, [sp, #12]
	adds r1, r2, #0
	adds r2, r3, r4
.L_08042a20:
	ldrb r3, [r2]
	adds r7, #1
	strh r3, [r1]
	ldr r0, [sp, #24]
	adds r2, #1
	adds r1, #2
	cmp r7, r0
	bcc .L_08042a20
	b .L_08042a3e
	.2byte 0x0000
.L_08042a34:
	.4byte gPartyState
.L_08042a38:
	mov r1, sp
	adds r1, #36
	str r1, [sp, #12]
.L_08042a3e:
	ldr r3, .L_08042a68
	ldr r4, [sp, #12]
	lsls r2, r7, #1
	strh r3, [r4, r2]
.L_08042a46:
	movs r0, #1
	negs r0, r0
	str r7, [sp, #24]
	cmp r10, r0
	bne .L_08042a54
	ldrh r1, [r5, #12]
	mov r10, r1
.L_08042a54:
	movs r3, #1
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	bne .L_08042a6c
	movs r3, #3
	negs r3, r3
	ands r2, r3
	mov r10, r2
	b .L_08042a6c
.L_08042a68:
	.4byte 0x000000ff
.L_08042a6c:
	ldr r4, [sp, #28]
	ldrb r3, [r4, #5]
	cmp r3, #0
	beq .L_08042a80
	movs r0, #0
	movs r1, #0
	bl BattleActor_CommitPlacementFar + 0x18
	cmp r0, #0
	bne .L_08042a8a
.L_08042a80:
	movs r3, #3
	mov r0, r10
	negs r3, r3
	ands r0, r3
	mov r10, r0
.L_08042a8a:
	mov r1, r10
	cmp r1, #9
	bne .L_08042a9e
	ldrh r0, [r5, #4]
	ldrh r1, [r5, #6]
	ldrh r2, [r5, #8]
	ldrh r3, [r5, #10]
	bl Func_0803911c
	b .L_08042d94
.L_08042a9e:
	ldr r2, [sp, #28]
	movs r3, #1
	strb r3, [r2, #6]
	ldrh r3, [r5, #12]
	cmp r3, r10
	bne .L_08042aba
	mov r0, r9
	bl RenderOutput_RedrawSavedRect
	mov r0, r9
	mov r1, r10
	bl Func_080426e0
	b .L_08042af2
.L_08042aba:
	ldrh r1, [r5, #6]
	ldrh r2, [r5, #8]
	ldrh r3, [r5, #10]
	ldrh r0, [r5, #4]
	bl Func_0803911c
	mov r0, r10
	bl Func_08042630
	ldrh r3, [r5, #8]
	mov r4, r9
	strh r3, [r4, #8]
	ldrh r3, [r5, #10]
	mov r0, r9
	strh r3, [r0, #10]
	ldrh r3, [r5, #4]
	mov r1, r9
	strh r3, [r1, #12]
	ldrh r0, [r5, #4]
	ldrh r1, [r5, #6]
	ldrh r2, [r5, #8]
	ldrh r3, [r5, #10]
	bl Func_0803a084
	mov r0, r9
	mov r1, r10
	bl Func_080426e0
.L_08042af2:
	movs r3, #2
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_08042b00
	movs r3, #5
	mov r8, r3
.L_08042b00:
	ldr r4, [sp, #24]
	movs r7, #0
	cmp r4, #0
	bne .L_08042b0a
	b .L_08042c3c
.L_08042b0a:
	ldr r1, [sp, #20]
	mov r0, sp
	lsls r1, r1, #3
	adds r0, #36
	movs r4, #0
	mov r3, r8
	movs r2, #1
	str r0, [sp, #12]
	str r1, [sp, #8]
	str r1, [sp, #16]
	str r4, [sp, #4]
	add r2, r8
	lsls r3, r3, #3
	mov r11, r2
	mov r8, r3
.L_08042b28:
	ldr r1, [sp, #4]
	ldr r2, [sp, #12]
	ldrh r0, [r1, r2]
	bl Owner_GetState
	adds r6, r0, #0
	movs r3, #56
	ldrsh r5, [r6, r3]
	movs r4, #52
	ldrsh r3, [r6, r4]
	cmp r5, #0
	bne .L_08042b48
	movs r0, #2
	bl Func_08041f70
	b .L_08042b62
.L_08042b48:
	cmp r3, #0
	bge .L_08042b4e
	adds r3, #3
.L_08042b4e:
	asrs r3, r3, #2
	cmp r5, r3
	bgt .L_08042b5c
	movs r0, #4
	bl Func_08041f70
	b .L_08042b62
.L_08042b5c:
	movs r0, #15
	bl Func_08041f70
.L_08042b62:
	ldr r0, [sp, #28]
	movs r3, #14
	strb r3, [r0, #7]
	ldrb r3, [r0, #5]
	movs r2, #0
	cmp r3, #0
	beq .L_08042b74
	movs r3, #5
	strb r3, [r0, #7]
.L_08042b74:
	ldr r3, [sp, #16]
	str r2, [sp, #0]
	mov r1, r9
	mov r2, r8
	adds r3, #8
	adds r0, r5, #0
	bl UiText_DrawPrefixedNumberAtOffset
	ldr r1, [sp, #28]
	movs r3, #15
	strb r3, [r1, #7]
	mov r2, r8
	mov r1, r9
	adds r0, r6, #0
	ldr r3, [sp, #16]
	bl UiText_DrawStringAtOffset
	movs r0, #15
	bl Func_08041f70
	movs r2, #52
	ldrsh r1, [r6, r2]
	cmp r1, #0
	beq .L_08042bca
	movs r3, #56
	ldrsh r5, [r6, r3]
	lsls r0, r5, #2
	adds r0, r0, r5
	lsls r0, r0, #3
	bl __divsi3
	adds r3, r0, #0
	cmp r3, #0
	bne .L_08042bbe
	cmp r5, #0
	beq .L_08042bbe
	movs r3, #1
.L_08042bbe:
	ldr r2, [sp, #20]
	mov r0, r9
	mov r1, r11
	adds r2, #2
	bl Func_08042808
.L_08042bca:
	movs r2, #1
	mov r3, r10
	ands r3, r2
	cmp r3, #0
	beq .L_08042c24
	ldr r4, [sp, #28]
	movs r3, #14
	strb r3, [r4, #7]
	ldrb r3, [r4, #5]
	cmp r3, #0
	beq .L_08042be4
	movs r3, #5
	strb r3, [r4, #7]
.L_08042be4:
	ldr r3, [sp, #8]
	movs r1, #58
	ldrsh r0, [r6, r1]
	adds r3, #16
	str r2, [sp, #0]
	mov r1, r9
	mov r2, r8
	bl UiText_DrawPrefixedNumberAtOffset
	movs r2, #54
	ldrsh r1, [r6, r2]
	cmp r1, #0
	beq .L_08042c24
	movs r3, #58
	ldrsh r5, [r6, r3]
	lsls r0, r5, #2
	adds r0, r0, r5
	lsls r0, r0, #3
	bl __divsi3
	adds r3, r0, #0
	cmp r3, #0
	bne .L_08042c18
	cmp r5, #0
	beq .L_08042c18
	movs r3, #1
.L_08042c18:
	ldr r2, [sp, #20]
	mov r0, r9
	mov r1, r11
	adds r2, #3
	bl Func_08042808
.L_08042c24:
	ldr r1, [sp, #4]
	ldr r2, [sp, #24]
	movs r4, #6
	movs r0, #48
	adds r1, #2
	adds r7, #1
	add r11, r4
	add r8, r0
	str r1, [sp, #4]
	cmp r7, r2
	beq .L_08042c3c
	b .L_08042b28
.L_08042c3c:
	ldr r4, [sp, #28]
	movs r3, #15
	strb r3, [r4, #7]
	ldrb r3, [r4, #5]
	cmp r3, #0
	bne .L_08042c4a
	b .L_08042d8e
.L_08042c4a:
	movs r3, #2
	mov r0, r10
	ands r3, r0
	cmp r3, #0
	bne .L_08042c56
	b .L_08042d8e
.L_08042c56:
	movs r3, #1
	ands r3, r0
	ldr r6, [sp, #20]
	movs r7, #0
	cmp r3, #0
	beq .L_08042c64
	adds r6, #1
.L_08042c64:
	add r5, sp, #32
	adds r1, r5, #0
	movs r0, #0
	bl BattleActor_CommitPlacementFar + 0x18
	movs r1, #160
	lsls r1, r1, #7
	adds r1, #1
	mov r0, r9
	movs r2, #0
	adds r3, r6, #0
	str r7, [sp, #0]
	bl Func_0803c378
	movs r1, #160
	lsls r1, r1, #7
	adds r1, #2
	mov r0, r9
	movs r2, #2
	adds r3, r6, #0
	str r7, [sp, #0]
	bl Func_0803c378
	adds r2, r6, #1
	movs r1, #160
	mov r8, r2
	lsls r1, r1, #7
	adds r1, #3
	mov r0, r9
	movs r2, #0
	mov r3, r8
	str r7, [sp, #0]
	bl Func_0803c378
	movs r1, #160
	lsls r1, r1, #7
	mov r3, r8
	adds r1, #4
	mov r0, r9
	movs r2, #2
	str r7, [sp, #0]
	bl Func_0803c378
	ldrb r3, [r5]
	cmp r3, #9
	bhi .L_08042cd2
	ldrb r3, [r5, #1]
	cmp r3, #9
	bhi .L_08042cd2
	ldrb r3, [r5, #2]
	cmp r3, #9
	bhi .L_08042cd2
	ldrb r3, [r5, #3]
	cmp r3, #9
	bls .L_08042ce6
.L_08042cd2:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08042da4
	ldr r1, .L_08042da8
	adds r2, #72
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08042ce6:
	ldrb r3, [r5]
	cmp r3, #9
	bls .L_08042cf6
	movs r4, #241
	lsls r4, r4, #8
	adds r4, #150
	adds r1, r3, r4
	b .L_08042d02
.L_08042cf6:
	ldrb r3, [r5]
	adds r1, r3, #0
	movs r3, #240
	adds r1, #48
	lsls r3, r3, #8
	orrs r1, r3
.L_08042d02:
	movs r3, #0
	str r3, [sp, #0]
	mov r0, r9
	adds r3, r6, #0
	movs r2, #1
	bl Func_0803c378
	ldrb r3, [r5, #1]
	cmp r3, #9
	bls .L_08042d20
	movs r0, #241
	lsls r0, r0, #8
	adds r0, #150
	adds r1, r3, r0
	b .L_08042d2c
.L_08042d20:
	ldrb r3, [r5, #1]
	adds r1, r3, #0
	movs r3, #240
	adds r1, #48
	lsls r3, r3, #8
	orrs r1, r3
.L_08042d2c:
	movs r3, #0
	str r3, [sp, #0]
	mov r0, r9
	adds r3, r6, #0
	movs r2, #3
	bl Func_0803c378
	ldrb r3, [r5, #2]
	cmp r3, #9
	bls .L_08042d4a
	movs r2, #241
	lsls r2, r2, #8
	adds r2, #150
	adds r1, r3, r2
	b .L_08042d56
.L_08042d4a:
	ldrb r3, [r5, #2]
	adds r1, r3, #0
	movs r3, #240
	adds r1, #48
	lsls r3, r3, #8
	orrs r1, r3
.L_08042d56:
	movs r3, #0
	str r3, [sp, #0]
	mov r0, r9
	mov r3, r8
	movs r2, #1
	bl Func_0803c378
	ldrb r3, [r5, #3]
	cmp r3, #9
	bls .L_08042d74
	movs r4, #241
	lsls r4, r4, #8
	adds r4, #150
	adds r1, r3, r4
	b .L_08042d80
.L_08042d74:
	ldrb r3, [r5, #3]
	adds r1, r3, #0
	movs r3, #240
	adds r1, #48
	lsls r3, r3, #8
	orrs r1, r3
.L_08042d80:
	movs r3, #0
	str r3, [sp, #0]
	mov r0, r9
	movs r2, #3
	mov r3, r8
	bl Func_0803c378
.L_08042d8e:
	ldr r0, [sp, #28]
	movs r3, #0
	strb r3, [r0, #6]
.L_08042d94:
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08042da4:
	.4byte Data_080aa1b8
.L_08042da8:
	.4byte 0x06003400
