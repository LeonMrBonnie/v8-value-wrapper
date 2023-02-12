#pragma once

#include "v8/include/v8.h"

#include "../Value.hpp"

namespace JSWrapper
{
#define DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(type)                                                      \
    v8::Local<v8::Number> Value<type, v8::Number>::ToJSValue()                                                \
    {                                                                                                         \
        return v8::Number::New(isolate, cppValue);                                                            \
    }                                                                                                         \
    type Value<type, v8::Number>::ToCppValue(type&& defaultVal)                                               \
    {                                                                                                         \
        v8::MaybeLocal<v8::Number> maybeVal = GetHandle()->ToNumber(isolate->GetEnteredOrMicrotaskContext()); \
        v8::Local<v8::Number> val;                                                                            \
        if(!maybeVal.ToLocal(&val)) return defaultVal;                                                        \
        return (type)val->Value();                                                                            \
    }
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(double);
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(float);
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(uint32_t);
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(int32_t);
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(uint16_t);
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(int16_t);
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(uint8_t);
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(int8_t);

    using Double = Value<double, v8::Number>;
    using Float = Value<float, v8::Number>;
    using Int = Value<int32_t, v8::Number>;
    using Int16 = Value<int16_t, v8::Number>;
    using Int8 = Value<int8_t, v8::Number>;
    using UInt = Value<uint32_t, v8::Number>;
    using UInt16 = Value<uint16_t, v8::Number>;
    using UInt8 = Value<uint8_t, v8::Number>;

    template<typename NumberType>
    using Number = Value<NumberType, v8::Number>;

}  // namespace JSWrapper
