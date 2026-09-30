.syntax unified
	.thumb
	.global Func_080d17e8
	.thumb_func
Func_080d17e8:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	ldr r5, [r3]
	adds r1, r0, #0
	cmp r5, #0
	beq .L_080d1816
	movs r2, #224
	ldr r3, .L_080d1818
	lsls r2, r2, #1
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	movs r3, #224
	lsls r3, r3, #4
	movs r0, #128
	adds r2, r5, r3
	lsls r0, r0, #9
	adds r1, r5, #0
	movs r3, #1
	bl Func_080d0e1c
.L_080d1816:
	pop {r5, pc}
.L_080d1818:
	.4byte IwramCopyWords
