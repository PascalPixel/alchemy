.syntax unified
	.thumb
	.global Func_08016544
	.thumb_func
Func_08016544:
	push {r5, lr}
	ldr r4, .L_080165ac
	movs r5, #0
	ldr r1, [r4, #40]
	ldrb r3, [r4, #11]
	strb r3, [r1]
	ldrb r2, [r4, #3]
	ldrb r3, [r4, #2]
	strh r5, [r1, #2]
	eors r3, r2
	strb r3, [r1, #1]
	movs r2, #132
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, #4
	adds r2, #6
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, [r4, #40]
	movs r1, #0
.L_08016570:
	ldrh r3, [r2]
	adds r1, #1
	adds r2, #2
	adds r5, r5, r3
	cmp r1, #13
	bls .L_08016570
	ldr r3, [r4, #40]
	mvns r2, r5
	strh r2, [r3, #2]
	ldrb r3, [r4]
	cmp r3, #0
	beq .L_0801658e
	ldr r2, .L_080165b0
	movs r3, #0
	strh r3, [r2]
.L_0801658e:
	movs r3, #1
	negs r3, r3
	str r3, [r4, #20]
	ldrb r3, [r4]
	cmp r3, #0
	beq .L_080165a6
	ldrb r3, [r4, #8]
	cmp r3, #0
	beq .L_080165a6
	ldr r2, .L_080165b0
	ldr r3, .L_080165a8
	strh r3, [r2]
.L_080165a6:
	pop {r5, pc}
.L_080165a8:
	.4byte 0x000000c0
.L_080165ac:
	.4byte Data_02005360
.L_080165b0:
	.4byte 0x0400010e
