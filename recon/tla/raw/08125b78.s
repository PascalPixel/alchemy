.syntax unified
	.thumb
	.global Func_08125b78
	.thumb_func
Func_08125b78:
	push {lr}
	ldr r2, .L_08125bac
	ldr r1, .L_08125bb0
	movs r3, #0
.L_08125b80:
	adds r3, #1
	stmia r0!, {r2}
	adds r2, r2, r1
	cmp r3, #63
	bls .L_08125b80
	ldr r2, .L_08125bac
	ldr r1, .L_08125bb0
	movs r3, #0
.L_08125b90:
	adds r3, #1
	stmia r0!, {r2}
	adds r2, r2, r1
	cmp r3, #55
	bls .L_08125b90
	movs r1, #136
	movs r2, #1
	ldr r3, .L_08125bb4
	lsls r1, r1, #2
	negs r2, r2
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_08125bac:
	.4byte 0x03020100
.L_08125bb0:
	.4byte 0x04040404
.L_08125bb4:
	.4byte IwramFillWords
