#include "init.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "fmt/format.h"
#include "tools.hpp"
#include "mod.hpp"

namespace mod
{
    inline void save()
    {
        rapidjson::Document document;
        document.SetObject();
        auto& allocator = document.GetAllocator();
        for (const auto& pair : mod_map)
        {
            std::string keyStr = fmt::format("{}", pair.first);
            rapidjson::Value jsonKey(keyStr.c_str(), allocator);
            rapidjson::Value jsonVal;
            const Module& mod = pair.second;

            if (mod::assert_type<bool>(pair.first)) {
                jsonVal.SetBool(mod::module_by_id<bool>(pair.first));
            } 
            if (mod::assert_type<int>(pair.first)) {
                jsonVal.SetInt(mod::module_by_id<int>(pair.first));
            } 
            if (mod::assert_type<float>(pair.first)) {
                jsonVal.SetFloat(mod::module_by_id<float>(pair.first));
            } 
            if (mod::assert_type<std::string>(pair.first)) {
                jsonVal.SetString(mod::module_by_id<std::string>(pair.first).c_str(), allocator);
            } 
            document.AddMember(jsonKey, jsonVal, allocator);
        }
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);

        auto str = buffer.GetString();
        create_directories("/storage/emulated/0/rbot/json");
        std::ofstream dst("/storage/emulated/0/rbot/json/mod.json");
        dst << str;
        dst.close();
    }

    inline void load()
    {

        std::string path = "/storage/emulated/0/rbot/json/mod.json";
        std::ifstream src(path);
        if (src.is_open())
        {
            std::stringstream buffer;
            buffer << src.rdbuf();
            auto json = buffer.str();

            rapidjson::Document document;
            document.Parse(json.c_str());

            if (document.HasParseError() || !document.IsObject()) return;

            for (auto& m : document.GetObject()) {
                auto id = ID_CAST(std::atoi(m.name.GetString()));
        
                if (mod::assert_type<bool>(id) && m.value.IsBool()) {
                    mod_map[id].value = m.value.GetBool();
                }
                if (mod::assert_type<int>(id) && m.value.IsInt()) {
                    mod_map[id].value = m.value.GetInt();
                }
                if (mod::assert_type<float>(id) && m.value.IsNumber()) {
                    mod_map[id].value = m.value.GetFloat();
                }
                if (mod::assert_type<std::string>(id) && m.value.IsString()) {
                    mod_map[id].value = m.value.GetString();
                }
            }
            src.close();
        }
    }
}