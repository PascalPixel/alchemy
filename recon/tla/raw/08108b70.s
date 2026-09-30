.syntax unified
	.thumb
	.global Func_08108b70
	.thumb_func
Func_08108b70:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #40
	movs r1, #0
	movs r2, #0
	str r1, [sp, #32]
	str r1, [sp, #20]
	mov r10, r3
	str r2, [r3, #36]
	movs r1, #7
	movs r2, #12
	movs r5, #2
	movs r3, #4
	movs r0, #18
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	mov r3, r10
	str r0, [r3, #12]
	bl Func_08109188
	movs r0, #0
	movs r1, #8
	movs r2, #15
	movs r3, #4
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	str r0, [sp, #32]
.L_08108bbc:
	ldr r1, [sp, #20]
	movs r5, #2
	str r1, [sp, #36]
	movs r2, #30
	movs r1, #12
	movs r3, #4
	movs r0, #0
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	movs r3, #128
	str r0, [sp, #28]
	lsls r3, r3, #3
	adds r3, #220
	add r3, r10
	ldr r2, [r3]
	movs r3, #18
	strb r3, [r2, #5]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #5
	add r2, r10
	movs r3, #12
	strb r3, [r2]
	movs r0, #0
	movs r2, #30
	movs r3, #3
	movs r1, #17
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	movs r3, #156
	lsls r3, r3, #2
	add r3, r10
	str r0, [sp, #24]
	str r3, [sp, #12]
	movs r2, #1
	mov r9, r2
.L_08108c08:
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #2
	add r3, r10
	movs r2, #0
	ldrsh r1, [r3, r2]
	mov r3, r9
	mov r8, r1
	cmp r3, #0
	beq .L_08108c76
	ldr r3, [sp, #36]
	ldr r1, [sp, #12]
	lsls r3, r3, #1
	ldrsh r5, [r3, r1]
	adds r0, r5, #0
	bl Item_Get
	movs r1, #7
	adds r6, r0, #0
	ldr r0, [sp, #36]
	bl Math_Mod
	adds r1, r0, #0
	lsls r1, r1, #5
	ldr r0, [sp, #28]
	subs r1, #8
	movs r2, #8
	bl Func_08108af0
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #5
	add r2, r10
	movs r3, #4
	strb r3, [r2]
	ldr r0, [sp, #28]
	ldr r1, [sp, #36]
	bl Func_08109068
	ldr r1, .L_08108f68
	ldr r0, [sp, #24]
	adds r1, r5, r1
	bl Func_08109270
	ldr r0, [sp, #32]
	bl RenderOutput_RedrawSavedRectFar
	movs r3, #0
	ldrh r2, [r6]
	ldr r0, [sp, #32]
	adds r1, r5, #0
	bl Func_081091cc
	movs r3, #0
	mov r9, r3
.L_08108c76:
	ldr r7, .L_08108f6c
	movs r3, #1
	ldr r2, [r7, #4]
	ands r2, r3
	cmp r2, #0
	beq .L_08108c84
	b .L_0810902a
.L_08108c84:
	ldr r3, [r7, #4]
	movs r6, #2
	ands r3, r6
	cmp r3, #0
	beq .L_08108c90
	b .L_0810901e
.L_08108c90:
	ldr r3, [r7]
	movs r1, #4
	ands r3, r1
	mov r11, r1
	cmp r3, #0
	beq .L_08108d18
	ldr r3, [sp, #36]
	ldr r2, [sp, #12]
	lsls r3, r3, #1
	ldrsh r2, [r3, r2]
	mov r8, r2
	mov r0, r8
	bl PartyInventory_CountItemFar
	adds r5, r0, #0
	mov r0, r8
	bl Item_Get
	str r0, [sp, #8]
	movs r0, #126
	bl Audio_PlayCue
	movs r3, #10
	movs r1, #3
	movs r2, #17
	movs r0, #13
	str r6, [sp, #0]
	bl UiWindow_CreateFar
	adds r2, r5, #0
	mov r1, r8
	adds r6, r0, #0
	bl Func_080f8038 + 0x8
	ldr r3, [r7]
	mov r2, r11
	ands r3, r2
	cmp r3, #0
	beq .L_08108cee
	movs r5, #4
.L_08108ce0:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7]
	ands r3, r5
	cmp r3, #0
	bne .L_08108ce0
.L_08108cee:
	adds r0, r6, #0
	movs r1, #2
	bl UiWork_FinalizeFar
	ldr r0, [sp, #28]
	ldr r1, [sp, #36]
	bl Func_08109068
	bl Func_08109188
	ldr r0, [sp, #32]
	bl RenderOutput_RedrawSavedRectFar
	ldr r3, [sp, #8]
	ldr r0, [sp, #32]
	ldrh r2, [r3]
	mov r1, r8
	movs r3, #0
	bl Func_081091cc
	b .L_08108d24
.L_08108d18:
	add r0, sp, #36
	mov r1, r8
	movs r2, #7
	bl Func_08108690
	mov r9, r0
.L_08108d24:
	movs r0, #1
	bl WaitFrames
	b .L_08108c08
.L_08108d2c:
	ldr r0, [sp, #24]
	movs r1, #2
	bl UiWork_FinalizeFar
	movs r1, #2
	ldr r0, [sp, #28]
	bl UiWork_FinalizeFar
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	beq .L_08108d48
	b .L_08109038
.L_08108d48:
	ldr r1, [sp, #20]
	movs r2, #156
	lsls r2, r2, #2
	lsls r3, r1, #1
	adds r3, r3, r2
	mov r1, r10
	movs r5, #128
	ldrh r3, [r1, r3]
	lsls r5, r5, #3
	adds r5, #252
	add r5, r10
	strh r3, [r5]
	ldr r0, .L_08108f70
	bl Func_081084f4
	ldrh r0, [r5]
	bl Item_Get
	movs r2, #1
	str r0, [sp, #4]
	str r2, [sp, #16]
	movs r6, #0
	movs r5, #2
	movs r1, #14
	movs r2, #13
	movs r3, #3
	movs r0, #0
	str r6, [sp, #36]
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #220
	add r3, r10
	ldr r2, [r3]
	movs r3, #4
	strb r3, [r2, #5]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #5
	movs r3, #12
	add r2, r10
	strb r3, [r2]
	movs r1, #2
	movs r2, #0
	adds r7, r0, #0
	bl Func_080f8058 + 0x8
	movs r3, #9
	movs r0, #16
	movs r1, #11
	movs r2, #14
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	movs r3, #2
	mov r11, r6
	mov r8, r0
	mov r9, r3
.L_08108dc0:
	mov r1, r11
	cmp r1, #0
	beq .L_08108dd4
	movs r2, #0
	ldr r0, .L_08108f70
	mov r11, r2
	bl Func_081084f4
	movs r3, #1
	mov r9, r3
.L_08108dd4:
	mov r1, r9
	cmp r1, #0
	beq .L_08108e62
	ldr r4, [sp, #36]
	movs r3, #153
	lsls r3, r3, #3
	lsls r2, r4, #1
	adds r2, r2, r3
	mov r3, r10
	adds r3, #2
	ldrsh r6, [r3, r2]
	adds r3, r4, #0
	cmp r4, #0
	bge .L_08108df2
	adds r3, r4, #3
.L_08108df2:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r4, r3
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #3
	movs r2, #0
	subs r1, #12
	adds r0, r7, #0
	bl Func_08108af0
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #5
	add r2, r10
	movs r3, #3
	strb r3, [r2]
	mov r2, r9
	cmp r2, #2
	bne .L_08108e30
	ldr r0, [sp, #36]
	cmp r0, #0
	bge .L_08108e22
	adds r0, #3
.L_08108e22:
	asrs r0, r0, #2
	lsls r0, r0, #2
	bl Func_080f8058
	movs r0, #1
	bl WaitFrames
.L_08108e30:
	movs r5, #128
	lsls r5, r5, #3
	adds r5, #252
	add r5, r10
	adds r0, r7, #0
	ldr r1, [sp, #36]
	ldrh r2, [r5]
	bl Func_0810928c
	ldrh r0, [r5]
	bl Func_080ad1d8 + 0x8
	cmp r0, #0
	bne .L_08108e58
	ldrh r2, [r5]
	mov r0, r8
	adds r1, r6, #0
	bl Func_081095b0
	b .L_08108e62
.L_08108e58:
	ldrh r2, [r5]
	mov r0, r8
	adds r1, r6, #0
	bl Func_081093a4
.L_08108e62:
	ldr r1, .L_08108f6c
	movs r3, #1
	ldr r2, [r1, #4]
	ands r2, r3
	cmp r2, #0
	beq .L_08108f3e
	movs r5, #128
	lsls r5, r5, #3
	adds r5, #252
	movs r3, #2
	adds r0, r7, #0
	add r5, r10
	mov r9, r3
	bl RenderOutput_PrepareForRedrawFar
	ldrh r1, [r5]
	adds r0, r6, #0
	bl Inventory_AddItemFar
	adds r1, r0, #0
	cmp r1, #0
	bge .L_08108ebe
	movs r0, #113
	bl Audio_PlayCue
	adds r0, r6, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldrh r0, [r5]
	movs r1, #2
	bl UiText_DrawQuantity
	adds r0, r6, #0
	bl Item_AdjustCounterFar + 0x8
	cmp r0, #15
	bne .L_08108eb6
	ldr r0, .L_08108f74
	bl Func_081084f4
	b .L_08108dc0
.L_08108eb6:
	ldr r0, .L_08108f78
	bl Func_081084f4
	b .L_08108dc0
.L_08108ebe:
	adds r0, r6, #0
	bl Inventory_RemoveFar
	ldr r2, .L_08108f7c
	ldr r1, [sp, #4]
	ldr r2, [r2, #16]
	ldrh r3, [r1]
	cmp r3, r2
	bls .L_08108ed2
	b .L_0810900c
.L_08108ed2:
	ldrh r1, [r5]
	adds r0, r6, #0
	bl Djinn_IsActiveFar + 0x18
	cmp r0, #0
	bne .L_08108efc
	movs r1, #1
	adds r0, r6, #0
	bl UiText_DrawQuantity
	ldr r0, .L_08108f80
	bl Func_081084f4
	movs r0, #0
	bl Func_08108630
	movs r2, #1
	mov r11, r2
	cmp r0, #0
	beq .L_08108efc
	b .L_08108dc0
.L_08108efc:
	movs r5, #128
	lsls r5, r5, #3
	movs r0, #112
	adds r5, #252
	bl Audio_PlayCue
	add r5, r10
	movs r0, #1
	bl WaitFrames
	ldrh r1, [r5]
	adds r0, r6, #0
	bl Func_08109624
	movs r1, #1
	movs r3, #1
	negs r1, r1
	str r0, [sp, #16]
	mov r11, r3
	cmp r0, r1
	bne .L_08108f28
	b .L_08108dc0
.L_08108f28:
	ldrh r1, [r5]
	adds r0, r6, #0
	ldr r2, [sp, #16]
	bl Func_081098c0
	adds r0, r7, #0
	mov r1, r8
	bl Func_0810a490
	movs r5, #0
	b .L_08108f84
.L_08108f3e:
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_08109000
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #4
	add r3, r10
	movs r1, #0
	ldrsb r1, [r3, r1]
	add r0, sp, #36
	movs r2, #4
	bl Func_08108690
	mov r9, r0
	movs r0, #1
	bl WaitFrames
	b .L_08108dc0
	.2byte 0x0000
.L_08108f68:
	.4byte 0x00000092
.L_08108f6c:
	.4byte gInput
.L_08108f70:
	.4byte 0x0000124e
.L_08108f74:
	.4byte 0x0000124f
.L_08108f78:
	.4byte 0x00001257
.L_08108f7c:
	.4byte gPartyState
.L_08108f80:
	.4byte 0x00001250
.L_08108f84:
	movs r0, #0
	bl Func_0810bea8
	bl Func_080f8058 + 0x10
	mov r0, r8
	movs r1, #2
	bl UiWork_FinalizeFar
	adds r0, r7, #0
	movs r1, #2
	bl UiWork_FinalizeFar
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bne .L_08108ff8
	movs r3, #129
	lsls r3, r3, #3
	adds r3, #255
	add r3, r10
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_08108ff8
	ldr r2, [sp, #16]
	cmp r5, r2
	bge .L_08108fda
	movs r6, #128
	lsls r6, r6, #3
	adds r6, #252
	add r6, r10
	adds r5, r2, #0
.L_08108fca:
	movs r1, #1
	ldrh r0, [r6]
	negs r1, r1
	subs r5, #1
	bl Item_AdjustCounterFar
	cmp r5, #0
	bne .L_08108fca
.L_08108fda:
	bl Func_081080a8
	cmp r0, #0
	beq .L_08109038
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #2
	add r3, r10
	movs r1, #0
	ldrsh r3, [r3, r1]
	ldr r2, [sp, #20]
	subs r3, #1
	cmp r2, r3
	ble .L_08108ff8
	str r3, [sp, #20]
.L_08108ff8:
	ldr r0, .L_08109060
	bl Func_081084f4
	b .L_08108bbc
.L_08109000:
	movs r0, #113
	movs r5, #1
	bl Audio_PlayCue
	negs r5, r5
	b .L_08108f84
.L_0810900c:
	movs r0, #113
	bl Audio_PlayCue
	movs r5, #1
	ldr r0, .L_08109064
	bl Func_0810857c
	negs r5, r5
	b .L_08108f84
.L_0810901e:
	movs r0, #113
	movs r5, #1
	bl Audio_PlayCue
	negs r5, r5
	b .L_08108d2c
.L_0810902a:
	ldr r3, [sp, #36]
	movs r0, #112
	str r3, [sp, #20]
	movs r5, #0
	bl Audio_PlayCue
	b .L_08108d2c
.L_08109038:
	ldr r0, [sp, #32]
	movs r1, #2
	bl UiWork_FinalizeFar
	mov r1, r10
	ldr r0, [r1, #12]
	movs r1, #2
	bl UiWork_FinalizeFar
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08109060:
	.4byte 0x00001259
.L_08109064:
	.4byte 0x0000124d
