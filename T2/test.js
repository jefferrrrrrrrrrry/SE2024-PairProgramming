import assert from "assert";

// Choose proper "import" depending on your PL.
// import { mancalaResult } from "./t2-as/build/release.js";
import {  mancalaResult } from "./t2_c/a.out.js"

// [Write your own "import" for other PLs.]
// import { readFileSync } from 'fs';

// const module = new WebAssembly.Instance(new WebAssembly.Module(readFileSync("1.js")));
// var count = 2;
// var buf = new ArrayBuffer(count * 4);
// var i8 = new Uint8Array(buf);
// var i32 = new Int32Array(buf);
// i32[0]=11;
// i32[1]=12;
assert.strictEqual(mancalaResult(1,[11,12],2),30001);
//assert.strictEqual(mancalaResult(1,[14],1),20001);

console.log("🎉 You have passed all the tests provided.");
