.syntax unified
	.thumb
	.global Func_081185c4
	.thumb_func
Func_081185c4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r1, .L_0811872c
	ldr r7, .L_08118730
	movs r4, #0
	movs r2, #148
	sub sp, #4
	mov r9, r1
	mov r8, r2
	mov r10, r4
.L_081185de:
	ldr r5, .L_08118734
	str r4, [sp, #0]
	ldrb r0, [r5, r4]
	bl Owner_GetState
	ldr r4, [sp, #0]
	adds r6, r0, #0
	ldrb r0, [r5, r4]
	bl BattleParty_IsUnitListed
	ldr r4, [sp, #0]
	cmp r0, #0
	bne .L_081185fa
	b .L_08118710
.L_081185fa:
	mov r1, r9
	ldrb r3, [r1, r4]
	cmp r3, #0
	bne .L_08118604
	b .L_08118710
.L_08118604:
	cmp r4, #2
	bgt .L_08118626
	movs r2, #150
	mov r3, r10
	lsls r2, r2, #1
	add r3, r9
	adds r1, r6, r2
	adds r2, r3, #0
	adds r2, #8
	movs r0, #3
.L_08118618:
	ldrb r3, [r2]
	subs r0, #1
	strb r3, [r1]
	adds r2, #1
	adds r1, #1
	cmp r0, #0
	bge .L_08118618
.L_08118626:
	ldrb r2, [r7]
	movs r1, #152
	lsls r1, r1, #1
	adds r3, r6, r1
	strb r2, [r3]
	ldrb r3, [r7, #1]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r3, [r7, #2]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r3, [r7, #3]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r2, [r7, #4]
	adds r1, #1
	adds r3, r6, r1
	strb r2, [r3]
	ldrb r3, [r7, #5]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r3, [r7, #6]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r3, [r7, #7]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r2, [r7, #8]
	adds r1, #1
	adds r3, r6, r1
	strb r2, [r3]
	ldrb r3, [r7, #9]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r3, [r7, #10]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r3, [r7, #11]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r2, [r7, #12]
	adds r1, #1
	adds r3, r6, r1
	strb r2, [r3]
	ldrb r3, [r7, #13]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r3, [r7, #14]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r3, [r7, #15]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r2, [r7, #16]
	adds r1, #1
	adds r3, r6, r1
	strb r2, [r3]
	ldrb r3, [r7, #17]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r3, [r7, #18]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r3, [r7, #19]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r2, [r7, #20]
	adds r1, #1
	adds r3, r6, r1
	strb r2, [r3]
	ldrb r3, [r7, #21]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r3, [r7, #22]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r3, [r7, #23]
	adds r1, #1
	adds r2, r6, r1
	strb r3, [r2]
	ldrb r2, [r7, #24]
	adds r1, #1
	adds r3, r6, r1
	strb r2, [r3]
	cmp r4, #2
	ble .L_08118710
	mov r2, r8
	mov r3, r9
	ldrh r1, [r2, r3]
	adds r0, r6, #0
	str r4, [sp, #0]
	bl Owner_RecalculateRatiosFar + 0x8
	ldr r3, .L_0811872c
	adds r0, r6, #0
	add r3, r8
	ldrh r1, [r3, #2]
	bl Owner_RecalculateRatiosFar + 0x10
	ldr r4, [sp, #0]
.L_08118710:
	movs r1, #4
	adds r4, #1
	add r8, r1
	add r10, r1
	adds r7, #28
	cmp r4, #4
	bgt .L_08118720
	b .L_081185de
.L_08118720:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0811872c:
	.4byte Data_0200ff58
.L_08118730:
	.4byte Data_0200ff6c
.L_08118734:
	.4byte Data_0812a16c
