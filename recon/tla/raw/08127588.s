.syntax unified
	.thumb
	.global Func_08127588
	.thumb_func
Func_08127588:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r2, #0
	mov r10, r2
	mov r9, r2
	lsls r3, r0, #1
	ldr r2, .L_08127648
	adds r3, r3, r0
	lsls r3, r3, #3
	adds r1, r3, r2
	ldrb r3, [r1, #10]
	sub sp, #32
	movs r4, #0
	cmp r3, #0
	bne .L_081275be
	adds r2, r1, #0
	adds r2, #10
.L_081275b0:
	adds r4, #1
	cmp r4, #4
	bhi .L_081275be
	adds r2, #1
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_081275b0
.L_081275be:
	cmp r4, #5
	bne .L_081275c8
	movs r0, #1
	negs r0, r0
	b .L_0812763a
.L_081275c8:
	adds r7, r1, #0
	movs r3, #15
	adds r3, r3, r7
	movs r4, #0
	mov r8, r3
.L_081275d2:
	mov r2, r8
	ldrb r3, [r2]
	movs r2, #1
	add r8, r2
	cmp r3, #0
	beq .L_08127618
	ldrh r5, [r7]
	str r4, [sp, #0]
	adds r0, r5, #0
	adds r0, #8
	bl Owner_GetRecordFar
	adds r6, r0, #0
	ldr r4, [sp, #0]
	cmp r6, #0
	beq .L_08127618
	movs r0, #186
	lsls r0, r0, #1
	bl GameFlag_Test
	ldr r4, [sp, #0]
	cmp r0, #0
	bne .L_08127610
	movs r3, #193
	lsls r3, r3, #3
	adds r0, r5, r3
	bl GameFlag_Test
	ldr r4, [sp, #0]
	cmp r0, #0
	beq .L_0812762c
.L_08127610:
	ldrb r3, [r6, #15]
	movs r2, #1
	add r9, r3
	add r10, r2
.L_08127618:
	adds r4, #1
	adds r7, #2
	cmp r4, #4
	bls .L_081275d2
	mov r3, r10
	cmp r3, #0
	bne .L_08127632
	movs r0, #3
	negs r0, r0
	b .L_0812763a
.L_0812762c:
	movs r0, #2
	negs r0, r0
	b .L_0812763a
.L_08127632:
	mov r0, r9
	mov r1, r10
	bl __divsi3
.L_0812763a:
	add sp, #32
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08127648:
	.4byte Data_0812ce7c
