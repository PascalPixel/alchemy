#!/usr/bin/env bun
import { split_static_sprite_banks } from "./static_sprite_series.ts";

// アトラス由来の銀行をフレーム毎PNGへ分割する薄い入口。
function option(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || index + 1 >= args.length) throw new Error(`${name} is required`);
  return args[index + 1];
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: split_sprite_bank.ts --index INDEX.json --palette PNG");
    return;
  }
  const indexPath = option(args, "--index"), palettePath = option(args, "--palette");
  const report = split_static_sprite_banks(indexPath, palettePath);
  console.log(`packages=${report.packages} converted=${report.converted} skipped=${(report.skipped as string[]).length}`);
  for (const line of report.skipped as string[]) console.log(`skip ${line}`);
}

if (import.meta.main) main(Bun.argv.slice(2));
