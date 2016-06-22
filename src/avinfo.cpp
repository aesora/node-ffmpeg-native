/*
ffmpeg-native - 0.1.0 - 22-06-2016
FFMPEG bindings for NodeJS

Copyright (c) 2016 Matthias Breithaupt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#include <nan.h>
#include "avinfo.h"

NAN_MODULE_INIT(AVInfo::Init) {
  v8::Local<v8::Object> version = Nan::New<v8::Object>();

  v8::Local<v8::Array> avfversion = Nan::New<v8::Array>(3);
  Nan::Set(avfversion, 0, Nan::New<v8::Integer>(LIBAVFORMAT_VERSION_MAJOR));
  Nan::Set(avfversion, 1, Nan::New<v8::Integer>(LIBAVFORMAT_VERSION_MINOR));
  Nan::Set(avfversion, 2, Nan::New<v8::Integer>(LIBAVFORMAT_VERSION_MICRO));
  Nan::Set(version, Nan::New<v8::String>("AVFormat").ToLocalChecked(), avfversion);

  v8::Local<v8::Array> avcversion = Nan::New<v8::Array>(3);
  Nan::Set(avcversion, 0, Nan::New<v8::Integer>(LIBAVCODEC_VERSION_MAJOR));
  Nan::Set(avcversion, 1, Nan::New<v8::Integer>(LIBAVCODEC_VERSION_MINOR));
  Nan::Set(avcversion, 2, Nan::New<v8::Integer>(LIBAVCODEC_VERSION_MICRO));
  Nan::Set(version, Nan::New<v8::String>("AVCodec").ToLocalChecked(), avcversion);

  v8::Local<v8::Array> avuversion = Nan::New<v8::Array>(3);
  Nan::Set(avuversion, 0, Nan::New<v8::Integer>(LIBAVUTIL_VERSION_MAJOR));
  Nan::Set(avuversion, 1, Nan::New<v8::Integer>(LIBAVUTIL_VERSION_MINOR));
  Nan::Set(avuversion, 2, Nan::New<v8::Integer>(LIBAVUTIL_VERSION_MICRO));
  Nan::Set(version, Nan::New<v8::String>("AVUtil").ToLocalChecked(), avuversion);

  v8::Local<v8::Array> avfiversion = Nan::New<v8::Array>(3);
  Nan::Set(avfiversion, 0, Nan::New<v8::Integer>(LIBAVFILTER_VERSION_MAJOR));
  Nan::Set(avfiversion, 1, Nan::New<v8::Integer>(LIBAVFILTER_VERSION_MINOR));
  Nan::Set(avfiversion, 2, Nan::New<v8::Integer>(LIBAVFILTER_VERSION_MICRO));
  Nan::Set(version, Nan::New<v8::String>("AVFilter").ToLocalChecked(), avfiversion);

  v8::Local<v8::Array> swsversion = Nan::New<v8::Array>(3);
  Nan::Set(swsversion, 0, Nan::New<v8::Integer>(LIBSWSCALE_VERSION_MAJOR));
  Nan::Set(swsversion, 1, Nan::New<v8::Integer>(LIBSWSCALE_VERSION_MINOR));
  Nan::Set(swsversion, 2, Nan::New<v8::Integer>(LIBSWSCALE_VERSION_MICRO));
  Nan::Set(version, Nan::New<v8::String>("SWScale").ToLocalChecked(), swsversion);

  v8::Local<v8::Array> swrversion = Nan::New<v8::Array>(3);
  Nan::Set(swrversion, 0, Nan::New<v8::Integer>(LIBSWRESAMPLE_VERSION_MAJOR));
  Nan::Set(swrversion, 1, Nan::New<v8::Integer>(LIBSWRESAMPLE_VERSION_MINOR));
  Nan::Set(swrversion, 2, Nan::New<v8::Integer>(LIBSWRESAMPLE_VERSION_MICRO));
  Nan::Set(version, Nan::New<v8::String>("SWResample").ToLocalChecked(), swrversion);

  Nan::Set(target, Nan::New<v8::String>("version").ToLocalChecked(), version);
}
