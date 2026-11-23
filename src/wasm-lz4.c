//  Copyright (c) 2018-present, Cruise LLC
//
//  This source code is licensed under the Apache License, Version 2.0,
//  found in the LICENSE file in the root directory of this source tree.
//  You may not use this file except in compliance with the License.

#include <stdio.h>
#include <string.h>
#include <emscripten/emscripten.h>
#include "../vendor/lz4/lib/lz4frame.h"

int main(int argc, char **argv)
{
}

#ifdef __cplusplus
extern "C"
{
#endif

  int EMSCRIPTEN_KEEPALIVE decompressFrame(
      LZ4F_dctx *dctx, char *dstBuffer, size_t dstSize, const void *srcBuffer, size_t srcSize)
  {
    size_t *dstSizePtr = &dstSize;
    size_t *srcSizePtr = &srcSize;
    int result = LZ4F_decompress(dctx, dstBuffer, dstSizePtr, srcBuffer, srcSizePtr, NULL);
    if (result != 0)
    {
      // an error or a partially decoded frame leaves state in the context
      // that would make the next, unrelated frame fail to decode
      LZ4F_resetDecompressionContext(dctx);
      return -1;
    }
    return dstSize;
  }

  static LZ4F_preferences_t makePreferences(int compressionLevel)
  {
    LZ4F_preferences_t preferences = LZ4F_INIT_PREFERENCES;
    preferences.compressionLevel = compressionLevel;
    return preferences;
  }

  size_t EMSCRIPTEN_KEEPALIVE compressFrameBound(size_t srcSize, int compressionLevel)
  {
    LZ4F_preferences_t preferences = makePreferences(compressionLevel);
    return LZ4F_compressFrameBound(srcSize, &preferences);
  }

  int EMSCRIPTEN_KEEPALIVE compressFrame(
      char *dstBuffer, size_t dstSize, const void *srcBuffer, size_t srcSize, int compressionLevel)
  {
    LZ4F_preferences_t preferences = makePreferences(compressionLevel);
    size_t result = LZ4F_compressFrame(dstBuffer, dstSize, srcBuffer, srcSize, &preferences);
    if (LZ4F_isError(result))
    {
      return -1;
    }
    return result;
  }

  LZ4F_dctx *EMSCRIPTEN_KEEPALIVE createDecompressionContext()
  {
    LZ4F_decompressionContext_t ctx = NULL;
    int ok = LZ4F_createDecompressionContext(&ctx, LZ4F_VERSION);
    return ctx;
  }

#ifdef __cplusplus
}
#endif
