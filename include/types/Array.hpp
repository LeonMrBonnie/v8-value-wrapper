#pragma once

#include "v8/include/v8.h"

#include "../Value.hpp"

namespace JSWrapper
{
    class Array : public Value<std::vector<Generic>, v8::Array>
    {
        v8::Local<v8::Array> ToJSValue()
        {
            v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
            v8::Local<v8::Array> arr = v8::Array::New(isolate, (int)cppValue.size());
            for(size_t i = 0; i < cppValue.size(); i++)
            {
                arr->Set(ctx, i, cppValue.at(i).GetValue());
            }
            return arr;
        }
        std::vector<Generic> ToCppValue(std::vector<Generic>&& defaultVal)
        {
            if(!GetHandle()->IsArray()) return defaultVal;

            v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
            uint32_t size = GetHandle().As<v8::Array>()->Length();
            std::vector<Generic> vector;
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

    public:
        Generic At(uint32_t index)
        {
            if(hasCppValue) return cppValue.at(index);
            else if(hasJSValue)
            {
                v8::MaybeLocal<v8::Value> maybeValue = GetHandle().As<v8::Array>()->Get(isolate->GetEnteredOrMicrotaskContext(), index);
                v8::Local<v8::Value> value;
                if(!maybeValue.ToLocal(&value)) return Generic{};
                return value;
            }
            else
                return Generic{};
        }

        Generic operator[](int index)
        {
            return At(index);
        }

        size_t GetSize()
        {
            if(hasCppValue) return cppValue.size();
            else if(hasJSValue)
                return GetHandle().As<v8::Array>()->Length();
            else
                return 0;
        }
    };

};  // namespace JSWrapper
