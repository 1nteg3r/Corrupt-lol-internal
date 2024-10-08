#include "renderer.h"

namespace Renderer
{
    XMFLOAT4X4* view_matrix;
    XMFLOAT4X4* proj_matrix;
    int* width;
    int* height;

    void Initialize(){
        view_matrix = (XMFLOAT4X4*)(offsets.oViewProjMatrices + 0x00);
        proj_matrix = (XMFLOAT4X4*)(offsets.oViewProjMatrices + 0x40);
        width = (int*)(offsets.oViewProjMatrices + 0x94);
        height = (int*)(offsets.oViewProjMatrices + 0x98);
    }
        
    XMINT2 WorldToScreen(XMFLOAT3 pos){
        FXMVECTOR V = XMVectorSetW(XMLoadFloat3(&pos), 1.f);
        FXMMATRIX M = XMLoadFloat4x4(view_matrix) * XMLoadFloat4x4(proj_matrix);
        FXMVECTOR coord = XMVector3TransformCoord(V, M);
        return {
          (int)((1 + XMVectorGetX(coord)) / 2 * *width),
          (int)((1 - XMVectorGetY(coord)) / 2 * *height)
        };


    }
}
