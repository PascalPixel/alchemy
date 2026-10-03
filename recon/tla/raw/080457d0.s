.syntax unified
	.thumb
	.global Func_080457d0
	.thumb_func
Func_080457d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #60
	str r0, [sp, #52]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #60]
	mov r8, r1
	str r0, [sp, #48]
	movs r0, #128
	lsls r0, r0, #3
	bl Resource_LoadIntoFreeSlot
	movs r2, #0
	movs r1, #1
	str r2, [sp, #40]
	str r1, [sp, #44]
	adds r7, r0, #0
	movs r0, #240
	adds r5, #228
	lsls r0, r0, #1
	ldr r5, [r5]
	bl Runtime_BumpAllocateAlternatePool
	mov r9, r0
	mov r3, r9
	movs r2, #255
	adds r3, #255
	mov r12, r9
.L_08045814:
	strb r2, [r3]
	subs r3, #1
	cmp r3, r12
	bge .L_08045814
	movs r2, #128
	lsls r2, r2, #1
	movs r3, #1
	add r2, r9
	str r3, [r2]
	ldr r3, [sp, #52]
	cmp r3, #0
	bne .L_0804583a
	movs r3, #6
	str r3, [sp, #0]
	movs r0, #20
	movs r3, #3
	movs r1, #17
	movs r2, #10
	b .L_08045846
.L_0804583a:
	movs r3, #6
	str r3, [sp, #0]
	movs r0, #22
	movs r3, #3
	movs r1, #17
	movs r2, #8
.L_08045846:
	bl UiWindow_Create
	movs r3, #214
	lsls r3, r3, #1
	add r3, r9
	str r0, [r3]
	movs r3, #226
	lsls r3, r3, #1
	movs r1, #1
	add r3, r9
	negs r1, r1
	str r1, [r3]
	ldr r6, [sp, #52]
	cmp r6, #0
	bne .L_0804592c
	movs r2, #228
	movs r3, #14
	lsls r2, r2, #1
	mov r0, r9
	str r3, [r0, r2]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r6, #1
	adds r3, #68
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0804589c
	subs r2, #196
	adds r3, r5, r2
	ldr r3, [r3]
	cmp r3, r1
	bne .L_0804589c
	bl Party_CountActiveOwnersFar
	cmp r0, #4
	ble .L_0804589c
	movs r2, #230
	lsls r2, r2, #1
	movs r3, #49
	mov r5, r9
	str r3, [r5, r2]
	movs r6, #2
.L_0804589c:
	mov r0, r8
	cmp r0, #0
	ble .L_080458c2
	ldr r3, .L_08045928
	movs r1, #166
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r3, r1
	ldrb r3, [r3]
	cmp r3, #2
	beq .L_080458c2
	movs r2, #228
	lsls r3, r6, #2
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r5, r9
	movs r2, #4
	str r2, [r5, r3]
	adds r6, #1
.L_080458c2:
	movs r1, #228
	lsls r3, r6, #2
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #7
	mov r0, r9
	adds r6, #1
	str r2, [r0, r3]
	lsls r3, r6, #2
	adds r3, r3, r1
	subs r2, #8
	str r2, [r0, r3]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r3, [r3]
	movs r2, #224
	ldr r3, [r3, #60]
	lsls r2, r2, #1
	add r2, r9
	str r3, [r2]
	b .L_080459b4
.L_080458ee:
	movs r1, #2
	negs r1, r1
	mov r10, r1
	b .L_08045ff8
.L_080458f6:
	movs r3, #224
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	movs r2, #228
	lsls r3, r3, #2
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r5, r9
	ldr r3, [r5, r3]
	mov r10, r3
	b .L_08045ff8
.L_0804590e:
	movs r6, #1
	movs r0, #113
	negs r6, r6
	bl Audio_PlayCue
	mov r10, r6
	b .L_08045ff8
.L_0804591c:
	movs r3, #228
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	mov r10, r3
	b .L_08045ff8
.L_08045928:
	.4byte gPartyState
.L_0804592c:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r3, [r3]
	movs r2, #224
	ldr r3, [r3, #64]
	lsls r2, r2, #1
	add r2, r9
	str r3, [r2]
	movs r3, #228
	mov r0, r9
	movs r5, #0
	lsls r3, r3, #1
	str r5, [r0, r3]
	adds r3, #4
	movs r5, #1
	str r5, [r0, r3]
	movs r0, #0
	bl Trade_GetOfferStateFar
	ldr r3, [r0]
	movs r5, #2
	cmp r3, #0
	beq .L_08045992
	add r5, sp, #56
	movs r0, #0
	adds r1, r5, #0
	bl Func_08118130
	mov r2, sp
	adds r2, #59
.L_0804596a:
	ldrb r3, [r5]
	ldr r1, [sp, #40]
	adds r5, #1
	adds r1, r1, r3
	str r1, [sp, #40]
	cmp r5, r2
	ble .L_0804596a
	movs r2, #232
	mov r5, r9
	lsls r2, r2, #1
	movs r3, #15
	str r3, [r5, r2]
	movs r5, #3
	cmp r1, #0
	beq .L_08045992
	adds r2, #4
	movs r3, #16
	mov r6, r9
	str r3, [r6, r2]
	movs r5, #4
.L_08045992:
	movs r1, #228
	lsls r1, r1, #1
	lsls r3, r5, #2
	mov r0, r9
	adds r3, r3, r1
	movs r2, #2
	adds r5, #1
	str r2, [r0, r3]
	lsls r3, r5, #2
	adds r3, r3, r1
	movs r2, #3
	adds r5, #1
	str r2, [r0, r3]
	lsls r3, r5, #2
	adds r3, r3, r1
	subs r2, #4
	str r2, [r0, r3]
.L_080459b4:
	movs r5, #136
	lsls r5, r5, #1
	add r5, r9
	strh r7, [r5]
	movs r6, #228
	lsls r6, r6, #1
	add r6, r9
	ldr r2, [r6]
	movs r5, #150
	movs r6, #230
	movs r3, #1
	movs r1, #0
	lsls r5, r5, #1
	lsls r6, r6, #1
	negs r3, r3
	mov r8, r1
	add r5, r9
	add r6, r9
	cmp r2, r3
	beq .L_080459fc
.L_080459dc:
	mov r1, r8
	mov r0, r9
	bl Func_080456f8
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #5
	bgt .L_080459fc
	strh r7, [r5]
	movs r3, #1
	ldmia r6!, {r2}
	negs r3, r3
	adds r5, #28
	cmp r2, r3
	bne .L_080459dc
.L_080459fc:
	movs r0, #216
	movs r2, #218
	lsls r0, r0, #1
	lsls r2, r2, #1
	movs r3, #160
	add r0, r9
	mov r5, r8
	add r2, r9
	lsls r3, r3, #1
	str r5, [r0]
	strh r3, [r2]
	movs r2, #219
	lsls r2, r2, #1
	subs r3, #16
	add r2, r9
	strh r3, [r2]
	adds r3, #136
	movs r1, #0
	add r3, r9
	strh r1, [r3]
	mov r8, r1
	ldr r3, [r0]
	cmp r8, r3
	bge .L_08045a6a
	movs r1, #140
	adds r4, r0, #0
	lsls r1, r1, #1
	movs r0, #138
	lsls r0, r0, #1
	movs r5, #136
	add r1, r9
.L_08045a3a:
	ldr r3, [r4]
	mov r6, r8
	subs r3, r6, r3
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #3
	adds r3, r2, #0
	adds r3, #155
	mov r7, r9
	str r3, [r0, r7]
	ldr r3, [sp, #52]
	cmp r3, #0
	beq .L_08045a5a
	adds r3, r2, #0
	adds r3, #171
	str r3, [r0, r7]
.L_08045a5a:
	str r5, [r1]
	movs r6, #1
	ldr r3, [r4]
	add r8, r6
	adds r1, #28
	adds r0, #28
	cmp r8, r3
	blt .L_08045a3a
.L_08045a6a:
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_08045afc
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_08045b00
	movs r0, #2
	movs r1, #136
	bl Runtime_SetIrqHandler
	movs r0, #216
	movs r2, #224
	lsls r0, r0, #1
	lsls r2, r2, #1
	add r0, r9
	add r2, r9
	adds r7, r0, #0
	adds r1, r2, #0
	str r7, [sp, #16]
	str r1, [sp, #24]
.L_08045a92:
	ldr r3, .L_08045b04
	ldr r1, .L_08045b08
	ldr r3, [r3]
	movs r2, #31
	lsls r3, r3, #1
	ands r3, r2
	lsls r3, r3, #1
	ldrh r1, [r1, r3]
	ldr r5, .L_08045b0c
	str r1, [sp, #36]
	adds r3, r1, r5
	cmp r3, #0
	bge .L_08045ab0
	adds r3, r1, #0
	subs r3, #253
.L_08045ab0:
	movs r6, #152
	asrs r3, r3, #2
	lsls r6, r6, #1
	adds r6, r3, r6
	str r6, [sp, #36]
	add r7, sp, #36
	ldrh r7, [r7]
	movs r3, #218
	lsls r3, r3, #1
	add r3, r9
	strh r7, [r3]
	movs r3, #219
	ldr r2, .L_08045b10
	lsls r3, r3, #1
	ldr r1, .L_08045af8
	add r3, r9
	adds r0, r7, #0
	strh r0, [r3]
	movs r3, #32
	strh r1, [r2, #4]
	strh r3, [r2, #6]
	ldr r2, [sp, #44]
	cmp r2, #0
	bne .L_08045ae2
	b .L_08045d24
.L_08045ae2:
	ldr r5, [sp, #48]
	movs r3, #0
	str r3, [sp, #44]
	movs r3, #1
	strb r3, [r5, #6]
	ldr r6, [sp, #52]
	cmp r6, #0
	bne .L_08045af4
	b .L_08045c1c
.L_08045af4:
	b .L_08045b14
	.2byte 0x0000
.L_08045af8:
	.4byte 0x00000000
.L_08045afc:
	.4byte Graphics_SetBg1Priority3
.L_08045b00:
	.4byte Func_08045780
.L_08045b04:
	.4byte gFrameCount
.L_08045b08:
	.4byte Data_0805e9c4
.L_08045b0c:
	.4byte 0xffffff00
.L_08045b10:
	.4byte Data_03001120
.L_08045b14:
	ldr r0, [sp, #16]
	movs r7, #0
	ldr r3, [r0]
	movs r2, #6
	mov lr, r7
	subs r3, r2, r3
	cmp lr, r3
	bge .L_08045b7a
	movs r3, #216
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	ldr r6, .L_08045b4c
	subs r2, r2, r3
	movs r7, #3
	mov r12, r2
	movs r4, #0
.L_08045b36:
	mov r2, lr
	ldr r5, [sp, #48]
	adds r3, r4, r2
	movs r1, #0
	lsls r3, r3, #1
	mov r8, r1
	adds r0, r3, r5
.L_08045b44:
	movs r2, #0
	adds r1, r0, #0
	b .L_08045b50
	.2byte 0x0000
.L_08045b4c:
	.4byte 0x0000f07f
.L_08045b50:
	adds r3, r2, #0
	ands r3, r7
	lsls r3, r3, #1
	movs r5, #138
	adds r3, r3, r1
	lsls r5, r5, #3
	adds r3, r3, r5
	adds r2, #1
	strh r6, [r3]
	cmp r2, #2
	ble .L_08045b50
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r0, #64
	cmp r2, #2
	ble .L_08045b44
	add lr, r1
	adds r4, #2
	cmp lr, r12
	blt .L_08045b36
.L_08045b7a:
	ldr r5, [sp, #16]
	movs r3, #0
	mov lr, r3
	ldr r3, [r5]
	cmp lr, r3
	blt .L_08045b88
	b .L_08045d24
.L_08045b88:
	movs r3, #216
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	movs r7, #0
	mov r11, r3
	lsls r3, r3, #1
	add r3, r11
	mov r10, r3
	mov r6, r10
	lsls r6, r6, #1
	str r6, [sp, #32]
	str r7, [sp, #8]
.L_08045ba2:
	ldr r2, [sp, #8]
	ldr r1, [sp, #48]
	add r2, lr
	lsls r3, r2, #1
	movs r0, #0
	adds r6, r2, #0
	mov r2, lr
	mov r8, r0
	adds r4, r3, r1
	lsls r5, r2, #4
.L_08045bb6:
	str r5, [sp, #4]
	movs r0, #0
	mov r12, r6
.L_08045bbc:
	adds r1, r0, #0
	movs r3, #3
	ands r1, r3
	mov r7, r12
	adds r2, r7, r1
	mov r3, r10
	ldr r7, .L_08045c18
	subs r2, r2, r3
	lsls r2, r2, #1
	adds r2, r2, r7
	ldr r7, [sp, #4]
	lsls r1, r1, #1
	adds r3, r7, r0
	movs r7, #128
	lsls r7, r7, #1
	adds r3, r3, r7
	strh r3, [r2]
	ldr r2, [sp, #32]
	movs r3, #128
	adds r1, r4, r1
	lsls r3, r3, #3
	ldr r7, .L_08045c14
	subs r1, r1, r2
	adds r3, #116
	adds r1, r1, r3
	adds r0, #1
	strh r7, [r1]
	cmp r0, #2
	ble .L_08045bbc
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r4, #64
	adds r6, #32
	adds r5, #4
	cmp r1, #2
	ble .L_08045bb6
	ldr r2, [sp, #8]
	add lr, r0
	adds r2, #2
	str r2, [sp, #8]
	cmp lr, r11
	blt .L_08045ba2
	b .L_08045d24
.L_08045c14:
	.4byte 0x00000000
.L_08045c18:
	.4byte 0x0600fd6c
.L_08045c1c:
	ldr r5, [sp, #16]
	movs r3, #0
	mov lr, r3
	ldr r3, [r5]
	movs r2, #6
	subs r3, r2, r3
	cmp lr, r3
	bge .L_08045c84
	movs r3, #216
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	ldr r6, .L_08045c54
	subs r2, r2, r3
	movs r7, #3
	mov r12, r2
	movs r4, #0
.L_08045c3e:
	mov r1, lr
	ldr r2, [sp, #48]
	adds r3, r4, r1
	movs r0, #0
	lsls r3, r3, #1
	mov r8, r0
	adds r0, r3, r2
.L_08045c4c:
	movs r2, #0
	adds r1, r0, #0
	b .L_08045c58
	.2byte 0x0000
.L_08045c54:
	.4byte 0x0000f07f
.L_08045c58:
	adds r3, r2, #0
	ands r3, r7
	movs r5, #128
	lsls r3, r3, #1
	lsls r5, r5, #3
	adds r3, r3, r1
	adds r5, #76
	adds r3, r3, r5
	adds r2, #1
	strh r6, [r3]
	cmp r2, #2
	ble .L_08045c58
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r0, #64
	cmp r2, #2
	ble .L_08045c4c
	add lr, r1
	adds r4, #2
	cmp lr, r12
	blt .L_08045c3e
.L_08045c84:
	ldr r5, [sp, #16]
	movs r3, #0
	mov lr, r3
	ldr r3, [r5]
	cmp lr, r3
	bge .L_08045d24
	movs r3, #216
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	movs r7, #0
	mov r11, r3
	lsls r3, r3, #1
	add r3, r11
	mov r10, r3
	mov r6, r10
	lsls r6, r6, #1
	str r6, [sp, #28]
	str r7, [sp, #12]
.L_08045caa:
	ldr r2, [sp, #12]
	ldr r1, [sp, #48]
	add r2, lr
	lsls r3, r2, #1
	movs r0, #0
	adds r6, r2, #0
	mov r2, lr
	mov r8, r0
	adds r4, r3, r1
	lsls r5, r2, #4
.L_08045cbe:
	str r5, [sp, #4]
	movs r0, #0
	mov r12, r6
.L_08045cc4:
	adds r1, r0, #0
	movs r3, #3
	ands r1, r3
	mov r7, r12
	adds r2, r7, r1
	mov r3, r10
	ldr r7, .L_08045d20
	subs r2, r2, r3
	lsls r2, r2, #1
	adds r2, r2, r7
	ldr r7, [sp, #4]
	lsls r1, r1, #1
	adds r3, r7, r0
	movs r7, #128
	lsls r7, r7, #1
	adds r3, r3, r7
	strh r3, [r2]
	ldr r2, [sp, #28]
	adds r1, r4, r1
	movs r3, #142
	ldr r7, .L_08045d1c
	subs r1, r1, r2
	lsls r3, r3, #3
	adds r1, r1, r3
	adds r0, #1
	strh r7, [r1]
	cmp r0, #2
	ble .L_08045cc4
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r4, #64
	adds r6, #32
	adds r5, #4
	cmp r1, #2
	ble .L_08045cbe
	ldr r2, [sp, #12]
	add lr, r0
	adds r2, #2
	str r2, [sp, #12]
	cmp lr, r11
	blt .L_08045caa
	b .L_08045d24
	.2byte 0x0000
.L_08045d1c:
	.4byte 0x00000000
.L_08045d20:
	.4byte 0x0600fd68
.L_08045d24:
	movs r7, #226
	movs r6, #224
	lsls r7, r7, #1
	lsls r6, r6, #1
	add r7, r9
	add r6, r9
	ldr r2, [r7]
	ldr r3, [r6]
	cmp r2, r3
	beq .L_08045d7c
	movs r5, #214
	lsls r5, r5, #1
	add r5, r9
	ldr r0, [r5]
	bl RenderOutput_PrepareForRedraw
	ldr r2, [r6]
	movs r0, #142
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r0, r0, #1
	lsls r3, r3, #2
	adds r3, r3, r0
	mov r1, r9
	ldr r0, [r1, r3]
	ldr r3, .L_08045db0
	ldr r1, [r5]
	adds r0, r0, r3
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	ldr r3, [r6]
	movs r2, #228
	str r3, [r7]
	lsls r2, r2, #1
	ldr r1, [r6]
	mov r5, r9
	lsls r3, r1, #2
	adds r3, r3, r2
	ldr r2, [r5, r3]
	mov r0, r9
	bl Func_080456f8
.L_08045d7c:
	movs r0, #218
	lsls r0, r0, #1
	add r0, r9
	bl AffineMatrix_BuildForEffect
	movs r3, #222
	lsls r3, r3, #1
	add r3, r9
	str r0, [r3]
	ldr r7, [sp, #16]
	movs r6, #0
	ldr r3, [r7]
	mov r8, r6
	cmp r8, r3
	bge .L_08045e56
	movs r5, #130
	movs r0, #63
	ldr r6, .L_08045dac
	lsls r5, r5, #1
	negs r0, r0
	add r5, r9
	adds r7, r0, #0
	b .L_08045db4
	.2byte 0x0000
.L_08045dac:
	.4byte 0xfffffe00
.L_08045db0:
	.4byte 0x0000003a
.L_08045db4:
	ldr r1, [sp, #24]
	ldr r3, [r1]
	cmp r8, r3
	bne .L_08045e24
	movs r3, #222
	lsls r3, r3, #1
	add r3, r9
	ldrb r2, [r3]
	ldrb r3, [r5, #7]
	movs r1, #31
	ands r2, r1
	lsls r2, r2, #1
	ands r3, r7
	orrs r3, r2
	strb r3, [r5, #7]
	ldrb r3, [r5, #5]
	movs r2, #3
	orrs r3, r2
	strb r3, [r5, #5]
	ldr r2, [sp, #36]
	ldr r1, [r5, #16]
	lsls r3, r2, #3
	subs r2, r3, r2
	cmp r2, #0
	bge .L_08045dee
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r2, r3
.L_08045dee:
	ldr r3, .L_08045e20
	asrs r2, r2, #9
	adds r2, r1, r2
	subs r2, #14
	ands r2, r3
	ldrh r3, [r5, #6]
	ands r3, r6
	orrs r3, r2
	strh r3, [r5, #6]
	ldr r0, [sp, #36]
	ldr r2, [r5, #20]
	lsls r3, r0, #1
	adds r3, r3, r0
	cmp r3, #0
	bge .L_08045e0e
	adds r3, #255
.L_08045e0e:
	asrs r3, r3, #8
	adds r3, r2, r3
	subs r3, #20
	strb r3, [r5, #4]
	adds r0, r5, #0
	movs r1, #241
	bl Runtime_PushSlotEntry
	b .L_08045e48
.L_08045e20:
	.4byte 0x000001ff
.L_08045e24:
	ldr r3, .L_08045e5c
	ldr r2, [r5, #16]
	movs r1, #4
	ands r2, r3
	ldrh r3, [r5, #6]
	negs r1, r1
	ands r3, r6
	orrs r3, r2
	strh r3, [r5, #6]
	ldr r3, [r5, #20]
	adds r2, r1, #0
	strb r3, [r5, #4]
	ldrb r3, [r5, #7]
	ands r3, r7
	strb r3, [r5, #7]
	ldrb r3, [r5, #5]
	ands r3, r2
	strb r3, [r5, #5]
.L_08045e48:
	ldr r0, [sp, #16]
	movs r2, #1
	ldr r3, [r0]
	add r8, r2
	adds r5, #28
	cmp r8, r3
	blt .L_08045db4
.L_08045e56:
	ldr r3, .L_08045e60
	movs r4, #192
	b .L_08045e64
.L_08045e5c:
	.4byte 0x000001ff
.L_08045e60:
	.4byte gInput
.L_08045e64:
	lsls r4, r4, #18
	ldr r5, [r3, #4]
	ldr r7, [r3, #12]
	adds r3, r4, #0
	adds r3, #228
	ldr r2, [r3]
	movs r1, #216
	adds r1, r1, r2
	mov r8, r1
	ldr r1, [r1]
	cmp r1, #0
	beq .L_08045f64
	adds r6, r2, #0
	adds r6, #220
	ldr r3, [r6]
	movs r7, #0
	movs r5, #0
	cmp r3, #0
	bne .L_08045f60
	adds r3, r2, #0
	adds r3, #224
	ldr r2, [r3]
	subs r3, r2, #5
	cmp r3, #1
	bhi .L_08045eb8
	ldr r2, [sp, #24]
	movs r5, #228
	ldr r3, [r2]
	lsls r5, r5, #1
	lsls r3, r3, #2
	adds r3, r3, r5
	mov r7, r9
	ldr r3, [r7, r3]
	cmp r3, #3
	bne .L_08045eb0
	movs r7, #1
	movs r5, #1
	b .L_08045eb4
.L_08045eb0:
	movs r7, #32
	movs r5, #32
.L_08045eb4:
	movs r3, #30
	b .L_08045f62
.L_08045eb8:
	cmp r2, #4
	bne .L_08045f56
	movs r0, #224
	lsls r0, r0, #1
	add r0, r9
	ldr r3, [r0]
	movs r2, #228
	lsls r2, r2, #1
	lsls r3, r3, #2
	adds r3, r3, r2
	mov r2, r9
	ldr r3, [r2, r3]
	cmp r3, #16
	beq .L_08045ede
	ldr r2, [sp, #40]
	cmp r2, #0
	bne .L_08045f4e
	cmp r3, #15
	bne .L_08045f4e
.L_08045ede:
	cmp r1, #1
	bne .L_08045f46
	ldr r1, [r4, #60]
	movs r2, #4
	ldrb r3, [r1, #5]
	orrs r3, r2
	strb r3, [r1, #5]
	mov r1, r9
	ldr r3, [r0]
	movs r0, #228
	lsls r3, r3, #2
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r3, [r1, r3]
	cmp r3, #15
	bne .L_08045f08
	bl Func_0804519c
	movs r1, #15
	adds r0, #10
	b .L_08045f14
.L_08045f08:
	cmp r3, #16
	bne .L_08045f1c
	bl Func_0804519c
	movs r1, #15
	adds r0, #9
.L_08045f14:
	movs r2, #8
	bl UiText_ShowMessageAndWaitComplete
	str r0, [sp, #20]
.L_08045f1c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #60]
	movs r3, #251
	ldrb r2, [r1, #5]
	movs r0, #102
	ands r3, r2
	strb r3, [r1, #5]
	movs r1, #155
	bl Func_08118128
	movs r1, #1
	ldr r0, [sp, #20]
	bl UiWork_Finalize
	mov r2, r8
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	movs r3, #45
	b .L_08045f62
.L_08045f46:
	movs r3, #200
	movs r7, #1
	movs r5, #1
	b .L_08045f62
.L_08045f4e:
	movs r3, #40
	movs r7, #16
	movs r5, #16
	b .L_08045f62
.L_08045f56:
	movs r3, #60
	str r3, [r6]
	movs r7, #1
	movs r5, #1
	b .L_08045f64
.L_08045f60:
	subs r3, #1
.L_08045f62:
	str r3, [r6]
.L_08045f64:
	movs r3, #192
	lsls r3, r3, #2
	ands r3, r5
	cmp r3, #0
	beq .L_08045f76
	ldr r3, [sp, #52]
	cmp r3, #0
	beq .L_08045f76
	b .L_080458ee
.L_08045f76:
	movs r3, #1
	ands r3, r5
	cmp r3, #0
	beq .L_08045f80
	b .L_080458f6
.L_08045f80:
	movs r3, #2
	ands r3, r5
	cmp r3, #0
	beq .L_08045f8a
	b .L_0804590e
.L_08045f8a:
	movs r3, #144
	ands r3, r7
	cmp r3, #0
	beq .L_08045faa
	movs r0, #111
	bl Audio_PlayCue
	ldr r5, [sp, #24]
	ldr r6, [sp, #16]
	ldr r0, [r5]
	ldr r1, [r6]
	adds r0, #1
	bl __modsi3
	str r0, [r5]
	b .L_08045fdc
.L_08045faa:
	movs r3, #96
	ands r3, r7
	cmp r3, #0
	beq .L_08045fcc
	movs r0, #111
	bl Audio_PlayCue
	ldr r7, [sp, #24]
	ldr r2, [sp, #16]
	ldr r0, [r7]
	ldr r1, [r2]
	adds r0, r0, r1
	subs r0, #1
	bl __modsi3
	str r0, [r7]
	b .L_08045fdc
.L_08045fcc:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r3, [r3]
	ldr r3, [r3, #76]
	cmp r3, #0
	bne .L_08045fdc
	b .L_0804591c
.L_08045fdc:
	movs r1, #184
	movs r0, #128
	lsls r1, r1, #5
	lsls r0, r0, #19
	adds r1, #65
	bl QueueIoWriteDelay2
	ldr r5, [sp, #48]
	movs r3, #0
	strb r3, [r5, #6]
	movs r0, #1
	bl WaitFrames
	b .L_08045a92
.L_08045ff8:
	ldr r6, [sp, #52]
	cmp r6, #0
	beq .L_08046012
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r2, [r3]
	movs r3, #224
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	str r3, [r2, #64]
	b .L_08046024
.L_08046012:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r2, [r3]
	movs r3, #224
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	str r3, [r2, #60]
.L_08046024:
	movs r3, #216
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	movs r7, #0
	mov r8, r7
	cmp r8, r3
	bge .L_08046052
	movs r6, #216
	movs r5, #136
	lsls r6, r6, #1
	lsls r5, r5, #1
	add r6, r9
	add r5, r9
.L_08046040:
	ldrh r0, [r5]
	bl Resource_ResetEntry
	ldr r3, [r6]
	movs r0, #1
	add r8, r0
	adds r5, #28
	cmp r8, r3
	blt .L_08046040
.L_08046052:
	ldr r1, [sp, #48]
	movs r3, #1
	strb r3, [r1, #6]
	movs r3, #214
	lsls r3, r3, #1
	add r3, r9
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_Finalize
	movs r3, #240
	movs r7, #128
	lsls r3, r3, #8
	movs r2, #3
	lsls r7, r7, #3
	adds r3, #127
	movs r4, #0
	mov r12, r2
	movs r5, #0
	adds r7, #82
	mov lr, r3
.L_0804607c:
	ldr r0, [sp, #48]
	adds r3, r5, r4
	movs r6, #0
	lsls r3, r3, #1
	mov r8, r6
	adds r1, r3, r0
.L_08046088:
	movs r2, #0
	adds r0, r1, #0
.L_0804608c:
	adds r3, r2, #0
	mov r6, r12
	ands r3, r6
	lsls r3, r3, #1
	adds r3, r3, r0
	adds r3, r3, r7
	mov r6, lr
	adds r2, #1
	strh r6, [r3]
	cmp r2, #2
	ble .L_0804608c
	movs r0, #1
	add r8, r0
	mov r2, r8
	adds r1, #64
	cmp r2, #2
	ble .L_08046088
	adds r4, #1
	adds r5, #2
	cmp r4, #6
	ble .L_0804607c
	ldr r5, [sp, #48]
	movs r3, #1
	strb r3, [r5, #3]
	bl WaitFrames
	ldr r0, .L_08046128
	bl Scheduler_RemoveCallback
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Runtime_SetIrqHandler
	ldr r1, .L_0804612c
	ldr r0, .L_08046130
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_08046102
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #168
	adds r3, r3, r1
	lsls r2, r2, #5
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_08046102:
	strh r4, [r0]
	mov r0, r9
	bl Sys_Free
	ldr r6, [sp, #48]
	movs r3, #0
	strb r3, [r6, #6]
	movs r0, #1
	bl WaitFrames
	mov r0, r10
	add sp, #60
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08046128:
	.4byte Graphics_SetBg1Priority3
.L_0804612c:
	.4byte gIoWriteQueue
.L_08046130:
	.4byte 0x04000208
