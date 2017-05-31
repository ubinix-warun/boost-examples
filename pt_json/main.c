#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

using namespace boost::property_tree;

// https://boostjp.github.io/tips/json.html

void write_sample() {

    ptree pt;

    pt.put("Data.value", 3);
    pt.put("Data.str", "Hello");

    ptree child;
    {
        ptree info;
        info.put("id", 1);
        info.put("name", "Alice");
        child.push_back(std::make_pair("", info));
    }
    {
        ptree info;
        info.put("id", 2);
        info.put("name", "Millia");
        child.push_back(std::make_pair("", info));
    }
    pt.add_child("Data.info", child);

    write_json("data_out.json", pt);

}

void read_sample() {

    ptree pt;
    
    read_json("data_out.json", pt);

    // Data.value
    if (boost::optional<int> value = pt.get_optional<int>("Data.value")) {
        std::cout << "value : " << value.get() << std::endl;
    }
    else {
        std::cout << "value is nothing" << std::endl;
    }

    // Data.str
    if (boost::optional<std::string> str = pt.get_optional<std::string>("Data.str")) {
        std::cout << "str : " << str.get() << std::endl;
    }
    else {
        std::cout << "str is nothing" << std::endl;
    }

    // Data.info
    BOOST_FOREACH (const ptree::value_type& child, pt.get_child("Data.info")) {
        const ptree& info = child.second;

        // Data.info.id
        if (boost::optional<int> id = info.get_optional<int>("id")) {
            std::cout << "id : " << id.get() << std::endl;
        }
        else {
            std::cout << "id is nothing" << std::endl;
        }

        // Data.info.name
        if (boost::optional<std::string> name = info.get_optional<std::string>("name")) {
            std::cout << "name : " << name.get() << std::endl;
        }
        else {
            std::cout << "name is nothing" << std::endl;
        }
    }
}

// https://gist.github.com/mloskot/1509935

void read_string() {
    try
    {
        std::stringstream ss;
        ss << "{ \"root\": { \"values\": [1, 2, 3, 4, 5 ] } }";

        boost::property_tree::ptree pt;
        boost::property_tree::read_json(ss, pt);

        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("root.values"))
        {
            assert(v.first.empty()); // array elements have no names
            std::cout << v.second.data() << std::endl;
        }
        return;
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }

}

int main()
{
    write_sample();
    read_sample();
    read_string();

}