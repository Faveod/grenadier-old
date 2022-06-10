#ifndef _RB_TREE_SITTER_MACROS_H
#define _RB_TREE_SITTER_MACROS_H

// Plain GETTER/SETTER/etc are for TypedData structs, reaching their top-level
// fields, and are of type VALUE
//
// DATA_* are for TypeData structs, raching their data field
// which can be of an arbitraty tuype.

#define GETTER(type, field)                                                    \
  static VALUE type##_get_##field(VALUE self) {                                \
    type##_t *ptr;                                                             \
    TypedData_Get_Struct(self, type##_t, &type##_data_type, ptr);              \
    return ptr->field;                                                         \
  }

#define SETTER(type, field)                                                    \
  static VALUE type##_set_##field(VALUE self, VALUE val) {                     \
    type##_t *ptr;                                                             \
    TypedData_Get_Struct(self, type##_t, &type##_data_type, ptr);              \
    ptr->field = val;                                                          \
    return Qnil;                                                               \
  }

#define ACCESSOR(type, field)                                                  \
  GETTER(type, field)                                                          \
  SETTER(type, field)

#define DATA_GETTER(type, field, cast)                                         \
  static VALUE type##_get_##field(VALUE self) {                                \
    type##_t *ptr;                                                             \
    TypedData_Get_Struct(self, type##_t, &type##_data_type, ptr);              \
    return cast(ptr->data.field);                                              \
  }

#define DATA_SETTER(type, field, cast)                                         \
  static VALUE type##_set_##field(VALUE self, VALUE val) {                     \
    type##_t *ptr;                                                             \
    TypedData_Get_Struct(self, type##_t, &type##_data_type, ptr);              \
    ptr->data.field = cast(val);                                               \
    return Qnil;                                                               \
  }

#define DATA_ACCESSOR(type, field, cast_get, cast_set)                         \
  DATA_GETTER(type, field, cast_get)                                           \
  DATA_SETTER(type, field, cast_set)

#define DATA_NEW(klass, struct, type)                                          \
  VALUE new_##type(const struct *ptr) {                                        \
    VALUE val = type##_allocate(klass);                                        \
    type##_t *obj;                                                             \
    TypedData_Get_Struct(val, type##_t, &type##_data_type, obj);               \
    obj->data = *ptr;                                                          \
    return val;                                                                \
  }                                                                            \
  VALUE new_##type##_by_val(struct ptr) {                                      \
    VALUE val = type##_allocate(klass);                                        \
    type##_t *obj;                                                             \
    TypedData_Get_Struct(val, type##_t, &type##_data_type, obj);               \
    obj->data = ptr;                                                           \
    return val;                                                                \
  }

#define DATA_DECLARE_DATA_TYPE(type)                                           \
  const rb_data_type_t type##_data_type = {                                    \
      .wrap_struct_name = #type "",                                            \
      .function =                                                              \
          {                                                                    \
              .dmark = NULL,                                                   \
              .dfree = type##_free,                                            \
              .dsize = type##_memsize,                                         \
              .dcompact = NULL,                                                \
          },                                                                   \
      .flags = RUBY_TYPED_FREE_IMMEDIATELY,                                    \
  };

#define DATA_FREE(type)                                                        \
  static void type##_free(void *ptr) { xfree(ptr); }

#define DATA_FROM_VALUE(struct, type)                                          \
  struct value_to_##type(VALUE self) {                                         \
    type##_t *obj;                                                             \
    TypedData_Get_Struct(self, type##_t, &type##_data_type, obj);              \
    return obj->data;                                                          \
  }

#define DATA_MEMSIZE(type)                                                     \
  static size_t type##_memsize(const void *ptr) {                              \
    type##_t *type = (type##_t *)ptr;                                          \
    return sizeof(type);                                                       \
  }

#define DATA_ALLOCATE(type)                                                    \
  static VALUE type##_allocate(VALUE klass) {                                  \
    type##_t *type;                                                            \
    return TypedData_Make_Struct(klass, type##_t, &type##_data_type, type);    \
  }

#define DATA_TYPE(klass, type)                                                 \
  typedef struct {                                                             \
    klass data;                                                                \
  } type##_t;

#define DATA_WRAP(klass, struct, type)                                         \
  DATA_TYPE(struct, type)                                                      \
  DATA_FREE(type)                                                              \
  DATA_MEMSIZE(type)                                                           \
  DATA_DECLARE_DATA_TYPE(type)                                                 \
  DATA_ALLOCATE(type)                                                          \
  DATA_NEW(klass, struct, type)                                                \
  DATA_FROM_VALUE(struct, type)

#define DEFINE_GETTER(klass, type, field)                                      \
  rb_define_method(klass, #field, type##_get_##field, 0);

#define DEFINE_SETTER(klass, type, field)                                      \
  rb_define_method(klass, #field "=", type##_set_##field, 1);

#define DEFINE_ACCESSOR(klass, type, field)                                    \
  DEFINE_GETTER(klass, type, field)                                            \
  DEFINE_SETTER(klass, type, field)

#endif