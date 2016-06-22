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


#ifndef SRC_AVINPUT_H_
#define SRC_AVINPUT_H_
extern "C" {
  #include <libavformat/avformat.h>
  #include <libavutil/avutil.h>
}

#include <nan.h>

class AVInput : public Nan::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init);
  static NAN_METHOD(New);
  static NAN_GETTER(GetMetadata);
  AVFormatContext* Input() const {return fmt_ctx;}
  static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value>);
 private:
  AVInput();
  ~AVInput();
  static Nan::Persistent<v8::Function> constructor;
  AVFormatContext* fmt_ctx;
  AVIOContext* avio_ctx;
};
#endif  // SRC_AVINPUT_H_
