#pragma once

#include "v8/include/v8.h"

#include "../Value.hpp"

namespace JSWrapper
{
    using ArrayBuffer = Value<std::vector<char>, v8::ArrayBuffer>;
    using SharedArrayBuffer = Value<std::vector<char>, v8::SharedArrayBuffer>;

    v8::Local<ArrayBuffer::JSValueType> ArrayBuffer::ToJSValue()
    {
        v8::Local<v8::ArrayBuffer> val = v8::ArrayBuffer::New(isolate, cppValue.size());
        std::memcpy(val->GetBackingStore()->Data(), cppValue.data(), cppValue.size() * sizeof(decltype(cppValue)::value_type));
        return val;
    }
    ArrayBuffer::CppValueType ArrayBuffer::ToCppValue(ArrayBuffer::CppValueType&& defaultVal)
    {
        if(!GetHandle()->IsArrayBuffer()) return defaultVal;
        std::shared_ptr<v8::BackingStore> store = jsValue.As<v8::ArrayBuffer>()->GetBackingStore();
        ArrayBuffer::CppValueType vec((char*)store->Data(), (char*)((uintptr_t)store->Data() + store->ByteLength()));
        return vec;
    }

    v8::Local<SharedArrayBuffer::JSValueType> SharedArrayBuffer::ToJSValue()
    {
        v8::Local<v8::SharedArrayBuffer> val = v8::SharedArrayBuffer::New(isolate, cppValue.size());
        std::memcpy(val->GetBackingStore()->Data(), cppValue.data(), cppValue.size() * sizeof(decltype(cppValue)::value_type));
        return val;
    }
    SharedArrayBuffer::CppValueType SharedArrayBuffer::ToCppValue(SharedArrayBuffer::CppValueType&& defaultVal)
    {
        if(!GetHandle()->IsSharedArrayBuffer()) return defaultVal;
        std::shared_ptr<v8::BackingStore> store = jsValue.As<v8::SharedArrayBuffer>()->GetBackingStore();
        SharedArrayBuffer::CppValueType vec((char*)store->Data(), (char*)((uintptr_t)store->Data() + store->ByteLength()));
        return vec;
    }

};  // namespace JSWrapper
