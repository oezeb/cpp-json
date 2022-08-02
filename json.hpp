#pragma once
#ifndef _JSON_
#define _JSON_

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

/// JSON parser
namespace json {
    /// `element` is the base class of all json elements.
    class element;
    /// `null` is a json null element.
    class null;
    /// `boolean` is a json boolean element.
    class boolean;
    /// `number` is a json number element.
    class number;
    /// `string` is a json string element.
    class string;
    /// `array` is a json array element.
    class array;
    /// `object` is a json object element.
    class object;
    /// `parse` parses a json string and returns a pointer to the root element.
    element* parse(const std::string&);
    // utility functions and variables
    element* _parse(const std::string&, size_t&);
    void _skip_whitespace(const std::string& str, size_t& i);
    const std::string _INDENT_ = "  ";
}

class json::element {
public:
    virtual std::string to_string(std::string indent = "") const = 0;
    /// `get_value` uses generics to return an equivalent C++ type or object.
    /// i.e. `get_value<bool>()` returns a C++ `bool`.
    template <typename T>
    T get_value() const { return *(T*)this->_get_value(); }
private:
    virtual void* _get_value() const = 0;
};

class json::null : public element {
public:
    std::string to_string(std::string = "") const override;

    static null* parse(const std::string&, size_t&);
private:
    void* _get_value() const override;
};

class json::boolean : public element {
public:
    boolean(bool value) : value(value) {}

    std::string to_string(std::string = "") const override;

    static boolean* parse(const std::string&, size_t&);
private:
    bool value;

    void* _get_value() const override;
};

class json::number : public element {
public:
    number(double value) : value(value) {}

    std::string to_string(std::string = "") const override;

    static number* parse(const std::string&, size_t&);
private:
    double value;

    void* _get_value() const override;
};

class json::string : public element, public std::string {
public:
    string(const std::string& value) : std::string(value) {}

    std::string to_string(std::string = "") const override;

    static string* parse(const std::string&, size_t&);
private:
    void* _get_value() const override;
};

class json::array : public element, public std::vector<element*> {
public:
    array() : std::vector<element*>() {}
    array(const std::vector<element*>& value) : std::vector<element*>(value) {}

    std::string to_string(std::string = "") const override;
    
    void add(element*);
    void add(const bool&);
    void add(const double&);
    void add(const char*);
    void add(const array&);
    void add(const object&);

    static array* parse(const std::string&, size_t&);
private:
    void* _get_value() const override;
};

class json::object : public element, public std::unordered_map<std::string, element*> {
public:
    object() : std::unordered_map<std::string, element*>() {}
    object(const std::unordered_map<std::string, element*>& value) 
        : std::unordered_map<std::string, element*>(value) {}

    std::string to_string(std::string = "") const override;

    void set(const std::string&, element*);
    void set(const std::string&, bool);
    void set(const std::string&, double);
    void set(const std::string&, int);
    void set(const std::string&, const char*);
    void set(const std::string&, const std::vector<element*>&);
    void set(const std::string&, const std::unordered_map<std::string, element*>&);

    static object* parse(const std::string&, size_t&);
private:
    void* _get_value() const override;
};

#endif // _JSON_
