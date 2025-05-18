#include "Model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>  // OpenGL fejlécek (győződj meg róla, hogy elérhető)

Model* load_model(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open OBJ file");
        return NULL;
    }

    Model* model = calloc(1, sizeof(Model));

    size_t vertex_cap = 1024;
    size_t texcoord_cap = 1024;
    size_t normal_cap = 1024;
    size_t face_cap = 1024;

    model->vertices = malloc(vertex_cap * sizeof(Vertex));
    model->texcoords = malloc(texcoord_cap * sizeof(TexCoord));
    model->normals = malloc(normal_cap * sizeof(Normal));
    model->faces = malloc(face_cap * sizeof(Face));

    char line[256];

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            if (model->vertex_count >= vertex_cap) {
                vertex_cap *= 2;
                model->vertices = realloc(model->vertices, vertex_cap * sizeof(Vertex));
            }
            sscanf(line + 2, "%f %f %f",
                &model->vertices[model->vertex_count].x,
                &model->vertices[model->vertex_count].y,
                &model->vertices[model->vertex_count].z);
            model->vertex_count++;
        } else if (strncmp(line, "vt ", 3) == 0) {
            if (model->texcoord_count >= texcoord_cap) {
                texcoord_cap *= 2;
                model->texcoords = realloc(model->texcoords, texcoord_cap * sizeof(TexCoord));
            }
            sscanf(line + 3, "%f %f",
                &model->texcoords[model->texcoord_count].u,
                &model->texcoords[model->texcoord_count].v);
            model->texcoord_count++;
        } else if (strncmp(line, "vn ", 3) == 0) {
            if (model->normal_count >= normal_cap) {
                normal_cap *= 2;
                model->normals = realloc(model->normals, normal_cap * sizeof(Normal));
            }
            sscanf(line + 3, "%f %f %f",
                &model->normals[model->normal_count].x,
                &model->normals[model->normal_count].y,
                &model->normals[model->normal_count].z);
            model->normal_count++;
        } else if (strncmp(line, "f ", 2) == 0) {
            if (model->face_count >= face_cap) {
                face_cap *= 2;
                model->faces = realloc(model->faces, face_cap * sizeof(Face));
            }

            Face* f = &model->faces[model->face_count];
            sscanf(line + 2, "%u/%u/%u %u/%u/%u %u/%u/%u",
                &f->v1, &f->vt1, &f->vn1,
                &f->v2, &f->vt2, &f->vn2,
                &f->v3, &f->vt3, &f->vn3);
            // Convert to zero-based indexing
            f->v1--; f->vt1--; f->vn1--;
            f->v2--; f->vt2--; f->vn2--;
            f->v3--; f->vt3--; f->vn3--;

            model->face_count++;
        }
    }

    fclose(file);
    return model;
}

void draw_model(const Model* model) {
    if (!model) return;

    glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i < model->face_count; i++) {
        Face* f = &model->faces[i];

        if (model->normal_count > 0) {
            glNormal3f(
                model->normals[f->vn1].x,
                model->normals[f->vn1].y,
                model->normals[f->vn1].z);
        }
        if (model->texcoord_count > 0) {
            glTexCoord2f(
                model->texcoords[f->vt1].u,
                model->texcoords[f->vt1].v);
        }
        glVertex3f(
            model->vertices[f->v1].x,
            model->vertices[f->v1].y,
            model->vertices[f->v1].z);

        if (model->normal_count > 0) {
            glNormal3f(
                model->normals[f->vn2].x,
                model->normals[f->vn2].y,
                model->normals[f->vn2].z);
        }
        if (model->texcoord_count > 0) {
            glTexCoord2f(
                model->texcoords[f->vt2].u,
                model->texcoords[f->vt2].v);
        }
        glVertex3f(
            model->vertices[f->v2].x,
            model->vertices[f->v2].y,
            model->vertices[f->v2].z);

        if (model->normal_count > 0) {
            glNormal3f(
                model->normals[f->vn3].x,
                model->normals[f->vn3].y,
                model->normals[f->vn3].z);
        }
        if (model->texcoord_count > 0) {
            glTexCoord2f(
                model->texcoords[f->vt3].u,
                model->texcoords[f->vt3].v);
        }
        glVertex3f(
            model->vertices[f->v3].x,
            model->vertices[f->v3].y,
            model->vertices[f->v3].z);
    }
    glEnd();
}

void free_model(Model* model) {
    if (!model) return;

    free(model->vertices);
    free(model->texcoords);
    free(model->normals);
    free(model->faces);
    free(model);
}
float get_model_min_y(const Model* model) {
    if (!model || model->vertex_count == 0) return 0.0f;

    float minY = model->vertices[0].y;
    for (unsigned int i = 1; i < model->vertex_count; i++) {
        if (model->vertices[i].y < minY) {
            minY = model->vertices[i].y;
        }
    }
    return minY;
}

