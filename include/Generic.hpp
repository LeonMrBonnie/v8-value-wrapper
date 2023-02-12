#pragma once

#include "v8/include/v8.h"

namespace JSWrapper
{
    enum class GenericType : uint8_t
    {
        INVALID,
        NUMBER,
        BIGINT,
        BOOL,
        STRING,
        ARRAY,
        OBJECT
    };

    class Generic
    {
        v8::Local<v8::Value> value;
        GenericType type;

    public:
        Generic() : type(GenericType::INVALID) {}
        Generic(v8::Local<v8::Value> _value) : value(_value), type(GetGenericType(_value)) {}

        GenericType GetType() const
        {
            return type;
        }
        v8::Local<v8::Value> GetValue() const
        {
            return value;
        }

        static GenericType GetGenericType(v8::Local<v8::Value> value)
        {
            if(value->IsNumber()) return GenericType::NUMBER;
            if(value->IsBigInt()) return GenericType::BIGINT;
            if(value->IsBoolean()) return GenericType::BOOL;
            if(value->IsString()) return GenericType::STRING;
            if(value->IsArray()) return GenericType::ARRAY;
            if(value->IsObject()) return GenericType::OBJECT;
            return GenericType::INVALID;
        }
    };
};  // namespace JSWrapper
