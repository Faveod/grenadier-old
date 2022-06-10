#include "tree_sitter.h"

extern VALUE mTreeSitter;

VALUE cPoint;

// This wrapper's raison d'etre is to avoid conversion and construction of Ruby
// VALUEs when accessing members.
typedef struct {
  TSPoint data;
} point_t;

DATA_FREE(point)
DATA_MEMSIZE(point)
DATA_DECLARE_DATA_TYPE(point)
DATA_ALLOCATE(point)
DATA_NEW(cPoint, TSPoint, point)
DATA_FROM_VALUE(TSPoint, point)
DATA_ACCESSOR(point, row, INT2NUM, NUM2INT)
DATA_ACCESSOR(point, column, INT2NUM, NUM2INT)

static VALUE point_inspect(VALUE self) {
  point_t *point;

  TypedData_Get_Struct(self, point_t, &point_data_type, point);

  return rb_sprintf("{row=%i, column=%i}", point->data.row, point->data.column);
}

void init_point(void) {
  cPoint = rb_define_class_under(mTreeSitter, "Point", rb_cObject);

  rb_define_alloc_func(cPoint, point_allocate);

  /* Class methods */
  DEFINE_ACCESSOR(cPoint, point, row)
  DEFINE_ACCESSOR(cPoint, point, column)
  rb_define_method(cPoint, "inspect", point_inspect, 0);
  rb_define_method(cPoint, "to_s", point_inspect, 0);
}
