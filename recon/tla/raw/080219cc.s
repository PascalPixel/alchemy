.syntax unified
	.thumb
	.global Func_080219cc
	.thumb_func
Func_080219cc:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldrb r2, [r5, #27]
	movs r6, #0
	cmp r6, r2
	bge .L_08021a7e
.L_080219d8:
	lsls r3, r6, #2
	adds r3, #40
	ldr r4, [r5, r3]
	cmp r4, #0
	beq .L_08021a78
	ldr r3, [r4, #16]
	cmp r3, #0
	beq .L_08021a78
.L_080219e8:
	ldrb r3, [r4, #20]
	ldr r1, [r4, #16]
	adds r2, r3, #1
	strb r2, [r4, #20]
	lsls r3, r3, #24
	lsrs r3, r3, #24
	ldrb r0, [r1, r3]
	adds r3, r2, #1
	strb r3, [r4, #20]
	lsls r2, r2, #24
	adds r3, r0, #0
	lsrs r2, r2, #24
	subs r3, #239
	ldrb r1, [r1, r2]
	cmp r3, #16
	bhi .L_08021a66
	ldr r2, .L_08021a80
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08021a10:
	.4byte .L_08021a6a
	.4byte .L_08021a54
	.4byte .L_08021a6a
	.4byte .L_08021a5c
	.4byte .L_08021a58
	.4byte .L_080219e8
	.4byte .L_080219e8
	.4byte .L_080219e8
	.4byte .L_080219e8
	.4byte .L_080219e8
	.4byte .L_080219e8
	.4byte .L_080219e8
	.4byte .L_08021a66
	.4byte .L_080219e8
	.4byte .L_08021a6a
	.4byte .L_08021a6a
	.4byte .L_08021a60
.L_08021a54:
	strb r1, [r4, #4]
	b .L_080219e8
.L_08021a58:
	strb r1, [r5, #22]
	b .L_080219e8
.L_08021a5c:
	strb r1, [r5, #23]
	b .L_080219e8
.L_08021a60:
	movs r3, #255
	strb r3, [r4, #23]
	b .L_080219e8
.L_08021a66:
	strb r0, [r4, #23]
	b .L_080219e8
.L_08021a6a:
	ldrb r3, [r4, #20]
	ldrb r2, [r5, #27]
	adds r3, #254
	strb r3, [r4, #20]
	movs r3, #1
	strb r3, [r5, #25]
	strh r3, [r4, #2]
.L_08021a78:
	adds r6, #1
	cmp r6, r2
	blt .L_080219d8
.L_08021a7e:
	pop {r5, r6, pc}
.L_08021a80:
	.4byte .L_08021a10
