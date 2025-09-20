#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <cstddef> // for size_t
#include <imgui.h>

#include <Core/Component.h>

namespace SAPPHIRE
{
    class ComponentInspector
    {
    public:
        using DrawFunc = std::function<void(EMERALD::ComponentBase*)>;

        // Register a draw function for a type ID
        static void Register(size_t typeID, const char* typeName, DrawFunc draw);

        // Draw registered component
        static void DrawComponent(EMERALD::ComponentBase* comp);

    private:
        static std::unordered_map<size_t, std::pair<std::string, DrawFunc>>& Registry();
    };
}
