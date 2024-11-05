#include <stdlib.h>    // malloc, free, exit
#include <stdio.h>     // fprintf, perror, fopen, etc.
#include <string.h>    // strlen, strcat, memset, strerror
#include <errno.h>     // errno
#include <sys/stat.h>  // stat
#include <brotli/encode.h>

#include <emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

typedef struct BrotliEncoderBuffer_s {
  void*  ptr;         /**< start of buffer */
  size_t size;        /**< size of buffer */
} BrotliEncoderBuffer;

uint32_t _BrotliEncoderVersion(void) {
  return BrotliEncoderVersion();
}

bool _BrotliEncoderSetParameter(unsigned int state, BrotliEncoderParameter param, uint32_t value) {
  return BrotliEncoderSetParameter((BrotliEncoderState*) state, param, value);
}

unsigned int _BrotliEncoderCreateInstance() {
  return (unsigned int)BrotliEncoderCreateInstance(0, 0, NULL);
}

void _BrotliEncoderDestroyInstance(unsigned int state) {
  BrotliEncoderDestroyInstance((BrotliEncoderState*) state);
}

unsigned int _BrotliEncoderPrepareDictionary(BrotliSharedDictionaryType type, size_t data_size, unsigned int data_ptr, int quality) {
  return (unsigned int)BrotliEncoderPrepareDictionary(type, data_size, (const uint8_t *)data_ptr, quality, 0, 0, NULL);
}

void _BrotliEncoderDestroyPreparedDictionary(unsigned int dictionary) {
  BrotliEncoderDestroyPreparedDictionary((BrotliEncoderPreparedDictionary*) dictionary);
}

size_t _BrotliEncoderGetPreparedDictionarySize(unsigned int dictionary) {
  return BrotliEncoderGetPreparedDictionarySize((const BrotliEncoderPreparedDictionary*) dictionary);
}

bool _BrotliEncoderAttachPreparedDictionary(unsigned int state, unsigned int dictionary) {
  return BrotliEncoderAttachPreparedDictionary((BrotliEncoderState*) state, (const BrotliEncoderPreparedDictionary*) dictionary);
}

size_t _BrotliEncoderMaxCompressedSize(size_t input_size) {
  return BrotliEncoderMaxCompressedSize(input_size);
}

bool _BrotliEncoderCompress(int quality, int lgwin, BrotliEncoderMode mode, BrotliEncoderBuffer& input_buffer, BrotliEncoderBuffer& encoded_buffer) {
  return BrotliEncoderCompress(quality, lgwin, mode, input_buffer.size, (const uint8_t *)input_buffer.ptr, &encoded_buffer.size, (uint8_t *)encoded_buffer.ptr);
}

bool _BrotliEncoderCompressStream(unsigned int state, BrotliEncoderOperation op, BrotliEncoderBuffer& input_buffer, BrotliEncoderBuffer& output_buffer) {
  return BrotliEncoderCompressStream((BrotliEncoderState*) state, op, &input_buffer.size, (const uint8_t **)&input_buffer.ptr, &output_buffer.size, (uint8_t **)&output_buffer.ptr, NULL);
}

bool _BrotliEncoderIsFinished(unsigned int state) {
  return BrotliEncoderIsFinished((BrotliEncoderState*) state);
}

bool _BrotliEncoderHasMoreOutput(unsigned int state) {
  return BrotliEncoderHasMoreOutput((BrotliEncoderState*) state);
}

// Structure setters/getters
static unsigned int BrotliEncoderBuffer_ptr_getter(const BrotliEncoderBuffer &buffer) {
  return (unsigned int)buffer.ptr;
}

static void BrotliEncoderBuffer_ptr_setter(BrotliEncoderBuffer &buffer, unsigned int ptr) {
  buffer.ptr = (void *)ptr;
}

EMSCRIPTEN_BINDINGS(BrotliEncoder) {
  // Wrapped functions
  function("Version", &_BrotliEncoderVersion);
  function("SetParameter", &_BrotliEncoderSetParameter);
  function("CreateInstance", &_BrotliEncoderCreateInstance);
  function("DestroyInstance", &_BrotliEncoderDestroyInstance);
  function("PrepareDictionary", &_BrotliEncoderPrepareDictionary);
  function("DestroyPreparedDictionary", &_BrotliEncoderDestroyPreparedDictionary);
  function("GetPreparedDictionarySize", &_BrotliEncoderGetPreparedDictionarySize);
  function("AttachPreparedDictionary", &_BrotliEncoderAttachPreparedDictionary);
  function("MaxCompressedSize", &_BrotliEncoderMaxCompressedSize);
  function("Compress", &_BrotliEncoderCompress);
  function("CompressStream", &_BrotliEncoderCompressStream);
  function("HasMoreOutput", &_BrotliEncoderHasMoreOutput);

  // Enums
  enum_<BrotliEncoderParameter>("Parameter")
    .value("MODE", BrotliEncoderParameter::BROTLI_PARAM_MODE)
    .value("QUALITY", BrotliEncoderParameter::BROTLI_PARAM_QUALITY)
    .value("LGWIN", BrotliEncoderParameter::BROTLI_PARAM_LGWIN)
    .value("LGBLOCK", BrotliEncoderParameter::BROTLI_PARAM_LGBLOCK)
    .value("DISABLE_LITERAL_CONTEXT_MODELING", BrotliEncoderParameter::BROTLI_PARAM_DISABLE_LITERAL_CONTEXT_MODELING)
    .value("SIZE_HINT", BrotliEncoderParameter::BROTLI_PARAM_SIZE_HINT)
    .value("LARGE_WINDOW", BrotliEncoderParameter::BROTLI_PARAM_LARGE_WINDOW)
    .value("NPOSTFIX", BrotliEncoderParameter::BROTLI_PARAM_NPOSTFIX)
    .value("NDIRECT", BrotliEncoderParameter::BROTLI_PARAM_NDIRECT)
    .value("STREAM_OFFSET", BrotliEncoderParameter::BROTLI_PARAM_STREAM_OFFSET);

  enum_<BrotliSharedDictionaryType>("SharedDictionaryType")
    .value("Raw", BrotliSharedDictionaryType::BROTLI_SHARED_DICTIONARY_RAW)
    .value("Serialized", BrotliSharedDictionaryType::BROTLI_SHARED_DICTIONARY_SERIALIZED);

  enum_<BrotliEncoderMode>("Mode")
    .value("GENERIC", BrotliEncoderMode::BROTLI_MODE_GENERIC)
    .value("TEXT", BrotliEncoderMode::BROTLI_MODE_TEXT)
    .value("FONT", BrotliEncoderMode::BROTLI_MODE_FONT);

  enum_<BrotliEncoderOperation>("Operation")
    .value("PROCESS", BrotliEncoderOperation::BROTLI_OPERATION_PROCESS)
    .value("FLUSH", BrotliEncoderOperation::BROTLI_OPERATION_FLUSH)
    .value("FINISH", BrotliEncoderOperation::BROTLI_OPERATION_FINISH)
    .value("EMIT_METADATA", BrotliEncoderOperation::BROTLI_OPERATION_EMIT_METADATA);

  // Structures
  class_<BrotliEncoderBuffer>("Buffer")
    .constructor<>()
    .property("ptr", &BrotliEncoderBuffer_ptr_getter, &BrotliEncoderBuffer_ptr_setter)
    .property("size", &BrotliEncoderBuffer::size);
}
