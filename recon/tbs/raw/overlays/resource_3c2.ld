/* resource_3c2: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_HEYA/TABLES.o"(.text)
        "*/resource_3c2_overlay.o"(.text.x0200806c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_HEYA/DIALOGUE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_HEYA/BLOWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_HEYA/STATE.o"(.text)
        "*/resource_3c2_overlay.o"(.text.x02008240)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_HEYA/RECORDS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_HEYA/IMPORT.o"(.text)
        "*/resource_3c2_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
