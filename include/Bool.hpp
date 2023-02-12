#pragma once

#include "v8/include/v8.h"

#include "Value.hpp"

namespace JSWrapper
{
    v8::Local<v8::Boolean> Value<bool, v8::Boolean>::ToJSValue()
    {
        return v8::Boolean::New(isolate, cppValue);
    }
    bool Value<bool, v8::Boolean>::ToCppValue(bool&& defaultVal)
    {
        return GetHandle()->ToBoolean(isolate)->Value();
    }

    using Bool = Value<bool, v8::Boolean>;
};  // namespace JSWrapper
