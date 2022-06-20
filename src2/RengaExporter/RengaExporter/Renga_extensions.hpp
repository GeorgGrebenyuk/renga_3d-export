#pragma once
#include "pch.hpp"
class Renga_object_definition;
class Renga_getting_data
{
public:
    Renga_getting_data(Renga::IApplicationPtr app, bool UseHiddenObjects,
        std::vector<double> coord_offset);
    std::map<int, Renga_object_definition> objects;
    int time_to_getting_objects_data; //~time to getting geometry and properties (COM total querry)
private:
    Renga::IProjectPtr renga_project;
    std::map<int, bool> object2vis_status; //objects visiability
    std::vector<double> coord_offset;
    void recalc_coords(Renga::FloatPoint3D *point) {
        double new_x = coord_offset.at(0) + (*point).X * cos(coord_offset.at(3)) - (*point).Y * sin(coord_offset.at(3));
        double new_y = coord_offset.at(1) + (*point).X * sin(coord_offset.at(3)) + (*point).Y * cos(coord_offset.at(3));
        double new_z = coord_offset.at(2) + (*point).Z;
        (*point).X = new_x;
        (*point).Y = new_y;
        (*point).Z = new_z;
    };
    void get_material(Renga::IModelObjectPtr obj, std::vector<double>* link_color);
    void get_material_grid(GUID object_type, int grid_type, std::vector<double>* link_color);
    void properties_work(Renga::IModelObjectPtr obj);
};
struct grid_definition {
public:
    std::vector<double> color;
    std::list<std::vector<double>> grid_geometry;
};
class Renga_object_definition {
public:
    //Renga_object_definition();
    bool object_visiable;
    //heometry and material's color
    /* std::vector<double> one triangle
     * std::list<std::vector<double>> triangles in grid = grid_definition.grid_geometry
     * grid_definition info about grid (color + geometry of triangles)
     * std::list<grid_definition> mesh geometry (couple of grids)
     * std::list<std::list<grid_definition>> all meshes geometry
     */
    std::list<std::list<grid_definition>> object_geometry;
    //properties
    std::map<const char*, const char*> object_properties;
    std::map<const char*, const char*> object_quantities;
    std::map<const char*, const char*> object_parameters;
    //type
    const char* object_type;
    const char* object_type_str;
    //name
    const char* object_name;

};