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


#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include "avinput.h"
#include "avinfo.h"
#include <nan.h>

NAN_METHOD(NewInput) {
  info.GetReturnValue().Set(AVInput::NewInstance(info[0]));
}

NAN_MODULE_INIT(Init) {
  AVInput::Init(target);
  AVInfo::Init(target);
  Nan::Set(target, Nan::New<v8::String>("open").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(NewInput)->GetFunction());
}

NODE_MODULE(ffmpeg, Init)
