.syntax unified
	.thumb
	.global Game_ResetForNewGame
	.global Func_0808a8e4
	.thumb_func
Game_ResetForNewGame:
Func_0808a8e4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #140]
	ldr	r1, [pc, #140]
	ldrb	r3, [r3, #0]
	mov	fp, r1
	cmp	r3, #0
	beq.n	.L_0808a932
	cmp	r0, #1
	bne.n	.L_0808a918
	ldr	r1, [pc, #132]
	movs	r4, #224
	ldr	r2, [pc, #132]
	lsls	r4, r4, #1
	adds	r3, r1, r4
	strh	r2, [r3, #0]
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	strh	r0, [r3, #0]
	b.n	.L_0808a94c
.L_0808a918:
	cmp	r0, #2
	bne.n	.L_0808a932
	ldr	r1, [pc, #104]
	movs	r4, #224
	ldr	r3, [pc, #108]
	lsls	r4, r4, #1
	movs	r0, #225
	adds	r2, r1, r4
	lsls	r0, r0, #1
	strh	r3, [r2, #0]
	adds	r2, r1, r0
	movs	r3, #1
	b.n	.L_0808a94a
.L_0808a932:
	bl	0x08077098
	ldr	r1, [pc, #80]
	movs	r4, #224
	ldr	r3, [pc, #88]
	lsls	r4, r4, #1
	movs	r0, #225
	adds	r2, r1, r4
	lsls	r0, r0, #1
	strh	r3, [r2, #0]
	adds	r2, r1, r0
	movs	r3, #2
.L_0808a94a:
	strh	r3, [r2, #0]
.L_0808a94c:
	ldr	r3, [pc, #56]
	ldr	r1, [pc, #72]
	adds	r2, r3, r1
	ldrb	r0, [r2, #0]
	ldr	r2, [pc, #68]
	adds	r3, r3, r2
	ldrb	r1, [r3, #0]
	bl	PaletteGlow_UpdateFar
	bl	Resource_InitializeTable
	bl	Scheduler_ResetTaskTable
	bl	Scheduler_ResetTaskTable
	ldr	r3, [pc, #52]
	mov	r9, r3
.L_0808a96e:
	ldr	r0, [pc, #52]
	bl	GameFlag_IsSet
	cmp	r0, #0
	beq.n	.L_0808a9a8
	ldr	r0, [pc, #40]
	bl	GameFlag_ClearBitFar
	b.n	.L_0808a9b0
	.4byte 0x03001f54
	.4byte 0x0809f1a8
	.4byte 0x02000240
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000205
	.4byte 0x00000206
	.4byte 0x050001c0
	.2byte 0x0101
	.2byte 0x0000
.L_0808a9a8:
	movs	r0, #144
	lsls	r0, r0, #1
	bl	Audio_PlayCue
.L_0808a9b0:
	ldr	r7, [pc, #92]
	movs	r4, #224
	lsls	r4, r4, #1
	adds	r4, r4, r7
	movs	r0, #0
	ldrsh	r3, [r4, r0]
	movs	r1, #225
	lsls	r3, r3, #3
	add	r3, fp
	lsls	r1, r1, #1
	mov	sl, r3
	adds	r3, r7, r1
	ldr	r1, [pc, #72]
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	ldrh	r3, [r1, #10]
	ldr	r2, [pc, #52]
	ands	r3, r2
	strh	r3, [r1, #10]
	ldr	r2, [pc, #52]
	ldrh	r3, [r1, #10]
	ands	r3, r2
	strh	r3, [r1, #10]
	mov	r8, r4
	ldrh	r3, [r1, #10]
	bl	Scheduler_ResetTaskTable
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl	Runtime_SetIrqHandler
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl	Runtime_SetIrqHandler
	bl	Runtime_InitializeHeap
	bl	Bg0_ClearTilemap
	bl	Resource_InitializeTable
	b.n	.L_0808aa18
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x02000240
	.2byte 0x00b0
	.2byte 0x0400
.L_0808aa18:
	mov	r0, r8
	movs	r1, #253
	movs	r4, #0
	ldrsh	r3, [r0, r4]
	lsls	r1, r1, #1
	cmp	r3, r1
	ble.n	.L_0808aaa8
	movs	r2, #254
	lsls	r2, r2, #1
	cmp	r3, r2
	beq.n	.L_0808aa6e
	cmp	r3, r2
	bgt.n	.L_0808aa3a
	subs	r2, #1
	cmp	r3, r2
	beq.n	.L_0808aa9e
	b.n	.L_0808aaa0
.L_0808aa3a:
	ldr	r4, [pc, #240]
	cmp	r3, r4
	beq.n	.L_0808aa52
	movs	r0, #255
	lsls	r0, r0, #1
	cmp	r3, r0
	bne.n	.L_0808aaa0
	adds	r0, r6, #0
	bl	0x080b50a0
	adds	r6, r0, #0
	b.n	.L_0808aaa0
.L_0808aa52:
	movs	r0, #64
	bl	Runtime_BumpAllocate
	adds	r5, r0, #0
	ldr	r3, [pc, #212]
	mov	r0, r9
	adds	r1, r5, #0
	ldr	r2, [pc, #208]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	bl	Func_080f4000
	b.n	.L_0808aa88
.L_0808aa6e:
	movs	r0, #64
	bl	Runtime_BumpAllocate
	adds	r5, r0, #0
	ldr	r3, [pc, #184]
	mov	r0, r9
	adds	r1, r5, #0
	ldr	r2, [pc, #180]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	bl	Func_080f6000
.L_0808aa88:
	ldr	r3, [pc, #164]
	adds	r6, r0, #0
	mov	r1, r9
	adds	r0, r5, #0
	ldr	r2, [pc, #160]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	bl	Party_Do
	b.n	.L_0808aaa0
.L_0808aa9e:
	movs	r6, #0
.L_0808aaa0:
	adds	r0, r6, #0
	bl	Party_SetReturnPoint
	b.n	.L_0808a96e
.L_0808aaa8:
	ldr	r5, [pc, #140]
	adds	r0, r5, #0
	bl	GameFlag_IsSet
	mov	r3, r8
	adds	r1, r0, #0
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	Scene_ResetFlagsOnEnter
	bl	Scene_ResolveInteractionResult
	adds	r0, r5, #0
	bl	GameFlag_IsSet
	cmp	r0, #0
	bne.n	.L_0808aaf0
	movs	r0, #141
	lsls	r0, r0, #1
	bl	GameFlag_IsSet
	cmp	r0, #0
	bne.n	.L_0808aae6
	ldr	r0, [pc, #100]
	bl	GameFlag_IsSet
	cmp	r0, #0
	bne.n	.L_0808aae6
	bl	Audio_PlayCueFromEventWork
	b.n	.L_0808ab0a
.L_0808aae6:
	movs	r0, #141
	lsls	r0, r0, #1
	bl	GameFlag_ClearBitFar
	b.n	.L_0808ab0a
.L_0808aaf0:
	ldr	r4, [pc, #76]
	adds	r3, r7, r4
	movs	r2, #1
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_0808ab06
	bl	Audio_PlayCue
	b.n	.L_0808ab0a
.L_0808ab06:
	bl	Audio_PlayCueFromEventWork
.L_0808ab0a:
	mov	r4, sl
	ldr	r3, [pc, #52]
	movs	r0, #237
	ldrh	r2, [r4, #4]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	strh	r2, [r3, #0]
	movs	r0, #0
	bl	BattleFx_LoadResourceGroup
	adds	r0, r6, #0
	bl	Func_0808c4f8
	bl	MapGroupTable_SelectEntry
	b.n	.L_0808a96e
	movs	r0, r0
	.4byte 0x000001fd
	.4byte 0x040000d4
	.4byte 0x84000010
	.4byte 0x00000109
	.4byte 0x0000011b
	.4byte 0x0000021e
	.4byte 0x02000240
