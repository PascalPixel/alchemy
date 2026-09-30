.syntax unified
	.thumb
	.global Func_08143354
	.thumb_func
Func_08143354:
	push {r5, r6, lr}
	ldr r6, .L_081433a4
	movs r1, #128
	movs r2, #1
	ldr r5, .L_081433a8
	adds r0, r6, #0
	lsls r1, r1, #1
	negs r2, r2
	mov lr, r5
	.2byte 0xf800
	movs r3, #128
	lsls r3, r3, #1
	adds r6, r6, r3
	adds r0, r6, #0
	movs r1, #128
	ldr r2, .L_081433ac
	mov lr, r5
	.2byte 0xf800
	movs r4, #128
	ldr r1, .L_081433b0
	lsls r4, r4, #10
	adds r6, #128
	movs r0, #0
	adds r4, #2
.L_08143384:
	movs r3, #0
.L_08143386:
	adds r3, #1
	stmia r6!, {r1}
	adds r1, r1, r4
	cmp r3, #8
	bne .L_08143386
	ldr r2, .L_081433ac
	movs r3, #0
.L_08143394:
	adds r3, #1
	stmia r6!, {r2}
	cmp r3, #8
	bne .L_08143394
	adds r0, #1
	cmp r0, #16
	bne .L_08143384
	pop {r5, r6, pc}
.L_081433a4:
	.4byte 0x0600f800
.L_081433a8:
	.4byte IwramFillWords
.L_081433ac:
	.4byte 0x03ff03ff
.L_081433b0:
	.4byte Data_02010200
