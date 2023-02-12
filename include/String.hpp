#pragma once

#include "v8/include/v8.h"

#include "Value.hpp"

namespace JSWrapper
{
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

    using String = Value<std::string, v8::String>;
    using WString = Value<std::string, v8::String>;

};  // namespace JSWrapper
