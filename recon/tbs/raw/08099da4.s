.syntax unified
	.thumb
	.global Func_08099da4
	.thumb_func
Func_08099da4:
	.global Battle_unk3_2
	.thumb_func
Battle_unk3_2:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #776]
	ldr	r3, [r3, #0]
	ldr	r0, [r3, #16]
	sub	sp, #124
	str	r0, [sp, #28]
	ldr	r7, [r3, #20]
	mov	fp, r3
	cmp	r7, #0
	bne.n	.L_08099dc6
	b.n	.L_0809a268
.L_08099dc6:
	bl	BattleEffect_InitializeSharedScene
	ldr	r1, [sp, #28]
	str	r7, [r1, #104]
	ldr	r0, [sp, #28]
	ldr	r1, [pc, #748]
	bl	Engine_ObjectSetScript
	mov	r3, fp
	ldr	r0, [r3, #4]
	add	r2, sp, #32
	str	r0, [r2, #0]
	mov	sl, r2
	ldr	r1, [r3, #8]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r1, r1, r2
	mov	r3, sl
	str	r1, [r3, #4]
	mov	r3, fp
	ldr	r2, [r3, #12]
	mov	r3, sl
	str	r2, [r3, #8]
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #8
	bl	BattleFx_SpawnItemBreakMode1
	mov	r1, sl
	adds	r6, r0, #0
	ldr	r2, [pc, #700]
	ldr	r0, [r1, #0]
	mov	r3, sl
	adds	r0, r0, r2
	ldr	r1, [r1, #4]
	ldr	r2, [r3, #8]
	movs	r3, #0
	str	r6, [sp, #20]
	bl	BattleFx_SpawnItemBreakMode1
	adds	r5, r0, #0
	ldr	r0, [sp, #20]
	str	r5, [sp, #24]
	cmp	r0, #0
	beq.n	.L_08099e28
	cmp	r5, #0
	bne.n	.L_08099e2e
.L_08099e28:
	bl	BattleFx_PrepareBufferInterpolation
	b.n	.L_0809a268
.L_08099e2e:
	movs	r0, #15
	bl	WaitFrames
	ldr	r1, [r7, #8]
	mov	r2, sl
	str	r1, [r2, #0]
	ldr	r2, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r2, r2, r3
	mov	r0, sl
	str	r2, [r0, #4]
	ldr	r3, [r7, #16]
	str	r3, [r0, #8]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r1, r1, r0
	adds	r0, r6, #0
	bl	Object_SetPosition
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldr	r3, [pc, #620]
	mov	r0, sl
	adds	r1, r1, r3
	ldr	r2, [r2, #4]
	ldr	r3, [r0, #8]
	adds	r0, r5, #0
	bl	Object_SetPosition
	adds	r0, r6, #0
	bl	Object_CommitPosition
	adds	r0, r5, #0
	bl	Object_CommitPosition
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	movs	r2, #0
	str	r2, [r6, #36]
	str	r3, [r6, #8]
	ldr	r0, [pc, #576]
	ldr	r3, [r1, #0]
	adds	r3, r3, r0
	str	r3, [r5, #8]
	ldr	r3, [pc, #572]
	movs	r1, #200
	str	r2, [r5, #36]
	str	r3, [r7, #108]
	lsls	r1, r1, #4
	ldr	r0, [pc, #564]
	bl	Scheduler_AddOrUpdateCallback
	movs	r0, #130
	bl	Audio_PlayCue
	adds	r1, r7, #0
	adds	r1, #85
	movs	r3, #4
	str	r1, [sp, #16]
	adds	r0, r7, #0
	strb	r3, [r1, #0]
	movs	r1, #0
	bl	ObjectDispatch_SetSingleChildField26Far
	ldr	r2, [sp, #20]
	cmp	r2, #0
	beq.n	.L_08099efe
	ldr	r3, [sp, #24]
	cmp	r3, #0
	beq.n	.L_08099efe
	ldr	r3, [r7, #12]
	ldr	r2, [r7, #20]
	movs	r0, #192
	subs	r3, r3, r2
	lsls	r0, r0, #13
	cmp	r3, r0
	bgt.n	.L_08099efe
	movs	r1, #192
	lsls	r1, r1, #7
.L_08099ed4:
	ldr	r3, [r6, #12]
	adds	r3, r3, r1
	str	r3, [r6, #12]
	ldr	r3, [r5, #12]
	adds	r3, r3, r1
	str	r3, [r5, #12]
	ldr	r3, [r7, #12]
	adds	r3, r3, r1
	str	r3, [r7, #12]
	movs	r0, #1
	str	r1, [sp, #0]
	bl	WaitFrames
	ldr	r2, [r7, #20]
	ldr	r3, [r7, #12]
	subs	r3, r3, r2
	movs	r2, #192
	lsls	r2, r2, #13
	ldr	r1, [sp, #0]
	cmp	r3, r2
	ble.n	.L_08099ed4
.L_08099efe:
	ldr	r3, [sp, #20]
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #8
	ldr	r0, [sp, #24]
	str	r1, [r3, #48]
	str	r2, [r3, #52]
	ldr	r3, [pc, #452]
	str	r1, [r0, #48]
	str	r2, [r0, #52]
	adds	r1, r7, #0
	ldr	r2, [pc, #448]
	str	r3, [r7, #48]
	adds	r1, #90
	movs	r3, #0
	str	r2, [r7, #52]
	strb	r3, [r1, #0]
	adds	r3, r7, #0
	adds	r3, #34
	str	r3, [sp, #12]
	ldr	r1, [sp, #12]
	movs	r3, #2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r5, sl
	mov	r9, r3
	str	r0, [sp, #8]
	b.n	.L_0809a0e8
.L_08099f3a:
	ldr	r3, [pc, #416]
	ldr	r0, [r3, #0]
	bl	BattleFx_GetCycledTableWord
	lsls	r0, r0, #16
	lsrs	r6, r0, #16
	ldr	r0, [pc, #408]
	cmp	r6, r0
	bne.n	.L_08099f84
	ldr	r1, [r7, #8]
	str	r1, [r5, #0]
	ldr	r2, [r7, #12]
	add	r2, r9
	str	r2, [r5, #4]
	ldr	r3, [r7, #16]
	ldr	r0, [sp, #20]
	str	r3, [r5, #8]
	add	r1, r9
	bl	Object_SetPosition
	ldr	r1, [r5, #0]
	ldr	r2, [pc, #352]
	ldr	r0, [sp, #24]
	adds	r1, r1, r2
	ldr	r3, [r5, #8]
	ldr	r2, [r5, #4]
	bl	Object_SetPosition
	ldr	r0, [sp, #20]
	movs	r1, #1
	bl	Engine_ObjectSetAnimation
	ldr	r0, [sp, #24]
	movs	r1, #1
	bl	Engine_ObjectSetAnimation
	b.n	.L_0809a0e8
.L_08099f84:
	ldr	r3, [r7, #8]
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	add	r3, r9
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	movs	r0, #128
	str	r3, [r5, #8]
	lsls	r0, r0, #10
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	Vector_AddPolarOffset
	ldr	r1, [r5, #0]
	ldr	r0, [sp, #20]
	add	r1, r9
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	bl	Object_SetPosition
	ldr	r1, [r5, #0]
	ldr	r3, [pc, #280]
	ldr	r0, [sp, #24]
	adds	r1, r1, r3
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	bl	Object_SetPosition
	ldr	r0, [sp, #20]
	bl	Object_CommitPosition
	ldr	r0, [sp, #24]
	bl	Object_CommitPosition
	ldr	r3, [r7, #8]
	str	r3, [r5, #0]
	ldr	r3, [r7, #20]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	mov	r0, r9
	str	r3, [r5, #8]
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	Vector_AddPolarOffset
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	0x08009250
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_0809a00a
	ldr	r3, [r7, #20]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r7, #20]
	mov	r1, sl
	adds	r0, r7, #0
	bl	Object_CheckMovementCollision
	ldr	r3, [r7, #20]
	ldr	r1, [pc, #196]
	adds	r3, r3, r1
	str	r3, [r7, #20]
	cmp	r0, #0
	ble.n	.L_0809a02e
.L_0809a00a:
	ldr	r0, [sp, #20]
	movs	r1, #4
	bl	Engine_ObjectSetAnimation
	ldr	r0, [sp, #24]
	movs	r1, #4
	bl	Engine_ObjectSetAnimation
	ldr	r3, [pc, #200]
	ldr	r3, [r3, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0809a0e8
	movs	r0, #114
	bl	Audio_PlayCue
	b.n	.L_0809a0e8
.L_0809a02e:
	movs	r0, #175
	bl	Audio_PlayCue
	ldr	r0, [sp, #20]
	mov	r2, sl
	movs	r1, #4
	ldr	r5, [r2, #0]
	ldr	r6, [r2, #8]
	bl	Engine_ObjectSetAnimation
	ldr	r0, [sp, #24]
	movs	r1, #4
	bl	Engine_ObjectSetAnimation
	movs	r0, #15
	bl	WaitFrames
	ldr	r1, [pc, #132]
	adds	r3, r7, #0
	adds	r3, #91
	mov	r0, r8
	strb	r0, [r3, #0]
	str	r1, [r7, #48]
	str	r1, [r7, #52]
	mov	r2, sl
	mov	r0, sl
	ldr	r1, [r2, #0]
	ldr	r3, [r0, #8]
	ldr	r2, [r2, #4]
	adds	r0, r7, #0
	bl	Object_SetPosition
	ldr	r3, [sp, #20]
	ldr	r1, [pc, #100]
	str	r1, [r3, #48]
	str	r1, [r3, #52]
	ldr	r2, [sp, #8]
	str	r1, [r2, #48]
	str	r1, [r2, #52]
	mov	r3, sl
	ldr	r1, [r3, #0]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r1, r1, r0
	ldr	r2, [r3, #4]
	ldr	r0, [sp, #20]
	ldr	r3, [r3, #8]
	bl	Object_SetPosition
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldr	r3, [pc, #48]
	mov	r0, sl
	adds	r1, r1, r3
	ldr	r2, [r2, #4]
	ldr	r3, [r0, #8]
	ldr	r0, [sp, #8]
	bl	Object_SetPosition
	adds	r0, r7, #0
	bl	Object_CommitPosition
	mov	r1, r8
	str	r5, [r7, #8]
	str	r6, [r7, #16]
	str	r1, [r7, #36]
	str	r1, [r7, #44]
	movs	r0, #10
	bl	WaitFrames
	b.n	.L_0809a0fc
	.4byte 0x03001f30
	.4byte 0x0809f0bc
	.4byte 0xffe00000
	.4byte 0xfff00000
	.4byte 0x08096b89
	.4byte 0x08099d19
	.4byte 0x00006666
	.4byte 0x00003333
	.4byte 0x03001ae8
	.4byte 0x0000ffff
	.2byte 0x1e40
	.2byte 0x0300
.L_0809a0e8:
	movs	r0, #1
	bl	WaitFrames
	ldr	r3, [pc, #396]
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #396]
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_0809a0fc
	b.n	.L_08099f3a
.L_0809a0fc:
	ldr	r0, [sp, #20]
	movs	r1, #4
	bl	Engine_ObjectSetAnimation
	ldr	r0, [sp, #24]
	movs	r1, #4
	bl	Engine_ObjectSetAnimation
	ldr	r0, [pc, #372]
	bl	Scheduler_RemoveCallback
	movs	r0, #135
	bl	Audio_PlayCue
	movs	r0, #15
	bl	WaitFrames
	movs	r0, #135
	bl	Audio_PlayCue
	movs	r0, #15
	bl	WaitFrames
	ldr	r3, [r7, #8]
	mov	r2, sl
	str	r3, [r2, #0]
	ldr	r3, [r7, #12]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r2, #4]
	ldr	r3, [r7, #16]
	mov	r1, sp
	str	r3, [r2, #8]
	adds	r1, #44
	movs	r2, #128
	lsls	r2, r2, #10
	movs	r3, #19
	str	r1, [sp, #4]
	mov	r9, r2
	mov	r8, r3
.L_0809a14e:
	mov	r0, sl
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	ldr	r3, [r0, #8]
	ldr	r0, [pc, #304]
	bl	Object_Spawn
	ldr	r2, [sp, #4]
	adds	r6, r0, #0
	stmia	r2!, {r6}
	adds	r1, r2, #0
	str	r1, [sp, #4]
	cmp	r6, #0
	beq.n	.L_0809a1a2
	ldr	r1, [pc, #288]
	bl	Engine_ObjectSetScript
	bl	Random16
	mov	r3, r9
	adds	r2, r6, #0
	adds	r2, #85
	str	r3, [r6, #52]
	add	r0, r9
	movs	r3, #0
	str	r0, [r6, #48]
	strb	r3, [r2, #0]
	bl	Random16
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r0, #128
	lsls	r0, r0, #12
	lsls	r5, r5, #3
	adds	r5, r5, r0
	bl	Random16
	adds	r1, r5, #0
	adds	r2, r0, #0
	adds	r0, r6, #0
	bl	Motion_SetTargetPositionFromMagnitudeAngle
.L_0809a1a2:
	movs	r1, #1
	negs	r1, r1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_0809a14e
	movs	r0, #131
	bl	Audio_PlayCue
	ldr	r0, [sp, #20]
	bl	Object_Destroy
	ldr	r0, [sp, #24]
	bl	Object_Destroy
	mov	r3, fp
	adds	r3, #68
	ldrb	r1, [r3, #0]
	adds	r0, r7, #0
	bl	Animation_ApplyChildValuesFar
	mov	r3, fp
	ldr	r1, [r3, #60]
	adds	r0, r7, #0
	bl	Engine_ObjectSetScript
	mov	r0, fp
	ldr	r3, [r0, #56]
	str	r3, [r7, #108]
	ldr	r1, [sp, #16]
	movs	r3, #3
	strb	r3, [r1, #0]
	movs	r3, #160
	lsls	r3, r3, #12
	str	r3, [r7, #40]
	ldr	r3, [pc, #164]
	str	r3, [r7, #68]
	ldr	r3, [sp, #12]
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r0, [sp, #28]
	str	r2, [r0, #108]
	ldr	r0, [sp, #28]
	movs	r1, #0
	bl	Animation_ApplyChildValuesFar
	mov	r3, fp
	adds	r3, #52
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0809a264
	ldr	r3, [r7, #40]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	blt.n	.L_0809a22c
.L_0809a216:
	movs	r0, #1
	bl	WaitFrames
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #89
	bgt.n	.L_0809a22c
	ldr	r3, [r7, #40]
	cmp	r3, #0
	bge.n	.L_0809a216
.L_0809a22c:
	movs	r0, #1
	bl	WaitFrames
	ldr	r3, [r7, #40]
	movs	r0, #0
	mov	r8, r0
	cmp	r3, #0
	bge.n	.L_0809a252
.L_0809a23c:
	movs	r0, #1
	bl	WaitFrames
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #89
	bgt.n	.L_0809a252
	ldr	r3, [r7, #40]
	cmp	r3, #0
	blt.n	.L_0809a23c
.L_0809a252:
	adds	r0, r7, #0
	bl	BattleFx_SpawnRadialParticleRing
	bl	BattleFx_PrepareBufferInterpolation
	movs	r0, #30
	bl	WaitFrames
	b.n	.L_0809a268
.L_0809a264:
	bl	BattleFx_PrepareBufferInterpolation
.L_0809a268:
	add	sp, #124
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001c94
	.4byte 0x00000303
	.4byte 0x08099d19
	.4byte 0x0000011d
	.4byte 0x0809f0d4
	.4byte 0x00003333
