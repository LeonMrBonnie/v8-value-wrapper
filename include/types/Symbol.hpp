#pragma once

#include "v8/include/v8.h"

#include "../Value.hpp"
#include "String.hpp"

namespace JSWrapper
{
    using Symbol = Value<std::string, v8::Symbol>;
    using WSymbol = Value<std::wstring, v8::Symbol>;

    v8::Local<Symbol::JSValueType> Symbol::ToJSValue()
    {
        v8::MaybeLocal<v8::String> maybeVal = v8::Symbol::New(isolate, String{ cppValue }.JSValue());
        v8::Local<v8::String> val;
        if(!maybeVal.ToLocal(&val)) return v8::Local<v8::String>();
        return val;
    }
    Symbol::CppValueType Symbol::ToCppValue(Symbol::CppValueType&& defaultVal)
    {
        if(!GetHandle()->IsSymbol()) return defaultVal;
        return String{ GetHandle().As<v8::Symbol>()->Description(isolate) }.CppValue();
    }

    v8::Local<WSymbol::JSValueType> WSymbol::ToJSValue()
    {
        v8::MaybeLocal<v8::String> maybeVal = v8::Symbol::New(isolate, WString{ cppValue }.JSValue());
        v8::Local<v8::String> val;
        if(!maybeVal.ToLocal(&val)) return v8::Local<v8::String>();
        return val;
    }
    WSymbol::CppValueType WSymbol::ToCppValue(WSymbol::CppValueType&& defaultVal)
    {
        if(!GetHandle()->IsSymbol()) return defaultVal;
        return WString{ GetHandle().As<v8::Symbol>()->Description(isolate) }.CppValue();
    }

};  // namespace JSWrapper
