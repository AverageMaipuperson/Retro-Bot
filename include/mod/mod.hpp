#pragma once
#include <map>
#include <functional>
#include "any.hpp"

enum id
{
    noclip,
    restart_button,
    speedhack,
    playback_label,
    speedhack_val,
    update_on_steps,
    show_hitboxes
};

enum category
{
    menu,
    values
};

using ModCallback = std::function<void()>;

struct Module {
    ::id id;
    ::category category;
    std::string name;
    std::string description;
    bool isCheat;
    linb::any value;
    ModCallback callback;

    Module& operator=(linb::any other)
    {
        value = other;
        return *this;
    }
};

inline std::map<id, Module> mod_map;

namespace mod
{
    template <typename T>
    void module(::id id, ::category category, std::string name, std::string description, T defaultValue, bool isCheat = false, const ModCallback& callback = [](){})
    {
        Module m;
        m.id = id;
        m.category = category;
        m.name = name;
        m.description = description;
        m.isCheat = isCheat;
        m.value = defaultValue;
        m.callback = std::move(callback);

        mod_map[id] = m;
    }

    template <typename T>
    T module_by_id(::id id)
    {
        return linb::any_cast<T>(mod_map[id].value);
    }

    inline const std::type_info& get_type(::id id)
    {
        return mod_map[id].value.type();
    }

    template <typename T>
    inline bool assert_type(::id id)
    {
        return (get_type(id) == typeid(T));
    }

    inline void set_target(::id id, const ModCallback& callback)
    {
        mod_map[id].callback = callback;
    }
}

#define ID_CAST(o) static_cast<::id>(o)