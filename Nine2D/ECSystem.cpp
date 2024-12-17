#include"pch.h"
#include "base/Vec2.h"
#include "renderer\Dx2DRenderable.h"

#define PICO_ECS_IMPLEMENTATION
#include "pico\pico_ecs.h"

ecs_t* ecs1 = NULL;

IRenderer* g_ECS_Renderer = nullptr;

// component IDs
ecs_id_t PositionCompID;
ecs_id_t VelocityCompID;
ecs_id_t TextureCompID;
ecs_id_t HouseCompID;
ecs_id_t UnitCompID;

// System IDs
ecs_id_t MovementSysID;
ecs_id_t RenderSysID1;
ecs_id_t RenderSysID2;

const WCHAR* g_Tex_Name[] = {
    L"Image\\Carrot.png",
    L"Image\\TownHall.png",
};

void register_components()
{
    PositionCompID  = ecs_register_component(ecs1, sizeof(Pos_t),  NULL, NULL);
    VelocityCompID  = ecs_register_component(ecs1, sizeof(Velocity_t),  NULL, NULL);
    TextureCompID  = ecs_register_component(ecs1, sizeof(Img_t),  NULL, NULL);
    HouseCompID  = ecs_register_component(ecs1, sizeof(char),  NULL, NULL);
    UnitCompID  = ecs_register_component(ecs1, sizeof(char),  NULL, NULL);

}


ecs_ret_t Movement_System(ecs_t* ecs,
                          ecs_id_t* entities,
                          int entity_count,
                          ecs_dt_t dtt,
                          void* udata)
{
    (void)udata;
    printf("Movement_System = %d \n", entity_count);
    float dt = dtt;

    for (int i = 0; i < entity_count; i++)
    {
        // Get entity ID
        ecs_id_t id = entities[i];

        Pos_t* pos = (Pos_t*)ecs_get(ecs, id, PositionCompID);
        Vec2* dir = (Vec2*)ecs_get(ecs, id, VelocityCompID);

        pos->x += dir->x * dt;
        pos->y += dir->y * dt;

        float right =  (float)(g_Dx11.width-50);
        float bottom =  (float)(g_Dx11.height-50);
        if (pos->x > right ||
            pos->x < 100.f ||
            pos->y > bottom ||
            pos->y < 100.f ) 
        { 
            int x = rand() % g_Dx11.width;
            int y = rand() % g_Dx11.height;
    
            dir->x = ((float)x) - pos->x;
            dir->y = ((float)y) - pos->y;
            dir->normalize();
            dir->x *= 100.f;
            dir->y *= 100.f;
        }

    }

    return 0;
}


ecs_ret_t Render_System(ecs_t* ecs,
                          ecs_id_t* entities,
                          int entity_count,
                          ecs_dt_t dt,
                          void* udata)
{
    Dx2DRenderable2 rd;
    rd.color = {1.f, 1.f, 1.f, 1.f};
    rd.ancherX = 0.5f;
    rd.ancherY = 0.5f;
    rd.h = 100;
    rd.w = 100;
    rd.texName = g_Tex_Name[0];
    rd.tex = nullptr;
    rd.vb = nullptr;

    (void)udata;
    printf("Render_System = %d \n", entity_count);

    for (int i = 0; i < entity_count; i++)
    {
        // Get entity ID
        ecs_id_t id = entities[i];

        Pos_t* pos = (Pos_t*)ecs_get(ecs, id, PositionCompID);
        Img_t* img = (Img_t*)ecs_get(ecs, id, TextureCompID);

        rd.position.x = pos->x;
        rd.position.y = pos->y;
        rd.angle = pos->angle; 
        rd.w = img->w;
        rd.h = img->h;
        rd.ancherX = img->ancherX; 
        rd.ancherY = img->ancherY; 
        rd.texName = img->texName;
        rd.tex = (ID3D11ShaderResourceView*)img->tex;
        
        g_ECS_Renderer->Draw2(&rd);

        img->tex = rd.tex;
    }

    return 0;
}




void register_systems()
{
    MovementSysID = ecs_register_system(ecs1, Movement_System, NULL, NULL, NULL);
    ecs_require_component(ecs1, MovementSysID, PositionCompID);
    ecs_require_component(ecs1, MovementSysID, VelocityCompID);

    RenderSysID1 = ecs_register_system(ecs1, Render_System, NULL, NULL, NULL);
    ecs_require_component(ecs1, RenderSysID1, PositionCompID);
    ecs_require_component(ecs1, RenderSysID1, TextureCompID);
    ecs_require_component(ecs1, RenderSysID1, HouseCompID);

    RenderSysID2 = ecs_register_system(ecs1, Render_System, NULL, NULL, NULL);
    ecs_require_component(ecs1, RenderSysID2, PositionCompID);
    ecs_require_component(ecs1, RenderSysID2, TextureCompID);
    ecs_require_component(ecs1, RenderSysID2, UnitCompID);
}

void Render_system_all(float dt)
{
    ecs_update_system(ecs1, RenderSysID1, dt);
    ecs_update_system(ecs1, RenderSysID2, dt);


}

void Update_system_all(float dt)
{
    double dtt = dt;
    ecs_update_system(ecs1, MovementSysID, dtt);


}


void create_entity1()
{
    // Create entity
    ecs_id_t id = ecs_create(ecs1);

    // Add components
    Pos_t* pos = (Pos_t*)ecs_add(ecs1, id, PositionCompID, NULL);
    Vec2*  dir = (Vec2*)ecs_add(ecs1, id, VelocityCompID, NULL);
    Img_t*  img = (Img_t*)ecs_add(ecs1, id, TextureCompID, NULL);
    char*  type = (char*)ecs_add(ecs1, id, UnitCompID, NULL);

    *type  = 2;
    *pos    = { 600, 400, 0.f };

    int x = rand() % g_Dx11.width;
    int y = rand() % g_Dx11.height;

    dir->x = ((float)x) - pos->x;
    dir->y = ((float)y) - pos->y;
    dir->normalize();
    *dir *= 100.f;

    img->ancherX = 0.5f;
    img->ancherY = 0.5f;
    img->w = 100;
    img->h = 100;
    img->texName = g_Tex_Name[0];
    img->tex = nullptr;

}

void create_entity2()
{
    // Create entity
    ecs_id_t id = ecs_create(ecs1);

    // Add components
    Pos_t* pos = (Pos_t*)ecs_add(ecs1, id, PositionCompID, NULL);
    Img_t*  img = (Img_t*)ecs_add(ecs1, id, TextureCompID, NULL);
    char*  type = (char*)ecs_add(ecs1, id, HouseCompID, NULL);

    *type = 1;

    pos->x = (float)( rand() % g_Dx11.width );
    pos->y = (float)( rand() % g_Dx11.height );
    pos->angle = 0.f;

    img->ancherX = 0.5f;
    img->ancherY = 0.5f;
    img->w = 100;
    img->h = 100;
    img->texName = g_Tex_Name[1];
    img->tex = nullptr;

}


