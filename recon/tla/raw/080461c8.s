.syntax unified
	.thumb
	.global Func_080461c8
	.thumb_func
Func_080461c8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r5, r0, #0
	movs r1, #0
	movs r0, #1
	sub sp, #4
	ldr r7, [r3, #36]
	bl BattleParty_ListActorIdsFar
	mov r8, r0
	lsls r3, r0, #1
	add r3, r8
	lsls r3, r3, #1
	movs r0, #29
	subs r0, r0, r3
	movs r3, #15
	str r3, [sp, #0]
	movs r1, #0
	movs r3, #5
	movs r2, #25
	bl Func_08046134
	ldrh r3, [r5]
	movs r6, #0
	cmp r3, #255
	beq .L_08046256
	movs r0, #0
.L_08046204:
	movs r3, #88
	ldrsh r2, [r7, r3]
	ldrh r3, [r0, r5]
	movs r1, #0
	b .L_0804621c
.L_0804620e:
	adds r1, #1
	cmp r1, #3
	bgt .L_08046226
	lsls r3, r1, #1
	adds r3, #88
	ldrsh r2, [r7, r3]
	ldrh r3, [r0, r5]
.L_0804621c:
	cmp r2, r3
	beq .L_08046226
	cmp r2, #255
	bne .L_0804620e
	movs r1, #4
.L_08046226:
	cmp r1, #4
	beq .L_08046246
	mov r3, r8
	subs r2, r3, r1
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #1
	movs r0, #29
	subs r0, r0, r3
	movs r3, #14
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #7
	movs r3, #5
	bl Func_08046134
.L_08046246:
	adds r6, #1
	cmp r6, #3
	bgt .L_08046256
	lsls r3, r6, #1
	adds r0, r3, #0
	ldrh r3, [r0, r5]
	cmp r3, #255
	bne .L_08046204
.L_08046256:
	movs r0, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
