.syntax unified
	.thumb
	.global Func_080443d8
	.thumb_func
Func_080443d8:
	push {r5, r6, r7, lr}
	adds r6, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	ldr r7, [sp, #20]
	mov r12, r3
	movs r5, #12
	ldrsh r3, [r0, r5]
	ldr r4, [sp, #16]
	adds r3, r1, r3
	adds r1, r3, #1
	movs r5, #14
	ldrsh r3, [r0, r5]
	lsls r7, r7, #12
	adds r3, r2, r3
	adds r2, r3, #1
	cmp r1, #0
	bge .L_08044402
	adds r6, r6, r1
	movs r1, #0
.L_08044402:
	adds r3, r1, r6
	cmp r3, #29
	ble .L_0804440c
	movs r3, #30
	subs r6, r3, r1
.L_0804440c:
	cmp r2, #0
	bge .L_08044414
	adds r4, r4, r2
	movs r2, #0
.L_08044414:
	adds r3, r2, r4
	cmp r3, #29
	ble .L_0804441e
	movs r3, #20
	subs r4, r3, r2
.L_0804441e:
	cmp r6, #0
	ble .L_08044458
	cmp r4, #0
	ble .L_08044458
	lsls r3, r1, #1
	lsls r2, r2, #6
	add r3, r12
	adds r0, r2, r3
.L_0804442e:
	adds r1, r0, #0
	adds r2, r6, #0
	adds r1, #8
	cmp r2, #0
	beq .L_0804444a
	ldr r5, .L_0804445c
.L_0804443a:
	ldrh r3, [r1]
	subs r2, #1
	ands r3, r5
	orrs r3, r7
	strh r3, [r1]
	adds r1, #2
	cmp r2, #0
	bne .L_0804443a
.L_0804444a:
	subs r4, #1
	adds r0, #64
	cmp r4, #0
	bne .L_0804442e
	movs r3, #1
	mov r2, r12
	strb r3, [r2, #3]
.L_08044458:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0804445c:
	.4byte 0xffffefff
