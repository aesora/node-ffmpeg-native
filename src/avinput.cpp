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


#include "avinput.h"

AVInput::AVInput() {}
AVInput::~AVInput() {
  if (fmt_ctx != NULL) {
    avformat_close_input(&fmt_ctx);
    fmt_ctx = NULL;
  }
  if (avio_ctx && avio_ctx != NULL) {
    av_freep(&avio_ctx->buffer);
    av_freep(&avio_ctx);
  }
}

Nan::Persistent<v8::Function> AVInput::constructor;

struct buffer_data{
  uint8_t* ptr;
  size_t size;
};

static int read_packet(void* opaque, uint8_t* buf, int buf_size) {
  struct buffer_data *bd = (struct buffer_data *)opaque;
  buf_size = FFMIN(buf_size, bd->size);

  memcpy(buf, bd->ptr, buf_size);
  bd->ptr += buf_size;
  bd->size -= buf_size;

  return buf_size;
}

static void DictionaryToLocalObject(AVDictionary * dict, v8::Local<v8::Object> obj) {
  AVDictionaryEntry* tag = NULL;
  while ((tag = av_dict_get(dict, "", tag, AV_DICT_IGNORE_SUFFIX))) {
    Nan::Set(obj, Nan::New<v8::String>(tag->key).ToLocalChecked(), Nan::New<v8::String>(tag->value).ToLocalChecked());
  }
}

NAN_MODULE_INIT(AVInput::Init) {
  Nan::HandleScope scope;
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New<v8::String>("AVInput").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("metadata").ToLocalChecked(), GetMetadata);
  constructor.Reset(tpl->GetFunction());
}

NAN_METHOD(AVInput::New) {
  if (info.IsConstructCall()) {
    // init
    AVInput* input = new AVInput();
    input->fmt_ctx = NULL;
    input->avio_ctx = NULL;

    // check arguments
    if (info.Length() < 1) {
      return Nan::ThrowTypeError("at least 1 argument is required");
    }
    v8::Local<v8::Object> src = v8::Local<v8::Object>::Cast(info[0]);
    if (src->IsUndefined() || !node::Buffer::HasInstance(src)) {
      return Nan::ThrowTypeError("first argument should be of type Buffer");
    }
    av_log_set_level(AV_LOG_QUIET);

    // open stream
    uint8_t* avio_ctx_buffer = NULL;
    size_t avio_ctx_buffer_size = 4096;
    struct buffer_data bd = {0, 0};

    av_register_all();

    bd.ptr = reinterpret_cast<uint8_t*>(node::Buffer::Data(src));
    bd.size = node::Buffer::Length(src);

    if (!(input->fmt_ctx = avformat_alloc_context())) {
      Nan::ThrowError("ENOMEM");
      return;
    }
    if (!(avio_ctx_buffer = static_cast<uint8_t*>(av_malloc(avio_ctx_buffer_size)))) {
      Nan::ThrowError("ENOMEM");
      return;
    }
    if (!(input->avio_ctx = avio_alloc_context(avio_ctx_buffer, avio_ctx_buffer_size, 0, &bd, &read_packet, NULL, NULL))) {
      Nan::ThrowError("ENOMEM");
      return;
    }

    input->fmt_ctx->pb = input->avio_ctx;

    if (avformat_open_input(&input->fmt_ctx, NULL, NULL, NULL) < 0) {
      Nan::ThrowError("Could not open input");
      return;
    }

    if (avformat_find_stream_info(input->fmt_ctx, NULL) < 0) {
      Nan::ThrowError("Could not find stream information");
      return;
    }

    // wrap and return
    input->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {info[0]};
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

v8::Local<v8::Object> AVInput::NewInstance(v8::Local<v8::Value> arg) {
  Nan::EscapableHandleScope scope;

  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { arg };
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);

  return scope.Escape(instance);
}

NAN_GETTER(AVInput::GetMetadata) {
  Nan::HandleScope scope;

  AVInput* input = Nan::ObjectWrap::Unwrap<AVInput>(info.Holder());
  if (input->fmt_ctx == NULL || input->avio_ctx == NULL) {
    Nan::ThrowError("The stream is not open");
    return;
  }

  v8::Local<v8::Object> out = Nan::New<v8::Object>();
  Nan::Set(out, Nan::New<v8::String>("duration").ToLocalChecked(), Nan::New<v8::Number>(1.0 * input->fmt_ctx->duration / AV_TIME_BASE));
  Nan::Set(out, Nan::New<v8::String>("inputformat").ToLocalChecked(), Nan::New<v8::String>(input->fmt_ctx->iformat->name).ToLocalChecked());
  v8::Local<v8::Object> parentmeta = Nan::New<v8::Object>();
  DictionaryToLocalObject(input->fmt_ctx->metadata, parentmeta);
  Nan::Set(out, Nan::New<v8::String>("metadata").ToLocalChecked(), parentmeta);

  v8::Local<v8::Array> streams = Nan::New<v8::Array>(input->fmt_ctx->nb_streams);
  unsigned int i = 0;
  for (i = 0; i < input->fmt_ctx->nb_streams; i++) {
    AVStream* avstream = input->fmt_ctx->streams[i];
    v8::Local<v8::Object> stream = Nan::New<v8::Object>();
    Nan::Set(stream, Nan::New<v8::String>("index").ToLocalChecked(), Nan::New<v8::Integer>(avstream->index));
    Nan::Set(stream, Nan::New<v8::String>("duration").ToLocalChecked(), Nan::New<v8::Number>(avstream->duration * av_q2d(avstream->time_base)));
    Nan::Set(stream, Nan::New<v8::String>("codec").ToLocalChecked(), Nan::New<v8::String>(avcodec_get_name(avstream->codec->codec_id)).ToLocalChecked());
    Nan::Set(stream, Nan::New<v8::String>("codectype").ToLocalChecked(), Nan::New<v8::String>(av_get_media_type_string(avstream->codec->codec_type)).ToLocalChecked());
    Nan::Set(stream, Nan::New<v8::String>("bitrate").ToLocalChecked(), Nan::New<v8::Number>(static_cast<double>(avstream->codec->bit_rate)));
    v8::Local<v8::Object> meta = Nan::New<v8::Object>();
    DictionaryToLocalObject(avstream->metadata, meta);
    Nan::Set(stream, Nan::New<v8::String>("metadata").ToLocalChecked(), meta);
    switch (avstream->codec->codec_type) {
      case AVMEDIA_TYPE_VIDEO:
        Nan::Set(stream, Nan::New<v8::String>("width").ToLocalChecked(), Nan::New<v8::Integer>(avstream->codec->width));
        Nan::Set(stream, Nan::New<v8::String>("height").ToLocalChecked(), Nan::New<v8::Integer>(avstream->codec->height));
        // Nan::Set(stream, Nan::New<v8::String>("format").ToLocalChecked(), Nan::New<v8::String>(av_get_pix_fmt_name(avstream->codec->pix_fmt)).ToLocalChecked());
        if (avstream->codec->sample_aspect_ratio.num == 0) {
          Nan::Set(stream, Nan::New<v8::String>("sar").ToLocalChecked(), Nan::Undefined());
        } else {
          Nan::Set(stream, Nan::New<v8::String>("sar").ToLocalChecked(), Nan::New<v8::Number>(av_q2d(avstream->codec->sample_aspect_ratio)));
        }
        if (avstream->avg_frame_rate.num == 0) {
          Nan::Set(stream, Nan::New<v8::String>("fps").ToLocalChecked(), Nan::Undefined());
        } else {
          Nan::Set(stream, Nan::New<v8::String>("fps").ToLocalChecked(), Nan::New<v8::Number>(av_q2d(avstream->avg_frame_rate)));
        }
        if (avstream->r_frame_rate.num == 0) {
          Nan::Set(stream, Nan::New<v8::String>("tbr").ToLocalChecked(), Nan::Undefined());
        } else {
          Nan::Set(stream, Nan::New<v8::String>("tbr").ToLocalChecked(), Nan::New<v8::Number>(av_q2d(avstream->r_frame_rate)));
        }
        break;
      case AVMEDIA_TYPE_AUDIO:
        Nan::Set(stream, Nan::New<v8::String>("channels").ToLocalChecked(), Nan::New<v8::Integer>(avstream->codec->channels));
        Nan::Set(stream, Nan::New<v8::String>("format").ToLocalChecked(), Nan::New<v8::String>(av_get_sample_fmt_name(avstream->codec->sample_fmt)).ToLocalChecked());
        Nan::Set(stream, Nan::New<v8::String>("samplerate").ToLocalChecked(), Nan::New<v8::Integer>(avstream->codec->sample_rate));
        break;
      default:
        break;
    }
    Nan::Set(streams, i, stream);
  }
  Nan::Set(out, Nan::New<v8::String>("streams").ToLocalChecked(), streams);

  info.GetReturnValue().Set(out);
}
