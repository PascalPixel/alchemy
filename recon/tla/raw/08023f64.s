.syntax unified
	.thumb
	.global Object_IsTargetUnset
	.thumb_func
Object_IsTargetUnset:
	push {lr}
	adds r3, r0, #0
	adds r3, #85
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_08023f7e
	ldr r3, [r0, #56]
	movs r2, #128
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_08023f90
	ldr r2, [r0, #60]
	b .L_08023f84
.L_08023f7e:
	ldr r2, [r0, #56]
	movs r3, #128
	lsls r3, r3, #24
.L_08023f84:
	cmp r2, r3
	bne .L_08023f90
	ldr r3, [r0, #64]
	movs r0, #1
	cmp r3, r2
	beq .L_08023f92
.L_08023f90:
	movs r0, #0
.L_08023f92:
	pop {pc}
