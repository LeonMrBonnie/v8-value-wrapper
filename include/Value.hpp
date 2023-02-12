#pragma once

#include "v8/include/v8.h"
#include <string>

#include "Generic.hpp"

// Whether to use persistent or local handles for storing the JS values.
// Using a persistent handle means the wrapper instance can be stored as long
// as the Isolate is alive, but comes at the cost of being slower to construct.
// If using local handles, the wrapper instance should only be used stack allocated.
#ifndef WRAPPER_USE_PERSISTENT_HANDLES
    #define WRAPPER_USE_PERSISTENT_HANDLES 0
#endif

namespace JSWrapper
{
    template<typename CppType, typename JSType>
    class Value
    {
    protected:
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        bool hasJSValue = false;
#if WRAPPER_USE_PERSISTENT_HANDLES == 1
        v8::Global<v8::Value> jsValue;
#else
        v8::Local<v8::Value> jsValue;
#endif
        bool hasCppValue = false;
        CppType cppValue;

        v8::Local<v8::Value> GetHandle()
        {
#if WRAPPER_USE_PERSISTENT_HANDLES == 1
            return jsValue.Get(isolate);
#else
            return jsValue;
#endif
        }

        v8::Local<JSType> ToJSValue()
        {
            static_assert(false, "Invalid Value<CppType, JSType>::ToJSValue call, using unsupported types?");
        }
        CppType ToCppValue(CppType&& defaultValue)
        {
            static_assert(false, "Invalid Value<CppType, JSType>::ToCppValue call, using unsupported types?");
        }

    public:
        using CppValueType = CppType;
        using JSValueType = JSType;

        Value() = delete;
        Value(v8::Local<v8::Value> _value) : hasJSValue(_value.IsEmpty())
        {
#if WRAPPER_USE_PERSISTENT_HANDLES == 1
            if(!_value.IsEmpty()) jsValue.Reset(isolate, _value);
#else
            if(!_value.IsEmpty()) jsValue = _value;
#endif
        }
        Value(v8::MaybeLocal<v8::Value> _value) : hasJSValue(_value.IsEmpty())
        {
#if WRAPPER_USE_PERSISTENT_HANDLES == 1
            if(!_value.IsEmpty()) jsValue.Reset(isolate, _value.ToLocalChecked());
#else
            if(!_value.IsEmpty()) jsValue = _value.ToLocalChecked();
#endif
        }
        Value(const CppType& _value) : hasCppValue(true), cppValue(_value) {}
        Value(const Generic& _value) : hasJSValue(_value.GetValue().IsEmpty())
        {
#if WRAPPER_USE_PERSISTENT_HANDLES == 1
            if(!_value.GetValue().IsEmpty()) jsValue.Reset(isolate, _value.GetValue());
#else
            if(!_value.GetValue().IsEmpty()) jsValue = _value.GetValue();
#endif
        }
        ~Value()
        {
#if WRAPPER_USE_PERSISTENT_HANDLES == 1
            jsValue.Reset();
#endif
        }

        v8::Local<JSType> JSValue()
        {
            if(!Valid()) return v8::Local<JSType>();
            if(!hasJSValue && hasCppValue)
            {
                hasJSValue = true;
#if WRAPPER_USE_PERSISTENT_HANDLES == 1
                jsValue.Reset(isolate, ToJSValue());
#else
                jsValue = ToJSValue();
#endif
            }
            return jsValue.IsEmpty() ? v8::Local<JSType>() : GetHandle().As<JSType>();
        }
        CppType CppValue(CppType&& defaultValue = CppType())
        {
            if(!Valid()) return defaultValue;
            if(!hasCppValue && hasJSValue)
            {
                hasCppValue = true;
                cppValue = ToCppValue(defaultValue);
            }
            return cppValue;
        }

        bool HasJSValue()
        {
            return hasJSValue;
        }
        bool HasCppValue()
        {
            return hasCppValue;
        }

        bool Valid()
        {
            return HasJSValue() || HasCppValue();
        }

        operator CppType()
        {
            return CppValue();
        }
        operator v8::Local<JSType>()
        {
            return JSValue();
        }
    };

    // *** Specializations

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

    // Number
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(double);
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(float);
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(uint32_t);
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(int32_t);
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(uint16_t);
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(int16_t);
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(uint8_t);
    DEFINE_WRAPPER_VALUE_NUMBER_SPECIALIZATION(int8_t);

    // BigInt
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

    // Bool
    v8::Local<v8::Boolean> Value<bool, v8::Boolean>::ToJSValue()
    {
        return v8::Boolean::New(isolate, cppValue);
    }
    bool Value<bool, v8::Boolean>::ToCppValue(bool&& defaultVal)
    {
        return GetHandle()->ToBoolean(isolate)->Value();
    }

    // String
    v8::Local<v8::String> Value<std::string, v8::String>::ToJSValue()
    {
        v8::MaybeLocal<v8::String> maybeVal = v8::String::NewFromUtf8(isolate, cppValue.c_str(), v8::NewStringType::kNormal, cppValue.length());
        v8::Local<v8::String> val;
        if(!maybeVal.ToLocal(&val)) return v8::Local<v8::String>();
        return val;
    }
    std::string Value<std::string, v8::String>::ToCppValue(std::string&& defaultVal)
    {
        return *v8::String::Utf8Value(isolate, GetHandle());
    }

    v8::Local<v8::String> Value<std::wstring, v8::String>::ToJSValue()
    {
        v8::MaybeLocal<v8::String> maybeVal = v8::String::NewFromTwoByte(isolate, (uint16_t*)cppValue.c_str(), v8::NewStringType::kNormal, cppValue.length());
        v8::Local<v8::String> val;
        if(!maybeVal.ToLocal(&val)) return v8::Local<v8::String>();
        return val;
    }
    std::wstring Value<std::wstring, v8::String>::ToCppValue(std::wstring&& defaultVal)
    {
        return (wchar_t*)*v8::String::Value(isolate, GetHandle());
    }

    // Using directives for easier access
    using Double = Value<double, v8::Number>;
    using Float = Value<float, v8::Number>;
    using Int = Value<int32_t, v8::Number>;
    using Int16 = Value<int16_t, v8::Number>;
    using Int8 = Value<int8_t, v8::Number>;
    using UInt = Value<uint32_t, v8::Number>;
    using UInt16 = Value<uint16_t, v8::Number>;
    using UInt8 = Value<uint8_t, v8::Number>;
    using BigInt = Value<int64_t, v8::BigInt>;
    using UnsignedBigInt = Value<uint64_t, v8::BigInt>;
    using Bool = Value<bool, v8::Boolean>;
    using String = Value<std::string, v8::String>;
    using WString = Value<std::string, v8::String>;

    template<typename NumberType>
    using Number = Value<NumberType, v8::Number>;

};  // namespace JSWrapper
