.syntax unified
	.thumb
	.global Func_080ca5d8
	.thumb_func
Func_080ca5d8:
	push {r5, r6, r7, lr}
	lsls r0, r0, #4
	adds r0, r0, r1
	lsls r0, r0, #16
	asrs r7, r0, #16
	movs r0, #182
	lsls r0, r0, #1
	ldr r5, .L_080ca69c
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ca5f4
	movs r6, #79
	b .L_080ca636
.L_080ca5f4:
	movs r2, #0
	ldrsh r3, [r5, r2]
	adds r5, #2
	lsls r2, r3, #16
	lsrs r1, r2, #16
	cmp r1, #0
	beq .L_080ca636
	lsls r3, r7, #16
	lsrs r0, r3, #16
	cmp r1, r0
	beq .L_080ca636
	movs r1, #240
	movs r4, #128
	lsls r1, r1, #4
	lsls r4, r4, #8
	adds r1, #255
	mov r12, r0
.L_080ca616:
	lsrs r2, r2, #16
	adds r3, r2, #0
	ands r3, r4
	cmp r3, #0
	beq .L_080ca624
	adds r6, r2, #0
	ands r6, r1
.L_080ca624:
	movs r2, #0
	ldrsh r3, [r5, r2]
	adds r5, #2
	lsls r2, r3, #16
	lsrs r3, r2, #16
	cmp r3, #0
	beq .L_080ca636
	cmp r3, r12
	bne .L_080ca616
.L_080ca636:
	lsls r3, r6, #16
	cmp r3, #0
	bne .L_080ca68c
	ldr r3, .L_080ca6a0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_080ca65e
	movs r0, #252
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	movs r6, #56
	cmp r0, #0
	beq .L_080ca68c
.L_080ca65e:
	movs r0, #4
	bl Func_080ca5a0
	movs r6, #50
	cmp r0, #0
	bne .L_080ca68c
	movs r0, #0
	bl Func_080ca5a0
	movs r6, #128
	lsls r6, r6, #2
	adds r6, #238
	cmp r0, #0
	bne .L_080ca68c
	movs r0, #5
	bl Func_080ca5a0
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	adds r6, r3, #0
	movs r3, #50
	subs r6, r3, r6
.L_080ca68c:
	ldr r3, .L_080ca6a0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #14
	adds r3, r3, r2
	strh r6, [r3]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ca69c:
	.4byte Data_080efd1c
.L_080ca6a0:
	.4byte gPartyState
