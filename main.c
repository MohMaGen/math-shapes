#include "raylib.h"
#include "stdlib.h"
#include "raymath.h"
#include "stdio.h"

#define BG ((Color) { 0x27, 0x2e, 0x33, 0xFF })
#define FG ((Color) { 0xd3, 0xc6, 0xaa, 0xFF })


char* arr3at(char *arr, const size_t a, const size_t b, size_t x, size_t y, size_t z) {
    return arr + z * a * b + y * a + x;
}
#define voxelsat(x, y, z) (arr3at(voxels, a, b, x, y, z))


typedef struct { float value; char  inc_by_time; char *name; Rectangle rect; } parameter_t;


int inside_rect(Rectangle rect, Vector2 pos) {
    return pos.x >= rect.x && pos.x <= rect.x + rect.width &&
            pos.y >= rect.y && pos.y <= rect.y + rect.height;
}


void update_parameter(parameter_t *param) {
    Vector2 mp = GetMousePosition();

    if (inside_rect(param->rect, mp)) {
        param->value += GetMouseWheelMove() * 0.25;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            param->inc_by_time = !param->inc_by_time;
    }
}

void draw_parameter(parameter_t param) {
    DrawRectangleRec(param.rect, FG);
    char buf[128] = {0};
    sprintf(buf, "%s: %.2f%s", param.name, param.value, param.inc_by_time ? " + t" : "");
    int width = MeasureText(buf, param.rect.height - 10);
    DrawText(buf,
             param.rect.x + (param.rect.width - width)/2,
             param.rect.y + 5,
             param.rect.height - 10,
             BG);
}

#define PARAM_WIDTH  120.0f
#define PARAM_HEIGHT 30.0f
parameter_t make_parameter(char *name) {
    return (parameter_t) { 1., 0, name, (Rectangle) { 0, 0, PARAM_WIDTH, PARAM_HEIGHT }, };
}

float get_parameter_value(parameter_t param, float time) {
    return param.value * (param.inc_by_time ? sin(time * 0.5) : 1.);
}


int main(int argc, char **argv) {
    InitWindow(800, 600, "Math_Shapes!!");


    const size_t a = 80; 
    const size_t b = 80;
    const size_t c = 80;
    char *voxels = calloc(a * b * c, sizeof(char));


    Vector3 zone_lbf = (Vector3) { -1, -1, -1 },
            zone_run = (Vector3) {  1,  1,  1 };


    Camera3D camera   = { 0 };
    camera.position   = (Vector3) { 2, 2, 2 };
    camera.up         = (Vector3) { 0, 0, 1 };
    camera.target     = (Vector3) { 0, 0, 0 };
    camera.fovy       = 6.;
    camera.projection = CAMERA_ORTHOGRAPHIC;


    Model cube = LoadModelFromMesh(GenMeshCube(
                    (zone_run.x - zone_lbf.x ) / (float)a,
                    (zone_run.y - zone_lbf.y ) / (float)b,
                    (zone_run.z - zone_lbf.z ) / (float)c));



    Shader shader = LoadShader("./shader.vs", "./shader.fs");

    cube.materials[0].shader = shader;

    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int fog_color                       = GetShaderLocation(shader, "fogColor");
	float bg_rgba[4] = {
            (float)BG.r / 255.0f,
            (float)BG.g / 255.0f,
            (float)BG.b / 255.0f,
            (float)BG.a / 255.0f 
        };

    SetShaderValue(shader, fog_color, &bg_rgba, SHADER_UNIFORM_VEC4);

    double time = 0;

    parameter_t p_A = make_parameter("A");
    parameter_t p_B = make_parameter("B");
    parameter_t p_C = make_parameter("C");
    parameter_t p_R = make_parameter("R");
    parameter_t p_x_0 = make_parameter("x_0");
    parameter_t p_y_0 = make_parameter("y_0");
    parameter_t p_z_0 = make_parameter("z_0");

    p_A.rect.x = 10.;
    p_B.rect.x = 10. + PARAM_WIDTH + 10.;
    p_C.rect.x = 10. + PARAM_WIDTH * 2 + 10. * 2;
    p_R.rect.x = 10. + PARAM_WIDTH * 3 + 10. * 3;

    p_A.rect.y = 10.;
    p_B.rect.y = 10.;
    p_C.rect.y = 10.;
    p_R.rect.y = 10.;

    p_x_0.rect.x = 10.;
    p_y_0.rect.x = 10. + PARAM_WIDTH + 10.;
    p_z_0.rect.x = 10. + PARAM_WIDTH * 2 + 10. * 2;

    p_x_0.rect.y = 10. + PARAM_HEIGHT + 10.;
    p_y_0.rect.y = 10. + PARAM_HEIGHT + 10.;
    p_z_0.rect.y = 10. + PARAM_HEIGHT + 10.;

    p_x_0.value= 0.;
    p_y_0.value= 0.;
    p_z_0.value= 0.;


    SetTargetFPS(30);
    while (!WindowShouldClose()) {
        time += GetFrameTime();

        update_parameter(&p_A);
        update_parameter(&p_B);
        update_parameter(&p_C);
        update_parameter(&p_R);
        update_parameter(&p_x_0);
        update_parameter(&p_y_0);
        update_parameter(&p_z_0);

        for (size_t z = 0; z < c; z++)
        for (size_t y = 0; y < b; y++)
        for (size_t x = 0; x < a; x++) {
            float _x = zone_lbf.x + (zone_run.x - zone_lbf.x ) / (float)a * (float)x;
            float _y = zone_lbf.y + (zone_run.y - zone_lbf.y ) / (float)b * (float)y;
            float _z = zone_lbf.z + (zone_run.z - zone_lbf.z ) / (float)c * (float)z;

            float A   = get_parameter_value(p_A, time);
            float B   = get_parameter_value(p_B, time);
            float C   = get_parameter_value(p_C, time);
            float R   = get_parameter_value(p_R, time);
            float x_0 = get_parameter_value(p_x_0, time);
            float y_0 = get_parameter_value(p_y_0, time);
            float z_0 = get_parameter_value(p_z_0, time);

            float F_xyz = 
                (_x - x_0) * (_x - x_0) * A +
                (_y - y_0) * (_y - y_0) * B +
                (_z - z_0) * (_z - z_0) * C - R * R;

            *voxelsat(x, y, z) = (fabsf(F_xyz) < 0.04) ? 1 : 0;
        }


        int width  = GetScreenWidth();
        int height = GetScreenHeight();

        if (IsKeyPressed(KEY_FIVE)) {
            camera.position = (Vector3) { 2, 2, 2 };
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 dm = GetMouseDelta();
            camera.position = Vector3RotateByAxisAngle(camera.position,
                                   (Vector3){ 0, 0, 1 },
                                   dm.x * -0.004);

            camera.position = Vector3RotateByAxisAngle(camera.position,
                                       Vector3Normalize(Vector3CrossProduct(
                                           camera.position, (Vector3){ 0, 0, 1 })),
                                       dm.y * 0.004);
        }

        SetShaderValue(shader,
                       shader.locs[SHADER_LOC_VECTOR_VIEW],
                       &camera.position.x, SHADER_UNIFORM_VEC3);

        BeginDrawing();
        ClearBackground(BG);
        BeginMode3D(camera);
            BeginShaderMode(shader);
            for (size_t z = 0; z < c; z++)
            for (size_t y = 0; y < b; y++)
            for (size_t x = 0; x < a; x++) {
                if (*voxelsat(x, y, z) == 0) continue;

                Vector3 size     = (Vector3) {
                    (zone_run.x - zone_lbf.x ) / (float)a,
                    (zone_run.y - zone_lbf.y ) / (float)b,
                    (zone_run.z - zone_lbf.z ) / (float)c,
                };
                Vector3 position = (Vector3) {
                    zone_lbf.x + size.x * (float)x,
                    zone_lbf.y + size.y * (float)y,
                    zone_lbf.z + size.z * (float)z
                };
                DrawModel(cube, position, 1., FG);
            }
            EndShaderMode();

            float c_a = 1.25;
            DrawLine3D((Vector3){ -c_a, -c_a, -c_a }, (Vector3){ -c_a, -c_a,  c_a }, FG);
            DrawLine3D((Vector3){ -c_a, -c_a, -c_a }, (Vector3){ -c_a,  c_a, -c_a }, FG);
            DrawLine3D((Vector3){ -c_a, -c_a, -c_a }, (Vector3){  c_a, -c_a, -c_a }, FG);

            DrawLine3D((Vector3){  c_a,  c_a,  c_a }, (Vector3){ -c_a,  c_a,  c_a }, FG); 
            DrawLine3D((Vector3){  c_a,  c_a,  c_a }, (Vector3){  c_a, -c_a,  c_a }, FG); 
            DrawLine3D((Vector3){  c_a,  c_a,  c_a }, (Vector3){  c_a,  c_a, -c_a }, FG); 

            DrawLine3D((Vector3){  c_a,  -c_a,  c_a }, (Vector3){  -c_a,  -c_a,  c_a }, FG); 
            DrawLine3D((Vector3){ -c_a,   c_a,  c_a }, (Vector3){  -c_a,  -c_a,  c_a }, FG); 

            DrawLine3D((Vector3){  c_a,   c_a, -c_a }, (Vector3){  -c_a,   c_a, -c_a }, FG); 
            DrawLine3D((Vector3){ -c_a,   c_a,  c_a }, (Vector3){  -c_a,   c_a, -c_a }, FG); 

            DrawLine3D((Vector3){  c_a,  -c_a,  c_a }, (Vector3){   c_a,  -c_a, -c_a }, FG); 
            DrawLine3D((Vector3){  c_a,   c_a, -c_a }, (Vector3){   c_a,  -c_a, -c_a }, FG); 

        EndMode3D();

        draw_parameter(p_A);
        draw_parameter(p_B);
        draw_parameter(p_C);
        draw_parameter(p_R);
        draw_parameter(p_x_0);
        draw_parameter(p_y_0);
        draw_parameter(p_z_0);

        EndDrawing();
    }
    free(voxels);

    UnloadShader(shader);
    UnloadModel(cube);
    CloseWindow();
    return 0;
}