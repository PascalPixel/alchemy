.syntax unified
	.thumb
	.global Menu_SelectTopEntry
	.thumb_func
Menu_SelectTopEntry:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	movs r0, #1
	negs r0, r0
	movs r6, #0
	bl Party_SumDjinnCountsFar
	cmp r0, #0
	bne .L_0804d50c
	movs r6, #1
.L_0804d50c:
	lsls r3, r6, #1
	adds r3, r3, r6
	ldr r2, .L_0804d568
	lsls r7, r3, #1
	adds r3, r5, r7
	ldrsb r3, [r2, r3]
	subs r5, r3, #1
	cmp r5, #0
	bge .L_0804d520
	movs r5, #0
.L_0804d520:
	bl AffineEffect_InitializeWork
	movs r0, #1
	bl Menu_AppendResourceEntry
	cmp r6, #0
	bne .L_0804d534
	movs r0, #15
	bl Menu_AppendResourceEntry
.L_0804d534:
	movs r0, #2
	bl Menu_AppendResourceEntry
	movs r0, #7
	bl Menu_AppendResourceEntry
	movs r0, #17
	movs r1, #7
	movs r2, #0
	bl Menu_CenterResourceEntries
	adds r0, r5, #0
	bl Menu_RunResourceSelectionLoop
	adds r5, r0, #0
	bl Menu_EndResourceSelection
	cmp r5, #0
	blt .L_0804d562
	ldr r2, .L_0804d56c
	adds r3, r5, r7
	adds r3, #1
	ldrsb r5, [r2, r3]
.L_0804d562:
	adds r0, r5, #0
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0804d568:
	.4byte Data_0805f8b3
.L_0804d56c:
	.4byte Data_0805f8a7
