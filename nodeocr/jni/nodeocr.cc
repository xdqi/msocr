#include <node.h>

extern "C" {
    void ocr_init(const char *path);
    char *ocr_recognize_bitmap(const uint8_t *lpBitmap, int width, int height);
    char *ocr_recognize_image(const uint8_t *lpData, size_t size);
    char *barcode_recognize_bitmap(const uint8_t *lpBitmap, int width, int height);
    char *barcode_recognize_image(const uint8_t *lpData, size_t size);
    void ocr_free(char *result);
}

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Exception;
using v8::Uint8Array;

namespace {
    const char* ToCString(Local<String> str) {
        String::Utf8Value value(str);
        return *value ? *value : "<string conversion failed>";
    }
}

namespace nodeocr {
    void Init(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();

        if (args.Length() != 1) {
            // Throw an Error that is passed back to JavaScript
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
            return;
        }

        if (!args[0]->IsString()) {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
            return;
        }

        auto path = args[0]->ToString();
        ocr_init(ToCString(path));
    }

    void RecognizeBitmap(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();

        if (args.Length() != 3) {
            // Throw an Error that is passed back to JavaScript
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
            return;
        }

        if (!args[0]->IsUint8Array() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
            return;
        }

        // read first argument as an Uint8Array.
        Local<Uint8Array> view = args[0].As<Uint8Array>();
        auto contents = view->Buffer()->GetContents();
        int width = args[1]->NumberValue();
        int height = args[2]->NumberValue();

        char *ocr_result = ocr_recognize_bitmap((const uint8_t *) contents.Data(), width, height);
        char *barcode_result = barcode_recognize_bitmap((const uint8_t *) contents.Data(), width, height);

        Local<Object> obj = Object::New(isolate);
        obj->Set(String::NewFromUtf8(isolate, "ocr"), String::NewFromUtf8(isolate, ocr_result));
        obj->Set(String::NewFromUtf8(isolate, "barcode"), String::NewFromUtf8(isolate, barcode_result));

        args.GetReturnValue().Set(obj);
        ocr_free(ocr_result);
        ocr_free(barcode_result);
    }

    void RecognizeImage(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();

        if (args.Length() != 1) {
            // Throw an Error that is passed back to JavaScript
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
            return;
        }

        if (!args[0]->IsUint8Array()) {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
            return;
        }

        // read first argument as an Uint8Array.
        Local<Uint8Array> view = args[0].As<Uint8Array>();
        auto contents = view->Buffer()->GetContents();

        char *ocr_result = ocr_recognize_image((const uint8_t *) contents.Data(), contents.ByteLength());
        char *barcode_result = barcode_recognize_image((const uint8_t *) contents.Data(), contents.ByteLength());

        Local<Object> obj = Object::New(isolate);
        obj->Set(String::NewFromUtf8(isolate, "ocr"), String::NewFromUtf8(isolate, ocr_result));
        obj->Set(String::NewFromUtf8(isolate, "barcode"), String::NewFromUtf8(isolate, barcode_result));

        args.GetReturnValue().Set(obj);
        ocr_free(ocr_result);
        ocr_free(barcode_result);
    }

    void init(Local<Object> exports) {
        NODE_SET_METHOD(exports, "init", Init);
        NODE_SET_METHOD(exports, "recognize_image", RecognizeImage);
        NODE_SET_METHOD(exports, "recognize_bitmap", RecognizeBitmap);
    }

    NODE_MODULE(addon, init)

}  // namespace demo
