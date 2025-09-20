#include "ComponentInspector.h"

#include <iostream>

namespace SAPPHIRE
{
    std::unordered_map<size_t, std::pair<std::string, ComponentInspector::DrawFunc>>& ComponentInspector::Registry()
    {
        static std::unordered_map<size_t, std::pair<std::string, DrawFunc>> s_Registry;
        return s_Registry;
    }

    void ComponentInspector::Register(size_t typeID, const char* typeName, DrawFunc draw)
    {
        Registry()[typeID] = { typeName, std::move(draw) };
    }

    void ComponentInspector::DrawComponent(EMERALD::ComponentBase* comp)
    {
        auto it = Registry().find(comp->GetTypeID());
        if (it != Registry().end())
        {
            const auto& [typeName, drawFunc] = it->second;

            if (ImGui::CollapsingHeader(typeName.c_str()))
            {
                drawFunc(comp); // Pass component to its registered drawer
            }
        }
        else
        {
            // Fallback if no drawer is registered
            ImGui::Text("%s (no inspector available)", comp->GetTypeName());
        }
    }
}