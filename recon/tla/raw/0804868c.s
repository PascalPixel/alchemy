.syntax unified
	.thumb
	.global Func_0804868c
	.thumb_func
Func_0804868c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #224
	str r0, [sp, #76]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #60]
	movs r2, #1
	str r0, [sp, #72]
	movs r1, #0
	negs r2, r2
	movs r0, #128
	str r1, [sp, #68]
	str r2, [sp, #64]
	bl Resource_LoadIntoFreeSlot
	str r0, [sp, #60]
	movs r0, #168
	lsls r0, r0, #1
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #56]
	ldr r0, [sp, #64]
	movs r3, #0
	str r3, [sp, #52]
	movs r3, #42
	movs r6, #0
	str r0, [sp, #48]
	str r3, [sp, #0]
	movs r1, #4
	movs r2, #30
	movs r3, #4
	movs r0, #0
	str r6, [sp, #40]
	str r6, [sp, #32]
	str r6, [sp, #80]
	str r6, [sp, #28]
	str r6, [sp, #24]
	str r6, [sp, #20]
	bl UiWindow_Create
	str r0, [sp, #44]
	movs r0, #1
	bl Func_08041c0c
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #9
	movs r2, #10
	movs r3, #11
	movs r0, #20
	bl UiWindow_Create
	mov r9, r0
	adds r5, #228
	ldr r3, [r5]
	ldr r1, [r3, #52]
	ldr r2, [r3, #48]
	ldr r3, [r3, #56]
	mov r11, r1
	mov r10, r2
	str r3, [sp, #36]
	ldr r0, [sp, #76]
	bl Owner_GetState
	adds r0, #248
	movs r7, #0
	mov r8, r0
.L_0804871e:
	ldr r0, [sp, #52]
	ldr r1, [sp, #56]
	lsls r3, r0, #2
	movs r6, #0
	adds r5, r3, r1
.L_08048728:
	mov r0, r8
	ldr r3, [r0, #16]
	movs r2, #1
	lsls r2, r6
	ands r3, r2
	cmp r3, #0
	beq .L_08048744
	lsls r3, r7, #8
	orrs r3, r6
	stmia r5!, {r3}
	ldr r1, [sp, #52]
	adds r1, #1
	str r1, [sp, #52]
	b .L_080487da
.L_08048744:
	mov r0, r8
	ldr r3, [r0]
	ands r3, r2
	cmp r3, #0
	beq .L_080487da
	ldr r1, [sp, #76]
	movs r0, #0
	cmp r1, #7
	bls .L_08048758
	movs r0, #1
.L_08048758:
	bl Resource_FarCall005
	movs r2, #148
	adds r3, r0, #0
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r3, [r3]
	movs r1, #0
	adds r0, #8
	movs r4, #0
	cmp r1, r3
	bge .L_080487b4
	ldrb r3, [r0, #2]
	ldr r2, [sp, #76]
	cmp r3, r2
	bne .L_08048784
	ldrb r3, [r0]
	cmp r3, r7
	bne .L_08048784
	ldrb r3, [r0, #1]
	cmp r3, r6
	beq .L_080487ae
.L_08048784:
	movs r2, #144
	lsls r2, r2, #1
	adds r3, r0, r2
	ldr r3, [r3]
	adds r1, #1
	cmp r1, r3
	bge .L_080487b2
	lsls r4, r1, #2
	adds r2, r0, r4
	ldrb r3, [r2, #2]
	mov r12, r3
	ldr r3, [sp, #76]
	cmp r12, r3
	bne .L_08048784
	ldrb r3, [r2]
	cmp r3, r7
	bne .L_08048784
	ldrb r3, [r2, #1]
	cmp r3, r6
	bne .L_08048784
	b .L_080487b4
.L_080487ae:
	movs r4, #0
	b .L_080487b4
.L_080487b2:
	lsls r4, r1, #2
.L_080487b4:
	lsls r2, r7, #8
	movs r3, #128
	lsls r3, r3, #9
	orrs r2, r6
	orrs r2, r3
	str r2, [r5]
	adds r3, r0, r4
	ldrb r3, [r3, #3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	ble .L_080487d2
	lsls r3, r3, #17
	orrs r2, r3
	str r2, [r5]
.L_080487d2:
	ldr r0, [sp, #52]
	adds r5, #4
	adds r0, #1
	str r0, [sp, #52]
.L_080487da:
	adds r6, #1
	cmp r6, #19
	ble .L_08048728
	movs r1, #4
	adds r7, #1
	add r8, r1
	cmp r7, #3
	ble .L_0804871e
	ldr r2, [sp, #52]
	ldr r0, [sp, #56]
	lsls r3, r2, #2
	movs r2, #128
	lsls r2, r2, #24
	str r2, [r3, r0]
	ldr r1, [sp, #72]
	movs r3, #1
	strb r3, [r1, #3]
	mov r2, sp
	mov r3, sp
	adds r2, #212
	adds r3, #84
	str r2, [sp, #4]
	str r3, [sp, #8]
.L_08048808:
	ldr r0, [sp, #48]
	cmp r11, r0
	bne .L_0804881c
	ldr r1, [sp, #64]
	cmp r10, r1
	bne .L_0804881c
	ldr r2, [sp, #28]
	cmp r2, #0
	bne .L_0804881c
	b .L_08048b3a
.L_0804881c:
	ldr r0, [sp, #56]
	mov r3, r11
	ldr r2, [sp, #72]
	add r3, r10
	lsls r3, r3, #2
	ldr r5, [r3, r0]
	movs r1, #0
	movs r3, #1
	str r1, [sp, #40]
	strb r3, [r2, #6]
	mov r1, r9
	movs r3, #12
	ldrsh r0, [r1, r3]
	movs r2, #14
	ldrsh r1, [r1, r2]
	ldr r2, [sp, #64]
	adds r0, #1
	lsls r3, r2, #1
	adds r1, r1, r3
	mov r3, r9
	ldrh r2, [r3, #8]
	movs r3, #15
	str r3, [sp, #0]
	adds r1, #1
	subs r2, #2
	movs r3, #1
	bl Func_08046134
	ldr r0, [sp, #32]
	cmp r0, #0
	beq .L_08048878
	ldr r0, [sp, #44]
	movs r1, #1
	bl UiWork_Finalize
	movs r3, #42
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #4
	movs r2, #30
	movs r3, #4
	bl UiWindow_Create
	str r0, [sp, #44]
	bl Func_080396bc
.L_08048878:
	ldr r2, [sp, #52]
	movs r1, #0
	str r1, [sp, #28]
	cmp r2, #0
	bne .L_08048884
	b .L_080489a0
.L_08048884:
	bl Func_0803cca8
	movs r3, #0
	str r3, [sp, #24]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r2, [r3]
	movs r3, #228
	ldr r3, [r2, r3]
	ldr r1, [sp, #28]
	cmp r3, r5
	bne .L_080488a4
	movs r0, #1
	str r0, [sp, #24]
	b .L_080488c0
.L_080488a4:
	adds r1, #1
	cmp r1, #7
	bgt .L_080488c0
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r2, [r3]
	lsls r3, r1, #2
	adds r3, #228
	ldr r3, [r2, r3]
	cmp r3, r5
	bne .L_080488a4
	movs r1, #1
	str r1, [sp, #24]
.L_080488c0:
	ldr r2, [sp, #24]
	cmp r2, #0
	beq .L_080488e8
	ldr r6, [sp, #8]
	ldr r0, .L_08048bf4
	adds r1, r6, #0
	movs r2, #52
	bl UiText_CopyMessageString
	ldr r3, [sp, #68]
	cmp r3, #0
	beq .L_080489ac
	adds r0, r3, #0
	movs r1, #1
	bl UiWork_Finalize
	movs r0, #0
	str r0, [sp, #68]
	str r0, [sp, #32]
	b .L_080489ac
.L_080488e8:
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r5
	cmp r3, #0
	beq .L_0804896a
	movs r0, #248
	lsls r0, r0, #14
	ands r0, r5
	cmp r0, #0
	beq .L_0804893c
	lsrs r0, r0, #17
	movs r1, #5
	bl Func_0803ccd0
	movs r3, #240
	lsls r3, r3, #4
	ands r3, r5
	lsrs r3, r3, #8
	lsls r0, r3, #2
	adds r0, r0, r3
	movs r3, #255
	ands r5, r3
	ldr r6, [sp, #8]
	ldr r3, .L_08048bf8
	lsls r0, r0, #2
	adds r0, r0, r5
	adds r1, r6, #0
	adds r0, r0, r3
	movs r2, #52
	bl UiText_CopyMessageString
	ldr r1, [sp, #68]
	cmp r1, #0
	beq .L_080489ac
	adds r0, r1, #0
	movs r1, #1
	bl UiWork_Finalize
	movs r2, #0
	str r2, [sp, #68]
	str r2, [sp, #32]
	b .L_080489ac
.L_0804893c:
	add r3, sp, #80
	str r3, [sp, #0]
	ldr r1, [sp, #76]
	adds r2, r5, #0
	ldr r3, [sp, #32]
	ldr r0, [sp, #68]
	bl Func_080464dc
	ldr r6, [sp, #8]
	str r0, [sp, #68]
	adds r1, r6, #0
	ldr r0, .L_08048bfc
	movs r2, #52
	bl UiText_CopyMessageString
	movs r3, #240
	lsls r3, r3, #4
	ands r5, r3
	lsrs r3, r5, #8
	movs r0, #1
	lsls r0, r3
	str r0, [sp, #40]
	b .L_080489ac
.L_0804896a:
	add r3, sp, #80
	adds r2, r5, #0
	str r3, [sp, #0]
	ldr r1, [sp, #76]
	ldr r3, [sp, #32]
	ldr r0, [sp, #68]
	bl Func_080464dc
	movs r3, #240
	lsls r3, r3, #4
	ands r3, r5
	lsrs r3, r3, #8
	str r0, [sp, #68]
	lsls r0, r3, #2
	adds r0, r0, r3
	movs r3, #255
	ands r5, r3
	ldr r6, [sp, #8]
	ldr r3, .L_08048bf8
	lsls r0, r0, #2
	adds r0, r0, r5
	adds r0, r0, r3
	adds r1, r6, #0
	movs r2, #52
	bl UiText_CopyMessageString
	b .L_080489ac
.L_080489a0:
	ldr r6, [sp, #8]
	ldr r0, .L_08048c00
	adds r1, r6, #0
	movs r2, #52
	bl UiText_CopyMessageString
.L_080489ac:
	ldr r2, [sp, #72]
	movs r1, #0
	strb r1, [r2, #6]
	ldr r3, [sp, #32]
	cmp r3, #0
	bne .L_080489de
	movs r3, #1
	strb r3, [r2, #6]
	ldr r0, [sp, #44]
	movs r1, #1
	bl UiWork_Finalize
	movs r3, #42
	str r3, [sp, #0]
	movs r1, #4
	movs r0, #0
	movs r2, #30
	movs r3, #4
	bl UiWindow_Create
	str r0, [sp, #44]
	add r0, sp, #32
	ldrb r0, [r0]
	ldr r1, [sp, #72]
	strb r0, [r1, #6]
.L_080489de:
	ldr r1, [sp, #44]
	movs r2, #0
	adds r0, r6, #0
	movs r3, #4
	bl Func_0803aae4
	ldr r2, [sp, #48]
	mov r1, r10
	str r1, [sp, #64]
	cmp r11, r2
	bne .L_080489fa
	ldr r3, [sp, #20]
	cmp r3, #1
	bne .L_08048aba
.L_080489fa:
	mov r0, r9
	bl RenderOutput_RedrawSavedRect
	ldr r1, [sp, #56]
	mov r0, r11
	lsls r3, r0, #2
	adds r3, r3, r1
	ldr r6, [r3]
	movs r2, #128
	lsls r2, r2, #24
	movs r7, #0
	cmp r6, r2
	beq .L_08048ab0
	mov r8, r3
.L_08048a16:
	movs r3, #240
	lsls r3, r3, #4
	adds r1, r6, #0
	movs r0, #160
	ands r1, r3
	lsls r0, r0, #7
	adds r0, #1
	lsrs r1, r1, #8
	adds r1, r1, r0
	lsls r3, r7, #1
	movs r2, #0
	mov r0, r9
	str r2, [sp, #0]
	bl Func_0803c378
	movs r3, #248
	lsls r3, r3, #14
	ands r3, r6
	cmp r3, #0
	beq .L_08048a46
	movs r0, #4
	bl Func_08041f70
	b .L_08048a56
.L_08048a46:
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r6
	cmp r3, #0
	beq .L_08048a56
	movs r0, #2
	bl Func_08041f70
.L_08048a56:
	movs r0, #240
	lsls r0, r0, #4
	adds r3, r6, #0
	ands r3, r0
	lsrs r3, r3, #8
	lsls r0, r3, #2
	adds r0, r0, r3
	movs r3, #255
	ands r3, r6
	lsls r0, r0, #2
	adds r0, r0, r3
	ldr r3, .L_08048c04
	lsls r5, r7, #4
	adds r0, r0, r3
	mov r1, r9
	movs r2, #8
	adds r3, r5, #0
	bl UiText_DrawCharacterAtOffset
	movs r0, #248
	lsls r0, r0, #14
	ands r0, r6
	cmp r0, #0
	beq .L_08048a94
	lsrs r0, r0, #17
	movs r1, #2
	mov r2, r9
	movs r3, #48
	str r5, [sp, #0]
	bl UiText_DrawNumberAtOffset
.L_08048a94:
	movs r0, #15
	adds r7, #1
	bl Func_08041f70
	cmp r7, #4
	bgt .L_08048ab0
	movs r1, #4
	add r8, r1
	mov r2, r8
	ldr r6, [r2]
	movs r3, #128
	lsls r3, r3, #24
	cmp r6, r3
	bne .L_08048a16
.L_08048ab0:
	mov r12, r11
	mov r0, r12
	str r0, [sp, #48]
	movs r0, #0
	str r0, [sp, #20]
.L_08048aba:
	ldr r1, [sp, #52]
	cmp r1, #5
	ble .L_08048b0c
	movs r7, #0
	adds r1, #4
	mov r8, r1
	b .L_08048afe
.L_08048ac8:
	movs r2, #243
	lsls r2, r2, #8
	adds r2, #1
	mov r0, r11
	movs r1, #5
	adds r6, r7, r2
	bl __divsi3
	cmp r7, r0
	bne .L_08048ae4
	movs r3, #243
	lsls r3, r3, #8
	adds r3, #11
	adds r6, r7, r3
.L_08048ae4:
	mov r0, r9
	ldrh r2, [r0, #8]
	movs r1, #0
	subs r2, r2, r5
	adds r2, r2, r7
	movs r3, #1
	str r1, [sp, #0]
	subs r2, #2
	adds r1, r6, #0
	negs r3, r3
	bl Func_0803c378
	adds r7, #1
.L_08048afe:
	mov r0, r8
	movs r1, #5
	bl __divsi3
	adds r5, r0, #0
	cmp r7, r5
	blt .L_08048ac8
.L_08048b0c:
	mov r3, r9
	movs r2, #14
	ldrsh r1, [r3, r2]
	movs r2, #12
	ldrsh r0, [r3, r2]
	mov r2, r10
	lsls r3, r2, #1
	adds r1, r1, r3
	mov r3, r9
	ldrh r2, [r3, #8]
	movs r3, #14
	adds r0, #1
	adds r1, #1
	str r3, [sp, #0]
	subs r2, #2
	movs r3, #1
	bl Func_08046134
	ldr r0, [sp, #72]
	movs r3, #1
	movs r1, #0
	strb r3, [r0, #3]
	strb r1, [r0, #6]
.L_08048b3a:
	ldr r2, [sp, #52]
	cmp r2, #5
	bgt .L_08048b42
	b .L_08048c58
.L_08048b42:
	movs r7, #0
	adds r2, #4
	mov r8, r2
	b .L_08048ba6
.L_08048b4a:
	movs r3, #243
	lsls r3, r3, #8
	adds r3, #1
	adds r6, r7, r3
	ldr r3, .L_08048c08
	movs r2, #128
	ldr r3, [r3]
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_08048b6c
	ldr r3, .L_08048c0c
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #11
	bhi .L_08048b80
.L_08048b6c:
	mov r0, r11
	movs r1, #5
	bl __divsi3
	cmp r7, r0
	bne .L_08048b80
	movs r0, #243
	lsls r0, r0, #8
	adds r0, #11
	adds r6, r7, r0
.L_08048b80:
	mov r1, r9
	ldrh r5, [r1, #8]
	mov r0, r8
	movs r1, #5
	bl __divsi3
	subs r5, r5, r0
	adds r5, r5, r7
	movs r2, #0
	subs r5, #2
	movs r3, #1
	str r2, [sp, #0]
	mov r0, r9
	adds r1, r6, #0
	adds r2, r5, #0
	negs r3, r3
	bl Func_0803c378
	adds r7, #1
.L_08048ba6:
	mov r0, r8
	movs r1, #5
	bl __divsi3
	cmp r7, r0
	blt .L_08048b4a
	ldr r3, .L_08048c08
	ldr r5, [r3]
	movs r3, #128
	lsls r3, r3, #1
	ands r5, r3
	cmp r5, #0
	bne .L_08048c10
	mov r3, r9
	ldrh r2, [r3, #8]
	movs r1, #243
	subs r2, r2, r0
	lsls r1, r1, #8
	movs r3, #1
	subs r2, #3
	mov r0, r9
	adds r1, #52
	negs r3, r3
	str r5, [sp, #0]
	bl Func_0803c378
	mov r0, r9
	ldrh r2, [r0, #8]
	movs r1, #243
	lsls r1, r1, #8
	movs r3, #1
	subs r2, #2
	adds r1, #53
	negs r3, r3
	str r5, [sp, #0]
	bl Func_0803c378
	b .L_08048c42
	.2byte 0x0000
.L_08048bf4:
	.4byte 0x00000d50
.L_08048bf8:
	.4byte 0x000009b1
.L_08048bfc:
	.4byte 0x00000cf9
.L_08048c00:
	.4byte 0x00000d4e
.L_08048c04:
	.4byte 0x000006d3
.L_08048c08:
	.4byte gInput
.L_08048c0c:
	.4byte Data_0300122c
.L_08048c10:
	mov r1, r9
	ldrh r2, [r1, #8]
	movs r1, #240
	subs r2, r2, r0
	movs r3, #0
	lsls r1, r1, #8
	subs r2, #3
	str r3, [sp, #0]
	mov r0, r9
	adds r1, #17
	subs r3, #1
	bl Func_0803c378
	mov r0, r9
	ldrh r2, [r0, #8]
	movs r1, #0
	str r1, [sp, #0]
	movs r1, #240
	lsls r1, r1, #8
	movs r3, #1
	subs r2, #2
	adds r1, #18
	negs r3, r3
	bl Func_0803c378
.L_08048c42:
	mov r0, r9
	movs r2, #14
	ldrsh r3, [r0, r2]
	ldr r1, [sp, #72]
	subs r3, #1
	lsrs r3, r3, #2
	movs r2, #2
	lsls r2, r3
	ldrb r3, [r1, #3]
	orrs r2, r3
	strb r2, [r1, #3]
.L_08048c58:
	ldr r3, .L_08048f50
	ldr r1, [r3, #4]
	ldr r7, [r3, #12]
	ldr r3, [r3]
	mov r8, r3
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r2, [r3]
	adds r3, r2, #0
	adds r3, #216
	ldr r3, [r3]
	cmp r3, #0
	beq .L_08048c90
	adds r2, #220
	ldr r3, [r2]
	movs r1, #0
	movs r7, #0
	mov r8, r1
	cmp r3, #0
	bne .L_08048c8c
	movs r3, #60
	str r3, [r2]
	movs r7, #1
	movs r1, #1
	b .L_08048c90
.L_08048c8c:
	subs r3, #1
	str r3, [r2]
.L_08048c90:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r2, [r3]
	ldr r3, [r2, #76]
	cmp r3, #0
	beq .L_08048ca6
	movs r3, #2
	ands r3, r1
	cmp r3, #0
	beq .L_08048cb2
.L_08048ca6:
	movs r0, #113
	movs r6, #1
	bl Audio_PlayCue
	negs r6, r6
	b .L_080490c2
.L_08048cb2:
	movs r3, #1
	ands r3, r1
	cmp r3, #0
	beq .L_08048d28
	ldr r3, [sp, #52]
	cmp r3, #0
	beq .L_08048d22
	mov r3, r11
	ldr r1, [sp, #56]
	add r3, r10
	lsls r3, r3, #2
	ldr r0, [r3, r1]
	movs r6, #248
	lsls r6, r6, #14
	adds r5, r0, #0
	ands r5, r6
	cmp r5, #0
	bne .L_08048cec
	ldr r3, [sp, #24]
	cmp r3, #0
	bne .L_08048d1a
	adds r6, r0, #0
	mov r1, r10
	mov r0, r11
	str r0, [r2, #52]
	str r1, [r2, #48]
	ldr r3, [sp, #36]
	str r3, [r2, #56]
	b .L_080490c2
.L_08048cec:
	ldr r0, [sp, #24]
	cmp r0, #0
	bne .L_08048d1a
	ands r5, r6
	bl Func_080396bc
	bl Func_0803cca8
	lsrs r0, r5, #17
	movs r1, #5
	bl Func_0803ccd0
	movs r2, #52
	ldr r1, [sp, #8]
	ldr r0, .L_08048f54
	bl UiText_CopyMessageString
	movs r2, #0
	ldr r0, [sp, #8]
	ldr r1, [sp, #44]
	movs r3, #4
	bl Func_0803aae4
.L_08048d1a:
	movs r0, #114
	bl Audio_PlayCue
	b .L_08048d28
.L_08048d22:
	movs r6, #1
	negs r6, r6
	b .L_080490c2
.L_08048d28:
	ldr r1, [sp, #52]
	cmp r1, #0
	bne .L_08048d30
	b .L_08048f82
.L_08048d30:
	movs r3, #128
	ands r3, r7
	cmp r3, #0
	beq .L_08048d5c
	movs r0, #111
	bl Audio_PlayCue
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #5
	beq .L_08048d52
	ldr r0, [sp, #52]
	mov r3, r11
	add r3, r10
	cmp r3, r0
	bne .L_08048d56
.L_08048d52:
	movs r1, #0
	mov r10, r1
.L_08048d56:
	mov r2, r10
	str r2, [sp, #36]
	b .L_08048f82
.L_08048d5c:
	movs r3, #64
	ands r3, r7
	cmp r3, #0
	beq .L_08048d9c
	movs r0, #111
	bl Audio_PlayCue
	movs r3, #1
	negs r3, r3
	add r10, r3
	mov r0, r10
	cmp r0, #0
	bge .L_08048d96
	ldr r0, [sp, #52]
	movs r1, #5
	subs r0, #1
	bl __divsi3
	lsls r3, r0, #2
	adds r3, r3, r0
	cmp r11, r3
	bne .L_08048d92
	ldr r1, [sp, #52]
	mov r2, r11
	subs r3, r1, r2
	subs r3, #1
	b .L_08048d94
.L_08048d92:
	movs r3, #4
.L_08048d94:
	mov r10, r3
.L_08048d96:
	mov r0, r10
	str r0, [sp, #36]
	b .L_08048f82
.L_08048d9c:
	movs r3, #128
	lsls r3, r3, #1
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	bne .L_08048daa
	b .L_08048ebe
.L_08048daa:
	ldr r2, [sp, #68]
	cmp r2, #0
	beq .L_08048e44
	ldr r0, [sp, #80]
	movs r5, #0
	cmp r5, r0
	bge .L_08048dfe
.L_08048db8:
	movs r3, #243
	lsls r3, r3, #8
	adds r3, #1
	adds r1, r5, r3
	ldr r3, .L_08048f58
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #11
	bhi .L_08048dde
	ldr r3, [sp, #32]
	subs r3, #1
	cmp r5, r3
	bne .L_08048dde
	ldr r2, [sp, #32]
	movs r3, #243
	lsls r3, r3, #8
	adds r3, #10
	adds r1, r2, r3
.L_08048dde:
	ldr r3, [sp, #68]
	ldrh r2, [r3, #8]
	subs r2, r2, r0
	movs r0, #0
	adds r2, r2, r5
	str r0, [sp, #0]
	adds r0, r3, #0
	movs r3, #1
	subs r2, #2
	negs r3, r3
	bl Func_0803c378
	ldr r0, [sp, #80]
	adds r5, #1
	cmp r5, r0
	blt .L_08048db8
.L_08048dfe:
	ldr r1, [sp, #68]
	movs r3, #0
	ldrh r2, [r1, #8]
	str r3, [sp, #0]
	subs r2, r2, r0
	adds r0, r1, #0
	movs r1, #243
	lsls r1, r1, #8
	subs r2, #3
	adds r1, #52
	subs r3, #1
	bl Func_0803c378
	ldr r0, [sp, #68]
	movs r1, #0
	ldrh r2, [r0, #8]
	str r1, [sp, #0]
	movs r1, #243
	lsls r1, r1, #8
	movs r3, #1
	subs r2, #2
	adds r1, #53
	negs r3, r3
	bl Func_0803c378
	ldr r2, [sp, #68]
	ldrh r3, [r2, #14]
	ldr r0, [sp, #72]
	lsls r3, r3, #16
	asrs r3, r3, #18
	movs r2, #2
	lsls r2, r3
	ldrb r3, [r0, #3]
	orrs r2, r3
	strb r2, [r0, #3]
.L_08048e44:
	ldr r1, [sp, #32]
	cmp r1, #0
	bne .L_08048e64
	ldr r0, [sp, #80]
	cmp r0, #0
	beq .L_08048e66
	ldr r2, [sp, #68]
	cmp r2, #0
	beq .L_08048e5c
	adds r0, r2, #0
	bl RenderOutput_ClearList
.L_08048e5c:
	movs r3, #1
	str r3, [sp, #32]
	str r3, [sp, #28]
	b .L_08048f82
.L_08048e64:
	ldr r0, [sp, #80]
.L_08048e66:
	ldr r1, [sp, #32]
	cmp r1, r0
	ble .L_08048e6e
	str r0, [sp, #32]
.L_08048e6e:
	ldr r2, [sp, #32]
	cmp r2, #0
	bne .L_08048e76
	b .L_08048f82
.L_08048e76:
	movs r3, #16
	ands r3, r7
	cmp r3, #0
	beq .L_08048e9c
	movs r0, #111
	bl Audio_PlayCue
	ldr r3, [sp, #32]
	adds r3, #1
	str r3, [sp, #32]
	ldr r0, [sp, #32]
	ldr r3, [sp, #80]
	cmp r0, r3
	ble .L_08048e96
	movs r1, #1
	str r1, [sp, #32]
.L_08048e96:
	movs r2, #1
	str r2, [sp, #28]
	b .L_08048f82
.L_08048e9c:
	movs r3, #32
	ands r3, r7
	cmp r3, #0
	beq .L_08048f82
	movs r0, #111
	bl Audio_PlayCue
	ldr r3, [sp, #32]
	subs r3, #1
	str r3, [sp, #32]
	cmp r3, #0
	bgt .L_08048eb8
	ldr r0, [sp, #80]
	str r0, [sp, #32]
.L_08048eb8:
	movs r1, #1
	str r1, [sp, #28]
	b .L_08048f82
.L_08048ebe:
	ldr r2, [sp, #32]
	cmp r2, #0
	beq .L_08048edc
	ldr r3, [sp, #68]
	cmp r3, #0
	beq .L_08048ed0
	adds r0, r3, #0
	bl RenderOutput_ClearList
.L_08048ed0:
	movs r1, #1
	movs r0, #0
	str r0, [sp, #32]
	str r1, [sp, #28]
	str r1, [sp, #20]
	b .L_08048f82
.L_08048edc:
	movs r3, #16
	ands r3, r7
	cmp r3, #0
	beq .L_08048f2a
	movs r0, #111
	bl Audio_PlayCue
	bl Func_080138a8
	ldr r2, [sp, #52]
	mov r3, r11
	adds r3, #5
	cmp r3, r2
	blt .L_08048f08
	mov r3, r11
	cmp r3, #0
	beq .L_08048f82
	ldr r1, [sp, #36]
	movs r0, #0
	mov r11, r0
	mov r10, r1
	b .L_08048f82
.L_08048f08:
	ldr r0, [sp, #52]
	ldr r2, [sp, #36]
	subs r0, #1
	movs r1, #5
	mov r11, r3
	mov r10, r2
	bl __divsi3
	lsls r3, r0, #2
	adds r3, r3, r0
	cmp r11, r3
	bne .L_08048f82
	ldr r0, [sp, #52]
	mov r1, r11
	subs r3, r0, r1
	ldr r2, [sp, #36]
	b .L_08048f78
.L_08048f2a:
	movs r3, #32
	ands r3, r7
	cmp r3, #0
	beq .L_08048f82
	movs r0, #111
	bl Audio_PlayCue
	bl Func_080138a8
	mov r3, r11
	cmp r3, #0
	beq .L_08048f5c
	ldr r1, [sp, #36]
	movs r0, #5
	negs r0, r0
	add r11, r0
	mov r10, r1
	b .L_08048f82
	.2byte 0x0000
.L_08048f50:
	.4byte gInput
.L_08048f54:
	.4byte 0x00000cf8
.L_08048f58:
	.4byte Data_0300122c
.L_08048f5c:
	ldr r0, [sp, #52]
	movs r1, #5
	subs r0, #1
	bl __divsi3
	ldr r2, [sp, #36]
	lsls r3, r0, #2
	adds r3, r3, r0
	mov r11, r3
	mov r10, r2
	cmp r3, #0
	beq .L_08048f82
	ldr r0, [sp, #52]
	subs r3, r0, r3
.L_08048f78:
	subs r3, #1
	mov r10, r3
	cmp r10, r2
	ble .L_08048f82
	mov r10, r2
.L_08048f82:
	mov r2, r9
	movs r1, #12
	ldrsh r3, [r2, r1]
	movs r1, #14
	ldrsh r2, [r2, r1]
	lsls r3, r3, #3
	subs r3, #2
	mov r0, r10
	str r3, [sp, #12]
	lsls r3, r0, #1
	adds r3, r3, r2
	lsls r3, r3, #3
	ldr r2, [sp, #4]
	adds r3, #20
	str r3, [sp, #16]
	movs r3, #128
	lsls r3, r3, #23
	str r3, [r2, #4]
	movs r3, #0
	str r3, [r2, #8]
	ldr r0, [sp, #60]
	ldr r1, .L_08048ff8
	bl Resource_GetBuffer
	ldr r3, .L_08048fe8
	ldr r1, [sp, #4]
	ands r0, r3
	ldr r2, .L_08048fec
	ldrh r3, [r1, #8]
	ldr r6, .L_08048ffc
	ands r3, r2
	orrs r3, r0
	ldr r0, [r6]
	adds r2, r1, #0
	strh r3, [r2, #8]
	movs r5, #4
	ldr r3, [sp, #12]
	ands r0, r5
	movs r1, #255
	lsrs r2, r0, #1
	lsls r1, r1, #8
	adds r2, r3, r2
	adds r1, #250
	adds r2, r2, r1
	ldr r3, .L_08048ff0
	ldr r1, [sp, #4]
	ands r2, r3
	ldrh r3, [r1, #6]
	ldr r1, .L_08048ff4
	b .L_08049000
	.2byte 0x0000
.L_08048fe8:
	.4byte 0x000003ff
.L_08048fec:
	.4byte 0xfffffc00
.L_08048ff0:
	.4byte 0x000001ff
.L_08048ff4:
	.4byte 0xfffffe00
.L_08048ff8:
	.4byte Data_080597f8
.L_08048ffc:
	.4byte Data_0300122c
.L_08049000:
	lsrs r0, r0, #2
	ands r3, r1
	orrs r3, r2
	ldr r2, [sp, #4]
	strh r3, [r2, #6]
	ldr r3, [sp, #16]
	subs r0, r3, r0
	adds r0, #248
	strb r0, [r2, #4]
	ldr r0, [sp, #52]
	cmp r0, #0
	beq .L_08049020
	ldr r0, [sp, #4]
	movs r1, #242
	bl Func_08014128
.L_08049020:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #64]
	ldr r6, [r6]
	ldrh r2, [r3, #12]
	ldr r7, [r3]
	movs r3, #2
	ands r3, r2
	ands r6, r5
	cmp r3, #0
	beq .L_0804907a
	movs r5, #0
.L_08049038:
	negs r3, r6
	orrs r3, r6
	lsrs r3, r3, #31
	adds r2, r3, #0
	ldr r1, [sp, #40]
	movs r3, #15
	subs r2, r3, r2
	movs r3, #1
	lsls r3, r5
	ands r3, r1
	cmp r3, #0
	bne .L_08049052
	movs r2, #15
.L_08049052:
	movs r3, #12
	ldrsh r0, [r7, r3]
	ldr r3, .L_08049130
	ldrb r3, [r3, r5]
	adds r0, r0, r3
	movs r3, #14
	ldrsh r1, [r7, r3]
	ldr r3, .L_08049134
	adds r0, #1
	ldrb r3, [r3, r5]
	str r2, [sp, #0]
	adds r1, r1, r3
	adds r1, #1
	movs r2, #2
	movs r3, #1
	adds r5, #1
	bl Func_08046134
	cmp r5, #3
	ble .L_08049038
.L_0804907a:
	ldr r3, .L_08049138
	movs r2, #4
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_080490a0
	ldr r5, .L_0804913c
	movs r2, #32
	adds r1, r5, #0
	ldr r6, .L_08049140
	ldr r0, .L_08049144
	mov lr, r6
	.2byte 0xf800
	ldr r0, .L_08049148
	adds r1, r5, #0
	movs r2, #32
	mov lr, r6
	.2byte 0xf800
	b .L_080490b8
.L_080490a0:
	ldr r3, .L_0804914c
	movs r1, #32
	ldr r2, .L_08049150
	ldr r0, .L_08049144
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_08049140
	ldr r0, .L_08049148
	ldr r1, .L_0804913c
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
.L_080490b8:
	movs r0, #1
	bl WaitFrames
	bl .L_08048808
.L_080490c2:
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #64]
	movs r3, #2
	ldrh r2, [r1, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_080490ec
	ldr r3, [r1]
	movs r1, #12
	ldrsh r0, [r3, r1]
	movs r2, #14
	ldrsh r1, [r3, r2]
	movs r3, #15
	str r3, [sp, #0]
	adds r0, #1
	adds r1, #1
	movs r2, #4
	movs r3, #4
	bl Func_08046134
.L_080490ec:
	ldr r0, [sp, #60]
	bl Func_08014274
	movs r1, #1
	ldr r0, [sp, #44]
	bl UiWork_Finalize
	movs r1, #1
	ldr r0, [sp, #68]
	bl UiWork_Finalize
	movs r1, #1
	mov r0, r9
	bl UiWork_Finalize
	bl Func_08041b68
	movs r0, #0
	bl Func_08041c0c
	ldr r0, [sp, #56]
	bl Sys_Free
	movs r0, #1
	bl WaitFrames
	adds r0, r6, #0
	add sp, #224
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08049130:
	.4byte Data_0805f897
.L_08049134:
	.4byte Data_0805f89b
.L_08049138:
	.4byte Data_0300122c
.L_0804913c:
	.4byte Data_0805f7b8
.L_08049140:
	.4byte IwramCopyWords
.L_08049144:
	.4byte 0x06006500
.L_08049148:
	.4byte 0x06006520
.L_0804914c:
	.4byte IwramFillWords
.L_08049150:
	.4byte 0x44444444
