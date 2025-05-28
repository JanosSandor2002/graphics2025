#ifndef MODEL_H
#define MODEL_H

typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    float u, v;
} TexCoord;

typedef struct {
    float x, y, z;
} Normal;

typedef struct {
    unsigned int v1, v2, v3;
    unsigned int vt1, vt2, vt3;
    unsigned int vn1, vn2, vn3;
} Face;

typedef struct {
    Vertex* vertices;
    TexCoord* texcoords;
    Normal* normals;
    Face* faces;

    unsigned int vertex_count;
    unsigned int texcoord_count;
    unsigned int normal_count;
    unsigned int face_count;
} Model;

void draw_model(const Model* model);
Model* load_model(const char* filename);
void free_model(Model* model);
float get_model_min_y(const Model* model);

#endif