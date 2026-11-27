/* The message decoder: a stored message expanded into the ring of decoded text,
   its arguments, names and numbers written out, and outside Japanese the
   articles, plurals and possessives each language puts around them. Part of
   window.c. */

/* The number of article kinds in each edition's table (articles.c), and the
   mask a name's kind is read through. */
#define ARTICLE_KINDS	  BY_LANGUAGE(8, 8, 8, 5, 8, 11)
#define ARTICLE_KIND_MASK SPANISH_OR(15, 7)

/* The article strings of one language, by kind, copied whole (articles.c). */
typedef struct ARTICLE_TABLE {
	Uint8 *text[ARTICLE_KINDS];
} ARTICLE_TABLE;

extern const ARTICLE_TABLE ArticleTable;
#if (LANGUAGE == LANGUAGE_SPANISH)
extern const ARTICLE_TABLE ArticleIndefinite;
#endif
#if (LANGUAGE == LANGUAGE_FRENCH)
/* The German "de" articles, elided and whole, copied whole (articles.c). */
typedef struct DE_ARTICLES {
	Uint8 *text[2];
} DE_ARTICLES;

extern const struct DE_ARTICLES TextDeArticles;
#endif

/* The control codes of a message as stored (the PO catalogs' names), which
   mnMessageDecode expands into decoded text. */
enum {
	MESSAGE_END = 0,		/* kept as TEXT_END: the window stays open */
	MESSAGE_PAGE_BREAK = 1, /* kept as TEXT_PAGE */
	/* kept as TEXT_WAIT_KEY: a button closes the window */
	MESSAGE_END_WAIT = 2,
	MESSAGE_LINE_BREAK = 3,	   /* kept as TEXT_NEWLINE */
	MESSAGE_TEXT_COLOR = 8,	   /* kept with its operand as TEXT_COLOR */
	MESSAGE_TEXT_EFFECT = 9,   /* kept with its operand as TEXT_OUTLINE */
	MESSAGE_LINE_SPACING = 10, /* kept with its operand as TEXT_LINE_SPACING */
	/* XXX (no evidence): what codes 11, 12 and 14 mean; every reader skips each
	   with its operands (one, one and two), and only one message uses one of
	   them (12, in MsgLemuriaRoomsWhatInWhatsWithAll), so nothing the game
	   shows tells them apart. German: a counted noun; Italian: a gender ending;
	   Spanish and French: the plural suffix */
	MESSAGE_LOCALIZED_WORD = 13,
	/* Kept with its operand as TEXT_WINDOW_ANIMATION: the window animation to
	   run. */
	MESSAGE_WINDOW_EFFECT = 15,
	/* The name of the unit MainRoutineStateWork.leaderActorId names. */
	MESSAGE_PROTAGONIST_NAME = 16,
	MESSAGE_ACTOR_NAME = 17, /* the name of the unit its operand names */
	/* The argument codes write out the next message argument of their kind
	   (MESSAGE_ARGUMENT_). */
	MESSAGE_ARGUMENT_ACTOR_NAME = 18,
	MESSAGE_ARGUMENT_CLASS_NAME = 19,
	MESSAGE_ARGUMENT_ITEM_NAME = 20,
	MESSAGE_ARGUMENT_ABILITY_NAME = 21,
	/* The number argument, with a minus sign when negative; outside Japanese
	   any number but -1, 0 and 1 makes what follows plural. */
	MESSAGE_ARGUMENT_SIGNED_NUMBER = 22,
	MESSAGE_ARGUMENT_LOCATION_NAME = 23,
	/* English: the plural suffix; German, Spanish, French and Italian: the
	   item's possessive */
	MESSAGE_PLURAL_SUFFIX = 25,
	/* A button drawn as two characters, codes BUTTON_ICON_FIRST_CODE + 2 * n
	   and the next for operand value n. */
	MESSAGE_BUTTON_ICON = 26,
	/* English: an apostrophe, and an s unless the name ended in s; German: an
	   apostrophe after s or z, else an s; nothing in Spanish, French and
	   Italian. */
	MESSAGE_POSSESSIVE_SUFFIX = 27,
	/* The first letters of a unit's name, a stammer: its operands are the unit
	   and how many letters (a kana keeps its mark). */
	MESSAGE_ACTOR_NAME_START = 28,
	/* The article class of the name it starts, in item and class names, which
	   mnMessageAppendArticleName reads and drops. */
	MESSAGE_ARTICLE_CLASS = 29,
	/* Kept as TEXT_END_ALT, which the message ends as it ends TEXT_END. */
	MESSAGE_END_NOW = 30,
	/* The em dash, stored as the character ° rather than The Broken Seal's
	   code 24: TEXT_ARGUMENT_SPACE and a hyphen, and outside English a space
	   after. The Japanese decoder keeps it as it is. */
	MESSAGE_EM_DASH = 176,
};

/* What the decode flags of mnMessageSetDecodeFlags put between characters. */
#define ENTRY_NARROW_GAP	   TEXT_PAUSE_SHORT
/* With spacingAfter (English and Japanese): TEXT_MARK_VOICED after a character
   of codes WIDE_GAP_FIRST to WIDE_GAP_LAST, other than the kana marks and the
   Japanese full stop, comma and middle dot. */
#define ENTRY_WIDE_GAP		   TEXT_MARK_VOICED
/* The codes a wide gap follows: the Japanese font's upper half, codes 128 to
   255, its kana (from を, code 0x86), to 256, its first kanji (神,
   charmap_tla_ja.tsv). */
#define WIDE_GAP_FIRST		   128
#define WIDE_GAP_LAST		   256
/* Halting speech breaks off with three dots at a space, or after this many
   codes. */
#define HALTING_SPEECH_LENGTH  10
/* An operand is stored one more than its value, so that none is 0, the end
   code; adding this takes the one off in the halfword written. */
#define OPERAND_UNBIAS		   ((Uint32)(Uint16)-1)
/* The Mystery Man's or Woman's name is decoded into ten halfwords, the end
   code's included. */
#define MYSTERY_NAME_CAPACITY  10
/* A possessive word is copied up to its sixteenth letter. */
#define POSSESSIVE_MAX_LETTERS 16
/* MESSAGE_BUTTON_ICON's first character code: a button's two characters are
   codes BUTTON_ICON_FIRST_CODE + 2 * n and the next. */
#define BUTTON_ICON_FIRST_CODE 128

/* The operand of an item, class or name code: how its name is written. */
typedef enum ARTICLE_FORM {
	ARTICLE_FORM_BARE = 0,	  /* the name alone */
	ARTICLE_FORM_ARTICLE = 1, /* with its article in front */
	/* English: the plural ending after it; elsewhere the article, and for an
	   item its plural name */
	ARTICLE_FORM_PLURAL = 2,
	/* The article while the last number was -1, 0 or 1, else as
	   ARTICLE_FORM_PLURAL */
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

/* Copies a name into the decoded text at writePos and returns the position
   after it: with its article in front (ARTICLE_FORM_ARTICLE, or
   ARTICLE_FORM_BY_COUNT for one) and an s or es after it (ARTICLE_FORM_PLURAL,
   or ARTICLE_FORM_BY_COUNT for several). With mode set, a space and a line
   spacing of TEXT_DEFAULT_LINE_SPACING go before the name, and a line spacing
   of 8 and a space after it. suffix is left 1 when the name ends in s. */
Uint32 mnMessageAppendArticleName(Sint32 mode, Uint16 *name, Uint32 writePos,
	Uint16 *entries, Sint32 articleForm, Sint32 plural, Sint32 *suffix)
{
	ARTICLE_TABLE tbl;
	Sint32		  kind;
	Uint16		  head;
	Uint16		  c;
	Uint8		 *article;
	Sint8		  letter;
	Sint32		  letters;

	if(mode != 0) {
		TEXT_APPEND(entries, writePos, ' ');
		TEXT_APPEND(entries, writePos, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePos, TEXT_DEFAULT_LINE_SPACING);
	}
	if(articleForm == ARTICLE_FORM_ARTICLE ||
		(articleForm == ARTICLE_FORM_BY_COUNT && plural == 0)) {
		kind = 0;
		tbl = ArticleTable;
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
		article = tbl.text[kind & ARTICLE_KIND_MASK];
		for(letters = 0; letters < 8; letters++) {
			if((letter = *article++) == 0)
				break;
			TEXT_APPEND(entries, writePos, letter);
		}
	} else {
		head = name[0];
		if(head == MESSAGE_ARTICLE_CLASS)
			name += 2;
	}
	while(*name != 0) {
		c = *name++;
		TEXT_APPEND(entries, writePos, (Sint16)c);
		if(c == 'S' || c == 's')
			*suffix = 1;
		else
			*suffix = 0;
	}
	if(articleForm == ARTICLE_FORM_PLURAL ||
		(articleForm == ARTICLE_FORM_BY_COUNT && plural != 0)) {
		if(*suffix != 0) {
			TEXT_APPEND(entries, writePos, 'e');
		}
		TEXT_APPEND(entries, writePos, 's');
	}
	if(mode != 0) {
		TEXT_APPEND(entries, writePos, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePos, 8);
		TEXT_APPEND(entries, writePos, ' ');
	}
	return writePos;
}

#elif (LANGUAGE != LANGUAGE_JAPANESE)
/* Copies a name into the decoded text at writePos and returns the position
   after it: ARTICLE_FORM_BARE and ARTICLE_FORM_DE take no article, the other
   forms the article of the name's class, its first letter a capital when suffix
   is 2 (a sentence's start); in French, ARTICLE_FORM_DE puts "de" (elided
   before a capital vowel) in front. mode sets the name apart as the English
   function does. suffix is left 1 when the name ends in s (in German also z or
   x). */
Uint32 mnMessageAppendArticleName(Sint32 mode, Uint16 *name,
	Uint32 writePosition, Uint16 *entries, Sint32 articleForm, Sint32 plural,
	Sint32 *suffix)
{
	ARTICLE_TABLE table;
#if (LANGUAGE == LANGUAGE_SPANISH)
	ARTICLE_TABLE indefiniteTable;
	Uint16		  count;
	Sint16		  indefinite;
#endif
	Sint32 kind;
	Uint16 head;
	Uint16 c;
	Uint8 *article;
	Sint8  letter;
	Sint32 letters;

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
		Uint8			  *word;
		Sint8			   letter;
		Sint32			   n;

		head = name[0];
		de = TextDeArticles;
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
	} else {
		kind = 0;
		table = ArticleTable;
#if (LANGUAGE == LANGUAGE_SPANISH)
		indefiniteTable = ArticleIndefinite;
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
		TEXT_APPEND(entries, writePosition, (Sint16)c);
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

/* The European editions keep a second item name table after the first, read for
   the plural forms; its distance from MsgItemName is a message id. */
#define PLURAL_ITEM_NAMES_OFFSET 461

/* What suffix starts as: the European editions use 2 to capitalize the first
   letter of a sentence. */
#define SUFFIX_START BY_LANGUAGE(0, 0, 2, 2, 2, 2)

/* The symbol decoder RAM_CODE_LOAD copies to RAM to run, and its size, as this
   symbol's address. */
extern const Uint8 Text_DecodeSymbolCode[];
extern Uint8	   Text_DecodeSymbolCodeSize[];

extern Uint8 MsgClassName[], MsgItemName[], MsgAbilityName[],
	MsgDebugAreaName[];

#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
#if LANGUAGE == LANGUAGE_FRENCH
extern const Sint8 LocalizedWordItemPronoun[];
#else
/* The possessives (articles.c), copied whole: Spanish {singular, plural},
   German and Italian one for each item gender (LocalizedWordItemGenders). */
#define POSSESSIVE_COUNT SPANISH_OR(2, 4)

/* The possessive endings, by kind, copied whole (articles.c). */
struct POSSESSIVE_TABLE {
	Sint8 *text[POSSESSIVE_COUNT];
};
extern const struct POSSESSIVE_TABLE LocalizedWordItemPossessives;
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_ITALIAN
extern const Sint8 LocalizedWordItemGenders[];
#endif
#if LANGUAGE == LANGUAGE_GERMAN
extern const Uint8 *const LocalizedWordCountedNouns[];
#endif
#endif
#endif

Uint32 mnMessageArgumentLookup(Uint32 name, Uint32 clear);
Uint8 *mnStringFormatNum(Uint8 *buf, Sint32 input, Sint32 width);

#if LANGUAGE != LANGUAGE_JAPANESE

Uint32 mnMessageAppendArticleName(Sint32 mode, Uint16 *name, Uint32 writePos,
	Uint16 *entries, Sint32 articleForm, Sint32 plural, Sint32 *suffix);

/* Decodes message into the ring of decoded text (WIN_STATE.textBuf) after the
   text decoded last, and returns where it starts; message -1 returns the last
   decoded message's start again. Its argument codes take the message arguments
   (mnMessageArgumentPush), which clear set empties after. Halting speech
   (mnMessageSetDecodeFlags) puts a pause before each character and breaks the
   message off with "..." after its first word, or its first
   HALTING_SPEECH_LENGTH characters. */
Sint32 mnMessageDecode(Sint32 message, Sint32 clear)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint32	   code = 0;
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
	Sint32 lineStart = 1;
#endif
	Sint32 quote = 0;
	Uint32 start = state->textBufEnd;
	Uint32 decodedCount = 0;
#if NO_DOUBLE_FULL_STOP_AFTER_ITEM_NAME
	Sint32 afterItem;
#endif
	Sint32	plural = 0;
	Sint32	suffix = SUFFIX_START;
	Bool	running = TRUE;
	Uint32	writePosition = start;
	Uint32	previousCode;
	Uint16 *entry = state->textBuf;
	Bool	overflow = FALSE;
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
	Sint32 form;
	Sint8 *word;
	Sint8  c;
#endif
	Sint32 (*decode)(TEXT_READER *);
	Uint32		nextPosition;
	Sint32		articleForm;
	Uint32		j;
	Uint32		i;
	Uint32		value;
	Sint32		argument;
	Sint32		magnitude;
	Uint32		length;
	UNIT	   *unit;
	Uint8	   *numberText;
	Uint8	   *nameBytes;
	Uint16	   *nameOutput;
	Uint16	   *p;
	Uint16		name[MESSAGE_NAME_HALFWORDS];
	Uint8		numberBuffer[TEXT_NUMBER_BUFFER_SIZE];
	TEXT_READER reader;
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_ITALIAN
	struct POSSESSIVE_TABLE possessives;
#endif
	HEAP_SLOTS *heap;

#if AFTER_ITEM_EARLY_CLEAR
	/* DEAD: German alone clears afterItem here, and the first code always
	   clears it before it is read; the ROM keeps the store. */
	afterItem = 0;
#endif
	if(message == -1) {
		start = state->lastMsgStart;
	} else {
		RAM_CODE_LOAD_SLOT(HEAP_SLOT_MESSAGE_DECODER, Text_DecodeSymbolCode,
			Text_DecodeSymbolCodeSize);
		heap = Ram_HeapSlots;
		decode = (Sint32 (*)(TEXT_READER *))heap->msgDecoder;
		mnMessageFind(&reader, message);

		do {
			previousCode = code;
			code = (Uint32)decode(&reader);
#if NO_DOUBLE_FULL_STOP_AFTER_ITEM_NAME
			/* A full stop right after an item name that ends in one is
			   skipped. */
			if(previousCode == MESSAGE_ARGUMENT_ITEM_NAME && afterItem == 1) {
				if(code == '.')
					code = (Uint32)decode(&reader);
				afterItem = 0;
			} else {
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
						running = FALSE;
						break;
					case MESSAGE_ARGUMENT_SIGNED_NUMBER:
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_NUMBER, clear);
						break;
					case MESSAGE_ARGUMENT_ITEM_NAME:
						decode(&reader);
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_ITEM, clear);
						break;
					case MESSAGE_ARGUMENT_ABILITY_NAME:
						mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ABILITY, clear);
						break;
					case MESSAGE_ARGUMENT_LOCATION_NAME:
						mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_LOCATION, clear);
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
						running = FALSE;
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
			} else {
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
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
						overflow = TRUE;
						if(decodedCount > HALTING_SPEECH_LENGTH)
							code = TEXT_SPACE;
					}
					if(code == '"') {
						quote ^= 1;
						if(quote != 0)
							code = TEXT_OPENING_QUOTE;
					}
					TEXT_APPEND(entry, writePosition, (Uint16)code);
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
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
				} else {
					switch(code) {
					case MESSAGE_END:
					case MESSAGE_END_WAIT:
					case MESSAGE_END_NOW:
						running = FALSE;
						break;
					case MESSAGE_TEXT_COLOR:
					case MESSAGE_TEXT_EFFECT:
					case MESSAGE_ARTICLE_CLASS:
						TEXT_APPEND(entry, writePosition, (Uint16)code);
						TEXT_APPEND(entry, writePosition,
							decode(&reader) + OPERAND_UNBIAS);
						break;
					case MESSAGE_ARGUMENT_SIGNED_NUMBER:
						value = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_NUMBER, clear);
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
						mnMessageDecodeRaw(argument + (Sint32)MsgClassName,
							name, MESSAGE_NAME_HALFWORDS);
						writePosition = mnMessageAppendArticleName(0, name,
							writePosition, entry, articleForm, plural, &suffix);
						break;
					case MESSAGE_ARGUMENT_ITEM_NAME:
						articleForm = decode(&reader) - 1;
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
						{
							Sint32 lastChar;
							Uint16 k;

#if LANGUAGE == LANGUAGE_SPANISH
							form = articleForm & ARTICLE_FORM_MASK;
#else
							form = articleForm;
#endif
							argument = mnMessageArgumentLookup(
										   MESSAGE_ARGUMENT_ITEM, clear) &
								INVENTORY_ITEM_ID_MASK;
							if(form == ARTICLE_FORM_PLURAL ||
								(form == ARTICLE_FORM_BY_COUNT &&
									plural != 0) ||
								form == ARTICLE_FORM_DE)
								mnMessageDecodeRaw(argument +
										(Sint32)(MsgItemName +
											PLURAL_ITEM_NAMES_OFFSET),
									name, MESSAGE_NAME_HALFWORDS);
							else
								mnMessageDecodeRaw(
									argument + (Sint32)MsgItemName, name,
									MESSAGE_NAME_HALFWORDS);
							writePosition = mnMessageAppendArticleName(0, name,
								writePosition, entry, articleForm, plural,
								&suffix);
							for(k = 0; name[k] != 0; k++)
								lastChar = (Sint16)name[k];
							if((Uint16)lastChar == '.')
								afterItem = 1;
							else
								afterItem = 0;
						}
#else
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ITEM, clear);
						mnMessageDecodeRaw((argument & INVENTORY_ITEM_ID_MASK) +
								(Sint32)MsgItemName,
							name, MESSAGE_NAME_HALFWORDS);
						writePosition = mnMessageAppendArticleName(0, name,
							writePosition, entry, articleForm, plural, &suffix);
#endif
						break;
					case MESSAGE_ARGUMENT_ABILITY_NAME:
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ABILITY, clear);
						mnMessageDecodeRaw(argument + (Sint32)MsgAbilityName,
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
					case MESSAGE_ARGUMENT_LOCATION_NAME:
						mnMessageDecodeRaw(
							fdAreaGetNameIndexStub(
								mnMessageArgumentLookup(
									MESSAGE_ARGUMENT_LOCATION, clear),
								1) +
								(Sint32)MsgDebugAreaName,
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
						writePosition = mnMessageAppendArticleName(0, name,
							writePosition, entry, articleForm, plural, &suffix);
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
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
						/* value counts the bytes here. */
						for(value = 0; value < length; value++)
							*nameOutput++ = *nameBytes++;
#else
						for(j = 0; j < length; j++) {
							*nameOutput++ = *nameBytes++;
							if((Uint8)(*nameBytes - TEXT_MARK_VOICED) <= 1)
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
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
					case MESSAGE_EM_DASH:
						TEXT_APPEND(entry, writePosition, ' ');
						TEXT_APPEND(entry, writePosition, '-');
						TEXT_APPEND(entry, writePosition, ' ');
						break;
#if LANGUAGE == LANGUAGE_GERMAN
					case MESSAGE_LOCALIZED_WORD:
						/* A counted noun, singular or plural, by its kind. */
						articleForm = decode(&reader) - 1;
						for(nameBytes = (Uint8 *)LocalizedWordCountedNouns
								[(articleForm & 7) * 2 + plural];
							*nameBytes != 0; nameBytes++) {
							TEXT_APPEND(entry, writePosition, *nameBytes);
						}
						break;
#endif
#if LANGUAGE == LANGUAGE_ITALIAN
					case MESSAGE_LOCALIZED_WORD:
						/* A gender ending, singular or plural: o or i after
						   operand 1, else a or e. */
						if(decode(&reader) == 1) {
							if(plural == 0)
								entry[writePosition] = 'o';
							else
								entry[writePosition] = 'i';
						} else {
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
						   gender first. */
						argument =
							mnMessageArgumentLookup(MESSAGE_ARGUMENT_ITEM, 0);
						argument &= INVENTORY_ITEM_ID_MASK;
						possessives = LocalizedWordItemPossessives;
						c = LocalizedWordItemGenders[argument];
						word = possessives.text[c & 3];
#else
						/* DEAD: the item argument is read and dropped; the ROM
						   keeps the call. */
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_ITEM, 0);
#if LANGUAGE == LANGUAGE_FRENCH
						word = (Sint8 *)LocalizedWordItemPronoun;
#else
						possessives = LocalizedWordItemPossessives;
						word = possessives.text[plural];
#endif
#endif
						/* The word's first letter is a capital at a sentence's
						   start; value counts the letters here. */
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
						TEXT_APPEND(entry, writePosition, (Uint16)code);
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
						TEXT_APPEND(entry, writePosition, (Uint16)code);
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

		TEXT_APPEND(entry, writePosition, (Uint16)code);
		entry[writePosition] = 0;
		nextPosition = (writePosition + 1) & TEXT_BUFFER_MASK;
		state->textBufEnd = (Uint16)nextPosition;
		syHeapSlotRelease(HEAP_SLOT_MESSAGE_DECODER);
		state->lastMsgStart = (Uint16)start;
	}

	if(clear != 0)
		mnMessageArgumentClearAll();

	return start;
}

#endif

#if LANGUAGE == LANGUAGE_JAPANESE

extern Uint8 MsgWaitingForOpponent[];

/* なぞの男 ("Mystery Man"), the message after MsgWaitingForOpponent;
   なぞの女 ("Mystery Woman") follows it. */
#define MYSTERY_MAN_MESSAGE ((Sint32)MsgWaitingForOpponent + 1)

/* Copies a name into the decoded text at writePos and returns the position
   after it. With mode set, a space and a line spacing of 10 go before the name,
   and a line spacing of 8 and a space after it. The other editions' function of
   this name, at the top of this file, also writes articles; Japanese has none,
   and its decoder inlines this copy. */
static inline Uint32 mnMessageAppendArticleName(Sint32 mode, Uint16 *name,
	Uint32 writePos, Uint16 *entries, Sint32 articleForm, Sint32 plural,
	Sint32 *suffix)
{
	Uint16 c;

	if(mode != 0) {
		TEXT_APPEND(entries, writePos, ' ');
		TEXT_APPEND(entries, writePos, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePos, TEXT_DEFAULT_LINE_SPACING);
	}
	while(*name != 0) {
		c = *name++;
		TEXT_APPEND(entries, writePos, c);
	}
	if(mode != 0) {
		TEXT_APPEND(entries, writePos, TEXT_LINE_SPACING);
		TEXT_APPEND(entries, writePos, 8);
		TEXT_APPEND(entries, writePos, ' ');
	}
	return writePos;
}

/* The Japanese mnMessageDecode: no articles or plurals, and a battle message
   (WINDOW_BATTLE_MESSAGE) sets its names off and doubles each space. Outside
   the field, the Mystery Man and Mystery Woman are named from
   MYSTERY_MAN_MESSAGE and the message after it rather than their unit names. */
Sint32 mnMessageDecode(Sint32 msg, Sint32 clear)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint32	   code = 0;
	Sint32	   lineStart = 1;
	Uint32	   start = state->textBufEnd;
	Uint32	   decodedCnt = 0;
	Bool	   running = TRUE;
	Uint32	   writePos = start;
	Uint16	  *entry = state->textBuf;
	Uint8	   mode = state->battleMode & WINDOW_BATTLE_MESSAGE;
	Sint32 (*decode)(TEXT_READER *);
	Bool		overflow = FALSE;
	Uint32		prevCode;
	Uint32		nextPos;
	Uint32		i;
	Uint32		j;
	Sint32		argument;
	Sint32		mysteryName;
	Sint32		value;
	Uint32		length;
	UNIT	   *unit;
	Uint8	   *numText;
	Uint8	   *nameBytes;
	Uint16	   *nameOutput;
	Uint16		name[MESSAGE_NAME_HALFWORDS];
	Uint8		numBuf[TEXT_NUMBER_BUFFER_SIZE];
	TEXT_READER reader;
	HEAP_SLOTS *heap;

	if(msg == -1) {
		start = state->lastMsgStart;
	} else {
		RAM_CODE_LOAD_SLOT(HEAP_SLOT_MESSAGE_DECODER, Text_DecodeSymbolCode,
			Text_DecodeSymbolCodeSize);
		heap = Ram_HeapSlots;
		decode = (Sint32 (*)(TEXT_READER *))heap->msgDecoder;
		mnMessageFind(&reader, msg);

		do {
			prevCode = code;
			code = (Uint32)decode(&reader);

			if(overflow != 0) {
				if(code < TEXT_SPACE) {
					switch(code) {
					case MESSAGE_ARGUMENT_CLASS_NAME:
						mnMessageArgumentLookup(MESSAGE_ARGUMENT_CLASS, clear);
						break;
					case MESSAGE_END:
					case MESSAGE_END_WAIT:
					case MESSAGE_END_NOW:
						running = FALSE;
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
						mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ABILITY, clear);
						break;
					case MESSAGE_ARGUMENT_LOCATION_NAME:
						mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_LOCATION, clear);
						break;
					case MESSAGE_ACTOR_NAME:
						decode(&reader);
						break;
					case MESSAGE_ARGUMENT_ACTOR_NAME:
						break;
					case MESSAGE_PAGE_BREAK:
						running = FALSE;
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
			} else {
				if(state->haltingSpeech != 0 && lineStart == 0 &&
					code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED) {
					TEXT_APPEND(entry, writePos, ENTRY_NARROW_GAP);
				}

				if(state->spacingAfter != 0 && lineStart == 0 &&
					code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED &&
					prevCode <= WIDE_GAP_LAST && prevCode >= WIDE_GAP_FIRST &&
					prevCode != TEXT_MARK_VOICED &&
					prevCode != TEXT_MARK_SEMIVOICED &&
					prevCode != TEXT_SPACE &&
					prevCode != TEXT_JAPANESE_MIDDLE_DOT &&
					prevCode != TEXT_JAPANESE_FULL_STOP &&
					prevCode != TEXT_JAPANESE_COMMA) {
					TEXT_APPEND(entry, writePos, ENTRY_WIDE_GAP);
				}

				if(code >= TEXT_SPACE) {
					if(mode != 0 && code == ' ') {
						TEXT_APPEND(entry, writePos, (Uint16)code);
					}
					/* The ellipsis here is three middle dots, then a full
					   stop. */
					if(state->haltingSpeech != 0 &&
						(code == TEXT_SPACE ||
							decodedCnt > HALTING_SPEECH_LENGTH)) {
						TEXT_APPEND(entry, writePos, TEXT_JAPANESE_MIDDLE_DOT);
						TEXT_APPEND(entry, writePos, TEXT_JAPANESE_MIDDLE_DOT);
						TEXT_APPEND(entry, writePos, TEXT_JAPANESE_MIDDLE_DOT);
						TEXT_APPEND(entry, writePos, TEXT_JAPANESE_FULL_STOP);
						overflow = TRUE;
						if(decodedCnt > HALTING_SPEECH_LENGTH)
							code = TEXT_SPACE;
					}
					TEXT_APPEND(entry, writePos, (Uint16)code);
					lineStart = 0;
				} else {
					switch(code) {
					case MESSAGE_END:
					case MESSAGE_END_WAIT:
					case MESSAGE_END_NOW:
						running = FALSE;
						break;
					case MESSAGE_TEXT_COLOR:
					case MESSAGE_TEXT_EFFECT:
						TEXT_APPEND(entry, writePos, (Uint16)code);
						TEXT_APPEND(
							entry, writePos, decode(&reader) + OPERAND_UNBIAS);
						break;
					case MESSAGE_ARGUMENT_SIGNED_NUMBER:
						value = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_NUMBER, clear);
						numText = mnStringFormatNum(numBuf, value, 0);
						i = numText - numBuf;
						while(i != TEXT_NUMBER_BUFFER_SIZE && numBuf[i] != 0) {
							TEXT_APPEND(entry, writePos, numBuf[i]);
							i++;
						}
						break;
					case MESSAGE_ARGUMENT_CLASS_NAME:
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_CLASS, clear);
						mnMessageDecodeRaw(argument + (Sint32)MsgClassName,
							name, MESSAGE_NAME_HALFWORDS);
						writePos = mnMessageAppendArticleName(
							mode, name, writePos, entry, 0, 0, 0);
						break;
					case MESSAGE_ARGUMENT_ITEM_NAME:
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ITEM, clear);
						mnMessageDecodeRaw((argument & INVENTORY_ITEM_ID_MASK) +
								(Sint32)MsgItemName,
							name, MESSAGE_NAME_HALFWORDS);
						writePos = mnMessageAppendArticleName(
							mode, name, writePos, entry, 0, 0, 0);
						break;
					case MESSAGE_ARGUMENT_ABILITY_NAME:
						argument = mnMessageArgumentLookup(
							MESSAGE_ARGUMENT_ABILITY, clear);
						mnMessageDecodeRaw(argument + (Sint32)MsgAbilityName,
							name, MESSAGE_NAME_HALFWORDS);
						writePos = mnMessageAppendArticleName(
							mode, name, writePos, entry, 0, 0, 0);
						break;
					case MESSAGE_ARGUMENT_LOCATION_NAME:
						mnMessageDecodeRaw(
							fdAreaGetNameIndexStub(
								mnMessageArgumentLookup(
									MESSAGE_ARGUMENT_LOCATION, clear),
								1) +
								(Sint32)MsgDebugAreaName,
							name, MESSAGE_NAME_HALFWORDS);
						writePos = mnMessageAppendArticleName(
							mode, name, writePos, entry, 0, 0, 0);
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
						writePos = mnMessageAppendArticleName(
							mode, name, writePos, entry, 0, 0, 0);
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
							mnMessageDecodeRaw(
								mysteryName, name, MYSTERY_NAME_CAPACITY);
							writePos = mnMessageAppendArticleName(
								mode, name, writePos, entry, 0, 0, 0);
						} else {
							nameBytes = unit->name;
							nameOutput = name;
							j = 0;
							do {
								*nameOutput = *nameBytes;
								j++;
								nameBytes++;
								nameOutput++;
							} while(j < UNIT_NAME_SIZE);
							writePos = mnMessageAppendArticleName(
								mode, name, writePos, entry, 0, 0, 0);
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
						writePos = mnMessageAppendArticleName(
							mode, name, writePos, entry, 0, 0, 0);
						break;
					case MESSAGE_ACTOR_NAME_START:
						argument = decode(&reader) - 1;
						unit = syUnitGet(argument);
						length = decode(&reader);
						nameBytes = unit->name;
						nameOutput = name;
						for(j = 0; j < length; j++) {
							*nameOutput++ = *nameBytes++;
							if((Uint8)(*nameBytes - TEXT_MARK_VOICED) <= 1)
								*nameOutput++ = *nameBytes++;
						}
						*nameOutput = 0;
						writePos = mnMessageAppendArticleName(
							mode, name, writePos, entry, 0, 0, 0);
						break;
					case -1:
						break;
					case MESSAGE_PAGE_BREAK:
					case MESSAGE_LINE_BREAK:
						lineStart = 1;
					default:
						TEXT_APPEND(entry, writePos, (Uint16)code);
						break;
					}
				}
			}
			decodedCnt++;
		} while(running != 0 && decodedCnt <= TEXT_BUFFER_MASK);

		TEXT_APPEND(entry, writePos, (Uint16)code);
		entry[writePos] = 0;
		nextPos = (writePos + 1) & TEXT_BUFFER_MASK;
		state->textBufEnd = (Uint16)nextPos;
		syHeapSlotRelease(HEAP_SLOT_MESSAGE_DECODER);
		state->lastMsgStart = (Uint16)start;
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
