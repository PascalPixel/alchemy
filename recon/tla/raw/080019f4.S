@ Thumb loader of the OAM list builder (GRAPHICS/BUILD_OAM_LIST.S): it reads
@ {DMA3 register base, kernel address, control} from a PC-relative block
@ with add r1, pc and ldmia r1, {r1, r2, r4}, has DMA3 copy the 320-byte
@ kernel into a stack buffer and calls the copy.
@ It stays disassembly without credit: it calls through lr with the second
@ halfword of a Thumb bl alone (0xf800), which the assembler cannot write as
@ an instruction.
.syntax unified
	.thumb
	.global Render_BuildOamList
	.thumb_func
Render_BuildOamList:
	push	{lr}
	add	r1, pc, #16
	ldmia	r1, {r1, r2, r4}
	sub	sp, #0x140
	mov	r3, sp
	stmia	r1!, {r2, r3, r4}
	mov	lr, sp
	.inst.n	0xf800
	add	sp, #0x140
	pop	{pc}
	.4byte	0x040000d4
	.4byte	Render_BuildOamListKernelCode
	.4byte	0x84000050
