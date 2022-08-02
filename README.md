# cpp-json

cpp json is a simple and lightweight JSON parser and generator for C++. 

## JSON Elements

We define several classes to represent JSON data. All JSON data inherits from an abstract class `json::element`.

| JSON data          | C++ type                                          |
| ------------------ | ------------------------------------------------- |
| json::object       | std::unordered_map\<std::string, json::element*\> |
| json::array        | std::vector\<json::element*\>                     |
| json::string       | std::string                                       |
| json::number       | double                                            |
| json::boolean      | bool                                              |
| json::null         | nullptr                                           |

`json::element` has a method `get_value` that uses generics to return an equivalent C++ type of the JSON data.

```c++
    json::boolean b(true);
    b.get_value<bool>() // true
```

## JSON Gen

i.e To generate the following User JSON data:

```json
    {
        "name": "Jayden Carr",
        "age": 24,
        "single": true,
        "phones": [
            "(945)-441-6267",
            "(945)-441-5532"
        ],
        "location": {
            "state": "Hawaii",
            "country": "United States",
            "city": "Carlsbad",
            "street": {
                "name": "Forest Ln",
                "number": 1810
            }
        }
    }
```

We can use the following code:

```c++
    // Create a User object
    auto user = new json::object();
    user->set("name", "Jayden Carr");
    user->set("age", 24);
    user->set("single", true);
        // create an array of phones
        auto phones = new json::array();
        phones->add("(945)-441-6267");
        phones->add("(945)-441-5532");
    user->set("phones", phones);
        // create a location object
        auto location = new json::object();
        location->set("state", "Hawaii");
        location->set("country", "United States");
        location->set("city", "Carlsbad");
            // create a street object
            auto street = new json::object();
            street->set("name", "Forest Ln");
            street->set("number", 1810);
        location->set("street", street);
    user->set("location", location);
```

## JSON Stringify

`json::element` contains a method `to_string` that can be used to get a JSON string representation of any JSON data.

```c++
string s = user->to_string();
```

## JSON Parse

To parse a given JSON string, we can use the `json::parse` function.

```c++
    auto e = json::parse(s); 
    // e is a json::element*
    // Considering the above example, it can be casted to a json::object*
    auto user = (json::object*)e;
```

## Testing

Compile and run `test.cpp`

    g++ test.cpp json.hpp json.cpp -o test
	./test
