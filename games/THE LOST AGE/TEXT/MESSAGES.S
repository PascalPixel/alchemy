@ The message archive: the edition's catalog in this directory, encoded when
@ the ROM is built, as pret's data files include the assembly its tools write
@ from editable sources. The archive's address words name its own labels, and
@ each message the code names defines Msg<Name> as its number.
	.section .rodata
	.include "text/messages.inc"
