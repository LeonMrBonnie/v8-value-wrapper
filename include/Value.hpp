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
        template<typename T = v8::Value>
        Value(v8::Local<T> _value) : hasJSValue(_value.IsEmpty())
        {
#if WRAPPER_USE_PERSISTENT_HANDLES == 1
            if(!_value.IsEmpty()) jsValue.Reset(isolate, _value);
#else
            if(!_value.IsEmpty()) jsValue = _value;
#endif
        }
        template<typename T = v8::Value>
        Value(v8::MaybeLocal<T> _value) : hasJSValue(_value.IsEmpty())
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

        bool HasJSValue() const
        {
            return hasJSValue;
        }
        bool HasCppValue() const
        {
            return hasCppValue;
        }

        bool Valid() const
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

};  // namespace JSWrapper
