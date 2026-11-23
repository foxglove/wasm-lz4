# @foxglove/wasm-lz4

https://github.com/lz4/lz4 compiled to WebAssembly. Both directions use the [LZ4 frame format](https://github.com/lz4/lz4/blob/dev/doc/lz4_Frame_format.md).

## API

`wasm-lz4` exports a decompression function, with a compression function attached to it:

```js
export default (buffer: Uint8Array, destinationByteSize: number) => Buffer;
export default.compress = (buffer: Uint8Array, compressionLevel?: number) => Buffer;
```

Here is an example of decompressing:

```js
import fs from "fs";
import decompress from "wasm-lz4";

const compressedBytes = fs.readFileSync("compressed.lz4");

// currently you need to know the exact expected size of the output buffer
// so the wasm runtime can allocate enough bytes to decompress into
// TODO: this should not be necessary...it's exposed in the lz4 C code
const outputBytes = compressedBytes * 10;
const decompressedBytes = decompress(compressedBytes, outputBytes);
```

And an example of compressing:

```js
import decompress from "wasm-lz4";

await decompress.isLoaded;
// Negative levels trade ratio for speed, 0 (the default) is fast mode, and 3 through 12 use the
// slower high-compression mode.
const compressedBytes = decompress.compress(uncompressedBytes, 0);
```

## Using the module in a browser

Emscripten compiled WebAssembly modules are built in 2 parts: a `.js` side and a `.wasm` side. In the browser the `.js` side needs to download the `.wasm` side from the server so it can compile it. There is [more information available in the emscripten documentation](https://kripken.github.io/emscripten-site/docs/compiling/Deploying-Pages.html).

### Usage with Webpack

We require the `wasm-lz4.wasm` module in our `locateFile` definition, so that module bundling with dynamic paths is possible. So if you are using Webpack, you can add the following entry:

```
...
rules : [
  ...
  {
    test: /\.wasm$/,
    type: "javascript/auto",
    use: {
      loader: "file-loader",
      options: {
        name: "[name]-[hash].[ext]",
      },
    },
  },
  ...
]
...
```

The `javascript/auto` type setting tells Webpack to bypass its default importing logic, and just import the file as-is. Hopefully this hack will go away with improved WebAssembly support in webpack.

## Asynchronous loading & compiling

After the `.wasm` binary is loaded (via `fetch` in the browser or `require` in node) it must be compiled by the WebAssembly runtime. If you call `decompress` or `compress` before it is finished compiling it will throw an error indicating it isn't ready yet. To wait for the module to be loaded you can do something like the following:

```
import decompress from 'wasm-lz4'

async function doWork() {
  await decompress.isLoaded;
  decompress(buffer, outputByteLength);
}
```

## Developing locally

1. Run `npm install` to install dependencies.
2. Run `npm run build` to invoke emcc inside a Docker container and compile the code in `wasm-lz4.c` as well as the required lz4 source files. The output will be in `dist/` on the host machine.
3. Run `npm test` to run the tests.

## License

@foxglove/wasm-lz4 is licensed under [MIT License](https://opensource.org/licenses/MIT).

## Releasing

1. Run `yarn version --[major|minor|patch]` to bump version
2. Run `git push && git push --tags` to push new tag
3. GitHub Actions will take care of the rest

## Stay in touch

Join our [Slack channel](https://foxglove.dev/slack) to ask questions, share feedback, and stay up to date on what our team is working on.
