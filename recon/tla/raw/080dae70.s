.syntax unified
	.thumb
	.global Func_080dae70
	.thumb_func
Func_080dae70:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #164
	ldr r4, [r3]
	movs r5, #0
	movs r2, #4
	ldrsh r3, [r4, r2]
	adds r1, r5, #0
	cmp r3, r0
	bne .L_080dae8a
	adds r5, r4, #4
	b .L_080daea2
.L_080dae8a:
	adds r1, #1
	cmp r1, #7
	bgt .L_080daea2
	lsls r3, r1, #3
	subs r3, r3, r1
	lsls r2, r3, #2
	adds r3, r2, #4
	ldrsh r3, [r4, r3]
	cmp r3, r0
	bne .L_080dae8a
	adds r3, r4, r2
	adds r5, r3, #4
.L_080daea2:
	adds r0, r5, #0
	pop {r5, r6, pc}
	.2byte 0x0000
