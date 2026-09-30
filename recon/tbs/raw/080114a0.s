.syntax unified
	.thumb
	.global Map_UpdateCurrentTileBlockUntilBlocked
	.thumb_func
Map_UpdateCurrentTileBlockUntilBlocked:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_08011560
	ldr r3, [r3]
	sub sp, #20
	movs r1, #0
	str r3, [sp, #16]
	str r1, [sp, #12]
	str r1, [sp, #8]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_080114ca
	ldmia r3!, {r2}
	str r2, [sp, #12]
	ldr r3, [r3, #4]
	str r3, [sp, #8]
.L_080114ca:
	ldr r1, [sp, #12]
	movs r2, #255
	lsls r2, r2, #24
	adds r3, r1, r2
	ldr r1, [sp, #8]
	ldr r2, .L_08011564
	asrs r3, r3, #25
	str r3, [sp, #12]
	adds r3, r1, r2
	asrs r3, r3, #25
	str r3, [sp, #8]
	movs r3, #0
	str r3, [sp, #4]
	mov r9, r3
.L_080114e6:
	ldr r2, [sp, #4]
	movs r1, #0
	ldr r6, [sp, #8]
	mov r10, r1
	mov r11, r2
.L_080114f0:
	adds r3, r6, #0
	movs r1, #15
	ands r3, r1
	movs r5, #0
	mov r8, r6
	lsls r7, r3, #4
.L_080114fc:
	ldr r2, [sp, #12]
	adds r1, r2, r5
	adds r3, r1, #0
	movs r2, #15
	ands r3, r2
	adds r4, r7, r3
	movs r2, #156
	lsls r3, r4, #1
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #16]
	ldrh r4, [r2, r3]
	movs r3, #0
	add r4, r11
	str r3, [sp, #0]
	mov r0, r9
	mov r2, r8
	adds r3, r4, #0
	bl Map_WriteLayerCellTile
	cmp r0, #0
	bne .L_0801154e
	adds r5, #1
	cmp r5, #1
	bls .L_080114fc
	movs r3, #1
	add r10, r3
	mov r1, r10
	adds r6, #1
	cmp r1, #1
	bls .L_080114f0
	ldr r2, [sp, #4]
	movs r3, #160
	lsls r3, r3, #1
	movs r1, #1
	adds r2, r2, r3
	add r9, r1
	str r2, [sp, #4]
	mov r2, r9
	cmp r2, #1
	bls .L_080114e6
.L_0801154e:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_08011560:
	.4byte gMapWork
.L_08011564:
	.4byte 0xfec00000
