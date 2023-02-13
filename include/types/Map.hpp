#pragma once

#include "v8/include/v8.h"
#include <map>

#include "../Value.hpp"
#include "../Generic.hpp"
#include "Array.hpp"
#include "String.hpp"

namespace JSWrapper
{
    using Map = Value<std::map<Generic, Generic>, v8::Map>;

    v8::Local<Map::JSValueType> Map::ToJSValue()
    {
        v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
        v8::Local<v8::Map> obj = v8::Map::New(isolate);
        for(auto& [key, value] : cppValue)
        {
            obj->Set(ctx, key.GetValue(), value.GetValue());
        }
        return obj;
    }
    Map::CppValueType Map::ToCppValue(Map::CppValueType&& defaultVal)
    {
        if(!GetHandle()->IsMap()) return defaultVal;

        v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
        std::vector<Generic> keys = Array{ GetHandle().As<v8::Map>()->AsArray() }.CppValue();
        Map::CppValueType map;
        uint32_t size = keys.size();
        for(uint32_t i = 0; i < size; i += 2)
        {
            v8::MaybeLocal<v8::Value> maybeVal = GetHandle().As<v8::Map>()->Get(ctx, keys[i].GetValue());
            v8::Local<v8::Value> val;
            if(!maybeVal.ToLocal(&val)) return defaultVal;
            map.insert({ keys[i], val });
        }
        return map;
    }

};  // namespace JSWrapper
