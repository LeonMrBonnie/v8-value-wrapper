#pragma once

#include "v8/include/v8.h"

#include "../Value.hpp"

namespace JSWrapper
{
    using External = Value<void*, v8::External>;

    v8::Local<External::JSValueType> External::ToJSValue()
    {
        return v8::External::New(isolate, cppValue);
    }
    External::CppValueType External::ToCppValue(External::CppValueType&& defaultVal)
    {
        if(!GetHandle()->IsExternal()) return defaultVal;
        return GetHandle().As<v8::External>()->Value();
    }

};  // namespace JSWrapper
