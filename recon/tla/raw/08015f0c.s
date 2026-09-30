.syntax unified
	.thumb
	.global Func_08015f0c
	.thumb_func
Func_08015f0c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #204
	ldr r3, [r3]
	adds r6, r0, #0
	adds r1, r3, #0
	movs r0, #0
	movs r5, #15
	movs r4, #0
	adds r1, #30
	adds r2, r3, #0
.L_08015f24:
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08015f3a
	ldrb r3, [r2, #15]
	cmp r6, r3
	bne .L_08015f3a
	ldrh r3, [r1]
	cmp r0, r3
	bcs .L_08015f3a
	adds r0, r3, #0
	adds r5, r4, #0
.L_08015f3a:
	adds r4, #3
	adds r1, #6
	adds r2, #3
	cmp r4, #14
	bls .L_08015f24
	adds r0, r5, #0
	pop {r5, r6, pc}
