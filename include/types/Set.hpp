#pragma once

#include "v8/include/v8.h"
#include <set>

#include "../Value.hpp"
#include "../Generic.hpp"
#include "Array.hpp"

namespace JSWrapper
{
    using Set = Value<std::set<Generic>, v8::Set>;

    v8::Local<Set::JSValueType> Set::ToJSValue()
    {
        v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
        v8::Local<v8::Set> set = v8::Set::New(isolate);
        for(auto& value : cppValue)
        {
            set->Add(ctx, value.GetValue());
        }
        return set;
    }
    Set::CppValueType Set::ToCppValue(Set::CppValueType&& defaultVal)
    {
        if(!GetHandle()->IsSet()) return defaultVal;

        v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
        std::vector<Generic> keys = Array{ GetHandle().As<v8::Set>()->AsArray() }.CppValue();
        Set::CppValueType set;
        for(auto& key : keys)
        {
            set.insert(key);
        }
        return set;
    }

};  // namespace JSWrapper
