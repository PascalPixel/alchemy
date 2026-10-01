.syntax unified
	.thumb
	.global Func_08046b08
	.thumb_func
Func_08046b08:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_08046c30
	movs r3, #0
	add sp, r5
	str r1, [sp, #148]
	str r2, [sp, #144]
	str r0, [sp, #152]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #60]
	movs r2, #1
	negs r2, r2
	str r0, [sp, #140]
	movs r1, #1
	adds r0, r2, #0
	str r1, [sp, #132]
	str r2, [sp, #124]
	str r3, [sp, #104]
	bl Party_SumDjinnCountsFar
	str r0, [sp, #100]
	movs r0, #168
	movs r4, #0
	lsls r0, r0, #1
	str r4, [sp, #92]
	str r4, [sp, #88]
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #84]
	movs r0, #0
	str r0, [sp, #80]
	str r0, [sp, #76]
	str r0, [sp, #72]
	ldr r1, [sp, #152]
	add r0, sp, #416
	ldrh r3, [r1]
	movs r1, #1
	strh r3, [r0]
	lsls r3, r3, #16
	lsrs r3, r3, #16
	str r3, [sp, #96]
	movs r3, #255
	strh r3, [r0, #2]
	bl BattleActor_SpawnObjectsForListFar
	movs r0, #128
	lsls r0, r0, #2
	bl Resource_LoadIntoFreeSlot
	str r0, [sp, #108]
	adds r5, #228
	ldr r5, [r5]
	ldr r2, [sp, #132]
	ldr r0, [r5, #68]
	str r2, [r5, #72]
	cmp r0, #0
	beq .L_08046b90
	movs r1, #1
	bl UiWork_Finalize
	ldr r3, [sp, #92]
	str r3, [r5, #68]
.L_08046b90:
	movs r0, #112
	bl Audio_PlayCue
	movs r6, #4
	movs r2, #0
	add r3, sp, #440
.L_08046b9c:
	subs r6, #1
	strb r2, [r3]
	subs r3, #1
	cmp r6, #0
	bge .L_08046b9c
	add r2, sp, #436
	movs r3, #0
	str r3, [r2, #8]
	str r3, [r2, #12]
	str r3, [r2, #16]
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	movs r4, #168
	str r0, [sp, #120]
	movs r0, #165
	lsls r4, r4, #2
	lsls r0, r0, #2
	add r4, sp
	add r0, sp
	movs r1, #1
	str r4, [sp, #36]
	str r0, [sp, #40]
	negs r1, r1
	mov r8, r1
	adds r5, r0, #0
	adds r7, r4, #0
	movs r6, #10
.L_08046bd4:
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	subs r6, #1
	mov r2, r8
	strb r2, [r5]
	stmia r7!, {r0}
	adds r5, #1
	cmp r6, #0
	bge .L_08046bd4
	ldr r3, .L_08046c34
	add r5, sp, #488
	add r7, sp, #420
	mov r8, r3
	movs r6, #3
.L_08046bf2:
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	movs r1, #1
	stmia r7!, {r0}
	negs r1, r1
	bl Resource_GetBuffer
	ldr r3, .L_08046c2c
	mov r4, r8
	ands r0, r3
	ldrh r3, [r5]
	subs r6, #1
	ands r3, r4
	orrs r3, r0
	strh r3, [r5]
	adds r5, #12
	cmp r6, #0
	bge .L_08046bf2
	ldr r0, [sp, #148]
	cmp r0, #0
	beq .L_08046c5e
	ldr r1, [sp, #152]
	movs r6, #0
	ldrh r3, [r1]
	cmp r3, #255
	beq .L_08046c5e
	b .L_08046c38
	.2byte 0x0000
.L_08046c2c:
	.4byte 0x000003ff
.L_08046c30:
	.4byte 0xfffffd1c
.L_08046c34:
	.4byte 0xfffffc00
.L_08046c38:
	cmp r3, #254
	beq .L_08046c42
	ldr r2, [sp, #144]
	cmp r3, r2
	beq .L_08046c5c
.L_08046c42:
	adds r6, #1
	cmp r6, #5
	bgt .L_08046c5e
	ldr r4, [sp, #152]
	lsls r3, r6, #1
	ldrh r3, [r3, r4]
	cmp r3, #255
	beq .L_08046c5e
	cmp r3, #254
	beq .L_08046c42
	ldr r0, [sp, #144]
	cmp r3, r0
	bne .L_08046c42
.L_08046c5c:
	str r6, [sp, #124]
.L_08046c5e:
	movs r3, #6
	movs r1, #0
	str r3, [sp, #0]
	movs r2, #30
	movs r3, #20
	movs r0, #0
	str r1, [sp, #112]
	str r1, [sp, #116]
	bl UiWindow_Create
	movs r3, #10
	str r0, [sp, #136]
	str r3, [sp, #0]
	movs r2, #30
	movs r3, #6
	movs r1, #14
	movs r0, #0
	bl UiWindow_Create
	str r0, [sp, #128]
	bl Func_08041b68
	ldr r2, [sp, #108]
	movs r3, #182
	movs r4, #179
	movs r0, #132
	lsls r3, r3, #2
	lsls r4, r4, #2
	lsls r0, r0, #2
	lsls r2, r2, #2
	add r3, sp
	add r4, sp
	add r0, sp
	str r2, [sp, #24]
	str r3, [sp, #28]
	str r4, [sp, #32]
	str r0, [sp, #44]
.L_08046ca8:
	ldr r3, .L_08046d18
	ldr r0, .L_08046d1c
	ldr r3, [r3, #12]
	add r5, sp, #468
	str r3, [sp, #68]
	bl Func_0804537c
	ldr r1, [sp, #132]
	cmp r1, #0
	beq .L_08046cc4
	ldr r0, [sp, #144]
	ldr r1, [sp, #108]
	bl Func_08045528
.L_08046cc4:
	ldr r3, .L_08046d20
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #8]
	ldr r2, [sp, #24]
	ldr r3, .L_08046d24
	ldrh r1, [r5, #8]
	adds r3, r2, r3
	ldrh r2, [r3, #2]
	ldr r3, .L_08046d28
	lsls r2, r2, #17
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
	ldrh r2, [r5, #6]
	ldr r3, .L_08046d2c
	adds r0, r5, #0
	ands r3, r2
	ldr r2, .L_08046d14
	movs r1, #240
	orrs r3, r2
	ldrb r2, [r5, #9]
	strh r3, [r5, #6]
	movs r3, #24
	strb r3, [r5, #4]
	movs r3, #15
	ands r3, r2
	movs r2, #224
	orrs r3, r2
	strb r3, [r5, #9]
	bl Runtime_PushSlotEntry
	ldr r3, [sp, #444]
	cmp r3, #24
	bhi .L_08046d9c
	ldr r2, .L_08046d30
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	b .L_08046d34
.L_08046d14:
	.4byte 0x00000008
.L_08046d18:
	.4byte gInput
.L_08046d1c:
	.4byte 0x060066c0
.L_08046d20:
	.4byte 0x80000400
.L_08046d24:
	.4byte ResourceTableEntries
.L_08046d28:
	.4byte 0xfffffc00
.L_08046d2c:
	.4byte 0xfffffe00
.L_08046d30:
	.4byte .L_08046d38
.L_08046d34:
	mov pc, r3
	.2byte 0x0000
.L_08046d38:
	.4byte .L_08046d9c
	.4byte .L_08046d9c
	.4byte .L_08046d9c
	.4byte .L_08046d9c
	.4byte .L_08046d9c
	.4byte .L_08046d9c
	.4byte .L_08046d9c
	.4byte .L_08046d9c
	.4byte .L_08046d9c
	.4byte .L_08046d9c
	.4byte .L_08046db0
	.4byte .L_08046db0
	.4byte .L_08046db0
	.4byte .L_08046db0
	.4byte .L_08046dba
	.4byte .L_08046dba
	.4byte .L_08046dba
	.4byte .L_08046dba
	.4byte .L_08046dba
	.4byte .L_08046dba
	.4byte .L_08046dba
	.4byte .L_08046dba
	.4byte .L_08046dc6
	.4byte .L_08046dd2
	.4byte .L_08046dde
.L_08046d9c:
	movs r3, #0
	add r2, sp, #436
	str r3, [r2, #16]
	ldr r3, [sp, #100]
	cmp r3, #0
	beq .L_08046dac
	movs r3, #9
	b .L_08046de6
.L_08046dac:
	movs r3, #7
	b .L_08046de6
.L_08046db0:
	add r2, sp, #436
	movs r3, #1
	str r3, [r2, #16]
	movs r3, #4
	b .L_08046de6
.L_08046dba:
	add r2, sp, #436
	movs r3, #2
	str r3, [r2, #16]
	ldr r4, [sp, #104]
	str r4, [r2, #20]
	b .L_08046de8
.L_08046dc6:
	add r2, sp, #436
	movs r3, #3
	str r3, [r2, #16]
	ldr r0, [sp, #76]
	str r0, [r2, #20]
	b .L_08046de8
.L_08046dd2:
	add r2, sp, #436
	movs r3, #4
	str r3, [r2, #16]
	ldr r1, [sp, #80]
	str r1, [r2, #20]
	b .L_08046de8
.L_08046dde:
	add r2, sp, #436
	movs r3, #5
	str r3, [r2, #16]
	ldr r3, [sp, #72]
.L_08046de6:
	str r3, [r2, #20]
.L_08046de8:
	ldr r1, [sp, #136]
	ldr r3, [sp, #136]
	movs r4, #12
	ldrsh r0, [r1, r4]
	movs r2, #14
	ldrsh r1, [r1, r2]
	movs r4, #15
	ldrh r2, [r3, #8]
	ldrh r3, [r3, #10]
	str r4, [sp, #0]
	bl Func_08046134
	ldr r3, .L_08047114
	movs r0, #1
	ldr r3, [r3, #4]
	ands r3, r0
	cmp r3, #0
	beq .L_08046e4c
	ldr r2, [sp, #92]
	adds r2, #1
	adds r3, r2, #0
	cmp r2, #0
	bge .L_08046e1a
	ldr r3, [sp, #92]
	adds r3, #4
.L_08046e1a:
	asrs r3, r3, #2
	str r3, [sp, #92]
	lsls r3, r3, #2
	subs r2, r2, r3
	str r2, [sp, #92]
	add r1, sp, #436
	adds r2, #3
	movs r3, #0
	str r2, [r1, #16]
	strb r3, [r1, r2]
	str r0, [r1, #20]
	ldr r4, [sp, #92]
	cmp r4, #0
	beq .L_08046e3e
	adds r3, r4, #0
	adds r3, #21
	str r3, [r1, #8]
	b .L_08046e42
.L_08046e3e:
	ldr r0, [sp, #92]
	str r0, [r1, #8]
.L_08046e42:
	movs r1, #1
	movs r0, #112
	str r1, [sp, #132]
	bl Audio_PlayCue
.L_08046e4c:
	ldr r2, [sp, #104]
	cmp r2, #0
	bne .L_08046e54
	b .L_08047166
.L_08046e54:
	movs r3, #218
	lsls r3, r3, #1
	add r3, sp
	ldr r0, [r3, #16]
	mov r8, r3
	cmp r0, #1
	bhi .L_08046efe
	ldr r4, [sp, #68]
	ldrsb r5, [r3, r0]
	movs r3, #128
	ands r3, r4
	cmp r3, #0
	beq .L_08046e96
	movs r0, #0
	str r0, [sp, #68]
	movs r0, #111
	bl Audio_PlayCue
	mov r1, r8
	ldr r3, [r1, #20]
	adds r5, #1
	cmp r5, r3
	bge .L_08046e84
	b .L_0804707c
.L_08046e84:
	ldr r3, [r1, #16]
	movs r5, #0
	cmp r3, #1
	beq .L_08046e8e
	b .L_0804707c
.L_08046e8e:
	movs r3, #2
	str r3, [r1, #16]
	ldrsb r5, [r1, r3]
	b .L_0804707c
.L_08046e96:
	ldr r2, [sp, #68]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	beq .L_08046ec8
	movs r3, #0
	movs r0, #111
	subs r5, #1
	str r3, [sp, #68]
	bl Audio_PlayCue
	cmp r5, #0
	blt .L_08046eb2
	b .L_0804707c
.L_08046eb2:
	mov r4, r8
	ldr r3, [r4, #20]
	subs r5, r3, #1
	ldr r3, [r4, #16]
	cmp r3, #1
	beq .L_08046ec0
	b .L_0804707c
.L_08046ec0:
	movs r3, #2
	str r3, [r4, #16]
	ldrsb r5, [r4, r3]
	b .L_0804707c
.L_08046ec8:
	ldr r0, [sp, #68]
	movs r3, #48
	ands r3, r0
	cmp r3, #0
	bne .L_08046ed4
	b .L_0804707c
.L_08046ed4:
	movs r1, #0
	movs r0, #111
	str r1, [sp, #68]
	bl Audio_PlayCue
	mov r2, r8
	ldr r3, [r2, #16]
	movs r2, #2
	eors r3, r2
	mov r4, r8
	str r3, [r4, #16]
	ldr r3, .L_08047114
	ldr r5, [sp, #104]
	ldr r2, [r3, #12]
	movs r3, #32
	ands r2, r3
	subs r5, #1
	cmp r2, #0
	beq .L_08046efc
	b .L_0804707c
.L_08046efc:
	b .L_0804707a
.L_08046efe:
	cmp r0, #2
	bne .L_08046f80
	mov r1, r8
	ldrsb r5, [r1, r0]
	ldr r2, [r1, #20]
	cmp r5, r2
	blt .L_08046f0e
	subs r5, r2, #1
.L_08046f0e:
	cmp r5, #0
	bge .L_08046f1e
	mov r2, r8
	movs r3, #0
	str r3, [r2, #16]
	movs r5, #0
	ldrsb r5, [r2, r5]
	b .L_0804707c
.L_08046f1e:
	ldr r4, [sp, #68]
	movs r3, #16
	ands r3, r4
	cmp r3, #0
	beq .L_08046f3c
	movs r0, #0
	adds r5, #1
	str r0, [sp, #68]
	cmp r5, r2
	blt .L_08046f58
	mov r1, r8
	str r0, [r1, #16]
	movs r5, #0
	ldrsb r5, [r1, r5]
	b .L_08046f58
.L_08046f3c:
	ldr r2, [sp, #68]
	movs r3, #32
	ands r3, r2
	cmp r3, #0
	beq .L_08046f60
	movs r3, #0
	subs r5, #1
	str r3, [sp, #68]
	cmp r5, #0
	bge .L_08046f58
	mov r4, r8
	str r3, [r4, #16]
	movs r5, #0
	ldrsb r5, [r4, r5]
.L_08046f58:
	movs r0, #111
	bl Audio_PlayCue
	b .L_0804707c
.L_08046f60:
	ldr r0, [sp, #68]
	movs r3, #192
	ands r3, r0
	cmp r3, #0
	bne .L_08046f6c
	b .L_0804707c
.L_08046f6c:
	movs r1, #0
	str r1, [sp, #68]
	mov r2, r8
	str r1, [r2, #16]
	movs r0, #111
	movs r5, #0
	ldrsb r5, [r2, r5]
	bl Audio_PlayCue
	b .L_0804707c
.L_08046f80:
	cmp r0, #3
	beq .L_08046f88
	cmp r0, #5
	bne .L_08047084
.L_08046f88:
	ldr r4, [sp, #68]
	mov r3, r8
	ldrsb r5, [r3, r0]
	movs r3, #128
	ands r3, r4
	cmp r3, #0
	beq .L_08046fb6
	movs r0, #0
	str r0, [sp, #68]
	movs r0, #111
	bl Audio_PlayCue
	ldr r1, [sp, #132]
	movs r3, #2
	orrs r1, r3
	str r1, [sp, #132]
	mov r2, r8
	ldr r3, [r2, #20]
	adds r5, #1
	cmp r5, r3
	blt .L_08047076
	movs r5, #0
	b .L_0804706e
.L_08046fb6:
	ldr r4, [sp, #68]
	movs r3, #64
	ands r3, r4
	cmp r3, #0
	beq .L_08046fe0
	movs r0, #0
	str r0, [sp, #68]
	movs r0, #111
	bl Audio_PlayCue
	ldr r1, [sp, #132]
	movs r3, #2
	orrs r1, r3
	subs r5, #1
	str r1, [sp, #132]
	cmp r5, #0
	bge .L_0804706e
	mov r2, r8
	ldr r3, [r2, #20]
	subs r5, r3, #1
	b .L_0804706e
.L_08046fe0:
	ldr r4, [sp, #68]
	movs r3, #16
	ands r3, r4
	cmp r3, #0
	beq .L_0804702c
	movs r0, #0
	str r0, [sp, #68]
	movs r0, #111
	bl Audio_PlayCue
	ldr r1, [sp, #132]
	movs r3, #2
	orrs r1, r3
	str r1, [sp, #132]
	mov r2, r8
	ldr r1, [r2, #20]
	adds r5, #4
	cmp r5, r1
	blt .L_08047076
	adds r3, r5, #0
	cmp r5, #0
	bge .L_0804700e
	adds r3, r5, #3
.L_0804700e:
	asrs r0, r3, #2
	subs r2, r1, #1
	subs r4, r0, #1
	adds r3, r2, #0
	cmp r2, #0
	bge .L_0804701c
	adds r3, r1, #2
.L_0804701c:
	asrs r3, r3, #2
	cmp r4, r3
	bne .L_08047028
	lsls r3, r0, #2
	subs r5, r5, r3
	b .L_0804706e
.L_08047028:
	adds r5, r2, #0
	b .L_0804706e
.L_0804702c:
	ldr r4, [sp, #68]
	movs r3, #32
	ands r3, r4
	cmp r3, #0
	beq .L_0804706e
	movs r0, #0
	str r0, [sp, #68]
	movs r0, #111
	bl Audio_PlayCue
	ldr r1, [sp, #132]
	movs r3, #2
	orrs r1, r3
	subs r5, #4
	str r1, [sp, #132]
	cmp r5, #0
	bge .L_0804706e
	mov r2, r8
	ldr r3, [r2, #20]
	cmp r3, #0
	bge .L_08047058
	adds r3, #3
.L_08047058:
	asrs r3, r3, #2
	adds r2, r5, #4
	lsls r1, r3, #2
	adds r3, r2, #0
	cmp r2, #0
	bge .L_08047066
	adds r3, r5, #7
.L_08047066:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	adds r5, r1, r3
.L_0804706e:
	ldr r3, [sp, #456]
	cmp r5, r3
	blt .L_08047076
	subs r5, r3, #1
.L_08047076:
	cmp r5, #0
	bge .L_0804707c
.L_0804707a:
	movs r5, #0
.L_0804707c:
	add r2, sp, #436
	ldr r3, [r2, #16]
	strb r5, [r2, r3]
	b .L_08047166
.L_08047084:
	cmp r0, #4
	bne .L_08047166
	ldr r4, [sp, #68]
	mov r3, r8
	ldrsb r6, [r3, r0]
	movs r3, #128
	ands r3, r4
	cmp r3, #0
	beq .L_080470b6
	movs r0, #0
	str r0, [sp, #68]
	movs r0, #111
	bl Audio_PlayCue
	ldr r1, [sp, #132]
	movs r3, #2
	orrs r1, r3
	str r1, [sp, #132]
	mov r2, r8
	ldr r3, [r2, #20]
	adds r6, #1
	cmp r6, r3
	blt .L_0804715a
	movs r6, #0
	b .L_08047152
.L_080470b6:
	ldr r4, [sp, #68]
	movs r3, #64
	ands r3, r4
	cmp r3, #0
	beq .L_080470e0
	movs r0, #0
	str r0, [sp, #68]
	movs r0, #111
	bl Audio_PlayCue
	ldr r1, [sp, #132]
	movs r3, #2
	orrs r1, r3
	subs r6, #1
	str r1, [sp, #132]
	cmp r6, #0
	bge .L_08047152
	mov r2, r8
	ldr r3, [r2, #20]
	subs r6, r3, #1
	b .L_08047152
.L_080470e0:
	ldr r4, [sp, #68]
	movs r3, #16
	ands r3, r4
	cmp r3, #0
	beq .L_08047118
	movs r0, #0
	str r0, [sp, #68]
	movs r0, #111
	bl Audio_PlayCue
	ldr r1, [sp, #132]
	movs r3, #2
	orrs r1, r3
	str r1, [sp, #132]
	mov r2, r8
	ldr r3, [r2, #20]
	adds r6, #3
	cmp r6, r3
	blt .L_0804715a
	adds r0, r6, #0
	movs r1, #3
	bl __modsi3
	adds r6, r0, #0
	b .L_08047152
	.2byte 0x0000
.L_08047114:
	.4byte gInput
.L_08047118:
	ldr r4, [sp, #68]
	movs r3, #32
	ands r3, r4
	cmp r3, #0
	beq .L_08047152
	movs r0, #0
	str r0, [sp, #68]
	movs r0, #111
	bl Audio_PlayCue
	ldr r1, [sp, #132]
	movs r3, #2
	orrs r1, r3
	subs r6, #3
	str r1, [sp, #132]
	cmp r6, #0
	bge .L_08047152
	mov r2, r8
	ldr r0, [r2, #20]
	movs r1, #3
	bl __divsi3
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r1, #3
	adds r0, r6, #3
	bl __modsi3
	adds r6, r5, r0
.L_08047152:
	ldr r3, [sp, #456]
	cmp r6, r3
	blt .L_0804715a
	subs r6, r3, #1
.L_0804715a:
	cmp r6, #0
	bge .L_08047160
	movs r6, #0
.L_08047160:
	add r2, sp, #436
	ldr r3, [r2, #16]
	strb r6, [r2, r3]
.L_08047166:
	movs r3, #218
	lsls r3, r3, #1
	add r3, sp
	ldr r2, [r3, #16]
	mov r8, r3
	cmp r2, #0
	bne .L_080471b2
	ldr r4, [sp, #100]
	ldrsb r2, [r3, r2]
	cmp r4, #0
	bne .L_0804717e
	adds r2, #9
.L_0804717e:
	ldr r3, .L_08047280
	lsls r2, r2, #3
	adds r2, r2, r3
	ldrb r3, [r2]
	mov r0, r8
	str r3, [r0, #8]
	ldrb r3, [r2, #1]
	str r3, [r0, #24]
	ldrb r3, [r2, #2]
	str r3, [r0, #28]
	ldr r3, [sp, #136]
	movs r1, #12
	ldrsh r0, [r3, r1]
	ldrb r3, [r2, #3]
	adds r0, r0, r3
	ldr r3, [sp, #136]
	adds r0, #1
	movs r4, #14
	ldrsh r1, [r3, r4]
	ldrb r3, [r2, #4]
	ldrb r2, [r2, #5]
	adds r1, r1, r3
	movs r3, #14
	str r3, [sp, #0]
	adds r1, #1
	b .L_08047278
.L_080471b2:
	cmp r2, #1
	bne .L_080471ce
	mov r4, r8
	ldrsb r3, [r4, r2]
	ldr r1, .L_08047284
	lsls r3, r3, #2
	ldrb r2, [r1, r3]
	adds r3, r3, r1
	str r2, [r4, #8]
	ldrb r2, [r3, #1]
	ldrb r3, [r3, #2]
	str r2, [r4, #24]
	str r3, [r4, #28]
	b .L_080472da
.L_080471ce:
	cmp r2, #2
	bne .L_08047228
	mov r0, r8
	ldrsb r3, [r0, r2]
	ldr r1, .L_08047288
	lsls r3, r3, #2
	ldrb r2, [r1, r3]
	adds r3, r3, r1
	str r2, [r0, #8]
	ldrb r1, [r3, #1]
	ldrb r2, [r3, #2]
	str r1, [r0, #24]
	str r2, [r0, #28]
	ldr r4, [sp, #40]
	movs r3, #0
	ldrsb r3, [r4, r3]
	cmp r3, #0
	bne .L_08047208
	ldr r2, [sp, #136]
	movs r1, #12
	ldrsh r0, [r2, r1]
	movs r3, #14
	ldrsh r1, [r2, r3]
	movs r3, #14
	str r3, [sp, #0]
	adds r0, #15
	adds r1, #1
	movs r2, #10
	b .L_08047278
.L_08047208:
	ldr r3, [sp, #136]
	movs r4, #12
	ldrsh r0, [r3, r4]
	adds r0, r0, r1
	movs r4, #14
	ldrsh r1, [r3, r4]
	movs r3, #14
	adds r1, r1, r2
	str r3, [sp, #0]
	adds r0, #1
	adds r1, #1
	movs r2, #2
	movs r3, #2
	bl Func_08046134
	b .L_080472da
.L_08047228:
	cmp r2, #3
	beq .L_08047230
	cmp r2, #5
	bne .L_0804728c
.L_08047230:
	mov r0, r8
	ldrsb r1, [r0, r2]
	adds r3, r1, #0
	cmp r1, #0
	bge .L_0804723c
	adds r3, r1, #3
.L_0804723c:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r1, r3
	lsls r3, r3, #24
	asrs r3, r3, #23
	adds r3, #8
	mov r1, r8
	str r3, [r1, #28]
	cmp r2, #3
	bne .L_08047258
	movs r3, #2
	negs r3, r3
	str r3, [r1, #24]
	b .L_08047260
.L_08047258:
	movs r3, #1
	negs r3, r3
	mov r2, r8
	str r3, [r2, #24]
.L_08047260:
	ldr r4, [sp, #136]
	movs r3, #12
	ldrsh r0, [r4, r3]
	movs r2, #14
	ldrsh r1, [r4, r2]
	ldr r3, [sp, #464]
	adds r0, #1
	adds r1, r1, r3
	movs r3, #14
	str r3, [sp, #0]
	adds r1, #1
	movs r2, #20
.L_08047278:
	movs r3, #1
	bl Func_08046134
	b .L_080472da
.L_08047280:
	.4byte Data_0805f7d8
.L_08047284:
	.4byte Data_0805f858
.L_08047288:
	.4byte Data_0805f868
.L_0804728c:
	mov r3, r8
	movs r6, #4
	ldrsb r6, [r3, r6]
	movs r1, #3
	adds r0, r6, #0
	bl __modsi3
	adds r5, r0, #0
	lsls r5, r5, #24
	asrs r5, r5, #23
	adds r5, #9
	mov r4, r8
	movs r1, #3
	str r5, [r4, #28]
	adds r0, r6, #0
	bl __divsi3
	lsls r0, r0, #24
	asrs r0, r0, #24
	lsls r3, r0, #3
	subs r3, r3, r0
	subs r3, #2
	mov r0, r8
	str r3, [r0, #24]
	ldr r2, [sp, #136]
	movs r1, #12
	ldrsh r0, [r2, r1]
	adds r0, r0, r3
	movs r3, #14
	ldrsh r1, [r2, r3]
	movs r3, #14
	adds r1, r1, r5
	str r3, [sp, #0]
	adds r0, #3
	adds r1, #1
	movs r2, #7
	movs r3, #1
	bl Func_08046134
.L_080472da:
	add r1, sp, #436
	ldr r3, [r1, #12]
	ldr r2, [r1, #8]
	cmp r3, r2
	beq .L_080472ee
	str r2, [r1, #12]
	ldr r4, [sp, #132]
	movs r3, #2
	orrs r4, r3
	str r4, [sp, #132]
.L_080472ee:
	ldr r3, [r1, #24]
	ldr r0, [sp, #96]
	lsls r3, r3, #3
	str r3, [sp, #112]
	ldr r3, [r1, #28]
	ldr r1, [sp, #144]
	lsls r3, r3, #3
	str r3, [sp, #116]
	cmp r0, r1
	beq .L_08047326
	add r2, sp, #144
	ldrh r2, [r2]
	add r5, sp, #412
	movs r3, #255
	strh r2, [r5]
	strh r3, [r5, #2]
	ldr r0, [sp, #96]
	bl ReleaseBattleObjectRecordsFar
	ldr r3, [sp, #144]
	adds r0, r5, #0
	movs r1, #1
	movs r2, #0
	str r3, [sp, #96]
	bl Func_08118148
	movs r4, #0
	str r4, [sp, #88]
.L_08047326:
	ldr r0, [sp, #144]
	bl GetBattleObjectSlotFar
	ldr r3, [r0]
	ldr r0, [sp, #28]
	ldr r2, [r3, #80]
	ldr r3, .L_08047394
	str r3, [r0, #4]
	movs r3, #0
	str r3, [r0, #8]
	cmp r2, #0
	beq .L_08047374
	ldrh r1, [r0, #8]
	ldrh r2, [r2, #8]
	ldr r3, .L_08047398
	lsls r2, r2, #22
	ands r3, r1
	ldr r1, [sp, #28]
	lsrs r2, r2, #22
	orrs r3, r2
	strh r3, [r1, #8]
	ldrh r2, [r1, #6]
	ldr r3, .L_0804739c
	ands r3, r2
	ldr r2, .L_0804738c
	orrs r3, r2
	ldr r2, [sp, #28]
	strh r3, [r2, #6]
	movs r3, #56
	strb r3, [r2, #4]
	ldr r3, [sp, #88]
	cmp r3, #0
	beq .L_08047370
	ldr r0, [sp, #28]
	movs r1, #240
	bl Runtime_PushSlotEntry
.L_08047370:
	movs r4, #1
	str r4, [sp, #88]
.L_08047374:
	ldr r0, [sp, #32]
	ldr r3, .L_080473a0
	str r3, [r0, #4]
	movs r3, #0
	str r3, [r0, #8]
	ldr r0, [sp, #120]
	ldr r1, .L_080473a4
	bl Resource_GetBuffer
	ldr r3, .L_08047390
	b .L_080473a8
	.2byte 0x0000
.L_0804738c:
	.4byte 0x000000ac
.L_08047390:
	.4byte 0x000003ff
.L_08047394:
	.4byte 0xc0002400
.L_08047398:
	.4byte 0xfffffc00
.L_0804739c:
	.4byte 0xfffffe00
.L_080473a0:
	.4byte 0x40000400
.L_080473a4:
	.4byte Data_080597f8
.L_080473a8:
	ldr r1, [sp, #32]
	ands r0, r3
	ldrh r2, [r1, #8]
	ldr r3, .L_0804743c
	ands r3, r2
	orrs r3, r0
	adds r2, r1, #0
	strh r3, [r2, #8]
	ldr r3, .L_08047440
	ldr r0, [sp, #136]
	ldr r2, [r3]
	movs r3, #4
	ands r2, r3
	movs r4, #12
	ldrsh r3, [r0, r4]
	ldr r1, [sp, #112]
	lsls r3, r3, #3
	adds r3, r3, r1
	adds r1, r3, #0
	ldr r3, [sp, #92]
	adds r1, #16
	cmp r3, #0
	bne .L_080473e4
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080473de
	adds r3, r2, #3
.L_080473de:
	asrs r3, r3, #2
	subs r3, r1, r3
	b .L_080473f0
.L_080473e4:
	adds r3, r2, #0
	cmp r3, #0
	bge .L_080473ec
	adds r3, #3
.L_080473ec:
	asrs r3, r3, #2
	adds r3, r1, r3
.L_080473f0:
	ldr r4, [sp, #32]
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	ldrh r1, [r4, #6]
	ands r2, r3
	ldr r3, .L_08047444
	adds r0, r4, #0
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #6]
	ldr r2, [sp, #136]
	ldr r4, [sp, #116]
	movs r1, #14
	ldrsh r3, [r2, r1]
	lsls r3, r3, #3
	adds r1, r4, r3
	ldr r3, .L_08047440
	ldr r2, [r3]
	movs r3, #4
	ands r2, r3
	cmp r2, #0
	bge .L_08047420
	adds r2, #3
.L_08047420:
	asrs r3, r2, #2
	ldr r0, [sp, #32]
	subs r3, r1, r3
	adds r3, #16
	strb r3, [r0, #4]
	ldr r1, [sp, #92]
	cmp r1, #0
	beq .L_08047448
	ldrb r2, [r0, #7]
	movs r3, #63
	negs r3, r3
	ands r3, r2
	strb r3, [r0, #7]
	b .L_0804745a
.L_0804743c:
	.4byte 0xfffffc00
.L_08047440:
	.4byte Data_0300122c
.L_08047444:
	.4byte 0xfffffe00
.L_08047448:
	ldr r2, [sp, #32]
	ldrb r3, [r2, #7]
	movs r2, #63
	negs r2, r2
	ands r2, r3
	movs r3, #16
	orrs r2, r3
	ldr r3, [sp, #32]
	strb r2, [r3, #7]
.L_0804745a:
	ldr r0, [sp, #32]
	movs r1, #241
	bl Runtime_PushSlotEntry
	ldr r4, [sp, #132]
	cmp r4, #0
	bne .L_0804746c
	bl .L_0804846e
.L_0804746c:
	ldr r0, [sp, #144]
	bl Owner_GetState
	str r0, [sp, #64]
	ldr r0, [sp, #92]
	cmp r0, #0
	bne .L_0804747e
	bl Ui_FillVramBlockPattern
.L_0804747e:
	ldr r1, [sp, #92]
	cmp r1, #3
	beq .L_08047486
	b .L_0804777c
.L_08047486:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r3, [r3]
	ldr r4, [sp, #64]
	adds r3, #150
	mov r11, r3
	movs r3, #1
	negs r3, r3
	mov r8, r3
	movs r3, #216
	ldrh r5, [r4, r3]
	movs r2, #0
	mov r9, r2
	cmp r5, #0
	beq .L_080474d8
	adds r7, r4, #0
	adds r7, #216
	mov r6, r11
.L_080474ac:
	adds r0, r5, #0
	str r2, [sp, #8]
	bl Item_Get
	adds r1, r5, #0
	ldr r0, [sp, #144]
	bl Item_ClassifyUseAbility
	ldr r2, [sp, #8]
	cmp r0, #0
	bne .L_080474ca
	movs r0, #1
	strh r5, [r6]
	add r9, r0
	adds r6, #2
.L_080474ca:
	adds r2, #1
	cmp r2, #15
	beq .L_080474d8
	adds r7, #2
	ldrh r5, [r7]
	cmp r5, #0
	bne .L_080474ac
.L_080474d8:
	ldr r1, [sp, #64]
	movs r3, #216
	ldrh r5, [r1, r3]
	movs r2, #0
	cmp r5, #0
	beq .L_0804751c
	mov r4, r9
	adds r7, r1, #0
	lsls r3, r4, #1
	mov r0, r11
	adds r7, #216
	adds r6, r3, r0
.L_080474f0:
	adds r0, r5, #0
	str r2, [sp, #8]
	bl Item_Get
	adds r1, r5, #0
	ldr r0, [sp, #144]
	bl Item_ClassifyUseAbility
	ldr r2, [sp, #8]
	cmp r0, #0
	beq .L_0804750e
	movs r1, #1
	strh r5, [r6]
	add r9, r1
	adds r6, #2
.L_0804750e:
	adds r2, #1
	cmp r2, #15
	beq .L_0804751c
	adds r7, #2
	ldrh r5, [r7]
	cmp r5, #0
	bne .L_080474f0
.L_0804751c:
	mov r2, r9
	lsls r3, r2, #1
	ldr r2, .L_0804754c
	mov r4, r11
	mov r0, r9
	strh r2, [r3, r4]
	str r0, [sp, #72]
	add r1, sp, #436
	movs r3, #5
	ldrsb r3, [r1, r3]
	adds r2, r3, #0
	cmp r9, r3
	bgt .L_0804753e
	mov r3, r9
	subs r3, #1
	strb r3, [r1, #5]
	adds r2, r3, #0
.L_0804753e:
	lsls r3, r2, #24
	asrs r2, r3, #24
	adds r3, r2, #0
	cmp r2, #0
	bge .L_08047550
	adds r3, r2, #3
	b .L_08047550
.L_0804754c:
	.4byte 0x00000000
.L_08047550:
	asrs r3, r3, #2
	lsls r1, r3, #2
	adds r3, r1, #0
	subs r3, r2, r3
	lsls r3, r3, #24
	mov r2, r9
	mov r10, r1
	asrs r3, r3, #24
	cmp r2, #0
	beq .L_08047596
	add r3, r10
	lsls r3, r3, #1
	mov r4, r11
	adds r5, r3, r4
	ldrh r1, [r5]
	ldr r0, [sp, #144]
	bl Item_ClassifyUseAbility
	cmp r0, #2
	bne .L_0804757e
	ldr r0, .L_0804761c
	add r1, sp, #284
	b .L_0804758e
.L_0804757e:
	ldrh r3, [r5]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	ldr r3, .L_08047620
	add r1, sp, #284
	adds r0, r0, r3
.L_0804758e:
	movs r2, #32
	bl UiText_CopyMessageString
	b .L_080475a0
.L_08047596:
	ldr r0, .L_08047624
	add r1, sp, #284
	movs r2, #32
	bl UiText_CopyMessageString
.L_080475a0:
	ldr r0, [sp, #136]
	bl RenderOutput_RedrawSavedRect
	ldr r2, [sp, #128]
	ldr r4, [sp, #128]
	movs r1, #12
	ldrsh r0, [r2, r1]
	movs r3, #14
	ldrsh r1, [r2, r3]
	ldrh r3, [r4, #10]
	ldrh r2, [r2, #8]
	movs r4, #0
	str r4, [sp, #0]
	bl Func_0803a2b0
	bl Ui_FillVramBlockPattern
	movs r2, #0
	add r0, sp, #284
	ldr r1, [sp, #128]
	movs r3, #20
	bl UiText_RenderWideStringAtOffset
	cmp r10, r8
	bne .L_080475d4
	b .L_080476f0
.L_080475d4:
	movs r3, #16
	str r3, [sp, #0]
	ldr r0, [sp, #136]
	movs r1, #0
	movs r3, #29
	movs r2, #16
	bl UiWindow_DrawDividerLine
	mov r1, r10
	lsls r3, r1, #1
	add r3, r11
	ldrh r5, [r3]
	movs r0, #0
	mov r8, r0
	cmp r5, #0
	beq .L_08047698
	movs r2, #64
	adds r7, r3, #0
	add r6, sp, #488
	mov r11, r2
.L_080475fc:
	adds r0, r5, #0
	bl Item_Get
	movs r0, #15
	bl Func_08041f70
	adds r1, r5, #0
	ldr r0, [sp, #144]
	bl Item_ClassifyUseAbility
	cmp r0, #0
	beq .L_08047628
	movs r0, #4
	bl Func_08041f70
	b .L_08047638
.L_0804761c:
	.4byte 0x00000d4f
.L_08047620:
	.4byte 0x00000092
.L_08047624:
	.4byte 0x00000d46
.L_08047628:
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r5
	cmp r3, #0
	beq .L_08047638
	movs r0, #2
	bl Func_08041f70
.L_08047638:
	movs r0, #128
	ldr r3, .L_08047694
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r5
	ldr r1, [sp, #136]
	adds r0, r0, r3
	movs r2, #32
	mov r3, r11
	bl UiText_DrawCharacterAtOffset
	movs r0, #15
	bl Func_08041f70
	mov r4, r8
	add r2, sp, #420
	lsls r3, r4, #2
	ldr r1, [r2, r3]
	adds r0, r5, #0
	bl Resource_LoadKind26EntryToBuffer
	ldr r3, .L_0804768c
	ldr r2, .L_08047690
	ands r0, r3
	ldrh r3, [r6]
	movs r1, #1
	ands r3, r2
	add r8, r1
	orrs r3, r0
	mov r2, r8
	movs r0, #16
	strh r3, [r6]
	add r11, r0
	adds r6, #12
	cmp r2, #3
	bgt .L_08047698
	adds r7, #2
	ldrh r5, [r7]
	cmp r5, #0
	bne .L_080475fc
	b .L_08047698
	.2byte 0x0000
.L_0804768c:
	.4byte 0x000003ff
.L_08047690:
	.4byte 0xfffffc00
.L_08047694:
	.4byte 0x0000025f
.L_08047698:
	mov r3, r8
	cmp r3, #3
	bgt .L_080476f0
	lsls r3, r3, #1
	add r3, r8
	add r4, sp, #740
	lsls r3, r3, #2
	mov r0, r8
	adds r3, r3, r4
	ldr r1, .L_080476ec
	adds r5, r3, #0
	lsls r3, r0, #2
	adds r3, r3, r4
	adds r6, r3, r1
	ldr r7, .L_080476e4
	movs r3, #4
	subs r0, r3, r0
	subs r5, #252
	mov r8, r0
.L_080476be:
	movs r1, #1
	ldmia r6!, {r0}
	negs r1, r1
	bl Resource_GetBuffer
	ldr r3, .L_080476e8
	movs r2, #1
	ands r0, r3
	ldrh r3, [r5]
	negs r2, r2
	ands r3, r7
	orrs r3, r0
	add r8, r2
	strh r3, [r5]
	mov r3, r8
	adds r5, #12
	cmp r3, #0
	bne .L_080476be
	b .L_080476f0
.L_080476e4:
	.4byte 0xfffffc00
.L_080476e8:
	.4byte 0x000003ff
.L_080476ec:
	.4byte 0xfffffec0
.L_080476f0:
	mov r4, r9
	cmp r4, #4
	bgt .L_080476f8
	b .L_08047d78
.L_080476f8:
	movs r0, #0
	mov r8, r0
	mov r5, r9
	adds r5, #3
	b .L_0804773a
.L_08047702:
	movs r1, #243
	lsls r1, r1, #8
	adds r1, #1
	mov r3, r10
	add r1, r8
	cmp r3, #0
	bge .L_08047712
	adds r3, #3
.L_08047712:
	asrs r3, r3, #2
	cmp r8, r3
	bne .L_08047720
	movs r1, #243
	lsls r1, r1, #8
	adds r1, #11
	add r1, r8
.L_08047720:
	ldr r3, [sp, #136]
	ldrh r2, [r3, #8]
	movs r3, #0
	subs r2, r2, r0
	add r2, r8
	str r3, [sp, #0]
	subs r2, #10
	ldr r0, [sp, #136]
	movs r3, #7
	bl UiWindow_SetTilemapEntry
	movs r4, #1
	add r8, r4
.L_0804773a:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_08047744
	mov r3, r9
	adds r3, #6
.L_08047744:
	asrs r0, r3, #2
	cmp r8, r0
	blt .L_08047702
	ldr r1, [sp, #136]
	movs r5, #0
	ldrh r2, [r1, #8]
	movs r1, #243
	subs r2, r2, r0
	lsls r1, r1, #8
	subs r2, #11
	ldr r0, [sp, #136]
	adds r1, #54
	movs r3, #7
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntry
	ldr r3, [sp, #136]
	movs r1, #243
	ldrh r2, [r3, #8]
	lsls r1, r1, #8
	adds r0, r3, #0
	adds r1, #55
	subs r2, #10
	movs r3, #7
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntry
	b .L_08047d78
.L_0804777c:
	ldr r4, [sp, #92]
	cmp r4, #1
	beq .L_08047784
	b .L_08047ae4
.L_08047784:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r3, [r3]
	movs r0, #0
	movs r1, #1
	adds r3, #150
	negs r1, r1
	str r0, [sp, #56]
	ldr r0, [sp, #136]
	mov r8, r1
	str r3, [sp, #60]
	bl RenderOutput_RedrawSavedRect
	ldr r2, [sp, #64]
	movs r3, #88
	ldrh r3, [r2, r3]
	movs r2, #252
	lsls r2, r2, #6
	adds r2, #255
	adds r5, r2, #0
	ands r5, r3
	mov r9, r8
	movs r1, #0
	cmp r5, #0
	beq .L_080477f4
	ldr r7, [sp, #64]
	ldr r6, [sp, #60]
	adds r7, #88
	adds r4, r2, #0
.L_080477c0:
	adds r0, r5, #0
	str r1, [sp, #12]
	str r4, [sp, #4]
	bl BattleAction_Get
	ldrb r2, [r0, #1]
	movs r3, #128
	ands r3, r2
	ldr r1, [sp, #12]
	ldr r4, [sp, #4]
	cmp r3, #0
	beq .L_080477e2
	strh r5, [r6]
	ldr r3, [sp, #56]
	adds r6, #2
	adds r3, #1
	str r3, [sp, #56]
.L_080477e2:
	adds r1, #1
	cmp r1, #32
	beq .L_080477f4
	adds r7, #4
	ldrh r3, [r7]
	adds r5, r4, #0
	ands r5, r3
	cmp r5, #0
	bne .L_080477c0
.L_080477f4:
	ldr r4, [sp, #56]
	ldr r2, .L_08047824
	ldr r0, [sp, #60]
	lsls r3, r4, #1
	strh r2, [r3, r0]
	str r4, [sp, #76]
	add r1, sp, #436
	movs r3, #3
	ldrsb r3, [r1, r3]
	ldr r4, [sp, #56]
	adds r2, r3, #0
	cmp r4, r3
	bgt .L_08047816
	adds r3, r4, #0
	subs r3, #1
	strb r3, [r1, #3]
	adds r2, r3, #0
.L_08047816:
	lsls r3, r2, #24
	asrs r2, r3, #24
	adds r3, r2, #0
	cmp r2, #0
	bge .L_08047828
	adds r3, r2, #3
	b .L_08047828
.L_08047824:
	.4byte 0x00000000
.L_08047828:
	asrs r3, r3, #2
	lsls r0, r3, #2
	adds r3, r0, #0
	subs r3, r2, r3
	lsls r3, r3, #24
	mov r10, r0
	asrs r5, r3, #24
	cmp r10, r8
	bne .L_08047840
	cmp r5, r9
	bne .L_08047840
	b .L_08047d78
.L_08047840:
	ldr r1, [sp, #140]
	movs r3, #1
	strb r3, [r1, #6]
	bl Ui_FillVramBlockPattern
	ldr r2, [sp, #56]
	cmp r2, #0
	beq .L_08047870
	ldr r0, [sp, #60]
	mov r4, r10
	adds r3, r4, r5
	lsls r3, r3, #1
	ldrh r3, [r3, r0]
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	ldr r3, .L_0804793c
	add r1, sp, #156
	adds r0, r0, r3
	movs r2, #52
	bl UiText_CopyMessageString
	b .L_0804787a
.L_08047870:
	ldr r0, .L_08047940
	add r1, sp, #156
	movs r2, #52
	bl UiText_CopyMessageString
.L_0804787a:
	ldr r0, [sp, #136]
	bl RenderOutput_RedrawSavedRect
	ldr r2, [sp, #128]
	ldr r4, [sp, #128]
	movs r1, #12
	ldrsh r0, [r2, r1]
	movs r3, #14
	ldrsh r1, [r2, r3]
	ldrh r3, [r4, #10]
	ldrh r2, [r2, #8]
	movs r4, #0
	str r4, [sp, #0]
	bl Func_0803a2b0
	movs r2, #0
	add r0, sp, #156
	ldr r1, [sp, #128]
	movs r3, #20
	bl UiText_RenderWideStringAtOffset
	cmp r10, r8
	bne .L_080478aa
	b .L_08047a58
.L_080478aa:
	movs r3, #16
	str r3, [sp, #0]
	ldr r0, [sp, #136]
	movs r1, #0
	movs r2, #16
	movs r3, #29
	bl UiWindow_DrawDividerLine
	ldr r2, [sp, #60]
	mov r1, r10
	lsls r3, r1, #1
	ldrh r5, [r3, r2]
	movs r0, #0
	mov r8, r0
	cmp r5, #0
	bne .L_080478cc
	b .L_08047a02
.L_080478cc:
	movs r3, #64
	str r3, [sp, #16]
	movs r4, #8
	add r7, sp, #488
	mov r9, r4
.L_080478d6:
	adds r0, r5, #0
	bl BattleAction_Get
	movs r1, #240
	adds r6, r0, #0
	lsls r1, r1, #8
	movs r0, #0
	str r0, [sp, #0]
	adds r1, #31
	ldr r0, [sp, #136]
	movs r2, #11
	mov r3, r9
	bl UiWindow_SetTilemapEntry
	movs r1, #0
	str r1, [sp, #0]
	movs r1, #240
	lsls r1, r1, #8
	ldr r0, [sp, #136]
	adds r1, #30
	movs r2, #12
	mov r3, r9
	bl UiWindow_SetTilemapEntry
	mov r4, r8
	add r2, sp, #420
	lsls r3, r4, #2
	ldr r1, [r2, r3]
	adds r0, r5, #0
	bl Resource_LoadIndexedEntryToBuffer
	ldr r3, .L_08047934
	ldr r2, .L_08047938
	ands r0, r3
	ldrh r3, [r7]
	ands r3, r2
	ldrb r2, [r6, #1]
	orrs r3, r0
	strh r3, [r7]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	bne .L_08047944
	movs r0, #4
	bl Func_08041f70
	b .L_0804796c
.L_08047934:
	.4byte 0x000003ff
.L_08047938:
	.4byte 0xfffffc00
.L_0804793c:
	.4byte 0x00000885
.L_08047940:
	.4byte 0x00000d48
.L_08047944:
	ldr r1, [sp, #64]
	ldrb r2, [r6, #9]
	movs r0, #58
	ldrsh r3, [r1, r0]
	cmp r2, r3
	ble .L_08047958
	movs r0, #2
	bl Func_08041f70
	b .L_0804796c
.L_08047958:
	ldr r2, [sp, #64]
	movs r4, #62
	adds r4, #255
	adds r3, r2, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804796c
	movs r0, #9
	bl Func_08041f70
.L_0804796c:
	ldr r1, [sp, #140]
	movs r0, #0
	mov r11, r0
	ldr r0, .L_080479c8
	movs r3, #5
	strb r3, [r1, #7]
	ldr r1, [sp, #136]
	ldr r3, [sp, #16]
	adds r0, r5, r0
	movs r2, #16
	bl UiText_DrawCharacterAtOffset
	ldr r2, [sp, #16]
	ldrb r0, [r6, #9]
	movs r3, #104
	str r2, [sp, #0]
	movs r1, #2
	ldr r2, [sp, #136]
	bl UiText_DrawNumberAtOffset
	movs r0, #15
	bl Func_08041f70
	ldr r4, [sp, #140]
	movs r3, #15
	strb r3, [r4, #7]
	ldrb r3, [r6, #2]
	cmp r3, #4
	beq .L_080479be
	movs r0, #160
	lsls r0, r0, #7
	adds r1, r3, #0
	adds r0, #1
	mov r2, r11
	adds r1, r1, r0
	str r2, [sp, #0]
	ldr r0, [sp, #136]
	movs r2, #15
	mov r3, r9
	bl UiWindow_SetTilemapEntry
.L_080479be:
	ldrb r3, [r6, #8]
	cmp r3, #255
	bne .L_080479cc
	movs r3, #11
	b .L_080479ce
.L_080479c8:
	.4byte 0x000005a7
.L_080479cc:
	subs r3, #1
.L_080479ce:
	mov r2, r9
	movs r4, #0
	movs r1, #16
	ldr r0, [sp, #136]
	str r4, [sp, #0]
	bl UiWindow_DrawThreeTileColumn
	ldr r0, [sp, #16]
	movs r2, #1
	add r8, r2
	adds r0, #16
	movs r1, #2
	mov r3, r8
	str r0, [sp, #16]
	adds r7, #12
	add r9, r1
	cmp r3, #3
	bgt .L_08047a02
	mov r3, r10
	ldr r4, [sp, #60]
	add r3, r8
	lsls r3, r3, #1
	ldrh r5, [r3, r4]
	cmp r5, #0
	beq .L_08047a02
	b .L_080478d6
.L_08047a02:
	mov r0, r8
	cmp r0, #3
	bgt .L_08047a58
	lsls r3, r0, #1
	add r3, r8
	add r1, sp, #740
	lsls r3, r3, #2
	adds r3, r3, r1
	ldr r2, .L_08047a54
	adds r5, r3, #0
	lsls r3, r0, #2
	adds r3, r3, r1
	adds r6, r3, r2
	ldr r7, .L_08047a4c
	movs r3, #4
	subs r0, r3, r0
	subs r5, #252
	mov r8, r0
.L_08047a26:
	movs r1, #1
	ldmia r6!, {r0}
	negs r1, r1
	bl Resource_GetBuffer
	ldr r3, .L_08047a50
	ands r0, r3
	ldrh r3, [r5]
	ands r3, r7
	orrs r3, r0
	strh r3, [r5]
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r4, r8
	adds r5, #12
	cmp r4, #0
	bne .L_08047a26
	b .L_08047a58
.L_08047a4c:
	.4byte 0xfffffc00
.L_08047a50:
	.4byte 0x000003ff
.L_08047a54:
	.4byte 0xfffffec0
.L_08047a58:
	ldr r0, [sp, #56]
	cmp r0, #4
	bgt .L_08047a60
	b .L_08047d6e
.L_08047a60:
	movs r1, #0
	mov r8, r1
	adds r5, r0, #0
	adds r5, #3
	b .L_08047aa2
.L_08047a6a:
	movs r1, #243
	lsls r1, r1, #8
	adds r1, #1
	mov r3, r10
	add r1, r8
	cmp r3, #0
	bge .L_08047a7a
	adds r3, #3
.L_08047a7a:
	asrs r3, r3, #2
	cmp r8, r3
	bne .L_08047a88
	movs r1, #243
	lsls r1, r1, #8
	adds r1, #11
	add r1, r8
.L_08047a88:
	ldr r3, [sp, #136]
	ldrh r2, [r3, #8]
	movs r3, #0
	subs r2, r2, r0
	add r2, r8
	str r3, [sp, #0]
	subs r2, #10
	ldr r0, [sp, #136]
	movs r3, #7
	bl UiWindow_SetTilemapEntry
	movs r4, #1
	add r8, r4
.L_08047aa2:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_08047aac
	ldr r3, [sp, #56]
	adds r3, #6
.L_08047aac:
	asrs r0, r3, #2
	cmp r8, r0
	blt .L_08047a6a
	ldr r1, [sp, #136]
	movs r5, #0
	ldrh r2, [r1, #8]
	movs r1, #243
	subs r2, r2, r0
	lsls r1, r1, #8
	subs r2, #11
	ldr r0, [sp, #136]
	adds r1, #54
	movs r3, #7
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntry
	ldr r3, [sp, #136]
	movs r1, #243
	ldrh r2, [r3, #8]
	lsls r1, r1, #8
	adds r0, r3, #0
	adds r1, #55
	subs r2, #10
	movs r3, #7
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntry
	b .L_08047d6e
.L_08047ae4:
	ldr r0, [sp, #92]
	cmp r0, #2
	beq .L_08047aec
	b .L_08047d78
.L_08047aec:
	movs r2, #1
	movs r1, #0
	negs r2, r2
	ldr r0, [sp, #136]
	mov r10, r1
	mov r11, r2
	mov r9, r1
	bl RenderOutput_RedrawSavedRect
	ldr r4, [sp, #128]
	movs r2, #14
	ldrsh r1, [r4, r2]
	movs r3, #12
	ldrsh r0, [r4, r3]
	ldrh r2, [r4, #8]
	ldrh r3, [r4, #10]
	mov r4, r10
	str r4, [sp, #0]
	bl Func_0803a2b0
	movs r3, #16
	str r3, [sp, #0]
	ldr r0, [sp, #136]
	movs r1, #0
	movs r2, #16
	movs r3, #29
	bl UiWindow_DrawDividerLine
	movs r0, #0
	ldr r7, [sp, #64]
	str r0, [sp, #80]
	mov r8, r0
	adds r7, #248
.L_08047b2e:
	ldr r1, [sp, #80]
	ldr r2, [sp, #84]
	lsls r3, r1, #2
	movs r6, #0
	adds r5, r3, r2
.L_08047b38:
	ldr r3, [r7, #16]
	movs r2, #1
	lsls r2, r6
	ands r3, r2
	cmp r3, #0
	beq .L_08047b54
	mov r4, r8
	lsls r3, r4, #8
	orrs r3, r6
	stmia r5!, {r3}
	ldr r0, [sp, #80]
	adds r0, #1
	str r0, [sp, #80]
	b .L_08047bec
.L_08047b54:
	ldr r3, [r7]
	ands r3, r2
	cmp r3, #0
	beq .L_08047bec
	ldr r1, [sp, #144]
	movs r0, #0
	cmp r1, #7
	bls .L_08047b66
	movs r0, #1
.L_08047b66:
	bl Trade_GetOfferStateFar
	movs r2, #148
	adds r3, r0, #0
	lsls r2, r2, #1
	adds r1, r3, #0
	adds r3, r3, r2
	ldr r3, [r3]
	movs r0, #0
	adds r1, #8
	movs r4, #0
	cmp r0, r3
	bge .L_08047bc4
	ldrb r3, [r1, #2]
	ldr r4, [sp, #144]
	cmp r3, r4
	bne .L_08047b94
	ldrb r3, [r1]
	cmp r3, r8
	bne .L_08047b94
	ldrb r3, [r1, #1]
	cmp r3, r6
	beq .L_08047bbe
.L_08047b94:
	movs r2, #144
	lsls r2, r2, #1
	adds r3, r1, r2
	ldr r3, [r3]
	adds r0, #1
	cmp r0, r3
	bge .L_08047bc2
	lsls r4, r0, #2
	adds r2, r1, r4
	ldrb r3, [r2, #2]
	mov r12, r3
	ldr r3, [sp, #144]
	cmp r12, r3
	bne .L_08047b94
	ldrb r3, [r2]
	cmp r3, r8
	bne .L_08047b94
	ldrb r3, [r2, #1]
	cmp r3, r6
	bne .L_08047b94
	b .L_08047bc4
.L_08047bbe:
	movs r4, #0
	b .L_08047bc4
.L_08047bc2:
	lsls r4, r0, #2
.L_08047bc4:
	mov r0, r8
	lsls r2, r0, #8
	movs r3, #128
	lsls r3, r3, #9
	orrs r2, r6
	orrs r2, r3
	str r2, [r5]
	adds r3, r1, r4
	ldrb r3, [r3, #3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	ble .L_08047be4
	lsls r3, r3, #17
	orrs r2, r3
	str r2, [r5]
.L_08047be4:
	ldr r1, [sp, #80]
	adds r5, #4
	adds r1, #1
	str r1, [sp, #80]
.L_08047bec:
	adds r6, #1
	cmp r6, #19
	ble .L_08047b38
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r7, #4
	cmp r3, #3
	ble .L_08047b2e
	ldr r4, [sp, #80]
	ldr r0, [sp, #84]
	movs r2, #128
	lsls r3, r4, #2
	lsls r2, r2, #24
	str r2, [r3, r0]
	add r1, sp, #436
	movs r3, #4
	ldrsb r3, [r1, r3]
	adds r2, r3, #0
	cmp r3, r4
	blt .L_08047c1e
	adds r3, r4, #0
	subs r3, #1
	strb r3, [r1, #4]
	adds r2, r3, #0
.L_08047c1e:
	ldr r1, [sp, #84]
	lsls r3, r2, #24
	ldr r2, [sp, #140]
	asrs r3, r3, #22
	ldr r5, [r3, r1]
	movs r3, #1
	strb r3, [r2, #6]
	ldr r3, [sp, #80]
	cmp r3, #0
	beq .L_08047c7a
	bl Func_0803cca8
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r5
	cmp r3, #0
	beq .L_08047c58
	movs r0, #248
	lsls r0, r0, #14
	ands r0, r5
	cmp r0, #0
	beq .L_08047c54
	lsrs r0, r0, #17
	movs r1, #5
	bl Func_0803ccd0
	b .L_08047c58
.L_08047c54:
	ldr r0, .L_08047cd8
	b .L_08047c70
.L_08047c58:
	movs r3, #240
	lsls r3, r3, #4
	ands r3, r5
	lsrs r3, r3, #8
	lsls r0, r3, #2
	adds r0, r0, r3
	movs r3, #255
	ands r5, r3
	ldr r3, .L_08047cdc
	lsls r0, r0, #2
	adds r0, r0, r5
	adds r0, r0, r3
.L_08047c70:
	add r1, sp, #156
	movs r2, #32
	bl UiText_CopyMessageString
	b .L_08047c84
.L_08047c7a:
	ldr r0, .L_08047ce0
	add r1, sp, #156
	movs r2, #32
	bl UiText_CopyMessageString
.L_08047c84:
	ldr r0, [sp, #140]
	mov r4, r9
	strb r4, [r0, #6]
	bl Ui_FillVramBlockPattern
	movs r2, #0
	add r0, sp, #156
	ldr r1, [sp, #128]
	movs r3, #20
	bl UiText_RenderWideStringAtOffset
	cmp r10, r11
	beq .L_08047d6e
	ldr r2, [sp, #84]
	movs r1, #0
	mov r8, r1
	str r2, [sp, #20]
	b .L_08047d40
.L_08047ca8:
	movs r1, #240
	lsls r1, r1, #4
	movs r3, #160
	ands r1, r7
	lsls r3, r3, #7
	adds r3, #1
	lsrs r1, r1, #8
	adds r1, r1, r3
	movs r3, #0
	str r3, [sp, #0]
	ldr r0, [sp, #136]
	adds r3, r5, #0
	adds r2, r6, #0
	bl UiWindow_SetTilemapEntry
	movs r3, #248
	lsls r3, r3, #14
	ands r3, r7
	cmp r3, #0
	beq .L_08047ce4
	movs r0, #4
	bl Func_08041f70
	b .L_08047cf4
.L_08047cd8:
	.4byte 0x00000cf9
.L_08047cdc:
	.4byte 0x000009b1
.L_08047ce0:
	.4byte 0x00000d4e
.L_08047ce4:
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r7
	cmp r3, #0
	beq .L_08047cf4
	movs r0, #2
	bl Func_08041f70
.L_08047cf4:
	movs r3, #240
	lsls r3, r3, #4
	ands r3, r7
	lsrs r3, r3, #8
	lsls r0, r3, #2
	adds r0, r0, r3
	movs r3, #255
	ands r3, r7
	lsls r0, r0, #2
	adds r0, r0, r3
	ldr r3, .L_0804804c
	lsls r6, r6, #3
	adds r2, r6, #0
	lsls r5, r5, #3
	adds r0, r0, r3
	adds r2, #8
	ldr r1, [sp, #136]
	adds r3, r5, #0
	bl UiText_DrawCharacterAtOffset
	movs r0, #248
	lsls r0, r0, #14
	ands r0, r7
	cmp r0, #0
	beq .L_08047d36
	adds r3, r6, #0
	lsrs r0, r0, #17
	adds r3, #40
	movs r1, #2
	ldr r2, [sp, #136]
	str r5, [sp, #0]
	bl UiText_DrawNumberAtOffset
.L_08047d36:
	movs r0, #15
	bl Func_08041f70
	movs r4, #1
	add r8, r4
.L_08047d40:
	mov r0, r8
	cmp r0, #8
	bgt .L_08047d6e
	ldr r2, [sp, #20]
	ldmia r2!, {r7}
	adds r1, r2, #0
	str r1, [sp, #20]
	movs r1, #3
	bl __divsi3
	lsls r3, r0, #3
	subs r6, r3, r0
	movs r1, #3
	mov r0, r8
	bl __modsi3
	movs r3, #128
	lsls r0, r0, #1
	adds r5, r0, #0
	lsls r3, r3, #24
	adds r5, #9
	cmp r7, r3
	bne .L_08047ca8
.L_08047d6e:
	ldr r4, [sp, #140]
	movs r2, #0
	movs r3, #1
	strb r3, [r4, #3]
	strb r2, [r4, #6]
.L_08047d78:
	ldr r0, [sp, #132]
	cmp r0, #0
	bne .L_08047d80
	b .L_080482b2
.L_08047d80:
	ldr r1, [sp, #92]
	cmp r1, #0
	bne .L_08047db2
	ldr r0, [sp, #136]
	bl RenderOutput_RedrawSavedRect
	ldr r3, [sp, #128]
	movs r4, #14
	ldrsh r1, [r3, r4]
	ldr r4, [sp, #92]
	movs r2, #12
	ldrsh r0, [r3, r2]
	ldrh r2, [r3, #8]
	ldrh r3, [r3, #10]
	str r4, [sp, #0]
	bl Func_0803a2b0
	movs r3, #14
	str r3, [sp, #0]
	ldr r0, [sp, #136]
	movs r1, #0
	movs r2, #14
	movs r3, #29
	bl UiWindow_DrawDividerLine
.L_08047db2:
	ldr r0, [sp, #64]
	ldr r1, [sp, #136]
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringAtOffset
	ldr r0, [sp, #124]
	cmp r0, #3
	ble .L_08047dd0
	ldr r0, .L_08048050
	ldr r1, [sp, #136]
	movs r2, #40
	movs r3, #16
	bl UiText_DrawCharacterAtOffset
.L_08047dd0:
	ldr r1, [sp, #136]
	ldr r0, .L_08048054
	movs r2, #56
	movs r3, #0
	bl UiText_DrawStringAtOffset
	ldr r1, [sp, #64]
	movs r3, #0
	ldr r2, [sp, #136]
	ldrb r0, [r1, #15]
	str r3, [sp, #0]
	movs r1, #2
	movs r3, #72
	bl UiText_DrawNumberInWindow
	ldr r7, .L_08048058
	ldr r1, [sp, #136]
	adds r0, r7, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	ldr r2, [sp, #64]
	movs r4, #146
	lsls r4, r4, #1
	adds r3, r2, r4
	ldr r0, [r3]
	movs r3, #8
	str r3, [sp, #0]
	ldr r2, [sp, #136]
	movs r1, #8
	movs r3, #40
	bl UiText_DrawNumberInWindow
	ldr r0, .L_0804805c
	ldr r1, [sp, #136]
	movs r2, #40
	movs r3, #24
	bl UiText_DrawStringAtOffset
	ldr r2, [sp, #64]
	movs r3, #24
	movs r1, #56
	ldrsh r0, [r2, r1]
	mov r8, r3
	str r3, [sp, #0]
	ldr r2, [sp, #136]
	movs r1, #4
	movs r3, #56
	bl UiText_DrawNumberInWindow
	ldr r5, .L_08048060
	ldr r1, [sp, #136]
	adds r0, r5, #0
	movs r2, #88
	movs r3, #24
	bl UiText_DrawStringAtOffset
	ldr r1, [sp, #64]
	mov r2, r8
	movs r4, #52
	ldrsh r0, [r1, r4]
	movs r3, #96
	str r2, [sp, #0]
	movs r1, #4
	ldr r2, [sp, #136]
	bl UiText_DrawNumberInWindow
	movs r3, #32
	ldr r1, [sp, #136]
	ldr r0, .L_08048064
	movs r2, #40
	bl UiText_DrawStringAtOffset
	ldr r4, [sp, #64]
	ldr r2, [sp, #136]
	movs r3, #58
	ldrsh r0, [r4, r3]
	movs r6, #32
	movs r1, #4
	movs r3, #56
	str r6, [sp, #0]
	bl UiText_DrawNumberInWindow
	adds r0, r5, #0
	ldr r1, [sp, #136]
	movs r2, #88
	movs r3, #32
	bl UiText_DrawStringAtOffset
	ldr r2, [sp, #64]
	movs r3, #96
	movs r1, #54
	ldrsh r0, [r2, r1]
	ldr r2, [sp, #136]
	movs r1, #4
	str r6, [sp, #0]
	bl UiText_DrawNumberInWindow
	adds r0, r7, #0
	ldr r1, [sp, #136]
	subs r0, #10
	movs r2, #136
	movs r3, #16
	bl UiText_DrawCharacterAtOffset
	ldr r3, [sp, #64]
	ldr r2, [sp, #136]
	ldrh r0, [r3, #60]
	movs r3, #16
	str r3, [sp, #0]
	movs r1, #3
	movs r3, #184
	bl UiText_DrawNumberInWindow
	adds r0, r7, #0
	ldr r1, [sp, #136]
	subs r0, #9
	movs r2, #136
	movs r3, #24
	bl UiText_DrawCharacterAtOffset
	ldr r4, [sp, #64]
	mov r1, r8
	ldrh r0, [r4, #62]
	ldr r2, [sp, #136]
	str r1, [sp, #0]
	movs r3, #184
	movs r1, #3
	bl UiText_DrawNumberInWindow
	adds r0, r7, #0
	ldr r1, [sp, #136]
	subs r0, #8
	movs r2, #136
	movs r3, #32
	bl UiText_DrawCharacterAtOffset
	ldr r3, [sp, #64]
	ldr r2, [sp, #136]
	adds r3, #64
	ldrh r0, [r3]
	movs r1, #3
	movs r3, #184
	str r6, [sp, #0]
	bl UiText_DrawNumberInWindow
	subs r0, r7, #7
	ldr r1, [sp, #136]
	movs r2, #136
	movs r3, #40
	bl UiText_DrawCharacterAtOffset
	ldr r3, [sp, #64]
	ldr r2, [sp, #136]
	adds r3, #66
	ldrb r0, [r3]
	movs r3, #40
	str r3, [sp, #0]
	movs r1, #3
	movs r3, #184
	bl UiText_DrawNumberInWindow
	ldr r2, [sp, #64]
	movs r4, #42
	adds r4, #255
	adds r3, r2, r4
	ldrb r0, [r3]
	ldr r3, .L_08048068
	ldr r1, [sp, #136]
	adds r0, r0, r3
	movs r2, #0
	movs r3, #48
	bl UiText_DrawCharacterAtOffset
	ldr r0, [sp, #92]
	cmp r0, #0
	bne .L_08048032
	ldr r1, [sp, #100]
	cmp r1, #0
	beq .L_08047f46
	subs r0, r7, #1
	ldr r1, [sp, #136]
	movs r2, #0
	movs r3, #72
	bl UiText_DrawCharacterAtOffset
.L_08047f46:
	ldr r1, [sp, #136]
	subs r0, r7, #5
	movs r2, #0
	movs r3, #80
	bl UiText_DrawCharacterAtOffset
	subs r0, r7, #4
	ldr r1, [sp, #136]
	movs r2, #0
	movs r3, #88
	bl UiText_DrawCharacterAtOffset
	subs r0, r7, #3
	ldr r1, [sp, #136]
	movs r2, #0
	movs r3, #96
	bl UiText_DrawCharacterAtOffset
	ldr r0, [sp, #64]
	movs r2, #0
	movs r1, #140
	mov r8, r2
	movs r3, #72
	movs r4, #40
	lsls r1, r1, #1
	movs r2, #7
	mov r11, r3
	mov r10, r4
	adds r6, r0, r1
	movs r7, #48
	mov r9, r2
.L_08047f84:
	ldr r4, [sp, #100]
	movs r3, #1
	cmp r4, #0
	beq .L_08047f8e
	movs r3, #0
.L_08047f8e:
	movs r1, #160
	lsls r1, r1, #7
	mov r0, r8
	movs r2, #0
	adds r1, #1
	add r1, r8
	str r2, [sp, #0]
	lsls r5, r0, #2
	adds r3, #8
	ldr r0, [sp, #136]
	mov r2, r9
	bl UiWindow_SetTilemapEntry
	ldr r1, [sp, #100]
	cmp r1, #0
	beq .L_08047fdc
	mov r2, r11
	ldrb r0, [r6, #4]
	movs r1, #1
	str r2, [sp, #0]
	mov r3, r10
	ldr r2, [sp, #136]
	bl UiText_DrawNumberInWindow
	ldr r0, .L_08048060
	ldr r1, [sp, #136]
	adds r2, r7, #0
	movs r3, #72
	bl UiText_DrawStringAtOffset
	adds r3, r7, #0
	ldrb r0, [r6]
	mov r4, r11
	adds r3, #8
	movs r1, #1
	ldr r2, [sp, #136]
	str r4, [sp, #0]
	bl UiText_DrawNumberInWindow
.L_08047fdc:
	ldr r0, [sp, #144]
	mov r1, r8
	bl Func_080ad1a0
	movs r3, #80
	ldr r2, [sp, #136]
	str r3, [sp, #0]
	movs r1, #2
	adds r3, r7, #0
	bl UiText_DrawNumberInWindow
	ldr r1, [sp, #64]
	adds r5, #72
	movs r3, #88
	ldrsh r0, [r1, r5]
	str r3, [sp, #0]
	ldr r2, [sp, #136]
	mov r3, r10
	movs r1, #3
	bl UiText_DrawNumberInWindow
	ldr r3, [sp, #64]
	movs r1, #3
	adds r5, r3, r5
	movs r3, #96
	movs r4, #2
	ldrsh r0, [r5, r4]
	ldr r2, [sp, #136]
	str r3, [sp, #0]
	mov r3, r10
	bl UiText_DrawNumberInWindow
	movs r2, #1
	add r8, r2
	movs r0, #32
	movs r1, #4
	mov r3, r8
	add r10, r0
	adds r6, #1
	adds r7, #32
	add r9, r1
	cmp r3, #3
	ble .L_08047f84
.L_08048032:
	ldr r0, [sp, #64]
	movs r6, #0
	movs r4, #56
	ldrsh r3, [r0, r4]
	cmp r3, #0
	bne .L_08048046
	ldr r1, [sp, #40]
	movs r3, #16
	strb r3, [r1]
	movs r6, #1
.L_08048046:
	ldr r3, [sp, #40]
	adds r2, r6, r3
	b .L_08048200
.L_0804804c:
	.4byte 0x000006d3
.L_08048050:
	.4byte 0x00000d1f
.L_08048054:
	.4byte Data_0805f888
.L_08048058:
	.4byte 0x00000d1a
.L_0804805c:
	.4byte Data_0805f88c
.L_08048060:
	.4byte Data_0805f890
.L_08048064:
	.4byte Data_0805f894
.L_08048068:
	.4byte 0x00000b63
.L_0804806c:
	ldr r4, [sp, #64]
	movs r0, #50
	adds r0, #255
	adds r1, r4, r0
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r3, #1
	bne .L_08048082
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_08048082:
	cmp r6, #7
	ble .L_08048088
	b .L_0804823a
.L_08048088:
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r3, #2
	bne .L_08048096
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_08048096:
	cmp r6, #7
	ble .L_0804809c
	b .L_0804823a
.L_0804809c:
	ldr r1, [sp, #64]
	movs r4, #62
	adds r4, #255
	adds r3, r1, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080480b2
	movs r3, #4
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_080480b2:
	cmp r6, #7
	ble .L_080480b8
	b .L_0804823a
.L_080480b8:
	ldr r0, [sp, #64]
	movs r1, #60
	adds r1, #255
	adds r3, r0, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080480ce
	movs r3, #3
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_080480ce:
	cmp r6, #7
	ble .L_080480d4
	b .L_0804823a
.L_080480d4:
	ldr r4, [sp, #64]
	movs r0, #158
	lsls r0, r0, #1
	adds r3, r4, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080480ea
	movs r3, #5
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_080480ea:
	cmp r6, #7
	ble .L_080480f0
	b .L_0804823a
.L_080480f0:
	ldr r1, [sp, #64]
	movs r4, #160
	lsls r4, r4, #1
	adds r3, r1, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08048106
	movs r3, #7
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_08048106:
	cmp r6, #7
	ble .L_0804810c
	b .L_0804823a
.L_0804810c:
	ldr r0, [sp, #64]
	movs r1, #156
	lsls r1, r1, #1
	adds r3, r0, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08048122
	movs r3, #6
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_08048122:
	cmp r6, #7
	ble .L_08048128
	b .L_0804823a
.L_08048128:
	ldr r4, [sp, #64]
	movs r0, #153
	lsls r0, r0, #1
	adds r3, r4, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804815e
	movs r1, #52
	adds r1, #255
	adds r3, r4, r1
	ldrb r1, [r3]
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	ble .L_08048150
	movs r3, #9
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_08048150:
	lsls r3, r1, #24
	cmp r3, #0
	bge .L_0804815e
	movs r3, #10
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_0804815e:
	cmp r6, #7
	bgt .L_0804823a
	ldr r4, [sp, #64]
	movs r0, #154
	lsls r0, r0, #1
	adds r3, r4, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08048198
	movs r1, #54
	adds r1, #255
	adds r3, r4, r1
	ldrb r1, [r3]
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	ble .L_0804818a
	movs r3, #11
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_0804818a:
	lsls r3, r1, #24
	cmp r3, #0
	bge .L_08048198
	movs r3, #12
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_08048198:
	cmp r6, #7
	bgt .L_0804823a
	ldr r4, [sp, #64]
	movs r0, #155
	lsls r0, r0, #1
	adds r3, r4, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080481d2
	movs r1, #56
	adds r1, #255
	adds r3, r4, r1
	ldrb r1, [r3]
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	ble .L_080481c4
	movs r3, #13
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_080481c4:
	lsls r3, r1, #24
	cmp r3, #0
	bge .L_080481d2
	movs r3, #14
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_080481d2:
	cmp r6, #7
	bgt .L_0804823a
	ldr r4, [sp, #64]
	movs r0, #72
	adds r0, #255
	adds r3, r4, r0
	ldrb r1, [r3]
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	ble .L_080481f2
	movs r3, #17
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_080481f2:
	lsls r3, r1, #24
	cmp r3, #0
	bge .L_0804823a
	movs r3, #18
	strb r3, [r2]
	adds r6, #1
	b .L_0804823a
.L_08048200:
	ldr r1, [sp, #64]
	movs r4, #152
	lsls r4, r4, #1
	adds r3, r1, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_0804821a
	movs r3, #15
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_0804821a:
	cmp r6, #7
	bgt .L_0804823a
	ldr r0, [sp, #64]
	movs r1, #66
	adds r1, #255
	adds r3, r0, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08048234
	movs r3, #8
	strb r3, [r2]
	adds r6, #1
	adds r2, #1
.L_08048234:
	cmp r6, #7
	bgt .L_0804823a
	b .L_0804806c
.L_0804823a:
	cmp r6, #0
	ble .L_0804825e
	ldr r7, [sp, #36]
	ldr r5, [sp, #40]
	mov r8, r6
.L_08048244:
	ldrb r0, [r5]
	ldmia r7!, {r1}
	lsls r0, r0, #24
	asrs r0, r0, #24
	bl Resource_LoadTableEntryToBuffer
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r3, r8
	adds r5, #1
	cmp r3, #0
	bne .L_08048244
.L_0804825e:
	cmp r6, #0
	bne .L_08048268
	ldr r4, [sp, #40]
	strb r6, [r4]
	movs r6, #1
.L_08048268:
	cmp r6, #10
	bgt .L_0804828c
	ldr r3, [sp, #40]
	movs r0, #1
	adds r2, r6, r3
	movs r3, #11
	negs r0, r0
	subs r3, r3, r6
	adds r1, r0, #0
	mov r8, r3
.L_0804827c:
	movs r4, #1
	negs r4, r4
	add r8, r4
	mov r0, r8
	strb r1, [r2]
	adds r2, #1
	cmp r0, #0
	bne .L_0804827c
.L_0804828c:
	str r6, [sp, #104]
	ldr r1, [sp, #40]
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r3, #0
	bne .L_080482b2
	ldr r4, [sp, #64]
	movs r2, #56
	ldrsh r3, [r4, r2]
	cmp r3, #0
	beq .L_080482a6
	ldr r0, .L_0804842c
	b .L_080482a8
.L_080482a6:
	ldr r0, .L_08048430
.L_080482a8:
	ldr r1, [sp, #136]
	movs r2, #112
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
.L_080482b2:
	ldr r0, [sp, #92]
	cmp r0, #0
	beq .L_080482ba
	b .L_08048464
.L_080482ba:
	movs r0, #128
	lsls r0, r0, #1
	bl Runtime_BumpAllocate
	ldr r3, [sp, #444]
	str r0, [sp, #52]
	cmp r3, #13
	bhi .L_080482cc
	b .L_080483f6
.L_080482cc:
	ldr r1, [sp, #40]
	subs r3, #14
	ldrsb r3, [r1, r3]
	str r3, [sp, #48]
	cmp r3, #0
	bne .L_080482e6
	ldr r4, [sp, #64]
	movs r2, #56
	ldrsh r3, [r4, r2]
	cmp r3, #0
	bne .L_080482e6
	movs r0, #16
	str r0, [sp, #48]
.L_080482e6:
	movs r1, #166
	lsls r1, r1, #1
	mov r8, r1
	mov r0, r8
	bl Runtime_BumpAllocate
	mov r2, r8
	ldr r1, [sp, #64]
	ldr r3, .L_08048434
	adds r6, r0, #0
	mov lr, r3
	.2byte 0xf800
	ldr r4, [sp, #64]
	ldr r0, [sp, #64]
	ldr r5, [sp, #64]
	ldrh r4, [r4, #60]
	ldrh r0, [r0, #62]
	adds r5, #64
	ldr r2, [sp, #64]
	ldrh r1, [r5]
	mov r10, r4
	movs r4, #52
	mov r11, r0
	adds r4, #255
	movs r0, #54
	adds r3, r2, r4
	movs r7, #0
	mov r9, r1
	adds r0, #255
	movs r1, #72
	strb r7, [r3]
	adds r1, #255
	adds r3, r2, r0
	strb r7, [r3]
	adds r3, r2, r1
	strb r7, [r3]
	ldr r0, [sp, #144]
	bl Owner_RecalculateStatsFar
	ldr r2, [sp, #64]
	mov r4, r10
	ldrh r3, [r2, #60]
	mov r0, r11
	subs r4, r4, r3
	ldrh r3, [r2, #62]
	mov r1, r9
	subs r0, r0, r3
	ldrh r3, [r5]
	mov r11, r0
	subs r1, r1, r3
	adds r0, r2, #0
	ldr r3, .L_08048434
	mov r2, r8
	mov r9, r1
	adds r1, r6, #0
	mov r10, r4
	mov lr, r3
	.2byte 0xf800
	adds r0, r6, #0
	bl Sys_Free
	ldr r3, [sp, #48]
	subs r3, #8
	cmp r3, #10
	bhi .L_080483de
	ldr r2, .L_08048438
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08048370:
	.4byte .L_0804839c
	.4byte .L_080483a8
	.4byte .L_080483ac
	.4byte .L_080483b2
	.4byte .L_080483b6
	.4byte .L_080483c6
	.4byte .L_080483c6
	.4byte .L_080483de
	.4byte .L_080483de
	.4byte .L_080483bc
	.4byte .L_080483c0
.L_0804839c:
	ldr r4, [sp, #64]
	movs r0, #66
	adds r0, #255
	adds r3, r4, r0
	ldrb r7, [r3]
	b .L_080483de
.L_080483a8:
	mov r7, r10
	b .L_080483de
.L_080483ac:
	mov r1, r10
	negs r7, r1
	b .L_080483de
.L_080483b2:
	mov r7, r11
	b .L_080483de
.L_080483b6:
	mov r2, r11
	negs r7, r2
	b .L_080483de
.L_080483bc:
	mov r7, r9
	b .L_080483de
.L_080483c0:
	mov r3, r9
	negs r7, r3
	b .L_080483de
.L_080483c6:
	ldr r4, [sp, #64]
	movs r0, #56
	adds r0, #255
	adds r3, r4, r0
	movs r2, #0
	ldrsb r2, [r3, r2]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r7, r3, #2
	cmp r7, #0
	bge .L_080483de
	negs r7, r7
.L_080483de:
	adds r0, r7, #0
	movs r1, #5
	bl Func_0803ccd0
	ldr r0, .L_0804843c
	movs r2, #128
	ldr r1, [sp, #48]
	adds r0, r1, r0
	ldr r1, [sp, #52]
	bl UiText_CopyMessageString
	b .L_08048452
.L_080483f6:
	cmp r3, #2
	bne .L_08048444
	ldr r2, [sp, #64]
	ldrb r3, [r2, #15]
	cmp r3, #98
	bhi .L_08048444
	adds r1, r3, #0
	adds r1, #1
	ldr r0, [sp, #144]
	bl Owner_GetLevelThresholdFar
	ldr r4, [sp, #64]
	movs r1, #146
	lsls r1, r1, #1
	adds r3, r4, r1
	ldr r3, [r3]
	movs r1, #5
	subs r0, r0, r3
	bl Func_0803ccd0
	ldr r0, .L_08048440
	ldr r1, [sp, #52]
	movs r2, #128
	bl UiText_CopyMessageString
	b .L_08048452
	.2byte 0x0000
.L_0804842c:
	.4byte 0x00000d1d
.L_08048430:
	.4byte 0x00000d1e
.L_08048434:
	.4byte IwramCopyWords
.L_08048438:
	.4byte .L_08048370
.L_0804843c:
	.4byte 0x00000d33
.L_08048440:
	.4byte 0x00000d20
.L_08048444:
	ldr r0, [sp, #444]
	ldr r3, .L_080484c8
	ldr r1, [sp, #52]
	adds r0, r0, r3
	movs r2, #128
	bl UiText_CopyMessageString
.L_08048452:
	ldr r0, [sp, #52]
	ldr r1, [sp, #128]
	movs r2, #0
	movs r3, #4
	bl UiText_RenderWideStringAtOffset
	ldr r0, [sp, #52]
	bl Sys_Free
.L_08048464:
	ldr r4, [sp, #140]
	movs r2, #0
	movs r3, #1
	str r2, [sp, #132]
	strb r3, [r4, #3]
.L_0804846e:
	ldr r0, .L_080484cc
	ldr r5, [sp, #44]
	ldr r7, [sp, #36]
	movs r1, #112
	movs r6, #0
	mov r10, r0
	mov r8, r1
.L_0804847c:
	ldr r3, .L_080484d0
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #8]
	ldmia r7!, {r3}
	ldr r1, .L_080484bc
	lsls r3, r3, #2
	add r3, r10
	ldrh r2, [r3, #2]
	ldrh r3, [r5, #8]
	lsls r2, r2, #17
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
	ldr r3, .L_080484c0
	mov r2, r8
	ands r2, r3
	ldr r4, .L_080484c4
	ldrh r3, [r5, #6]
	ands r3, r4
	orrs r3, r2
	strh r3, [r5, #6]
	ldr r0, [sp, #136]
	movs r2, #14
	ldrsh r3, [r0, r2]
	lsls r3, r3, #3
	adds r3, #8
	strb r3, [r5, #4]
	ldr r1, [sp, #40]
	b .L_080484d4
	.2byte 0x0000
.L_080484bc:
	.4byte 0xfffffc00
.L_080484c0:
	.4byte 0x000001ff
.L_080484c4:
	.4byte 0xfffffe00
.L_080484c8:
	.4byte 0x00000d21
.L_080484cc:
	.4byte ResourceTableEntries
.L_080484d0:
	.4byte 0x40000400
.L_080484d4:
	ldrsb r3, [r1, r6]
	cmp r3, #0
	ble .L_080484e6
	adds r0, r5, #0
	movs r1, #240
	str r4, [sp, #4]
	bl Runtime_PushSlotEntry
	ldr r4, [sp, #4]
.L_080484e6:
	movs r2, #15
	adds r6, #1
	add r8, r2
	adds r5, #12
	cmp r6, #10
	ble .L_0804847c
	ldr r3, [sp, #92]
	cmp r3, #1
	beq .L_080484fc
	cmp r3, #3
	bne .L_08048578
.L_080484fc:
	ldr r0, .L_0804853c
	movs r6, #0
	mov r8, r0
	add r5, sp, #480
	movs r7, #0
.L_08048506:
	ldr r3, .L_08048540
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #8]
	add r2, sp, #420
	lsls r3, r6, #2
	ldr r3, [r2, r3]
	ldr r1, .L_08048534
	lsls r3, r3, #2
	add r3, r8
	ldrh r2, [r3, #2]
	ldrh r3, [r5, #8]
	lsls r2, r2, #17
	ands r3, r1
	lsrs r2, r2, #22
	orrs r3, r2
	strh r3, [r5, #8]
	ldr r1, [sp, #92]
	cmp r1, #3
	bne .L_08048544
	ldrh r3, [r5, #6]
	ldr r2, .L_08048538
	b .L_08048548
.L_08048534:
	.4byte 0xfffffc00
.L_08048538:
	.4byte 0x00000018
.L_0804853c:
	.4byte ResourceTableEntries
.L_08048540:
	.4byte 0x40000400
.L_08048544:
	ldrh r3, [r5, #6]
	ldr r2, .L_08048574
.L_08048548:
	ands r3, r4
	orrs r3, r2
	strh r3, [r5, #6]
	ldr r0, [sp, #136]
	movs r1, #240
	movs r2, #14
	ldrsh r3, [r0, r2]
	adds r0, r5, #0
	adds r3, r7, r3
	lsls r3, r3, #3
	adds r3, #68
	strb r3, [r5, #4]
	str r4, [sp, #4]
	bl Runtime_PushSlotEntry
	adds r6, #1
	adds r5, #12
	adds r7, #2
	ldr r4, [sp, #4]
	cmp r6, #3
	ble .L_08048506
	b .L_08048578
.L_08048574:
	.4byte 0x00000008
.L_08048578:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r3, [r3]
	ldr r3, [r3, #76]
	cmp r3, #0
	beq .L_08048602
	ldr r3, .L_08048688
	movs r2, #2
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_08048602
	ldr r1, [sp, #148]
	cmp r1, #0
	beq .L_080485f8
	ldr r2, [sp, #68]
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080485c8
	ldr r3, [sp, #124]
	adds r3, #1
	str r3, [sp, #124]
	cmp r3, r1
	blt .L_080485b2
	movs r4, #0
	str r4, [sp, #124]
.L_080485b2:
	ldr r0, [sp, #124]
	ldr r1, [sp, #152]
	lsls r3, r0, #1
	ldrh r3, [r3, r1]
	movs r2, #1
	movs r0, #111
	str r3, [sp, #144]
	str r2, [sp, #132]
	bl Audio_PlayCue
	b .L_080485f8
.L_080485c8:
	ldr r4, [sp, #68]
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r4
	cmp r3, #0
	beq .L_080485f8
	ldr r0, [sp, #124]
	subs r0, #1
	str r0, [sp, #124]
	cmp r0, #0
	bge .L_080485e4
	ldr r1, [sp, #148]
	subs r1, #1
	str r1, [sp, #124]
.L_080485e4:
	ldr r2, [sp, #124]
	ldr r4, [sp, #152]
	lsls r3, r2, #1
	ldrh r3, [r3, r4]
	movs r0, #1
	str r0, [sp, #132]
	movs r0, #111
	str r3, [sp, #144]
	bl Audio_PlayCue
.L_080485f8:
	movs r0, #1
	bl WaitFrames
	bl .L_08046ca8
.L_08048602:
	add r5, sp, #420
	movs r6, #3
.L_08048606:
	ldmia r5!, {r0}
	subs r6, #1
	bl Resource_ResetEntry
	cmp r6, #0
	bge .L_08048606
	ldr r5, [sp, #36]
	movs r6, #10
.L_08048616:
	ldmia r5!, {r0}
	subs r6, #1
	bl Resource_ResetEntry
	cmp r6, #0
	bge .L_08048616
	ldr r0, [sp, #108]
	bl Resource_ResetEntry
	ldr r0, [sp, #120]
	bl Resource_ResetEntry
	movs r0, #1
	bl WaitFrames
	bl Func_08041b68
	movs r1, #1
	ldr r0, [sp, #136]
	bl UiWork_Finalize
	movs r5, #192
	ldr r0, [sp, #128]
	movs r1, #1
	lsls r5, r5, #18
	bl UiWork_Finalize
	ldr r3, [r5, #36]
	adds r5, #228
	adds r3, #65
	ldrb r0, [r3]
	bl Func_0804297c
	ldr r2, [r5]
	movs r3, #0
	str r3, [r2, #72]
	ldr r0, [sp, #96]
	bl ReleaseBattleObjectRecordsFar
	bl BattleActor_CommitPlacementFar
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #84]
	bl Sys_Free
	movs r3, #185
	lsls r3, r3, #2
	movs r0, #0
	add sp, r3
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08048688:
	.4byte gInput
