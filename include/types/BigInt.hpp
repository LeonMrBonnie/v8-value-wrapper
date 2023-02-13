#pragma once

#include "v8/include/v8.h"

#include "../Value.hpp"

namespace JSWrapper
{
    using BigInt = Value<int64_t, v8::BigInt>;
    using UnsignedBigInt = Value<uint64_t, v8::BigInt>;

    v8::Local<BigInt::JSValueType> BigInt::ToJSValue()
    {
        return v8::BigInt::New(isolate, cppValue);
    }
    BigInt::CppValueType BigInt::ToCppValue(BigInt::CppValueType&& defaultVal)
    {
        v8::MaybeLocal<v8::BigInt> maybeVal = GetHandle()->ToBigInt(isolate->GetEnteredOrMicrotaskContext());
        v8::Local<v8::BigInt> val;
        if(!maybeVal.ToLocal(&val)) return defaultVal;
        return val->Int64Value();
    }

    v8::Local<UnsignedBigInt::JSValueType> UnsignedBigInt::ToJSValue()
    {
        return v8::BigInt::NewFromUnsigned(isolate, cppValue);
    }
    UnsignedBigInt::CppValueType UnsignedBigInt::ToCppValue(UnsignedBigInt::CppValueType&& defaultVal)
    {
        v8::MaybeLocal<v8::BigInt> maybeVal = GetHandle()->ToBigInt(isolate->GetEnteredOrMicrotaskContext());
        v8::Local<v8::BigInt> val;
        if(!maybeVal.ToLocal(&val)) return defaultVal;
        return val->Uint64Value();
    }

};  // namespace JSWrapper
