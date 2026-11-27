/* @file */
/* The save signature and sector header template, and link test data nothing */
/* reads. */

#include "system.h"

/* The signature that marks a save part: sySaveInitState */
/* counts a sector as a save only when its first seven bytes match it. */
const u8 Save_Signature[] = "CAMELOT";
/* The sector header's first eight bytes: the signature, then a part id byte */
/* each writer replaces. */
const u8 Save_HeaderTemplate[8] = {'C', 'A', 'M', 'E', 'L', 'O', 'T', 'T'};

/* Unused: a word and two strings for a link test, which nothing reads. */
const u32 Link_TestWord = 0;
const u8 Link_TestDataA[] = "Trans Data A: This is a pen.";
const u8 Link_TestDataB[] = "Trans Data B: Hello world.";
