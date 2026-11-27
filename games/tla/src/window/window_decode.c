/* @file */
/* The message decoder: a stored message expanded into the ring of decoded */
/* text, its arguments, names and numbers written out, and outside Japanese */
/* the articles, plurals and possessives each language puts around them. */
/* Part of window.c. */

/* The number of article kinds in each edition's table (articles.c), and the */
/* mask a name's kind is read through. */
#if (LANGUAGE == LANGUAGE_ITALIAN)
#define ARTICLE_KINDS 11
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define ARTICLE_KINDS 5
#else
#define ARTICLE_KINDS 8
#endif
#if (LANGUAGE == LANGUAGE_SPANISH)
#define ARTICLE_KIND_MASK 15
#else
#define ARTICLE_KIND_MASK 7
#endif

/* The article strings of one language, by kind, copied whole (articles.c). */
typedef struct ARTICLE_TABLE {
	u8 *text[ARTICLE_KINDS];
} ARTICLE_TABLE;

extern const ARTICLE_TABLE Article_Table;
#if (LANGUAGE == LANGUAGE_SPANISH)
extern const ARTICLE_TABLE Article_Indefinite;
#endif
#if (LANGUAGE == LANGUAGE_FRENCH)
/* The German "de" articles, elided and whole, copied whole (articles.c). */
typedef struct DE_ARTICLES {
	u8 *text[2];
} DE_ARTICLES;

extern const struct DE_ARTICLES Text_DeArticles;
#endif

/* The control codes of a message as stored (the PO catalogs' names), which */
/* mnMessageDecode expands into decoded text. */
enum {
	MESSAGE_END = 0,		/* kept as TEXT_END: the window stays open */
	MESSAGE_PAGE_BREAK = 1, /* kept as TEXT_PAGE */
	MESSAGE_END_WAIT =
		2, /* kept as TEXT_WAIT_KEY: a button closes the window */
	MESSAGE_LINE_BREAK = 3,	   /* kept as TEXT_NEWLINE */
	MESSAGE_TEXT_COLOR = 8,	   /* kept with its operand as TEXT_COLOR */
	MESSAGE_TEXT_EFFECT = 9,   /* kept with its operand as TEXT_OUTLINE */
	MESSAGE_LINE_SPACING = 10, /* kept with its operand as TEXT_LINE_SPACING */
	/* XXX (no evidence): what codes 11, 12 and 14 mean; every reader skips each
	 */
	/* with its operands (one, one and two), and only one message uses one of */
	/* them (12, in MsgLemuriaRoomsWhatInWhatsWithAll), so nothing the game */
	/* shows tells them apart. */
	/* German: a counted noun; Italian: a gender ending; Spanish and French: the
	 */
	/* plural suffix */
	MESSAGE_LOCALIZED_WORD = 13,
	/* Kept with its operand as TEXT_WINDOW_ANIMATION: the window animation to
	 */
	/* run. */
	MESSAGE_WINDOW_EFFECT = 15,
	/* The name of the unit MainRoutineStateWork.leaderActorId names. */
	MESSAGE_PROTAGONIST_NAME = 16,
	MESSAGE_ACTOR_NAME = 17, /* the name of the unit its operand names */
	/* The argument codes write out the next message argument of their kind */
	/* (MESSAGE_ARGUMENT_). */
	MESSAGE_ARGUMENT_ACTOR_NAME = 18,
	MESSAGE_ARGUMENT_CLASS_NAME = 19,
	MESSAGE_ARGUMENT_ITEM_NAME = 20,
	MESSAGE_ARGUMENT_ABILITY_NAME = 21,
	/* The number argument, with a minus sign when negative; outside Japanese */
	/* any number but -1, 0 and 1 makes what follows plural. */
	MESSAGE_ARGUMENT_SIGNED_NUMBER = 22,
	MESSAGE_ARGUMENT_LOCATION_NAME = 23,
	/* English: the plural suffix; German, Spanish, French and Italian: the */
	/* item's possessive */
	MESSAGE_PLURAL_SUFFIX = 25,
	/* A button drawn as two characters, codes BUTTON_ICON_FIRST_CODE + 2 * n */
	/* and the next for operand value n. */
	MESSAGE_BUTTON_ICON = 26,
	/* English: an apostrophe, and an s unless the name ended in s; German: an
	 */
	/* apostrophe after s or z, else an s; nothing in Spanish, French and */
	/* Italian. */
	MESSAGE_POSSESSIVE_SUFFIX = 27,
	/* The first letters of a unit's name, a stammer: its operands are the unit
	 */
	/* and how many letters (a kana keeps its mark). */
	MESSAGE_ACTOR_NAME_START = 28,
	/* The article class of the name it starts, in item and class names, which
	 */
	/* mnMessageAppendArticleName reads and drops. */
	MESSAGE_ARTICLE_CLASS = 29,
	/* Kept as TEXT_END_ALT, which the message ends as it ends TEXT_END. */
	MESSAGE_END_NOW = 30,
	/* The em dash, stored as the character ° rather than The Broken Seal's */
	/* code 24: TEXT_ARGUMENT_SPACE and a hyphen, and outside English a space */
	/* after. The Japanese decoder keeps it as it is. */
	MESSAGE_EM_DASH = 176,
};

/* What the decode flags of mnMessageSetDecodeFlags put between characters. */
#define ENTRY_NARROW_GAP TEXT_PAUSE_SHORT
/* With spacingAfter (English and Japanese): TEXT_MARK_VOICED after a character
 */
/* of codes WIDE_GAP_FIRST to WIDE_GAP_LAST, other than the kana marks and the
 */
/* Japanese full stop, comma and middle dot. */
#define ENTRY_WIDE_GAP TEXT_MARK_VOICED
/* The codes a wide gap follows: the Japanese font's upper half, codes 128 to */
/* 255, its kana (from を, code 0x86), to 256, its first kanji (神, */
/* charmap_tla_ja.tsv). */
#define WIDE_GAP_FIRST 128
#define WIDE_GAP_LAST  256
/* Halting speech breaks off with three dots at a space, or after this many */
/* codes. */
#define HALTING_SPEECH_LENGTH 10
/* An operand is stored one more than its value, so that none is 0, the end */
/* code; adding this takes the one off in the halfword written. */
#define OPERAND_UNBIAS ((u32)(u16)-1)
/* The Mystery Man's or Woman's name is decoded into ten halfwords, the end */
/* code's included. */
#define MYSTERY_NAME_CAPACITY 10
/* A possessive word is copied up to its sixteenth letter. */
#define POSSESSIVE_MAX_LETTERS 16
/* MESSAGE_BUTTON_ICON's first character code: a button's two characters are */
/* codes BUTTON_ICON_FIRST_CODE + 2 * n and the next. */
#define BUTTON_ICON_FIRST_CODE 128

/* The operand of an item, class or name code: how its name is written. */
typedef enum ARTICLE_FORM {
	ARTICLE_FORM_BARE = 0,	  /* the name alone */
	ARTICLE_FORM_ARTICLE = 1, /* with its article in front */
	/* English: the plural ending after it; elsewhere the article, and for an */
	/* item its plural name */
	ARTICLE_FORM_PLURAL = 2,
	/* The article while the last number was -1, 0 or 1, else as */
	/* ARTICLE_FORM_PLURAL */
	ARTICLE_FORM_BY_COUNT = 3,
	/* No article; an item's plural name, and in French "de" in front */
	ARTICLE_FORM_DE = 4,
	ARTICLE_FORM_MASK = 7,
	/* Spanish: the indefinite article rather than the definite */
	ARTICLE_FORM_INDEFINITE = 8
} ARTICLE_FORM;

#if (LANGUAGE == LANGUAGE_ENGLISH)
/* The English articles (articles.c) a name with no article class takes. */
enum {
	ARTICLE_A = 1,
	ARTICLE_AN = 2 /* before a capital vowel */
};

/* Copies a name into the decoded text at writePosition and returns the */
/* position after it: with its article in front (ARTICLE_FORM_ARTICLE, or */
/* ARTICLE_FORM_BY_COUNT for one) and an s or es after it (ARTICLE_FORM_PLURAL,
 */
/* or ARTICLE_FORM_BY_COUNT for several). With mode set, a space and a line */
/* spacing of TEXT_DEFAULT_LINE_SPACING go before the name, and a line spacing
 */
/* of 8 and a space after it. suffix is left 1 when the name ends in s. */
u32 mnMessageAppendArticleName(s32 mode, u16 *name, u32 writePosition,
							   u16 *entries, s32 articleForm, s32 plural,
							   s32 *suffix) {
	ARTICLE_TABLE table;
	s32 kind;
	u16 head;
	u16 c;
	u8 *article;
	s8 letter;
	s32 letters;

	if(mode != 0) {
		TEXT_APPEND(entries, writePosition, ' ');
		TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePosition, TEXT_DEFAULT_LINE_SPACING);
	}
	if(articleForm == ARTICLE_FORM_ARTICLE ||
	   (articleForm == ARTICLE_FORM_BY_COUNT && plural == 0)) {
		kind = 0;
		table = Article_Table;
		head = name[0];
		if(head == MESSAGE_ARTICLE_CLASS) {
			kind = name[1] - 1;
			name += 2;
		}
		if(kind == 0) {
			if(head == 'A' || head == 'I' || head == 'U' || head == 'E' ||
			   head == 'O')
				kind = ARTICLE_AN;
			else
				kind = ARTICLE_A;
		}
		article = table.text[kind & ARTICLE_KIND_MASK];
		for(letters = 0; letters < 8; letters++) {
			if((letter = *article++) == 0)
				break;
			TEXT_APPEND(entries, writePosition, letter);
		}
	}
	else {
		head = name[0];
		if(head == MESSAGE_ARTICLE_CLASS)
			name += 2;
	}
	while(*name != 0) {
		c = *name++;
		TEXT_APPEND(entries, writePosition, (s16)c);
		if(c == 'S' || c == 's')
			*suffix = 1;
		else
			*suffix = 0;
	}
	if(articleForm == ARTICLE_FORM_PLURAL ||
	   (articleForm == ARTICLE_FORM_BY_COUNT && plural != 0)) {
		if(*suffix != 0) {
			TEXT_APPEND(entries, writePosition, 'e');
		}
		TEXT_APPEND(entries, writePosition, 's');
	}
	if(mode != 0) {
		TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePosition, 8);
		TEXT_APPEND(entries, writePosition, ' ');
	}
	return writePosition;
}

#elif (LANGUAGE != LANGUAGE_JAPANESE)
/* Copies a name into the decoded text at writePosition and returns the */
/* position after it: ARTICLE_FORM_BARE and ARTICLE_FORM_DE take no article, */
/* the other forms the article of the name's class, its first letter a capital
 */
/* when suffix is 2 (a sentence's start); in French, ARTICLE_FORM_DE puts "de"
 */
/* (elided before a capital vowel) in front. mode sets the name apart as the */
/* English function does. suffix is left 1 when the name ends in s (in German */
/* also z or x). */
u32 mnMessageAppendArticleName(s32 mode, u16 *name, u32 writePosition,
							   u16 *entries, s32 articleForm, s32 plural,
							   s32 *suffix) {
	ARTICLE_TABLE table;
#if (LANGUAGE == LANGUAGE_SPANISH)
	ARTICLE_TABLE indefiniteTable;
	u16 count;
	s16 indefinite;
#endif
	s32 kind;
	u16 head;
	u16 c;
	u8 *article;
	s8 letter;
	s32 letters;

#if (LANGUAGE == LANGUAGE_SPANISH)
	count = articleForm & ARTICLE_FORM_MASK;
	indefinite = articleForm & ARTICLE_FORM_INDEFINITE;
#endif
	if(mode != 0) {
		TEXT_APPEND(entries, writePosition, ' ');
		TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePosition, TEXT_DEFAULT_LINE_SPACING);
	}
#if (LANGUAGE == LANGUAGE_FRENCH)
	if(articleForm == ARTICLE_FORM_DE) {
		struct DE_ARTICLES de;
		u8 *word;
		s8 letter;
		s32 n;

		head = name[0];
		de = Text_DeArticles;
		if(head == 'A' || head == 'I' || head == 'U' || head == 'E' ||
		   head == 'O')
			word = de.text[0];
		else
			word = de.text[1];
		for(n = 0; n < 8; n++) {
			if((letter = *word++) == 0)
				break;
			TEXT_APPEND(entries, writePosition, letter);
		}
	}
#endif
#if (LANGUAGE == LANGUAGE_SPANISH)
	if(count == ARTICLE_FORM_BARE || count == ARTICLE_FORM_DE) {
#else
	if(articleForm == ARTICLE_FORM_BARE || articleForm == ARTICLE_FORM_DE) {
#endif
		head = name[0];
		if(head == MESSAGE_ARTICLE_CLASS)
			name += 2;
	}
	else {
		kind = 0;
		table = Article_Table;
#if (LANGUAGE == LANGUAGE_SPANISH)
		indefiniteTable = Article_Indefinite;
#endif
		if(name[0] == MESSAGE_ARTICLE_CLASS) {
			kind = name[1];
			name += 2;
		}
#if (LANGUAGE == LANGUAGE_SPANISH)
		if(indefinite == 0)
			article = table.text[kind & ARTICLE_KIND_MASK];
		else
			article = indefiniteTable.text[kind & ARTICLE_KIND_MASK];
#else
		article = table.text[kind & ARTICLE_KIND_MASK];
#endif
		for(letters = 0; letters < 8; letters++) {
			if((letter = *article++) == 0)
				break;
			if(letters == 0 && *suffix == 2)
				letter -= 'a' - 'A';
			TEXT_APPEND(entries, writePosition, letter);
		}
	}
	while(*name != 0) {
		c = *name++;
#if (LANGUAGE == LANGUAGE_ITALIAN)
		TEXT_APPEND(entries, writePosition, c);
#else
		TEXT_APPEND(entries, writePosition, (s16)c);
#if (LANGUAGE == LANGUAGE_GERMAN)
		if(c == 's' || c == 'S' || c == 'z' || c == 'Z' || c == 'x' || c == 'X')
#else
		if(c == 'S' || c == 's')
#endif
			*suffix = 1;
		else
			*suffix = 0;
#endif
	}
	if(mode != 0) {
		TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePosition, 8);
		TEXT_APPEND(entries, writePosition, ' ');
	}
	return writePosition;
}
#endif

/* The European editions keep a second item name table after the first, */
/* read for the plural forms; its distance from MsgItemName is a message id. */
#define PLURAL_ITEM_NAMES_OFFSET 461

/* What suffix starts as: the European editions use 2 to capitalize the */
/* first letter of a sentence. */
#if TLA_PATCH_2
#define SUFFIX_START 2
#else
#define SUFFIX_START 0
#endif

/* The symbol decoder syRamCodeLoad copies to RAM to run, and its size, as this
 */
/* symbol's address. */
extern const u8 Text_DecodeSymbolCode[];
extern u8 Text_DecodeSymbolCodeSize[];

extern u8 MsgClassName[], MsgItemName[], MsgAbilityName[], MsgDebugAreaName[];

#if TLA_PATCH_2
#if LANGUAGE == LANGUAGE_FRENCH
extern const s8 LocalizedWord_ItemPronoun[];
#else
/* The possessives (articles.c), copied whole: Spanish {singular, plural}, */
/* German and Italian one for each item gender (LocalizedWord_ItemGenders). */
#if LANGUAGE == LANGUAGE_SPANISH
#define POSSESSIVE_COUNT 2
#else
#define POSSESSIVE_COUNT 4
#endif

/* The possessive endings, by kind, copied whole (articles.c). */
struct POSSESSIVE_TABLE {
	s8 *text[POSSESSIVE_COUNT];
};
extern const struct POSSESSIVE_TABLE LocalizedWord_ItemPossessives;
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_ITALIAN
extern const s8 LocalizedWord_ItemGenders[];
#endif
#if LANGUAGE == LANGUAGE_GERMAN
extern const u8 *const LocalizedWord_CountedNouns[];
#endif
#endif
#endif

u32 mnMessageArgumentLookup(u32 name, u32 clear);
u8 *mnStringFormatNum(u8 *buffer, s32 input, s32 width);

#if LANGUAGE != LANGUAGE_JAPANESE

u32 mnMessageAppendArticleName(s32 mode, u16 *name, u32 writePosition,
							   u16 *entries, s32 articleForm, s32 plural,
							   s32 *suffix);

/* Decodes message into the ring of decoded text (WIN_STATE.textBuf) after
 */
/* the text decoded last, and returns where it starts; message -1 returns the */
/* last decoded message's start again. Its argument codes take the message */
/* arguments (mnMessageArgumentPush), which clear set empties after. Halting */
/* speech (mnMessageSetDecodeFlags) puts a pause before each character and */
/* breaks the message off with "..." after its first word, or its first */
/* HALTING_SPEECH_LENGTH characters. */
s32 mnMessageDecode(s32 message, s32 clear) {
	WIN_STATE *state = WINDOW_STATE;
	u32 code = 0;
#if !TLA_PATCH_2
	s32 lineStart = 1;
#endif
	s32 quote = 0;
	u32 start = state->textBufEnd;
	u32 decodedCount = 0;
#if TLA_PATCH_2
	s32 afterItem;
#endif
	s32 plural = 0;
	s32 suffix = SUFFIX_START;
	s32 running = 1;
	u32 writePosition = start;
	u32 previousCode;
	u16 *entry = state->textBuf;
	s32 overflow = 0;
#if TLA_PATCH_2
	s32 form;
	s8 *word;
	s8 c;
#endif
	s32 (*decode)(TEXT_READER *);
	u32 nextPosition;
	s32 articleForm;
	u32 j;
	u32 i;
	u32 value;
	s32 argument;
	s32 magnitude;
	u32 length;
	UNIT *unit;
	u8 *numberText;
	u8 *nameBytes;
	u16 *nameOutput;
	u16 *p;
	u16 name[MESSAGE_NAME_HALFWORDS];
	u8 numberBuffer[TEXT_NUMBER_BUFFER_SIZE];
	TEXT_READER reader;
#if TLA_PATCH_2 && LANGUAGE != LANGUAGE_FRENCH
	struct POSSESSIVE_TABLE possessives;
#endif
	HEAP_SLOTS *heap;

#if TLA_PATCH_2 && !TLA_PATCH_3
	/* DEAD: German alone clears afterItem here, and the first code always */
	/* clears it before it is read; the ROM keeps the store. */
	afterItem = 0;
#endif
	if(message == -1) {
		start = state->lastMsgStart;
	}
	else {
		syRamCodeLoadSlot(HEAP_SLOT_MESSAGE_DECODER, Text_DecodeSymbolCode,
						  Text_DecodeSymbolCodeSize);
		heap = Ram_HeapSlots;
		decode = (s32 (*)(TEXT_READER *))heap->msgDecoder;
		mnMessageFind(&reader, message);

		do {
			previousCode = code;
			code = (u32)decode(&reader);
#if TLA_PATCH_2
			/* A full stop right after an item name that ends in one is skipped.
			 */
			if(previousCode == MESSAGE_ARGUMENT_ITEM_NAME && afterItem == 1) {
				if(code == '.')
					code = (u32)decode(&reader);
				afterItem = 0;
			}
			else {
				afterItem = 0;
			}
#endif
			if(code > TEXT_LAST_FONT_CODE)
				code = '@';

			if(overflow != 0) {
				if(code < TEXT_SPACE || code == MESSAGE_EM_DASH) {
					switch(code) {
					case MESSAGE_ARGUMENT_CLASS_NAME:
						decode(&reader);
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_CLASS, clear);
						break;
					case MESSAGE_END:
					case MESSAGE_END_WAIT:
					case MESSAGE_END_NOW:
						running = 0;
						break;
					case MESSAGE_ARGUMENT_SIGNED_NUMBER:
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_NUMBER, clear);
						break;
					case MESSAGE_ARGUMENT_ITEM_NAME:
						decode(&reader);
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_ITEM, clear);
						break;
					case MESSAGE_ARGUMENT_ABILITY_NAME:
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_ABILITY,
												clear);
						break;
					case MESSAGE_ARGUMENT_LOCATION_NAME:
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_LOCATION,
												clear);
						break;
					case MESSAGE_TEXT_COLOR:
					case MESSAGE_TEXT_EFFECT:
						decode(&reader);
						break;
					case MESSAGE_ACTOR_NAME:
						decode(&reader);
						break;
					case MESSAGE_ARGUMENT_ACTOR_NAME:
						decode(&reader);
						break;
					case MESSAGE_ARTICLE_CLASS:
						decode(&reader);
						break;
					case MESSAGE_PAGE_BREAK:
						running = 0;
						code = MESSAGE_END_WAIT;
						break;
					case MESSAGE_PROTAGONIST_NAME:
						break;
					case -1:
						break;
					default:
						break;
					}
				}
			}
			else {
#if TLA_PATCH_2
				if(state->haltingSpeech != 0) {
					TEXT_APPEND(entry, writePosition, ENTRY_NARROW_GAP);
				}
#else
				if(state->haltingSpeech != 0 && lineStart == 0 &&
				   code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED) {
					TEXT_APPEND(entry, writePosition, ENTRY_NARROW_GAP);
				}

				if(state->spacingAfter != 0 && lineStart == 0 &&
				   code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED &&
				   previousCode <= WIDE_GAP_LAST &&
				   previousCode >= WIDE_GAP_FIRST &&
				   previousCode != TEXT_MARK_VOICED &&
				   previousCode != TEXT_MARK_SEMIVOICED &&
				   previousCode != TEXT_SPACE &&
				   previousCode != TEXT_JAPANESE_MIDDLE_DOT &&
				   previousCode != TEXT_JAPANESE_FULL_STOP &&
				   previousCode != TEXT_JAPANESE_COMMA) {
					TEXT_APPEND(entry, writePosition, ENTRY_WIDE_GAP);
				}
#endif

				if(code >= TEXT_SPACE && code != MESSAGE_EM_DASH) {
					if(state->haltingSpeech != 0 &&
					   (code == TEXT_SPACE ||
						decodedCount > HALTING_SPEECH_LENGTH)) {
						TEXT_APPEND(entry, writePosition, '.');
						TEXT_APPEND(entry, writePosition, '.');
						TEXT_APPEND(entry, writePosition, '.');
						overflow = 1;
						if(decodedCount > HALTING_SPEECH_LENGTH)
							code = TEXT_SPACE;
					}
					if(code == '"') {
						quote ^= 1;
						if(quote != 0)
							code = TEXT_OPENING_QUOTE;
					}
					TEXT_APPEND(entry, writePosition, (u16)code);
#if TLA_PATCH_2
					/* suffix: 2 after a sentence ends, 1 after an s or z. */
#if LANGUAGE == LANGUAGE_SPANISH
					if(code != ' ' && code != TEXT_INVERTED_EXCLAMATION_MARK &&
					   code != TEXT_INVERTED_QUESTION_MARK) {
#else
					if(code != ' ') {
#endif
						suffix = 0;
						if(code == '.' || code == '!' || code == '?')
							suffix = 2;
						if(code == 's' || code == 'S' || code == 'z' ||
						   code == 'Z')
							suffix = 1;
					}
#else
					lineStart = 0;
#endif
				}
				else {
					switch(code) {
					case MESSAGE_END:
					case MESSAGE_END_WAIT:
					case MESSAGE_END_NOW:
						running = 0;
						break;
					case MESSAGE_TEXT_COLOR:
					case MESSAGE_TEXT_EFFECT:
					case MESSAGE_ARTICLE_CLASS:
						TEXT_APPEND(entry, writePosition, (u16)code);
						TEXT_APPEND(entry, writePosition,
									decode(&reader) + OPERAND_UNBIAS);
						break;
					case MESSAGE_ARGUMENT_SIGNED_NUMBER:
						value = mnMessageArgumentLookup(MESSAGE_ARGUMENT_NUMBER,
														clear);
						magnitude = value;
						if(magnitude < 0)
							magnitude = -value;
						plural = 1;
						if(magnitude <= 1)
							plural = 0;
						numberText = mnStringFormatNum(numberBuffer, value, 0);
						for(i = numberText - numberBuffer;
							i != TEXT_NUMBER_BUFFER_SIZE &&
							numberBuffer[i] != 0;
							i++) {
							TEXT_APPEND(entry, writePosition, numberBuffer[i]);
						}
						break;
					case MESSAGE_ARGUMENT_CLASS_NAME:
						articleForm = decode(&reader) - 1;
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_CLASS, clear);
						mnMessageDecodeRaw(argument + (s32)MsgClassName, name,
										   MESSAGE_NAME_HALFWORDS);
						writePosition = mnMessageAppendArticleName(
							0, name, writePosition, entry, articleForm, plural,
							&suffix);
						break;
					case MESSAGE_ARGUMENT_ITEM_NAME:
						articleForm = decode(&reader) - 1;
#if TLA_PATCH_2
						{
							s32 lastChar;
							u16 k;

#if LANGUAGE == LANGUAGE_SPANISH
							form = articleForm & ARTICLE_FORM_MASK;
#else
							form = articleForm;
#endif
							argument = mnMessageArgumentLookup(
										   MESSAGE_ARGUMENT_ITEM, clear) &
									   INVENTORY_ITEM_ID_MASK;
							if(form == ARTICLE_FORM_PLURAL ||
							   (form == ARTICLE_FORM_BY_COUNT && plural != 0) ||
							   form == ARTICLE_FORM_DE)
								mnMessageDecodeRaw(
									argument + (s32)(MsgItemName +
													 PLURAL_ITEM_NAMES_OFFSET),
									name, MESSAGE_NAME_HALFWORDS);
							else
								mnMessageDecodeRaw(argument + (s32)MsgItemName,
												   name,
												   MESSAGE_NAME_HALFWORDS);
							writePosition = mnMessageAppendArticleName(
								0, name, writePosition, entry, articleForm,
								plural, &suffix);
							for(k = 0; name[k] != 0; k++)
								lastChar = (s16)name[k];
							if((u16)lastChar == '.')
								afterItem = 1;
							else
								afterItem = 0;
						}
#else
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ITEM, clear);
						mnMessageDecodeRaw((argument & INVENTORY_ITEM_ID_MASK) +
											   (s32)MsgItemName,
										   name, MESSAGE_NAME_HALFWORDS);
						writePosition = mnMessageAppendArticleName(
							0, name, writePosition, entry, articleForm, plural,
							&suffix);
#endif
						break;
					case MESSAGE_ARGUMENT_ABILITY_NAME:
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ABILITY, clear);
						mnMessageDecodeRaw(argument + (s32)MsgAbilityName, name,
										   MESSAGE_NAME_HALFWORDS);
						p = name;
						nextPosition = writePosition;
						while(*p != 0) {
							entry[nextPosition] = *p;
							p++;
							nextPosition =
								(nextPosition + 1) & TEXT_BUFFER_MASK;
						}
						writePosition = nextPosition;
						break;
					case MESSAGE_ARGUMENT_LOCATION_NAME:
						mnMessageDecodeRaw(
							fdAreaGetNameIndexStub(
								mnMessageArgumentLookup(
									MESSAGE_ARGUMENT_LOCATION, clear),
								1) +
								(s32)MsgDebugAreaName,
							name, MESSAGE_NAME_HALFWORDS);
						p = name;
						nextPosition = writePosition;
						while(*p != 0) {
							entry[nextPosition] = *p;
							p++;
							nextPosition =
								(nextPosition + 1) & TEXT_BUFFER_MASK;
						}
						writePosition = nextPosition;
						break;
					case MESSAGE_PROTAGONIST_NAME:
						nameBytes =
							syUnitGet(MainRoutineStateWork.leaderActorId)->name;
						nameOutput = name;
						i = 0;
						do {
							*nameOutput = *nameBytes;
							i++;
							nameBytes++;
							nameOutput++;
						} while(i < UNIT_NAME_SIZE);
						writePosition = mnMessageAppendArticleName(
							0, name, writePosition, entry, 0, 0, &suffix);
						break;
					case MESSAGE_ARGUMENT_ACTOR_NAME:
						articleForm = decode(&reader) - 1;
						nameBytes = (syUnitGet(mnMessageArgumentLookup(
										 MESSAGE_ARGUMENT_ACTOR, clear)))
										->name;
						nameOutput = name;
						j = 0;
						do {
							*nameOutput = *nameBytes;
							j++;
							nameBytes++;
							nameOutput++;
						} while(j < UNIT_NAME_SIZE);
						writePosition = mnMessageAppendArticleName(
							0, name, writePosition, entry, articleForm, plural,
							&suffix);
						break;
					case MESSAGE_ACTOR_NAME:
						argument = decode(&reader) - 1;
						nameBytes = syUnitGet(argument)->name;
						nameOutput = name;
						j = 0;
						do {
							*nameOutput = *nameBytes;
							j++;
							nameBytes++;
							nameOutput++;
						} while(j < UNIT_NAME_SIZE);
						writePosition = mnMessageAppendArticleName(
							0, name, writePosition, entry, 0, 0, &suffix);
						break;
					case MESSAGE_ACTOR_NAME_START:
						argument = decode(&reader) - 1;
						unit = syUnitGet(argument);
						length = decode(&reader);
						nameBytes = unit->name;
						nameOutput = name;
#if TLA_PATCH_2
						/* value counts the bytes here. */
						for(value = 0; value < length; value++)
							*nameOutput++ = *nameBytes++;
#else
						for(j = 0; j < length; j++) {
							*nameOutput++ = *nameBytes++;
							if((u8)(*nameBytes - TEXT_MARK_VOICED) <= 1)
								*nameOutput++ = *nameBytes++;
						}
#endif
						*nameOutput = 0;
						p = name;
						nextPosition = writePosition;
						while(*p != 0) {
							entry[nextPosition] = *p;
							p++;
							nextPosition =
								(nextPosition + 1) & TEXT_BUFFER_MASK;
						}
						writePosition = nextPosition;
						break;
					case MESSAGE_BUTTON_ICON:
						argument = ((decode(&reader) - 1) << 1) +
								   BUTTON_ICON_FIRST_CODE;
						TEXT_APPEND(entry, writePosition, argument);
						TEXT_APPEND(entry, writePosition, argument + 1);
						break;
#if TLA_PATCH_2
					case MESSAGE_EM_DASH:
						TEXT_APPEND(entry, writePosition, ' ');
						TEXT_APPEND(entry, writePosition, '-');
						TEXT_APPEND(entry, writePosition, ' ');
						break;
#if LANGUAGE == LANGUAGE_GERMAN
					case MESSAGE_LOCALIZED_WORD:
						/* A counted noun, singular or plural, by its kind. */
						articleForm = decode(&reader) - 1;
						for(nameBytes = (u8 *)LocalizedWord_CountedNouns
								[(articleForm & 7) * 2 + plural];
							*nameBytes != 0; nameBytes++) {
							TEXT_APPEND(entry, writePosition, *nameBytes);
						}
						break;
#endif
#if LANGUAGE == LANGUAGE_ITALIAN
					case MESSAGE_LOCALIZED_WORD:
						/* A gender ending, singular or plural: o or i after
						 * operand 1, else */
						/* a or e. */
						if(decode(&reader) == 1) {
							if(plural == 0)
								entry[writePosition] = 'o';
							else
								entry[writePosition] = 'i';
						}
						else {
							if(plural == 0)
								entry[writePosition] = 'a';
							else
								entry[writePosition] = 'e';
						}
						writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
						break;
#endif
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
					case MESSAGE_LOCALIZED_WORD:
						if(plural != 0) {
							if(suffix != 0) {
								TEXT_APPEND(entry, writePosition, 'e');
							}
							TEXT_APPEND(entry, writePosition, 's');
						}
						break;
#endif
					case MESSAGE_PLURAL_SUFFIX:
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_ITALIAN
						/* The item's possessive, by its gender; c holds the
						 * gender */
						/* first. */
						argument =
							mnMessageArgumentLookup(MESSAGE_ARGUMENT_ITEM, 0);
						argument &= INVENTORY_ITEM_ID_MASK;
						possessives = LocalizedWord_ItemPossessives;
						c = LocalizedWord_ItemGenders[argument];
						word = possessives.text[c & 3];
#else
						/* DEAD: the item argument is read and dropped; the ROM
						 * keeps the */
						/* call. */
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_ITEM, 0);
#if LANGUAGE == LANGUAGE_FRENCH
						word = (s8 *)LocalizedWord_ItemPronoun;
#else
						possessives = LocalizedWord_ItemPossessives;
						word = possessives.text[plural];
#endif
#endif
						/* The word's first letter is a capital at a sentence's
						 * start; */
						/* value counts the letters here. */
						for(value = 0; (c = *word++) != 0;) {
							if(value == 0 && suffix == 2)
								c -= 'a' - 'A';
							TEXT_APPEND(entry, writePosition, c);
							value++;
							if(value >= POSSESSIVE_MAX_LETTERS)
								break;
						}
#if LANGUAGE != LANGUAGE_SPANISH
						suffix = 0;
#endif
						break;
					case MESSAGE_POSSESSIVE_SUFFIX:
#if LANGUAGE == LANGUAGE_GERMAN
						/* An apostrophe after s or z, else an s. */
						if(suffix == 1)
							entry[writePosition] = '\'';
						else
							entry[writePosition] = 's';
						writePosition = (writePosition + 1) & TEXT_BUFFER_MASK;
#endif
						break;
					case -1:
						break;
					case MESSAGE_PAGE_BREAK:
					default:
						TEXT_APPEND(entry, writePosition, (u16)code);
						break;
#else
					case MESSAGE_EM_DASH:
						TEXT_APPEND(entry, writePosition, TEXT_ARGUMENT_SPACE);
						TEXT_APPEND(entry, writePosition, '-');
						break;
					case MESSAGE_PLURAL_SUFFIX:
						if(plural != 0) {
							if(suffix != 0) {
								TEXT_APPEND(entry, writePosition, 'e');
							}
							TEXT_APPEND(entry, writePosition, 's');
						}
						break;
					case MESSAGE_POSSESSIVE_SUFFIX:
						TEXT_APPEND(entry, writePosition, '\'');
						if(suffix == 0) {
							TEXT_APPEND(entry, writePosition, 's');
						}
						break;
					case -1:
						break;
					case MESSAGE_PAGE_BREAK:
					case MESSAGE_LINE_BREAK:
						lineStart = 1;
					default:
						TEXT_APPEND(entry, writePosition, (u16)code);
						if(code == 's' || code == 'S')
							suffix = 1;
						else
							suffix = 0;
						break;
#endif
					}
				}
			}
			decodedCount++;
		} while(running != 0 && decodedCount <= TEXT_BUFFER_MASK);

		TEXT_APPEND(entry, writePosition, (u16)code);
		entry[writePosition] = 0;
		nextPosition = (writePosition + 1) & TEXT_BUFFER_MASK;
		state->textBufEnd = (u16)nextPosition;
		syHeapSlotRelease(HEAP_SLOT_MESSAGE_DECODER);
		state->lastMsgStart = (u16)start;
	}

	if(clear != 0)
		mnMessageArgumentClearAll();

	return start;
}

#endif

#if LANGUAGE == LANGUAGE_JAPANESE

extern u8 MsgWaitingForOpponent[];

/* なぞの男 ("Mystery Man"), the message after MsgWaitingForOpponent; なぞの女
 */
/* ("Mystery Woman") follows it. */
#define MYSTERY_MAN_MESSAGE ((s32)MsgWaitingForOpponent + 1)

/* Copies a name into the decoded text at writePosition and returns the */
/* position after it. With mode set, a space and a line spacing of 10 go before
 */
/* the name, and a line spacing of 8 and a space after it. The other editions'
 */
/* function of this name, at the top of this file, also writes articles; */
/* Japanese has none, and its decoder inlines this copy. */
static inline u32 mnMessageAppendArticleName(s32 mode, u16 *name,
											 u32 writePosition, u16 *entries,
											 s32 articleForm, s32 plural,
											 s32 *suffix) {
	u16 c;

	if(mode != 0) {
		TEXT_APPEND(entries, writePosition, ' ');
		TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePosition, TEXT_DEFAULT_LINE_SPACING);
	}
	while(*name != 0) {
		c = *name++;
		TEXT_APPEND(entries, writePosition, c);
	}
	if(mode != 0) {
		TEXT_APPEND(entries, writePosition, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePosition, 8);
		TEXT_APPEND(entries, writePosition, ' ');
	}
	return writePosition;
}

/* The Japanese mnMessageDecode: no articles or plurals, and a battle message */
/* (WINDOW_BATTLE_MESSAGE) sets its names off and doubles each space. Outside */
/* the field, the Mystery Man and Mystery Woman are named from */
/* MYSTERY_MAN_MESSAGE and the message after it rather than their unit names. */
s32 mnMessageDecode(s32 message, s32 clear) {
	WIN_STATE *state = WINDOW_STATE;
	u32 code = 0;
	s32 lineStart = 1;
	u32 start = state->textBufEnd;
	u32 decodedCount = 0;
	s32 running = 1;
	u32 writePosition = start;
	u16 *entry = state->textBuf;
	u8 mode = state->battleMode & WINDOW_BATTLE_MESSAGE;
	s32 (*decode)(TEXT_READER *);
	s32 overflow = 0;
	u32 previousCode;
	u32 nextPosition;
	u32 i;
	u32 j;
	s32 argument;
	s32 mysteryName;
	s32 value;
	u32 length;
	UNIT *unit;
	u8 *numberText;
	u8 *nameBytes;
	u16 *nameOutput;
	u16 name[MESSAGE_NAME_HALFWORDS];
	u8 numberBuffer[TEXT_NUMBER_BUFFER_SIZE];
	TEXT_READER reader;
	HEAP_SLOTS *heap;

	if(message == -1) {
		start = state->lastMsgStart;
	}
	else {
		syRamCodeLoadSlot(HEAP_SLOT_MESSAGE_DECODER, Text_DecodeSymbolCode,
						  Text_DecodeSymbolCodeSize);
		heap = Ram_HeapSlots;
		decode = (s32 (*)(TEXT_READER *))heap->msgDecoder;
		mnMessageFind(&reader, message);

		do {
			previousCode = code;
			code = (u32)decode(&reader);

			if(overflow != 0) {
				if(code < TEXT_SPACE) {
					switch(code) {
					case MESSAGE_ARGUMENT_CLASS_NAME:
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_CLASS, clear);
						break;
					case MESSAGE_END:
					case MESSAGE_END_WAIT:
					case MESSAGE_END_NOW:
						running = 0;
						break;
					case MESSAGE_TEXT_COLOR:
					case MESSAGE_TEXT_EFFECT:
						decode(&reader);
						break;
					case MESSAGE_ARGUMENT_SIGNED_NUMBER:
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_NUMBER, clear);
						break;
					case MESSAGE_ARGUMENT_ITEM_NAME:
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_ITEM, clear);
						break;
					case MESSAGE_ARGUMENT_ABILITY_NAME:
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_ABILITY,
												clear);
						break;
					case MESSAGE_ARGUMENT_LOCATION_NAME:
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_LOCATION,
												clear);
						break;
					case MESSAGE_ACTOR_NAME:
						decode(&reader);
						break;
					case MESSAGE_ARGUMENT_ACTOR_NAME:
						break;
					case MESSAGE_PAGE_BREAK:
						running = 0;
						code = MESSAGE_END_WAIT;
						break;
					case MESSAGE_PROTAGONIST_NAME:
						break;
					case -1:
						break;
					default:
						break;
					}
				}
			}
			else {
				if(state->haltingSpeech != 0 && lineStart == 0 &&
				   code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED) {
					TEXT_APPEND(entry, writePosition, ENTRY_NARROW_GAP);
				}

				if(state->spacingAfter != 0 && lineStart == 0 &&
				   code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED &&
				   previousCode <= WIDE_GAP_LAST &&
				   previousCode >= WIDE_GAP_FIRST &&
				   previousCode != TEXT_MARK_VOICED &&
				   previousCode != TEXT_MARK_SEMIVOICED &&
				   previousCode != TEXT_SPACE &&
				   previousCode != TEXT_JAPANESE_MIDDLE_DOT &&
				   previousCode != TEXT_JAPANESE_FULL_STOP &&
				   previousCode != TEXT_JAPANESE_COMMA) {
					TEXT_APPEND(entry, writePosition, ENTRY_WIDE_GAP);
				}

				if(code >= TEXT_SPACE) {
					if(mode != 0 && code == ' ') {
						TEXT_APPEND(entry, writePosition, (u16)code);
					}
					/* The ellipsis here is three middle dots, then a full stop.
					 */
					if(state->haltingSpeech != 0 &&
					   (code == TEXT_SPACE ||
						decodedCount > HALTING_SPEECH_LENGTH)) {
						TEXT_APPEND(entry, writePosition,
									TEXT_JAPANESE_MIDDLE_DOT);
						TEXT_APPEND(entry, writePosition,
									TEXT_JAPANESE_MIDDLE_DOT);
						TEXT_APPEND(entry, writePosition,
									TEXT_JAPANESE_MIDDLE_DOT);
						TEXT_APPEND(entry, writePosition,
									TEXT_JAPANESE_FULL_STOP);
						overflow = 1;
						if(decodedCount > HALTING_SPEECH_LENGTH)
							code = TEXT_SPACE;
					}
					TEXT_APPEND(entry, writePosition, (u16)code);
					lineStart = 0;
				}
				else {
					switch(code) {
					case MESSAGE_END:
					case MESSAGE_END_WAIT:
					case MESSAGE_END_NOW:
						running = 0;
						break;
					case MESSAGE_TEXT_COLOR:
					case MESSAGE_TEXT_EFFECT:
						TEXT_APPEND(entry, writePosition, (u16)code);
						TEXT_APPEND(entry, writePosition,
									decode(&reader) + OPERAND_UNBIAS);
						break;
					case MESSAGE_ARGUMENT_SIGNED_NUMBER:
						value = mnMessageArgumentLookup(MESSAGE_ARGUMENT_NUMBER,
														clear);
						numberText = mnStringFormatNum(numberBuffer, value, 0);
						i = numberText - numberBuffer;
						while(i != TEXT_NUMBER_BUFFER_SIZE &&
							  numberBuffer[i] != 0) {
							TEXT_APPEND(entry, writePosition, numberBuffer[i]);
							i++;
						}
						break;
					case MESSAGE_ARGUMENT_CLASS_NAME:
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_CLASS, clear);
						mnMessageDecodeRaw(argument + (s32)MsgClassName, name,
										   MESSAGE_NAME_HALFWORDS);
						writePosition = mnMessageAppendArticleName(
							mode, name, writePosition, entry, 0, 0, 0);
						break;
					case MESSAGE_ARGUMENT_ITEM_NAME:
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ITEM, clear);
						mnMessageDecodeRaw((argument & INVENTORY_ITEM_ID_MASK) +
											   (s32)MsgItemName,
										   name, MESSAGE_NAME_HALFWORDS);
						writePosition = mnMessageAppendArticleName(
							mode, name, writePosition, entry, 0, 0, 0);
						break;
					case MESSAGE_ARGUMENT_ABILITY_NAME:
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ABILITY, clear);
						mnMessageDecodeRaw(argument + (s32)MsgAbilityName, name,
										   MESSAGE_NAME_HALFWORDS);
						writePosition = mnMessageAppendArticleName(
							mode, name, writePosition, entry, 0, 0, 0);
						break;
					case MESSAGE_ARGUMENT_LOCATION_NAME:
						mnMessageDecodeRaw(
							fdAreaGetNameIndexStub(
								mnMessageArgumentLookup(
									MESSAGE_ARGUMENT_LOCATION, clear),
								1) +
								(s32)MsgDebugAreaName,
							name, MESSAGE_NAME_HALFWORDS);
						writePosition = mnMessageAppendArticleName(
							mode, name, writePosition, entry, 0, 0, 0);
						break;
					case MESSAGE_PROTAGONIST_NAME:
						nameBytes =
							syUnitGet(MainRoutineStateWork.leaderActorId)->name;
						nameOutput = name;
						i = 0;
						do {
							*nameOutput = *nameBytes;
							i++;
							nameBytes++;
							nameOutput++;
						} while(i < UNIT_NAME_SIZE);
						writePosition = mnMessageAppendArticleName(
							mode, name, writePosition, entry, 0, 0, 0);
						break;
					case MESSAGE_ARGUMENT_ACTOR_NAME:
						unit = syUnitGet(mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ACTOR, clear));
						if(state->battleMode != WINDOW_FIELD &&
						   (unit->enemyId == ENEMY_MYSTERY_MAN ||
							unit->enemyId == ENEMY_MYSTERY_WOMAN)) {
							mysteryName = MYSTERY_MAN_MESSAGE;
							if(unit->enemyId == ENEMY_MYSTERY_WOMAN)
								mysteryName++;
							mnMessageDecodeRaw(mysteryName, name,
											   MYSTERY_NAME_CAPACITY);
							writePosition = mnMessageAppendArticleName(
								mode, name, writePosition, entry, 0, 0, 0);
						}
						else {
							nameBytes = unit->name;
							nameOutput = name;
							j = 0;
							do {
								*nameOutput = *nameBytes;
								j++;
								nameBytes++;
								nameOutput++;
							} while(j < UNIT_NAME_SIZE);
							writePosition = mnMessageAppendArticleName(
								mode, name, writePosition, entry, 0, 0, 0);
						}
						break;
					case MESSAGE_ACTOR_NAME:
						argument = decode(&reader) - 1;
						nameBytes = syUnitGet(argument)->name;
						nameOutput = name;
						j = 0;
						do {
							*nameOutput = *nameBytes;
							j++;
							nameBytes++;
							nameOutput++;
						} while(j < UNIT_NAME_SIZE);
						writePosition = mnMessageAppendArticleName(
							mode, name, writePosition, entry, 0, 0, 0);
						break;
					case MESSAGE_ACTOR_NAME_START:
						argument = decode(&reader) - 1;
						unit = syUnitGet(argument);
						length = decode(&reader);
						nameBytes = unit->name;
						nameOutput = name;
						for(j = 0; j < length; j++) {
							*nameOutput++ = *nameBytes++;
							if((u8)(*nameBytes - TEXT_MARK_VOICED) <= 1)
								*nameOutput++ = *nameBytes++;
						}
						*nameOutput = 0;
						writePosition = mnMessageAppendArticleName(
							mode, name, writePosition, entry, 0, 0, 0);
						break;
					case -1:
						break;
					case MESSAGE_PAGE_BREAK:
					case MESSAGE_LINE_BREAK:
						lineStart = 1;
					default:
						TEXT_APPEND(entry, writePosition, (u16)code);
						break;
					}
				}
			}
			decodedCount++;
		} while(running != 0 && decodedCount <= TEXT_BUFFER_MASK);

		TEXT_APPEND(entry, writePosition, (u16)code);
		entry[writePosition] = 0;
		nextPosition = (writePosition + 1) & TEXT_BUFFER_MASK;
		state->textBufEnd = (u16)nextPosition;
		syHeapSlotRelease(HEAP_SLOT_MESSAGE_DECODER);
		state->lastMsgStart = (u16)start;
	}

	if(clear != 0)
		mnMessageArgumentClearAll();

	return start;
}

#endif

#undef ARTICLE_KINDS
#undef ARTICLE_KIND_MASK
#undef PLURAL_ITEM_NAMES_OFFSET
#undef SUFFIX_START
#undef POSSESSIVE_COUNT
#undef MYSTERY_MAN_MESSAGE
