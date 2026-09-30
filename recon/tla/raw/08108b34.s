.syntax unified
	.thumb
	.global Func_08108b34
	.thumb_func
Func_08108b34:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	mov	r8, r1
	adds	r0, r2, #0
	cmp	r7, #95
	bgt.n	.L_08108b6a
	bl	Resource_GetTableEntry
	adds	r6, r0, #0
	mov	r0, r8
	bl	Func_08014d78
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	Func_0801591c
	adds	r0, r7, #0
	mov	r1, r8
	adds	r2, r5, #0
	bl	VramBlock_LoadCached
	adds	r0, r5, #0
	bl	Func_08013164
.L_08108b6a:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #40
	movs	r1, #0
	movs	r2, #0
	str	r1, [sp, #32]
	str	r1, [sp, #20]
	mov	sl, r3
	str	r2, [r3, #36]
	movs	r1, #7
	movs	r2, #12
	movs	r5, #2
	movs	r3, #4
	movs	r0, #18
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	mov	r3, sl
	str	r0, [r3, #12]
	bl	Func_08109188
	movs	r0, #0
	movs	r1, #8
	movs	r2, #15
	movs	r3, #4
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	str	r0, [sp, #32]
.L_08108bbc:
	ldr	r1, [sp, #20]
	movs	r5, #2
	str	r1, [sp, #36]
	movs	r2, #30
	movs	r1, #12
	movs	r3, #4
	movs	r0, #0
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	movs	r3, #128
	str	r0, [sp, #28]
	lsls	r3, r3, #3
	adds	r3, #220
	add	r3, sl
	ldr	r2, [r3, #0]
	movs	r3, #18
	strb	r3, [r2, #5]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #5
	add	r2, sl
	movs	r3, #12
	strb	r3, [r2, #0]
	movs	r0, #0
	movs	r2, #30
	movs	r3, #3
	movs	r1, #17
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	movs	r3, #156
	lsls	r3, r3, #2
	add	r3, sl
	str	r0, [sp, #24]
	str	r3, [sp, #12]
	movs	r2, #1
	mov	r9, r2
.L_08108c08:
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #2
	add	r3, sl
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	mov	r3, r9
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_08108c76
	ldr	r3, [sp, #36]
	ldr	r1, [sp, #12]
	lsls	r3, r3, #1
	ldrsh	r5, [r3, r1]
	adds	r0, r5, #0
	bl	Item_Get
	movs	r1, #7
	adds	r6, r0, #0
	ldr	r0, [sp, #36]
	bl	Math_Mod
	adds	r1, r0, #0
	lsls	r1, r1, #5
	ldr	r0, [sp, #28]
	subs	r1, #8
	movs	r2, #8
	bl	Func_08108af0
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #5
	add	r2, sl
	movs	r3, #4
	strb	r3, [r2, #0]
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #36]
	bl	.L_08109068
	ldr	r1, [pc, #784]
	ldr	r0, [sp, #24]
	adds	r1, r5, r1
	bl	0x08109270
	ldr	r0, [sp, #32]
	bl	RenderOutput_RedrawSavedRectFar
	movs	r3, #0
	ldrh	r2, [r6, #0]
	ldr	r0, [sp, #32]
	adds	r1, r5, #0
	bl	0x081091cc
	movs	r3, #0
	mov	r9, r3
.L_08108c76:
	ldr	r7, [pc, #756]
	movs	r3, #1
	ldr	r2, [r7, #4]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08108c84
	b.n	.L_0810902a
.L_08108c84:
	ldr	r3, [r7, #4]
	movs	r6, #2
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_08108c90
	b.n	.L_0810901e
.L_08108c90:
	ldr	r3, [r7, #0]
	movs	r1, #4
	ands	r3, r1
	mov	fp, r1
	cmp	r3, #0
	beq.n	.L_08108d18
	ldr	r3, [sp, #36]
	ldr	r2, [sp, #12]
	lsls	r3, r3, #1
	ldrsh	r2, [r3, r2]
	mov	r8, r2
	mov	r0, r8
	bl	0x080ad2a8
	adds	r5, r0, #0
	mov	r0, r8
	bl	Item_Get
	str	r0, [sp, #8]
	movs	r0, #126
	bl	Audio_PlayCue
	movs	r3, #10
	movs	r1, #3
	movs	r2, #17
	movs	r0, #13
	str	r6, [sp, #0]
	bl	UiWindow_CreateFar
	adds	r2, r5, #0
	mov	r1, r8
	adds	r6, r0, #0
	bl	0x080f8040
	ldr	r3, [r7, #0]
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08108cee
	movs	r5, #4
.L_08108ce0:
	movs	r0, #1
	bl	WaitFrames
	ldr	r3, [r7, #0]
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_08108ce0
.L_08108cee:
	adds	r0, r6, #0
	movs	r1, #2
	bl	UiWork_FinalizeFar
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #36]
	bl	.L_08109068
	bl	Func_08109188
	ldr	r0, [sp, #32]
	bl	RenderOutput_RedrawSavedRectFar
	ldr	r3, [sp, #8]
	ldr	r0, [sp, #32]
	ldrh	r2, [r3, #0]
	mov	r1, r8
	movs	r3, #0
	bl	0x081091cc
	b.n	.L_08108d24
.L_08108d18:
	add	r0, sp, #36
	mov	r1, r8
	movs	r2, #7
	bl	Func_08108690
	mov	r9, r0
.L_08108d24:
	movs	r0, #1
	bl	WaitFrames
	b.n	.L_08108c08
.L_08108d2c:
	ldr	r0, [sp, #24]
	movs	r1, #2
	bl	UiWork_FinalizeFar
	movs	r1, #2
	ldr	r0, [sp, #28]
	bl	UiWork_FinalizeFar
	movs	r0, #1
	bl	WaitFrames
	cmp	r5, #0
	beq.n	.L_08108d48
	b.n	.L_08109038
.L_08108d48:
	ldr	r1, [sp, #20]
	movs	r2, #156
	lsls	r2, r2, #2
	lsls	r3, r1, #1
	adds	r3, r3, r2
	mov	r1, sl
	movs	r5, #128
	ldrh	r3, [r1, r3]
	lsls	r5, r5, #3
	adds	r5, #252
	add	r5, sl
	strh	r3, [r5, #0]
	ldr	r0, [pc, #524]
	bl	0x081084f4
	ldrh	r0, [r5, #0]
	bl	Item_Get
	movs	r2, #1
	str	r0, [sp, #4]
	str	r2, [sp, #16]
	movs	r6, #0
	movs	r5, #2
	movs	r1, #14
	movs	r2, #13
	movs	r3, #3
	movs	r0, #0
	str	r6, [sp, #36]
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #220
	add	r3, sl
	ldr	r2, [r3, #0]
	movs	r3, #4
	strb	r3, [r2, #5]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #5
	movs	r3, #12
	add	r2, sl
	strb	r3, [r2, #0]
	movs	r1, #2
	movs	r2, #0
	adds	r7, r0, #0
	bl	0x080f8060
	movs	r3, #9
	movs	r0, #16
	movs	r1, #11
	movs	r2, #14
	str	r5, [sp, #0]
	bl	UiWindow_CreateFar
	movs	r3, #2
	mov	fp, r6
	mov	r8, r0
	mov	r9, r3
.L_08108dc0:
	mov	r1, fp
	cmp	r1, #0
	beq.n	.L_08108dd4
	movs	r2, #0
	ldr	r0, [pc, #420]
	mov	fp, r2
	bl	0x081084f4
	movs	r3, #1
	mov	r9, r3
.L_08108dd4:
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L_08108e62
	ldr	r4, [sp, #36]
	movs	r3, #153
	lsls	r3, r3, #3
	lsls	r2, r4, #1
	adds	r2, r2, r3
	mov	r3, sl
	adds	r3, #2
	ldrsh	r6, [r3, r2]
	adds	r3, r4, #0
	cmp	r4, #0
	bge.n	.L_08108df2
	adds	r3, r4, #3
.L_08108df2:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r4, r3
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #3
	movs	r2, #0
	subs	r1, #12
	adds	r0, r7, #0
	bl	Func_08108af0
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #5
	add	r2, sl
	movs	r3, #3
	strb	r3, [r2, #0]
	mov	r2, r9
	cmp	r2, #2
	bne.n	.L_08108e30
	ldr	r0, [sp, #36]
	cmp	r0, #0
	bge.n	.L_08108e22
	adds	r0, #3
.L_08108e22:
	asrs	r0, r0, #2
	lsls	r0, r0, #2
	bl	Func_080f8058
	movs	r0, #1
	bl	WaitFrames
.L_08108e30:
	movs	r5, #128
	lsls	r5, r5, #3
	adds	r5, #252
	add	r5, sl
	adds	r0, r7, #0
	ldr	r1, [sp, #36]
	ldrh	r2, [r5, #0]
	bl	0x0810928c
	ldrh	r0, [r5, #0]
	bl	0x080ad1e0
	cmp	r0, #0
	bne.n	.L_08108e58
	ldrh	r2, [r5, #0]
	mov	r0, r8
	adds	r1, r6, #0
	bl	0x081095b0
	b.n	.L_08108e62
.L_08108e58:
	ldrh	r2, [r5, #0]
	mov	r0, r8
	adds	r1, r6, #0
	bl	0x081093a4
.L_08108e62:
	ldr	r1, [pc, #264]
	movs	r3, #1
	ldr	r2, [r1, #4]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08108f3e
	movs	r5, #128
	lsls	r5, r5, #3
	adds	r5, #252
	movs	r3, #2
	adds	r0, r7, #0
	add	r5, sl
	mov	r9, r3
	bl	0x08038060
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	bl	0x080ad020
	adds	r1, r0, #0
	cmp	r1, #0
	bge.n	.L_08108ebe
	movs	r0, #113
	bl	Audio_PlayCue
	adds	r0, r6, #0
	movs	r1, #1
	bl	UiText_DrawQuantity
	ldrh	r0, [r5, #0]
	movs	r1, #2
	bl	UiText_DrawQuantity
	adds	r0, r6, #0
	bl	0x080ad1f0
	cmp	r0, #15
	bne.n	.L_08108eb6
	ldr	r0, [pc, #196]
	bl	0x081084f4
	b.n	.L_08108dc0
.L_08108eb6:
	ldr	r0, [pc, #192]
	bl	0x081084f4
	b.n	.L_08108dc0
.L_08108ebe:
	adds	r0, r6, #0
	bl	Inventory_RemoveFar
	ldr	r2, [pc, #180]
	ldr	r1, [sp, #4]
	ldr	r2, [r2, #16]
	ldrh	r3, [r1, #0]
	cmp	r3, r2
	bls.n	.L_08108ed2
	b.n	.L_0810900c
.L_08108ed2:
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	bl	0x080ad1c8
	cmp	r0, #0
	bne.n	.L_08108efc
	movs	r1, #1
	adds	r0, r6, #0
	bl	UiText_DrawQuantity
	ldr	r0, [pc, #152]
	bl	0x081084f4
	movs	r0, #0
	bl	0x08108630
	movs	r2, #1
	mov	fp, r2
	cmp	r0, #0
	beq.n	.L_08108efc
	b.n	.L_08108dc0
.L_08108efc:
	movs	r5, #128
	lsls	r5, r5, #3
	movs	r0, #112
	adds	r5, #252
	bl	Audio_PlayCue
	add	r5, sl
	movs	r0, #1
	bl	WaitFrames
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	bl	0x08109624
	movs	r1, #1
	movs	r3, #1
	negs	r1, r1
	str	r0, [sp, #16]
	mov	fp, r3
	cmp	r0, r1
	bne.n	.L_08108f28
	b.n	.L_08108dc0
.L_08108f28:
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	ldr	r2, [sp, #16]
	bl	0x081098c0
	adds	r0, r7, #0
	mov	r1, r8
	bl	0x0810a490
	movs	r5, #0
	b.n	.L_08108f84
.L_08108f3e:
	ldr	r3, [r1, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08109000
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #4
	add	r3, sl
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	add	r0, sp, #36
	movs	r2, #4
	bl	Func_08108690
	mov	r9, r0
	movs	r0, #1
	bl	WaitFrames
	b.n	.L_08108dc0
	movs	r0, r0
	.4byte 0x00000092
	.4byte 0x03001150
	.4byte 0x0000124e
	.4byte 0x0000124f
	.4byte 0x00001257
	.4byte 0x02000240
	.2byte 0x1250
	.2byte 0x0000
.L_08108f84:
	movs	r0, #0
	bl	Func_0810bea8
	bl	0x080f8068
	mov	r0, r8
	movs	r1, #2
	bl	UiWork_FinalizeFar
	adds	r0, r7, #0
	movs	r1, #2
	bl	UiWork_FinalizeFar
	movs	r0, #1
	bl	WaitFrames
	cmp	r5, #0
	bne.n	.L_08108ff8
	movs	r3, #129
	lsls	r3, r3, #3
	adds	r3, #255
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_08108ff8
	ldr	r2, [sp, #16]
	cmp	r5, r2
	bge.n	.L_08108fda
	movs	r6, #128
	lsls	r6, r6, #3
	adds	r6, #252
	add	r6, sl
	adds	r5, r2, #0
.L_08108fca:
	movs	r1, #1
	ldrh	r0, [r6, #0]
	negs	r1, r1
	subs	r5, #1
	bl	0x080ad1e8
	cmp	r5, #0
	bne.n	.L_08108fca
.L_08108fda:
	bl	Func_081080a8
	cmp	r0, #0
	beq.n	.L_08109038
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #2
	add	r3, sl
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	ldr	r2, [sp, #20]
	subs	r3, #1
	cmp	r2, r3
	ble.n	.L_08108ff8
	str	r3, [sp, #20]
.L_08108ff8:
	ldr	r0, [pc, #100]
	bl	0x081084f4
	b.n	.L_08108bbc
.L_08109000:
	movs	r0, #113
	movs	r5, #1
	bl	Audio_PlayCue
	negs	r5, r5
	b.n	.L_08108f84
.L_0810900c:
	movs	r0, #113
	bl	Audio_PlayCue
	movs	r5, #1
	ldr	r0, [pc, #76]
	bl	0x0810857c
	negs	r5, r5
	b.n	.L_08108f84
.L_0810901e:
	movs	r0, #113
	movs	r5, #1
	bl	Audio_PlayCue
	negs	r5, r5
	b.n	.L_08108d2c
.L_0810902a:
	ldr	r3, [sp, #36]
	movs	r0, #112
	str	r3, [sp, #20]
	movs	r5, #0
	bl	Audio_PlayCue
	b.n	.L_08108d2c
.L_08109038:
	ldr	r0, [sp, #32]
	movs	r1, #2
	bl	UiWork_FinalizeFar
	mov	r1, sl
	ldr	r0, [r1, #12]
	movs	r1, #2
	bl	UiWork_FinalizeFar
	movs	r0, #1
	bl	WaitFrames
	movs	r0, #0
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00001259
	.2byte 0x124d
	.2byte 0x0000
.L_08109068:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r1, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #2
	movs	r1, #156
	adds	r3, r5, r2
	lsls	r1, r1, #2
	adds	r7, r5, r1
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	mov	r9, r0
	str	r1, [sp, #4]
	ldr	r0, [sp, #8]
	movs	r1, #7
	bl	Math_Div
	lsls	r3, r0, #3
	subs	r6, r3, r0
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_08109178
	mov	r0, r9
	bl	0x08038060
	cmp	r6, #0
	beq.n	.L_081090da
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #238
	adds	r3, r5, r1
	ldrh	r0, [r3, #0]
	movs	r3, #16
	negs	r3, r3
	movs	r1, #128
	str	r3, [sp, #0]
	mov	r2, r9
	movs	r3, #216
	lsls	r1, r1, #23
	bl	RenderOutput_CreateFar
	movs	r2, #0
	movs	r3, #17
	strb	r2, [r0, #4]
	strb	r3, [r0, #5]
	strh	r2, [r0, #12]
.L_081090da:
	ldr	r2, [sp, #4]
	adds	r3, r6, #7
	cmp	r3, r2
	bge.n	.L_08109104
	movs	r1, #158
	lsls	r1, r1, #3
	adds	r3, r5, r1
	ldrh	r0, [r3, #0]
	movs	r1, #128
	movs	r3, #24
	str	r3, [sp, #0]
	mov	r2, r9
	movs	r3, #216
	lsls	r1, r1, #23
	bl	RenderOutput_CreateFar
	movs	r2, #0
	movs	r3, #15
	strb	r2, [r0, #4]
	strb	r3, [r0, #5]
	strh	r2, [r0, #12]
.L_08109104:
	ldr	r3, [sp, #4]
	movs	r2, #0
	mov	sl, r2
	cmp	r6, r3
	bcs.n	.L_08109178
	lsls	r3, r6, #1
	adds	r3, r3, r7
	movs	r1, #16
	mov	r8, r3
	mov	fp, r1
.L_08109118:
	mov	r3, r8
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	adds	r0, r5, #0
	bl	Item_Get
	mov	r1, sl
	movs	r2, #0
	lsls	r3, r1, #5
	str	r2, [sp, #0]
	adds	r7, r0, #0
	mov	r2, r9
	adds	r0, r5, #0
	movs	r1, #1
	bl	RenderOutput_CreateLoadedFar
	movs	r3, #252
	strb	r3, [r0, #15]
	ldr	r2, [sp, #8]
	cmp	r6, r2
	bne.n	.L_0810914e
	movs	r3, #9
	strb	r3, [r0, #5]
	movs	r3, #10
	strh	r3, [r0, #12]
	movs	r3, #253
	strb	r3, [r0, #15]
.L_0810914e:
	mov	r2, fp
	mov	r1, r9
	movs	r3, #0
	ldrh	r0, [r7, #0]
	bl	0x081087e0
	movs	r3, #251
	movs	r1, #1
	strb	r3, [r0, #15]
	add	sl, r1
	movs	r3, #32
	add	fp, r3
	movs	r2, #2
	mov	r3, sl
	add	r8, r2
	adds	r6, #1
	cmp	r3, #6
	bhi.n	.L_08109178
	ldr	r1, [sp, #4]
	cmp	r6, r1
	bcc.n	.L_08109118
.L_08109178:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
