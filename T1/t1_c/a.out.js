import fs from "fs"

export function bocchiShutUp(n, arr, length) {
  const binary = fs.readFileSync('./t1_c/a.out.wasm');

  var HEAPU8 = new Uint8Array();
  var _emscripten_resize_heap = (requestedSize) => {
    var oldSize = HEAPU8.length;
    // With CAN_ADDRESS_2GB or MEMORY64, pointers are already unsigned.
    requestedSize >>>= 0;
    abortOnCannotGrowMemory(requestedSize);
  };

  var wasmImports = {
    emscripten_resize_heap: _emscripten_resize_heap
  };

  var info = {
    'env': wasmImports,
    'wasi_snapshot_preview1': wasmImports,
  };

  const module = new WebAssembly.Module(binary);
  const instance = new WebAssembly.Instance(module, info);

  const memory = instance.exports.memory;
  const ptr = instance.exports.stackAlloc(length * 4);
  const start = ptr >> 2;
  const heap = new Int32Array(memory.buffer);
  for (let i = 0; i < length; ++ i) {
    heap[start + i] = arr[i];
  }
  const result = instance.exports.bocchiShutUp(n, ptr, length);
  return result;
}