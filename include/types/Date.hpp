#pragma once

#include "v8/include/v8.h"
#include <chrono>

#include "../Value.hpp"

namespace JSWrapper
{
    v8::Local<v8::Date> Value<std::chrono::system_clock::time_point, v8::Date>::ToJSValue()
    {
        v8::MaybeLocal<v8::Value> maybeVal =
          v8::Date::New(isolate->GetEnteredOrMicrotaskContext(), std::chrono::duration_cast<std::chrono::milliseconds>(cppValue.time_since_epoch()).count());
        v8::Local<v8::Date> val;
        if(!maybeVal.ToLocal(&val)) return v8::Local<v8::Date>();
        return val;
    }
    std::chrono::system_clock::time_point Value<std::chrono::system_clock::time_point, v8::Date>::ToCppValue(std::chrono::system_clock::time_point&& defaultVal)
    {
        if(!jsValue->IsDate()) return std::chrono::system_clock::time_point{};
        return std::chrono::system_clock::time_point{} + std::chrono::milliseconds((int64_t)jsValue.As<v8::Date>()->ValueOf());
    }

    using Date = Value<std::chrono::system_clock::time_point, v8::Date>;

};  // namespace JSWrapper
