#ifndef __SHAPE_GENERATOR_H__
#define __SHAPE_GENERATOR_H__

#include <cmath>
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

typedef struct Cluster {
    std::vector<uint32_t> indices;
} Cluster;

typedef struct ClusterGroup {
    std::vector<Cluster> clusters;

} ClusterGroup;

typedef struct ClusterMesh_ {
    std::vector<ClusterGroup> clusterGroups;
    std::vector<glm::vec4> vertices;
    glm::vec3 center;

    float edge_length;

} ClusterMesh;

#define GENERATE_DDH_VERTICES(grd, grd_inv, center_x, center_y, center_z) { \
    /* Orange */ \
    { center_x +  1.0f, center_y +  1.0f, center_z +  1.0f, 1.0f }, /* 0 */ \
    { center_x + -1.0f, center_y +  1.0f, center_z +  1.0f, 1.0f }, /* 1 */ \
    { center_x +  1.0f, center_y + -1.0f, center_z +  1.0f, 1.0f }, /* 2 */ \
    { center_x + -1.0f, center_y + -1.0f, center_z +  1.0f, 1.0f }, /* 3 */ \
    { center_x +  1.0f, center_y +  1.0f, center_z + -1.0f, 1.0f }, /* 4 */ \
    { center_x + -1.0f, center_y +  1.0f, center_z + -1.0f, 1.0f }, /* 5 */ \
    { center_x +  1.0f, center_y + -1.0f, center_z + -1.0f, 1.0f }, /* 6 */ \
    { center_x + -1.0f, center_y + -1.0f, center_z + -1.0f, 1.0f }, /* 7 */ \
    /* Green */ \
    { center_x + 0.0f, center_y +  grd,    center_z + grd_inv, 1.0f }, /* 8 */ \
    { center_x + 0.0f, center_y + -grd,    center_z + grd_inv, 1.0f }, /* 9 */ \
    { center_x + 0.0f, center_y +  grd,    center_z + -grd_inv, 1.0f }, /* 10 */ \
    { center_x + 0.0f, center_y + -grd,    center_z + -grd_inv, 1.0f }, /* 11 */ \
    /* Blue */ \
    { center_x + grd_inv, center_y + 0.0f, center_z +  grd,    1.0f }, /* 12 */ \
    { center_x + -grd_inv, center_y + 0.0f, center_z +  grd,    1.0f }, /* 13 */ \
    { center_x + grd_inv, center_y + 0.0f, center_z + -grd,    1.0f }, /* 14 */ \
    { center_x + -grd_inv, center_y + 0.0f, center_z + -grd,    1.0f }, /* 15 */ \
    /* Pink */ \
    { center_x + grd,    center_y + grd_inv, center_z + 0.0f,  1.0f }, /* 16 */ \
    { center_x + -grd,    center_y + grd_inv, center_z + 0.0f,  1.0f }, /* 17 */ \
    { center_x + grd,    center_y + -grd_inv, center_z + 0.0f,  1.0f }, /* 18 */ \
    { center_x + -grd,    center_y + -grd_inv, center_z + 0.0f,  1.0f }  /* 19 */ \
}



const std::vector<uint32_t> ddh_faces_indices = {
        2, 9, 3, 13, 12,
        6 ,11, 9, 2, 18,
        11, 7, 19, 3, 9,
        6, 14, 15, 7, 11,
        4, 14, 6, 18, 16,
        16, 18, 2, 12, 0,
        0, 8, 10, 4, 16,
        12, 13, 1, 8, 0,
        3, 19, 17, 1, 13,
        17, 19, 7, 15, 5,
        10, 5, 15, 14, 4,
        8, 1, 17, 5, 10
};

const std::vector<uint32_t> cluster_indices =
        {
                27, 20, 25,
                27, 28, 20,
                25, 20, 21,
                20, 22, 21,
                20, 28, 22,
                28, 27, 31,
                27, 26, 31,
                27, 25, 26,
                28, 31, 29,
                28, 29, 22,
                25, 21, 24,
                25, 24, 26,
                30, 29, 23,
                30, 24, 23,
                23, 29, 22,
                23, 22, 21,
                23, 21, 22,
                24, 30, 26,
                26, 30, 31,
                26, 31, 27,
                21, 23, 24,
                31, 29, 30
        };

void generate_pentagon(  std::vector<float>& vertices,
                         std::vector<uint32_t>& indices,
                         float h_r,
                         std::pair<float, float> centre);

void generate_cluster(std::vector<glm::vec4>& ddh_vertices,
                      const std::vector<uint32_t >& ddh_faces,
                      std::vector<uint32_t> pentagon_centres,
                      float edge_length,
                      std::vector<uint32_t>& indices);

void generate_dodecahedra(ClusterMesh& mesh,
                          float edge_length,
                          glm::vec3 center);

void increaseLODForClusterGroup(ClusterMesh& mesh,
                                int groupIndex);

void decreaseLODForClusterGroup(std::vector<float>& vertices,
                                std::vector<uint32_t>& indices,
                                ClusterGroup& clusterGroup);

#endif // __SHAPE_GENERATOR_H__
