/*
Add-on implementation is here !
*/

#include <ultimaille/all.h>

using namespace UM;

vec3 triNormal(Triangles& m, Surface::Facet f) {    
    const vec3 & p1 = m.points[f.vertex(0)];
    const vec3 & p2 = m.points[f.vertex(1)];
    const vec3 & p3 = m.points[f.vertex(2)];

    return cross(p2 - p1, p3 - p1);
}

void smooth_iter(Triangles &m, PointAttribute<bool>& lock) {
    
    PointAttribute<vec4> bary(m.points);

    for (auto v : m.iter_vertices()) {
        for (auto h : v.iter_halfedges()) {
            vec3 neighbor_pos = h.to().pos();
            bary[v] += {neighbor_pos.x, neighbor_pos.y, neighbor_pos.z, 1.};
        }
    }

    for (auto v : m.iter_vertices()) {
        if (lock[v]) continue;
        vec4 cur_bary = bary[v];
        vec3 p = vec3(cur_bary[0], cur_bary[1], cur_bary[2]) / cur_bary[3];
        v.pos() = p;
    }
}

std::string smooth(std::string filename, std::string lock_attr_name, int n_iter, std::string str_result_path) {
    
    // Get parameters
    std::filesystem::path result_path(str_result_path);

    // Print
    std::cout << "Input model: " << filename << std::endl;

    Triangles m;
    SurfaceAttributes attr = read_by_extension(filename, m);
    PointAttribute<bool> lock_attr(lock_attr_name, attr, m);
    m.connect();

    // Converge to a minimal surface
    for (int i = 0; i < n_iter; i ++)
        smooth_iter(m, lock_attr);
    
    // Save

    // Output model to output directory at working dir if no result_path given
    if (result_path.empty() && !std::filesystem::is_directory("output")) {
        std::filesystem::create_directories("output");
        result_path = "output";
    }

    // Get file name and output path
    std::string file = std::filesystem::path(filename).filename().string();
    std::string out_filename = (result_path / file).string();


    write_by_extension(out_filename, m, {attr.points, attr.facets, attr.corners});
	return out_filename;
}

// Wrapper
extern "C" {
    const char* smooth_c(const char* filename, const char* lock_attr_name, int n_iter, const char* str_result_path) {
        return smooth(std::string(filename), lock_attr_name, n_iter, str_result_path).c_str();
    }
}