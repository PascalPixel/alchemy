@ Three words where the mixer saves r2, sl and sp before it moves the stack
@ to copy samples onto it (add r9, pc, #516; stm r9, {r2, sl, sp}) and
@ from which it restores them (add ip, pc, #212; ldm ip, {r2, r3, sp}).
@ Run-time variables carry no credit.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x0800110c
	.set AlchemyRunAddress, 0x0300099c
@ 終端分岐間の予約語。
	.space 0x0c, 0
