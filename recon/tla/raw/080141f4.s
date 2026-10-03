.syntax unified
	.thumb
	.global Func_080141f4
	.thumb_func
Func_080141f4:
	push {lr}
	ldr r4, .L_0801421c
	movs r2, #128
	movs r1, #0
	movs r0, #0
	lsls r2, r2, #2
.L_08014200:
	ldrb r3, [r4]
	adds r4, #1
	cmp r3, #255
	beq .L_0801420c
	movs r1, #0
	b .L_08014214
.L_0801420c:
	adds r1, #1
	cmp r0, r1
	bge .L_08014214
	adds r0, r1, #0
.L_08014214:
	subs r2, #1
	cmp r2, #0
	bne .L_08014200
	pop {pc}
.L_0801421c:
	.4byte ResourceBlockOwners
