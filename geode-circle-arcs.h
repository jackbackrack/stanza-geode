#ifndef __IS_CIRCLE_ARCS__
#define __IS_CIRCLE_ARCS__

#include <geode/mesh/io.h>
#include <geode/vector/Matrix3x3.h>
#include <geode/vector/Matrix4x4.h>
#include <geode/vector/Matrix4x4.h>
#include <geode/vector/Rotation.h>
#include <geode/geometry/platonic.h>
#include <geode/geometry/Triangle3d.h>
#include <geode/geometry/polygon.h>
#include <geode/geometry/offset_mesh.h>
#include <geode/geometry/arc_fitting.h>
#include <geode/exact/mesh_csg.h>
#include <geode/exact/circle_csg.h>
#include <geode/exact/circle_offsets.h>
#include <geode/exact/find_overlapping_offsets.h>
// #include <geode/exact/delaunay.h>
#include <geode/exact/polygon_csg.h>
#include <geode/mesh/SegmentSoup.h>
#include <geode/mesh/TriangleTopology.h>

#include <cstdio>
#include <math.h>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>

using namespace geode;

typedef real T;
typedef Vector<T,2> TV2;
typedef Vector<T,3> TV3;
typedef Vector<int,2> IV2;
typedef Vector<int,3> IV3;
typedef Vector<int,4> IV4;

extern void error (std::string msg);
extern void error (std::string msg, std::string arg);
extern void ensure (bool val, std::string msg);

enum GeomKind {
  args_kind,
  num_kind,
  string_kind,
  v2d_kind,
  v3d_kind,
  v3i_kind,
  circle_arc_kind,
  mat_kind,
  array_v2d_kind,
  array_v3d_kind,
  array_v3i_kind,
  array_circle_arc_kind,
  nested_v2d_kind,
  nested_v3d_kind,
  nested_circle_arc_kind,
  poly_kind,
  mesh_kind,
  expr_kind,
  octree_kind,
  meshy_kind
};

class Geom;

class Geom {
 public:
  GeomKind k;
 Geom(GeomKind k) : k(k) { }
};

class ArgsGeom : public Geom {
 public:
  std::vector<Geom*> val;
 ArgsGeom(void) : Geom(args_kind) { }
};

class V2dGeom : public Geom {
 public:
  TV2 val;
 V2dGeom(TV2 val) : Geom(v2d_kind), val(val) { }
};

class ArrayV2dGeom : public Geom {
 public:
  Array<TV2> val;
 ArrayV2dGeom(Array<TV2> val) : Geom(array_v2d_kind), val(val) { }
};

class CircleArcGeom : public Geom {
 public:
  CircleArc val;
 CircleArcGeom(CircleArc val) : Geom(circle_arc_kind), val(val) { }
};

class ArrayCircleArcGeom : public Geom {
 public:
  Array<CircleArc> val;
 ArrayCircleArcGeom(Array<CircleArc> val) : Geom(array_circle_arc_kind), val(val) { }
};

class NestedCircleArcGeom : public Geom {
 public:
  Nested<CircleArc> val;
 NestedCircleArcGeom(Nested<CircleArc> val) : Geom(nested_circle_arc_kind), val(val) { }
};

class NestedV2dGeom : public Geom {
 public:
  Nested<TV2> val;
 NestedV2dGeom(Nested<TV2> val) : Geom(nested_v2d_kind), val(val) { }
};

extern TV2 g_v2d_val(Geom* g);
extern Array<TV2> g_array_v2d_val(Geom* g);
extern bool all_args_kind (std::vector<Geom*> args, int kind);
extern Geom* g_circle_arc (CircleArc v);
extern Geom* g_array_circle_arc(Array<CircleArc> arcs);
extern Geom* g_nested_circle_arc(Nested<CircleArc> arcz);
extern Geom* g_circle_arcs (std::vector<Geom*> args);

extern "C" Geom* g_args_fab(void);
extern "C" void g_args_delete(Geom *g);
extern "C" Geom* g_args_add(Geom* g, Geom* e);
extern "C" int g_args_len(Geom* g);
extern std::vector<Geom*> g_args_val(Geom* g);

extern "C" Geom* g_v2d_fab(float x, float y);
extern "C" void g_v2d_delete(Geom* g);

extern "C" void g_array_v2d_delete(Geom *g);
extern "C" Geom* g_array_v2d_elt(Geom* g, int idx);
extern "C" int g_array_v2d_len(Geom* g);

extern "C" void g_nested_v2d_delete(Geom *g);
extern "C" Geom* g_nested_v2d_elt(Geom* g, int idx);
extern "C" int g_nested_v2d_len(Geom* g);

extern "C" Geom* g_circle_arc_fab(float x, float y, float q);
extern "C" void g_circle_arc_delete(Geom *g);
extern "C" float g_circle_arc_x(Geom* g);
extern "C" float g_circle_arc_y(Geom* g);
extern "C" float g_circle_arc_q(Geom* g);
extern "C" Geom* g_array_circle_arc_fab(Geom* args);
extern "C" void g_array_circle_arc_delete(Geom *g);
extern "C" Geom* g_array_circle_arc_elt(Geom* g, int idx);
extern "C" int g_array_circle_arc_len(Geom* g);
extern "C" Geom* g_nested_circle_arc_fab(Geom* args);
extern "C" void g_nested_circle_arc_delete(Geom *g);
extern "C" Geom* g_nested_circle_arc_elt(Geom* g, int idx);
extern "C" int g_nested_circle_arc_len(Geom* g);
extern "C" int   g_nested_circle_arc_len(Geom* g);
extern "C" Geom* g_nested_circle_arc_fab(Geom* g);
extern "C" Geom* g_nested_circle_arc_elt(Geom* g, int i);
extern "C" Geom* g_nested_circle_arc_open_offset(float d, Geom* p);
extern "C" Geom* g_nested_circle_arc_closed_offset(float d, Geom* p);
extern "C" Geom* g_nested_circle_arc_discretize(float max_deviation, int is_closed, Geom* p);
extern "C" Geom* g_nested_circle_arc_union(Geom* a, Geom* b);
extern "C" Geom* g_nested_circle_arc_union1(Geom* a);
extern "C" Geom* g_nested_circle_arc_intersection(Geom* a, Geom* b);
extern "C" Geom* g_nested_circle_arc_intersection1(Geom* a);
extern "C" Geom* g_nested_circle_arc_concat(Geom* a, Geom* b);
extern "C" Geom* g_nested_circle_arc_concat_all(Geom* arcz);
extern "C" Geom* g_nested_circle_arc_not(Geom* a);
extern "C" Geom* g_nested_circle_arc_difference(Geom* a, Geom* b);
extern "C" Geom* g_nested_circle_arc_bounds(Geom* g);
extern "C" Geom* g_nested_circle_arc_find_overlapping_offsets(float d, Geom* a);

extern Geom* g_array_circle_arc(Array<CircleArc> arcs);
extern Geom* g_array_circle_arc(RawArray<CircleArc> arcs);
extern Array<CircleArc> g_array_circle_arc_val(Geom* g);

extern Geom* g_nested_circle_arc(Nested<CircleArc> arcs);
extern Geom* g_arcs(Nested<TV2> arcz);
extern Nested<CircleArc> g_nested_circle_arc_val(Geom* g);

#endif
