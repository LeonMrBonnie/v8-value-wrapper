#pragma once

#include "v8/include/v8.h"

#include "../Value.hpp"

namespace JSWrapper
{
    using Bool = Value<bool, v8::Boolean>;

    v8::Local<Bool::JSValueType> Bool::ToJSValue()
    {
        return v8::Boolean::New(isolate, cppValue);
    }
    Bool::CppValueType Bool::ToCppValue(Bool::CppValueType&& defaultVal)
    {
        return GetHandle()->ToBoolean(isolate)->Value();
    }

};  // namespace JSWrapper
