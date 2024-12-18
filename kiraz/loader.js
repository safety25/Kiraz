(async () => {
    try {
        const response = await fetch('../module_hello.wasm');
        if (!response.ok) {
            throw new Error(`Failed to fetch WASM file: ${response.statusText}`);
        }

        const imports = {
            io: {
                memory: new WebAssembly.Memory({ initial: 1, maximum: 2 }),
                print_b: (arg1, arg2) => {
                    console.log(`print_b called with: ${arg1}, ${arg2}`);
                },
                print_i: (arg1, arg2) => {
                    console.log(`print_i called with: ${arg1}, ${arg2}`);
                },
                print_s: (arg1, arg2, arg3) => {
                    console.log(`print_s called with: ${arg1}, ${arg2}, ${arg3}`);
                }
            }
        };

        const buffer = await response.arrayBuffer();
        const wasmModule = await WebAssembly.instantiate(buffer, imports);

        console.log("WASM Module Loaded Successfully!");
  
        const exports = wasmModule.instance.exports;
        console.log("Exported functions:", Object.keys(exports));
  
        if (exports.main) {
            console.log("Running exported 'main' function:");
            exports.main();
        }
    } catch (error) {
        console.error("Error loading WASM module:", error);
    }
})();
