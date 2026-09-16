@ Retired from C: no ordinary C source reproduced this owner. The note stays
@ in r0 and the next pointer's register is reused for the stored zero, an
@ allocation the approved compiler did not produce from any spelling tried.
@ Disassembled from the audited extent at 0x080f9a30 (32 bytes).
.syntax unified
	.thumb
	.global SoundNote_Detach
	.global Func_080f9a30
	.thumb_func
SoundNote_Detach:
Func_080f9a30:
	ldr	r3, [r0, #44]	@ 0x2c
	cmp	r3, #0
	beq.n	.L_080f9a4e
	ldr	r1, [r0, #52]	@ 0x34
	ldr	r2, [r0, #48]	@ 0x30
	cmp	r2, #0
	beq.n	.L_080f9a42
	str	r1, [r2, #52]	@ 0x34
	b.n	.L_080f9a44
.L_080f9a42:
	str	r1, [r3, #32]
.L_080f9a44:
	cmp	r1, #0
	beq.n	.L_080f9a4a
	str	r2, [r1, #48]	@ 0x30
.L_080f9a4a:
	movs	r1, #0
	str	r1, [r0, #44]	@ 0x2c
.L_080f9a4e:
	bx	lr
