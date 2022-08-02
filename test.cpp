#include "json.hpp"

#include <cassert>
#include <iostream>

using namespace std;

void log(const string& s) {
    cout << "[INFO]: " << s << endl;
}

int main() {
    // i.e. json = 
    // {
    //   "name": "Jayden Carr",
    //   "age": 24,
    //   "single": true,
    //   "phones": [
    //     "(945)-441-6267",
    //     "(945)-441-5532"
    //   ],
    //   "location": {
    //     "state": "Hawaii",
    //     "country": "United States",
    //     "city": "Carlsbad",
    //     "street": {
    //       "name": "Forest Ln",
    //       "number": 1810
    //     }
    //   }
    // }

    log("Starting JSON test");
    log("Generating JSON");
    auto user = new json::object();
    user->set("name", "Jayden Carr");
    log("User name set to " + user->at("name")->to_string());
    user->set("age", 24);
    log("User age set to " + user->at("age")->to_string());
    user->set("single", true);
    log("User single set to " + user->at("single")->to_string());
        auto phones = new json::array();
        phones->add("(945)-441-6267");
        log("Phone number added: " + phones->at(0)->to_string());
        phones->add("(945)-441-5532");
        log("Phone number added: " + phones->at(1)->to_string());
    user->set("phones", phones);
        auto location = new json::object();
        location->set("state", "Hawaii");
        log("Location state set to " + location->at("state")->to_string());
        location->set("country", "United States");
        log("Location country set to " + location->at("country")->to_string());
        location->set("city", "Carlsbad");
        log("Location city set to " + location->at("city")->to_string());
            auto street = new json::object();
            street->set("name", "Forest Ln");
            log("Street name set to " + street->at("name")->to_string());
            street->set("number", 1810);
            log("Street number set to " + street->at("number")->to_string());
        location->set("street", street);
    user->set("location", location);

    log("User JSON generated");
    string s = user->to_string();
    log("User = " + s);

    log("Parsing Generated JSON");
    auto user2 = (json::object*)json::parse(s);
    log("User JSON parsed");

    log("comparing JSON objects");
    
    assert(user->at("name")->get_value<string>() == user2->at("name")->get_value<string>());
    log("User name matches");
    assert(user->at("age")->get_value<double>() == user2->at("age")->get_value<double>());
    log("User age matches");
    assert(user->at("single")->get_value<bool>() == user2->at("single")->get_value<bool>());
    log("User single matches");
    auto phones2 = (json::array*)user2->at("phones");
    assert(phones->size() == phones2->size());
    for (int i = 0; i < phones->size(); i++) {
        assert(phones->at(i)->get_value<string>() == phones2->at(i)->get_value<string>());
        log("Phone number " + to_string(i) + " matches");
    }
    log("User phones match");
    auto location2 = (json::object*)user2->at("location");
    assert(location->at("state")->get_value<string>() == location2->at("state")->get_value<string>());
    log("Location state matches");
    assert(location->at("country")->get_value<string>() == location2->at("country")->get_value<string>());
    log("Location country matches");
    assert(location->at("city")->get_value<string>() == location2->at("city")->get_value<string>());
    log("Location city matches");
    auto street2 = (json::object*)location2->at("street");
    assert(street->at("name")->get_value<string>() == street2->at("name")->get_value<string>());
    log("Street name matches");
    assert(street->at("number")->get_value<double>() == street2->at("number")->get_value<double>());
    log("Street number matches");
    log("User JSON matches");
    log("JSON test complete");
    return 0;
}
