#pragma once

#include "v8/include/v8.h"

#include "../Value.hpp"

namespace JSWrapper
{
    v8::Local<v8::External> Value<void*, v8::External>::ToJSValue()
    {
        return v8::External::New(isolate, cppValue);
    }
    void* Value<void*, v8::External>::ToCppValue(void*&& defaultVal)
    {
        if(!GetHandle()->IsExternal()) return defaultVal;
        return GetHandle().As<v8::External>()->Value();
    }

    using External = Value<void*, v8::External>;

};  // namespace JSWrapper
