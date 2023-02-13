#pragma once

#include "v8/include/v8.h"
#include <unordered_map>

#include "../Value.hpp"
#include "../Generic.hpp"
#include "Array.hpp"
#include "String.hpp"

namespace JSWrapper
{
    using Object = Value<std::unordered_map<std::string, Generic>, v8::Object>;

    v8::Local<Object::JSValueType> Object::ToJSValue()
    {
        v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
        v8::Local<v8::Object> obj = v8::Object::New(isolate);
        for(auto& [key, value] : cppValue)
        {
            obj->Set(ctx, String{ key }.JSValue(), value.GetValue());
        }
        return obj;
    }
    Object::CppValueType Object::ToCppValue(Object::CppValueType&& defaultVal)
    {
        if(!GetHandle()->IsObject()) return defaultVal;

        v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
        std::vector<Generic> keys = Array{ GetHandle().As<v8::Object>()->GetOwnPropertyNames(ctx) }.CppValue();
        Object::CppValueType map;
        for(auto& key : keys)
        {
            v8::MaybeLocal<v8::Value> maybeVal = GetHandle().As<v8::Object>()->Get(ctx, key.GetValue());
            v8::Local<v8::Value> val;
            if(!maybeVal.ToLocal(&val)) return defaultVal;
            map.insert({ String{ key }.CppValue(), val });
        }
        return map;
    }

};  // namespace JSWrapper
