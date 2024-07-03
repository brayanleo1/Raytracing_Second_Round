#include "rt3.h"
#include "triangle.h"

#include <chrono>
#include <iostream>
#include <iterator>



namespace rt3 {

bool Triangle::intersect(const Ray& r, Surfel* sf) const {
  // Implement the intersection between the ray and sphere
  // Calculate the intersection point, surface normal, and other required information
  // Assign the values to the Surfel object (sf)

  constexpr float epsilon = std::numeric_limits<float>::epsilon();
  //v[0], v[1], v[2] are actually from the mesh in the way
  //v[0] = mesh->vertices[v[0]];
  auto v0 = mesh.get()->vertices[v[0]];
  auto v1 = mesh.get()->vertices[v[1]];
  auto v2 = mesh.get()->vertices[v[2]];

  auto n0 = mesh.get()->normals[n[0]];
  auto n1 = mesh.get()->normals[n[1]];
  auto n2 = mesh.get()->normals[n[2]];

    Point3f edge1 = v1 - v0;
    Point3f edge2 = v2 - v0;
    Point3f ray_cross_e2 = cross(r.direction(), edge2);
    float det = dot(edge1, ray_cross_e2);

    if (det > -epsilon && det < epsilon) {
      return false;    // This ray is parallel to this triangle.
    }
    float inv_det = 1.0 / det;
    Point3f s = r.origin() - v0;
    float u = inv_det * dot(s, ray_cross_e2);

    if (u < 0 || u > 1) {
      return false;
    }
    Point3f s_cross_e1 = cross(s, edge1);
    float v = inv_det * dot(r.direction(), s_cross_e1);

    if (v < 0 || u + v > 1) {
      return false;
    }
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = inv_det * dot(edge2, s_cross_e1);

    if (t > epsilon && t < r.t_max) // ray intersection
    {
      r.t_max = t;
      // Assign the values to the Surfel object (sf)
      sf->time = t;
      // Assign the contact point for the triangle, using the barycentric coordinates.
      sf->p = v0 * (1 - u - v) + v1 * u + v2 * v;
      sf->n = n0 * (1 - u - v) + n1 * u + n2 * v; sf->n.make_unit_vector();
      sf->wo = -r.direction();
      sf->primitive = nullptr; //it is assigned outside of this function
      sf->uv = Point2f{u, v};
      return true;
    }
    // This means that there is a line intersection but not a ray intersection.
    return false;

  // Return true if intersection found, false otherwise
  return false;
}

bool Triangle::intersect_p(const Ray& r) const {
  
  constexpr float epsilon = std::numeric_limits<float>::epsilon();
  //v[0], v[1], v[2] are actually from the mesh in the way
  //v[0] = mesh->vertices[v[0]];
  auto v0 = mesh.get()->vertices[v[0]];
  auto v1 = mesh.get()->vertices[v[1]];
  auto v2 = mesh.get()->vertices[v[2]];

    Point3f edge1 = v1 - v0;
    Point3f edge2 = v2 - v0;
    Point3f ray_cross_e2 = cross(r.direction(), edge2);
    float det = dot(edge1, ray_cross_e2);

    if (det > -epsilon && det < epsilon) {
      return false;    // This ray is parallel to this triangle.
    }
    float inv_det = 1.0 / det;
    Point3f s = r.origin() - v0;
    float u = inv_det * dot(s, ray_cross_e2);

    if (u < 0 || u > 1) {
      return false;
    }
    Point3f s_cross_e1 = cross(s, edge1);
    float v = inv_det * dot(r.direction(), s_cross_e1);

    if (v < 0 || u + v > 1) {
      return false;
    }
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = inv_det * dot(edge2, s_cross_e1);

    if (t > epsilon && t < r.t_max) // ray intersection
    {
      r.t_max = t;
      return true;
    }
    // This means that there is a line intersection but not a ray intersection.
    return false;

  /*Vector3f oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = 2.0 * dot(oc, r.direction());
  float c = dot(oc, oc) - radius * radius;
  float discriminant = b * b - 4 * a * c;
  return (discriminant > 0);*/
}


/// To help debug triangles.
std::ostream &operator<<(std::ostream &os, const Triangle &t) {
  constexpr short X{0};
  constexpr short Y{1};
  constexpr short Z{2};

  os << "    + V0: (" << t.mesh->vertices[t.v[0]][X] << ", "
     << t.mesh->vertices[t.v[0]][Y] << ", " << t.mesh->vertices[t.v[0]][Z]
     << ")\n"
     << "    + V1: (" << t.mesh->vertices[t.v[1]][X] << ", "
     << t.mesh->vertices[t.v[1]][Y] << ", " << t.mesh->vertices[t.v[1]][Z]
     << ")\n"
     << "    + V2: (" << t.mesh->vertices[t.v[2]][X] << ", "
     << t.mesh->vertices[t.v[2]][Y] << ", " << t.mesh->vertices[t.v[2]][Z]
     << ")\n"

     << "    + N0: (" << t.mesh->normals[t.n[0]][X] << ", "
     << t.mesh->normals[t.n[0]][Y] << ", " << t.mesh->normals[t.n[0]][Z]
     << ")\n"
     << "    + N1: (" << t.mesh->normals[t.n[1]][X] << ", "
     << t.mesh->normals[t.n[1]][Y] << ", " << t.mesh->normals[t.n[1]][Z]
     << ")\n"
     << "    + N2: (" << t.mesh->normals[t.n[2]][X] << ", "
     << t.mesh->normals[t.n[2]][Y] << ", " << t.mesh->normals[t.n[2]][Z]
     << ")\n"

     << "    + UV0: (" << t.mesh->uvcoords[t.uv[0]][X] << ", "
     << t.mesh->uvcoords[t.uv[0]][Y] << ")\n"
     << "    + UV1: (" << t.mesh->uvcoords[t.uv[1]][X] << ", "
     << t.mesh->uvcoords[t.uv[1]][Y] << ")\n"
     << "    + UV2: (" << t.mesh->uvcoords[t.uv[2]][X] << ", "
     << t.mesh->uvcoords[t.uv[2]][Y] << ")\n";

  return os;
}

// This is the function called by the API to create a triangle mesh shape.
/*
 * This is the entry-point function for the client code.
 * This function decodes the `ParamSet` data and, based on that, it either
 * (1) creates a triangle mesh manually (from the XML attributes), or
 * (2) calls another function to load an OBJ file and create the triangle mesh.
 *
 * @param flip_normals This flag asks to invert the normal of all triangles.
 * This is a global flag, set by a API command.
 * @param ps The ParamSet object sent from the client code with all the
 * information related to the triangle mesh read from the scene file.
 *
 * @return The list of Shape (triangles) that we read from the scene file.
 */
vector<std::shared_ptr<Shape>> create_triangle_mesh_shape(bool flip_normals,
                                                     const ParamSet &ps) {
  bool bkfc_on{
      true}; // Controls whether the backface cull should be done or not.
  bool reverse_vertex_order{false}; // If this is true, we store vertices in
                                    // reverse order inside the mesh.
  bool compute_normals{false};      // Indicate whether we need to calculate the
                                    // triangle's normals manually.

  // This is a collection of data structures passed between function calls.
  // This structure may be filled in with information from a OBJ file
  // or from the XML attributes extracted from the scene file itself.
  std::shared_ptr<TriangleMesh> mesh =
      std::make_shared<TriangleMesh>(); // Default Ctro.

  // Retrieve filename.
  string filename =
      retrieve(ps,"filename",std::string("")); // Retrieving data associated with 'filename' attrib.
  // Retrieve backface ON/OFF
  string bkf_on_str = retrieve(ps,"backface_cull",std::string("false"));
  if (bkf_on_str == "off" or bkf_on_str == "false") {
    bkfc_on = false;
  }
  // Retrieve Reverse vertex order ON/OFF
  string rvo_str = retrieve(ps,"reverse_vertex_order",std::string("false"));
  if (rvo_str == "on" or rvo_str == "true") {
    reverse_vertex_order = true;
  }
  // Retrieve compute normals flag
  string cn_str = retrieve(ps,"compute_normals", std::string("false"));
  if (cn_str == "on" or cn_str == "true") {
    compute_normals = true;
  }

  // Here we have only two option: read mesh data from OBJ file or from the
  // ParamSet.
  if (filename !=
      "") { // Read mesh data from an OBJ file provided in the scene file.
    // Call our auxiliary function that interfaces with tinyobjloader API.
    if (load_mesh_data(filename, reverse_vertex_order, compute_normals,
                       flip_normals, mesh)) {
      std::cout << ">>> Mesh data successfuly loaded!\n";
    } else {
      std::cout << ">>> Mesh data loading failed!\n";
    }
  } else { // This means we must read the mesh data from the scene file, not
           // from the OBJ file.

    /*
    <object type="trianglemesh" material="grey"
            ntriangles ="2"
            vertices ="-4 0 4  4 0 4  4 0 -4  -4 0 -4"
            indices ="0 1 2   0 2 3"
            normals ="0 1 0  0 1 0  0 1 0  0 1 0 "
            reverse_vertex_order="false"
            backface_cull ="off"/>
    */

    // TODO: retrieve the attributes of the triangle mesh present in the XML
    // scene file.
    auto ntriangles = retrieve(ps,"ntriangles",0);
    mesh->n_triangles = ntriangles;
    //with ntriangles we can calculate the number of vertices, normals, indices and uvcoords
    // lets start now
    // Retrieve the vertices

    auto vertices = retrieve(ps,"vertices",std::vector<Point3f>());
    for (auto &v : vertices) {
      // Retrieve the vertex data and store it in the mesh data structure.
      mesh->vertices.push_back(v);
    }

    // Now we must check if we need to compute the normals.
    if (compute_normals) {
      // Compute the normals here.
      for(auto i = 0; i < ntriangles; i++){
        auto v0 = mesh->vertices[mesh->vertex_indices[3*i]];
        auto v1 = mesh->vertices[mesh->vertex_indices[3*i+1]];
        auto v2 = mesh->vertices[mesh->vertex_indices[3*i+2]];
        auto n = compute_normal(v0,v1,v2);
        mesh->normals.push_back(n);
        mesh->normals.push_back(n);
        mesh->normals.push_back(n);
      }
    } else {
      auto normals = retrieve(ps,"normals",std::vector<Point3f>());
      for (auto &n : normals) {
        // Retrieve the normal data and store it in the mesh data structure.
        mesh->normals.push_back(n);
      }
    }
    
    auto uvcoords = retrieve(ps,"uv",std::vector<Point2f>());
    for (auto &uv : uvcoords) {
      // Retrieve the uv data and store it in the mesh data structure.
      mesh->uvcoords.push_back(uv);
    }
    //indices are 

    auto indices = retrieve(ps,"indices",std::vector<Vector3i>());
    for (auto &idx : indices) {
      // Retrieve the indices data and store it in the mesh data structure.
      mesh->vertex_indices.push_back(idx[0]);
      mesh->vertex_indices.push_back(idx[1]);
      mesh->vertex_indices.push_back(idx[2]);
      mesh->normal_indices.push_back(idx[0]);
      mesh->normal_indices.push_back(idx[1]);
      mesh->normal_indices.push_back(idx[2]);
      mesh->uvcoord_indices.push_back(idx[0]);
      mesh->uvcoord_indices.push_back(idx[1]);
      mesh->uvcoord_indices.push_back(idx[2]);
    }
    

    //
    // Here you must retrieve from the ParamSet all the data that might appear
    // in a trianglemesh object. Usually these are the vertices, normals, and
    // indices.
    //
    // You should read all the data into the `tri_mesh_data` object so that the
    // function call below works, regardless if the data came from the OBJ file
    // or was read directly from the scene file.
  }

  // At this point, the tri_mesh_data object has already been filled in with
  // data coming either from a OBJ file or from the scene file.
  return create_triangle_mesh(mesh,
                              bkfc_on); // Note the use of move semantics here.
}

/// This function calls the basic tinyobjloader loading function and stores all
/// the data into the tinyobjloader's internal data structures.
bool load_mesh_data(const std::string &filename, bool rvo, bool cn, bool fn,
                    std::shared_ptr<TriangleMesh> md) {
  // Default load parameters
  const char *basepath = nullptr;
  bool triangulate = true;

  std::cout << "Loading " << filename << std::endl;

  // This is the tinyobj internal data structures.
  tinyobj::attrib_t
      attrib; // Hold all the data, such as vertex, normals, UV coords
  std::vector<tinyobj::shape_t>
      shapes; // Represents a collection of faces, grouped in a unit.
  std::vector<tinyobj::material_t> materials; // Materials, NOT USED HERE.

  // Warnings and Error values.
  std::string warn;
  std::string err;

  // Timing loading.
  //================================================================================
  auto start = std::chrono::steady_clock::now();
  //================================================================================
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                              filename.c_str(), basepath, triangulate);
  //================================================================================
  std::chrono::time_point<std::chrono::steady_clock> end =
      std::chrono::steady_clock::now();
  // Store the time difference between start and end
  std::chrono::duration<double> diff = end - start;

  // Milliseconds (10^-3)
  std::cout << "\t\t>>> "
            << std::chrono::duration<double, std::milli>(diff).count() << " ms"
            << std::endl;

  // Nanoseconds (10^-9)
  std::cout << "\t\t>>> "
            << std::chrono::duration<double, std::nano>(diff).count() << " ns"
            << std::endl;

  // Seconds
  auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
  std::cout << "\t\t>>> " << diff_sec.count() << " s" << std::endl;
  //================================================================================

  if (!warn.empty()) {
    std::cout << "WARN: " << warn << std::endl;
  }

  if (!err.empty()) {
    std::cerr << "ERR: " << err << std::endl;
  }

  if (!ret) {
    std::cout << "Failed to load/parse .obj.\n";
    return false;
  }

  // Let us now "convert" or "migrate" the data from tinyobjloader data
  // structure into our mesh data format.
  extract_obj_data(attrib, shapes, // TinyObjeLoader data structures (IN)
                   rvo, cn, fn,    // Mesh modifiers (IN)
                   md); // Reference to the mesh data to fill in. (OUT)

  return true;
}

/// This is the function that converts from the tinyobjloader's internal storage
/// to our mesh data organization.
void extract_obj_data(const tinyobj::attrib_t &attrib,
                      const std::vector<tinyobj::shape_t> &shapes,
                      bool reverse_order, bool cn, bool fn,
                      /* OUT */ std::shared_ptr<TriangleMesh> md) {
  std::cout << "-- SUMMARY of the OBJ file --\n";
  std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
  std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
  std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
            << std::endl;
  std::cout << "# of shapes    : " << shapes.size() << std::endl;
  std::cout << "-----------------------------\n";

  // Retrieve the complete list of vertices.
  auto n_vertices{attrib.vertices.size() / 3};
  for (auto idx_v{0U}; idx_v < n_vertices; idx_v++) {
    std::cout << "   v[" << static_cast<long>(idx_v) << "] = ( "
         << static_cast<double>(attrib.vertices[3 * idx_v + 0]) << ", "
         << static_cast<double>(attrib.vertices[3 * idx_v + 1]) << ", "
         << static_cast<double>(attrib.vertices[3 * idx_v + 2]) << " )\n";

    // Store the vertex in the mesh data structure.
    md->vertices.push_back(Point3f{attrib.vertices[3 * idx_v + 0],
                                   attrib.vertices[3 * idx_v + 1],
                                   attrib.vertices[3 * idx_v + 2]});
  }

  // Read the normals
  // TODO: flip normals if requested.
  tinyobj::real_t flip = (fn) ? -1 : 1;
  auto n_normals{attrib.normals.size() / 3};

  // Do we need to compute the normals? Yes only if the user requeste or there
  // are no normals in the file. if ( cn == true or n_normals == 0)
  // {
  //    // TODO: COmpute normals here.
  //    // compute_normals();
  // }
  // else {// Read normals from file. This corresponds to the entire 'for'
  // below.

  // Traverse the normals read from the OBJ file.
  for (auto idx_n{0U}; idx_n < n_normals; idx_n++) {
    std::cout << "   n[" << static_cast<long>(idx_n) << "] = ( "
         << static_cast<double>(attrib.normals[3 * idx_n + 0]) << ", "
         << static_cast<double>(attrib.normals[3 * idx_n + 1]) << ", "
         << static_cast<double>(attrib.normals[3 * idx_n + 2]) << " )\n";

    // Store the normal.
    md->normals.push_back(Normal3f{attrib.normals[3 * idx_n + 0] * flip,
                                   attrib.normals[3 * idx_n + 1] * flip,
                                   attrib.normals[3 * idx_n + 2] * flip});
  }

  // Read the complete list of texture coordinates.
  auto n_texcoords{attrib.texcoords.size() / 2};
  for (auto idx_tc{0U}; idx_tc < n_texcoords; idx_tc++) {
    std::cout << "   t[" << static_cast<long>(idx_tc) << "] = ( "
         << static_cast<double>(attrib.texcoords[2 * idx_tc + 0]) << ", "
         << static_cast<double>(attrib.texcoords[2 * idx_tc + 1]) << " )\n";

    // Store the texture coords.
    md->uvcoords.push_back(Point2f{attrib.texcoords[2 * idx_tc + 0],
                                   attrib.texcoords[2 * idx_tc + 1]});
  }

  // Read mesh connectivity and store it as lists of indices to the real data.
  auto n_shapes{shapes.size()};
  md->n_triangles = 0; // We must reset this here.
  // In case the OBJ file has the triangles organized in several shapes or
  // groups, we ignore this and store all triangles as a single mesh dataset.
  // This is why we need to reset the triangle count here.
  for (auto idx_s{0U}; idx_s < n_shapes; idx_s++) {
    std::cout << "The shape[" << idx_s << "].name = " << shapes[idx_s].name << '\n';
    std::cout << "Size of shape[" << idx_s << "].mesh.indices: "
         << static_cast<unsigned long>(shapes[idx_s].mesh.indices.size())
         << '\n';
    std::cout << "shape[" << idx_s << "].num_faces: "
         << static_cast<unsigned long>(
                shapes[idx_s].mesh.num_face_vertices.size())
         << '\n';

    // For each face print out the indices lists.
    size_t index_offset = 0;
    // # of triangles for this "shape" (group).
    // NOTE that we are accumulate the number of triangles coming from the
    // shapes present in the OBJ file.
    md->n_triangles += shapes[idx_s].mesh.num_face_vertices.size();
    for (auto idx_f{0}; idx_f < md->n_triangles; idx_f++) {
      // Number of vertices per face (always 3, in our case)
      size_t fnum = shapes[idx_s].mesh.num_face_vertices[idx_f];

      std::cout << "  face[" << idx_f
           << "].fnum = " << static_cast<unsigned long>(fnum) << '\n';

      // TODO: Invert order of vertices if flag is on. (DONE!)
      if (reverse_order) {
        std::cout << "Reverse order\n";
        // For each vertex in the face print the corresponding indices
        for (int v = fnum - 1; v >= 0; v--) {
          tinyobj::index_t idx = shapes[idx_s].mesh.indices[index_offset + v];
          std::cout << "    face[" << idx_f << "].v[" << v
               << "].indices = " << idx.vertex_index << "/" << idx.normal_index
               << "/" << idx.texcoord_index << '\n';
          // Add the indices to the global list of indices we need to pass on to
          // the mesh object.
          md->vertex_indices.push_back(idx.vertex_index);
          md->normal_indices.push_back(idx.normal_index);
          md->uvcoord_indices.push_back(idx.texcoord_index);
        }
      } else { // Keep the original vertex order
        // For each vertex in the face get the corresponding indices
        for (size_t v = 0; v < fnum; v++) {
          tinyobj::index_t idx = shapes[idx_s].mesh.indices[index_offset + v];
          std::cout << "    face[" << idx_f << "].v[" << v
               << "].indices = " << idx.vertex_index << "/" << idx.normal_index
               << "/" << idx.texcoord_index << '\n';
          // Add the indices to the global list of indices we need to pass on to
          // the mesh object. This goes to the mesh data structure.
          md->vertex_indices.push_back(idx.vertex_index);
          md->normal_indices.push_back(idx.normal_index);
          md->uvcoord_indices.push_back(idx.texcoord_index);
        }
      }

      // Advance over to the next triangle.
      index_offset += fnum;
    }
  }

  std::cout << "This is the list of indices: \n";

  std::cout << "   + Vertices [ ";
  std::copy(md->vertex_indices.begin(), md->vertex_indices.end(),
            std::ostream_iterator<int>(std::cout, " "));
  std::cout << "]\n";

  std::cout << "   + Normals [ ";
  std::copy(md->normal_indices.begin(), md->normal_indices.end(),
            std::ostream_iterator<int>(std::cout, " "));
  std::cout << "]\n";

  std::cout << "   + UV coords [ ";
  std::copy(md->uvcoord_indices.begin(), md->uvcoord_indices.end(),
            std::ostream_iterator<int>(std::cout, " "));
  std::cout << "]\n";
}

/// This function creates the internal data structure, required by the RT3.
vector<std::shared_ptr<Shape>> create_triangle_mesh(std::shared_ptr<TriangleMesh> mesh,
                                               bool backface_cull) {
  // List of shapes (triangles) we need to return to the client.
  vector<std::shared_ptr<Shape>> tris;
  // Create the triangles, which are just references to the mesh database.
  tris.reserve(mesh->n_triangles);
  for (int i = 0; i < mesh->n_triangles; ++i) {
    tris.push_back(std::make_shared<Triangle>(mesh, i, backface_cull));
  }

  return tris;
}


Bounds3f Triangle::world_bounds() const {
  Bounds3f bounds;
  // For each point, i access with mesh.get()->vertices[v[0]];
  auto v0 = mesh.get()->vertices[v[0]];
  auto v1 = mesh.get()->vertices[v[1]];
  auto v2 = mesh.get()->vertices[v[2]];
  bounds.pMin = min(min(v0, v1), v2);
  bounds.pMax = max(max(v0, v1), v2);

  // Assign the values to the bounds object
  return bounds;
}

// oldly used
Triangle *create_triangle(const ParamSet &ps) {
  // Create a new triangle object and return it.
  return nullptr;
}

//compute normals of the triangle
Vector3f compute_normal(const Point3f &v0, const Point3f &v1, const Point3f &v2) {
  Vector3f edge1 = v1 - v0;
  Vector3f edge2 = v2 - v0;
  return cross(edge1, edge2);
}

} // namespace rt3