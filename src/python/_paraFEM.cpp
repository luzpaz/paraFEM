#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/eigen.h>

#include "node.h"
#include "element.h"
#include "case.h"
#include "material.h"

#include "vtkWriter.h"

#include "unwrap.h"


PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);


namespace py = pybind11;

void init_paraFEM(py::module &m){
    py::module::import("paraEigen");

    py::class_<paraFEM::VtkWriter, std::shared_ptr<paraFEM::VtkWriter>>(m, "vtkWriter")
        .def(py::init<const char*>())
        .def("writeCase", &paraFEM::VtkWriter::writeCase);

    py::class_<paraFEM::Node, paraFEM::NodePtr> (m, "Node")
        .def(py::init<double, double, double>())
        .def_readonly("position", &paraFEM::Node::position)
        .def_readonly("velocity", &paraFEM::Node::velocity)
        .def_readonly("acceleration", &paraFEM::Node::acceleration)
        .def_readwrite("fixed", &paraFEM::Node::fixed)
        .def_readwrite("massInfluence", &paraFEM::Node::massInfluence)
        .def("add_external_force", &paraFEM::Node::add_external_force);

    py::class_<paraFEM::Material>(m, "Material")
        .def_readwrite("rho", &paraFEM::Material::rho)
        .def_readwrite("d_structural", &paraFEM::Material::d_structural)
        .def_readwrite("d_velocity", &paraFEM::Material::d_velocity)
        .def_readwrite("elasticity", &paraFEM::Material::elasticity);
    
    py::class_<paraFEM::TrussMaterial, paraFEM::TrussMaterialPtr>(m, "TrussMaterial", py::base<paraFEM::Material>())
        .def(py::init<double>());
    
    py::class_<paraFEM::MembraneMaterial, paraFEM::MembraneMaterialPtr>(m, "MembraneMaterial", py::base<paraFEM::Material>())
        .def(py::init<double, double>());

    py::class_<paraFEM::Element, paraFEM::ElementPtr>(m, "__Element")
        .def("getStress", &paraFEM::Element::getStress)
        .def_readonly("nodes", &paraFEM::Element::nodes);

    py::class_<paraFEM::Membrane, paraFEM::MembranePtr>(m, "__Membrane", py::base<paraFEM::Element>())
        .def_readwrite("pressure", &paraFEM::Membrane::pressure)
        .def("setConstPressure", &paraFEM::Membrane::setConstPressure);
        
    py::class_<paraFEM::Truss, paraFEM::TrussPtr> (m, "Truss", py::base<paraFEM::Element>())
        .def(py::init<std::vector<paraFEM::NodePtr>, paraFEM::TrussMaterialPtr>());
    
    py::class_<paraFEM::Membrane3, paraFEM::Membrane3Ptr> (m, "Membrane3", py::base<paraFEM::Membrane>())
        .def(py::init<std::vector<paraFEM::NodePtr>, paraFEM::MembraneMaterialPtr>());
      
    py::class_<paraFEM::Membrane4, paraFEM::Membrane4Ptr> (m, "Membrane4", py::base<paraFEM::Membrane>())
        .def(py::init<std::vector<paraFEM::NodePtr>, paraFEM::MembraneMaterialPtr>())
        .def(py::init<std::vector<paraFEM::NodePtr>, paraFEM::MembraneMaterialPtr, bool>());

    py::class_<paraFEM::FemCase, paraFEM::FemCasePtr> (m, "Case")
        .def(py::init<std::vector<paraFEM::ElementPtr>>())
        .def("explicitStep", &paraFEM::FemCase::explicitStep, "make one explicit step",
            py::arg("h") = 0.0001, py::arg("externalFactor") = 1);

    py::class_<paraFEM::LscmRelax>(m, "LscmRelax")
        .def(py::init<std::vector<std::array<double, 3>>, std::vector<std::array<long, 3>>, std::vector<long>>())
        .def(py::init<paraFEM::ColMat<double, 3>, paraFEM::ColMat<long, 3>, std::vector<long>>())
        .def("lscm", &paraFEM::LscmRelax::lscm)
        .def("relax", &paraFEM::LscmRelax::relax)
        .def("rotate_by_min_bound_area", &paraFEM::LscmRelax::rotate_by_min_bound_area)
        .def_readonly("rhs", &paraFEM::LscmRelax::rhs)
        .def_readonly("MATRIX", &paraFEM::LscmRelax::MATRIX)
        .def_property_readonly("area", &paraFEM::LscmRelax::get_area)
        .def_property_readonly("flat_area", &paraFEM::LscmRelax::get_flat_area)
        .def_property_readonly("flat_vertices", [](paraFEM::LscmRelax& L){return L.flat_vertices.transpose();}, py::return_value_policy::copy)
        .def_property_readonly("flat_vertices_3D", &paraFEM::LscmRelax::get_flat_vertices_3D);
}


PYBIND11_PLUGIN(_paraFEM){
    py::module m("_paraFEM");
    init_paraFEM(m);
    return m.ptr();
};