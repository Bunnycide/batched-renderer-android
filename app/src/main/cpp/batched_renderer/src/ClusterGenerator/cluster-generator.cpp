#include "ClusterGenerator/cluster-generator.h"
#include "../../../AndroidOut.h"

#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void generate_dodecahedra(ClusterMesh& mesh,
                          float edge_length,
                          glm::vec3 center){

    mesh.center = center;
    mesh.edge_length = edge_length;

    // calculate apothem of the pentagon
    const float grd = 1.618f;
    const float grd_inv = 1.0f/grd;

    // radius of the circumscribed sphere **  that touches all vertices **
    const float r_u = 1.401f * edge_length;
    // radius of the circumscribed sphere ** tangent to each of the dodecahedron faces **
    const float r_i = 1.114f * edge_length;

    mesh.vertices = GENERATE_DDH_VERTICES(grd, grd_inv, center.x, center.y, center.z);

    //  calculate centroids
    std::vector<glm::vec4> centroids(ddh_faces_indices.size() / 5);
    auto k = 0;
    for(auto i = 0, iter = 0; i < ddh_faces_indices.size(); i+=5, iter+=4){
        k = i == 0 ? 0 : i / 5;

        centroids[k] = (mesh.vertices[ddh_faces_indices[i]] +
                       mesh.vertices[ddh_faces_indices[i + 1]] +
                       mesh.vertices[ddh_faces_indices[i + 2]] +
                       mesh.vertices[ddh_faces_indices[i + 3]] +
                       mesh.vertices[ddh_faces_indices[i + 4]]) / 5.0f;

        // Calculate normal
        glm::vec3 P1  = centroids[k] - mesh.vertices[ddh_faces_indices[i]];

        glm::vec3 P2 =  centroids[k] - mesh.vertices[ddh_faces_indices[i + 1]];

        auto normal = glm::cross(P1, P2);
        normal = glm::normalize(normal);

        // Check orientation
        glm::vec3 from_centroid_to_ddh_centre  = centroids[k];

        if(glm::dot(normal, from_centroid_to_ddh_centre) >= 0) {
            std::cout << "Normal is inwards " << i << std::endl;
        }

        // displace the centroid
        centroids[k].x = centroids[k].x - (r_u - r_i) * normal.x;
        centroids[k].y = centroids[k].y - (r_u - r_i) * normal.y;
        centroids[k].z = centroids[k].z - (r_u - r_i) * normal.z;
    }

    // Add centroids to vertices
    // 20 vertices 12 centroids
    mesh.vertices.insert(mesh.vertices.end(), centroids.begin(), centroids.end());

    std::vector<float> root_cluster_vertices;
    std::vector<uint32_t> root_cluster_indices;
    std::vector<float> t_vertices;
    std::vector<uint32_t> t_indices;

    mesh.clusterGroups.resize(cluster_indices.size() / 3);

    // Initially each cluster groups has only on cluster
    for(auto i = 0; i < cluster_indices.size()/3; i++){
        mesh.clusterGroups[i].clusters.resize(1);
    }

    for(int i = 0; i < cluster_indices.size()/3; i++){
        generate_cluster(mesh.vertices,
                         ddh_faces_indices,
                         {cluster_indices[i * 3], cluster_indices[i * 3 + 1 ], cluster_indices[i * 3 + 2]},
                         edge_length,
                         root_cluster_indices);

        t_indices.insert(t_indices.end(), root_cluster_indices.begin(), root_cluster_indices.end());
        t_vertices.insert(t_vertices.end(), root_cluster_vertices.begin(), root_cluster_vertices.end());

        mesh.clusterGroups[i].clusters[0].indices.insert(
                mesh.clusterGroups[i].clusters[0].indices.end(),
                root_cluster_indices.begin(),
                root_cluster_indices.end());
    }
}

void generate_cluster(std::vector<glm::vec4>& ddh_vertices,
                      const std::vector<uint32_t >& ddh_faces,
                      std::vector<uint32_t> pentagon_centres,
                      float edge_length,
                      std::vector<uint32_t>& indices) {
    std::vector<int> order = {0, 1, 1, 2, 2, 0};
    uint32_t pentagon_face_index = 0, k = 0;
    std::vector<std::pair<uint32_t, uint32_t>> common_edges(3);
    std::vector<std::vector<uint32_t>> faces(3);

    // radius of the circumscribed sphere **  that touches all vertices **
    const float r_u = 1.401f * edge_length;
    // radius of the circumscribed sphere ** tangent to each of the dodecahedron faces **
    const float r_i = 1.114f * edge_length;

    // Get offset into faces vector
    for (auto centre: pentagon_centres) {
        pentagon_face_index = centre - 20;
        faces[k] = {
                ddh_faces[pentagon_face_index * 5],
                ddh_faces[pentagon_face_index * 5 + 1],
                ddh_faces[pentagon_face_index * 5 + 2],
                ddh_faces[pentagon_face_index * 5 + 3],
                ddh_faces[pentagon_face_index * 5 + 4],
        };

        k++;
    }

    // Find common edges
    k = 0;
    uint32_t common_vertex = 0;
    for (auto i = 0; i < order.size(); i += 2) {
        std::sort(faces[order[i]].begin(), faces[order[i]].end());
        std::sort(faces[order[i + 1]].begin(), faces[order[i + 1]].end());
        std::vector<uint32_t> _;

        std::set_intersection(faces[order[i]].begin(), faces[order[i]].end(),
                              faces[order[i + 1]].begin(), faces[order[i + 1]].end(),
                              std::back_inserter(_));

        common_edges[k].first = _[0];
        common_edges[k].second = _[1];

        if (k != 0) {
            for (auto vertex_: _) {
                if (vertex_ == common_edges[k - 1].first || vertex_ == common_edges[k - 1].second) {
                    common_vertex = vertex_;
                }
            }
        }

        k++;
    }

    k = 0;
    glm::vec3 edge_midpoint{};
    std::vector<glm::vec3> midpoints;
    // Find intersection of line extending from common edges to the pentagon centre
    for (auto common_edge: common_edges) {
        // Find common edge midpoint
        edge_midpoint = ( ddh_vertices[common_edge.first] + ddh_vertices[common_edge.second] ) / 2.0f;

        // Displace the midpoint in the direction of the ddh centre to the midpoint, so that distance between new
        // midpoint and the centre is the same as r_u

        glm::vec3 normal_to_point = edge_midpoint;

        normal_to_point = glm::normalize(normal_to_point);

        edge_midpoint.x = edge_midpoint.x - (r_i - r_u) / 4 * normal_to_point.x;
        edge_midpoint.y = edge_midpoint.y - (r_i - r_u) / 4 * normal_to_point.y;
        edge_midpoint.z = edge_midpoint.z - (r_i - r_u) / 4 * normal_to_point.z;

        midpoints.push_back(edge_midpoint);
    }

    // Assemble the cluster
    uint32_t index_0 = ddh_vertices.size();

    for (int i = 0; i < 3; i++) {
        ddh_vertices.insert(
                ddh_vertices.end(), {
                    ddh_vertices[pentagon_centres[i]]
                });

        ddh_vertices.insert(ddh_vertices.end(), {midpoints[i],1.0f});

        indices = {
                common_vertex, index_0, index_0 + 1,
                common_vertex, index_0 + 1, index_0 + 2,
                common_vertex, index_0 + 2, index_0 + 3,
                common_vertex, index_0 + 3, index_0 + 4,
                common_vertex, index_0 + 4, index_0 + 5,
                common_vertex, index_0 + 5, index_0
        };
    }
}

void increaseLODForClusterGroup(ClusterMesh& mesh,
                                int groupIndex){

    ClusterGroup& clusterGroup = mesh.clusterGroups[groupIndex];

    std::vector<Cluster> newClusters;
    newClusters.resize(clusterGroup.clusters.size() * 4);

    uint32_t triangle_assembly_indices[4][3] = {
            1, 2, 14,
            2, 5, 8,
            14, 8, 11,
            2, 8, 14
    };

    uint32_t edge_midpoint_order[3][2] = {
            {0, 1},{1, 2},{2, 0}
    };

    aout << "Edge length " << mesh.edge_length << std::endl;

    // radius of the circumscribed sphere **  that touches all mesh.vertices **
    const float r_u = 1.401f * mesh.edge_length;
    // radius of the circumscribed sphere ** tangent to each of the dodecahedron faces **
    const float r_i = 1.114f * mesh.edge_length;

    auto k = 0;

    glm::highp_f32vec3 P1, P2, midpoint;

    std::vector<glm::vec4> centroids;

    auto centroid_index = 0;

    std::vector<unsigned int> indices_winding_order(7);

    glm::vec3 normal;

    // Iterate over cluster group
    for(auto& oldCluster : clusterGroup.clusters){
        // Subdivide each cluster in 4 new clusters
        centroids.resize(4);
        for(k =0; k < 4; k++){
            indices_winding_order.clear();
            
            newClusters[k].indices = {
                    oldCluster.indices[triangle_assembly_indices[k][0]],
                    oldCluster.indices[triangle_assembly_indices[k][1]],
                    oldCluster.indices[triangle_assembly_indices[k][2]]
            };

            centroids[k] = ( mesh.vertices[newClusters[k].indices[0]] +
                             mesh.vertices[newClusters[k].indices[1]] +
                             mesh.vertices[newClusters[k].indices[2]]) / 3.0f;

            mesh.vertices.push_back(centroids[k]);
                      
            centroid_index = mesh.vertices.size() - 1;

            indices_winding_order.push_back(centroid_index);
            
            for(auto& edge_midpoint_operand_index : edge_midpoint_order){
                P1 = mesh.vertices[newClusters[k].indices[edge_midpoint_operand_index[0]]];
                P2 = mesh.vertices[newClusters[k].indices[edge_midpoint_operand_index[1]]];
                midpoint = (P1 + P2) / 2.0f;

                // Push the first one back
                mesh.vertices.insert(mesh.vertices.end(), {midpoint, 1.0f});
                indices_winding_order.push_back(newClusters[k].indices[edge_midpoint_operand_index[0]]);
                indices_winding_order.push_back(mesh.vertices.size() - 1);
            }

            newClusters[k].indices = {
                    indices_winding_order[0], indices_winding_order[1], indices_winding_order[2],
                    indices_winding_order[0], indices_winding_order[2], indices_winding_order[3],
                    indices_winding_order[0], indices_winding_order[3], indices_winding_order[4],
                    indices_winding_order[0], indices_winding_order[4], indices_winding_order[5],
                    indices_winding_order[0], indices_winding_order[5], indices_winding_order[6],
                    indices_winding_order[0], indices_winding_order[6], indices_winding_order[1],
            };
        }
    }

    mesh.clusterGroups[groupIndex].clusters = newClusters;
}

void decreaseLODForClusterGroup(std::vector<float>& vertices,
                                std::vector<uint32_t>& indices,
                                ClusterGroup& clusterGroup){
    // TODO: Implement this ...
}

void generate_pentagon(  std::vector<float>& vertices,
                         std::vector<uint32_t>& indices,
                         float h_r,
                         std::pair<float, float> centre) {
#define SIDES_OF_PENTAGON 5
    float x = 0.0f, y = 0.0f;
    vertices.resize(4 * 3);
    for(auto i = 0 ; i < SIDES_OF_PENTAGON; i++){
        x = std::cos(2 * M_PI / 5 * static_cast<double>(i)) * h_r + centre.first;
        y = std::sin(2 * M_PI / 5 * static_cast<double>(i)) * h_r + centre.second;

        vertices.insert(vertices.begin(), {x, y, 1.0f, 1.0f});
    }

    indices = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 1
    };
}