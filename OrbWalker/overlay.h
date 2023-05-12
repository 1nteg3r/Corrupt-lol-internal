#pragma once
#include "../imgui/imgui.h"
#include "Vector2.h"
#include "Vector3.h"
#include "renderer.h"
#include "pch.h"

class Overlay {
private:
	ImDrawList* drawList;
public:

	void RenderScene();

    void Circle(const XMFLOAT3& pos, const float& radius, const ImColor& color = ImColor(1.f, 1.f, 1.f, 0.5f), const float& thickness = 2.f, int circlePoints = 80)
    {
        static ImVec2 points[200];
        static ImVec2 innerPoints[200];
        XMFLOAT3 worldSpace;
        XMINT2 screenSpace;

        float step = 6.2831f / circlePoints;
        float theta = 0.f;

        for (int i = 0; i < circlePoints; i++, theta += step) {
            worldSpace = { pos.x + radius * cos(theta), pos.y, pos.z - radius * sin(theta) };
            screenSpace = Renderer::WorldToScreen(worldSpace);

            points[i].x = screenSpace.x;
            points[i].y = screenSpace.y;
        }

        drawList->AddPolyline(points, circlePoints, color, true, thickness);
    }
};
inline Overlay overlay;