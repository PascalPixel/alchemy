.syntax unified
	.thumb
	.global Func_08125bb8
	.thumb_func
Func_08125bb8:
	push {r5, r6, lr}
	movs r1, #128
	movs r2, #1
	ldr r5, .L_08125c00
	lsls r1, r1, #1
	negs r2, r2
	adds r6, r0, #0
	mov lr, r5
	.2byte 0xf800
	movs r3, #128
	lsls r3, r3, #1
	adds r6, r6, r3
	adds r0, r6, #0
	movs r1, #128
	ldr r2, .L_08125c04
	mov lr, r5
	.2byte 0xf800
	movs r1, #128
	ldr r2, .L_08125c08
	lsls r1, r1, #10
	adds r6, #128
	movs r3, #0
	adds r1, #2
.L_08125be6:
	adds r3, #1
	stmia r6!, {r2}
	adds r2, r2, r1
	cmp r3, #239
	bls .L_08125be6
	movs r1, #160
	ldr r3, .L_08125c00
	adds r0, r6, #0
	lsls r1, r1, #2
	ldr r2, .L_08125c04
	mov lr, r3
	.2byte 0xf800
	pop {r5, r6, pc}
.L_08125c00:
	.4byte IwramFillWords
.L_08125c04:
	.4byte 0x03ff03ff
.L_08125c08:
	.4byte Data_02010200
