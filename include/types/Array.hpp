#pragma once

#include "v8/include/v8.h"

#include "../Value.hpp"
#include "../Generic.hpp"

namespace JSWrapper
{
    using Array = Value<std::vector<Generic>, v8::Array>;

    v8::Local<Array::JSValueType> Array::ToJSValue()
    {
        v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
        v8::Local<v8::Array> arr = v8::Array::New(isolate, (int)cppValue.size());
        for(size_t i = 0; i < cppValue.size(); i++)
        {
            arr->Set(ctx, i, cppValue.at(i).GetValue());
        }
        return arr;
    }
    Array::CppValueType Array::ToCppValue(Array::CppValueType&& defaultVal)
    {
        if(!GetHandle()->IsArray()) return defaultVal;

        v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
        uint32_t size = GetHandle().As<v8::Array>()->Length();
        Array::CppValueType vector;
        vector.reserve(size);
        for(uint32_t i = 0; i < size; i++)
        {
            v8::MaybeLocal<v8::Value> maybeVal = GetHandle().As<v8::Array>()->Get(ctx, i);
            v8::Local<v8::Value> val;
            if(!maybeVal.ToLocal(&val)) return defaultVal;
            vector.push_back(val);
        }
        return vector;
    }

};  // namespace JSWrapper
