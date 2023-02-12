# V8 Value Wrapper

A header-only library to simplify working with V8 JavaScript values.

Documentation still WIP.

## Usage

Simply include `include/Wrapper.hpp` in your project and use the `JSWrapper` namespace classes.

Available classes:

### Value

A two-way wrapper for JavaScript <-> C++ values. Pass the C++ type and the V8 type to the template of the class and then construct
the Value using either the C++ value or the JS handle.

For convenience, there are using directives available for the supported types such as `JSWrapper::String` which is a `JSWrapper::Value<std::string, v8::String>`
value.

### Array

WIP

### Object

WIP

## Configuration

This library supports the usage of persistent or local handles to store inside the value wrapper, to toggle which one to use
see [Value.hpp#L8-L14](include/Value.hpp#L8-L14).

## Planned features

- [ ] Implement STL-like methods for Array and Object to allow range-based for loops
- [ ] Implement support for more JavaScript types (Set, Map, etc.)
