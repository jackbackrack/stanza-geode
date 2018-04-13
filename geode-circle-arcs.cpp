#include "geode-circle-arcs.h"

void error (std::string msg) {
  fprintf(stderr, "MSG %s\n", msg.c_str());
  exit(-1);
}

void error (std::string msg, std::string arg) {
  fprintf(stderr, "MSG %s %s\n", msg.c_str(), arg.c_str());
  exit(-1);
}

void ensure (bool val, std::string msg) {
  if (!val)
    error(msg);
}

Geom* g_v2d(TV2 v) { return new V2dGeom(v); }
void g_v2d_delete(Geom* g) { delete g; }
Geom* g_array_v2d(Array<TV2> line) { return new ArrayV2dGeom(line); }
Geom* g_array_v2d(RawArray<TV2> line) { return g_array_v2d(line.copy()); }

bool all_args_kind (std::vector<Geom*> args, int kind) {
  bool res = true;
  for (auto arg : args) 
    res = res && arg->k == kind;
  return res;
}

std::vector<Geom*> g_args_val(Geom* g) {
  ensure(g->k == args_kind, "NOT ARGS");
  return ((ArgsGeom*)g)->val;
}
Geom* g_args_fab(void) { return new ArgsGeom(); }
void g_args_delete(Geom* g) { delete ((ArgsGeom*)g); }
Geom* g_args_add(Geom* g, Geom* e) {
  ((ArgsGeom*)g)->val.push_back(e);
  return g;
}
int g_args_len(Geom* g) { return g_args_val(g).size(); }

TV2 g_v2d_val(Geom* g) {
  ensure(g->k == v2d_kind, "NOT V2D");
  return ((V2dGeom*)g)->val;
}
Array<TV2> g_array_v2d_val(Geom* g) {
  ensure(g->k == array_v2d_kind, "NOT ARRAY_V2D");
  return ((ArrayV2dGeom*)g)->val;
}
Geom* g_array_v2d_fab(Geom* args) {
  Array<TV2> v;
  for (auto arg : g_args_val(args)) 
    v.append(g_v2d_val(arg));
  return g_array_v2d(v);
}
void g_array_v2d_delete(Geom* g) { delete ((ArrayV2dGeom*)g); }
Geom* g_array_v2d_elt(Geom* g, int idx) { return g_v2d(g_array_v2d_val(g)[idx]); }
int g_array_v2d_len(Geom* g) { return g_array_v2d_val(g).size(); }

Geom* g_circle_arc_fab(float x, float y, float q) {
  CircleArc data(x, y, q);
  auto res = g_circle_arc(data);
  // printf("%p = [%f, %f, %f]\n", res, x, y, z);
  return res;
}
void g_circle_arc_delete(Geom* g) { delete ((CircleArcGeom*)g); }
Geom* g_array_circle_arc_elt(Geom* g, int idx) { return g_circle_arc(g_array_circle_arc_val(g)[idx]); }
int g_array_circle_arc_len(Geom* g) { return g_array_circle_arc_val(g).size(); }
CircleArc g_circle_arc_val(Geom* g) {
  ensure(g->k == circle_arc_kind, "NOT CIRCLE_ARC");
  return ((CircleArcGeom*)g)->val;
}
float g_circle_arc_x(Geom* g) { return g_circle_arc_val(g).x[0]; }
float g_circle_arc_y(Geom* g) { return g_circle_arc_val(g).x[1]; }
float g_circle_arc_q(Geom* g) { return g_circle_arc_val(g).q; }
Array<CircleArc> g_array_circle_arc_val(Geom* g) {
  ensure(g->k == array_circle_arc_kind, "NOT ARRAY_CIRCLE_ARC");
  return ((ArrayCircleArcGeom*)g)->val;
}
Array<CircleArc> g_array_circle_arc(Array<TV2> args) {
  Array<CircleArc> v;
  for (auto arg : args) {
    CircleArc arc(arg.x, arg.y, 0.0);
    v.append(arc);
  }
  return v;
}
Array<CircleArc> g_array_circle_arc(RawArray<TV2> line) { return g_array_circle_arc(line.copy()); }

Geom* g_nested_circle_arc_fab(Geom* args) {
  Nested<CircleArc, false> v;
  for (auto arg : g_args_val(args)) 
    v.append(g_array_circle_arc_val(arg));
  v.freeze();
  return g_nested_circle_arc(v);
}
void g_nested_circle_arc_delete(Geom* g) { delete ((NestedCircleArcGeom*)g); }
Geom* g_nested_circle_arc(Nested<TV2> args) {
  Nested<CircleArc, false> v;
  for (auto arg : args) 
    v.append(g_array_circle_arc(arg));
  v.freeze();
  return new NestedCircleArcGeom(v);
}
bool is_nested_circle_arc(Geom* g) {
  return g->k == nested_circle_arc_kind;
}
bool is_nested_v2d(Geom* g) {
  return g->k == nested_v2d_kind || g->k == array_v2d_kind;
}
template<class ET>
Nested<ET> array_to_nested(Array<ET> contour) {
  Nested<ET,false> poly;
  poly.append(contour);
  poly.freeze();
  return poly;
}
Nested<TV2> g_nested_v2d_val(Geom* g) {
  ensure(is_nested_v2d(g), "NOT NESTED_V3D");
  if (g->k == array_v2d_kind)
    return array_to_nested(g_array_v2d_val(g));
  else
    return ((NestedV2dGeom*)g)->val;
}
void g_nested_v2d_delete(Geom* g) { delete g; }
Geom* g_nested_v2d_elt(Geom* g, int idx) { return g_array_v2d(g_nested_v2d_val(g)[idx]); }
int g_nested_v2d_len(Geom* g) { return g_nested_v2d_val(g).size(); }

Nested<CircleArc> g_nested_circle_arc_val(Geom* g) {
  ensure(is_nested_circle_arc(g), "NOT NESTED_CIRCLE_ARC");
  return ((NestedCircleArcGeom*)g)->val;
}

Geom* g_array_circle_arc_fab(Geom* args) {
  Array<CircleArc> v;
  for (auto arg : g_args_val(args)) 
    v.append(g_circle_arc_val(arg));
  return g_array_circle_arc(v);
}
void g_array_circle_arc_delete(Geom* g) { delete ((ArrayCircleArcGeom*)g); }
Geom* g_nested_circle_arc_elt(Geom* g, int idx) { return g_array_circle_arc(g_nested_circle_arc_val(g)[idx]); }
int g_nested_circle_arc_len(Geom* g) { return g_nested_circle_arc_val(g).size(); }

Geom* g_circle_arc(CircleArc v) { return new CircleArcGeom(v); }
Geom* g_array_circle_arc(Array<CircleArc> arcs) { return new ArrayCircleArcGeom(arcs); }
Geom* g_array_circle_arc(RawArray<CircleArc> arcs) { return g_array_circle_arc(arcs.copy()); }
Geom* g_nested_circle_arc(Nested<CircleArc> arcz) { return new NestedCircleArcGeom(arcz); }

Geom* g_arcs (std::vector<Geom*> args) {
  Nested< TV2,false > lines;
  if (all_args_kind(args, array_v2d_kind)) {
    for (auto arg : args)
      lines.append(g_array_v2d_val(arg));
  } else if (all_args_kind(args, v2d_kind)) {
    Array< TV2 > points;
    for (auto arg : args)
      points.append(g_v2d_val(arg));
    lines.append(points);
  } else {
    error("Bad Poly Args"); return NULL;
  }
  lines.freeze();
  return g_nested_circle_arc(lines);
}
Geom* g_nested_circle_arc_find_overlapping_offsets(float d, Geom* a) {
  return new NestedCircleArcGeom(find_overlapping_offsets(g_nested_circle_arc_val(a), d));
}
Geom* g_nested_circle_arc_intersection(Geom* a, Geom* b) {
  auto i = circle_arc_intersection(g_nested_circle_arc_val(a), g_nested_circle_arc_val(b));
  return new NestedCircleArcGeom(i);
}
Geom* g_nested_circle_arc_intersection1(Geom* a) {
  auto i = circle_arc_intersection(g_nested_circle_arc_val(a));
  return new NestedCircleArcGeom(i);
}
static Nested<CircleArc> circle_arc_concat_all(std::vector<Nested<CircleArc>> arcz) {
  Nested<CircleArc,false> res;
  for (auto a : arcz)
    for (auto e : a)
      res.append(e);
  res.freeze();
  return res;
}
Geom* g_nested_circle_arc_concat(Geom* a, Geom* b) {
  std::vector<Nested<CircleArc>> arcz;
  arcz.push_back(g_nested_circle_arc_val(a));
  arcz.push_back(g_nested_circle_arc_val(b));
  return new NestedCircleArcGeom(circle_arc_concat_all(arcz));
}
Geom* g_nested_circle_arc_concat_all(std::vector<Geom*> args) {
  if (all_args_kind(args, nested_circle_arc_kind)) {
    std::vector<Nested< CircleArc >> arcz;
    for (auto arg : args)
      arcz.push_back(g_nested_circle_arc_val(arg));
    return new NestedCircleArcGeom(circle_arc_concat_all(arcz));
  } else {
    error("Bad args for concat all"); return NULL;
  }
}
Geom* g_nested_circle_arc_concat_all(Geom* args) {
  if (args->k == args_kind) {
    return g_nested_circle_arc_concat_all(((ArgsGeom*)args)->val);
  } else {
    error("Bad args for concat all"); return NULL;
  }
}
static Nested<CircleArc> nested_circle_arc_not (Nested<CircleArc> ca) {
  Nested<CircleArc> res = ca.copy();
  reverse_arcs(res);
  return res;
}
Geom* g_nested_circle_arc_difference(Geom* a, Geom* b) {
  auto r = circle_arc_union(g_nested_circle_arc_val(a), nested_circle_arc_not(g_nested_circle_arc_val(b)));
  return new NestedCircleArcGeom(r);
}
Geom* g_nested_circle_arc_not(Geom* a) {
  auto r = nested_circle_arc_not(g_nested_circle_arc_val(a));
  return new NestedCircleArcGeom(r);
}
Geom* g_nested_circle_arc_union(Geom* a, Geom* b) {
  auto r = circle_arc_union(g_nested_circle_arc_val(a), g_nested_circle_arc_val(b));
  return new NestedCircleArcGeom(r);
}
Geom* g_nested_circle_arc_union1(Geom* a) {
  auto u = circle_arc_union(g_nested_circle_arc_val(a));
  return new NestedCircleArcGeom(u);
}
Geom* g_nested_circle_arc_open_offset(float r, Geom* g) {
  auto o = offset_open_arcs(g_nested_circle_arc_val(g), r);
  return new NestedCircleArcGeom(o);
}
Geom* g_nested_circle_arc_closed_offset(float r, Geom* g) {
  auto o = offset_arcs(g_nested_circle_arc_val(g), r);
  return new NestedCircleArcGeom(o);
}
Geom* g_nested_circle_arc_discretize(float max_deviation, int is_closed, Geom* g) {
  auto d = discretize_nested_arcs(g_nested_circle_arc_val(g), is_closed, max_deviation);
  return new NestedV2dGeom(d);
}
