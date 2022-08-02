#include "json.hpp"

// ---------------------------------- json::null ----------------------------------

std::string json::null::to_string(std::string) const {
    return "null";
}

json::null* json::null::parse(const std::string& s, size_t& i) {
    if (i + 4 <= s.size() && s.substr(i, 4) == "null") {
        i += 4;
        return new null();
        }
        throw std::runtime_error("invalid json null");
}

void* json::null::_get_value() const {
    return nullptr;
}

// ---------------------------------- json::boolean ----------------------------------

std::string json::boolean::to_string(std::string) const {
    return value ? "true" : "false";
}

json::boolean* json::boolean::parse(const std::string& s, size_t& i) {
    if (i + 4 <= s.size() && s.substr(i, 4) == "true") {
        i += 4;
        return new boolean(true);
    }
    else if (i + 5 <= s.size() && s.substr(i, 5) == "false") {
        i += 5;
        return new boolean(false);
    }
    throw std::runtime_error("invalid json bool");
}

void* json::boolean::_get_value() const {
    return new bool(value);
}

// ---------------------------------- json::number ----------------------------------

std::string json::number::to_string(std::string) const {
    return value == (int)value ? std::to_string((int)value) : std::to_string(value);
}

json::number* json::number::parse(const std::string& s, size_t& i) {
    double value = 0;
    bool is_negative = false;
    if (s[i] == '-') {
        is_negative = true;
        i++;
    }
    while (i < s.size() && s[i] >= '0' && s[i] <= '9') {
        value = value * 10 + (s[i] - '0');
        i++;
    }
    if (i < s.size() && s[i] == '.') {
        i++;
        double divider = 0.1;
        while (i < s.size() && s[i] >= '0' && s[i] <= '9') {
            value += (s[i] - '0') * divider;
            divider *= 0.1;
            i++;
        }
    }
    if (is_negative) {
        value = -value;
    }
    return new number(value);
}

void* json::number::_get_value() const {
    return new double(value);
}

// ---------------------------------- json::string ----------------------------------

std::string json::string::to_string(std::string) const {
    return "\"" + *this + "\"";
}

json::string* json::string::parse(const std::string& s, size_t& i) {
    if (s[i] == '"') {
        i++;
        std::string value;
        while (i < s.size()) {
            if (s[i] == '"' && i - 1 >= 0 && s[i - 1] != '\\') {
                break;
            }
            value += s[i];
            i++;
        }
        if (i < s.size() && s[i] == '"') {
            i++;
            return new string(value);
        }
    }
    throw std::runtime_error("invalid json string");
}

void* json::string::_get_value() const {
    return new std::string(*this);
}

// ---------------------------------- json::array ----------------------------------

std::string json::array::to_string(std::string indent) const {
    std::string s = "[\n";
    for (size_t i = 0; i < this->size(); i++) {
        s += indent + _INDENT_ + (*this)[i]->to_string(indent + _INDENT_);
        if (i + 1 < this->size()) {
            s += ",\n";
        }
    }
    return s + "\n" + indent + "]";
}

void json::array::add(element* value) {
    this->push_back(value);
}

void json::array::add(const bool& value) {
    add(new boolean(value));
}

void json::array::add(const double& value) {
    add(new number(value));
}

void json::array::add(const char* value) {
    add(new string(value));
}

void json::array::add(const json::array& value) {
    add(new array(value));
}

void json::array::add(const json::object& value) {
    add(new object(value));
}

json::array* json::array::parse(const std::string& s, size_t& i) {
    if (s[i] == '[') {
        i++;
        array* array = new json::array();
        do {
            _skip_whitespace(s, i);
            if (i < s.size() && s[i] == ',') {
                i++;
            }
            _skip_whitespace(s, i);
            array->push_back(_parse(s, i));
            _skip_whitespace(s, i);
        } while (i < s.size() && s[i] == ',');
        if (i < s.size() && s[i] == ']') {
            i++;
            return array;
        }
    }
    throw std::runtime_error("invalid json array");
}

void* json::array::_get_value() const {
    return new std::vector<element*>(*this);
}

// ---------------------------------- json::object ----------------------------------

std::string json::object::to_string(std::string indent) const {
    std::string s = "{\n";
    auto it = this->begin();
    while (it != this->end()) {
        s += indent + _INDENT_ + "\"" + it->first + "\": " + it->second->to_string(indent + _INDENT_);
        it++;
        if (it != this->end()) {
            s += ",\n";
        }
    }
    return s + "\n" + indent + "}";
}


void json::object::set(const std::string& key, element* value) {
    this->operator[](key) = value;
}

void json::object::set(const std::string& key, bool value) {
    set(key, new boolean(value));
}

void json::object::set(const std::string& key, double value) {
    set(key, new number(value));
}

void json::object::set(const std::string& key, int value) {
    set(key, new number(value));
}

void json::object::set(const std::string& key, const char* value) {
    set(key, new string(value));
}

void json::object::set(const std::string& key, const std::vector<element*>& value) {
    set(key, new array(value));
}

void json::object::set(const std::string& key, const std::unordered_map<std::string, element*>& value) {
    set(key, new object(value));
}

json::object* json::object::parse(const std::string& s, size_t& i) {
    if (s[i] == '{') {
        i++;
        object* object = new json::object();
        do {
            _skip_whitespace(s, i);
            if (i < s.size() && s[i] == ',') {
                i++;
            }

            _skip_whitespace(s, i);
            string key = *(string*)_parse(s, i);
            _skip_whitespace(s, i);
            if (i < s.size() && s[i] == ':') {
                i++;
                _skip_whitespace(s, i);
                (*object)[key] = _parse(s, i);
            }
            else {
                throw std::runtime_error("invalid json object");
            }
            _skip_whitespace(s, i);
        } while(i < s.size() && s[i] == ',');

        if (i < s.size() && s[i] == '}') {
            i++;
            return object;
        }
    }
    throw std::runtime_error("invalid json object");
}


void* json::object::_get_value() const {
    return new std::unordered_map<std::string, element*>(*this);
}

// ---------------------------------- json::parse ----------------------------------

json::element* json::parse(const std::string& str) {
    size_t i = 0;
    element* value = _parse(str, i);
    _skip_whitespace(str, i);
    if (i < str.size()) {
        throw std::runtime_error("invalid json");
    }
    return value;
}

json::element* json::_parse(const std::string& str, size_t& i) {
    _skip_whitespace(str, i);
    if (i < str.size()) {
        switch (str[i]) {
        case '{': return object::parse(str, i);
        case '[': return array::parse(str, i);
        case '"': return string::parse(str, i);
        case 't': return boolean::parse(str, i);
        case 'f': return boolean::parse(str, i);
        case 'n': return null::parse(str, i);
        default:
            if (str[i] >= '0' && str[i] <= '9') {
                return number::parse(str, i);
            }
        }
    }
    throw std::runtime_error("invalid json. line");
}

void json::_skip_whitespace(const std::string& str, size_t& i) {
    while (i < str.size() && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r')) {
        ++i;
    }
}
