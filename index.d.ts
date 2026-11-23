declare module "@foxglove/wasm-lz4" {
  function decompress(buffer: Uint8Array, size: number): Buffer;
  namespace decompress {
    const isLoaded: Promise<boolean>;
    function compress(buffer: Uint8Array, compressionLevel?: number): Buffer;
  }

  export default decompress;
}
