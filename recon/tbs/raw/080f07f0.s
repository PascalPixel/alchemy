@ Uncredited native TBS drawer, freshly rederived from the owned English ROM.
@ Its complete 620-byte extent includes the 52-byte literal pool; no tail follows.
.syntax unified
	.text
	.type DisplayScroll_DrawLine, %function
	.thumb
	.global DisplayScroll_DrawLine
	.thumb_func
DisplayScroll_DrawLine:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r6, #144
	lsls r6, r6, #4
	sub sp, #44
	mov r10, r0
	adds r0, r6, #0
	str r1, [sp, #8]
	adds r7, r2, #0
	bl Runtime_BumpAllocateAlternatePool
	movs r1, #0
	movs r2, #192
	mov r3, r10
	str r0, [sp, #4]
	str r1, [sp, #0]
	mov r9, r2
	cmp r3, #0
	bne .L0
	movs r0, #1
	negs r0, r0
	b .L1
.L0:
	movs r5, #128
	lsls r5, r5, #2
	adds r0, r5, #0
	bl GameFlag_TestFar
	cmp r0, #0
	bne .L2
	ldr r3, [pc, #496]
	ldr r0, [sp, #4]
	adds r1, r6, #0
	movs r2, #0
	bl _call_via_r3
	adds r0, r5, #0
	bl GameFlag_SetBitFar
	b .L3
.L2:
	ldr r4, [sp, #4]
	movs r5, #128
	lsls r5, r5, #4
	movs r2, #128
	adds r1, r4, r5
	ldr r3, [pc, #472]
	lsls r2, r2, #1
	adds r0, r4, #0
	bl _call_via_r3
	movs r2, #128
	ldr r1, [sp, #4]
	lsls r2, r2, #1
	adds r0, r1, r2
	ldr r3, [pc, #448]
	adds r1, r5, #0
	movs r2, #0
	bl _call_via_r3
.L3:
	mov r4, r10
	ldrb r0, [r4]
	movs r3, #0
	mov r8, r3
	adds r4, #1
	cmp r0, #0
	beq .L4
	ldr r2, [pc, #432]
.L6:
	cmp r0, #31
	bls .L5
	adds r3, r0, #0
	subs r3, #32
	ldrb r3, [r2, r3]
	add r8, r3
.L5:
	ldrb r0, [r4]
	adds r4, #1
	cmp r0, #0
	bne .L6
.L4:
	cmp r7, #2
	bne .L7
	mov r4, r9
	mov r1, r8
	subs r4, r4, r1
	str r4, [sp, #0]
	b .L8
.L7:
	cmp r7, #1
	bne .L8
	mov r2, r9
	mov r4, r8
	subs r3, r2, r4
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #0]
.L8:
	mov r4, r10
	ldrb r0, [r4]
	movs r1, #0
	adds r4, #1
	mov r8, r1
	mov r10, r4
	cmp r0, #0
	beq .L9
.L15:
	cmp r0, #31
	bls .L10
	movs r2, #32
	negs r2, r2
	adds r2, r2, r0
	ldr r1, [pc, #356]
	lsls r3, r2, #3
	adds r4, r1, r3
	mov lr, r2
	ldr r1, [sp, #0]
	ldr r2, [sp, #4]
	adds r3, r2, r1
	mov r2, r8
	adds r1, r3, r2
	movs r3, #0
	mov r12, r3
	movs r2, #1
	movs r3, #15
	mov r11, r2
	mov r9, r3
.L13:
	ldr r3, [pc, #332]
	ldrb r7, [r4]
	movs r6, #128
	adds r4, #1
	movs r5, #7
	adds r2, r1, r3
.L12:
	adds r3, r7, #0
	ands r3, r6
	cmp r3, #0
	beq .L11
	mov r3, r11
	strb r3, [r2]
	mov r3, r9
	strb r3, [r1]
.L11:
	subs r5, #1
	adds r2, #1
	adds r1, #1
	lsrs r6, r6, #1
	cmp r5, #0
	bge .L12
	movs r2, #1
	add r12, r2
	mov r3, r12
	adds r1, #248
	cmp r3, #7
	ble .L13
	movs r3, #1
	cmp r0, #31
	bls .L14
	ldr r4, [pc, #264]
	mov r1, lr
	ldrb r3, [r4, r1]
.L14:
	add r8, r3
.L10:
	mov r2, r10
	ldrb r0, [r2]
	movs r3, #1
	add r10, r3
	cmp r0, #0
	bne .L15
.L9:
	movs r4, #24
	movs r2, #96
	mov r10, r4
	ldr r4, [sp, #4]
	mov r8, r2
	movs r6, #128
	movs r3, #7
	movs r2, #192
	adds r1, r4, #0
	movs r7, #96
	lsls r6, r6, #1
	mov r12, r3
	mov lr, r2
.L18:
	cmp r7, #0
	beq .L16
	mov r5, r8
	adds r2, r4, #0
.L17:
	ldrb r3, [r2, #1]
	ldrb r0, [r2]
	lsls r3, r3, #4
	orrs r0, r3
	subs r5, #1
	strb r0, [r1]
	adds r2, #2
	adds r4, #2
	adds r1, #1
	cmp r5, #0
	bne .L17
.L16:
	subs r3, r1, r7
	mov r2, lr
	adds r1, r3, r6
	subs r3, r4, r2
	adds r4, r3, r6
	movs r3, #1
	negs r3, r3
	add r12, r3
	mov r2, r12
	cmp r2, #0
	bge .L18
	mov r3, r10
	cmp r3, #0
	beq .L19
	ldr r4, [sp, #8]
	ldr r0, [sp, #4]
	lsls r1, r4, #5
	mov r12, r10
.L20:
	ldr r3, [pc, #164]
	ldr r4, [pc, #168]
	adds r2, r1, r3
	ldr r3, [r0]
	str r3, [r2]
	adds r2, r1, r4
	movs r4, #128
	lsls r4, r4, #1
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #128
	str r3, [r2]
	ldr r3, [pc, #148]
	lsls r4, r4, #2
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #192
	str r3, [r2]
	ldr r3, [pc, #140]
	lsls r4, r4, #2
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #128
	str r3, [r2]
	ldr r3, [pc, #128]
	lsls r4, r4, #3
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #160
	str r3, [r2]
	ldr r3, [pc, #120]
	lsls r4, r4, #3
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #192
	str r3, [r2]
	ldr r3, [pc, #108]
	lsls r4, r4, #3
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #224
	str r3, [r2]
	ldr r3, [pc, #100]
	lsls r4, r4, #3
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	str r3, [r2]
	movs r2, #1
	negs r2, r2
	add r12, r2
	mov r3, r12
	adds r1, #32
	adds r0, #4
	cmp r3, #0
	bne .L20
.L19:
	ldr r0, [sp, #4]
	bl Runtime_BumpFree
	movs r0, #0
.L1:
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte IwramFillWords
	.4byte IwramCopyWords
	.4byte DisplayScroll_GlyphWidths
	.4byte DisplayScroll_Font
	.4byte 0x00000101
	.4byte 0x06010000
	.4byte 0x06010004
	.4byte 0x06010008
	.4byte 0x0601000c
	.4byte 0x06010010
	.4byte 0x06010014
	.4byte 0x06010018
	.4byte 0x0601001c
	.size DisplayScroll_DrawLine, .-DisplayScroll_DrawLine
