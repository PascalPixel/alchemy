.syntax unified
	.thumb
	.global Func_080b005c
	.thumb_func
Func_080b005c:
	push {lr}
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrh r0, [r3]
	bl Owner_GetRecord
	adds r0, #42
	ldrb r1, [r0]
	cmp r1, #47
	bls .L_080b0074
	movs r1, #0
.L_080b0074:
	ldr r3, .L_080b0080
	lsls r2, r1, #1
	adds r2, r2, r1
	lsls r2, r2, #3
	ldr r0, [r3, r2]
	pop {pc}
.L_080b0080:
	.4byte Data_080c6684
