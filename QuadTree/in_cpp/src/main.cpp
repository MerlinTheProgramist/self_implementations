#include <Camera2D.hpp>
#include <cassert>
#include "player.hpp"
#include "raylib-cpp.hpp"
#include "quadTree.h"
#include "blob.h"
#include <Functions.hpp>
#include <cmath>
#include <filesystem>
#include <math.h>
#include <memory>
#include <random>
#include <raylib.h>
#include <unordered_set>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
int const screenWidth = 800;
int const screenHeight = 450;
Vec2 const screenVec{screenWidth, screenHeight};
Vec2 const halfScreenVec{screenVec/2.0};

raylib::Camera2D camera{halfScreenVec, {0,0}};

std::vector<Blob> objects{};
std::unique_ptr<std::unordered_set<size_t>> dynamic{
    std::make_unique<std::unordered_set<size_t>>()
};

QuadTree<size_t, 5, screenWidth, screenHeight> world{};

Player player{};

typedef std::mt19937 rng_type;
rng_type rng;


//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void);     // Update and Draw one frame

//----------------------------------------------------------------------------------
// Main Enry Point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    raylib::Window window(screenWidth, screenHeight, "raylib-cpp [core] example - basic window");

    
    std::uniform_int_distribution<rng_type::result_type> xdist(0, screenWidth);
    std::uniform_int_distribution<rng_type::result_type> ydist(0, screenHeight);

    for(int i=0;i<100;++i){
        Point const pos{static_cast<Num>(xdist(rng)), static_cast<Num>(ydist(rng))};
        objects.emplace_back(pos);
        world.insert(pos, objects.size()-1);
    }

    objects.emplace_back(Point{screenWidth/2.,screenHeight/2.}, 20, PURPLE);
    world.insert(Point{0,0}, objects.size()-1);
    player = Player(objects.size()-1);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    

    // Main game loop
    while (!window.ShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    return 0;
}


//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
// Point dragStart{0,0};
// bool dragging{false};
void UpdateDrawFrame(void){
    const float dt = GetFrameTime();

    // Input
    // if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
    //     dragging = true;
    //     dragStart = (Point)GetMousePosition();
    // }
    // if(!IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
    //     dragging = false;
    // }

    
    Point dir{(Point)GetMousePosition() - halfScreenVec};
    dir.normalize();
    {
        const Point vel{dir*10};
        Point playerCenter{0,0};
        Num biases{0.001};
        for(auto id : player.blobs_ids){
            dynamic->insert(id);
            objects[id].velocity = vel;
            playerCenter += objects[id].pos * static_cast<Num>(objects[id].radius);
            biases += objects[id].radius;
        }
        player.center = playerCenter / biases;
    }
    camera.target = player.center;
    // std::cout << "Player pos: "<< player.center << std::endl;


    // Shut mini blob
    if(IsKeyPressed(KEY_E)){
        // from each player's blob
        for(auto id: player.blobs_ids){
            objects.push_back(objects[id].shut_mini_out(dir));
            size_t mini_id = objects.size()-1;
            dynamic->insert(mini_id);
            world.insert(objects.back().pos, mini_id);
        }
    }

    // Split
    if(IsKeyPressed(KEY_Z)){
        for(auto id: player.blobs_ids){
            objects.push_back(objects[id].split(dir));
            size_t other_id = objects.size()-1;
            dynamic->insert(other_id);
            world.insert(objects.back().pos, other_id);
            player.blobs_ids.push_back(other_id);
        }
    }


    // for(auto&& player : players){
        std::vector<size_t> in_view;
        world.query_rect(AABB((Vec2)camera.target-halfScreenVec, (Vec2)camera.target+halfScreenVec), in_view);
        for(size_t id : player.blobs_ids){
            for(size_t close : in_view){
                if(id==close)
                    continue;
                if(objects[id].test_against(objects[close])){
                    if(world.remove(objects[close].pos))
                        objects[id].merge_size(objects[close]);
                }
            }           
        }
    // }
    
    
    {
        std::unique_ptr<std::unordered_set<size_t>> new_dynamic{
            std::make_unique<std::unordered_set<size_t>>()
        };
        new_dynamic->reserve(dynamic->size());
        for(auto id : *dynamic){
            world.remove(objects[id].pos);
            if(objects[id].update(dt))
                new_dynamic->insert(id);
            world.insert(objects[id].pos, id);
        }
        dynamic.swap(new_dynamic);
    }
    
    // Draw
    BeginDrawing();
        ClearBackground(RAYWHITE);

        camera.BeginMode();
        {
            for(auto&& blob_id : in_view){
                const Blob& blob = objects[blob_id];
                DrawCircleV(blob.pos, blob.radius, blob.color);
            }

        }        
        camera.EndMode();
        // if(dragging){
        //     Point drag_p1 = (Point)GetMousePosition();
        //     Point drag_p2 = dragStart;
        //     if(drag_p1.x > drag_p2.x)
        //         std::swap(drag_p2.x, drag_p1.x);
        //     if(drag_p1.y > drag_p2.y)
        //         std::swap(drag_p2.y, drag_p1.y);

        //     // std::cout << "pos1 :"<< drag_p1 << ", pos2: "<< drag_p2 << std::endl;
        //     const AABB window{drag_p1, drag_p2};
        //     // std::cout <<"box: [" << window.x_0<< ',' << window.y_0 << "][" << window.x_1 << ',' << window.y_1 << "]" << std::endl;
            
        //     std::vector<size_t> windowed_objects;
        //     world.query_rect(window, windowed_objects);

        //     for(size_t blob_id : windowed_objects){
        //         DrawCircleV(objects[blob_id].pos, 3, GREEN);
        //     }
        //     DrawRectangleLines(drag_p1.x, drag_p1.y, drag_p2.x-drag_p1.x, drag_p2.y-drag_p1.y, PURPLE);
        // }

        DrawFPS(10, 10);
        // raylib::DrawText("Congrats! You created your first raylib-cpp window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
