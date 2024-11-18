#include <nan.h>

#include "pow.h"  // NOLINT(build/include)

using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::GetFunction;
using Nan::HandleScope;
using Nan::New;
using Nan::Null;
using Nan::Set;
using Nan::To;
using v8::Context;
using v8::Function;
using v8::FunctionTemplate;
using v8::Handle;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

Nan::TryCatch try_catch;

class EquihashSolutionWorker : public AsyncWorker {
 public:
  EquihashSolutionWorker(const unsigned n, const unsigned k, _POW::Seed seed,
                         Callback* callback)
      : AsyncWorker(callback), n(n), k(k), seed(seed) {}
  ~EquihashSolutionWorker() {}

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute() {
    _POW::Equihash equihash(n, k, seed);
    _POW::Proof p = equihash.FindProof();
    solution = p.inputs;
    nonce = p.nonce;
    // printhex("solution", &solution[0], solution.size());
  }

  // Executed when the async work is complete
  // this function will be run inside the main event loop
  // so it is safe to use V8 again
  void HandleOKCallback() {
    HandleScope scope;
    Local<Object> proofValue =
        Nan::CopyBuffer((const char*)&solution[0], solution.size() * 4)
            .ToLocalChecked();

    Local<Value> argv[] = {Null(), proofValue, New(nonce), New(n), New(k)};

    callback->Call(5, argv);
  }

 private:
  unsigned n;
  unsigned k;
  _POW::Nonce nonce;
  _POW::Seed seed;
  std::vector<_POW::Input> solution;
};

NAN_METHOD(Solve) {
  // ensure first argument is an object
  if (!info[0]->IsObject()) {
    Nan::ThrowTypeError("'options' must be an object");
    return;
  }
  // ensure second argument is a callback
  if (!info[1]->IsFunction()) {
    Nan::ThrowTypeError("'callback' must be a function");
    return;
  }

  Handle<Object> object = Handle<Object>::Cast(info[0]);
  Callback* callback = new Callback(info[1].As<Function>());

  v8::Local<v8::String> k_str = Nan::New<v8::String>("k").ToLocalChecked();
  v8::MaybeLocal<v8::Value> kValue = Nan::Get(object, k_str).ToLocalChecked();
  v8::Local<v8::String> n_str = Nan::New<v8::String>("n").ToLocalChecked();
  v8::MaybeLocal<v8::Value> nValue = Nan::Get(object, n_str);
  v8::Local<v8::String> nonce_str =
      Nan::New<v8::String>("nonce").ToLocalChecked();
  v8::MaybeLocal<v8::Value> nonceValue = Nan::Get(object, nonce_str);
  v8::Local<v8::String> seed_str =
      Nan::New<v8::String>("seed").ToLocalChecked();
  v8::MaybeLocal<v8::Value> seedValue = Nan::Get(object, seed_str);

  const unsigned n = To<uint32_t>(nValue.ToLocalChecked()).FromJust();
  const unsigned k = To<uint32_t>(kValue.ToLocalChecked()).FromJust();
  size_t bufferLength = node::Buffer::Length(seedValue.ToLocalChecked()) / 4;
  unsigned* seedBuffer =
      (unsigned*)node::Buffer::Data(seedValue.ToLocalChecked());

  _POW::Seed seed(seedBuffer, bufferLength);

  AsyncQueueWorker(new EquihashSolutionWorker(n, k, seed, callback));
}

NAN_METHOD(Verify) {
  // ensure first argument is an object
  if (!info[0]->IsObject()) {
    Nan::ThrowTypeError("'options' must be an object");
    return;
  }

  Handle<Object> object = Handle<Object>::Cast(info[0]);

  v8::Local<v8::String> k_str = Nan::New<v8::String>("k").ToLocalChecked();
  Nan::MaybeLocal<v8::Value> kValue = Nan::Get(object, k_str);
  v8::Local<v8::String> n_str = Nan::New<v8::String>("n").ToLocalChecked();
  Nan::MaybeLocal<v8::Value> nValue = Nan::Get(object, n_str);
  v8::Local<v8::String> nonce_str =
      Nan::New<v8::String>("nonce").ToLocalChecked();
  Nan::MaybeLocal<v8::Value> nonceValue = Nan::Get(object, nonce_str);
  v8::Local<v8::String> seed_str =
      Nan::New<v8::String>("seed").ToLocalChecked();
  Nan::MaybeLocal<v8::Value> seedValue = Nan::Get(object, seed_str);
  v8::Local<v8::String> value_str =
      Nan::New<v8::String>("value").ToLocalChecked();
  Nan::MaybeLocal<v8::Value> inputValue = Nan::Get(object, value_str);

  const unsigned n = To<uint32_t>(nValue.ToLocalChecked()).FromJust();
  const unsigned k = To<uint32_t>(kValue.ToLocalChecked()).FromJust();
  const unsigned nonce = To<uint32_t>(nonceValue.ToLocalChecked()).FromJust();
  size_t seedBufferLength =
      node::Buffer::Length(seedValue.ToLocalChecked()) / 4;
  unsigned* seedBuffer =
      (unsigned*)node::Buffer::Data(seedValue.ToLocalChecked());
  size_t inputBufferLength =
      node::Buffer::Length(inputValue.ToLocalChecked()) / 4;
  unsigned* inputBuffer =
      (unsigned*)node::Buffer::Data(inputValue.ToLocalChecked());

  // initialize the proof object
  _POW::Seed seed(seedBuffer, seedBufferLength);
  std::vector<_POW::Input> inputs;
  inputs.resize(inputBufferLength, 0);
  std::copy(inputBuffer, inputBuffer + inputBufferLength, inputs.begin());
  _POW::Proof p(n, k, seed, nonce, inputs);

  // check the proof
  info.GetReturnValue().Set(p.Test());
}

NAN_MODULE_INIT(InitAll) {
  Set(target, New<String>("solve").ToLocalChecked(),
      GetFunction(New<FunctionTemplate>(Solve)).ToLocalChecked());
  Set(target, New<String>("verify").ToLocalChecked(),
      GetFunction(New<FunctionTemplate>(Verify)).ToLocalChecked());
}

NODE_MODULE(addon, InitAll)
