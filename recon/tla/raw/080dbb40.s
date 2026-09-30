.syntax unified
	.thumb
	.global Func_080dbb40
	.thumb_func
Func_080dbb40:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r5, [r3]
	movs r1, #224
	lsls r1, r1, #3
	adds r1, #242
	adds r2, r5, r1
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, #0
	beq .L_080dbb72
	movs r3, #0
	strh r3, [r2]
	ldr r0, .L_080dbb74
	bl Func_08014644
	movs r2, #254
	lsls r2, r2, #3
	adds r3, r5, r2
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Func_08014274
.L_080dbb72:
	pop {r5, pc}
.L_080dbb74:
	.4byte Func_080dba44
