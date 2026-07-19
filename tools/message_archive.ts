#!/usr/bin/env bun

export const ROM_BASE = 0x08000000;
export const ARCHIVE_ADDRESS = 0x08037464;
export const OFFSET_TABLE_ADDRESS = 0x08038334;
export const MODEL_DIRECTORY_ADDRESS = 0x0803842c;
export const MESSAGE_ADDRESS = 0x08038434;
export const MESSAGE_DIRECTORY_ADDRESS = 0x080736b8;
export const ARCHIVE_END = 0x08073808;
export const CONTEXT_COUNT = 123;
export const OFFSET_COUNT = 124;
export const BANK_COUNT = 42;
export const BANK_SIZE = 256;

type Atom = string | { control: number };
type MessageSource = null | string | Atom[];
type TreeNode = { symbol: number } | { left: TreeNode; right: TreeNode };

export interface ContextSource {
  id: number;
  tree: string;
  leaves: number[];
}

export interface MessageArchiveSource {
  format: 1;
  kind: "golden-sun-message-archive";
  address: string;
  size: string;
  bank_size: number;
  banks: MessageSource[][];
}

interface CompiledContext {
  root: TreeNode;
  paths: Map<number, string>;
}

class BitReader {
  readonly source: Uint8Array;
  position: number;
  readonly limit: number;

  constructor(source: Uint8Array, byte: number, limit = source.length * 8) {
    this.source = source;
    this.position = byte * 8;
    this.limit = limit;
  }

  read(): number {
    if (this.position >= this.limit) throw new Error("bit stream exceeds its extent");
    const bit = (this.source[this.position >>> 3] >>> (this.position & 7)) & 1;
    this.position++;
    return bit;
  }
}

function integer(value: unknown, label: string): number {
  const parsed = typeof value === "string" ? Number(value) : value;
  if (!Number.isSafeInteger(parsed)) throw new Error(`${label} must be an integer`);
  return parsed as number;
}

function bounded(value: unknown, minimum: number, maximum: number, label: string): number {
  const parsed = integer(value, label);
  if (parsed < minimum || parsed > maximum) throw new Error(`${label} is outside its range`);
  return parsed;
}

function hex(value: number, width = 8): string {
  return `0x${value.toString(16).padStart(width, "0")}`;
}

function dataView(source: Uint8Array): DataView {
  return new DataView(source.buffer, source.byteOffset, source.byteLength);
}

function romOffset(address: number, source: Uint8Array, allowEnd = false): number {
  const offset = address - ROM_BASE;
  if (offset < 0 || offset > source.length || (!allowEnd && offset === source.length)) {
    throw new Error(`${hex(address)} is outside the ROM`);
  }
  return offset;
}

function pointer(source: Uint8Array, offset: number): number {
  if (offset < 0 || offset + 4 > source.length) throw new Error("pointer is outside the ROM");
  return dataView(source).getUint32(offset, true);
}

function halfword(source: Uint8Array, offset: number): number {
  if (offset < 0 || offset + 2 > source.length) throw new Error("halfword is outside the ROM");
  return dataView(source).getUint16(offset, true);
}

function bitBuffer(bits: string): Buffer {
  if (!/^[01]*$/.test(bits)) throw new Error("bit source is not binary");
  const result = Buffer.alloc(Math.ceil(bits.length / 8));
  for (let index = 0; index < bits.length; index++) {
    if (bits[index] === "1") result[index >>> 3] |= 1 << (index & 7);
  }
  return result;
}

function symbolAt(source: Uint8Array, boundary: number, rank: number): number {
  const distance = rank + (rank >>> 1);
  const first = boundary - distance - 1;
  const second = first - 1;
  if (second < 0 || first >= source.length) throw new Error("prefix-code alphabet exceeds its record");
  return (rank & 1) === 0
    ? (source[first] << 4) | (source[second] >>> 4)
    : ((source[first] & 0x0f) << 8) | source[second];
}

function packedLeaves(leaves: unknown): Buffer {
  if (!Array.isArray(leaves) || leaves.length === 0) throw new Error("context leaves must be a nonempty array");
  const groups: Buffer[] = [];
  for (let index = 0; index < leaves.length; index += 2) {
    const first = bounded(leaves[index], 0, 0xfff, "context symbol");
    if (index + 1 === leaves.length) {
      groups.push(Buffer.from([(first & 0x0f) << 4, first >>> 4]));
      continue;
    }
    const second = bounded(leaves[index + 1], 0, 0xfff, "context symbol");
    groups.push(Buffer.from([
      second & 0xff,
      ((first & 0x0f) << 4) | (second >>> 8),
      first >>> 4,
    ]));
  }
  return Buffer.concat(groups.reverse());
}

function compileContext(source: ContextSource): CompiledContext {
  if (Object.keys(source).sort().join(",") !== "id,leaves,tree") {
    throw new Error("context source has unknown fields");
  }
  const bits = String(source.tree);
  if (!/^[01]+$/.test(bits)) throw new Error(`context ${source.id} has an invalid tree`);
  if (!Array.isArray(source.leaves) || source.leaves.length === 0) {
    throw new Error(`context ${source.id} has no leaves`);
  }
  let position = 0;
  let leaf = 0;
  const paths = new Map<number, string>();
  const parse = (path: string): TreeNode => {
    if (position >= bits.length) throw new Error(`context ${source.id} tree is truncated`);
    const bit = bits[position++];
    if (bit === "1") {
      if (leaf >= source.leaves.length) throw new Error(`context ${source.id} has too few leaves`);
      const symbol = bounded(source.leaves[leaf++], 0, 0xfff, "context symbol");
      if (paths.has(symbol)) throw new Error(`context ${source.id} repeats symbol ${symbol}`);
      paths.set(symbol, path);
      return { symbol };
    }
    return { left: parse(`${path}0`), right: parse(`${path}1`) };
  };
  const root = parse("");
  if (position !== bits.length || leaf !== source.leaves.length) {
    throw new Error(`context ${source.id} tree and leaves differ`);
  }
  return { root, paths };
}

function compileContexts(source: ContextSource[]): Map<number, CompiledContext> {
  const result = new Map<number, CompiledContext>();
  for (const item of source) {
    const id = bounded(item.id, 0, CONTEXT_COUNT - 1, "context ID");
    if (result.has(id)) throw new Error(`context ${id} is duplicated`);
    result.set(id, compileContext(item));
  }
  return result;
}

function parseDocument(value: unknown): MessageArchiveSource {
  if (typeof value !== "object" || value === null) throw new Error("message archive source must be an object");
  const source = value as Partial<MessageArchiveSource>;
  if (Object.keys(source).sort().join(",") !== "address,bank_size,banks,format,kind,size") {
    throw new Error("message archive source has unknown fields");
  }
  if (source.format !== 1 || source.kind !== "golden-sun-message-archive") {
    throw new Error("unsupported message archive source");
  }
  if (source.address !== hex(ARCHIVE_ADDRESS) || source.size !== hex(ARCHIVE_END - ARCHIVE_ADDRESS) ||
      source.bank_size !== BANK_SIZE) {
    throw new Error("message archive layout differs");
  }
  if (!Array.isArray(source.banks) || source.banks.length !== BANK_COUNT) {
    throw new Error("message archive collections differ");
  }
  source.banks.forEach((bank, index) => {
    const expected = index + 1 === BANK_COUNT ? 227 : BANK_SIZE;
    if (!Array.isArray(bank) || bank.length !== expected) throw new Error(`message bank ${index} has the wrong size`);
  });
  return source as MessageArchiveSource;
}

function buildModel(source: ContextSource[]): { data: Buffer; contexts: Map<number, CompiledContext> } {
  const contexts = compileContexts(source);
  const entries = new Map(source.map((item) => [item.id, item]));
  const records: Buffer[] = [];
  const offsets = Buffer.alloc(OFFSET_COUNT * 2);
  let cursor = 0;
  for (let id = 0; id < CONTEXT_COUNT; id++) {
    const entry = entries.get(id);
    if (entry === undefined) {
      offsets.writeUInt16LE(0x8000, id * 2);
      continue;
    }
    const leaves = packedLeaves(entry.leaves);
    const tree = bitBuffer(entry.tree);
    offsets.writeUInt16LE(cursor + leaves.length, id * 2);
    records.push(leaves, tree);
    cursor += leaves.length + tree.length;
  }
  if (cursor !== OFFSET_TABLE_ADDRESS - ARCHIVE_ADDRESS) throw new Error("context model size differs");
  const model = Buffer.concat(records);
  const directory = Buffer.alloc(8);
  directory.writeUInt32LE(ARCHIVE_ADDRESS, 0);
  directory.writeUInt32LE(OFFSET_TABLE_ADDRESS, 4);
  return { data: Buffer.concat([model, offsets, directory]), contexts };
}

function tokensFromMessage(source: MessageSource): number[] | null {
  if (source === null) return null;
  const atoms: Atom[] = typeof source === "string" ? [source] : source;
  if (!Array.isArray(atoms)) throw new Error("message must be text, atoms, or null");
  const tokens: number[] = [];
  for (const atom of atoms) {
    if (typeof atom === "string") {
      for (const character of atom) {
        const token = character.codePointAt(0)!;
        if (token < 32 || token > 122) throw new Error("message text is outside the recovered ASCII range");
        tokens.push(token);
      }
      continue;
    }
    if (typeof atom !== "object" || atom === null || Object.keys(atom).length !== 1 || !("control" in atom)) {
      throw new Error("message control atom is invalid");
    }
    tokens.push(bounded(atom.control, 1, 31, "message control"));
  }
  return tokens;
}

function messageFromTokens(tokens: number[]): MessageSource {
  const atoms: Atom[] = [];
  let text = "";
  const flush = (): void => {
    if (text !== "") atoms.push(text);
    text = "";
  };
  for (const token of tokens) {
    if (token >= 32 && token <= 122) text += String.fromCodePoint(token);
    else {
      if (token <= 0 || token > 31) throw new Error(`message symbol ${token} has no source representation`);
      flush();
      atoms.push({ control: token });
    }
  }
  flush();
  if (atoms.length === 0) return "";
  if (atoms.length === 1 && typeof atoms[0] === "string") return atoms[0];
  return atoms;
}

function derivedContexts(banks: MessageSource[][]): ContextSource[] {
  const transitions = Array.from({ length: CONTEXT_COUNT }, () => new Map<number, { count: number; order: number }>());
  for (const bank of banks) {
    for (const message of bank) {
      const tokens = tokensFromMessage(message);
      if (tokens === null) continue;
      let previous = 0;
      for (const symbol of [...tokens, 0]) {
        const context = transitions[previous];
        if (context === undefined) throw new Error(`message enters unsupported context ${previous}`);
        const item = context.get(symbol);
        if (item === undefined) context.set(symbol, { count: 1, order: context.size });
        else item.count++;
        previous = symbol;
      }
    }
  }

  const result: ContextSource[] = [];
  transitions.forEach((symbols, id) => {
    if (symbols.size === 0) return;
    let order = symbols.size;
    const nodes = [...symbols].map(([symbol, item]) => ({
      count: item.count,
      order: item.order,
      node: { symbol } as TreeNode,
    }));
    while (nodes.length > 1) {
      nodes.sort((left, right) => left.count - right.count || left.order - right.order);
      const left = nodes.shift()!;
      const right = nodes.shift()!;
      nodes.push({
        count: left.count + right.count,
        order: order++,
        node: { left: left.node, right: right.node },
      });
    }
    let tree = "";
    const leaves: number[] = [];
    const walk = (node: TreeNode): void => {
      if ("symbol" in node) {
        tree += "1";
        leaves.push(node.symbol);
      } else {
        tree += "0";
        walk(node.left);
        walk(node.right);
      }
    };
    walk(nodes[0].node);
    result.push({ id, tree, leaves });
  });
  return result;
}

function encodeMessage(tokens: number[], contexts: Map<number, CompiledContext>): Buffer {
  let context = 0;
  let bits = "";
  for (const symbol of [...tokens, 0]) {
    const codec = contexts.get(context);
    if (codec === undefined) throw new Error(`message enters absent context ${context}`);
    const path = codec.paths.get(symbol);
    if (path === undefined) throw new Error(`context ${context} cannot encode symbol ${symbol}`);
    bits += path;
    context = symbol;
  }
  return bitBuffer(bits);
}

export function build_message_archive(value: unknown): Buffer {
  const source = parseDocument(value);
  const model = buildModel(derivedContexts(source.banks));
  if (model.data.length !== MESSAGE_ADDRESS - ARCHIVE_ADDRESS) throw new Error("context model extent differs");
  const banks: Buffer[] = [];
  const descriptors = Buffer.alloc(BANK_COUNT * 8);
  let address = MESSAGE_ADDRESS;
  source.banks.forEach((messages, bank) => {
    const payloads: Buffer[] = [];
    const lengths = Buffer.alloc(messages.length);
    messages.forEach((message, index) => {
      const tokens = tokensFromMessage(message);
      const encoded = tokens === null ? Buffer.alloc(0) : encodeMessage(tokens, model.contexts);
      if (encoded.length > 0xfe) throw new Error(`message ${hex((bank << 8) | index, 4)} is too large`);
      if (encoded.length === 0 && !(bank + 1 === BANK_COUNT && index + 1 === messages.length && message === null)) {
        throw new Error(`message ${hex((bank << 8) | index, 4)} is unexpectedly null`);
      }
      lengths[index] = encoded.length;
      payloads.push(encoded);
    });
    const payload = Buffer.concat(payloads);
    descriptors.writeUInt32LE(address, bank * 8);
    descriptors.writeUInt32LE(address + payload.length, bank * 8 + 4);
    banks.push(payload, lengths);
    address += payload.length + lengths.length;
  });
  if (address !== MESSAGE_DIRECTORY_ADDRESS) throw new Error("message banks do not reach their directory");
  const result = Buffer.concat([model.data, ...banks, descriptors]);
  if (result.length !== ARCHIVE_END - ARCHIVE_ADDRESS) throw new Error("message archive size differs");
  return result;
}

function readTree(source: Uint8Array, boundary: number): { tree: string; leaves: number[]; end: number } {
  const bits = new BitReader(source, boundary);
  let tree = "";
  let leafCount = 0;
  const parse = (): void => {
    const bit = bits.read();
    tree += String(bit);
    if (bit !== 0) {
      leafCount++;
      return;
    }
    parse();
    parse();
  };
  parse();
  const leaves = Array.from({ length: leafCount }, (_, rank) => symbolAt(source, boundary, rank));
  const end = Math.ceil(bits.position / 8);
  for (let position = bits.position; position < end * 8; position++) {
    if (((source[position >>> 3] >>> (position & 7)) & 1) !== 0) throw new Error("context tree padding is nonzero");
  }
  return { tree, leaves, end };
}

function exportModel(source: Uint8Array): { absent: number[]; contexts: ContextSource[] } {
  const offsets = romOffset(OFFSET_TABLE_ADDRESS, source);
  const descriptor = romOffset(MODEL_DIRECTORY_ADDRESS, source);
  if (pointer(source, descriptor) !== ARCHIVE_ADDRESS || pointer(source, descriptor + 4) !== OFFSET_TABLE_ADDRESS) {
    throw new Error("context model directory differs");
  }
  const absent: number[] = [];
  const contexts: ContextSource[] = [];
  let cursor = romOffset(ARCHIVE_ADDRESS, source);
  for (let id = 0; id < CONTEXT_COUNT; id++) {
    const offset = halfword(source, offsets + id * 2);
    if (offset === 0x8000) {
      absent.push(id);
      continue;
    }
    const boundary = romOffset(ARCHIVE_ADDRESS + offset, source);
    const decoded = readTree(source, boundary);
    const leafBytes = Math.ceil(decoded.leaves.length * 12 / 8);
    if (boundary - leafBytes !== cursor) throw new Error(`context ${id} record is not contiguous`);
    const packed = packedLeaves(decoded.leaves);
    if (!packed.equals(Buffer.from(source.subarray(cursor, boundary)))) {
      throw new Error(`context ${id} alphabet differs`);
    }
    contexts.push({ id, tree: decoded.tree, leaves: decoded.leaves });
    cursor = decoded.end;
  }
  if (halfword(source, offsets + CONTEXT_COUNT * 2) !== 0 || cursor !== offsets) {
    throw new Error("context model terminal record differs");
  }
  return { absent, contexts };
}

function decodeMessage(source: Uint8Array, byte: number, size: number, contexts: Map<number, CompiledContext>): number[] {
  const bits = new BitReader(source, byte, (byte + size) * 8);
  const tokens: number[] = [];
  let context = 0;
  for (let guard = 0; guard < 0x10000; guard++) {
    const codec = contexts.get(context);
    if (codec === undefined) throw new Error(`message enters absent context ${context}`);
    let node = codec.root;
    while ("left" in node) node = bits.read() === 0 ? node.left : node.right;
    if (node.symbol === 0) {
      while (bits.position < bits.limit) {
        if (bits.read() !== 0) throw new Error("message padding is nonzero");
      }
      return tokens;
    }
    tokens.push(node.symbol);
    context = node.symbol;
  }
  throw new Error("message does not terminate");
}

export function export_message_archive(source: Uint8Array): MessageArchiveSource {
  const model = exportModel(source);
  const skeleton: MessageArchiveSource = {
    format: 1,
    kind: "golden-sun-message-archive",
    address: hex(ARCHIVE_ADDRESS),
    size: hex(ARCHIVE_END - ARCHIVE_ADDRESS),
    bank_size: BANK_SIZE,
    banks: [],
  };
  const contexts = compileContexts(model.contexts);
  const directory = romOffset(MESSAGE_DIRECTORY_ADDRESS, source);
  const banks: MessageSource[][] = [];
  for (let bank = 0; bank < BANK_COUNT; bank++) {
    const payload = romOffset(pointer(source, directory + bank * 8), source);
    const lengths = romOffset(pointer(source, directory + bank * 8 + 4), source);
    const count = bank + 1 === BANK_COUNT ? directory - lengths : BANK_SIZE;
    if (count <= 0 || count > BANK_SIZE) throw new Error(`message bank ${bank} has an invalid count`);
    const messages: MessageSource[] = [];
    let byte = payload;
    for (let index = 0; index < count; index++) {
      const size = source[lengths + index];
      if (size === 0) messages.push(null);
      else messages.push(messageFromTokens(decodeMessage(source, byte, size, contexts)));
      byte += size;
    }
    if (byte !== lengths) throw new Error(`message bank ${bank} payload extent differs`);
    banks.push(messages);
  }
  skeleton.banks = banks;
  const built = build_message_archive(skeleton);
  const start = romOffset(ARCHIVE_ADDRESS, source);
  if (!built.equals(Buffer.from(source.subarray(start, start + built.length)))) {
    throw new Error("exported message archive does not round-trip");
  }
  return skeleton;
}

export function format_message_archive(source: MessageArchiveSource): string {
  const lines = [
    "{",
    '  "format": 1,',
    '  "kind": "golden-sun-message-archive",',
    `  "address": ${JSON.stringify(source.address)},`,
    `  "size": ${JSON.stringify(source.size)},`,
    `  "bank_size": ${source.bank_size},`,
    '  "banks": [',
  ];
  source.banks.forEach((bank, bankIndex) => {
    lines.push("    [");
    bank.forEach((message, messageIndex) => {
      lines.push(`      ${JSON.stringify(message)}${messageIndex + 1 === bank.length ? "" : ","}`);
    });
    lines.push(`    ]${bankIndex + 1 === source.banks.length ? "" : ","}`);
  });
  lines.push("  ]", "}");
  return `${lines.join("\n")}\n`;
}

function selfTest(): void {
  const leaves = [0x123, 0x456, 0x789];
  const packed = packedLeaves(leaves);
  if (leaves.some((symbol, rank) => symbolAt(packed, packed.length, rank) !== symbol)) {
    throw new Error("context alphabet self-test failed");
  }
  const compiled = compileContext({ id: 0, tree: "00111", leaves: [65, 66, 67] });
  if (compiled.paths.get(65) !== "00" || compiled.paths.get(66) !== "01" || compiled.paths.get(67) !== "1") {
    throw new Error("context tree self-test failed");
  }
  const bits = "101100010111";
  const packedBits = bitBuffer(bits);
  const reader = new BitReader(packedBits, 0, bits.length);
  if ([...bits].some((bit) => reader.read() !== Number(bit))) throw new Error("bit stream self-test failed");
}

function option(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  return index < 0 ? undefined : args[index + 1];
}

async function main(args: string[]): Promise<void> {
  if (args[0] === "--self-test") {
    selfTest();
    console.log("self-test=ok");
    return;
  }
  const command = args[0];
  const input = args[1];
  if (command === "export") {
    const output = option(args, "--output") ?? option(args, "-o");
    if (input === undefined || output === undefined) throw new Error("export requires ROM and --output");
    const rom = new Uint8Array(await Bun.file(input).arrayBuffer());
    const source = export_message_archive(rom);
    await Bun.write(output, format_message_archive(source));
    console.log(`identical=true contexts=${derivedContexts(source.banks).length} messages=${source.banks.flat().length} bytes=${ARCHIVE_END - ARCHIVE_ADDRESS}`);
    return;
  }
  if (command === "build") {
    const output = option(args, "--output") ?? option(args, "-o");
    if (input === undefined || output === undefined) throw new Error("build requires SOURCE and --output");
    const built = build_message_archive(await Bun.file(input).json());
    await Bun.write(output, built);
    console.log(`bytes=${built.length}`);
    return;
  }
  if (command === "verify") {
    const sourcePath = args[2];
    if (input === undefined || sourcePath === undefined) throw new Error("verify requires ROM and SOURCE");
    const rom = new Uint8Array(await Bun.file(input).arrayBuffer());
    const built = build_message_archive(await Bun.file(sourcePath).json());
    const start = ARCHIVE_ADDRESS - ROM_BASE;
    if (!built.equals(Buffer.from(rom.subarray(start, start + built.length)))) {
      throw new Error("message archive differs from ROM");
    }
    console.log(`identical=true bytes=${built.length}`);
    return;
  }
  throw new Error("usage: message_archive.ts {export ROM --output SOURCE|build SOURCE --output FILE|verify ROM SOURCE|--self-test}");
}

if (import.meta.main) await main(Bun.argv.slice(2));
