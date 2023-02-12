#pragma once

#include "v8/include/v8.h"
#include <unordered_map>

#include "../Value.hpp"
#include "../Generic.hpp"
#include "Array.hpp"
#include "String.hpp"

namespace JSWrapper
{
    class Object : public Value<std::unordered_map<std::string, Generic>, v8::Object>
    {
        v8::Local<v8::Object> ToJSValue()
        {
            v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
            v8::Local<v8::Object> obj = v8::Object::New(isolate);
            for(auto& [key, value] : cppValue)
            {
                obj->Set(ctx, String{ key }.JSValue(), value.GetValue());
            }
            return obj;
        }
        std::unordered_map<std::string, Generic> ToCppValue(std::unordered_map<std::string, Generic>&& defaultVal)
        {
            if(!GetHandle()->IsObject()) return defaultVal;
            v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
            std::vector<Generic> keys = Array{ GetHandle().As<v8::Object>()->GetOwnPropertyNames(ctx) }.CppValue();
            std::unordered_map<std::string, Generic> map;
            for(auto& key : keys)
            {
                v8::MaybeLocal<v8::Value> maybeVal = GetHandle().As<v8::Object>()->Get(ctx, key.GetValue());
                v8::Local<v8::Value> val;
                if(!maybeVal.ToLocal(&val)) return defaultVal;
                map.insert({ String{ key }.CppValue(), val });
            }
            return map;
        }

    public:
        Generic At(const std::string& key)
        {
            if(hasCppValue) return cppValue.at(key);
            else if(hasJSValue)
            {
                v8::MaybeLocal<v8::Value> maybeValue = GetHandle().As<v8::Array>()->Get(isolate->GetEnteredOrMicrotaskContext(), String{ key }.JSValue());
                v8::Local<v8::Value> value;
                if(!maybeValue.ToLocal(&value)) return Generic{};
                return value;
            }
            else
                return Generic{};
        }

        Generic operator[](const std::string& key)
        {
            return At(key);
        }
        Generic operator[](const char* key)
        {
            return At(key);
        }
    };

};  // namespace JSWrapper
