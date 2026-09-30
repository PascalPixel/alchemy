.syntax unified
	.thumb
	.global Func_081089e4
	.thumb_func
Func_081089e4:
	push {r5, lr}
	adds r5, r0, #0
	ldr r4, [r5]
	cmp r4, #0
	beq .L_08108a82
	movs r1, #8
	ldrsh r3, [r5, r1]
	ldrh r2, [r4, #6]
	subs r0, r2, r3
	adds r3, r0, #0
	cmp r0, #0
	bge .L_081089fe
	adds r3, r0, #3
.L_081089fe:
	asrs r3, r3, #2
	cmp r3, #0
	bge .L_08108a06
	negs r3, r3
.L_08108a06:
	cmp r0, #0
	ble .L_08108a1c
	cmp r3, #0
	beq .L_08108a12
	subs r3, r2, r3
	b .L_08108a2a
.L_08108a12:
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	b .L_08108a2a
.L_08108a1c:
	cmp r0, #0
	bge .L_08108a40
	cmp r3, #0
	beq .L_08108a28
	adds r3, r2, r3
	b .L_08108a2a
.L_08108a28:
	adds r3, r2, #1
.L_08108a2a:
	strh r3, [r4, #6]
	ldrh r3, [r4, #6]
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	ands r2, r3
	ldrh r1, [r4, #22]
	ldr r3, .L_08108a84
	ands r3, r1
	orrs r3, r2
	strh r3, [r4, #22]
.L_08108a40:
	movs r1, #10
	ldrsh r3, [r5, r1]
	ldrh r2, [r4, #8]
	subs r0, r2, r3
	adds r3, r0, #0
	cmp r0, #0
	bge .L_08108a50
	adds r3, r0, #3
.L_08108a50:
	asrs r3, r3, #2
	cmp r3, #0
	bge .L_08108a58
	negs r3, r3
.L_08108a58:
	cmp r0, #0
	ble .L_08108a6e
	cmp r3, #0
	beq .L_08108a64
	subs r3, r2, r3
	b .L_08108a7c
.L_08108a64:
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	b .L_08108a7c
.L_08108a6e:
	cmp r0, #0
	bge .L_08108a82
	cmp r3, #0
	beq .L_08108a7a
	adds r3, r2, r3
	b .L_08108a7c
.L_08108a7a:
	adds r3, r2, #1
.L_08108a7c:
	strh r3, [r4, #8]
	ldrh r3, [r4, #8]
	strb r3, [r4, #20]
.L_08108a82:
	pop {r5, pc}
.L_08108a84:
	.4byte 0xfffffe00
