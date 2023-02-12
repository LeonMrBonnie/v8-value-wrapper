#pragma once

#include "v8/include/v8.h"

#include "../Value.hpp"

namespace JSWrapper
{
    v8::Local<v8::BigInt> Value<int64_t, v8::BigInt>::ToJSValue()
    {
        return v8::BigInt::New(isolate, cppValue);
    }
    int64_t Value<int64_t, v8::BigInt>::ToCppValue(int64_t&& defaultVal)
    {
        v8::MaybeLocal<v8::BigInt> maybeVal = GetHandle()->ToBigInt(isolate->GetEnteredOrMicrotaskContext());
        v8::Local<v8::BigInt> val;
        if(!maybeVal.ToLocal(&val)) return defaultVal;
        return val->Int64Value();
    }

    v8::Local<v8::BigInt> Value<uint64_t, v8::BigInt>::ToJSValue()
    {
        return v8::BigInt::NewFromUnsigned(isolate, cppValue);
    }
    uint64_t Value<uint64_t, v8::BigInt>::ToCppValue(uint64_t&& defaultVal)
    {
        v8::MaybeLocal<v8::BigInt> maybeVal = GetHandle()->ToBigInt(isolate->GetEnteredOrMicrotaskContext());
        v8::Local<v8::BigInt> val;
        if(!maybeVal.ToLocal(&val)) return defaultVal;
        return val->Uint64Value();
    }

    using BigInt = Value<int64_t, v8::BigInt>;
    using UnsignedBigInt = Value<uint64_t, v8::BigInt>;

};  // namespace JSWrapper
