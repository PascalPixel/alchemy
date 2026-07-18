@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_08009080, 0x08009080
	.set sub_080091a8, 0x080091a8
	.set sub_080091e0, 0x080091e0
	.set sub_0808b3ec, 0x0808b3ec
	.set sub_0808b7b8, 0x0808b7b8
	.set sub_0808b7c4, 0x0808b7c4
	.set sub_0808b9f8, 0x0808b9f8
	.global Region_0808b674
Region_0808b674:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #276]
	ldr r3, [r3, #0]
	mov r10, r0
	mov r8, r3
	movs r0, #250
	ldr r3, [pc, #272]
	lsls r0, r0, #1
	movs r5, #128
	adds r3, r3, r0
	lsls r5, r5, #2
	ldr r7, [r3, #0]
	add r5, r8
	ldr r3, [pc, #260]
	adds r2, r5, #0
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	movs r2, #134
	lsls r2, r2, #2
	add r2, r8
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	mov r3, r8
	movs r2, #0
	adds r3, #12
	mov r12, r8
.L_0808b6b6:
	str r2, [r3, #0]
	subs r3, #4
	cmp r3, r12
	bge .L_0808b6b6
	bl sub_0808b9f8
	ldr r3, [pc, #220]
	ldr r6, [pc, #208]
	movs r1, #238
	strh r3, [r5, #2]
	strh r7, [r5, #0]
	lsls r1, r1, #1
	adds r3, r6, r1
	ldr r3, [r3, #0]
	movs r2, #0
	str r2, [r5, #12]
	movs r2, #242
	str r3, [r5, #8]
	lsls r2, r2, #1
	adds r3, r6, r2
	ldr r3, [r3, #0]
	movs r4, #244
	str r3, [r5, #16]
	lsls r4, r4, #1
	adds r3, r6, r4
	ldr r3, [r3, #0]
	adds r0, r5, #0
	strh r3, [r5, #20]
	adds r1, r7, #0
	bl sub_0808b3ec
	mov r0, r10
	movs r1, #8
	bl sub_0808b3ec
	lsls r3, r7, #2
	adds r3, #20
	mov r0, r8
	movs r1, #246
	ldr r5, [r0, r3]
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrh r3, [r3, #0]
	adds r2, r5, #0
	adds r2, #34
	strb r3, [r2, #0]
	ldr r3, [r5, #8]
	cmp r3, #0
	bge .L_0808b71c
	ldr r2, [pc, #136]
	adds r3, r3, r2
.L_0808b71c:
	asrs r2, r3, #20
	ldr r3, [r5, #16]
	cmp r3, #0
	bge .L_0808b728
	ldr r4, [pc, #124]
	adds r3, r3, r4
.L_0808b728:
	asrs r3, r3, #20
	lsls r3, r3, #7
	adds r3, r2, r3
	ldr r0, [pc, #120]
	lsls r3, r3, #2
	adds r2, r3, r0
	ldr r4, [pc, #116]
	movs r0, #240
	lsls r0, r0, #1
	adds r1, r3, r4
	adds r3, r6, r0
	ldr r3, [r3, #0]
	cmp r3, #0
	beq sub_0808b7b8
	ldrb r3, [r2, #2]
	cmp r3, #253
	bne sub_0808b7b8
	ldrb r3, [r1, #2]
	cmp r3, #253
	bne sub_0808b7b8
	movs r1, #249
	lsls r1, r1, #1
	adds r2, r6, r1
	movs r3, #1
	strb r3, [r2, #0]
	ldr r3, [pc, #84]
	ldr r2, [r5, #16]
	ldr r1, [r5, #8]
	adds r2, r2, r3
	movs r0, #0
	bl sub_080091a8
	ldr r4, [pc, #72]
	ldr r3, [r5, #12]
	adds r0, r0, r4
	adds r3, r3, r0
	str r3, [r5, #12]
	str r3, [r5, #20]
	adds r2, r5, #0
	ldr r3, [pc, #24]
	adds r2, #85
	adds r0, r5, #0
	movs r1, #0
	strb r3, [r2, #0]
	bl sub_080091e0
	adds r0, r5, #0
	movs r1, #12
	bl sub_08009080
	b sub_0808b7c4
