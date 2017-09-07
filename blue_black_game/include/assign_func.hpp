#ifndef __ASSIGN_FUNC_HPP__
#define __ASSIGN_FUNC_HPP__

typedef void (*AssignFunc)(const char *, void *);

static void assign_int(const char *val, void *buffer);

static void assign_double(const char *val, void *buffer);

static void assign_str(const char *val, void *buffer);

static void assign_point(const char *val, void *buffer);

#endif
